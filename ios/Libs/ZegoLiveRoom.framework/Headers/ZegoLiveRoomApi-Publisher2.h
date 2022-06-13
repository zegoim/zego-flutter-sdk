//
//  ZegoLiveRoomApi-Publisher.h
//  ZegoLiveRoom
//
//  Copyright © 2017年 zego. All rights reserved.
//


#import "ZegoLiveRoomApi.h"
#import "ZegoLiveRoomApiDefines-Publisher.h"
#import <AVFoundation/AVFoundation.h>

@interface ZegoLiveRoomApi (DuoPublisher)

typedef void(^ZegoUpdatePublishTargetCompletionBlock)(int errorCode, NSString *streamID);

/**
 设置本地预览视图
 
 * 注意：
 * 1.建议在调用 -startPreview: 开启预览前，调用该 API 设置本地预览视图，才能在视图上预览采集的视频。
 * 2.建议本地预览结束后，调用该 API 设置预览视图为 nil。
 
 @param view 用于渲染本地预览视频的视图
 @param index 推流 channel Index
 @return 设置结果。true 成功，false 失败
 */
- (bool)setPreviewView:(ZEGOView *)view channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置预览控件的背景颜色
 @param color 颜色,取值为0x00RRGGBB
 @param index 推流 channel Index
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)setPreviewViewBackgroundColor:(int)color channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 启动本地预览
 
 * 注意：
 * 1.建议在启动本地预览前，调用 -setPreviewView:channelIndex: 设置本地预览视图。
 * 2.在退出房间后，SDK 内部会停止预览，如果需要继续预览，需要重新调用本方法开启预览。
 
 @param index 推流 channel Index
 @return true 成功，false 失败
 */
- (bool)startPreview:(ZegoAPIPublishChannelIndex)index;

/**
 停止本地预览
 
 @param index 推流 channel Index
 @return true 成功，false 失败
 */
- (bool)stopPreview:(ZegoAPIPublishChannelIndex)index;

/**
 设置推流加密密钥
 
 @param key 加密密钥（支持16/24/32字节）
 @param channelIndex 推流使用的推流通道
 */
- (void)setPublishEncryptKey:(NSData *)key channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置采集视频源类型
 
 注意：
 1. 在推流前调用，推流过程中不能更改此值，否则可能会出现不可预知问题;
 2. 当 srcType 为 ZEGOAPI_VIDEO_SRC_EXTERNAL_CAPTURE 但对应通道没有外部采集工厂时，将自动修正为 ZEGOAPI_VIDEO_SRC_CAMERA;
 3. 当 srcType 设置为 ZEGOAPI_VIDEO_SRC_NONE 时，表示不启动视频采集，即没有视频数据;
 4. 当 srcType 设置为 ZEGOAPI_VIDEO_SRC_DEFAULT 时，如果有设置外部视频采集工厂，则使用外部采集，否则使用内部采集;
 5. 当 srcType 设置为 ZEGOAPI_VIDEO_SRC_CAMERA 时，即使设置了外部视频采集工厂，也会使用摄像头作为视频数据源;
 6. 当 index 为 ZEGOAPI_CHN_MAIN 时，srcType 不能设置为 ZEGOAPI_VIDEO_SRC_MAIN_PUBLISH_CHN;
 7. 一旦使用过此 API 设置采集视频源类型，在停止推流时，SDK 不会重置视频采集源类型;
 
 @param srcType 源类型
 @param index 推流 channel Index
 @return true 设置成功，false 设置失败
 */
- (bool)setVideoSource:(ZegoVideoSourceType)srcType channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 获取 SDK 支持的最大同时推流数
 
 @return 最大支持推流数
 */
+ (int)getMaxPublishChannelCount;

