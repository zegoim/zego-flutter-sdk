//
//  ZegoLiveRoomApi-Player.h
//  ZegoLiveRoom
//
//  Copyright © 2017年 zego. All rights reserved.
//

#import "ZegoLiveRoomApi.h"
#import "ZegoLiveRoomApiDefines.h"
#import "ZegoLiveRoomApiDefines-IM.h"

@protocol ZegoLivePlayerDelegate;
@protocol ZegoLiveApiAudioRecordDelegate;


@interface ZegoLiveRoomApi (Player)

/**
 设置直播观众相关信息通知的回调，以接收拉流事件回调

 * 注意：
 * 1. 在调用初始化 SDK 接口之后、拉流接口 startPlayingStream 之前设置。

 @param playerDelegate 实现了 ZegoLivePlayerDelegate 协议的代理对象，以接收拉流事件
 @return true 设置成功，false 设置失败
 */
- (bool)setPlayerDelegate:(id<ZegoLivePlayerDelegate>)playerDelegate;

/**
 设置拉流解密密钥（只用于拉流之后更新密钥）

 @param streamID 播放流 ID
 @param key 解密密钥（支持16/24/32字节）
 */
- (void)updatePlayDecryptKey:(NSString *)streamID key:(NSData*)key;

/**
 播放多媒体流
 
 * 注意：
 * 1. 调用此 API 可播放直播流，必须在调用初始化 SDK 接口之后调用。
 * 2. 调用成功后，等待 [ZegoLivePlayerDelegate -onPlayStateUpdate:streamID:] 回调。
 * 3. 如果有收到 [ZegoRoomDelegate -onDisconnect:roomID:] 回调，则不会再收到 [ZegoLivePlayerDelegate -onPlayStateUpdate:streamID:] 回调。
 
 @param streamID 需要播放的用户流 ID
 @param view 用来渲染播放视频的视图，设置为空时不显示视频，只播放音频
 @return true 调用成功，false 调用失败
 */
- (bool)startPlayingStream:(NSString *)streamID inView:(ZEGOView *)view;

/**
 播放多媒体流
 
 * 注意：
 * 1. 调用此 API可播放直播流，必须在调用初始化 SDK 接口之后调用。
 * 2. 调用成功后，等待 [ZegoLivePlayerDelegate -onPlayStateUpdate:streamID:] 回调。
 * 3. 如果有收到 [ZegoRoomDelegate -onDisconnect:roomID:] 回调，则不会再收到 [ZegoLivePlayerDelegate -onPlayStateUpdate:streamID:] 回调。
 
 @param streamID 需要播放的用户流 ID
 @param view 用来渲染播放视频的视图，设置为空时不显示视频，只播放音频
 @param params 拉流参数
 @return true 调用成功，false 调用失败
 */
- (bool)startPlayingStream:(NSString *)streamID inView:(ZEGOView *)view params:(NSString *)params;


/**
 根据用户配置的多媒体流附加信息，播放多媒体流
 
 * 注意：
 * 1. 调用此 API可播放直播流，必须在调用初始化 SDK 接口之后调用。
 * 2. 调用成功后，等待 [ZegoLivePlayerDelegate -onPlayStateUpdate:streamID:] 回调。
 
 @param streamID 需要播放的用户流 ID
 @param view 用来渲染播放视频的视图，设置为空时不显示视频，只播放音频
 @param info 多媒体流附加信息
 @return true 调用成功，false 调用失败
 */
- (bool)startPlayingStream:(NSString *)streamID inView:(ZEGOView *)view extraInfo:(ZegoAPIStreamExtraPlayInfo*)info;

