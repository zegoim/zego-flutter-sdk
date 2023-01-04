#ifndef zego_api_audio_processing_oc_h
#define zego_api_audio_processing_oc_h

#import <Foundation/Foundation.h>

#import "zego-api-defines-oc.h"

/** 音频混响模式
 * @deprecated 参考 ZegoAPIVoiceReverbType
*/
typedef NS_ENUM(NSUInteger, ZegoAPIAudioReverbMode)
{
    ZEGOAPI_AUDIO_REVERB_MODE_SOFT_ROOM = 0,
    ZEGOAPI_AUDIO_REVERB_MODE_WARM_CLUB = 1,
    ZEGOAPI_AUDIO_REVERB_MODE_CONCERT_HALL = 2,
    ZEGOAPI_AUDIO_REVERB_MODE_LARGE_AUDITORIUM = 3,
};

/**
 预设音频混响效果
 */
typedef NS_ENUM(NSUInteger, ZegoAPIVoiceReverbType)
{
    /** 关闭混响 */
    ZEGO_AUDIO_REVERB_TYPE_OFF = 0,
    /** 房间模式 */
    ZEGO_AUDIO_REVERB_TYPE_SOFT_ROOM = 1,
    /** 俱乐部（大房间）模式 */
    ZEGO_AUDIO_REVERB_TYPE_WARM_CLUB = 2,
    /** 音乐厅模式 */
    ZEGO_AUDIO_REVERB_TYPE_CONCERT_HALL = 3,
    /** 大教堂模式 */
    ZEGO_AUDIO_REVERB_TYPE_LARGE_AUDITORIUM = 4,
    /** 录音棚 */
    ZEGO_AUDIO_REVERB_TYPE_RECORDING_STUDIO = 5,
    /** 地下室 */
    ZEGO_AUDIO_REVERB_TYPE_BASEMENT = 6,
    /** KTV */
    ZEGO_AUDIO_REVERB_TYPE_KTV = 7,
    /** 流行 */
    ZEGO_AUDIO_REVERB_TYPE_POPULAR = 8,
    /** 摇滚 */
    ZEGO_AUDIO_REVERB_TYPE_ROCK = 9,
    /** 演唱会 */
    ZEGO_AUDIO_REVERB_TYPE_VOCAL_CONCERT = 10,
    /** 嘻哈 */
    ZEGO_AUDIO_REVERB_TYPE_HIPHOP = 11,
    /** 飘渺（空旷）*/
    ZEGO_AUDIO_REVERB_TYPE_MISTY = 12,
    /** 3D人声 */
    ZEGO_AUDIO_REVERB_TYPE_3DVOICE = 13,
    /** 留声机 */
    ZEGO_AUDIO_REVERB_TYPE_GRAMOPHONE = 14,
};

/**
 预设变声器类型
 */
