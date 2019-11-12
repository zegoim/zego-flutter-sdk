//
//  ZegoLiveRoomApi-Publisher.h
//  ZegoLiveRoom
//
//  Copyright © 2017年 zego. All rights reserved.
//


#import "ZegoLiveRoomApi.h"
#import "ZegoLiveRoomApiDefines-Publisher.h"
#import "ZegoVideoCapture.h"

@protocol ZegoLivePublisherDelegate;

/**
 推流相关功能分类
 */
@interface ZegoLiveRoomApi (Publisher)

/**
 设置 Publisher 代理对象
 
 * 注意：使用 Publisher 功能前需要设置代理对象，用于接收 ZegoLivePublisherDelegate 回调方法。
 
 @param publisherDelegate 遵守 ZegoLivePublisherDelegate 协议的代理对象，SDK 内部会弱引用该对象。
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)setPublisherDelegate:(id<ZegoLivePublisherDelegate>)publisherDelegate;

/**
 设置是否使用分层视频编码

 * 使用分层视频编码可以实现开发者(连麦/混流)业务中不同终端显示不同质量的视频流、根据网络状态自适应拉取视频流的质量的场景，分层视频编码更详细的描述见官网相关文档。
 
 * 注意：
 * 1.在推流前调用有效。
 * 2.分层视频编码使用的是 ZEGO 的私有协议，仅限于连麦/混流方使用。
 * 3.推流方设置使用分层视频编码后，拉流方可以使用 [ZegoLiveRoomApi(Player) -activateVideoPlayStream:active:videoLayer:] 接口指定拉取的视频分层。
 
 @param codecId 是否使用分层视频编码，默认 VIDEO_CODEC_DEFAULT 即不使用分层视频编码
 @param channel 要设置的推流通道，默认设置主通道 ZEGOAPI_CHN_MAIN
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)setVideoCodecId:(ZegoVideoCodecAvc)codecId ofChannel:(ZegoAPIPublishChannelIndex)channel;

/**
 设置本地预览视图
 
 * 注意：
 * 1.可以在初始化 SDK 之后的任意时间调用，建议在调用 -startPreview 开启预览前，调用该 API 设置本地预览视图，才能在视图上预览采集的视频。
 * 2.建议本地预览结束后调用该 API 将本地预览视图置空。

 @param view 用于渲染本地预览视频的视图，SDK 内部会强引用该对象，传空表示停止 SDK 的本地预览渲染
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)setPreviewView:(ZEGOView *)view;

/**
 设置预览控件的背景颜色
 @param color 颜色,取值为0x00RRGGBB
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)setPreviewViewBackgroundColor:(int)color;

/**
 启动本地预览
 
 * 注意：
 * 1.可以在初始化 SDK 之后的任意时间调用。
 * 2.调用 API 前，需要调用 [ZegoLiveRoomApi(Publisher) -setPreviewView:] 设置本地预览视图才可以看到采集的画面。
 * 3.在退出房间后，SDK 内部会停止预览，如果需要继续预览，需要重新调用本方法开启预览。
 * 4.本地预览默认开启预览镜像功能。

 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)startPreview;

/**
 停止本地预览

 * 注意：
 * 1.可以在初始化 SDK 之后的任意时间调用。
 * 2.建议本地预览结束后调用 [ZegoLiveRoomApi(Publisher) -setPreviewView:] 将本地预览视图置空。

 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)stopPreview;

/**
 开始发布直播（推流）
 
 * 注意：
 * 1.可以在登录房间成功后调用，发布直播后可以使用 [ZegoLiveRoomApi(Publisher) -stopPublishing] 停止发布直播。
 * 2.发布直播 API 调用成功后，SDK 会在 [ZegoLivePublisherDelegate -onPublishStateUpdate:streamID:streamInfo:] 回调中通知发布结果，同一房间的其他人会在 [ZegoLiveRoomDelegate -onStreamUpdated:streams:roomID:] 回调中收到流新增通知。
 * 3.为了满足客户的秒开播需求，该接口可以在调用登录房间接口后同步调用，如果登录房间失败，则直播发布也失败。

 @param streamID 发布直播流的流ID，仅支持长度不超过 256 字节的数字、下划线、字母，streamID 需要在整个 AppID 内全局唯一
 @param title 直播名称，可空，默认为主播用户名
 @param flag 发布直播的模式，参考 ZegoApiPublishFlag 定义
 @return true 表示调用成功，false 表示调用失败。
 @see -stopPublishing
 */
- (bool)startPublishing:(NSString *)streamID title:(NSString *)title flag:(int)flag;

