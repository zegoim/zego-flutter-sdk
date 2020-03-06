//
//  ZGVideoFilterAsyncDemo.m
//  LiveRoomPlayground-iOS
//
//  Created by Paaatrick on 2019/7/30.
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZGVideoFilterAsyncDemo.h"
#import "ZGPixelBufferUtils.h"

@interface ZGVideoFilterAsyncDemo ()

@property (atomic) int pendingCount;

@end

@implementation ZGVideoFilterAsyncDemo {
    id<ZegoVideoFilterClient> client_;      // SDK 创建的外部滤镜客户端，用于传递数据
    id<ZegoVideoBufferPool> buffer_pool_;
    
    dispatch_queue_t queue_;
    int width_;
    int height_;
    int stride_;
    CVPixelBufferPoolRef pool_;
    int buffer_count_;
}

#pragma mark -- ZegoVideoFilter Delgate

// 初始化外部滤镜使用的资源
- (void)zego_allocateAndStart:(id<ZegoVideoFilterClient>) client {
    client_ = client;
    if ([client_ conformsToProtocol:@protocol(ZegoVideoBufferPool)]) {
        buffer_pool_ = (id<ZegoVideoBufferPool>)client;
    }
    
    width_ = 0;
    height_ = 0;
    stride_ = 0;
    pool_ = nil;
    buffer_count_ = 4;
    self.pendingCount = 0;
    
    queue_ = dispatch_queue_create("video.filter", nil);
}

// 停止并释放外部滤镜占用的资源
- (void)zego_stopAndDeAllocate {
    if (queue_) {
        dispatch_sync(queue_, ^ {
        });
        queue_ = nil;
    }
    
    if (pool_) {
        [ZGPixelBufferUtils destroyPixelBufferPool:&pool_];
        pool_ = nil;
    }
    
    if (client_) {
        [client_ destroy];
        client_ = nil;
        buffer_pool_ = nil;
    }
}

- (ZegoVideoBufferType)supportBufferType {
    // * 返回滤镜的类型：此滤镜为异步滤镜
    return ZegoVideoBufferTypeAsyncPixelBuffer;
}

#pragma mark -- ZegoVideoBufferPool Delegate

// SDK 回调。从 App 获取 CVPixelBufferRef 对象，用于保存视频帧数据
- (CVPixelBufferRef)dequeueInputBuffer:(int)width height:(int)height stride:(int)stride {
    // * 按需创建 CVPixelBufferPool
    if (width_ != width || height_ != height || stride_ != stride) {
        if (pool_) {
            [ZGPixelBufferUtils destroyPixelBufferPool:&pool_];
        }
        
        if ([ZGPixelBufferUtils create32BGRAPixelBufferPool:&pool_ width:width height:height]) {
            width_ = width;
            height_ = height;
            stride_ = stride;
        } else {
            return nil;
        }
    }
    
    // * 如果处理不及时，未处理帧超过了 pool 的大小，则丢弃该帧
    if (self.pendingCount >= buffer_count_) {
        return nil;
    }
    
    CVPixelBufferRef pixel_buffer = nil;
    CVReturn ret = CVPixelBufferPoolCreatePixelBuffer(kCFAllocatorDefault, pool_, &pixel_buffer);
    
    if (ret != kCVReturnSuccess) {
        return nil;
    } else {
        self.pendingCount = self.pendingCount + 1;
        // * 返回一个可以用于存储采集到的图像的 CVPixelBuffer 实例
        return pixel_buffer;
    }
}

// SDK 回调。App 在此接口中获取 SDK 采集到的视频帧数据，并进行处理
- (void)queueInputBuffer:(CVPixelBufferRef)pixel_buffer timestamp:(unsigned long long)timestamp_100n {
    // * 采集到的图像数据通过这个传进来，这个点需要异步处理
    dispatch_async(queue_, ^ {
        int imageWidth = (int)CVPixelBufferGetWidth(pixel_buffer);
        int imageHeight = (int)CVPixelBufferGetHeight(pixel_buffer);
        int imageStride = (int)CVPixelBufferGetBytesPerRowOfPlane(pixel_buffer, 0);
        
        CVPixelBufferRef dst = [self->buffer_pool_ dequeueInputBuffer:imageWidth height:imageHeight stride:imageStride];
        
        if (dst) {
            
            // 示例 1️⃣：此处示例使用 FaceUnity 作为外部滤镜
//            CVPixelBufferRef output = [[FUManager shareManager] renderItemsToPixelBuffer:pixel_buffer];
//            BOOL result = [ZGPixelBufferUtils copyPixelBufferFrom:output to:dst];
            
            
            // 示例 2️⃣：此处示例手动反转画面颜色
            CVPixelBufferLockBaseAddress(pixel_buffer, 0);
            unsigned char *bgra32 = (unsigned char *)CVPixelBufferGetBaseAddress(pixel_buffer);
            for (int i = 0; i < (imageStride * imageHeight); i += 4) {
                unsigned char b = bgra32[i];
                unsigned char g = bgra32[i + 1];
                unsigned char r = bgra32[i + 2];
                bgra32[i] = 255 - b;
                bgra32[i + 1] = 255 - g;
                bgra32[i + 2] = 255 - r;
            }
            CVPixelBufferUnlockBaseAddress(pixel_buffer, 0);
            BOOL result = [ZGPixelBufferUtils copyRGBBuffer:bgra32 to:dst strides:imageStride width:imageWidth height:imageHeight];
            
            
            // 将处理完成的 Pixel Buffer 回传给 SDK
            if (result) {
                [self->buffer_pool_ queueInputBuffer:dst timestamp:timestamp_100n];
            }
        }
        
        self.pendingCount = self.pendingCount - 1;
        CVPixelBufferRelease(pixel_buffer);
    });
}

@end

