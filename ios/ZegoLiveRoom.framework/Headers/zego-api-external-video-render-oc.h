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
 @warning Deprecated 请使用 ZegoVideoRenderDelegate

 视频外部渲染视频数据及其格式的回调
 */
@protocol ZegoExternalVideoRenderDelegate <NSObject>

@optional
/**
 SDK 从 App 端获取 CVPixelBufferRef 地址
 
 * 已调用 +enableExternalVideoRender:type: 开启外部渲染，并调用 -setExternalVideoRenderDelegate: 成功设置外部渲染代理对象后，SDK 会触发此回调，App 端在此 API 中获取视频数据的宽高后分配能够保存此视频数据的 CVPixelBufferRef; SDK 通过此 API 获取到 App 指定的 CVPixelBufferRef 后，将采集的视频源数据拷贝进去。
 
 @param width 视频宽度，SDK 给出当前采集的图像数据的宽
 @param height 视频高度，SDK 给出当前采集的图像数据的高
 @param cvPixelFormatType SDK 给出的视频数据格式类型 ，用来创建 CVPixelBufferRef 对象
 @return CVPixelBufferRef 对象
 */
- (CVPixelBufferRef)onCreateInputBufferWithWidth:(int)width height:(int)height cvPixelFormatType:(OSType)cvPixelFormatType;

/**
 SDK 从 App 端获取 CVPixelBufferRef 地址
 
 * 已调用 +enableExternalVideoRender:type: 开启外部渲染，并调用 -setExternalVideoRenderDelegate: 成功设置外部渲染代理对象后，SDK 会触发此回调，App 端在此 API 中获取视频数据的宽高后分配能够保存此视频数据的 CVPixelBufferRef; SDK 通过此 API 获取到 App 指定的 CVPixelBufferRef 后，将采集的视频源数据拷贝进去。
 
 @param width 视频宽度，SDK 给出当前采集的图像数据的宽
 @param height 视频高度，SDK 给出当前采集的图像数据的高
 @param cvPixelFormatType SDK 给出的视频数据格式类型，用来创建CVPixelBufferRef对象
 @param streamID 当前正在推流/拉流的流名
 @return CVPixelBufferRef 对象
 */
- (CVPixelBufferRef)onCreateInputBufferWithWidth:(int)width height:(int)height cvPixelFormatType:(OSType)cvPixelFormatType streamID:(NSString *)streamID;

/**
 SDK 通知 App 端已将视频数据拷贝到其指定的 CVPixelBufferRef
 
 * SDK 通过此回调通知 App 端已将视频数据拷贝到其指定的 CVPixelBufferRef 中。当对拉流数据进行外部渲染时，streamID 为拉流流名。当对推流数据进行外部渲染时，streamID 为常量 kZegoVideoDataMainPublishingStream 表示是第一路推流数据；streamID 为常量 kZegoVideoDataAuxPublishingStream 表示是第二路推流数据；一般是为了在同时推两路流并使用外部渲染时对流进行区分。
 
 @param pixelBuffer 已填充视频数据的 CVPixelBufferRef 地址
 @param streamID 推流/拉流的流名
 */
- (void)onPixelBufferCopyed:(CVPixelBufferRef)pixelBuffer streamID:(NSString *)streamID;

/**
 SDK 通知下一帧数据是否需要翻转

 @param mode 翻转类型
 @param streamID 流名
 @discussion 仅本地预览的外部渲染会回调。此处的 mode 是基于推流图像计算出来的，和 SetVideoMirrorMode 不一定一致，请基于 SetFlipMode 的参数决定是否翻转
 */
- (void)onSetFlipMode:(int)mode streamID:(NSString *)streamID;

@end

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
 @streamID 流名
 */
- (void)onVideoRenderCallback:(unsigned char **)data dataLen:(int*)dataLen width:(int)width height:(int)height strides:(int[])strides pixelFormat:(VideoPixelFormat)pixelFormat streamID:(const char *)streamID;

/**
 SDK 通知下一帧数据是否需要翻转

 @param mode 翻转类型
 @param streamID 流名
 @discussion 仅本地预览的外部渲染会回调。此处的 mode 是基于推流图像计算出来的，和 SetVideoMirrorMode 不一定一致，请基于 SetFlipMode 的参数决定是否翻转
 */
- (void)onSetFlipMode:(int)mode streamID:(NSString *)streamID;

@end