/**
 开始发布直播（推流）
 
 * 注意：
 * 1.可以在登录房间成功后调用，发布直播后可以使用 [ZegoLiveRoomApi(Publisher) -stopPublishing] 停止发布直播。
 * 2.发布直播 API 调用成功后，SDK 会在 [ZegoLivePublisherDelegate -onPublishStateUpdate:streamID:streamInfo:] 回调中通知发布结果，同一房间的其他人会在 [ZegoLiveRoomDelegate -onStreamUpdated:streams:roomID:] 回调中收到流新增通知。
 * 3.为了满足客户的秒开播需求，该接口可以在调用登录房间接口后同步调用，如果登录房间失败，则直播发布也失败。

 @param streamID 发布直播流的流ID，仅支持长度不超过 256 字节的数字、下划线、字母，streamID 需要在整个 AppID 内全局唯一
 @param title 直播名称，可空，默认为主播用户名
 @param flag 发布直播的模式，参考 ZegoApiPublishFlag 定义
 @param extraInfo 流初始附加信息，可空，传空表示附加信息为空字符串，仅支持长度不超过 1024 字节的字符串
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)startPublishing:(NSString *)streamID title:(NSString *)title flag:(int)flag extraInfo:(NSString *)extraInfo;

/**
 更新流附加信息
 
 * 接收端只要开始拉流就会收到流附加信息及其后续更新，适用于想要在流信息中附带附加信息的情况（非变动频繁的信息）。
 
 * 注意：
 * 1.可以在初始化 SDK 之后的任意时间调用，会在直播流中附带该信息。
 * 2.更新流附加信息后，同一房间播放该直播流的人会收到 [ZegoLiveRoomDelegate -onStreamExtraInfoUpdated:roomID:] 回调。
 
 @param extraInfo 流附加信息，可空，传空表示附加信息为空字符串，仅支持长度不超过 1024 字节的字符串
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)updateStreamExtraInfo:(NSString *)extraInfo;

/**
 停止发布直播（结束推流）
 
 * 注意：
 * 1.用于停止已发布的直播，可以在发布直播后调用。
 * 2.停止发布直播不会触发 [ZegoLivePublisherDelegate -onPublishStateUpdate:streamID:streamInfo:] 回调。
 
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)stopPublishing;

/**
 自定义发布直播（推流）配置
 
 * 使用连麦/混流推流模式时，可以通过该接口指定即构服务器转推 CDN 的地址。
 
 * 注意：推流开始前调用有效。

 @param config 推流配置信息，目前 key 仅支持 kPublishCustomTarget，value 为用户指定的转推 RTMP 地址，非 RTMP 地址可能导致转推失败
 */
- (void)setPublishConfig:(NSDictionary<NSString*,NSString*> *)config;

/**
 响应连麦请求
 
 @param seq 连麦请求序列号，标识当次连麦请求
 @param rspResult 响应结果，0 表示同意连麦
 @return true 成功，false 失败
 @discussion 主播端通过 [ZegoLivePublisherDelegate -onJoinLiveRequest:fromUserID:fromUserName:seq:] 收到观众连麦申请，再调用本 API 响应
 */
- (bool)respondJoinLiveReq:(int)seq result:(int)rspResult;

/**
 邀请连麦
 
 @param userID 准备邀请的用户 ID
 @param blk 邀请成功后，通过 blk 回调结果
 @return true 成功，false 失败
 @discussion 主播邀请连麦成功后，被邀请的观众收到  [ZegoLivePlayerDelegate -onInviteJoinLiveRequest:fromUserID:fromUserName:roomID:] 通知
 */
- (bool)inviteJoinLive:(NSString *)userID responseBlock:(ZegoResponseBlock)blk;


/**
 结束连麦
 
 @param userId 指定UserId停止连麦
 @param block 信令发送结果
 @return true 成功，false 失败
 */
- (bool)endJoinLive:(NSString *)userId completionBlock:(ZegoCustomCommandBlock)block;

/**
 设置混流配置
 
 @warning Deprecated，请使用 zego-api-mix-stream-oc.h 中的 [ZegoStreamMixer mixStreamEx:mixStreamID:] 代替
 */
- (bool)setMixStreamConfig:(NSDictionary *)config;

/**
 更新混流配置
 
 @warning Deprecated，请使用 zego-api-mix-stream-oc.h 中的 [ZegoStreamMixer mixStreamEx:mixStreamID:] 代替
 */
- (bool)updateMixInputStreams:(NSArray<ZegoMixStreamInfo*> *)lstMixStreamInfo;


/**
 开始混流
 
 @warning Deprecated，请使用 zego-api-mix-stream-oc.h 中的 [ZegoStreamMixer mixStreamEx:mixStreamID:] 代替
 
 @param completeMixConfig 混流配置
 @param seq 请求序号，回调会带回次 seq
 @return true 成功，等待回调，false 失败
 @discussion 每次需要更新混流配置时，都可以调用此接口；如果需要多次调用，可以通过传入不同的 seq 区分回调
 */
- (bool)mixStream:(ZegoCompleteMixStreamConfig *)completeMixConfig seq:(int)seq;

/**
 硬件编码开关
 
 * 注意：
 * 1.在推流之前设置有效。
 * 2.硬编硬解开关最好后台可控，避免碰到版本升级或者硬件升级时出现硬编硬解失败的问题。

 @param bRequire true 开启，false 关闭。默认 false
 @return true 表示调用成功，false 表示调用失败。
 */
+ (bool)requireHardwareEncoder:(bool)bRequire;

