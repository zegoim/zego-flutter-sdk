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


/** 混流不存在的流名，值为 NSArray<NSString*>* */
ZEGO_EXTERN NSString *const kZegoMixNonExistsStreamIDKey;
/** 混流请求 seq，值为 @(int) */
ZEGO_EXTERN NSString *const kZegoMixStreamReqSeqKey;


@protocol ZegoMixStreamDelegate <NSObject>

@optional
/**
 混流配置更新结果回调
 
 @param errorCode 错误码，0 表示没有错误
 @param mixStreamID 混流任务ID
 @param info 混流播放信息
 @discussion 调用 [ZegoStreamMixer -mixStream:seq:] 更新混流配置后，通过此 API 通知调用方
 @note 常见错误码及其含义请参考ZegoError中kMixStream开头的错误码定义
 */
- (void)onMixStreamConfigUpdate:(int)errorCode mixStream:(NSString *)mixStreamID streamInfo:(NSDictionary *)info;

@end

@protocol ZegoMixStreamExDelegate <NSObject>

@optional
/**
 混流配置更新结果回调
 
 @param errorCode 错误码，0 表示没有错误
 @param mixStreamID 混流任务ID
 @param info 混流播放信息
 @discussion 调用 [ZegoStreamMixer -mixStreamEx:mixStreamID:] 更新混流配置后，通过此 API 通知调用方
 @note 常见错误码及其含义请参考ZegoError中kMixStream开头的错误码定义
 */
- (void)onMixStreamExConfigUpdate:(int)errorCode mixStream:(NSString *)mixStreamID streamInfo:(ZegoMixStreamResultEx *)info;


/**
 混流转推CDN状态回调

 @param statesInfo 转推CDN状态信息
 @param streamID 混流任务ID
 @discussion 混流直推即构服务器时，ZegoAPIStreamRelayCDNInfo 中的 rtmpUrl 字段以 avertp:// 开头
 */
- (void)onMixStreamRelayCDNStateUpdate:(NSArray<ZegoAPIStreamRelayCDNInfo *> *)statesInfo mixStreamID:(NSString*)streamID;

@end

@protocol ZegoLiveSoundLevelInMixedStreamDelegate <NSObject>

@optional

- (void)onSoundLevelInMixedStream:(NSArray<ZegoSoundLevelInMixedStreamInfo *> *)soundLevelList;

@end


@interface ZegoStreamMixer : NSObject

/**
 设置混流配置的回调，对应于 [ZegoStreamMixer -mixStream:seq:]
 */
- (void)setDelegate:(id<ZegoMixStreamDelegate>)delegate;

/**
 设置拉取混流时带上音量信息的回调
 
 note: 可以通过这个回调实现音浪的功能。
 */
- (void)setSoundLevelInMixedStreamDelegate:(id<ZegoLiveSoundLevelInMixedStreamDelegate>)delegate;

/**
 混流接口，只支持单路混流。
 
 @param completeMixConfig 混流配置
 @param seq 请求序号，回调会带回次 seq
 @return true 成功，等待回调，false 失败
 @discussion 每次需要更新混流配置时，都可以调用此接口；如果需要多次调用，可以通过传入不同的 seq 区分回调
 @note Deprecated，请使用 mixStreamEx:mixStreamID:
 */
- (bool)mixStream:(ZegoCompleteMixStreamConfig *)completeMixConfig seq:(int)seq;

/**
 设置混流配置的回调，对应于 [ZegoStreamMixer -mixStreamEx:mixStreamID:]
 */
- (void)setMixStreamExDelegate:(id<ZegoMixStreamExDelegate>)delegate;

/**
 混流接口，支持多路混流
 
 @param config 混流配置
 @param mixStreamID 混流任务ID
 @return >0 表示调用成功，且返回值是调用序号seq，<=0 表示调用失败
 @discussion 每次需要更新混流配置时，都可以调用此接口；通过传入不同的 seq 区分回调
 */
- (int)mixStreamEx:(ZegoMixStreamConfig *)config mixStreamID:(NSString *)mixStreamID;

@end

#endif /* zego_api_mix_stream_oc_h */