/**
 播放多媒体流
 
 * 注意：
 * 1. 调用此 API 可播放直播流，必须在调用初始化 SDK 接口之后调用。
 * 2. 调用成功后，等待 [ZegoLivePlayerDelegate -onPlayStateUpdate:streamID:] 回调。
 * 3. 如果有收到 [ZegoRoomDelegate -onDisconnect:roomID:] 回调，则不会再收到 [ZegoLivePlayerDelegate -onPlayStateUpdate:streamID:] 回调。
 
 @param params 拉流参数, 详见 ZegoAPIPlayStreamParams
 @return true 调用成功，false 调用失败
 */
- (bool)startPlayingStreamWithParams:(ZegoAPIPlayStreamParams *)params;

/**
 更新播放视图
 
 * 注意：
 * 1. 调用 startPlayingStream 播放流成功以后，如果要切换流播放 View 或者停止显示流画面，调用该 API 变更。
 
 @param view 播放视图
 @param streamID 流 ID
 @return true 调用成功，false 调用失败
 */
- (bool)updatePlayView:(ZEGOView *)view ofStream:(NSString *)streamID;

/**
 停止播放流
 
 * 主播停止流推后，会通过 [ZegoRoomDelegate -onStreamUpdated:streams:roomID:] 通知房间内用户流删除，用户需要调用此 API 停止播放流。停止播放流后，注意移除相关的UI控件
 
 @param streamID 要停止播放的流 ID
 @return true 调用成功，false 调用失败
 */
- (bool)stopPlayingStream:(NSString *)streamID;

/**
 设置拉流优先级的权重，被置为 focus 的流，优先保证其质量。

 @param streamID 流 ID，当为空时，恢复初始状态，所有流的权重相同。
 @return true: 设置成功, false 设置失败
 @attention 默认所有流的权重相同。
 @attention 只能设置一路流是高优先级的，以最后设置的为准。
 @attention 在本地网络不好的时候，保证focus流的同时，可能造成其他的卡顿更多。
 @attention 设置高优先级的流需要在开始拉流后设置，流停止之后自动恢复初始状态，所有流的权重相同。
 */
- (bool)setPlayStreamFocus:(NSString *)streamID;

/**
 设置多路混音时突出的拉流ID，当有多路流同时发声，将突出streamIDs中的流。

 @param mixMode ZEGOAPI_AUDIO_MIX_MODE_RAW时关闭该功能，ZEGOAPI_AUDIO_MIX_MODE_FOCUSED时开启
 @param streamIDs 需要突出的拉流ID，最多4个
 @return true: 设置成功, false 设置失败
 @attention 任意时刻调用有效，退出房间重置状态。
 @attention 当mixMode为ZEGOAPI_AUDIO_MIX_MODE_FOCUSED 且 num为0时，自动挑选突出最多4路流。
 @attention 当mixMode为ZEGOAPI_AUDIO_MIX_MODE_FOCUSED 且 所有设置的流都没在拉时，自动挑选突出最多4路流。
 */
- (bool)setAudioMixMode:(ZegoAPIAudioMixMode)mixMode ofStreams:(NSArray<NSString*> *)streamIDs;

/**
 设置拉的流的虚拟立体声源相对于本端的角度，可以实现空间音频效果。

 @param enable true 开启，false 关闭，默认关闭
 @param angle 虚拟立体声中声源的角度，范围为0～360，90为正前方，0对应正右方
 @param streamID 拉流ID
 @return true: 设置成功, false 设置失败
 */
- (bool)enablePlayVirtualStereo:(bool)enable angle:(int)angle ofStream:(NSString *)streamID;

/**
 请求连麦
 
 @param blk 回调 block
 @return true 成功，false 失败
 @discussion 不支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启）。
 @discussion 多房间模式下请使用 [ZegoLiveRoomApi requestJoinLive:responseBlock:]，指定房间 ID。
 @discussion 观众请求连麦后，主播会收到 [ZegoLivePublisherDelegate -onJoinLiveRequest:fromUserID:fromUserName:roomID:] 回调。连麦成功后，结果通过 blk 回调
 */
- (bool)requestJoinLive:(ZegoResponseBlock)blk;