/**
 设置发布直播（推流）视频配置
 
 * 调用本 API 可以配置发布直播时视频采集、编码的参数。
 
 * 注意：
 * 1.在推流之前设置有效，在推流后设置的视频采集分辨率将无效并且不推荐在推流之后修改设置。
 * 2.推荐使用 [ZegoLiveRoomApi(Publisher) -enableTrafficControl:properties:] 流量控制，让 SDK 进行动态码率、帧率、分辨率控制。

 @param config 发布直播视频配置参数（视频编码输出分辨率、视频采集分辨率、视频帧率、视频码率）
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)setAVConfig:(ZegoAVConfig *)config;

#if TARGET_OS_IPHONE
/**
 设置手机方向
 
 * 本设置用于校正输出视频朝向，默认竖屏。建议在预览和推流之前设置好方向。
 * 不建议在推流过程中旋转画面，原因：
 * 1. 如果刚开始为竖屏开播，此时旋转为横屏，但是分辨率仍然是竖屏的，会导致横屏后，画面放大的效果(按照等比填充的模式) 或者 上下黑边的效果(按照等比缩放的模式)，除非旋转为横屏后，画面的分辨率也做修改。
 * 2. 直播过程中修改分辨率的话，会导致录制文件(回放)有异常，因此不建议中途旋转画面。
 
 @param orientation 手机方向
 @return true 成功，false 失败
 @see -startPreview
 @see -startPublishing:title:flag:
 */
- (bool)setAppOrientation:(UIInterfaceOrientation)orientation;
#endif

/**
 主播方开启美颜功能
 
 @param feature 美颜特性，参考 ZegoBeautifyFeature 定义。默认无美颜
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置
 */
- (bool)enableBeautifying:(int)feature;

/**
 设置美颜磨皮的采样步长
 
 @param step 采样步长，取值范围[1,16]。默认 4.0
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置。设置时需确保对应美颜特性开启
 */
- (bool)setPolishStep:(float)step;

/**
 设置美颜采样颜色阈值
 
 @param factor 采样颜色阈值，取值范围[0,16]。默认 4.0
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置。设置时需确保对应美颜特性开启
 
 */
- (bool)setPolishFactor:(float)factor;

/**
 设置美颜美白的亮度修正参数
 
 @param factor 亮度修正参数，取值范围[0,1]，值越大亮度越暗。默认 0.5
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置。设置时需确保对应美颜特性开启
 */
- (bool)setWhitenFactor:(float)factor;

/**
 设置锐化参数
 
 @param factor 锐化参数，取值范围[0,2]，值越大锐化越强。默认 0.2
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置。设置时需确保对应美颜特性开启
 */
- (bool)setSharpenFactor:(float)factor;

/**
 设置滤镜
 
 @param filter 滤镜种类，参考 ZegoFilter 定义。默认不使用滤镜
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置
 */
- (bool)setFilter:(ZegoFilter)filter;

/**
 设置本地预览视图的模式
 
 * 视图模式默认为 ZegoVideoViewModeScaleAspectFill。
 
 @param mode 视图模式。详见 ZegoVideoViewMode 定义
 @return true 成功，false 失败
 */
- (bool)setPreviewViewMode:(ZegoVideoViewMode)mode;

/**
 设置预览渲染图像的朝向
 
 * 注意：
 * 1.只能设置(0/90/180/270)，默认为0。
 * 2.该设置只是设置预览渲染图像的朝向，不会改变推流时的图像朝向。
 
 @param rotate 逆时针旋转角度
 @return true 成功，false 失败
 @see -setAppOrientation:
 */
- (bool)setPreviewRotation:(int)rotate;

/**
 是否启用预览镜像
 
 @param enable true 启用，false 不启用。默认 true
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置
 */
- (bool)enablePreviewMirror:(bool)enable;

/**
 是否启用摄像头采集结果镜像
 
 @param enable true 启用，false 不启用。默认 false
 @return true 成功，false 失败
 @discussion 推流时可调用本 API 进行参数配置
 */
- (bool)enableCaptureMirror:(bool)enable;

/**
 设置预览和推流镜像
 
 * 预览镜像只对前置摄像头有效，后置无效。
 * 默认预览启用镜像，推流不启用镜像（即 ZegoVideoMirrorModePreviewMirrorPublishNoMirror）
 
 @param mode 镜像模式，详见 ZegoVideoMirrorMode
 @return true 成功，false 失败
 */
- (bool)setVideoMirrorMode:(ZegoVideoMirrorMode)mode;

/**
 是否开启码率控制
 
 * 注意：
 * 1.在推流之前设置有效。
 * 2.不推荐使用此接口，可使用 [ZegoLiveRoomApi(Publisher) -setVideoEncoderRateControlConfig:encoderCRF:] 调整编码器的码率控制策略。

 @param enable true 启用，false 不启用。默认 false
 @return true 表示调用成功，false 表示调用失败。
 @see -setVideoEncoderRateControlConfig:encoderCRF:
 */
- (bool)enableRateControl:(bool)enable;

/**
 设置编码器码率控制策略
 
 * 注意：在推流之前设置有效。

 @param strategy 码率控制策略，默认 ZEGOAPI_RC_CBR 恒定码率
 @param encoderCRF 当策略为恒定质量（ZEGOAPI_RC_VBR/ZEGOAPI_RC_CRF）有效，取值范围 [0,51]，越小质量越好，但是码率会相应变大。默认 23，建议取值范围 [18,28]
 */
