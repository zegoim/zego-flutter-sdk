//
//  zego-api-network-probe-oc.h
//  ZegoLiveRoom
//
//  Created by zego.
//

#ifndef zego_api_network_probe_oc_h
#define zego_api_network_probe_oc_h

#import <Foundation/Foundation.h>
#import "zego-api-network-probe-defines-oc.h"

/**
 网络测速模块
 */
@protocol ZegoNetWorkProbeDelegate <NSObject>

@optional
/**
 网络测速模块连通回调 探测连接和测速都会回调此值

 @param errcode 错误码
 @param info 链接的结果相关信息
 @param type 结果的类型
 */
 - (void)onConnectResult:(int)errcode connectinfo:(ZegoNetConnectInfo*)info probeType:(ZegoProbeType)type;

/**
 速度测试时的回调 回调当前的质量

 @param quality 速度测试的结果
 @param type 结果的类型
 */
- (void)onUpdateSpeed:(ZegoNetQuality*)quality probeType:(ZegoProbeType)type;

/**
 网络测速异常结束的回调

 @param errcode 0 正常结束
 @param type 结束的类型
*/

- (void)onTestStop:(int)errcode probeType:(ZegoProbeType)type;


@end

/**
 网络测速管理类

 */
@interface ZegoNetWorkProbe : NSObject

/**
 获取 ZegoNetWorkProbe 的单例对象

 @return ZegoNetWorkProbe 的单例对象
 */
+ (instancetype)sharedInstance;

/**
 设置网络测速的回调

 @param delegate 回调
 */
- (void)setNetWorkProbeDelegate:(id<ZegoNetWorkProbeDelegate>)delegate;

/**
 启动连通性测试 InitSDK 后调用 同一时间内与startSpeedTest只有一个生效 推拉流会中断此操作
 此接口仅仅只会检测与zego服务的连通性，不会产生媒体数据
 */
- (void)startConnectivityTest;

/**
 停止连通性测试

 */
- (void)stopConnectivityTest;

/**
 开始上行测速 InitSDK 后调用 同一时间内与startConnectivityTest只有一个生效 启动推拉流会中断此操作
 不建议长时间测速，可能会影响推拉流体验

 @param bitrate 测速推流时的比特率 单位bps

 */
- (void)startUplinkSpeedTest:(int)bitrate;

/**
 设置刷新网络质量的时间间隔

 @param interval 测速时回调质量的时间间隔 单位ms 
*/
- (void)setQualityCallbackInterval:(int)interval;

/**
 停止上行测速
 */
- (void)stopUplinkSpeedTest;

/**
 开始下行测速 InitSDK 后调用 同一时间内与startConnectivityTest只有一个生效 启动推拉流会中断此操作
 不建议长时间测速，可能会影响推拉流体验

 @param bitrate 测速推流时的比特率 单位bps

 */
- (void)startDownlinkSpeedTest:(int)bitrate;

/**
 停止下行测速
 */
- (void)stopDownlinkSpeedTest;

@end

#endif /* zego_api_network_probe_oc_h */
