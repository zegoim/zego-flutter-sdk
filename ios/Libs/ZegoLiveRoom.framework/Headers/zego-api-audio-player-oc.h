#ifndef zego_api_audio_layer_oc_h
#define zego_api_audio_layer_oc_h

#import <Foundation/Foundation.h>

#import "zego-api-defines-oc.h"

/**
 * 音效播放器回调
 */ 
@protocol ZegoAudioPlayerDelegate <NSObject>

@optional

/*
 * 播放音效
 * 
 * @param soundID 音效 ID
 * @param error 0 成功，-1 失败
 */
- (void)onPlayEffect:(unsigned int)soundID error:(int)error;

/**
 * 播放音效完成
 * 
 * @param soundID 音效 ID
 */
- (void)onPlayEnd:(unsigned int)soundID;

/*
 * 预加载音效
 * 
 * @param soundID 音效 ID
 * @param error 0 成功，-1 失败
 */
- (void)onPreloadEffect:(unsigned int)soundID error:(int)error;

/**
 * 预加载音效完成
 * 
 * @param soundID 音效 ID
 */
- (void)onPreloadComplete:(unsigned int)soundID;

@end

/**
 * 音效播放器
 */
@interface ZegoAudioPlayer : NSObject

/**
 * 设置回调
 */
- (void)setDelegate:(id<ZegoAudioPlayerDelegate>)delegate;

/**
 * 播放音效
 * 
 * @param soundID 音效 ID
 * @param path 音效资源文件的本地路径
 * @param loopCount 循环次数
 * @param publish 是否放入推流中
 * 
 * @attention 如果是播放预加载的音效，指定音效 ID, 音效资源文件填 nil
 */
- (void)playEffect:(unsigned int)soundID source:(NSString *)path loop:(int)loopCount publish:(BOOL)publish;

/**
 * 停止播放音效
 * 
 * @param soundID 音效 ID
 */
- (void)stopEffect:(unsigned int)soundID;

/**
 * 暂停播放音效
 * 
 * @param soundID 音效 ID
 */
- (void)pauseEffect:(unsigned int)soundID;

/**
 * 恢复播放音效
 * 
 * @param soundID 音效 ID
 */
- (void)resumeEffect:(unsigned int)soundID;

/**
 * 设置单个音效的音量
 * 
 * @param soundID 音效 ID
 * @param volume 音量，取值范围[0, 200]，默认 100
 */
- (void)setEffect:(unsigned int)soundID volume:(int)volume;

/**
 * 设置所有音效的音量
 *
 * @param volume 音量，取值范围[0, 200]，默认 100
 */
- (void)setVolumeAll:(int)volume;

/**
 * 暂停全部音效
 */
- (void)pauseAll;

/**
 * 恢复全部音效
 */
- (void)resumeAll;

/**
 * 停止全部音效
 */
- (void)stopAll;

/**
 * 预加载音效
 *
 * @param soundID 音效 ID
 * @param path 音效资源文件的本地路径
 */
- (void)preloadEffect:(unsigned int)soundID source:(NSString*)path;

/**
 * 删除预加载音效
 * 
 * @param soundID 音效 ID
 */
- (void)unloadEffect:(unsigned int)soundID;

/**
 * 设置进度
 *
 * @param soundID 音效 ID
 * @param timestamp 进度, 单位毫秒
 * @return 返回 -1 表示失败, 返回 0 表示成功
 */
- (int)seekTo:(unsigned int)soundID timestamp:(long)timestamp;

/**
 * 获取音效的总时长
 *
 * @param soundID 音效 ID
 * @return 返回音效的总时长, 失败返回 0
 */
- (long)getDuration:(unsigned int)soundID;

/**
 * 获取音效的当前进度
 *
 * @param soundID 音效 ID
 * @return 返回音效的当前进度, 失败返回 -1
 */
- (long)getCurrentDuration:(unsigned int)soundID;

@end


#endif /* zego_api_audio_layer_oc_h */
