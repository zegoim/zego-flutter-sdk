//
//  ZegoAudioPlayerController.m
//  Pods-Runner
//
//  Created by 李展鹏@ZEGO on 2019/12/12.
//

#import "ZegoAudioPlayerController.h"

@interface ZegoAudioPlayerController() <ZegoAudioPlayerDelegate>
{
    FlutterResult _startResult;
    FlutterResult _loadResult;
}

@property (nonatomic, strong) ZegoAudioPlayer* audioPlayer;
@property (nonatomic, weak) id<ZegoAudioPlayerControllerDelegate> delegate;
@property (nonatomic, strong) NSMutableDictionary<NSNumber*, FlutterResult> *startResultList;
@property (nonatomic, strong) NSMutableDictionary<NSNumber*, FlutterResult> *loadResultList;

@end


@implementation ZegoAudioPlayerController

+ (instancetype)instance {
    static ZegoAudioPlayerController *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[self alloc] init];
    });
    
    return instance;
}


- (instancetype)init {
    self = [super init];
    if(self) {
        _startResultList = [[NSMutableDictionary alloc] init];
        _loadResultList = [[NSMutableDictionary alloc] init];
    }
    
    return self;
}

- (void)initObject {
    _audioPlayer = [[ZegoAudioPlayer alloc] init];
    [_audioPlayer setDelegate:self];
}

- (void)uninitObject {
    [_audioPlayer setDelegate:nil];
    _audioPlayer = nil;
}

- (void)playAudioEffect:(NSDictionary *)args register:(NSObject<FlutterPluginRegistrar>*)reg result:(FlutterResult)result {
    
    NSString *path = args[@"path"];
    unsigned int soundID = [self numberToUintValue:args[@"soundID"]];
    int loopCount = [self numberToIntValue:args[@"loopCount"]];
    BOOL isPublish = [self numberToBoolValue:args[@"isPublish"]];
    BOOL isAssetPath = [self numberToBoolValue:args[@"isAssetPath"]];
    
    NSString *url;
    if(isAssetPath) {
        NSString *realPath = [reg lookupKeyForAsset:path];
        url = [[NSBundle mainBundle] pathForResource:realPath ofType:nil];
        NSLog(@"[playAudioEffect] bundlePath: %@", url);
    } else {
        url = path;
    }
    
    [_audioPlayer playEffect:soundID source:url loop:loopCount publish:isPublish];
    [self.startResultList setObject:result forKey:@(soundID)];
}

- (void)stopAudioEffect:(NSDictionary *)args result:(FlutterResult)result {
    
    unsigned int soundID = [self numberToUintValue:args[@"soundID"]];
    [_audioPlayer stopEffect:soundID];
    result(nil);
}

- (void)pauseAudioEffect:(NSDictionary *)args result:(FlutterResult)result {
    
    unsigned int soundID = [self numberToUintValue:args[@"soundID"]];
    [_audioPlayer pauseEffect:soundID];
    result(nil);
}

- (void)resumeAudioEffect:(NSDictionary *)args result:(FlutterResult)result {
    
    unsigned int soundID = [self numberToUintValue:args[@"soundID"]];
    [_audioPlayer resumeEffect:soundID];
    result(nil);
}

- (void)preloadAudioEffect:(NSDictionary *)args register:(NSObject<FlutterPluginRegistrar>*)reg result:(FlutterResult)result {
    
    NSString *path = args[@"path"];
    unsigned int soundID = [self numberToUintValue:args[@"soundID"]];
    BOOL isAssetPath = [self numberToBoolValue:args[@"isAssetPath"]];

    NSString *url;
    if(isAssetPath) {
        NSString *realPath = [reg lookupKeyForAsset:path];
        url = [[NSBundle mainBundle] pathForResource:realPath ofType:nil];
        NSLog(@"[playAudioEffect] bundlePath: %@", url);
    } else {
        url = path;
    }

    [_audioPlayer preloadEffect:soundID source:url];
    [self.loadResultList setObject:result forKey:@(soundID)];
}