/**
 开始指定通道发布直播（推流）
 
 * 注意：
 * 1.可以登录房间成功后调用，发布直播后可以使用 [ZegoLiveRoomApi(Publisher2) -stopPublishing:] 停止发布直播。
 * 2.发布直播 API 调用成功后，SDK 会在 [ZegoLivePublisherDelegate -onPublishStateUpdate:streamID:streamInfo:] 回调中通知发布结果，同一房间的其他人会在 [ZegoLiveRoomDelegate -onStreamUpdated:streams:roomID:] 回调中收到流新增通知。
 * 3.如果有收到 [ZegoRoomDelegate -onDisconnect:roomID:] 回调，则不会再收到 [ZegoLivePublisherDelegate -onPublishStateUpdate:streamID:streamInfo:] 回调。
 * 4.为了满足客户的秒开播需求，该接口可以在调用登录房间接口后同步调用，如果登录房间失败，则直播发布也失败。
 * 5.[ZegoLiveRoonApi(Publisher) -startPublishing] 既是主通道推流。
 
 @param streamID 发布直播流的流ID，仅支持长度不超过 256 字节的数字、下划线、字母，streamID 需要在整个 AppID 内全局唯一
 @param title 直播名称，可空，默认为主播用户名
 @param flag 发布直播的模式
 @param channelIndex 推流使用的推流通道
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)startPublishing2:(NSString *)streamID title:(NSString *)title flag:(int)flag channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 开始发布直播（指定通道推流）
 
 * 注意：
 * 1.可以登录房间成功后调用，发布直播后可以使用 [ZegoLiveRoomApi(Publisher2) -stopPublishing:] 停止发布直播。
 * 2.发布直播 API 调用成功后，SDK 会在 [ZegoLivePublisherDelegate -onPublishStateUpdate:streamID:streamInfo:] 回调中通知发布结果，同一房间的其他人会在 [ZegoLiveRoomDelegate -onStreamUpdated:streams:roomID:] 回调中收到流新增通知。
 * 3.如果有收到 [ZegoRoomDelegate -onDisconnect:roomID:] 回调，则不会再收到 [ZegoLivePublisherDelegate -onPublishStateUpdate:streamID:streamInfo:] 回调。
 * 4.为了满足客户的秒开播需求，该接口可以在调用登录房间接口后同步调用，如果登录房间失败，则直播发布也失败。
 * 5.[ZegoLiveRoonApi(Publisher) -startPublishing] 既是主通道推流。
 
 @param streamID 发布直播流的流ID，仅支持长度不超过 256 字节的数字、下划线、字母，streamID 需要在整个 AppID 内全局唯一
 @param title 直播名称，可空，默认为主播用户名
 @param flag 发布直播的模式，参考 ZegoApiPublishFlag 定义
 @param extraInfo 流附加信息，可空，传空表示附加信息为空字符串，仅支持长度不超过 1024 字节的字符串
 @param channelIndex 推流使用的推流通道
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)startPublishing2:(NSString *)streamID title:(NSString *)title flag:(int)flag extraInfo:(NSString *)extraInfo channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 开始发布直播
 
 @param streamID 流 ID
 @param title 直播名称，可选，默认为主播用户名
 @param flag 直播属性，参考 ZegoApiPublishFlag 定义
 @param extraInfo 流附加信息, 最大为 1024 字节
 @param params 推流参数
 @param index 推流 channel Index
 @return true 成功，false 失败
 @discussion 发布直播成功后，等待 [ZegoLivePublisherDelegate -onPublishStateUpdate:streamID:streamInfo:] 通知
 */
- (bool)startPublishing2:(NSString *)streamID title:(NSString *)title flag:(int)flag extraInfo:(NSString *)extraInfo params:(NSString *)params channelIndex:(ZegoAPIPublishChannelIndex)index;


/**
 开始发布直播
 
 @param params 推流参数, 详见 ZegoAPIPublishStreamParams
 @return true 成功，false 失败
 @discussion 发布直播成功后，等待 [ZegoLivePublisherDelegate -onPublishStateUpdate:streamID:streamInfo:] 通知
 */
- (bool)startPublishWithParams:(ZegoAPIPublishStreamParams *)params;


/**
 推流时是否发送视频数据。

 @param mute true 不发送(仅预览)，false 发送
 @param channelIndex 推流通道索引
 @return 0 代表设置成功成功，否则设置失败
 @attention 拉流端通过 OnRemoteCameraStatusUpdate 回调监听此状态是否改变;
 @attention 仅拉 UDP 流时，才能接收到状态变更通知;
 */
- (int)muteVideoPublish:(bool)mute channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 推流时是否发送音频数据。
 
 @param mute true 不发送，false 发送
 @param channelIndex 推流通道索引
 @return 0 代表设置成功成功，否则设置失败
 @attention 可以通过 ZegoLiveApiAudioRecordDelegate 回调获取本地音频数据
 */
