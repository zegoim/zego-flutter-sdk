//
//  zego-api-audio-frame-oc.h
//  ZegoLiveRoom
//

#import <Foundation/Foundation.h>

/** 音频帧类型 */
typedef enum ZegoAPIAudioFrameType
{
    kZegoAPIAudioFrameTypePCM          = 0x1001,   /** PCM 数据 */
    kZegoAPIAudioFrameTypeAACStream    = 0x1003,   /** AAC 编码数据 */
} ZegoAPIAudioFrameType;

/**
 音频帧结构
 */
@interface ZegoAPIAudioFrame : NSObject

/**
 设置采集的采样率

 @param sampleRate 采样率，PCM 和 AAC 帧的可设置的值为 [8000，16000，22050，24000，32000，44100，48000]
 @param channels 通道数，支持 1(单声道) 或 2(立体声)
 @return 详见 enum ZegoAPIErrorCode
 */
- (int)setSampleRate:(int)sampleRate channels:(int)channels;

/**
 设置采集到的数据和其采样数

 * 注意：仅支持 16 比特量化深度
 @param data 采集到的数据，注意：音频数据传给 SDK 后，需要由调用方管理其生命周期。
 @param samples 采样数，若是 PCM 帧，此值是此帧包含的 PCM 采样数；若是 AAC 帧，此值是 AAC 编码一帧所需要的采样数，可设置的值为[480,512,1024,1960,2048]
 @return 详见 enum ZegoAPIErrorCode
 */
- (int)setCapturedData:(unsigned char*)data samples:(int)samples;

/**
 设置帧数据类型

 @param type 帧数据类型，详见 enum ZegoAPIAudioFrameType，默认是 kZegoAudioFrameTypePCM。
 @return 详见 enum ZegoAPIErrorCode
 */
- (int)setFrameType:(ZegoAPIAudioFrameType)type;

/**
 设置 AAC 编码数据属性

 @param timestamp 编码时间戳，如果数据中只有 AAC 配置信息(specialConfig)，填 0
 @param dataLength 总数据长度，注意 dataLength = AAC 编码结果长度 + specialConfigLength
 @param specialConfigLength AAC 配置信息长度
 @return 详见 enum ZegoAPIErrorCode
 */
- (int)setAacProperties:(double)timestamp dataLength:(int)dataLength specialConfigLength:(int)specialConfigLength;

@end

