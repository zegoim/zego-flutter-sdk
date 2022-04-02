//
//  ZegoLiveRoomApi-ReplayLive.h
//  ZegoLiveRoom
//
//  Copyright © 2017年 zego. All rights reserved.
//

#import "ZegoLiveRoomApi.h"
#import <ReplayKit/ReplayKit.h>

@interface ZegoLiveRoomApi (ReplayLive)

/**
 初始化 ReplayLive
 
 @discussion 必须在 InitSDK 前调用
 */
+ (void)prepareReplayLiveCapture;

/**
  设置屏幕共享 Mic 音频音量
 
 @discussion 必须在 startPreview 或 startPublishing 成功后调用
 @param volume 有效范围 [0, 200]，默认值 100。
 */
- (void)setReplayLiveMicVolume:(int)volume;

/**
  设置屏幕共享 App 音频音量
 
 @discussion 必须在 startPreview 或 startPublishing 成功后调用
 @param volume 有效范围 [0, 200]，默认值 100。
 */
- (void)setReplayLiveAppVolume:(int)volume;

/**
 处理视频数据
 
 @param sampleBuffer ReplayLiveKit 返回的视频数据
 */
- (void)handleVideoInputSampleBuffer:(CMSampleBufferRef)sampleBuffer;

/**
 处理视频数据
 
 @param sampleBuffer ReplayLiveKit 返回的视频数据
 @param time 透传给SDK的时间戳
 */
- (void)handleVideoInputSampleBuffer:(CMSampleBufferRef)sampleBuffer timeStamp:(CMTime)timeStamp;

/**
 处理音频数据
 
 @param sampleBuffer ReplayLiveKit 返回的音频数据
 @param sampleBufferType 类型 RPSampleBufferTypeAudioApp, RPSampleBufferTypeAudioMic
 */
- (void)handleAudioInputSampleBuffer:(CMSampleBufferRef)sampleBuffer withType:(RPSampleBufferType)sampleBufferType;

@end