- (int)muteAudioPublish:(bool)mute channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 更新流附加信息
 
 @param extraInfo 流附加信息, 最大为 1024 字节
 @param index 推流 channel Index
 @return true 成功，false 失败
 @discussion 通常在主播方的 [ZegoLivePublisherDelegate -onPublishStateUpdate:streamID:streamInfo:] 通知中，或其他需更新流附加信息的场合下调用。更新流附加信息成功后，除调用方外，同一房间内的其他人会收到 [ZegoLiveRoomDelegate -onStreamExtraInfoUpdated:roomID] 通知
 */
- (bool)updateStreamExtraInfo:(NSString *)extraInfo channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 停止指定通道发布直播（推流）
 
 * 注意：
 * 1.用于停止已发布的直播，可以在发布直播后调用。
 * 2.停止发布直播不会触发 [ZegoLivePublisherDelegate -onPublishStateUpdate:streamID:streamInfo:] 回调。
 
 @param index 推流使用的推流通道
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)stopPublishing:(ZegoAPIPublishChannelIndex)index;

/**
 自定义推流配置
 
 @param config 配置信息 key-value，直推 CDN 时, 使用kPublishCDNTarget, value 为用户指定的直推 CDN 地址. 连麦推流时, 使用kPublishCustomTarget, value 为用户指定的转推 RTMP 地址，非 RTMP 地址可能导致转推失败。参考 zego-api-defines-oc.h 中相关定义
 @param index 推流 channel Index
 @discussion 开发者如果使用自定义转推功能，推流开始前，必须调用此接口设置转推 RTMP 地址（SDK 推流方式必须为 UDP，转推地址必须为 RTMP），否则可能导致转推失败。
 */
- (void)setPublishConfig:(NSDictionary *)config channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置自定义直推CDN的配置，可支持TCP, QUIC协议
 
 @param target 自定义直推 CDN 地址的配置信息，详见 [ZegoAPICDNPublishTarget]
 @param index 推流 channel Index
 @return true 调用成功 false 调用失败
 */
- (bool)setCustomCDNPublishTarget:(ZegoAPICDNPublishTarget *)target channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 添加已发布直播的转推地址

 @param target 添加的转推地址（支持rtmp/avertp）
 @param streamID 正在发布的直播流ID
 @param completionBlock 完成回调
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)addPublishTarget:(NSString *)target streamID:(NSString *)streamID completion:(ZegoUpdatePublishTargetCompletionBlock)completionBlock;

/**
 删除转推地址
 
 @param strTarget 转推地址（支持rtmp/avertp）
 @param pszStreamID 推流ID
 @param completionBlock 删除转推地址回调
 @return true 成功，false 失败
 @attention 在InitSDK之后调用
 */
- (bool)deletePublishTarget:(NSString *)target streamID:(NSString *)streamID completion:(ZegoUpdatePublishTargetCompletionBlock)completionBlock;

/**
 设置视频配置
 
 @param config 配置参数（视频编码输出分辨率、视频采集分辨率、视频帧率、视频码率），参考 ZegoAVConfig 定义
 @param index 推流 channel Index
 @return true 成功，false 失败
 @discussion 推流开始前调用本 API 进行视频采集参数配置
 */
- (bool)setAVConfig:(ZegoAVConfig *)config channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置视频关键帧间隔
 
 * 注意：推流前调用有效。
 
 @param intervalSecond 关键帧间隔，单位为秒，默认 2 秒
 @param index 推流使用的推流通道
 @return true 表示调用成功，false 表示调用失败
 */
- (bool)setVideoKeyFrameInterval:(int)intervalSecond channelIndex:(ZegoAPIPublishChannelIndex)index;

#if TARGET_OS_IPHONE
/**
 设置手机方向
 
 * 本设置用于校正输出视频朝向，默认竖屏。建议在预览和推流之前设置好方向。
 * 不建议在推流过程中旋转画面，原因：
 * 1. 如果刚开始为竖屏开播，此时旋转为横屏，但是分辨率仍然是竖屏的，会导致横屏后，画面放大的效果(按照等比填充的模式) 或者 上下黑边的效果(按照等比缩放的模式)，除非旋转为横屏后，画面的分辨率也做修改。
 * 2. 直播过程中修改分辨率的话，会导致录制文件(回放)有异常，因此不建议中途旋转画面。
 
 @param orientation 手机方向
 @param index 推流 channel Index
 @return true 成功，false 失败
 @see -startPreview:
 @see -startPublishing2:title:flag:channelIndex:
 */
