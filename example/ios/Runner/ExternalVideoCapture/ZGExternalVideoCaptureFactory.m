//
//  ZGExternalVideoCaptureFactory.m
//  LiveRoomPlayGround
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZGExternalVideoCaptureFactory.h"

@interface ZGExternalVideoCaptureFactory ()

@property (nonatomic, weak) id<ZegoVideoCaptureDevice> mDevice;

@end

@implementation ZGExternalVideoCaptureFactory


- (instancetype)initWithDevice:(id<ZegoVideoCaptureDevice>)device {
    if (self = [super init]) {
        _mDevice = device;
    }
    return self;
}

#pragma mark - ZegoVideoCaptureFactory

- (nonnull id<ZegoVideoCaptureDevice>)zego_create:(nonnull NSString*)deviceId {
    NSLog(@"%s.%@", __func__, self);
    return self.mDevice;
}

- (void)zego_destroy:(nonnull id<ZegoVideoCaptureDevice>)device {
    NSLog(@"%s. %@", __func__, self);
    self.mDevice = nil;
}
@end
