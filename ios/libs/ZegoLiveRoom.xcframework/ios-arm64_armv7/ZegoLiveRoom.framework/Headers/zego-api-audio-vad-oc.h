//
//  zego-api-audio-vad-tool-oc.h
//  ZegoLiveRoom
//
//  Created by kebo on 2021/1/14.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * 检测结果类型
 */
typedef NS_ENUM(NSInteger, ZEGOAPIAudioVADType)
{
    // 噪声
    ZEGOAPIAudioVADTypeNoise = 0,
    // 语音
    ZEGOAPIAudioVADTypeSpeech = 1,
};

/**
 * 检测音频包类
 */
@interface ZegoAudioVAD : NSObject

/**
 * 检测音频包是否包含语音
 * 
 * @param buf 音频数据
 * @param bufLen 音频数据长度
 * @param rate 采样率
 * @param channels 通道数
 * @return 返回值参考 ZEGOAPIAudioVADType 定义
 */
- (ZEGOAPIAudioVADType)update:(unsigned char*)buf bufLen:(int)bufLen sampleRate:(int)rate channels:(int)channels;

/**
 * 重置检测音频包类对象
 * 
 * @note 当音频数据不连续时，需要调用该接口重置状态。
 * @note 在切换流之后应该重置
 * @note 当发生音频数据丢失或丢弃时应该重置
 * @return 0 表示成功，-1 表示失败
 */
- (int)reset;

@end

NS_ASSUME_NONNULL_END