- (void)setVideoEncoderRateControlConfig:(ZegoAPIVideoEncoderRateControlStrategy)strategy encoderCRF:(int)encoderCRF;

/**
 是否使用前置摄像头，否则使用后置。
 
 * 如果使用前置摄像头，本地预览和推流都将使用前置摄像头拍摄的数据。SDK 默认使用前置摄像头。
 
 @param bFront 是否使用前置摄像头
 @return true 成功，false 失败
 */
- (bool)setFrontCam:(bool)bFront;

/**
 是否开启麦克风
 
 * 开启麦克风后，推流数据才包含麦克风采集的音频数据。SDK 默认开启麦克风。
 
 @param bEnable true 打开，false 关闭
 @return true 成功，false 失败
 */
- (bool)enableMic:(bool)bEnable;

/**
 是否开启视频采集
 
 * 开启视频采集后，预览视图才能显示预览画面，推流时才包含采集的视频数据。SDK 默认开启视频采集。

 @param bEnable 是否开启
 @return 设置结果。true 成功，false 失败
 */
- (bool)enableCamera:(bool)bEnable;

/**
 是否开启手电筒
 
 * 推流时可调用本 API 进行参数配置。SDK 默认关闭手电筒。
 
 @param bEnable 是否开启
 @return true 成功，false 失败
 */
- (bool)enableTorch:(bool) bEnable;

/**
 预览截图
 
 * 截图结果通过 blk 回调。
 
 @param blk 截图结果回调
 @return true 成功，false 失败
 */
- (bool)takePreviewSnapshot:(ZegoSnapshotCompletionBlock)blk;

/**
 是否开启采集监听（即耳返）
 
 * 开启耳返后，主播方讲话会听到自己的声音。
 * 在连接上耳麦时耳返功能才实际生效。
 
 @param bEnable true 打开，false 关闭。默认 false
 @return true 成功，false 失败
 */
- (bool)enableLoopback:(bool)bEnable;

/**
 设置采集监听（耳返）音量
 
 @param volume 音量大小。取值范围为（0, 100），默认 80
 */
- (void)setLoopbackVolume:(int)volume;

/**
 设置采集音量
 
 * SDK 采集音频时会根据该 API 设置的音量值进行音量大小的控制。开发者可在推流时调用该 API 调节音频数据的音量大小。
 
 @param volume 音量大小，取值（0, 100）。默认 100
 */
- (void)setCaptureVolume:(int)volume;

/**
 混音开关
 
 @param enable true 启用混音输入，false 关闭混音输入。默认 false
 @return true 成功，false 失败
 @discussion 推流开始前调用本 API 进行参数配置。主播端开启混音后，SDK 在 ZegoLiveRoomApi (Publisher) -onAuxCallback:dataLen:sampleRate:channelCount: 中获取混音输入数据
 @warning Deprecated，请使用 ZegoAudioAux 的 enableAux:
 */
- (bool)enableAux:(BOOL)enable;

/**
 混音静音开关
 
 @note 1. 当开启静音后，主播端将听不到混音内容，观众端依然能听到混音声音。
 
 * 2.此 API 可以在混音中的任意时间调用，取决于用户需求。
 @param bMute  true 表示静音，false 表示恢复音量。
 @return  true 表示调用成功，false 表示调用失败。
 @see 相关接口请查看 ZegoLiveRoomApi (Publisher) -setAuxVolume:
 @warning Deprecated，请使用 ZegoAudioAux 的 muteAux:
 */
- (bool)muteAux:(bool)bMute;

/**
 获取当前采集的音量
 
 @return 当前采集的音量值
 */
- (float)getCaptureSoundLevel;

/**
 设置水印的图片路径
 
 @param filePath 图片路径。如果是完整路径则添加 file: 前缀，如：@"file:/var/image.png"；资产则添加 asset: 前缀，如：@"asset:watermark"
 @discussion 推流开始前调用本 API 进行参数配置
 */
- (void)setWaterMarkImagePath:(NSString *)filePath;

/**
 设置水印在采集视频中的位置
 
 @param waterMarkRect 水印的位置与尺寸
 @discussion 推流开始前调用本 API 进行参数配置。左上角为坐标系原点，区域不能超过编码分辨率设置的大小
 */
- (void)setPublishWaterMarkRect:(CGRect)waterMarkRect;

/**
 设置水印在预览视频中的位置
 
 @param waterMarkRect 水印的位置与尺寸
 @discussion 推流开始前调用本 API 进行参数配置。左上角为坐标系原点，区域不能超过预览视图的大小
 */
- (void)setPreviewWaterMarkRect:(CGRect)waterMarkRect;