/**
 请求连麦
 
 @param roomID 房间 ID
 @param blk 回调 block
 @return true 成功，false 失败
 @discussion 多房间模式下，必须指定房间 ID，否则调用失败。
 @discussion 观众请求连麦后，主播会收到 [ZegoLivePublisherDelegate -onJoinLiveRequest:fromUserID:fromUserName:roomID:] 回调。连麦成功后，结果通过 blk 回调
 */
- (bool)requestJoinLive:(NSString *)roomID responseBlock:(ZegoResponseBlock)blk;

/**
 回应主播端的邀请连麦请求
 
 @param seq 连麦请求序列号，标识当次连麦请求
 @param rspResult 回应，0 为同意
 @return true 成功，false 失败
 @discussion 不支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启）。
 @discussion 多房间模式下请使用 [ZegoLiveRoomApi respondInviteJoinLiveReq:result:room:]，指定房间 ID。
 @discussion 一般在 [ZegoLivePlayerDelegate -onInviteJoinLiveRequest:fromUserID:fromUserName:roomID:] 中调用本 API 回应邀请连麦请求
 */
- (bool)respondInviteJoinLiveReq:(int)seq result:(int)rspResult;

/**
 回应主播端的邀请连麦请求
 
 @param seq 连麦请求序列号，标识当次连麦请求
 @param rspResult 回应，0 为同意
 @return true 成功，false 失败
 @discussion 多房间模式下，必须指定房间 ID，否则调用失败。
 @discussion 一般在 [ZegoLivePlayerDelegate -onInviteJoinLiveRequest:fromUserID:fromUserName:roomID:] 中调用本 API 回应邀请连麦请求
 */
- (bool)respondInviteJoinLiveReq:(int)seq result:(int)rspResult room:(NSString *)roomID;

/**
 硬件解码
 
 * 注意：
 * 1. 如果要打开，必须在调用拉流接口 startPlayingStream 前设置。
 * 2. 打开硬编硬解开关需后台可控，避免碰到版本升级或者硬件升级时出现硬编硬解失败的问题。
 
 @param bRequired true 打开，false 关闭。默认 false
 @return true 成功，false 失败
 */
+ (bool)requireHardwareDecoder:(bool)bRequire;

/**
 为指定拉流通道开启/关闭硬件解码。

 * 注意：
 * 1. 必须在调用拉流前设置；
 * 2. 在反初始化 SDK 前，设置的值一直有效；
 * 3. 打开硬编硬解开关需后台可控，避免碰到版本升级或者硬件升级时出现硬编硬解失败的问题；
 * 4. 指定流的值会与全局值相互覆盖，具体取决于其调用顺序。

 @param enable true 打开，false 关闭。默认 false
 @param streamID 拉流 ID
 @return true 成功，false 失败
*/
- (bool)requireHardwareDecoder:(bool)bRequire ofStream:(NSString *)streamID;

/**
 （声音输出）扬声器开关
 
 * 注意：
 * 1. 设置为关闭后，默认扬声器和耳机均无声音输出。
 
 @param bEnable true 不静音，false 静音。默认 true
 @return true 成功，false 失败
 */
- (bool)enableSpeaker:(bool) bEnable;

/**
 默认扬声器开关
 
 * 注意：
 * 1.设置为关闭后，扬声器无声音，耳机仍有声音输出。
 * 2.在推流之前设置, 且 setAudioDeviceMode 设置为 ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION 或 ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION2 或 ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION3 或 ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION4 时有效。
 
 @param bOn true 打开，false 关闭。默认 true
 @return true 成功，false 失败
 */
- (bool)setBuiltInSpeakerOn:(bool)bOn;

/**
 设置拉流的播放音量
 
 * 注意：
 * 1. 必须在拉流 startPlayingStream 后调用才有效。
 * 2. 播放直播过程中可通过此 API 调整播放音量。
 
 @param volume 音量大小, 取值范围[0, 200], 默认 100
 @return true 成功, false 失败
 */
