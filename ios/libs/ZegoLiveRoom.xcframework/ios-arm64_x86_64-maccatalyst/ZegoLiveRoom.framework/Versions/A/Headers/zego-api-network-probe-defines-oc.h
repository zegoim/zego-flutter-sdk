//
//  zego_api_network_probe_defines_oc_h
//  ZegoLiveRoom
//
//  Copyright © 2019年 Zego. All rights reserved.
//

#ifndef zego_api_network_probe_defines_oc_h
#define zego_api_network_probe_defines_oc_h

#import <Foundation/Foundation.h>

/**
 探测类型
 
 - TYPE_CONNECT: 连接性测试
 - TYPE_UPLINK_SPEED: 上行测速
 - TYPE_DOWNLINK_SPEED: 下行测速
 */
typedef NS_ENUM(NSInteger, ZegoProbeType) {
    
    TYPE_CONNECT            = 1,
    TYPE_UPLINK_SPEED       = 2,
    TYPE_DOWNLINK_SPEED     = 3,
};

@interface ZegoNetConnectInfo : NSObject
/**
 链接消耗时间 单位ms
 */
@property (nonatomic, assign) int connectcost;

@end


@interface ZegoNetQuality : NSObject
/**
 链接消耗时间
 */
@property (nonatomic, assign) int connectcost;
/**
 延时(ms)
 */
@property (nonatomic, assign) int rtt;
/**
 丢包率(0~255)
 */
@property (nonatomic, assign) int pktlostrate;
/**
 网络质量 0 ~ 3 分别对应优良中差
 */
@property (nonatomic, assign) int quality;

@end

#endif /* zego_api_network_probe_defines_oc_h */
