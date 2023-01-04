//
//  zego_api_network_trace_defines_oc_h
//  ZegoLiveRoom
//
//  Copyright © 2019年 Zego. All rights reserved.
//

#ifndef zego_api_network_trace_defines_oc_h
#define zego_api_network_trace_defines_oc_h

#import <Foundation/Foundation.h>

/**
 * 网络探测接口
*/
@interface ZegoNetworkTraceConfig : NSObject

/**
 * 探测是否强制启动traceroute 默认不启动0 启动1
 * 注意 开启traceroute 将会显著提升探测时间 最多jump 30次 (traceroute结果供参考，不代表最终连通性结果。优先已http,tcp,udp为准)
 */
@property (nonatomic, assign) int traceroute;

@end

/**
 * http探测结果
*/
@interface ZegoHttpTraceResult : NSObject

/**
 * 检测结果 0正常，非0 失败
 */
@property (nonatomic, assign) unsigned int code;

/**
 * 探测消耗的时间 单位ms
 */
@property (nonatomic, assign) int requestMs;

@end

/**
 * tcp探测结果
*/
@interface ZegoTcpTraceResult : NSObject

/**
 * 检测结果 0正常，非0 失败
 */
@property (nonatomic, assign) unsigned int code;

/**
 *tcp 链接消耗时间 单位ms
 */
@property (nonatomic, assign) int connectMs;

/**
 *tcp rtt 单位ms
 */
@property (nonatomic, assign) int rtt;

@end

/**
 * udp探测结果
*/
@interface ZegoUdpTraceResult : NSObject

/**
 * 检测结果 0正常，非0 失败
 */
@property (nonatomic, assign) unsigned int code;

/**
 * udp rtt 单位ms
 */
@property (nonatomic, assign) int rtt;

@end

/*
 * traceroute探测结果
*/
@interface ZegoTracerouteResult : NSObject

/**
 * 检测结果 0正常，非0 失败
 */
@property (nonatomic, assign) unsigned int code;

/**
 * tracert 单位ms
 */
@property (nonatomic, assign) int time;

@end

/**
 * 检测结果
*/
@interface ZegoNetworkTraceResult : NSObject

/**
 * 检测结果 0正常，非0 失败
 */
@property (nonatomic, assign) unsigned int code;

/**
 * http 探测结果
*/
@property (nonatomic, strong) ZegoHttpTraceResult *httpResult;

/**
 * tcp 探测结果
*/
@property (nonatomic, strong) ZegoTcpTraceResult *tcpResult;

/**
 * udp 探测结果
*/
@property (nonatomic, strong) ZegoUdpTraceResult *udpResult;

/**
 * traceroute 探测结果
*/
@property (nonatomic, strong) ZegoTracerouteResult *tracerouteResult;

@end

#endif /* zego_api_network_trace_defines_oc_h */