- (bool)setPlayVolume:(int)volume;

/**
 设置指定拉流的播放音量
 
 * 注意：
 * 1. 必须在拉流 startPlayingStream 后调用才有效，每次拉流都需要重新设置。
 * 2. 播放直播过程中可通过此 API 调整播放音量。
 
 @param volume 音量大小, 取值范围[0, 200], 默认 100
 @param streamID  流ID. ID为空时, 统一设置所有拉流的播放音量
 @return true 成功, false 失败
 */
- (bool)setPlayVolume:(int)volume ofStream:(NSString *)streamID;

/**
 获取当前播放视频的实时音量
 
 * 注意：
 * 1. 必须在拉流 startPlayingStream 后调用才有效。
 * 2.播放直播时通过此 API 获取当前音量。
 
 @param streamID 播放流 ID
 @return 视频的音量值
 */
- (float)getSoundLevelOfStream:(NSString *)streamID;

/**
 设置观看直播的 View 的模式
 
 * 注意：
 * 1. 必须在拉流 startPlayingStream 后调用才有效，每次拉流都需要重新设置。
 * 2. 一般在流播放、流新增、全屏切换等其他流尺寸可能变化的场合时调用。
 
 @param mode 模式，参考 ZegoVideoViewMode 定义。默认 ZegoVideoVideModeScaleAspectFit
 @param streamID 播放流 ID
 @return true 成功，false 失败
 */
- (bool)setViewMode:(ZegoVideoViewMode)mode ofStream:(NSString *)streamID;

/**
 设置播放渲染朝向
 
 * 注意：
 * 1. 必须在拉流 startPlayingStream 后调用才有效，每次拉流都需要重新设置。
 * 2. 一般用于全屏切换、旋转设备时调用，调整播放方向。
 
 @param rotate 逆时针旋转角度(0/90/180/270)。默认 0
 @param streamID 播放流 ID
 @return true 成功，false 失败
 */
- (bool)setViewRotation:(int)rotate ofStream:(NSString *)streamID;

/**
 开启播放镜像
 
 * 注意：
 * 1. 必须在拉流 startPlayingStream 后调用才有效, 每次拉流都需要重新设置

 @param enable true 开启, false 关闭, 默认 false
 @param streamID 播放流 ID
 @return true 成功，false 失败
 */
- (bool)enableViewMirror:(bool)enable ofStream:(NSString *)streamID;

/**
 设置视频控件的背景颜色
 
 * 注意：
 * 1. 必须在拉流 startPlayingStream 后调用才有效, 每次拉流都需要重新设置
 
 @param color 颜色,取值为0x00RRGGBB
 @param streamID 播放流 ID
 @return true 成功，false 失败
 */
- (bool)setViewBackgroundColor:(int)color ofStream:(NSString *)streamID;

/**
 对观看直播视图进行截图
 
 * 注意：
 * 1. 必须在拉流 startPlayingStream 后调用才有效。
 * 2. 截图成功后，通过回调 blk 返回结果。
 
 @param streamID 需要截图的流 ID
 @return true 调用成功，false 调用失败
 */
- (bool)takeSnapshotOfStream:(NSString *)streamID withCompletionBlock:(ZegoSnapshotCompletionBlock)blk;

/**
 拉流是否接收音频数据
 
 * 注意：
 * 1. 可以在拉流 startPlayingStream 之前或者之后调用, 每次拉流都需要重新设置

 @param streamID 播放流 ID
 @param active true 接收，false 不接收
 @return 0 成功，否则失败
 */
- (int)activateAudioPlayStream:(NSString *)streamID active:(bool)active;