- (void)unloadAudioEffect:(NSDictionary *)args result:(FlutterResult)result {
    
    unsigned int soundID = [self numberToUintValue:args[@"soundID"]];
    [_audioPlayer unloadEffect:soundID];
    result(nil);
}

- (void)setAudioEffectVolume:(NSDictionary *)args result:(FlutterResult)result{
    
    int vol = [self numberToIntValue:args[@"volume"]];
    unsigned int soundID = [self numberToUintValue:args[@"soundID"]];
    [_audioPlayer setEffect:soundID volume:vol];
    result(nil);
}

- (void)setAllEffectVolume:(NSDictionary *)args result:(FlutterResult)result{
    
    int vol = [self numberToIntValue:args[@"volume"]];
    [_audioPlayer setVolumeAll:vol];
    result(nil);
}

- (void)pauseAllEffect:(FlutterResult)result {
    
    [_audioPlayer pauseAll];
    result(nil);
}

- (void)stopAllEffect:(FlutterResult)result {
    
    [_audioPlayer stopAll];
    result(nil);
}

- (void)resumeAllEffect:(FlutterResult)result {
    
    [_audioPlayer resumeAll];
    result(nil);
}

- (void)seekTo:(NSDictionary *)args result:(FlutterResult)result {
    unsigned int soundID = [self numberToUintValue:args[@"soundID"]];
    long timestamp = [self numberToLongValue:args[@"timestamp"]];
    result(@([_audioPlayer seekTo:soundID timestamp:timestamp]));
}

- (void)getTotalDuration:(NSDictionary *)args result:(FlutterResult)result {
    unsigned int soundID = [self numberToUintValue:args[@"soundID"]];
    result(@([_audioPlayer getDuration:soundID]));
}

- (void)getCurrentDuration:(NSDictionary *)args result:(FlutterResult)result {
    unsigned int soundID = [self numberToUintValue:args[@"soundID"]];
    result(@([_audioPlayer getCurrentDuration:soundID]));
}

- (void)setAudioPlayerEventDelegate:(id<ZegoAudioPlayerControllerDelegate>)delegate {
    _delegate = delegate;
}

#pragma mark ZegoAudioPlayerDelegate

/*
 * 播放音效
 *
 * @param soundID 音效 ID
 * @param error 0 成功，-1 失败
 */
- (void)onPlayEffect:(unsigned int)soundID error:(int)error {
    
    FlutterResult result = [self.startResultList objectForKey:@(soundID)];
    if(result) {
        result(@{@"errorCode": @(error), @"soundID": @(soundID)});
        [self.startResultList removeObjectForKey:@(soundID)];
    }
}

/**
 * 播放音效完成
 *
 * @param soundID 音效 ID
 */
- (void)onPlayEnd:(unsigned int)soundID {
    if(_delegate) {
        if([_delegate respondsToSelector:@selector(onAudioPlayEnd:)])
            [_delegate onAudioPlayEnd:soundID];
    }
}

/*
 * 预加载音效
 *
 * @param soundID 音效 ID
 * @param error 0 成功，-1 失败
 */
- (void)onPreloadEffect:(unsigned int)soundID error:(int)error {
    
    FlutterResult result = [self.loadResultList objectForKey:@(soundID)];
    if(result) {
        result(@{@"errorCode": @(error), @"soundID": @(soundID)});
        [self.loadResultList removeObjectForKey:@(soundID)];
    }
}

/**
 * 预加载音效完成
 *
 * @param soundID 音效 ID
 */
- (void)onPreloadComplete:(unsigned int)soundID {
    
}

- (unsigned int)numberToUintValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0 : [number unsignedIntValue];
}

- (bool)numberToBoolValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? false : [number boolValue];
}

- (int)numberToIntValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0 : [number intValue];
}

- (long)numberToLongValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0 : [number longValue];
}

@end
