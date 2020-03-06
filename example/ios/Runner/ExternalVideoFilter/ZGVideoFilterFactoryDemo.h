//
//  ZGVideoFilterFactoryDemo.h
//  Runner
//
//  Created by Patrick Fu on 2020/3/5.
//  Copyright © 2020 The Chromium Authors. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ZegoLiveRoom/zego-api-external-video-filter-oc.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZGVideoFilterFactoryDemo : NSObject<ZegoVideoFilterFactory>

// 此处的 bufferType 对应四种滤镜类型，以创建不同的外部滤镜实例
@property (nonatomic, assign) ZegoVideoBufferType bufferType;
// TODO: Demo 暂时仅实现了 ZegoVideoBufferTypeAsyncPixelBuffer(BGRA32) 与 ZegoVideoBufferTypeSyncPixelBuffer(BGRA32)

@end

NS_ASSUME_NONNULL_END
