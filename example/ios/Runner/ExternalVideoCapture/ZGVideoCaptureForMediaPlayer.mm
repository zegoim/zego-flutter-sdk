//
//  ZGVideoCaptureForMediaPlayer.m
//  LiveRoomPlayground
//
//  Copyright © 2018年 Zego. All rights reserved.
//

#import "ZGVideoCaptureForMediaPlayer.h"
#import <sys/time.h>
#import <memory>
#import <thread>

@interface ZGVideoCaptureForMediaPlayer () {
    id<ZegoVideoCaptureClientDelegate> client_;
    BOOL capture_started_;
    std::mutex capture_lock_;
    
    CVPixelBufferPoolRef pool_;
    int video_width_;
    int video_height_;
}
@end

@implementation ZGVideoCaptureForMediaPlayer

#pragma mark - ZegoVideoCaptureDevice

- (void)zego_allocateAndStart:(id<ZegoVideoCaptureClientDelegate>)client {
    NSLog(@"%s", __func__);
    client_ = client;
}

- (void)zego_stopAndDeAllocate {
    NSLog(@"%s", __func__);
    [client_ destroy];
    client_ = nil;
}

- (int)zego_startCapture {
    NSLog(@"%s", __func__);
    std::lock_guard<std::mutex> lg(capture_lock_);
    capture_started_ = YES;
    return 0;
}

- (int)zego_stopCapture {
    NSLog(@"%s", __func__);
    std::lock_guard<std::mutex> lg(capture_lock_);
    capture_started_ = NO;
    return 0;
}

- (ZegoVideoCaptureDeviceOutputBufferType)zego_supportBufferType {
    NSLog(@"%s", __func__);
    return ZegoVideoCaptureDeviceOutputBufferTypeCVPixelBuffer;
}

#pragma mark - ZegoMediaPlayerControllerVideoDataDelegate
- (void)onPlayerVideoFrame:(CVPixelBufferRef)buffer timeStamp:(unsigned long long)timeStamp {
    [self handleFrame:buffer time:timeStamp];
}

- (void)handleFrame:(CVPixelBufferRef)frame time:(unsigned long long)timeMS {
    CMTime pts = CMTimeMake(timeMS, 1000);
    std::lock_guard<std::mutex> lg(capture_lock_);
    if (capture_started_) {
        [client_ onIncomingCapturedData:frame withPresentationTimeStamp:pts];
    }
}

@end

