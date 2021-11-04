//
//  ZegoMediaplayerController.h
//  Pods-Runner
//
//  Created by 李展鹏@ZEGO on 2020/4/30.
//

#ifndef ZegoMediaplayerController_h
#define ZegoMediaplayerController_h

#import <Foundation/Foundation.h>
#import <Flutter/Flutter.h>
#import <ZegoLiveRoom/zego-api-mediaplayer-oc.h>

#import "ZegoRendererController.h"

@protocol ZegoMediaPlayerControllerVideoDataDelegate <NSObject>

- (void)onPlayerVideoFrame: (CVPixelBufferRef)buffer timeStamp: (unsigned long long)timeStamp index:(ZegoMediaPlayerIndex)playerIndex;

@end

@protocol ZegoMediaPlayerControllerDelegate <NSObject>

- (void)onPlayBegin:(ZegoMediaPlayerIndex)playerIndex;

- (void)onPlayPause:(ZegoMediaPlayerIndex)playerIndex;

- (void)onPlayResume:(ZegoMediaPlayerIndex)playerIndex;

- (void)onPlayEnd:(ZegoMediaPlayerIndex)playerIndex;

- (void)onPlayStop:(ZegoMediaPlayerIndex)playerIndex;

- (void)onPlayError:(int)errorCode index:(ZegoMediaPlayerIndex)playerIndex;

- (void)onBufferBegin:(ZegoMediaPlayerIndex)playerIndex;

- (void)onBufferEnd:(ZegoMediaPlayerIndex)playerIndex;

- (void)onAudioBegin:(ZegoMediaPlayerIndex)playerIndex;

- (void)onVideoBegin:(ZegoMediaPlayerIndex)playerIndex;

- (void)onProcessInterval:(long)timestamp index:(ZegoMediaPlayerIndex)playerIndex;

- (void)onSeekComplete:(int)errorCode when:(long)millisecond index:(ZegoMediaPlayerIndex)playerIndex;

@end

@interface ZegoMediaPlayerController : NSObject

//+ (instancetype)instance;

//- (void)initPlayerWithType:(MediaPlayerType)type;

//- (void)uninit;


/// 播放器的 id
@property (nonatomic, assign) ZegoMediaPlayerIndex zegoPlayIndex;

- (instancetype)initObjectWithIndex:(ZegoMediaPlayerIndex)playerIndex;

- (void)uninitObject;

- (void)setDelegate:(id<ZegoMediaPlayerControllerDelegate>)delegate;

- (void)setVideoDataDelegate:(id<ZegoMediaPlayerControllerVideoDataDelegate>)delegate;

- (void)setRenderController:(ZegoRendererController *)rndController;

- (void)setPlatformView:(UIView *)view;

- (void)start:(NSString *)path repeat:(BOOL)repeat asset:(BOOL)asset reg:(NSObject<FlutterPluginRegistrar>*)reg result:(FlutterResult)result;

- (void)stop:(FlutterResult)result;

- (void)pause:(FlutterResult)result;

- (void)resume:(FlutterResult)result;

- (void)seekTo:(long)millisecond result:(FlutterResult)result;

- (long)getDuration;

- (long)getCurrentDuration;

- (void)muteLocal:(BOOL)mute;

- (void)load:(NSString *)path asset:(BOOL)asset reg:(NSObject<FlutterPluginRegistrar>*)reg result:(FlutterResult)result;

- (void)setVolume:(int)volume;

- (void)setPlayerType:(MediaPlayerType)type;

- (void)setOnlineResourceCacheDuration:(int)time andSize:(int)size;

- (void)getOnlineResourceCacheStat:(int*)time andSize:(int*)size;

- (void)setBufferThreshold:(int)threshold;

- (void)setLoadResourceTimeout:(int)timeout;

- (void)setPublishVolume:(int)volume;

- (void)setPlayVolume:(int)volume;

- (int)getPublishVolume;

- (int)getPlayVolume;

- (long)setAudioStream:(long)streamIndex;

- (long)getAudioStreamCount;

- (void)enableRepeatMode:(BOOL)enable;

- (void)setProcessInterval:(long)interval;

- (BOOL)isPlaying;

@end

#endif /* ZegoMediaplayerController_h */