- (bool)setAppOrientation:(UIInterfaceOrientation)orientation channelIndex:(ZegoAPIPublishChannelIndex)index;



/**
 设置预览的裁剪视图配置
 
 @attention 推流前或者推流过程中设置都有效，当次推流有效。
 @param rect 裁剪坐标
 @param originalSize 原始图像尺寸
 @param idx 推流通道号，参见 ZegoAPIPublishChannelIndex
 */
- (void)setPreviewCropRect:(CGRect)rect originalSize:(CGSize)originalSize publishChannel:(ZegoAPIPublishChannelIndex)index;
#endif //TARGET_OS_IPHONE

/**
 主播方开启美颜功能
 
 @param feature 美颜特性，参考 ZegoBeautifyFeature 定义。默认无美颜
 @param index 推流 channel Index
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置
 */
- (bool)enableBeautifying:(int)feature channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置美颜磨皮的采样步长
 
 @param step 采样步长，取值范围[1,16]。默认 4.0
 @param index 推流 channel Index
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置。设置时需确保对应美颜特性开启
 */
- (bool)setPolishStep:(float)step channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置美颜采样颜色阈值
 
 @param factor 采样颜色阈值，取值范围[0,16]。默认 4.0
 @param index 推流 channel Index
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置。设置时需确保对应美颜特性开启
 
 */
- (bool)setPolishFactor:(float)factor channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置美颜美白的亮度修正参数
 
 @param factor 亮度修正参数，取值范围[0,1]，值越大亮度越暗。默认 0.5
 @param index 推流 channel Index
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置。设置时需确保对应美颜特性开启
 */
- (bool)setWhitenFactor:(float)factor channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置锐化参数
 
 @param factor 锐化参数，取值范围[0,2]，值越大锐化越强。默认 0.2
 @param index 推流 channel Index
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置。设置时需确保对应美颜特性开启
 */
- (bool)setSharpenFactor:(float)factor channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置滤镜
 
 @param filter 滤镜种类，参考 ZegoFilter 定义。默认不使用滤镜
 @param index 推流 channel Index
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置
 */
- (bool)setFilter:(ZegoFilter)filter channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置本地预览视图的模式
 
 * 视图模式默认为 ZegoVideoViewModeScaleAspectFill。
 
 @param mode 视图模式。详见 ZegoVideoViewMode 定义
 @param index 推流 channel Index
 @return true 成功，false 失败
 */
- (bool)setPreviewViewMode:(ZegoVideoViewMode)mode channelIndex:(ZegoAPIPublishChannelIndex)index;


/**
 设置预览渲染图像的朝向
 
 * 注意：
 * 1.只能设置(0/90/180/270)，默认为0。
 * 2.该设置只是设置预览渲染图像的朝向，不会改变推流时的图像朝向。
 
 @param rotate 逆时针旋转角度
 @return true 成功，false 失败
 @see -setAppOrientation:channelIndex:
 */
- (bool)setPreviewRotation:(int)rotate channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置视频采集的旋转角度
 
 * 注意：
 * 1.只能设置(0/90/180/270)，默认为0。
 * 2.该设置会改变推流时的图像朝向。
 * 3.在InitSDK之后调用有效，UninitSDK前有效。支持推流时调用。
 
 @param rotate 逆时针旋转角度
 @param index 推流 channel Index
 @return true 成功，false 失败
 */
- (bool)setCaptureFrameRotation:(int)rotate channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 是否启用预览镜像
 
 @param enable true 启用，false 不启用。默认 true
 @param index 推流 channel Index
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置
 */
- (bool)enablePreviewMirror:(bool)enable channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 是否启用摄像头采集结果镜像
 
 @param enable true 启用，false 不启用。默认 false
 @param index 推流 channel Index
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置
 */
- (bool)enableCaptureMirror:(bool)enable channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置预览和推流镜像
 
 * 预览镜像只对前置摄像头有效，后置无效。
 * 默认预览启用镜像，推流不启用镜像（即 ZegoVideoMirrorModePreviewMirrorPublishNoMirror）
 
 @param mode 镜像模式，详见 ZegoVideoMirrorMode
 @param index 推流 channel Index
 @return true 成功，false 失败
 */
