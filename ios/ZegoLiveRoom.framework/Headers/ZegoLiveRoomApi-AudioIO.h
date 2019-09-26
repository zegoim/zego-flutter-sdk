//
//  ZegoLiveRoomApi-AudioIO.h
//  ZegoLiveRoom
//
//  Copyright © 2017年 zego. All rights reserved.
//

#ifndef ZegoLiveRoomApi_AudioIO_h
#define ZegoLiveRoomApi_AudioIO_h

#import "ZegoLiveRoomApi.h"
#import "ZegoAudioCapture.h"

@interface ZegoLiveRoomApi (AudioIO)

#if TARGET_OS_IPHONE

/**
 启用外部音频采集
 
 @warning Deprecated，请使用 [ZegoExternalAudioDevice enableExternalAudioDevice:]
 */
+ (void)enableExternalAudioDevice:(bool)enable __attribute__ ((deprecated));

/**
 获取 IAudioDataInOutput 对象
 
 @warning Deprecated，请使用 [ZegoExternalAudioDevice getIAudioDataInOutput]
 */
- (AVE::IAudioDataInOutput *)getIAudioDataInOutput __attribute__ ((deprecated));

/**
 是否在耳机插入状态下启用硬件回声消除
 
 * 建议在推流前调用设置。
 
 * 注意：
 * 1.只在 [ZegoLiveRoomApi(Publisher) +setAudioDeviceMode:] 设置了音频设备模式为 Auto, Communication 时，使用耳机状态的硬件回音消除才会生效。
 * 2.iOS 的 SDK 默认使用开启，非 iOS 的 SDK 默认关闭。
 
 @param enable 是否启用
 */
- (void)enableAECWhenHeadsetDetected:(bool)enable;

#endif

/**
 设置音频前处理函数
 
 @warning Deprecated，请使用 [ZegoLiveRoomApi (AudioIO) setAudioPrep2:dataCallback:]
 */
+ (void)setAudioPrep:(void(*)(const short* inData, int inSamples, int sampleRate, short *outData))prep;

/**
 对音频进行前处理
 
 * 当开发者需要自处理音频数据时采用此接口，一般是实现特殊功能如变声；SDK 会在 callback 参数中抛出采集的音频数据，开发者从其中获取音频数据进行变声等处理，最后将处理后的数据回传给 SDK。
 * 注意：
 * 1.此 API 必须在初始化 SDK 前调用。 
 * 2.SDK 已经针对采集的音频数据进行了回声消除、噪声抑制等处理，通常情况下，开发者无需再重复处理。
 
 @param prepSet 预处理音频数据的采样率等设置参数，详见 struct ExtPrepSet。
 @param callback 采样数据回调，inFrame 为 SDK 采集的音频数据，outFrame 为开发者处理后应该返回给 SDK 的数据。
 */
+ (void)setAudioPrep2:(AVE::ExtPrepSet)prepSet dataCallback:(void(*)(const AVE::AudioFrame& inFrame, AVE::AudioFrame& outFrame))callback;

@end

#endif /* ZegoLiveRoomApi_AudioIO_h */
