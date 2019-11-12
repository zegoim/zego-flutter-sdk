//
//  ZegoLiveRoomApi-Publisher.h
//  ZegoLiveRoom
//
//  Copyright © 2017年 zego. All rights reserved.
//


#import "ZegoLiveRoomApi.h"
#import "ZegoLiveRoomApiDefines-Publisher.h"
#import "ZegoVideoCapture.h"

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
 开始指定通道发布直播（推流）
 
 * 注意：
 * 1.可以登录房间成功后调用，发布直播后可以使用 [ZegoLiveRoomApi(Publisher2) -stopPublishing:] 停止发布直播。
 * 2.发布直播 API 调用成功后，SDK 会在 [ZegoLivePublisherDelegate -onPublishStateUpdate:streamID:streamInfo:] 回调中通知发布结果，同一房间的其他人会在 [ZegoLiveRoomDelegate -onStreamUpdated:streams:roomID:] 回调中收到流新增通知。
 * 3.为了满足客户的秒开播需求，该接口可以在调用登录房间接口后同步调用，如果登录房间失败，则直播发布也失败。
 * 4.[ZegoLiveRoonApi(Publisher) -startPublishing] 既是主通道推流。
 
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
 * 3.为了满足客户的秒开播需求，该接口可以在调用登录房间接口后同步调用，如果登录房间失败，则直播发布也失败。
 * 4.[ZegoLiveRoonApi(Publisher) -startPublishing] 既是主通道推流。
 
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
 
 @param config 配置信息 key-value，目前 key 仅支持 kPublishCustomTarget ，value 为用户自定义的转推 RTMP 地址。参考 zego-api-defines-oc.h 中相关定义
 @param index 推流 channel Index
 @discussion 开发者如果使用自定义转推功能，推流开始前，必须调用此接口设置转推 RTMP 地址（SDK 推流方式必须为 UDP，转推地址必须为 RTMP），否则可能导致转推失败。
 */
- (void)setPublishConfig:(NSDictionary *)config channelIndex:(ZegoAPIPublishChannelIndex)index;

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
#endif

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
 @discussion 推流开始前调用本 API 进行参数配置。左上角为坐标系原点，区域不能超过编码分辨率设置的大小
 */
- (void)setPublishWaterMarkRect:(CGRect)waterMarkRect channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置水印在预览视频中的位置
 
 @param waterMarkRect 水印的位置与尺寸
 @param index 推流 channel Index
 @discussion 推流开始前调用本 API 进行参数配置。左上角为坐标系原点，区域不能超过预览视图的大小
 */
- (void)setPreviewWaterMarkRect:(CGRect)waterMarkRect channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置外部采集模块
 
 @param factory 工厂对象，遵循 ZegoVideoCaptureFactory 协议的对象
 @param index 推流 channel Index
 @discussion 必须在 InitSDK 前调用，并且不能置空
 */
+ (void)setVideoCaptureFactory:(id<ZegoVideoCaptureFactory>)factory channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置外部滤镜模块
 
 @param factory 工厂对象，遵循 ZegoVideoFilterFactory 协议的对象
 @param index 推流 channel Index
 @discussion 必须在 Init 前调用，并且不能置空
 */
+ (void)setVideoFilterFactory:(id<ZegoVideoFilterFactory>)factory channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 发送媒体次要信息开关
 
 @param start true 开启, false 关闭
 @param onlyAudioPublish true 纯音频直播，不传输视频数据, false 音视频直播，传输视频数据
 @param index 推流 channel Index
 @discussion onlyAudioPublish 开关在 start 开关开启时才生效
 */
- (void)setMediaSideFlags:(bool)start onlyAudioPublish:(bool)onlyAudioPublish channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 发送媒体次要信息开关
 
 @param start true 开启, false 关闭
 @param onlyAudioPublish true 纯音频直播，不传输视频数据, false 音视频直播，传输视频数据
 @param mediaInfoType 请参考 MediaInfoType 定义，建议使用 SeiZegoDefined
 @param seiSendType 请参考 SeiSendType 定义，此参数只对发送SEI时有效，当mediaInfoType为 SideInfoZegoDefined 时此参数无效，当发送SEI时建议使用 SeiSendInVideoFrame
 @param index 推流 channel Index
 @discussion onlyAudioPublish 开关在 start 开关开启时才生效
 */
- (void)setMediaSideFlags:(bool)start onlyAudioPublish:(bool)onlyAudioPublish mediaInfoType:(int)mediaInfoType seiSendType:(int)seiSendType channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 发送媒体次要信息
 
 @param inData 媒体次要信息数据
 @param dataLen 数据长度
 @param packet 是否外部已经打包好包头，true 已打包, false 未打包
 @param index 推流 channel Index
 @discussion 主播端开启媒体次要信息开关，并调用此 API 发送媒体次要信息后，观众端在 [ZegoLiveRoomApi (Player) -setMediaSideCallback:] 设置的回调中获取媒体次要信息
 */
- (void)sendMediaSideInfo:(const unsigned char *)inData dataLen:(int)dataLen packet:(bool)packet channelIndex:(ZegoAPIPublishChannelIndex)index;

@end