- (bool)setVideoMirrorMode:(ZegoVideoMirrorMode)mode channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 是否开启码率控制

 * 注意：1. 在推流之前设置有效；
2. 只适用于 RTMP 推流。
 
 @param enable true 启用，false 不启用。默认不启用
 @param index 推流 channel Index
 @return true 成功，false 失败
 @discussion 开启后，在带宽不足的情况下码率自动适应当前带宽
 */
- (bool)enableRateControl:(bool)enable channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置编码器码率控制策略
 
 @param strategy 策略配置，参考 ZegoVideoEncoderRateControlStrategy
 @param encoderCRF 当策略为恒定质量（ZEGOAPI_RC_VBR/ZEGOAPI_RC_CRF）有效，取值范围 [0~51]，越小质量越好，但是码率会相应变大。建议取值范围 [18, 28]
 @param index 推流 channel Index
 */
- (void)setVideoEncoderRateControlConfig:(ZegoAPIVideoEncoderRateControlStrategy)strategy encoderCRF:(int)encoderCRF channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 是否使用前置摄像头，否则使用后置。
 
 * 如果使用前置摄像头，本地预览和推流都将使用前置摄像头拍摄的数据。SDK 默认使用前置摄像头。
 
 @param bFront 是否使用前置摄像头
 @param index 推流 channel Index
 @return true 成功，false 失败
 */
- (bool)setFrontCam:(bool)bFront channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 是否开启视频采集
 
 * 开启视频采集后，预览视图才能显示预览画面，推流时才包含采集的视频数据。SDK 默认开启视频采集。
 
 @param bEnable 是否开启
 @param index 推流 channel Index
 @return 设置结果。true 成功，false 失败
 */
- (bool)enableCamera:(bool)bEnable channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 是否开启手电筒
 
 * 推流时可调用本 API 进行参数配置。SDK 默认关闭手电筒。
 
 @param bEnable 是否开启
 @param index 推流 channel Index
 @return true 成功，false 失败
 */
- (bool)enableTorch:(bool)bEnable channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 预览截图
 
 @param blk 截图结果通过 blk 回调
 @param index 推流 channel Index
 @return true 成功，false 失败
 */
- (bool)takePreviewSnapshot:(ZegoSnapshotCompletionBlock)blk channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置关闭摄像头时所推静态图片的路径
 
 @param filePath 图片路径。如果是完整路径则添加 file: 前缀，如：@"file:/var/image.png"；资产则添加 asset: 前缀，如：@"asset:watermark"
 @param index 推流 channel Index
 @note InitSDK之后，关闭摄像头前调用本 API 进行参数配置，关闭摄像头后会推静态图片
 @note 支持推静态图片时调用此 API 动态更换静态图片
 */
- (void)setDummyCaptureImagePath:(NSString *)filePath channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置水印的图片路径
 
 @param filePath 图片路径。如果是完整路径则添加 file: 前缀，如：@"file:/var/image.png"；资产则添加 asset: 前缀，如：@"asset:watermark"
 @param index 推流 channel Index
 @discussion 推流开始前调用本 API 进行参数配置
 */
- (void)setWaterMarkImagePath:(NSString *)filePath channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置水印在采集视频中的位置
 
 @param waterMarkRect 水印的位置与尺寸
 @param index 推流 channel Index
 @discussion 左上角为坐标系原点，区域不能超过编码分辨率设置的大小
 */
- (void)setPublishWaterMarkRect:(CGRect)waterMarkRect channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置水印在预览视频中的位置
 
 @param waterMarkRect 水印的位置与尺寸
 @param index 推流 channel Index
 @discussion 左上角为坐标系原点，区域不能超过预览视图的大小
 */
- (void)setPreviewWaterMarkRect:(CGRect)waterMarkRect channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 开启屏幕分享编码优化

 @param enable 是否开启。true：开启，false：关闭。默认是关闭的。
 @param channelIndex 推流通道
 @attention 前置条件：仅在软件编码的情况下使用 OpenH264 编码时有效。在默认情况下 SDK 就是使用 OpenH264 编码器。
 @attention 调用 +requireHardwareEncoder:开启硬件编码后，本接口失效。
 @attention 调用 -setVideoCodecId:ofChannel: 设置非默认选项时，本接口失效。
 @note 推流之前设置有效，当次推流结束后失效。
 @note 推荐使用场景：在屏幕共享推流时，SDK 会优化编码，使得清晰度会好很多，视频码率比较稳定，还支持分层编码。
 */
