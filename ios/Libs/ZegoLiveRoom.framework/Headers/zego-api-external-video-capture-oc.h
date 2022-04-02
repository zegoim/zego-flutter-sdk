//
//  zego-api-external-video-capture-oc.h
//  zegoavkit
//
//  Created by Randy Qiu on 2017/8/18.
//  Copyright © 2017年 Zego. All rights reserved.
//

#ifndef zego_api_external_video_capture_oc_h
#define zego_api_external_video_capture_oc_h

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#elif TARGET_OS_OSX
#import <AppKit/AppKit.h>
#endif

#import "zego-api-defines-oc.h"

/** 视频填充模式 */
typedef enum : NSUInteger {
    /** 等比缩放，可能有黑边 */
    ZegoVideoFillModeBlackBar,
    /** 裁剪 */
    ZegoVideoFillModeCrop,
    /** 拉伸 */
    ZegoVideoFillModeStretch,
} ZegoVideoFillMode;

/** 视频采集设备输出格式 */
typedef enum : NSUInteger {
    /** CVImageBufferRef */
    ZegoVideoCaptureDeviceOutputBufferTypeCVPixelBuffer,
    /** GLuint */
    ZegoVideoCaptureDeviceOutputBufferTypeGlTexture2D,
    /** EncodedFrame */
    ZegoVideoCaptureDeviceOutputBufferTypeEncodedFrame,
} ZegoVideoCaptureDeviceOutputBufferType;

/** 视频编码器格式 */
typedef enum : NSUInteger {
    /** AVC_AVCC 格式 */
    ZegoVideoCodecTypeAVCAVCC    = 0,
    /** AVC_ANNEXB 格式 */
    ZegoVideoCodecTypeAVCANNEXB  = 1,
} ZegoVideoCodecType;

/** 视频编码配置 */
typedef struct {
    /** 编码宽度 */
    int width;
    /** 编码高度 */
    int height;
    /** 编码格式 */
    ZegoVideoCodecType codecType;
    /** 逆时间旋转角度（注意：仅支持 udp 流，对 rtmp 流无效） */
    int rotation;
} ZegoVideoCodecConfig;

/** 翻转模式 */
typedef enum : NSUInteger {
    /** 不翻转 */
    ZegoVideoFlipNone = 0,
    /** 水平翻转，镜像 */
    ZegoVideoFlipHorizontal = 1 << 0,
    /** 垂直翻转 */
    ZegoVideoFlipVertical = 1 << 1,
} ZegoVideoFlipMode;


typedef struct {
    /** 编码码流 */
    int videoBitrate;
    /** 编码帧率 */
    int videoFrameRate;
    /** 编码宽度 */
    int width;
    /** 编码高度 */
    int height;
} ZegoTrafficControlQuality;

/** 视频外部采集代理 */
@protocol ZegoVideoCaptureDelegate <NSObject>

/**
 接收视频帧数据
 
 @param image 采集到的视频数据
 @param time 采集时间戳
 @discussion 设置成功视频外部采集对象，并启动采集后，在此通知中获取视频帧数据
 */
- (void)onIncomingCapturedData:(nonnull CVImageBufferRef)image withPresentationTimeStamp:(CMTime)time;

/**
 接收视频帧数据
 
 @param textureID texture ID, 可以通过 CVOpenGLESTextureGetName(texture) 取得
 @param width 帧宽
 @param height 帧高
 @param time 采集时间戳
 */
- (void)onIncomingCapturedData:(GLuint)textureID width:(int)width height:(int)height withPresentationTimeStamp:(CMTime)time;

/**
 接受已编码的视频帧数据
 
 @param data 已编码数据，目前只支持 H.264 码流
 @param config 编码配置，请参考 ZegoVideoCodecConfig 定义
 @param bKeyframe 是否为关键帧，建议关键帧间隔在 2S 左右
 @param time 采集到该帧的时间戳，用于音画同步，如果采集实现是摄像头，最好使用系统采集回调的原始时间戳。如果不是，最好是生成该帧的UTC时间戳
 @discussion 推荐每 2S 一个 gop，每个 I 帧必须携带 sps 和 pps，且放在最前面。
 @discussion 仅接受 I 帧 和 P 帧，不接受 B 帧
 */
- (void)onEncodedFrame:(nonnull NSData *)data config:(ZegoVideoCodecConfig)config bKeyframe:(bool)bKeyframe withPresentationTimeStamp:(CMTime)time;

@optional

/**
 
 @warning Deprecated
 */
- (void)onTakeSnapshot:(nonnull CGImageRef)image __attribute__ ((deprecated));

@end


/** 视频外部采集客户端代理 */
@protocol ZegoVideoCaptureClientDelegate <NSObject, ZegoVideoCaptureDelegate>

/**
 销毁
 
 @discussion 调用者需要在此 API 中进行相关的销毁操作
 */