/**
 设置音频编码码率
 
 * 注意：
 * 1.可以在初始化 SDK 后任意时间设置，但不推荐在推流时动态修改码率。
 * 2.推荐使用 [ZegoLiveRoomApi(Publisher) -enableTrafficControl:properties:] 让 SDK 进行动态码率控制。

 @param bitrate 音频编码码率(bps)
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)setAudioBitrate:(int)bitrate;

/**
 设置音频设备模式
 
 * 使用该 API 设置不同的模式可实现硬件音频前处理，功能包括回音消除、噪声抑制。
 * 建议开发者根据情景选择合适的模式，以达到前处理的最优效果。
 * 直播场景下，推荐使用 ZEGOAPI_AUDIO_DEVICE_MODE_AUTO；
 * 实时音、视频场景下（比如教育、游戏等），推荐使用 ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION，可避免 SDK 内部因切换 Mode 而启动、停止音频设备等耗时操作。
 * 当业务方的用户主要在带耳机的情况下推流时，如果需要保证较好的音质效果，可使用 ZEGOAPI_AUDIO_DEVICE_MODE_GENERAL，除此情景之外，不推荐使用 GENERAL 模式，因为无法保证采集的音质效果。
 * 默认使用 ZEGOAPI_AUDIO_DEVICE_MODE_AUTO。
 
 * 注意：
 * 1.必须在 Init SDK 前调用才能生效。
 * 2.如果需要在使用耳机时进行音频前处理，需要调用 [ZegoLiveRoomApi(AudioIO) -enableAECWhenHeadsetDetected:] 设置 YES，该情况只在音频设备模式为 ZEGOAPI_AUDIO_DEVICE_MODE_AUTO，ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION 下才有效。
 
 @param mode 音频设备模式，详见 ZegoAPIAudioDeviceMode
 @see -enableAEC:
 @see -enableNoiseSuppress:
 */
+ (void)setAudioDeviceMode:(ZegoAPIAudioDeviceMode) mode;

/**
 是否开启软件回声消除
 
 * 推荐结合硬件音频前处理一起使用，当硬件音频前处理效果不好、被关闭或不被支持时，可开启软件音频前处理。
 * 建议在推流前调用设置。
 
 * 注意：
 * 1.软件回音消除只能处理 zego SDK 播放的音频，不能处理第三方播放器播放的视频。
 * 2.如果使用音频外部采集，需要开发者自己去回声消除，该开关无效。
 * 3.iOS 默认不开启该开关，Android 是默认开启该开关。
 
 @param enable 是否开启
 @return true 成功 false 失败
 @see -setAECMode:
 @see -enableNoiseSuppress:
 @see +setAudioDeviceMode:
 */
- (bool)enableAEC:(bool)enable;

/**
 设置回声消除模式

 * 建议在推流前调用设置。默认为 ZEGOAPI_AEC_MODE_ARRGRESSIVE。
 
 @param mode 回声消除模式
 */
- (void)setAECMode:(ZegoAPIAECMode)mode;

/**
 音频采集自动增益开关
 
 * 开启该开关后，SDK 可将太小或太大的音量处理成一定范围的音量，让整段音频的音量控制在一定范围内。在教育、视频会议等对音量稳定性有要求的场景可启用该开关。
 * 建议在推流前调用设置。默认关闭。
 
 @param enable 是否开启
 @return true 成功，false 失败
 */
- (bool)enableAGC:(bool)enable;

/**
 设置外部采集模块
 
 @param factory 工厂对象，遵循 ZegoVideoCaptureFactory 协议的对象
 @discussion 必须在 InitSDK 前调用，并且不能置空
 @warning Deprecated，请使用 zego-api-external-video-capture-oc.h 中的 [ZegoExternalVideoCapture setVideoCaptureFactory:channelIndex:]
 */
+ (void)setVideoCaptureFactory:(id<ZegoVideoCaptureFactory>)factory;

/**
 设置外部滤镜模块
 
 @param factory 工厂对象，遵循 ZegoVideoFilterFactory 协议的对象
 @discussion 必须在 Init 前调用，并且不能置空
 @warning Deprecated，请使用 zego-api-external-video-filter-oc.h 中的 [ZegoExternalVideoFilter setVideoFilterFactory:channelIndex:]
 */
+ (void)setVideoFilterFactory:(id<ZegoVideoFilterFactory>)factory;

/**
 发送媒体次要信息开关
 
 @param start true 开启媒体次要信息传输, false 关闭媒体次要信息传输。start 为 true 时，onlyAudioPublish 开关才有效
 @param onlyAudioPublish true 纯音频直播，不传输视频数据，false 音视频直播，传输视频数据。默认为 false。如果本次只有音频直播，必须将 onlyAudioPublish 置为 true，此时会由音频来驱动次要信息的传输，同时忽略视频流传输
 @discussion 初始化 SDK 后，开始推流前调用。
 */
- (void)setMediaSideFlags:(bool)start onlyAudioPublish:(bool)onlyAudioPublish;

/**
 发送媒体次要信息
 
 @param inData 需要传输的音视频次要信息数据，外部输入
 @param dataLen 传入的 inData 长度，不能大于 1000 Bytes
 @param packet 是否外部已经打包好包头，true 已打包, false 未打包。
 @discussion 主播端开启媒体次要信息开关，开始推流后调用。调用此 API 发送媒体次要信息后，观众端在 [ZegoLiveRoomApi (Player) -setMediaSideCallback:] 设置的回调中获取媒体次要信息。不需要发送媒体次要信息时，可调用 [ZegoLiveRoomApi (Publisher) setMediaSideFlags:false onlyAudioPublish:false] 关闭通道
 */