- (void)enableScreenCaptureEncodeOptimization:(bool)enable channelIndex:(ZegoAPIPublishChannelIndex)channelIndex;

/**
 给推流通道设置扩展参数，一般不建议修改

 @param param_config 参数配置信息
 @param idx 推流通道索引
 @attention 配置项写法，例如 "zego_channel_param_key_video_x264_config_tune=animation", 等号后面值的类型要看下面每一项的定义
 @attention "zego_channel_param_key_video_x264_config_tune", string value: animation, 设置编码的 tune 值，目前只支持 animation，仅使用 X264 编码时有效
 @attention 初始化 SDK 之后推流前设置才生效，推流过程中设置无效
*/
- (void)setChannelExtraParam:(NSString *)param_config channelIndex:(ZegoAPIPublishChannelIndex)channelIndex;

/**
 获取当前推流通道使用的 AVCaptureDevice 对象

 @param idx 推流通道索引

 @attention 必须在预览之后或者推流之后才能调用
*/
- (AVCaptureDevice*)getAVCaptureDevice:(ZegoAPIPublishChannelIndex)channelIndex;

/**
 设置指定推流通道的延迟模式
 
 * 设置 SDK 推流时音频使用的延迟模式，可以根据自己的业务场景选择最合适的延迟模式，详情可咨询 ZEGO 技术支持。
 
 * 注意：在InitSDK之后，推流前调用有效。
 
 @param mode 延迟模式，默认 ZEGOAPI_LATENCY_MODE_NORMAL 普通延迟模式
 @param channelIndex 推流通道, 详见 ZegoAPIPublishChannelIndex
 */
- (void)setLatencyMode:(ZegoAPILatencyMode)mode channelIndex:(ZegoAPIPublishChannelIndex)channelIndex;

/**
 设置指定推流通道的推流音频声道数
 
 * 注意：
 * 1.在InitSDK之后，推流前调用有效。
 * 2.[ZegoLiveRoomApi(Publisher) -setLatencyMode:] 中将延迟模式设置为 ZEGO_LATENCY_MODE_NORMAL、ZEGO_LATENCY_MODE_NORMAL2、ZEGO_LATENCY_MODE_LOW3 才能设置双声道。
 * 3.在移动端双声道通常需要配合音频前处理才能体现效果。

 @param count 声道数，1 或 2，默认为 1（单声道）
 @param channelIndex 推流通道, 详见 ZegoAPIPublishChannelIndex
 */
- (void)setAudioChannelCount:(int)count channelIndex:(ZegoAPIPublishChannelIndex)channelIndex;

/**
 是否开启流量控制
 
 * 开启流量控制开关可以使 SDK 通过动态调整音视频的码率、帧率、分辨率来适应当前网络环境，从而保证直播的流畅发布。
 
 * 注意：
 * 1.在推流前调用，连麦/混流方式调用此接口有效。
 * 2.当关闭流量控制时，流量控制属性将被忽略。
 * 3.只适用于 UDP 推流。

 @param enable true 开启；false 关闭，默认 true
 @param properties 流量控制属性 (码率、帧率、分辨率），可以多选，默认为 ZEGOAPI_TRAFFIC_CONTROL_ADAPTIVE_FPS 自适应帧率，参考 ZegoAPITrafficControlProperty 类型选项
 @param channelIndex 推流通道, 详见 ZegoAPIPublishChannelIndex
 */
- (void)enableTrafficControl:(bool)enable properties:(NSUInteger)properties channelIndex:(ZegoAPIPublishChannelIndex)channelIndex;

/**
 设置流量控制中视频码率最小值
 
 * 开启流量控制后，设置视频码率最小值可以让 SDK 当网络不足以达到发送视频最小码率时采取用户设置的策略。
 
 * 注意：可以在初始化之后的任意时间调用，需要开启流量控制该参数才会生效。

 @param bitrate 视频最小码率(bps)，默认为 0，即尽可能的保持视频流畅
 @param mode 视频码率低于最小值后 SDK 的视频推流策略，默认为 ZEGOAPI_TRAFFIC_CONTROL_MIN_VIDEO_BITRATE_NO_VIDEO 停止视频发送
 @param channelIndex 推流通道, 详见 ZegoAPIPublishChannelIndex
 */