/**
 拉流是否接收音频数据
 
 * 注意：
 * 1. 可以在initSDK之后调用，支持拉流时设置
 * 2. 本地用户会取消或恢复接收所有远端用户的音频流，包括在调用该方法后新加入房间的用户的音频流
 * 3. 必需此接口和activateAudioPlayStream接口均未禁止音频数据，才会接收音频数据
 * 4. 此接口不影响activateAudioPlayStream接口的生命周期

 @param active true 接收，false 不接收
 @return 0 成功，否则失败
 */
- (int)activateAllAudioPlayStream:(bool)active;

/**
 拉流是否接收视频数据
 
 * 注意：
 * 1. 可以在拉流 startPlayingStream 之前或者之后调用, 每次拉流都需要重新设置
 
 @param streamID 播放流 ID
 @param active true 接收，false 不接收
 @return 0 成功，否则失败
 */
- (int)activateVideoPlayStream:(NSString *)streamID active:(bool)active;

/**
 拉流是否接收视频数据
 
 * 注意：
 * 1. 可以在拉流 startPlayingStream 之前或者之后调用, 每次拉流都需要重新设置
 * 2. 当推流方开启了分层编码后，拉流方可以通过该 API 在适当时机选择拉取大分辨率图层或小分辨率图层。
 
 @param streamID 播放流 ID
 @param active true 接收，false 不接收
 @param videoLayer 视频分层类型
 @return 0 成功，否则失败
 */
- (int)activateVideoPlayStream:(NSString *)streamID active:(bool)active videoLayer:(VideoStreamLayer)videoLayer;

/**
 拉流是否接收视频数据
 
 * 注意：
 * 1. 可以在initSDK之后调用，支持拉流时设置
 * 2. 本地用户会取消或恢复接收所有远端用户的视频流，包括在调用该方法后新加入房间的用户的视频流
 * 3. 必需此接口和activateVideoPlayStream接口均未禁止视频数据，才会接收视频数据
 * 4. 此接口不影响activateVideoPlayStream接口的生命周期
 
 @param active true 接收，false 不接收
 @return 0 成功，否则失败
 */
- (int)activateAllVideoPlayStream:(bool)active;

/**
 设置拉流播放缓存自适应调整的区间范围（上下限）

 * 注意:
 * 1. 调用时机为拉流前或者拉流中, 每次拉流都需要重新设置
 * 2. 上下限在未设置时, 内部默认区间范围 [0, 4000]
 * 3. 当设置的上限小于下限时，会自动将上限取值为下限
 
 @param minBufferLevel 缓存自适应区间下限, 单位毫秒. 当 minBufferLevel <= 0 表示未设置下限.
 @param maxBufferLevel 缓存自适应区间上限, 单位毫秒. 当 maxBufferLevel <= 0 表示未设置上限. 当设置的上限超过4000时，会取值4000.
 @param streamID 流 ID
 @return true 设置成功, false 设置失败
 */
- (bool)setRecvMinBufferLevel:(int)minBufferLevel maxBufferLevel:(int)maxBufferLevel streamID:(NSString *)streamID;

/**
 设置拉流质量监控周期
 
 * 注意：
 * 1. 设置该 API 后，[ZegoLivePlayerDelegate -onPlayQualityUpdate:stream:videoFPS:videoBitrate:]  将会按照设置值的频率回调。
 
 @param timeInMS 时间周期，单位为毫秒，取值范围为(500, 60000)。默认为 3000
 */
+ (void)setPlayQualityMonitorCycle:(unsigned int)timeInMS;

/**
 音频录制开关
 
 @warning Deprecated，请使用 enableSelectedAudioRecord:
 @discussion 在启动推流或者启动本地录制（MediaRecorder）的时候，才能开启音频录制
 
 */
- (bool)enableSelectedAudioRecord:(unsigned int)mask sampleRate:(int)sampleRate;

