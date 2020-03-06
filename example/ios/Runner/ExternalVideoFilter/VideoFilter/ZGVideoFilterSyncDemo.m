//
//  ZGVideoFilterSyncDemo.m
//  LiveRoomPlayground-iOS
//
//  Created by Paaatrick on 2019/7/30.
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZGVideoFilterSyncDemo.h"
#import "ZGPixelBufferUtils.h"

@implementation ZGVideoFilterSyncDemo {
    id<ZegoVideoFilterClient> client_;
    id<ZegoVideoFilterDelegate> delegate_;
}

#pragma mark -- ZegoVideoFilter Delgate

// 初始化外部滤镜使用的资源
- (void)zego_allocateAndStart:(id<ZegoVideoFilterClient>) client {
    client_ = client;
    if ([client_ conformsToProtocol:@protocol(ZegoVideoFilterDelegate)]) {
        delegate_ = (id<ZegoVideoFilterDelegate>)client;
    }
}

// 停止并释放外部滤镜占用的资源
- (void)zego_stopAndDeAllocate {
    [client_ destroy];
    client_ = nil;
    delegate_ = nil;
}

- (ZegoVideoBufferType)supportBufferType {
    // * 返回滤镜的类型：此滤镜为同步滤镜
    return ZegoVideoBufferTypeSyncPixelBuffer;
}

#pragma mark -- ZegoVideoFilterDelegate Delegate

- (void)onProcess:(CVPixelBufferRef)pixel_buffer withTimeStatmp:(unsigned long long)timestamp_100 {
    // * 采集到的图像数据通过这个传进来，同步处理完返回处理结果
    
    
    
    // 示例 1️⃣：此处示例使用 FaceUnity 作为外部滤镜（本工程未导入 FaceUnity 相关文件）
//    CVPixelBufferRef output = [[FUManager shareManager] renderItemsToPixelBuffer:pixel_buffer];
//    [delegate_ onProcess:output withTimeStatmp:timestamp_100];
    
    
    
    // 示例 2️⃣：此处示例手动反转画面颜色
    int imageHeight = (int)CVPixelBufferGetHeight(pixel_buffer);
    int imageStride = (int)CVPixelBufferGetBytesPerRowOfPlane(pixel_buffer, 0);
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
    
    [delegate_ onProcess:pixel_buffer withTimeStatmp:timestamp_100];
}

@end

