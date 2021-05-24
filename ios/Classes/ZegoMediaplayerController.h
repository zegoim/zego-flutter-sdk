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

- (void)onPlayerVideoFrame: (CVPixelBufferRef)buffer timeStamp: (unsigned long long)timeStamp;

@end

@protocol ZegoMediaPlayerControllerDelegate <NSObject>

- (void)onPlayBegin;

- (void)onPlayPause;

- (void)onPlayResume;

- (void)onPlayEnd;

- (void)onPlayStop;

- (void)onPlayError:(int)errorCode;

- (void)onBufferBegin;

- (void)onBufferEnd;

- (void)onAudioBegin;

- (void)onVideoBegin;

- (void)onProcessInterval:(long)timestamp;

- (void)onSeekComplete:(int)errorCode when:(long)millisecond;

@end

@interface ZegoMediaPlayerController : NSObject

+ (instancetype)instance;

//- (void)initPlayerWithType:(MediaPlayerType)type;

//- (void)uninit;

- (void)initObject;

- (void)uninitObject;

- (void)setDelegate:(id<ZegoMediaPlayerControllerDelegate>)delegate;

- (void)setVideoDataDelegate:(id<ZegoMediaPlayerControllerVideoDataDelegate>)delegate;

- (void)setRenderController:(ZegoRendererController *)rndController;

- (void)start:(NSString *)path repeat:(BOOL)repeat pathMode:(int)pathMode reg:(NSObject<FlutterPluginRegistrar>*)reg result:(FlutterResult)result;

- (void)stop:(FlutterResult)result;

- (void)pause:(FlutterResult)result;

- (void)resume:(FlutterResult)result;

- (void)seekTo:(long)millisecond result:(FlutterResult)result;

- (long)getDuration;

- (long)getCurrentDuration;

- (void)muteLocal:(BOOL)mute;

- (void)load:(NSString *)path pathMode:(int)pathMode reg:(NSObject<FlutterPluginRegistrar>*)reg result:(FlutterResult)result;

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
