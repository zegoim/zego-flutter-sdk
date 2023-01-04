//
//  zego-api-mix-stream-oc.h
//
//  Copyright © 2018年 Zego. All rights reserved.
//

#ifndef zego_api_mix_stream_oc_h
#define zego_api_mix_stream_oc_h

#import <Foundation/Foundation.h>
#include "zego-api-defines-oc.h"
#include "zego-api-mix-stream-defines-oc.h"


/** 混流不存在的流名键值，值为 NSArray<NSString*>* */
ZEGO_EXTERN NSString *const kZegoMixNonExistsStreamIDKey;
/** 混流请求 seq 键值，值为 @(int) */
ZEGO_EXTERN NSString *const kZegoMixStreamReqSeqKey;

/**
 混流配置信息的回调接口
 */
@protocol ZegoMixStreamDelegate <NSObject>

@optional
/**
 混流配置更新的回调
 
 * 调用 ZegoStreamMixer -setDelegate: 设置了回调监听，并调用 ZegoStreamMixer -mixStream:seq: 设置混流配置后，SDK 通过此 API 通知调用方混流配置信息。
 * 注意：常见错误码及其含义请参考 ZegoError 中 kMixStream 开头的错误码定义。
 
 @param errorCode 错误码，0 表示混流启动成功，非 0 表示混流启动失败。
 @param mixStreamID 混流 ID，与 ZegoCompleteMixStreamConfig 中的 outputStream 参数一致。
 @param info 混流播放信息
 
 * info 参数里面的 key 包括：
 * mixStreamID  : 混流 ID
 * rtmpList     : rtmp url 列表
 * flvList      : flv url 列表
 * hlsList      : hls url 列表
 * nonExists    : 混流不存在输入流列表
 * mixConfigSeq : 每次混流的Seq
 */
- (void)onMixStreamConfigUpdate:(int)errorCode mixStream:(NSString *)mixStreamID streamInfo:(NSDictionary *)info;

@end

/**
 混流配置信息的回调接口
 */
@protocol ZegoMixStreamExDelegate <NSObject>

@optional
/**
 混流配置更新的回调
 
 * 调用 ZegoStreamMixer -setMixStreamExDelegate: 设置了回调监听，并调用 ZegoStreamMixer -mixStreamEx:mixStreamID: 设置混流配置后，SDK 通过此 API 通知调用方混流配置结果。
 * 注意：
 * 1. 如果输出流列表中输出流使用 URL 作为输出，则此回调中的 info 参数中不会包含有效的 rtmp、hls、flv 格式的混流信息，即使用该参数中的 url 不能拉取混流。
 * 2.常见错误码及其含义请参考 ZegoError 中 kMixStream 开头的错误码定义。
 
 @param errorCode 错误码，0 表示混流启动成功，非 0 表示混流启动失败。
 @param mixStreamID 混流任务 ID，与 ZegoStreamMixer -mixStreamEx:mixStreamID: 中的 mixStreamID 参数一致。
 @param info 混流播放信息
 */
- (void)onMixStreamExConfigUpdate:(int)errorCode mixStream:(NSString *)mixStreamID streamInfo:(ZegoMixStreamResultEx *)info;


/**
 混流转推CDN状态回调
 
 @param statesInfo 转推CDN状态信息
 @param streamID 混流任务ID
 */
- (void)onMixStreamRelayCDNStateUpdate:(NSArray<ZegoAPIStreamRelayCDNInfo *> *)statesInfo mixStreamID:(NSString*)streamID;

@end

/**
 混流中的发言者及其说话音量的回调通知接口
 */
@protocol ZegoLiveSoundLevelInMixedStreamDelegate <NSObject>

@optional
/**
 混流中的发言者及其说话音量信息的回调
 
 * 注意：此接口是高频率同步回调，每秒钟10次通知，不拉流没有通知；请勿在该回调中处理耗时任务。
 
 @param soundLevelList 混流中各单流的音量信息列表
 */
- (void)onSoundLevelInMixedStream:(NSArray<ZegoSoundLevelInMixedStreamInfo *> *)soundLevelList;

@end

/**
  混流类
  */
@interface ZegoStreamMixer : NSObject

/**
 设置接收混流配置更新结果的回调，对应于 ZegoStreamMixer -mixStream:seq: 接口的回调监听。
 
 * 注意：
 * 1.Deprecated 此方法不建议使用，建议使用 ZegoStreamMixer -setMixStreamExDelegate: 和 ZegoStreamMixer -mixStreamEx:mixStreamID:
 * 2.若不再需要接收混流配置更新信息，调用 setDelegate:nil 去除回调监听。
 
 @param delegate 实现了 ZegoMixStreamDelegate 代理的方法，用于混流时接收混流请求结果及配置更新信息。
 @see ZegoStreamMixer -mixStream:seq:
 */
