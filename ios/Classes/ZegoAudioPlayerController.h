//
//  ZegoAudioPlayerController.h
//  Pods
//
//  Created by 李展鹏@ZEGO on 2019/12/12.
//

#ifndef ZegoAudioPlayerController_h
#define ZegoAudioPlayerController_h

#import <Foundation/Foundation.h>
#import <Flutter/Flutter.h>
#import <ZegoLiveRoom/zego-api-audio-player-oc.h>

@protocol ZegoAudioPlayerControllerDelegate <NSObject>
- (void)onAudioPlayBegin:(unsigned int)soundID errorCode:(int)errorCode;

- (void)onAudioLoad:(unsigned int)soundID errorCode:(int)errorCode;

- (void)onAudioLoadComplete:(unsigned int)soundID;

- (void)onAudioPlayEnd:(unsigned int)soundID;

@end


@interface ZegoAudioPlayerController : NSObject

+ (instancetype)instance;

- (void)initObject;
- (void)uninitObject;

- (void)playAudioEffect:(NSDictionary *)args register:(NSObject<FlutterPluginRegistrar>*)reg result:(FlutterResult)result;
- (void)stopAudioEffect:(NSDictionary *)args result:(FlutterResult)result;
- (void)pauseAudioEffect:(NSDictionary *)args result:(FlutterResult)result;
- (void)resumeAudioEffect:(NSDictionary *)args result:(FlutterResult)result;
- (void)preloadAudioEffect:(NSDictionary *)args register:(NSObject<FlutterPluginRegistrar>*)reg result:(FlutterResult)result;
- (void)unloadAudioEffect:(NSDictionary *)args result:(FlutterResult)result;
- (void)setAudioEffectVolume:(NSDictionary *)args result:(FlutterResult)result;

- (void)setAllEffectVolume:(NSDictionary *)args result:(FlutterResult)result;
- (void)pauseAllEffect:(FlutterResult)result;
- (void)stopAllEffect:(FlutterResult)result;
- (void)resumeAllEffect:(FlutterResult)result;

- (void)seekTo:(NSDictionary *)args result:(FlutterResult)result;
- (void)getTotalDuration:(NSDictionary *)args result:(FlutterResult)result;
- (void)getCurrentDuration:(NSDictionary *)args result:(FlutterResult)result;

- (BOOL)isPlaying:(int)soundID;

- (void)setAudioPlayerEventDelegate:(id<ZegoAudioPlayerControllerDelegate>)delegate;


@end


#endif /* ZegoAudioPlayerController_h */