- (void)setMinVideoBitrateForTrafficControl:(int)bitrate mode:(ZegoAPITrafficControlMinVideoBitrateMode)mode channelIndex:(ZegoAPIPublishChannelIndex)channelIndex;

/**
 设置流量控制中视频帧率最小值
 
 * 开启流量控制后，设置视频帧率最小值可以让 SDK 采取用户设置的策略。
 
 * 注意：可以在初始化之后的任意时间调用，需要开启流量控制该参数才会生效。

 @param fps 视频最小帧率(fps)，默认为 0，即尽可能的保持视频流畅
 */
- (void)setMinVideoFpsForTrafficControl:(int)fps channelIndex:(ZegoAPIPublishChannelIndex)channelIndex;

/**
 设置流量控制中视频分辨率最小值
 
 * 开启流量控制后，设置视频分辨率最小值可以让 SDK 采取用户设置的策略。
 
 * 注意：可以在初始化之后的任意时间调用，需要开启流量控制该参数才会生效。

 @param width 视频分辨率的宽, 默认为 0，即尽可能的保持视频流畅
 @param height 视频分辨率的高, 默认为 0，即尽可能的保持视频流畅
 */
- (void)setMinVideoResolutionForTrafficControl:(int)width height:(int)height channelIndex:(ZegoAPIPublishChannelIndex)channelIndex;

/**
 设置触发流量控制的关注因素

 * 在初始化 SDK 之后，推流之前调用有效。如果每次推流前不重新设置，将继续使用上次的配置。在反初始化 SDK 之后失效。
 
 * 当 focusOn 设置为 ZEGOAPI_TRAFFIC_CONTROL_FOCUS_ON_REMOTE 时，当对方的网络拥塞时，推流端可能限定推流的码率。
 
 * 当通过 enableTrafficControl:properties: 接口开启了指定推流通道的流量控制后，可通过该接口控制是否因为远端网络状况差而启动流量控制。在 1v1 场景下默认是开启的。
 
 @param focusOn 触发流量控制的关注因素。选择 ZEGOAPI_TRAFFIC_CONTROL_FOCUS_ON_LOCAL_ONLY 时，只关注本地网络状况。选择 ZEGOAPI_TRAFFIC_CONTROL_FOCUS_ON_REMOTE 时，不仅关注本地网络，同时也兼顾远端网络，目前只在 1v1 场景下有效。
 @param channelIndex 推流通道, 详见 ZegoAPIPublishChannelIndex
*/
- (void)setTrafficControlFocusOn:(ZegoAPITrafficControlFocusOn)focusOn channelIndex:(ZegoAPIPublishChannelIndex)channelIndex;

/**
 设置指定通道推流是否校准网络时间，用于混流时各路流的对齐。
 
 @param alignment 1开启，0关闭，默认关闭
 @param index 推流通道
 @return 0 代表设置成功成功，否则设置失败
 @attention 此方法依赖网络时间模块，且需要调用推流接口startPublishWithParams中推流，且参数 ZegoAPIPublishStreamParams.forceSynchronousNetworkTime 为1开启网络时间同步
 @attention initSDK 后, 推流前后都可设置推流之前设置
 */
- (int)setStreamAlignmentProperty:(int)alignment channelIndex:(ZegoAPIPublishChannelIndex)index;


#if TARGET_OS_OSX

/**
 设置视频编码模式
 @param mode 视频编码模式，详见 ZegoVideoEncodeMode。默认是 ZEGOAPI_VIDEO_ENCODE_MODE_LOW_DELAY。
 @param channelIndex 推流通道, 详见 ZegoAPIPublishChannelIndex
 @note 推流之前设置有效，在开始推流后设置，下次推流生效。
 @note ZEGOAPI_VIDEO_ENCODE_MODE_LOW_BITRATE 模式只在使用 x264 软件编码的情况下生效，需要通过 ZEGO 的特殊渠道发包才能指定 x264 编码器。
 */
- (void)setVideoEncodeMode:(ZegoAPIVideoEncodeMode)mode channelIndex:(ZegoAPIPublishChannelIndex)channelIndex;

#endif // TARGET_OS_OSX

@end