- (void)setDelegate:(id<ZegoMixStreamDelegate>)delegate;

/**
 设置拉取混流时对混流中音量的回调监听
 
 * 注意：
 * 1.此方法由拉取混流方调用。
 * 2.若不再需要接收混流中各单流的音量信息，调用 setSoundLevelInMixedStreamDelegate:nil 去除回调监听。
 
 @param delegate 实现了 ZegoLiveSoundLevelInMixedStreamDelegate 代理的方法，用于拉取混流时接收混流中各单流的音量信息，可以根据此回调实现音浪。
 */
- (void)setSoundLevelInMixedStreamDelegate:(id<ZegoLiveSoundLevelInMixedStreamDelegate>)delegate;

/**
 混流接口，只支持输出一路混流。
 
 * 1.此 API 既是开始混流接口，也是停止混流接口；需要停止混流时，将 ZegoCompleteMixStreamConfig 参数中的 inputStreamList 置为空列表，即清空输入流列表，且开始、停止混流两次调用的 seq 参数保持一致。
 * 2.当混流信息变更（例如：混流的输入流列表发生增减、调整混流视频的输出码率等）时，需要调用此接口更新 ZEGO 混流服务器上的混流配置信息，且注意每次调用时此 API 的 seq 参数需保证一致。
 * 3.如果需要启动多个不同的混流，可以通过传入不同的 seq 来区分接收的 -onMixStreamConfigUpdate:mixStream:streamInfo: 回调。
 * 4.调用推流 API -startPublishing:title:flag: 或者 -startPublishing:title:flag:extraInfo: 时，需指定 flag 参数为 ZEGO_MIX_STREAM。
 
 @param completeMixConfig 混流配置，详细配置信息请查看 ZegoCompleteMixStreamConfig
 @param seq 开始混流的请求序号，回调会带回此 seq
 @return true-同步调用成功，等待回调；false-同步调用失败，收不到回调。
 @note Deprecated 建议使用 ZegoStreamMixer -setMixStreamExDelegate: 和 ZegoStreamMixer -mixStreamEx:mixStreamID: 接口实现混流。
 */
- (bool)mixStream:(ZegoCompleteMixStreamConfig *)completeMixConfig seq:(int)seq;

/**
 设置接收混流配置更新结果的回调监听，对应于 ZegoStreamMixer -mixStreamEx:mixStreamID: 接口的回调监听。
 
 * 注意：若不再需要接收混流配置更新信息，调用 setMixStreamExDelegate:nil 去除回调监听。
 
 @param delegate 实现了 ZegoMixStreamExDelegate 代理的方法，用于混流时接收混流请求结果及配置更新信息。
 @see ZegoStreamMixer -mixStreamEx:mixStreamID:
 */
- (void)setMixStreamExDelegate:(id<ZegoMixStreamExDelegate>)delegate;

/**
 混流接口，支持输出单路或者多路混流。
 
 * 1.混流任务ID，表示混流任务的唯一ID，调用方应该保证 mixStreamID 的唯一性。如果 mixStreamID 相同，服务端就认为是更新同一个混流。
 * 2.此 API 既是开始混流、更新混流接口，也是停止混流接口。
 * 3.需要停止混流时，将 ZegoMixStreamConfig 参数中的 inputStreamList 置为空列表（即清空输入流列表），将 mixStreamID 参数设置为和开始或更新混流的一致。
 * 4.当混流信息变更（例如：混流的输入流列表发生增减、调整混流视频的输出码率等）时，需要调用此接口更新 ZEGO 混流服务器上的混流配置信息，且注意每次调用时此 API 的 mixStreamID 参数需保证一致。
 * 5.如果需要启动多个不同的混流，可以传入不同的 mixStreamID，通过返回的 seq 来区分接收的 -onMixStreamExConfigUpdate:mixStream:streamInfo:回调。
 * 6.调用推流 API -startPublishing:title:flag: 或者 -startPublishing:title:flag:extraInfo: 时，需指定 flag 参数为 ZEGO_MIX_STREAM。
 
 @param config 混流配置信息
 @param mixStreamID 混流任务ID
 @return 大于 0 表示调用成功，且返回值为调用序号（seq），用以区分 -onMixStreamExConfigUpdate:mixStream:streamInfo: 回调；小于等于 0 表示调用失败。
  */
- (int)mixStreamEx:(ZegoMixStreamConfig *)config mixStreamID:(NSString *)mixStreamID;

@end

#endif /* zego_api_mix_stream_oc_h */