typedef NS_ENUM(NSUInteger, ZegoAPIVoiceChangerType)
{
    /** 恢复原声 */
    ZEGOAPI_VOICE_CHANGER_TYPE_CHANGER_OFF = 0,
    /** 汽车人(擎天柱)音效 */
    ZEGOAPI_VOICE_CHANGER_TYPE_OPTIMUS_PRIME = 1,
    /** AI 机器人音效 */
    ZEGOAPI_VOICE_CHANGER_TYPE_AI_ROBOT = 2,
    /** 外国人音效 */
    ZEGOAPI_VOICE_CHANGER_TYPE_FOREIGNER = 3,
    /** 空灵音效 */
    ZEGOAPI_VOICE_CHANGER_TYPE_ELUSIVE = 4,
    /** 磁性男声音效 */
    ZEGOAPI_VOICE_CHANGER_TYPE_MALE_MAGNETIC = 5,
    /** 清新女声音效 */
    ZEGOAPI_VOICE_CHANGER_TYPE_FEMALE_FRESH = 6,
    /** 男声变童声 */
    ZEGOAPI_VOICE_CHANGER_TYPE_MEN_TO_CHILD = 7,
    /** 男声变女声 */
    ZEGOAPI_VOICE_CHANGER_TYPE_MEN_TO_WOMEN = 8,
    /** 女声变童声 */
    ZEGOAPI_VOICE_CHANGER_TYPE_WOMEN_TO_CHILD = 9,
    /** 女声变男声 */
    ZEGOAPI_VOICE_CHANGER_TYPE_WOMEN_TO_MEN = 10,
    /** 女活力 */
    ZEGOAPI_VOICE_CHANGER_TYPE_FEMALE_ENERGETIC = 11,
    /** 浑厚 */
    ZEGOAPI_VOICE_CHANGER_TYPE_RICHNESS = 12,
    /** 低沉 */
    ZEGOAPI_VOICE_CHANGER_TYPE_MUFFLED = 13,
    /** 圆润 */
    ZEGOAPI_VOICE_CHANGER_TYPE_ROUNDNESS = 14,
    /** 假音 */
    ZEGOAPI_VOICE_CHANGER_TYPE_DICTIONARIES = 15,
    /** 饱满 */
    ZEGOAPI_VOICE_CHANGER_TYPE_FULLNESS = 16,
    /** 清澈 */
    ZEGOAPI_VOICE_CHANGER_TYPE_CLEAR = 17,
    /** 高亢 */
    ZEGOAPI_VOICE_CHANGER_TYPE_HIGHLY_RESONANT = 18,
    /** 嘹亮 */
    ZEGOAPI_VOICE_CHANGER_TYPE_LOUD_CLEAR = 19,
    /** 小黄人 */
    ZEGOAPI_VOICE_CHANGER_TYPE_MINIONS = 20,
    /** C大调电音 */
    ZEGOAPI_VOICE_CHANGER_TYPE_MAJOR_C = 21,
    /** A小调电音 */
    ZEGOAPI_VOICE_CHANGER_TYPE_MINOR_A = 22,
    /** 和声小调电音 */
    ZEGOAPI_VOICE_CHANGER_TYPE_HARMONIC_MINOR = 23,
};

/**
 电音的调式
 */
typedef NS_ENUM(NSUInteger, ZegoAPIElectronicEffectsMode)
{
    /** 大调 */
    ZEGOAPI_ELECTRONIC_EFFECTS_MODE_MAJOR = 0,
    /** 小调 */
    ZEGOAPI_ELECTRONIC_EFFECTS_MODE_MINOR = 1,
    /** 和声小调 */
    ZEGOAPI_ELECTRONIC_EFFECTS_MODE_HARMONIC_MINOR = 2,
};

/**
 无变声音效 值: 0.0
  @deprecated 参考 setVoicePreset:ZEGOAPI_VOICE_CHANGER_TYPE_CHANGER_OFF
 */
ZEGO_EXTERN float const ZEGOAPI_VOICE_CHANGER_NONE;

/**
 女声变男声 值: -3.0
  @deprecated 参考 setVoicePreset:ZEGOAPI_VOICE_CHANGER_TYPE_WOMEN_TO_MEN
 */
ZEGO_EXTERN float const ZEGOAPI_VOICE_CHANGER_WOMEN_TO_MEN;
/**
 男声变女声 值: 4.0
  @deprecated 参考 setVoicePreset:ZEGOAPI_VOICE_CHANGER_TYPE_MEN_TO_WOMEN
 */
ZEGO_EXTERN float const ZEGOAPI_VOICE_CHANGER_MEN_TO_WOMEN;
/**
 女声变童声 值: 6.0
  @deprecated 参考 setVoicePreset:ZEGOAPI_VOICE_CHANGER_TYPE_WOMEN_TO_CHILD
 */
ZEGO_EXTERN float const ZEGOAPI_VOICE_CHANGER_WOMEN_TO_CHILD;
/**
 男声变童声 值: 8.0
  @deprecated 参考 setVoicePreset:ZEGOAPI_VOICE_CHANGER_TYPE_MEN_TO_CHILD
 */
ZEGO_EXTERN float const ZEGOAPI_VOICE_CHANGER_MEN_TO_CHILD;

/**
 音频混响参数
 */
typedef struct ZegoAudioReverbParam
{
    /** 房间大小，取值范围[0.0, 1.0]，用于控制产生混响"房间"的大小，房间越大，混响越强 */
    float roomSize;
    
    /** 余响，取值范围[0.0, 0.5]，用于控制混响的拖尾长度 */
    float reverberance;
    
    /** 混响阻尼， 取值范围[0.0， 2.0]，控制混响的衰减程度，阻尼越大，衰减越大 */
    float damping;
    
    /** 干湿比，取值范围 >= 0.0。 控制混响与直达声和早期反射声之间的比例，干(dry)的部分默认定为1，当干湿比设为较小时，湿(wet)的比例较大，此时混响较强 */
    float dryWetRatio;
} ZegoAudioReverbParam;

