#ifndef zego_api_sound_level_oc_h
#define zego_api_sound_level_oc_h

#import <Foundation/Foundation.h>
#include "zego-api-defines-oc.h"

/** 某条流的音浪信息 */
@interface ZegoSoundLevelInfo: NSObject

/** 此音浪信息的流 ID，当为采集音浪信息时，不必关心此值 */
@property(nonatomic, copy) NSString *streamID;
/** streamID 对应的流的音量值，取值范围：[0.0, 100.0] */
@property(nonatomic, assign) float soundLevel;
/** streamID 对应的流是否包含人声，0:表示噪声，1:表示人声，调用[ZegoSoundLevel -enableVADMonitor:] 设置true后，此值才有效 */
@property(nonatomic, assign) int vad;

@end


@protocol ZegoSoundLevelDelegate <NSObject>

@optional

/**
 拉流音浪信息更新回调。回调频率由 setSoundLevelMonitorCycle 指定，默认 200ms 回调一次

 @param soundLevels 房间内所有流（非自己推的流）的音浪信息，参考 ZegoSoundLevelInfo 定义
 */
- (void)onSoundLevelUpdate:(NSArray<ZegoSoundLevelInfo *> *)soundLevels;


/**
 推流音浪信息更新回调。回调频率由 setSoundLevelMonitorCycle 指定，默认 200ms 回调一次

 @param captureSoundLevel 房间内采集的音浪信息（自己推的流），参考 ZegoSoundLevelInfo 定义
 */
- (void)onCaptureSoundLevelUpdate:(ZegoSoundLevelInfo *)captureSoundLevel;

@end


@interface ZegoSoundLevel : NSObject

/**
 获取 ZegoSoundLevel 的单例对象

 @return ZegoSoundLevel 的单例对象
 */
+ (instancetype)sharedInstance;

/**
 设置代理对象

 @param delegate 实现 ZegoSoundLevelDelegate 的代理对象
 @discussion 未设置代理，或者设置代理失败，会造成无法正常回调音浪信息
 */
- (void)setSoundLevelDelegate:(id<ZegoSoundLevelDelegate>)delegate;

/**
 设置音浪回调的监控周期

 @param timeInMS 时间周期，单位为毫秒，取值范围 [100, 3000]。默认 200 ms。
 @return true 成功；false 失败
 @discussion 该设置会影响 [ZegoSoundLevelDelegate -onSoundLevelUpdate:] [ZegoSoundLevelDelegate -onCaptureSoundLevelUpdate:] 的回调频率
 */
- (bool)setSoundLevelMonitorCycle:(unsigned int)timeInMS;

/**
 启动音浪监听

 @return true 成功，等待回调；false 失败
 @discussion startSoundLevelMonitor 后一定要 stopSoundLevelMonitor
 @discussion StartSoundLevelMonitor 后，即使未启动本地音频采集，[ZegoSoundLevelDelegate -onCaptureSoundLevelUpdate:] 也会有回调，且音浪值为0
 */
- (bool)startSoundLevelMonitor;

/**
 停止音浪监听

 @return true 成功；false 失败
 */
- (bool)stopSoundLevelMonitor;

/**
 设置音浪回调是否检测VAD，VAD算法有性能开销，建议按需设置

 @param enable true 开启；false 关闭。默认 关闭。
 @return true 成功；false 失败
 */
- (bool)enableVADMonitor:(bool)enable;

@end

#endif /* zego_api_sound_level_oc_h */