- (void)sendMediaSideInfo:(const unsigned char *)inData dataLen:(int)dataLen packet:(bool)packet;

/**
 设置视频采集缩放时机
 
 * 注意：
 * 1.建议在启动本地预览前调用，之后调用不会立即生效，而是在下次摄像头启动预览时生效。
 * 2.如果设置为先缩放（ZEGOAPI_CAPTURE_PIPELINE_SCALE_MODE_PRE），预览的分辨率就是编码分辨率；设置为后缩放（ZEGOAPI_CAPTURE_PIPELINE_SCALE_MODE_POST），预览的分辨率就是采集分辨率。
 * 默认为 ZEGOAPI_CAPTURE_PIPELINE_SCALE_MODE_PRE。
 
 @param mode 视频采集缩放时机。详见 ZegoAPICapturePipelineScaleMode
 */
- (void)SetCapturePipelineScaleMode:(ZegoAPICapturePipelineScaleMode)mode;

/**
 设置延迟模式
 
 * 设置 SDK 推流时音频使用的延迟模式，可以根据自己的业务场景选择最合适的延迟模式，详情可咨询 ZEGO 技术支持。
 
 * 注意：在推流前调用有效。
 
 @param mode 延迟模式，默认 ZEGOAPI_LATENCY_MODE_NORMAL 普通延迟模式
 */
- (void)setLatencyMode:(ZegoAPILatencyMode)mode;

/**
 设置推流音频声道数
 
 * 注意：
 * 1.在推流前调用有效。
 * 2.[ZegoLiveRoomApi(Publisher) -setLatencyMode:] 中将延迟模式设置为 ZEGO_LATENCY_MODE_NORMAL、ZEGO_LATENCY_MODE_NORMAL2、ZEGO_LATENCY_MODE_LOW3 才能设置双声道。
 * 3.在移动端双声道通常需要配合音频前处理才能体现效果。

 @param count 声道数，1 或 2，默认为 1（单声道）
 */
- (void)setAudioChannelCount:(int)count;


/**
 设置混音音量
 
 * 注意：
 * 1.可以在初始化 SDK 之后任意时刻调用。
 * 2.SetAuxVolume 的工作逻辑都是基于对引擎的输入输出数据进行处理，即对输入 SDK 的音频数据的音量大小进行设置，与进行混音的推流设备的系统音量没有关系。
 * 3.采集混音需要的音频数据时采用 general 模式，一般是媒体音量；采用 communication 模式，一般是通话音量；采用 auto 模式，连麦时会变成通话音量。
 @param volume 音量值范围 [0,100]，默认为 50
 @see 相关接口请查看 [ZegoLiveRoomApi(Publisher) -muteAux:]，[ZegoLiveRoomApi(Publisher) -enableAux:]
 @warning Deprecated，请使用 [ZegoAudioAux -setAuxVolume:]
 */
- (void)setAuxVolume:(int)volume;

/**
 静音后不发送音频包（离散音频包发送）
 
 * 开启离散音频包发送可以在关闭麦克风或者静音的状态下，停止音频包的发送，节省用户流量。
 
 * 注意：
 * 1.推流前调用有效，连麦/混流方式调用此接口有效。
 * 2.可以和 [ZegoLiveRoomApi(Publisher) -enableVAD:] 配合使用。
 * 3.如果为音频单流(非混流)转推 CDN，开启此选项将导致 CDN 在设定时间内未收到数据包认为流关闭而断开连接，所以此种情况不建议开启。
 
 @param enable true 开启，false 关闭，默认 true
 */
- (void)enableDTX:(bool)enable;

/**
 是否开启语音活动检测
 
 * 开启语音活动检测可以在检测到没有语音只有底噪声的情况下，直接发送静音包，节省用户流量。
 
 * 注意：
 * 1.推流前调用有效，连麦/混流方式调用此接口有效。
 * 2.可以和 [ZegoLiveRoomApi(Publisher) -enableDTX:] 配合使用。

 @param enable true 开启；false 关闭，默认 false
 */
- (void)enableVAD:(bool)enable;

/**
 是否开启流量控制
 
 * 开启流量控制开关可以使 SDK 通过动态调整音视频的码率、帧率、分辨率来适应当前网络环境，从而保证直播的流畅发布。
 
 * 注意：
 * 1.在推流前调用，连麦/混流方式调用此接口有效。
 * 2.当关闭流量控制时，流量控制属性将被忽略。

 @param enable true 开启；false 关闭，默认 true
 @param properties 流量控制属性 (码率、帧率、分辨率），可以多选，默认为 ZEGOAPI_TRAFFIC_CONTROL_ADAPTIVE_FPS 自适应帧率，参考 ZegoAPITrafficControlProperty 类型选项
 */
- (void)enableTrafficControl:(bool)enable properties:(NSUInteger)properties;

/**
 设置流量控制中视频码率最小值
 
 * 开启流量控制后，设置视频码率最小值可以让 SDK 当网络不足以达到发送视频最小码率时采取用户设置的策略。
 
 * 注意：可以在初始化之后的任意时间调用，需要开启流量控制该参数才会生效。

 @param bitrate 视频最小码率(bps)，默认为 0，即尽可能的保持视频流畅
 @param mode 视频码率低于最小值后 SDK 的视频推流策略，默认为 ZEGOAPI_TRAFFIC_CONTROL_MIN_VIDEO_BITRATE_NO_VIDEO 停止视频发送
 */
