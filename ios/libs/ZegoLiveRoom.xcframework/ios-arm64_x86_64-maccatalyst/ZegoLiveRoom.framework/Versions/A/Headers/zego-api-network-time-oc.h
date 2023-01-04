#ifndef zego_api_network_time_oc_h
#define zego_api_network_time_oc_h

#import <Foundation/Foundation.h>

#import "zego-api-defines-oc.h"

/**
网络时间同步回调
 */
@protocol ZegoNetworkTimeDelegate <NSObject>

@optional
/**
 当用户调用InitSDK时，SDK内部会去同步网络时间，同步完成时，会触发此回调（此回调在Init->Unint之间只会触发一次）

 */
 - (void)onNetworkTimeSynchronized;

@end


@interface ZegoNetworkTimeInfo : NSObject
/**
 返回同步的时间戳，如果返回 0，表示尚未同步
 */
@property (nonatomic, assign) unsigned long long timestamp;
/**
 最大误差(ms)
 */
@property (nonatomic, assign) int maxDeviation;

@end


@interface ZegoNetworkTime : NSObject

/**
 设置网络时间模块回调,建议initSDK 前设置

 * 
 */
+ (void)setNetworkTimeDelegate:(id<ZegoNetworkTimeDelegate>)delegate;

/**
 获取同步网络时间信息

 * @return 返回同步的时间信息
 */
+ (ZegoNetworkTimeInfo*)getNetworkTimeInfo;

@end

#endif /* zego_api_network_time_oc_h */