typedef struct ZegoAudioAdvancedReverbParam
{
    /** Room Size (%): [0,100].Sets the size of the simulated room. 0% is like a closet, 100% is like a huge cathedral or large auditorium. A high value will simulate the reverberation effect of a large room and a low value will simulate the effect of a small room. */
    float roomSize;
    /** Pre-delay (ms):[0~200]. Delays the onset of the reverberation for the set time after the start of the original input. This also delays the onset of the reverb tail. Careful adjustment of this parameter can improve the clarity of the result. */
    float preDelay;
    /** Reverberance (%):[0,100]. Sets the length of the reverberation tail. This determines how long the reverberation continues for after the original sound being reverbed comes to an end, and so simulates the "liveliness" of the room acoustics. For any given reverberance value, the tail will be greater for larger room sizes. */
    float reverberance;
    /** Damping (%):[0,100]. Increasing the damping produces a more "muted" effect. The reverberation does not build up as much, and the high frequencies decay faster than the low frequencies. Simulates the absorption of high frequencies in the reverberation. */
    float hfDamping;
    /** ToneLow(%):[0,100]. Setting this control below 100% reduces the low frequency components of the reverberation, creating a less "boomy" effect. */
    float toneLow;
    /** ToneHigh(%):[0,100].Setting this control below 100% reduces the high frequency components of the reverberation, creating a less "bright" effect. */
    float toneHigh;
    /** Wet Gain (dB):[-20,10]. Applies volume adjustment to the reverberation ("wet") component in the mix. Increasing this value relative to the "Dry Gain" (below) increases the strength of the reverb. */
    float wetGain;
    /** Dry Gain (dB):[-20,10]. Applies volume adjustment to the original ("dry") audio in the mix. Increasing this value relative to the "Wet Gain" (above) reduces the strength of the reverb. If the Wet Gain and Dry Gain values are the same, then the mix of wet effect and dry audio to be output to the track will be made louder or softer by exactly this value. */
    float dryGain;
    /** Stereo Width (%): [0,100]. Sets the apparent "width" of the Reverb effect for stereo tracks only. Increasing this value applies more variation between left and right channels, creating a more "spacious" effect. When set at zero, the effect is applied independently to left and right channels. */
    float stereoWidth;
    /** Wet Only */
    bool  wetOnly;
} ZegoAudioAdvancedReverbParam;

/**
 音频回声参数
 */
typedef struct ZegoReverbEchoParam
{
    /** 回声数，取值范围[0, 7] */
    int   numDelays;
    
    /** 输入音频信号的增益，取值范围[0.0, 1.0] */
    float inGain;
    
    /** 输出音频信号的增益，取值范围[0.0, 1.0] */
    float outGain;
    
    /** 回声信号分别的延时，取值范围[0, 5000] ms */
    int   delay[7];

    /** 回声信号分别的增益值，取值范围[0.0, 1.0] */
    float decay[7];
} ZegoReverbEchoParam;

@interface ZegoAudioProcessing : NSObject

/**
 设置虚拟立体声
 
 @param enable true 开启，false 关闭
 @param angle 虚拟立体声中声源的角度，范围为 -1 ~ 360，90 为正前方，0 / 180 / 270 分别对应最右边 / 最左边 / 正后方；特别的，设置 -1 时为全方位虚拟立体声效果。
 @return true 成功，false 失败
 @discussion 必须在初始化 SDK 后调用，并且需要设置双声道(参考 setAudioChannelCount)，
             推流成功后动态设置不同的 angle 都会生效
 */
+ (bool)enableVirtualStereo:(bool)enable angle:(int)angle;

/**
 设置音频混响
 
 @param enable true 开启，false 关闭
 @param mode 混响模式，参考 ZegoAPIAudioReverbMode
 @return true 成功，false 失败
 @discussion 必须在初始化 SDK 后调用，推流成功后动态设置不同的 mode 都会生效
 @deprecated 请使用 setReverbPreset:(ZegoAPIVoiceReverbType)type;
 */
+ (bool)enableReverb:(bool)enable mode:(ZegoAPIAudioReverbMode)mode;