- (void)setMinVideoBitrateForTrafficControl:(int)bitrate mode:(ZegoAPITrafficControlMinVideoBitrateMode)mode;

/**
 是否开启软件音频采集噪声抑制
 
 *  推荐结合硬件音频前处理一起使用，当硬件音频前处理效果不好、被关闭或不被支持时，可开启软件音频前处理。
 * 建议在推流前调用设置。
 
 * 注意：
 * 1.软件回音消除只能处理 zego SDK 播放的音频，不能处理第三方播放器播放的视频。
 * 2.如果使用音频外部采集，需要开发者自己去噪声抑制，该开关无效。
 * 3.iOS 默认不开启该开关，Android 是默认开启该开关。
 
 @param enable 是否开启
 @return true 成功 false 失败
 @see -enableAEC:
 @see +setAudioDeviceMode:
 */
- (bool)enableNoiseSuppress:(bool)enable;

/**
 设置发布直播（推流）质量监控周期
 
 * 注意：
 * 1.推流前调用有效。
 * 2.该设置会影响 [ZegoLivePublisherDelegate -onPublishQualityUpdate:quality:] 的回调频率。

 @param timeInMS 质量监控回调的时间周期，单位为毫秒，取值范围为[500,60000]。默认为 3000
 */
+ (void)setPublishQualityMonitorCycle:(unsigned int)timeInMS;


/**
 音效均衡器

 @param bandGain 取值范围[-15, 15]。默认值是0，如果所有频带的增益值全部为0，则会关闭EQ功能
 @param bandIndex 取值范围[0, 9]。分别对应10个频带，其中心频率分别是[31, 62, 125, 250, 500, 1K, 2K, 4K, 8K, 16K]Hz
 @return true 调用成功，false 调用失败
 @attention 在InitSDK之后调用有效。使用此接口前请与即构技术支持联系确认是否支持此功能。
 */
- (bool)setAudioEqualizerGain:(float)bandGain index:(int)bandIndex;

/**
 推流时是否发送视频数据。

 @param mute true 不发送(仅预览)，false 发送
 @return 0 代表设置成功成功，否则设置失败
 @attention 拉流端通过 OnRemoteCameraStatusUpdate 回调监听此状态是否改变;
 @attention 仅拉 UDP 流时，才能接收到状态变更通知;
 */
- (int)muteVideoPublish:(bool)mute;

/**
 推流时是否发送音频数据。
 
 @param mute true 不发送，false 发送
 @return 0 代表设置成功成功，否则设置失败
 @attention 可以通过 ZegoLiveApiAudioRecordDelegate 回调获取本地音频数据
 */
- (int)muteAudioPublish:(bool)mute;


@end


/**
 Publisher 代理
 */
@protocol ZegoLivePublisherDelegate <NSObject>

/**
 发布直播（推流）状态更新
 
 * 用户调用发布直播接口后，会通过该 API 通知用户发布直播操作的状态更新，在发布直播状态变化后就会回调，即可能会回调多次。
 
 * 注意：
 * 2.当回调失败，发布直播就已经停止，无需调用停止发布直播接口，SDK 不会自动重试发布直播，需要调用方重新尝试发布直播，或者提示用户发布失败执行其他业务逻辑。
 * 3.用户手动调用停止发布直播接口、房间登录回调错误的情况下，该 API 不会回调。
 * 4.info 目前的结构为 @{kZegoRtmpUrlListKey:@[RtmpUrlList], kZegoHlsUrlListKey:@[HlsUrlList], kZegoFlvUrlListKey:@[FlvUrlList]}。

 @param stateCode 发布直播状态码，详见 ZegoErrorCode，0 为推流操作成功，否则为操作失败
 @param streamID 发布直播的流ID，在用户同时推多路流的情况下需要以此判断本次回调表示的是哪一路流的状态更新
 @param info CDN 转推相关信息
 */
- (void)onPublishStateUpdate:(int)stateCode streamID:(NSString *)streamID streamInfo:(NSDictionary<NSString*,NSArray<NSString*>*> *)info;

@optional

/**
 收到连麦请求
 
 @param seq 连麦请求序列号，标识当次连麦请求
 @param userId 来源用户 ID
 @param userName 来源用户名
 @param roomID 房间 ID
 @discussion 观众调用 [ZegoLiveRoomApi (Player) -requestJoinLive] 申请连麦后，主播端会收到本通知
 */
- (void)onJoinLiveRequest:(int)seq fromUserID:(NSString *)userId fromUserName:(NSString *)userName roomID:(NSString *)roomID;

