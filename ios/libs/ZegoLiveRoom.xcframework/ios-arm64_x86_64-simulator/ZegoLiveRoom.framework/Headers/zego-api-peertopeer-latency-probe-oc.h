#ifndef zego_api_peertopeer_latency_probe_oc_h
#define zego_api_peertopeer_latency_probe_oc_h

#import <Foundation/Foundation.h>

#import "zego-api-defines-oc.h"

@protocol ZegoPeerToPeerLatencyProbeDelegate <NSObject>

@optional
/**
 延迟测量结果回调

 @param  delay 大于0为延迟(ms) -1为推流端时间同步异常 -2为拉流端时间同步异常
 @param  streamID  当前流ID信息，标记当前回调的信息属于哪条流。
*/

- (void)onPeerToPeerLatencyProbeResult:(int)delay ofStream:(NSString *)streamID;

@end


@interface ZegoPeerToPeerLatencyProbe : NSObject

/**
 获取 ZegoPeerToPeerLatencyProbe 的单例对象

 @return ZegoPeerToPeerLatencyProbe 的单例对象
 */
+ (instancetype)sharedInstance;

/**
 拉流端设置延迟测量的回调

 @param delegate 回调
 */
- (void)setPeerToPeerLatencyProbeDelegate:(id<ZegoPeerToPeerLatencyProbeDelegate>)delegate;

/**
 推流端激活延迟测量功能，默认时间间隔为3秒

 * 注意：必须在调用初始化 SDK 接口之后、推流接口之前设置。
 * 
 @param enable          true开启 false关闭
 @param channelIndex    推流通道 index，详见 enum ZegoAPIPublishChannelIndex。
 */
- (void)enablePeerToPeerLatencyProbe:(bool)enable channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 推流端设置延迟数据发送时间间隔毫秒数，默认为3000

 * 注意：必须在调用初始化 SDK 接口之后、推流接口之前设置。
 * 
 @param interval        interval 时间间隔，取值范围[1000,300000]
 @param channelIndex    推流通道 index，详见 enum ZegoAPIPublishChannelIndex。
 */
- (void)setPeerToPeerLatencyProbeInterval:(int)interval channelIndex:(ZegoAPIPublishChannelIndex)index;

@end

#endif /* zego_api_peertopeer_latency_probe_oc_h */