/**
 开/关音频录制功能
 
 * 注意：
 * 1.必须在初始化 SDK 后调用。
 * 2.开启音频录制后，App 需要设置音频录制代理(-setAudioRecordDelegate:)，才能通过 -onAudioRecord:sampleRate:numOfChannels:bitDepth:type: 获取 SDK 抛出的音频数据。
 * 3.在启动推流或者启动本地录制（MediaRecorder）之后，才能开启音频录制。
 
 @param config 配置信息, 详细请参考 struct ZegoAPIAudioRecordConfig
 @return true 调用成功，false 调用失败
 */
- (bool)enableSelectedAudioRecord:(ZegoAPIAudioRecordConfig)config;

/**
 设置音频录制代理对象
 
 * 开启音频录制功能，需要设置代理对象。未设置代理对象，或对象设置错误，可能导致 App 无法正常收到 -onAudioRecord:sampleRate:numOfChannels:bitDepth:type: 回调。
 
 @param audioRecordDelegate 音频录制回调
 */
- (void)setAudioRecordDelegate:(id<ZegoLiveApiAudioRecordDelegate>)audioRecordDelegate;

/**
 获取 SDK 支持的最大同时播放流数
 
 @return 最大支持播放流数
 */
+ (int)getMaxPlayChannelCount;

/**
 帧顺序检测开关
 
 * 注意：
 * 1.必须在初始化 SDK 前调用
 
 @param enable true 检测帧顺序，不支持B帧； false 不检测帧顺序，支持B帧，可能出现短暂花屏
 */
+ (void)enableCheckPoc:(bool)enable;

/**
 获取发布指定流 ID 的发布者信息
 
 @attention 需保证该流已经存在
 
 @param streamID 流 ID
 @return ZegoUser 发布者信息
 */
- (ZegoUser *)getUserByStreamID:(NSString *)streamID;

/**
 获取是否支持指定解码能力
 
 * 注意：
 * 1. 必须在初始化 SDK 后调用

 @param codecId 解码ID
 @return true - 支持，false - 不支持
 @discussion 可以在拉流前调用本接口判断本机是否支持指定解码器
 */
- (bool)isVideoDecoderSupported:(ZegoVideoCodecAvc)codecID;

#if TARGET_OS_IPHONE
/**
 设置拉流的裁剪视图配置
 
 @attention 拉流前或者拉流过程中设置都有效，当次拉流有效。
 @param rect 裁剪坐标
 @param originalSize 原始图像尺寸
 @param streamID 流 ID
 */
- (void)setViewCropRect:(CGRect)rect originalSize:(CGSize)originalSize streamID:(NSString *)streamID;
#endif // TARGET_OS_IPHONE


/**
 设置拉流是否校准网络时间，用于有对齐属性的各路流进行时间对齐

 @param alignment 1 开启，0 关闭
 @return 0 表示成功，-1 为失败
 @attention InitSDK成功后才能调用本接口，可实时更改生效。
 */
- (int)setPlayStreamsAlignmentProperty:(int)alignment;

@end


@protocol ZegoLivePlayerDelegate <NSObject>

/**
 播放流状态更新通知
 
 * 调用 [ZegoLiveRoomApi (Player) -startPlayingStream:inView:] 或 [ZegoLiveRoomApi (Player) -startPlayingStream:inView:params:] 拉流成功后，会收到此回调通知。
 
 @param stateCode 状态码，0 为无错误。更多错误码详见 enum ZegoErrorCode
 @param streamID 播放流 ID
 */
- (void)onPlayStateUpdate:(int)stateCode streamID:(NSString *)streamID;

@optional

/**
 收到主播端的邀请连麦请求
 
 @param seq 连麦请求序列号，标识当次连麦请求
 @param userId 来源用户 ID
 @param userName 来源用户名
 @param roomID 房间 ID
 @discussion 主播端调用 [ZegoLiveRoomApi (Publisher) -inviteJoinLive:responseBlock:] 邀请观众连麦后，观众端会收到此通知。建议在此通知中，调用 [ZegoLiveRoomApi (Player) -respondInviteJoinLiveReq:result:] 回应邀请连麦请求
 */