- (void)destroy;

/**
 发生错误调用该接口，以通知远端
 ZEGOAPI_DEVICE_ERROR_REASON_DISABLED/ZEGOAPI_DEVICE_ERROR_REASON_MUTE是不能设置的，ZEGOAPI_DEVICE_ERROR_REASON_DISABLED通过enableCamera触发的，ZEGOAPI_DEVICE_ERROR_REASON_MUTE通过muteVideoPublish触发
 初始化外部采集后默认设备状态是好的，用户需要在外部采集通知StartCapture后补一个初始状态，如果前后原因都是属于设备关闭，那么引擎只会通知第一个原因
 
 @param is_closed 当前设备是否处于关闭状态
 @param reason 错误原因，参考 ZegoAPIDeviceErrorReason 定义
 */
- (void)setStatusReason:(bool)is_closed withReason:(ZegoAPIDeviceErrorReason)reason;


/**
 设置图像填充模式

 @param mode 填充模式，参考 ZegoVideoFillMode 定义
 @discussion supportBufferType 为 ZegoVideoCaptureDeviceOutputBufferTypeCVPixelBuffer 或 ZegoVideoCaptureDeviceOutputBufferTypeGlTexture2D 时有效
 */
- (void)setFillMode:(ZegoVideoFillMode)mode;

/**
 设置图像翻转模式

 @param mode 翻转模式, 参考 ZegoVideoFlipMode 的定义
 @discussion supportBufferType 为 ZegoVideoCaptureDeviceOutputBufferTypeGlTexture2D 时有效
 @discussion 默认值 ZegoVideoFlipVertical, 如果不需要，设置成 ZegoVideoFlipNone
 */
- (void)setFlipMode:(int)mode;

/**
  设置图像旋转角度

  @param rotation 顺时针旋转角度，取值范围：0 / 90 / 180 / 270
  @discussion supportBufferType 为 ZegoVideoCaptureDeviceOutputBufferTypeCVPixelBuffer 时有效
  @discussion 与 setConfig("vcap_external_handle_rotation=false") 联用时，可以将角度旋转放在拉流端处理。默认在推流端处理
  @discussion 注意此处设置的角度是顺时针方向旋转角度，而在拉流端处理时， 给的角度是按逆时针方向旋转的
 */
- (void)setRotation:(int)rotation;

@end


/** 视频外部采集接口 */
@protocol ZegoSupportsVideoCapture

@optional
/**
 设置视频采样帧率回调

 @param framerate 帧率
 @return 0 设置成功，其他值失败
 @discussion 调用 SDK 相关接口设置成功后，会通过此 API 通知调用者
 */
- (int)zego_setFrameRate:(int)framerate;

/**
 设置视频采集分辨率回调

 @param width 宽
 @param height 高
 @return 0 设置成功，其他值失败
 @discussion 调用 SDK 相关接口设置成功后，会通过此 API 通知调用者
 */
- (int)zego_setWidth:(int)width andHeight:(int)height;

/**
 切换前后摄像头回调

 @param bFront true 表示前摄像头，false 表示后摄像头
 @return 0 切换成功，其他值失败
 @discussion 调用 SDK 相关接口设置成功后，会通过此 API 通知调用者
 */
- (int)zego_setFrontCam:(int)bFront;

#if TARGET_OS_IPHONE
/**
 设置采集使用载体视图回调，移动端使用

 @param view 载体视图
 @return 0 设置成功，其他值失败
 @discussion 调用 SDK 相关接口设置成功后，会通过此 API 通知调用者
 */
- (int)zego_setView:(UIView* _Nullable )view;
#elif TARGET_OS_OSX

/**
 设置采集使用载体视图回调，PC 端使用

 @param view 载体视图
 @return 0 设置成功，其他值失败
 @discussion 调用 SDK 相关接口设置成功后，会通过此 API 通知调用者
 */
- (int)zego_setView:(NSView* _Nullable )view;
#endif

/**
 设置采集预览的模式回调

 @param mode 预览模式
 @return 0 设置成功，其他值失败
 @discussion 调用 SDK 相关接口设置成功后，会通过此 API 通知调用者
 */
- (int)zego_setViewMode:(int)mode;

/**
 设置采集预览的逆时针旋转角度回调

 @param rotation 旋转角度
 @return 0 设置成功，其他值失败
 @discussion 调用 SDK 相关接口设置成功后，会通过此 API 通知调用者
 */
- (int)zego_setViewRotation:(int)rotation;

/**
 设置采集 buffer 的顺时针旋转角度回调

 @param rotaion 旋转角度
 @return 0 设置成功，其他值失败
 @discussion 调用 SDK 相关接口设置成功后，会通过此 API 通知调用者
 */
- (int)zego_setCaptureRotation:(int)rotaion;

