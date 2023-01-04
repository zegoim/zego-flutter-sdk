//
//  zego-api-audio-observer-oc.h
//  ZegoLiveRoom
//
//  Created by arvin on 2021/3/20.
//
#ifndef zego_api_audio_observer_oc_h
#define zego_api_audio_observer_oc_h

#import <Foundation/Foundation.h>
#include "zego-api-defines-oc.h"

/** 音频源类型 */
typedef enum : NSUInteger
{
    /** 采集音频 */
    ZEGOAPI_AUDIO_OBSERVER_CAPTURE       = 0x01,
    /** 播放音频 */
    ZEGOAPI_AUDIO_OBSERVER_PLAYBACK      = 0x02,
    /** 采集与播放混音音频 */
    ZEGOAPI_AUDIO_OBSERVER_MIX           = 0x04,
    
} ZegoAPIAudioObserverSource;

@protocol ZegoAudioObserverDelegate <NSObject>
@required
/**
 错误回调
 
 * 设置了音频观察器代理(-setAudioObserverDelegage:)，启动音频观察器出现错误时，能收到此回调。
 
 @param errorCode 错误码
 */
- (void)onAudioObserverError:(int)errorCode;

@optional
/**
 采集音频数据回调
 
 * 设置了音频观察器代理(-setAudioObserverDelegage:)，在音频观察器启动成功后，如果监听了采集音频，会不断收到此回调。
 * 同步回调，不能做耗时操作。拷贝数据后，切换线程再处理数据。
 
 @param data 音频数据，PCM格式
 @param sampleRate 采样率，不固定，以当前值为准
 @param numOfChannels 声道数量，单通道
 @param bitDepth 位深度，16 bit
 */
- (void)onCapturedAudioData:(NSData *)data sampleRate:(int)sampleRate numOfChannels:(int)numOfChannels bitDepth:(int)bitDepth;

/**
 播放音频数据回调
 
 * 设置了音频观察器代理(-setAudioObserverDelegage:)，在音频观察器启动成功后，如果监听了播放音频，会不断收到此回调。
 * 同步回调，不能做耗时操作。拷贝数据后，切换线程再处理数据。
 
 @param data 音频数据，PCM格式
 @param sampleRate 采样率，不固定，以当前值为准
 @param numOfChannels 声道数量，单通道
 @param bitDepth 位深度，16 bit
 */
- (void)onPlaybackAudioData:(NSData *)data sampleRate:(int)sampleRate numOfChannels:(int)numOfChannels bitDepth:(int)bitDepth;

/**
 采集与播放混音后的音频数据回调
 
 * 设置了音频观察器代理(-setAudioObserverDelegage:)，在音频观察器启动成功后，如果监听了混合音频，会不断收到此回调。
 * 同步回调，不能做耗时操作。拷贝数据后，切换线程再处理数据。
 
 @param data 音频数据，PCM格式
 @param sampleRate 采样率，不固定，以当前值为准
 @param numOfChannels 声道数量，单通道
 @param bitDepth 位深度，16 bit
 */
- (void)onMixAudioData:(NSData *)data sampleRate:(int)sampleRate numOfChannels:(int)numOfChannels bitDepth:(int)bitDepth;

@end

/**
 音频观察器，旁路回调相应的音频数据，该数据已经过3A处理，方便用户对相关音频数据进行录制、分析等自定义处理。
 */
@interface ZegoAudioObserver : NSObject

/**
 启动音频观察器
 
 * 1.必须在初始化 SDK之后调用。
 * 2.该模块与enableSelectedAudioRecord:不兼容
 * 3.当source包含 ZEGOAPI_AUDIO_OBSERVER_CAPTURE 时，对应音频数据通过 onCapturedAudioData 获取
 * 4.当source包含 ZEGOAPI_AUDIO_OBSERVER_PLAYBACK 时，对应音频数据通过 onPlaybackAudioData 获取
 * 5.当source包含 ZEGOAPI_AUDIO_OBSERVER_MIX 时，对应音频数据通过 onMixAudioData 获取
 
 @param source 关注的音频源类型，只有关注了该类型的音频才会有相应的回调，详见ZegoAPIAudioObserverSource
 @param sampleRate 音频采样率，包括8000, 16000, 22050, 24000, 32000, 44100, 48000
 @param numOfChannels 声道数，1：单声道，2：双声道
 @param return true 调用成功，false 调用失败
 @see - stopAudioObserver:
 */
- (BOOL)startAudioObserver:(int)source sampleRate:(int)sampleRate numOfChannels:(int)numOfChannels;

/**
 停止音频观察器，不会再回调相应的音频数据

 */
- (void)stopAudioObserver;

/**
 设置音频观察器代理对象
 
 * 设置音频观察器代理后，在启动音频观察器（startAudioObserver）之后才能收到相应类型的音频回调。
 
 @param delegate 音频观察器代理，详见 ZegoAudioObserverDelegate
 */
- (void)setAudioObserverDelegate:(id<ZegoAudioObserverDelegate>)delegate;

@end

#endif
