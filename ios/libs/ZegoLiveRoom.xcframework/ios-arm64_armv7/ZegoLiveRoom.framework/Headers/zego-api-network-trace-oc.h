//
//  zego_api_network_trace_oc_h
//  ZegoLiveRoom
//
//  Created by zego.
//

#ifndef zego_api_network_trace_oc_h
#define zego_api_network_trace_oc_h

#import <Foundation/Foundation.h>
#import "zego-api-network-trace-defines-oc.h"

/**
  trace 模块回调
 */
@protocol ZegoNetworkTraceDelegate <NSObject>

@optional

/**
 @param result 检测结果
 */
 - (void)onNetworkTrace:(unsigned int)errcode resultInfo:(ZegoNetworkTraceResult*)result;

@end

/**
 网络整体测试管理类
 */
@interface ZegoNetworkTrace : NSObject

/**
 获取 ZegoNetworkTrace 的单例对象

 @return ZegoNetworkTrace 的单例对象
 */
+ (instancetype)sharedInstance;

/**
 设置trace的回调

 @param delegate 回调
 */
- (void)setNetworkTraceDelegate:(id<ZegoNetworkTraceDelegate>)delegate;

/**
 开启网络整体质量测试

 @param config 配置参数
 */
- (void)startNetworkTrace:(ZegoNetworkTraceConfig*)config;

/**
 停止网络整体质量测试
 */
- (void)stopNetworkTrace;

@end

#endif /* zego_api_network_trace_oc_h */
