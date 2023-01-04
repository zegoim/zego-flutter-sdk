//
//  zego-api-auto-mix-stream-oc.h
//  ZegoLiveRoom
//
//  Created by zego.
//

#ifndef zego_api_auto_mix_stream_oc_h
#define zego_api_auto_mix_stream_oc_h

#import <Foundation/Foundation.h>
#import "zego-api-auto-mix-stream-defines-oc.h"

/**
 自动混流配置信息的回调接口
 */
@protocol ZegoAutoMixStreamDelegate <NSObject>

@optional

/**
 调用开始自动混流的回调

 @param seq 调用序号, 和调用接口时返回的序号一一对应
 @param error 错误码
 */
- (void)onStartAutoMixStream:(int)seq withError:(unsigned int)error;

/**
 调用结束自动混流的回调

 @param seq 调用序号, 和调用接口时返回的序号一一对应
 @param error 错误码
 */
- (void)onStopAutoMixStream:(int)seq withError:(unsigned int)error;
@end

/**
 * 自动混流中的发言者及其说话音量的回调通知接口
 */
@protocol ZegoSoundLevelInAutoMixedStreamDelegate <NSObject>

@optional

/**
 自动混流中的发言者及其说话音量信息的回调
 
 * 注意：此接口是高频率同步回调，每秒钟10次通知，不拉流没有通知；请勿在该回调中处理耗时任务。
 
 @param soundLevelList 混流中各单流的音量信息列表
 */
- (void)onSoundLevelInAutoMixedStream:(NSArray<ZegoSoundLevelInAutoMixedStreamInfo *> *)soundLevelList;

@end

/**
 自动混流类
 */
@interface ZegoAutoMixStream : NSObject

/**
 设置自动混流的事件回调

 @param delegate 回调
 */
- (void)setDelegate:(id<ZegoAutoMixStreamDelegate>)delegate;

/**
 设置自动混流中的发言者及其说话音量的回调通知

 @param delegate 回调
 */
- (void)setSoundLevelDelegate:(id<ZegoSoundLevelInAutoMixedStreamDelegate>)delegate;

/**
 开始自动混流
    
 @param taskID 自动混流任务 ID
 @param roomID 房间 ID
 @param config 自动混流配置
 @return > 0 表示成功, 其值为调用序号. -1 为失败
 */
- (int)startAutoMixStream:(NSString*)taskID room:(NSString*)roomID config:(ZegoAutoMixStreamConfig*)config;

/**
 停止自动混流

 @param taskID 自动混流任务 ID
 @param roroomIDom_id 房间 ID
 @return > 0 表示成功, 其值为调用序号. -1 为失败
 */
- (int)stopAutoMixStream:(NSString*)taskID room:(NSString*)roomID;

@end




#endif /* zego_api_auto_mix_stream_oc_h */