- (void)onInviteJoinLiveRequest:(int)seq fromUserID:(NSString *)userId fromUserName:(NSString *)userName roomID:(NSString *)roomID;


/**
 收到结束连麦信令
 
 @param fromUserId 来源用户 ID
 @param fromUserName 来源用户名
 @param roomID 房间 ID
 */
- (void)onEndJoinLiveCommad:(NSString *)fromUserId userName:(NSString *)fromUserName roomID:(NSString *)roomID;

/**
 视频尺寸变更通知
 
 * 注意：
 * 1. 调用 startPlayingStream 之后，在 SDK 获取到第一帧数据时，会收到该 API 回调通知；
 * 2. 直播过程中，视频宽高发生变化，会收到该 API 回调通知。
 * 3. 从调用开始拉流接口到显示第一帧数据的过程中可能存在一个短暂的时间差（具体时长取决于当前的网络状态），推荐在进入直播页面时加载一张预览图以提升用户体验，然后在收到本回调后去掉预览图。
 
 @param size 视频大小
 @param streamID 发生视频尺寸变更的流 ID
 */
- (void)onVideoSizeChangedTo:(CGSize)size ofStream:(NSString *)streamID;

/**
 远端摄像头状态通知
 
 * 注意：
 * 1. 仅拉 UDP 流有效；
 * 2. 当房间内其他流的摄像头状态发生改变，如其他用户调用了 EnableCamera (true/false) 后，会收到该 API 回调通知。
 
 @param status 参考 zego-api-defines-oc.h 中 ZegoAPIDeviceStatus 的定义
 @param streamID 流的唯一标识
 @param reason 参考 zego-api-defines-oc.h 中 ZegoAPIDeviceErrorReason 的定义
 */
- (void)onRemoteCameraStatusUpdate:(int)status ofStream:(NSString *)streamID reason:(int)reason;

/**
 远端麦克风状态通知

 * 注意：
 * 1. 仅拉 UDP 流有效；
 * 2. 当房间内其他流的麦克风状态发生改变，如其他用户调用了 EnableMic (true/false) 后，会收到该 API 回调通知。 

 @param status 参考 zego-api-defines-oc.h 中 ZegoAPIDeviceStatus 的定义
 @param streamID 流的唯一标识
 @param reason 参考 zego-api-defines-oc.h 中 ZegoAPIDeviceErrorReason 的定义
 */
- (void)onRemoteMicStatusUpdate:(int)status ofStream:(NSString *)streamID reason:(int)reason;

/**
 远端麦克风状态通知

 * 注意：
 * 1. 仅拉 UDP 流有效；
 * 2. 当 status 为 Device_Closed 时，reason 表示原因，可能的取值有：ZEGOAPI_DEVICE_ERROR_REASON_GENERIC，ZEGOAPI_DEVICE_ERROR_REASON_DISABLED，ZEGOAPI_DEVICE_ERROR_REASON_MUTE，ZEGOAPI_DEVICE_ERROR_REASON_INTERRUPTION

 @param status 参考 zego-api-defines-oc.h 中 ZegoAPIDeviceStatus 的定义
 @param streamID 流的唯一标识
 @param reason 参考 zego-api-defines-oc.h 中 ZegoAPIDeviceErrorReason 的定义
 */
- (void)onRemoteSpeakerStatusUpdate:(int)status ofStream:(NSString *)streamID reason:(int)reason;

/**
 接收到远端音频的首帧通知
 
 * 注意：
 * 1. 当远端音频的首帧到达后，会收到该 API 回调通知。
 
 @param streamID 流的唯一标识
 */
- (void)onRecvRemoteAudioFirstFrame:(NSString *)streamID;