/**
 设置音频混响参数

 @param roomSize 房间大小，取值范围 0.0 ~ 1.0。 值越大说明混响时间越长，产生的混响拖尾效应越大
 @param dryWetRatio 干湿比，取值范围 >= 0.0。 干湿比的值越小，wet的值越大，产生的混响效果越大
 @return true 成功，false 失败
 @discussion 任意一个参数设置为0.0时，混响关闭
 @discussion 必须在初始化 SDK 后调用，推流成功后动态设置不同的参数都会生效
 */
+ (bool)setReverbParam:(float)roomSize dryWetRatio:(float)dryWetRatio;


/**
 设置音频混响参数

 @param param 混响参数，各个参数含义参考 ZegoAudioReverbParam 的定义
 @return true 成功，false 失败
 @discussion 必须在初始化 SDK 后调用，推流成功后动态设置不同的 param 都会生效
 */
+ (bool)setReverbParam:(ZegoAudioReverbParam)param;

/**
 设置音频混响参数以获得不同的混响效果

 @param enable true 开启混响效果，false 关闭混响效果
 @param config 混响参数，各个参数含义参考 ZegoAudioAdvancedReverbParam
 @return true 设置成功，false 设置失败
 @discussion 必须在初始化 SDK 后调用，推流成功后动态设置不同的 config 都会生效
 @discussion 仅在 enable 为 true 时, config 值才有效
 */
+ (bool)setAdvancedReverbParam:(bool)enable config:(ZegoAudioAdvancedReverbParam)config;

/**
 设置预设的音频混响效果

 @param type 预设的混响效果，参考 ZegoAPIVoiceReverbType
 @return true 成功，false 失败
 @discussion 必须在初始化 SDK 后调用，推流成功后动态设置不同的 type 都会生效
 */
+ (bool)setReverbPreset:(ZegoAPIVoiceReverbType)type;

/**
 设置变声器参数，用于调整声调
 
 @param param 变声器参数
 @return true 成功，false 失败
 @discussion 必须在初始化 SDK 后调用
 @discussion 变声音效只针对采集的声音有效
 @discussion 取值[-12.0, 12.0]，几种典型的变声音效(男变女，女变男等)定义见上文
 */
+ (bool)setVoiceChangerParam:(float)param;

/**
 设置回声参数
 
 @param param 回声参数，各个参数含义参考 ZegoReverbEchoParam 的定义
 @return true 成功，false 失败
 @discussion 必须在初始化 SDK 后调用，支持推流成功后动态设置
 */
+ (bool)setReverbEchoParam:(ZegoReverbEchoParam)param;

/**
 预设变声配置
 
 @param type 变声器类型，参考 ZegoAPIVoiceChangerType
 @return true 成功，false 失败
 @discussion 必须在初始化 SDK 后调用
 @discussion 变声音效只针对采集的声音有效
 @discussion 该接口的效果可由SetReverbParam、SetVoiceChangerParam、SetReverbEchoParam组合实现，
             调用该接口后，若想有单独调用上述接口的效果，需先调用
             SetVoicePreset(ZEGO_VOICE_CHANGER_TYPE_CHANGER_OFF)还原配置
 */
+ (bool)setVoicePreset:(ZegoAPIVoiceChangerType)type;

/**
 调整音效均衡器参数

 @param bandGain 取值范围[-15, 15]。默认值是0，如果所有频带的增益值全部为0，则会关闭EQ功能
 @param bandIndex 取值范围[0, 9]。分别对应10个频带，其中心频率分别是[31, 62, 125, 250, 500, 1K, 2K, 4K, 8K, 16K]Hz
 @return true 调用成功，false 调用失败
 @attention 在 InitSDK 之后调用有效。使用此接口前请与即构技术支持联系确认是否支持此功能。
 */
+ (bool)setAudioEqualizerGain:(float)bandGain index:(int)bandIndex;

/**
 设置电音配置
 
 @param enable true 开启，false 关闭，默认关闭
 @param mode 电音的调式，参考 ZegoAPIElectronicEffectsMode，默认大调
 @param tonal 电音在某个调式下的起始音高，代表声音一个八度的12个半音，取值范围 [0, 11], 默认0
 @return true 成功，false 失败
 @attention 支持版本：6.4.0 及以上。
 @attention 在 InitSDK 之后调用有效。使用此接口前请与即构技术支持联系确认是否支持此功能。
 */
+ (bool)setElectronicEffects:(bool)enable mode:(ZegoAPIElectronicEffectsMode)mode tonal:(int)tonal;

@end

#endif /* zego_api_audio_processing_oc_h */
