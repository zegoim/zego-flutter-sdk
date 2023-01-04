
#ifndef zego_api_frequency_spectrum_oc_h
#define zego_api_frequency_spectrum_oc_h

#include "zego-api-defines-oc.h"

@interface ZegoFrequencySpectrumInfo: NSObject

/** 流 ID */
@property(nonatomic, copy) NSString *streamID;
/** 频域功率谱 信息，元素类型为float */
@property(nonatomic, strong) NSArray<NSNumber *> *spectrumList;

@end

@protocol ZegoFrequencySpectrumDelegate <NSObject>

@optional


/**
 获取 拉流的 频域功率谱 信息

 @param spectrumInfos 房间内所有流 （非自己推的流）的 频域功率谱 信息
 */
- (void)onFrequencySpectrumUpdate:(NSArray<ZegoFrequencySpectrumInfo *> *)spectrumInfos;


/**
 获取 采集的 频域功率谱 信息

 @param captureSpectrum 采集的 频率功率谱 信息
 */
- (void)onCaptureFrequencySpectrumUpdate:(ZegoFrequencySpectrumInfo *)captureSpectrum;

@end

@interface ZegoFrequencySpectrum : NSObject


/**
 频域功率谱 单例对象

 @return ZegoFrequencySpectrum 的单例对象
 */
+ (instancetype)sharedInstance;


/**
 设置 频域功率谱 的代理

 @param delegate 代理
 */
- (void)setFrequencySpectrumDelegate:(id<ZegoFrequencySpectrumDelegate>)delegate;


/**
 设置获取 频域功率谱 的监控周期

 @param timeInMS 时间间隔，最小值为10ms,默认为500ms
 @return YES 成功，NO 失败
 @discussion 在启动监听之前设置有效
 */
- (BOOL)setFrequencySpectrumMonitorCycle:(unsigned int)timeInMS;


/**
 开始 频域功率谱 监听

 @return YES 成功，NO 失败
 */
- (BOOL)startFrequencySpectrumMonitor;


/**
 停止 频域功率谱 监听

 @return YES 成功，NO 失败
 */
- (BOOL)stopFrequencySpectrumMonitor;

@end

#endif /* zego_api_frequency_spectrum_oc_h */