/**
 视频外部渲染类型

 @warning Deprecated
 
 - VideoExternalRenderTypeDecodeRgbSeries: 外部视频渲染回调抛解码后的数据(kCVPixelFormatType_32BGRA)，SDK 内部不会渲染推流/拉流视频数据，即推流时即使设置了预览视图（setPreviewView）并启动本地预览（startPreview）也无法显示视频画面，拉流时设置了播放视图也无法显示视频画面，需要 App 实现视频的渲染。
 - VideoExternalRenderTypeDecodeRender: 外部视频渲染回调抛解码后数据(kCVPixelFormatType_32BGRA)，SDK 内部会对推流/拉流视频数据进行渲染，即 App 设置视图后能正常显示视频画面，但是此类型默认没有开启外部渲染，App 若想拿到视频数据需要通过 + enableVideoRender:streamID: 、+ enableVideoPreview:channelIndex: 开启 SDK 的外部渲染。
 */
typedef NS_ENUM(NSInteger, VideoExternalRenderType) {
    
    VideoExternalRenderTypeDecodeRgbSeries = 0,
    VideoExternalRenderTypeDecodeRender = 3
};

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
 设置外部渲染代理对象

 * 注意：
 * 1.未设置代理，不会收到视频数据的回调。
 * 2.推荐使用单例对象设置外部渲染代理，保证回调是全局唯一的。
 
 @warning Deprecated 请使用 setZegoVideoRenderDelegate

 @param delegate 外部渲染代理，详见 delegate ZegoExternalVideoRenderDelegate。
 */
- (void)setExternalVideoRenderDelegate:(id<ZegoExternalVideoRenderDelegate>)delegate;


/**
 设置是否开启外部渲染

 * 当出现 SDK 的渲染无法满足需求时，或者 App 需要获取 SDK 采集或拉流的视频数据进行特殊处理时，又或者 App 使用了跨平台界面框架（例如 QT 需要有复杂层级关系的界面以实现高体验的交互）或游戏引擎（例如 Unity3D、Cocos2d-x）等情况时建议使用 SDK 的自定义视频渲染功能。
 * 注意：
 * 1.此 API 必须在初始化 SDK 之前调用。
 * 2.开启外部渲染后，需要设置外部渲染代理(-setExternalVideoRenderDelegate:)才能收到视频数据再做相应的渲染处理。
 
 @warning Deprecated 请使用 setVideoRenderType
 
 @param enable YES 表示开启外部渲染，NO 表示关闭外部渲染
 @param type 视频外部渲染类型，详见 enum VideoExternalRenderType。
 */
+ (void)enableExternalVideoRender:(BOOL)enable type:(VideoExternalRenderType)type;

/**
 开/关外部视频渲染(拉流)，拉流时 App 能拿到 SDK 抛出的视频数据。
 
 * 注意：
 * 1.只有当 +enableExternalVideoRender:type:  的 type 参数设置为 VideoExternalRenderTypeDecodeRender，该接口才有效。
 * 2.必须在拉流(startPlayingStream)之后调用才生效。
 
 @param enable YES 开启外部视频渲染， NO 关闭外部视频渲染，默认为不开启
 @param streamID 拉流的流 ID
 */
+ (bool)enableVideoRender:(BOOL)enable streamID:(NSString *)streamID;

/**
 开/关外部视频渲染(推流预览)，推流时 App 能拿到 SDK 抛出的视频数据。
 
 * 注意：
 * 1.只有当 +enableExternalVideoRender:type:  的 type 参数设置为 VideoExternalRenderTypeDecodeRender，该接口才有效。
 * 2.必须在 初始化 SDK 之后调用才生效。
 
 @param enable YES 开启外部视频渲染， NO 关闭外部视频渲染，默认为不开启
 @param channelIndex 推流通道，详见 enum ZegoAPIPublishChannelIndex，默认为主推流通道。
 */
+ (bool)enableVideoPreview:(BOOL)enable channelIndex:(ZegoAPIPublishChannelIndex)channelIndex;

/**
 设置开启外部渲染时要求 SDK 提供的数据格式及渲染方式

 @param type 指定 SDK 提供的数据格式及渲染方式
 @note 在初始化 SDK 之前调用有效
 */
+ (void)setVideoRenderType:(VideoRenderType)type;

/**
 设置外部渲染回调

 @param delegate 外部渲染回调代理，用于接收待渲染的视频数据
 */
- (void)setZegoVideoRenderDelegate:(id<ZegoVideoRenderDelegate>)delegate;

@end

#endif /* zego_api_external_video_render_oc_h */
