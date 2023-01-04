//
//  ZegoLiveRoomApi-RangeAudio.h
//  ZegoLiveRoom
//
//  Copyright © 2022年 zego. All rights reserved.
//

@protocol ZegoRangeAudioDelegate;


/** 范围语音模式 */
typedef enum
{
    /** 面向所有人 */
    ZEGO_RANGE_AUDIO_MODE_WORLD = 1,  
    /** 仅小队 */
    ZEGO_RANGE_AUDIO_MODE_TEAM = 2, 
    /** 隐秘小队，可以接收到世界范围内的声音 */
    ZEGO_RANGE_AUDIO_MODE_SECRETE_TEAM =3, 
} ZegoRangeAudioMode;

/** 范围语音麦克风状态 */
typedef enum
{
    /** 正在打开中 */
    ZEGO_RANGE_AUDIO_MICROPHONE_STATE_OPENING = 1,
    /** 打开成功 */
    ZEGO_RANGE_AUDIO_MICROPHONE_STATE_SUCCESS = 2,
    /** 临时中断，正在重试中 */
    ZEGO_RANGE_AUDIO_MICROPHONE_STATE_TEMPBROKEN = 3,
    /** 打开失败 */
    ZEGO_RANGE_AUDIO_MICROPHONE_STATE_FAILED = 4,
} ZegoRangeAudioMicrophoneState;

@interface ZegoLiveRoomApi (RangeAudio)

/**
 设置范围语音代理对象
 
 * 注意：
 * 1.在 init 之前设置。
 
 @param rangeAudioDelegate 遵循 ZegoRangeAudioDelegate 协议的代理对象。
 @return 设置结果。true 成功，false 失败。
 */
- (bool)setRangeAudioDelegate:(id<ZegoRangeAudioDelegate> _Nullable) rangeAudioDelegate;

/**
 初始化范围语音。

 * 注意：
 * 1.在 InitSDK 之后调用才有效。
 * 2.不支持多房间 SDK。与 unInit 配对调用。
 
 @return true 成功，false 失败。
 */
- (bool)initRangeAudio;

/**
 反初始化范围语音。
 
 * 注意：
 * 1.在 init 之后调用。
 * 2.与InitRangeAudio 配对调用。
 
 */
- (void)unInitRangeAudio;

/**
 设置小队模式。
 
 * 如设置 ZEGO_RANGE_AUDIO_MODE_WORLD 则会听到自己小队内和房间内其它用户模式为 ZEGO_RANGE_AUDIO_MODE_WORLD ，且在距离我音频接收范围内的语音。
 * 如设置 ZEGO_RANGE_AUDIO_MODE_TEAM 则只能听到与自己同一小队的语音。
 * 默认值 : ZEGO_RANGE_AUDIO_MODE_WORLD
 
 * 注意：
 * 1.在 init 之后，unInit 之前调用，可随时变更。
 
 @param mode 详见 ZegoRangeAudioMode 。
 */
- (void)setRangeAudioMode:(ZegoRangeAudioMode)mode;

/**
 设置小队ID 。
 
 * 设置小队ID 设置后，将能听到与房间内的其他用户属于同一小队下的声音, 且不会随距离方向产生变化。
 
 * 注意：
 * 1.在 init 之后，unInit 之前调用，可随时变更。
 
 @param teamID 小队ID 。
 */
- (void)setRangeAudioTeamID:(NSString * _Nonnull)teamID;

/**
 设置音频的接收范围。
 
 * 设置范围后，在开启3D音效情况下，声音将会随距离衰减，直至超出设置范围，则不再有声音。
 * 小队内的语音，将不会受到此值的限制。

 * 注意：
 * 1.在 init 之后，unInit 之前调用，可随时变更。
 
 @param range 接受范围。
 */
- (void)setAudioRecvRange:(float)range;

/**
 开启麦克风。
 
 * 开启麦克风后，SDK将会自动使用主通道推音频流。

 * 注意：
 * 1.在 init 之后，unInit 之前调用。
 * 2.ZegoRangeAudioDelegate 中的 OnRangAudioMicrophone 回调查看开启麦克风结果。
 
 @param enable true 开启，false 关闭。
 */