/**
 接收到远端视频的首帧通知
 
 * 注意：
 * 1. 当远端视频频的首帧到达后，会收到该 API 回调通知。
 * 2. 从调用开始拉流接口到显示第一帧数据的过程中可能存在一个短暂的时间差（具体时长取决于当前的网络状态），推荐在进入直播页面时加载一张预览图以提升用户体验，然后在收到本回调后去掉预览图。
 
 @param streamID 流的唯一标识
 */
- (void)onRecvRemoteVideoFirstFrame:(NSString *)streamID;

/**
 远端视频渲染首帧通知
 
 * 注意：
 * 1. 当开始要渲染远端的视频首帧时，会收到该 API 回调通知。
 
 @param streamID 渲染视频的流 ID
 */
- (void)onRenderRemoteVideoFirstFrame:(NSString *)streamID;

/**
 播放流质量更新通知
 
 * 注意：
 * 1. 拉流成功后会多次收到此回调通知；
 * 2. 默认回调频率为 3000ms 一次，通过 [ZegoLiveRoomApi (Player) setPlayQualityMonitorCycle] 可自定义回调频率。
 
 @param quality 0 ~ 3 分别对应优、良、中、差
 @param streamID 观看流ID
 @param fps 帧率(frame rate)
 @param kbs 码率(bit rate) kb/s
 */
- (void)onPlayQualityUpdate:(int)quality stream:(NSString *)streamID videoFPS:(double)fps videoBitrate:(double)kbs;

/**
 播放流质量更新通知
 
 * 注意：
 * 1. 拉流成功后会多次收到此回调通知；
 * 2. 默认回调频率为 3000ms 一次，通过 [ZegoLiveRoomApi (Player) setPlayQualityMonitorCycle] 可自定义回调频率。
 
 @param streamID 观看流 ID
 @param quality ZegoPlayQuality 对象，内部包含了各项质量数据
 */
- (void)onPlayQualityUpate:(NSString *)streamID quality:(ZegoApiPlayQuality)quality;

/**
 拉的所有rtc和l3流的统计数据更新回调
 
 @param stats ZegoAPIPlayStats 对象，内部包含了各项统计数据
 */
- (void)onPlayStatsUpdate:(ZegoAPIPlayStats *)stats;

/**
 视频解码失败回调
 @param codecID 编解码器 ID
 @param errorCode 错误码, 详见 zego-api-defines-oc.h 中 ZegoAPICodecError 的定义
 @param streamID 流 ID
*/
- (void)onVideoDecoder:(ZegoVideoCodecAvc)codecID error:(int)errorCode stream:(NSString*)streamID;

@end


@protocol ZegoLiveApiAudioRecordDelegate <NSObject>

@optional

/**
 音频录制回调
 
 * 1.开启音频录制(-enableSelectedAudioRecord:)并成功设置音频录制代理(-setAudioRecordDelegate:)对象后，SDK 会触发该回调，App 可通过此 API 获取 SDK 抛出的音频数据。用户可自行对数据进行处理，例如：存储等。
 * 2.存储数据时注意取 sampleRate、numOfChannels、bitDepth 参数写包头信息。退出房间或停止录制后，不会再收到该回调。
 
 @param audioData SDK 返回的音频源数据
 @param sampleRate 采样率，与 - enableSelectedAudioRecord: 参数中设置的值一致
 @param numOfChannels 通道数量，单通道
 @param bitDepth 位深度，16 bit
 @param type 音源类型，请参考 enum ZegoAPIAudioRecordMask
 */
- (void)onAudioRecord:(NSData *)audioData sampleRate:(int)sampleRate numOfChannels:(int)numOfChannels bitDepth:(int)bitDepth type:(unsigned int)type;

/**
 音频录制回调
 
 @warning Deprecated，请使用 onAudioRecord:sampleRate:numOfChannels:bitDepth:type:
 */
- (void)onAudioRecord:(NSData *)audioData sampleRate:(int)sampleRate numOfChannels:(int)numOfChannels bitDepth:(int)bitDepth __attribute__ ((deprecated));
@end