/**
 启动预览回调

 @return 0 设置成功，其他值失败
 @discussion 调用 SDK 相关接口设置成功后，会通过此 API 通知调用者
 */
- (int)zego_startPreview;

/**
 停止预览回调

 @return 0 设置成功，其他值失败
 @discussion 调用 SDK 相关接口设置成功后，会通过此 API 通知调用者
 */
- (int)zego_stopPreview;

/**
 开启手电筒回调

 @param enable true 开启，false 不开启
 @return 0 设置成功，其他值失败
 @discussion 调用 SDK 相关接口设置成功后，会通过此 API 通知调用者
 */
- (int)zego_enableTorch:(bool)enable;

/**
 对采集预览进行截图回调

 @return 0 截图成功，其他值失败
 @discussion 调用 SDK 相关接口设置成功后，会通过此 API 通知调用者
 */
- (int)zego_takeSnapshot;

/**
 
 @warning Deprecated
 */
- (int)zego_setPowerlineFreq:(unsigned int)freq __attribute__ ((deprecated));

@end


/** 视频外部采集设备接口 */
@protocol ZegoVideoCaptureDevice <NSObject, ZegoSupportsVideoCapture>

@required

/**
 初始化采集使用的资源（例如启动线程等）回调
 
 @param client SDK 实现回调的对象，一定要保存
 @discussion 第一次调用开始预览／推流／拉流时调用
 */
- (void)zego_allocateAndStart:(nonnull id<ZegoVideoCaptureClientDelegate>) client;

/**
 停止并且释放采集占用的资源
 
 @discussion 在此之后，不能再调用 client 对象的接口
 */
- (void)zego_stopAndDeAllocate;

/**
 启动采集，采集的数据通过 [client -onIncomingCapturedData:withPresentationTimeStamp:] 通知 SDK
 
 @return 0 表示成功，其他是错误
 @discussion 一定要实现，不要做丢帧逻辑，SDK内部已经包含了丢帧策略
 */
- (int)zego_startCapture;

/**
 停止采集
 
 @return 0 表示成功，其它是错误
 @discussion 一定要实现
 */
- (int)zego_stopCapture;


@optional
/**
 支持的 buffer 类型
 
 @return 支持的 buffer 类型
 @discussion 如果不实现，则为 ZegoVideoCaptureDeviceOutputBufferTypeCVPixelBuffer
 */
- (ZegoVideoCaptureDeviceOutputBufferType)zego_supportBufferType;

@end

/** 视频外部采集工厂接口 */
@protocol ZegoVideoCaptureFactory <NSObject>

@required

/**
 创建采集设备
 
 @param deviceId 设备 Id
 @return 采集设备实例
 @discussion 一定要实现
 @discussion 在 SDK 线程内执行，不要在其中直接调用 SDK 的接口，如有必要可切换线程后再调用
 */
- (nonnull id<ZegoVideoCaptureDevice>)zego_create:(nonnull NSString*)deviceId;

/**
 销毁采集设备
 
 @param device zego_create返回的采集设备实例
 @discussion 一定要实现
 @discussion 在 SDK 线程内执行，不要在其中直接调用 SDK 的接口，如有必要可切换线程后再调用
 */
- (void)zego_destroy:(nonnull id<ZegoVideoCaptureDevice>)device;

@end

/** 视频外部采集流控回调接口 */
@protocol ZegoTrafficControlCallback <NSObject>

@required

/**
 流控回调。
 
 @param deviceId 设备 Id
 @return 采集设备实例
 @discussion 当引擎认为需要改变当前推流质量时回调
 @discussion 一定要实现
 */
- (void)onTrafficControlCallback:(ZegoTrafficControlQuality)recommendQuality channelIndex:(ZegoAPIPublishChannelIndex)idx;

@end

@interface ZegoExternalVideoCapture : NSObject

/**
 设置外部采集工厂

 @param factory 外部采集工厂实例；当置空时，关闭外部采集功能。
 @param idx 推流通道
 @discussion 可以在任何时机调用
 @discussion 在 SDK 回调销毁后，才可以销毁对应的 factory
 */
+ (void)setVideoCaptureFactory:(nullable id<ZegoVideoCaptureFactory>)factory channelIndex:(ZegoAPIPublishChannelIndex)idx;

/**
 设置外部流控回调
 @param cb 流控回调实例
 @param idx 推流通道
 @discussion 必须在 推流 前调用；
 @discussion 在 推流 过程中不要改变该回调实例。
 @discussion 必须使用 EnableTrafficControl(int, boolean) 开启流控才会触发此回调；
 @discussion 仅在使用外部码流时有效
 */
+ (void)setTrafficControlCallback:(nullable id<ZegoTrafficControlCallback>)cb channelIndex:(ZegoAPIPublishChannelIndex)idx;

@end

#endif /* zego_api_external_video_capture_oc_h */