- (void)enableRangeAudioMicrophone:(bool)enable;

/**
 开启扬声器。
 
 * 开启扬声器后，SDK将会自动使用主通道推音频流。
 * 若当前是仅小队模式，将只会拉取同一小队的音频流，停止非小队的音频流。
 * 若当前是世界模式, 将优先拉取同一小队的语音，非小队成员，则按距离我最近的的开始拉取。

 * 注意：
 * 1.在 init 之后，unInit 之前调用。
 
 @param enable true 开启，false 关闭。
 */
- (void)enableRangeAudioSpeaker:(bool)enable;

/**
 开启 3D 空间音效。
 
 * 开启后，在房间内非小队成员的音频，会随离我的距离，和方向产生空间变化。

 * 注意：
 * 1.在 init 之后，unInit 之前调用。
 
 @param enable true 开启，false 关闭。
 */
- (void)enableSpatializer:(bool)enable;

/**
 更新房间内的其它用户(音频源)位置。

 * 注意：
 * 1.在 init 之后，unInit 之前调用。
 
 @param userID 房间内其它用户的 userid。
 @param position 用户的三维空间坐标 顺序为前,右,上。
 */
- (void)updateAudioSource:(NSString * _Nonnull)userID position:(float[_Nonnull 3])position;

/**
 更新自己的位置。

 * 注意：
 * 1.在 init 之后，unInit 之前调用。
 
 @param position 自己的三维空间坐标，顺序为前,右,上。
 @param axisForward 前朝向的单位向量。
 @param axisRight 右朝向的单位向量。
 @param axisUp  上朝向的单位向量。
 */
- (void)updateSelfPosition:(float[_Nonnull 3])position axisForward:(float[_Nonnull 3])axisForward
                 axisRight:(float[_Nonnull 3])axisRight
                    axisUp:(float[_Nonnull 3])axisUp;

/**
 是否可接收指定用户音频数据。

 * 注意：
 * 1.在 init 之后，unInit 之前调用。
 
 @param userID 指定用户 ID，设置是否接受该用户的音频数据，默认接收。
 @param mute true 表示不接收，false表示接收。
 */
- (void)muteRangeAudioUser:(NSString * _Nonnull)userID mute:(bool)mute;

/**
 设置范围语音位置更新频率。

 * 注意：
 * 1.在 init 之后，unInit 之前调用。
 
 @param frequency 单位ms 最小为15ms  默认100ms。
 @attention 调用该接口后，SDK内部将已 frequency 为时间间隔, 定期更新UpdateSelfPosition 和UpdateAudioSource 设置的位置值。
 */
- (void)setPositionUpdateFrequency:(int)frequency;

/**
 设置范围语音音量。

 * 注意：
 * 1.在 init 之后，unInit 之前调用。
 
 @param volume 音量 默认100。[0,200].
 */
- (void)setRangeAudioVolume:(int)volume;

/**
 设置流的发声范围, 注意该流为调用StartPlayStream自定义的流。

 * 注意：
 * 1.在 init 之后，且StartPlayStream 之后 。
 
 @param streamID 流id。
 @param vocalRange 发声范围。
 */
- (void)updateStreamVocalRange:(NSString * _Nonnull)streamID vocalRange:(float)vocalRange;

/**
 设置流的位置, 注意该流为调用StartPlayStream自定义的流。

 * 注意：
 * 1.在 init 之后，且StartPlayStream 之后 。
 
 @param streamID 流id。
 @param position 用户的三维空间坐标 顺序为前,右,上。
 */
- (void)updateStreamPosition:(NSString * _Nonnull)streamID position:(float[_Nonnull 3])position;

@end


/**
 范围语音代理协议。
 */
@protocol ZegoRangeAudioDelegate <NSObject>

@optional

/**
 范围语音开启麦克风回调。

 * 开启麦克风时自己推流时会回调此结果。
 
 @param errorCode 
 @param state  麦克风状态，参见 ZegoRangeAudioMicrophoneState 。
 */
- (void)OnRangAudioMicrophone:(ZegoRangeAudioMicrophoneState)state errorCode:(int)errorCode;

@end
