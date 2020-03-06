//
//  ZGVideoFilterFactoryDemo.m
//  Runner
//
//  Created by Patrick Fu on 2020/3/5.
//  Copyright © 2020 The Chromium Authors. All rights reserved.
//

#import "ZGVideoFilterFactoryDemo.h"
#import "ZGVideoFilterAsyncDemo.h"
#import "ZGVideoFilterSyncDemo.h"

@implementation ZGVideoFilterFactoryDemo {
    id<ZegoVideoFilter> g_filter_;
}

#pragma mark - ZegoVideoFilterFactory Delegate

// 创建外部滤镜实例
- (id<ZegoVideoFilter>)zego_create {
    if (g_filter_ == nil) {
        // 此处的 bufferType 对应四种滤镜类型，以创建不同的外部滤镜实例
        switch (self.bufferType) {
            case ZegoVideoBufferTypeAsyncPixelBuffer:
                g_filter_ = [[ZGVideoFilterAsyncDemo alloc] init];
                break;
            
            case ZegoVideoBufferTypeSyncPixelBuffer:
                g_filter_ = [[ZGVideoFilterSyncDemo alloc] init];
                break;
            
            // TODO: NV12/I420 async video filter
            default:
                break;
        }
    }
    return g_filter_;
}

// 销毁外部滤镜实例
- (void)zego_destroy:(id<ZegoVideoFilter>)filter {
    if (g_filter_ == filter) {
        g_filter_ = nil;
    }
}


@end
