//
//  zego-api-external-video-render-oc.h
//  zegoavkit
//
//  Copyright © 2017年 Zego. All rights reserved.
//

#ifndef zego_api_external_video_render_oc_h
#define zego_api_external_video_render_oc_h

#include <Foundation/Foundation.h>
#include <AVFoundation/AVFoundation.h>
#include "zego-api-defines-oc.h"

/**
 视频帧格式
 */
typedef NS_ENUM(NSInteger, VideoPixelFormat) {
    PixelFormatUnknown = 0,
    PixelFormatI420 = 1,
    PixelFormatNV12 = 2,
    PixelFormatNV21 = 3,
    PixelFormatBGRA32 = 4,
    PixelFormatRGBA32 = 5,
    PixelFormatARGB32 = 6,
    PixelFormatABGR32 = 7,
    PixelFormatI422 = 8
};

@protocol ZegoVideoRenderDelegate <NSObject>

/**
 SDK 待渲染视频数据

 @param data 待渲染数据, 当 VideoRenderType 设置为 VideoRenderTypeExternalInternalRgb 或者 VideoRenderTypeExternalInternalYuv 时，SDK 会使用修改后的 data 进行渲染
 @param dataLen 待渲染数据每个平面的数据大小，共 4 个面
 @param width 图像宽
 @param height 图像高
 @param strides 每个平面一行字节数，共 4 个面（RGBA 只需考虑 strides[0]）
 @param pixelFormat format type, 用于指定 data 的数据类型
 @param streamID 流名
 */
- (void)onVideoRenderCallback:(unsigned char **)data dataLen:(int*)dataLen width:(int)width height:(int)height strides:(int[])strides pixelFormat:(VideoPixelFormat)pixelFormat streamID:(NSString *)streamID;

/**
 SDK 通知下一帧数据是否需要翻转

 @param mode 翻转类型
 @param streamID 流名
 @discussion 仅本地预览的外部渲染会回调。此处的 mode 是基于推流图像计算出来的，和 SetVideoMirrorMode 不一定一致，请基于 SetFlipMode 的参数决定是否翻转
 */
- (void)onSetFlipMode:(int)mode streamID:(NSString *)streamID;

/**
 SDK 通知下一帧数据需要旋转的角度

 @param rotation 逆时针旋转角度
 @param streamID 流名
 */
- (void)onSetRotation:(int)rotation streamID:(NSString *)streamID;

@end

@protocol ZegoVideoRenderCVPixelBufferDelegate <NSObject>

/**
 SDK 待渲染视频数据

 @param data 待渲染的 CVPixelBuffer 类型数据
 @param streamID 流名
 */
- (void)onVideoRenderCallback:(CVPixelBufferRef)data streamID:(NSString *)streamID;

/**
 SDK 通知下一帧数据是否需要翻转

 @param mode 翻转类型
 @param streamID 流名
 @discussion 仅本地预览的外部渲染会回调。此处的 mode 是基于推流图像计算出来的，和 SetVideoMirrorMode 不一定一致，请基于 SetFlipMode 的参数决定是否翻转
 */
- (void)onSetFlipMode:(int)mode streamID:(NSString *)streamID;

/**
 SDK 通知下一帧数据需要旋转的角度

 @param rotation 逆时针旋转角度
 @param streamID 流名
 */
- (void)onSetRotation:(int)rotation streamID:(NSString *)streamID;

@end

/**
 视频外部渲染数据格式及渲染方式
 */
typedef NS_ENUM(NSInteger, VideoRenderType) {
    /** 默认值, 不开外部渲染 */
    VideoRenderTypeNone = 0,
    /** 外部渲染同时内部不渲染 */
    VideoRenderTypeRgb = 1,
    /** 外部渲染同时内部不渲染 */
    VideoRenderTypeYuv = 2,
    /** 由返回参数指定类型， 外部渲染同时内部不渲染*/
    VideoRenderTypeAny = 3,
    /** 外部渲染同时内部渲染 */
    VideoRenderTypeExternalInternalRgb = 4,
    /** 外部渲染同时内部渲染 */
    VideoRenderTypeExternalInternalYuv = 5
};

/**
 提供视频外部渲染功能
 */
@interface ZegoExternalVideoRender : NSObject


/**
 获取 ZegoExternalVideoRender 单例

 @return ZegoExternalVideoRender 单例对象
 */
+ (instancetype)sharedInstance;

/**
 开/关外部视频渲染(拉流)，拉流时 App 能拿到 SDK 抛出的视频数据。
 
 * 注意：
 * 1.只有当 +setVideoRenderType:type: 的 type 参数设置为非 VIDEO_RENDER_TYPE_NONE 时，该接口才有效。
 * 2.必须在拉流(startPlayingStream)之后调用才生效。
 
 @param enable YES 开启外部视频渲染， NO 关闭外部视频渲染，默认为不开启
 @param streamID 拉流的流 ID
 */
+ (bool)enableVideoRender:(BOOL)enable streamID:(NSString *)streamID;

/**
 开/关外部视频渲染(推流预览)，推流时 App 能拿到 SDK 抛出的视频数据。
 
 * 注意：
 * 1.只有当 +setVideoRenderType:type: 的 type 参数设置为非 VIDEO_RENDER_TYPE_NONE 时，该接口才有效。
 * 2.必须在 初始化 SDK 之后调用才生效。
 
 @param enable YES 开启外部视频渲染， NO 关闭外部视频渲染，默认为不开启
 @param channelIndex 推流通道，详见 enum ZegoAPIPublishChannelIndex，默认为主推流通道。
 */
+ (bool)enableVideoPreview:(BOOL)enable channelIndex:(ZegoAPIPublishChannelIndex)channelIndex;

/**
 设置开启外部渲染时要求 SDK 提供的数据格式及渲染方式

 @param type 指定 SDK 提供的数据格式及渲染方式
 @note 在启动推/拉流 及 预览 前设置有效
 */
+ (void)setVideoRenderType:(VideoRenderType)type;

/**
 设置外部渲染回调

 @param delegate 外部渲染回调代理，用于接收待渲染的视频数据
 */
- (void)setZegoVideoRenderDelegate:(id<ZegoVideoRenderDelegate>)delegate;

/**
 设置用于接收 CVPixelBuffer 类型数据的外部渲染回调

 @param delegate 外部渲染回调代理，用于接收待渲染的 CVPixelBuffer 类型视频数据
 */
- (void)setZegoVideoRenderCVPixelBufferDelegate:(id<ZegoVideoRenderCVPixelBufferDelegate>)delegate;

@end

#endif /* zego_api_external_video_render_oc_h */