/**
 发布直播（推流）质量更新
 
 * 可以在此 API 中获取当前发布直播的质量，加以处理。
 
 * 注意：
 * 1.此接口目前不建议使用，推荐使用 [ZegoLivePublisherDelegate -onPublishQualityUpdate:quality:] 获取质量。
 * 2.在发布直播后，此 API 就会以 [ZegoLiveRoomApi(Publisher) -setPublishQualityMonitorCycle:] 接口设置的回调间隔持续回调。

 @param quality 发布直播质量，[0,3]分别对应优、良、中、差
 @param streamID 发布直播的流ID，在用户同时推多路流的情况下需要以此判断本次回调表示的是哪一路流的状态更新
 @param fps 发布直播的帧率(fps)
 @param kbs 发布直播的码率（kbps）
 @see -setPublishQualityMonitorCycle:
 */
- (void)onPublishQualityUpdate:(int)quality stream:(NSString *)streamID videoFPS:(double)fps videoBitrate:(double)kbs;

/**
 发布直播（推流）质量更新
 
 * 可以在此 API 中获取当前发布直播的质量，加以处理。
 
 * 注意：在发布直播后，此 API 就会以 [ZegoLiveRoomApi(Publisher) -setPublishQualityMonitorCycle:] 设置的回调间隔持续回调。

 @param streamID streamID 发布直播的流ID，在用户同时推多路流的情况下需要以此判断本次回调表示的是哪一路流的状态更新
 @param quality 发布直播质量结构体
 @see -setPublishQualityMonitorCycle:
 */
- (void)onPublishQualityUpdate:(NSString *)streamID quality:(ZegoApiPublishQuality)quality;

/**
 采集视频的宽度和高度变化通知
 
 @param size 视频大小
 @discussion 发布直播成功后，当视频尺寸变化时，发布者会收到此通知
 */
- (void)onCaptureVideoSizeChangedTo:(CGSize)size;

- (void)onCaptureVideoSizeChangedTo:(CGSize)size channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 混流配置更新结果回调
 
 @warning Deprecated，请使用 zego-api-mix-stream-oc.h 中的 [ZegoMixStreamExDelegate onMixStreamExConfigUpdate:mixStream:streamInfo:] 代替
 
 @param errorCode 错误码，0 表示没有错误
 @param mixStreamID 混流ID
 @param info 混流播放信息
 @discussion 调用 [ZegoLiveRoomApi (Publisher) -setMixStreamConfig:] 设置混流配置，及 [ZegoLiveRoomApi (Publisher) -updateMixInputStreams:] 更新混流配置后，通过此 API 通知调用方
 @note 常见错误码及其含义请参考ZegoError中kMixStream开头的错误码定义
 */
- (void)onMixStreamConfigUpdate:(int)errorCode mixStream:(NSString *)mixStreamID streamInfo:(NSDictionary *)info;

/**
 混音数据输入回调
 
 * 注意：
 * 1.最大支持 48k 采样率、双声道、16位深的 PCM 音频数据。
 * 2.实际数据长度应根据当前音频数据的采样率及声道数决定。
 * 3.为确保混音效果，请不要在此 API 中执行耗时操作。
 * 4.用户调用该 API 将混音数据传递给 SDK，混音数据 bit depth 必须为 16。
 @param pData 混音数据
 @param pDataLen pDataLen既是输入参数也是输出参数；
                 作为输入参数，SDK会提供好长度值，用户按照这个长度写入数据即可，数据充足的情况下，无需更改*pDataLen的值
                 作为输出参数，如果填写的数据不足SDK提供的长度值，则*pDataLen = 0,
                 或者最后的尾音不足 SDK提供的长度值，可以用静音数据补齐。
 @param pSampleRate 混音数据采样率，支持16k、32k、44.1k、48k
 @param pChannelCount 混音数据声道数，支持1、2
 @warning Deprecated，请使用 ZegoAudioAuxDelgate 代替
 */
- (void)onAuxCallback:(void *)pData dataLen:(int *)pDataLen sampleRate:(int *)pSampleRate channelCount:(int *)pChannelCount;

/**
 发布直播转推 CDN 状态信息更新
 
 * 可以通过此 API 获取当前发布的直播转推 CDN 的状态更新。

 @param statesInfo 转推CDN状态信息数组
 @param streamID 发布直播的流ID，在用户同时推多路流的情况下需要以此判断本次回调表示的是哪一路流的状态更新
 */
- (void)onRelayCDNStateUpdate:(NSArray<ZegoAPIStreamRelayCDNInfo *> *)statesInfo streamID:(NSString*)streamID;

/**
 采集视频的首帧通知
 
 * 可以通过此 API 获取 SDK 采集到视频首帧的时间，可以在此 API 中移除预览占位图，然后设置预览视图。
 * 开始预览和开始发布直播均会触发采集。
 */
- (void)onCaptureVideoFirstFrame;

/**
 采集视频的首帧通知
 
 * 可以通过此 API 获取 SDK 采集到视频首帧的时间，可以在此 API 中移除预览占位图，然后设置预览视图。
 
 * 注意：开始预览和开始发布直播均会触发采集。

 @param index 推流使用的推流通道
 */
- (void)onCaptureVideoFirstFrame:(ZegoAPIPublishChannelIndex)index;

/**
 采集音频的首帧通知
 
 * 可以通过此 API 获取 SDK 采集到音频首帧的时间。
 
 * 注意：开始预览和开始发布直播均会触发采集。
 */
- (void)onCaptureAudioFirstFrame;

@end
