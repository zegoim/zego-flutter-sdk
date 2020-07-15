//
//  ZegoMediaplayerController.mm
//  Pods-Runner
//
//  Created by 李展鹏@ZEGO on 2020/4/30.
//

#import "ZegoMediaplayerController.h"

static NSString * const KEY_START = @"start";
static NSString * const KEY_STOP = @"stop";
static NSString * const KEY_PAUSE = @"pause";
static NSString * const KEY_RESUME = @"resume";
static NSString * const KEY_LOAD = @"load";
static NSString * const KEY_SEEK_TO = @"seek_to";

@interface ZegoMediaPlayerController() <ZegoMediaPlayerEventWithIndexDelegate>


@property (nonatomic, strong) ZegoMediaPlayer* mediaPlayer;
@property (nonatomic, strong) NSMutableDictionary* callbackMap;
//@property (nonatomic, weak) id<ZegoAudioPlayerControllerDelegate> delegate;
@property (nonatomic, weak) id<ZegoMediaPlayerControllerDelegate> delegate;

@end

@implementation ZegoMediaPlayerController

+ (instancetype)instance {
    static ZegoMediaPlayerController *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[self alloc] init];
    });
    
    return instance;
}

- (instancetype)init {
    self = [super init];
    if(self) {
        _callbackMap = [[NSMutableDictionary alloc] init];
    }
    
    return self;
}

- (void)initObject {
    _mediaPlayer = [[ZegoMediaPlayer alloc] initWithPlayerType:MediaPlayerTypeAux playerIndex:ZegoMediaPlayerIndexFirst];
    
    [_mediaPlayer setEventWithIndexDelegate:self];
}

- (void)uninitObject {
    [_mediaPlayer uninit];
    _mediaPlayer = nil;
}

- (void)setDelegate:(id<ZegoMediaPlayerControllerDelegate>)delegate {
    _delegate = delegate;
}

/*- (void)initPlayerWithType:(MediaPlayerType)type {
    _mediaPlayer = [[ZegoMediaPlayer alloc] initWithPlayerType:type playerIndex:ZegoMediaPlayerIndexFirst];
}

- (void)uninit {
    [_mediaPlayer uninit];
    _mediaPlayer = nil;
}*/

- (void)start:(NSString *)path repeat:(BOOL)repeat asset:(BOOL)asset reg:(NSObject<FlutterPluginRegistrar>*)reg result:(FlutterResult)result{
    
    NSString *url;
    if(asset) {
        NSString *realPath = [reg lookupKeyForAsset:path];
        url = [[NSBundle mainBundle] pathForResource:realPath ofType:nil];
        NSLog(@"[startMediaPlayer] bundlePath: %@", url);
    } else {
        url = path;
    }
    
    [self.callbackMap setObject:result forKey:KEY_START];
    [self.mediaPlayer start:url repeat:repeat];
}

- (void)stop:(FlutterResult)result {
    
    [self.callbackMap setObject:result forKey:KEY_STOP];
    [self.mediaPlayer stop];
}

- (void)pause:(FlutterResult)result {
    
    [self.callbackMap setObject:result forKey:KEY_PAUSE];
    [self.mediaPlayer pause];
}

- (void)resume:(FlutterResult)result {
    
    [self.callbackMap setObject:result forKey:KEY_RESUME];
    [self.mediaPlayer resume];
}

- (void)seekTo:(long)millisecond result:(FlutterResult)result{
    
    [self.callbackMap setObject:result forKey:KEY_SEEK_TO];
    [self.mediaPlayer seekTo:millisecond];
}

- (long)getDuration {
    return [self.mediaPlayer getDuration];
}

- (long)getCurrentDuration {
    return [self.mediaPlayer getCurrentDuration];
}

- (void)muteLocal:(BOOL)mute {
    [self.mediaPlayer muteLocal:mute];
}

- (void)load:(NSString *)path asset:(BOOL)asset reg:(NSObject<FlutterPluginRegistrar>*)reg result:(FlutterResult)result{
    
    NSString *url;
    if(asset) {
        NSString *realPath = [reg lookupKeyForAsset:path];
        url = [[NSBundle mainBundle] pathForResource:realPath ofType:nil];
        NSLog(@"[startMediaPlayer] bundlePath: %@", url);
    } else {
        url = path;
    }
    
    [self.callbackMap setObject:result forKey:KEY_LOAD];
    [self.mediaPlayer load:url];
}

- (void)setVolume:(int)volume {
    [self.mediaPlayer setVolume:volume];
}

- (void)setPlayerType:(MediaPlayerType)type {
    [self.mediaPlayer setPlayerType:type];
}

- (void)setOnlineResourceCacheDuration:(int)time andSize:(int)size {
    [self.mediaPlayer setOnlineResourceCacheDuration:time andSize:size];
    
}

- (void)getOnlineResourceCacheStat:(int*)time andSize:(int*)size {
    [self.mediaPlayer getOnlineResourceCacheStat:time andSize:size];
}

- (void)setBufferThreshold:(int)threshold {
    [self.mediaPlayer setBufferThreshold:threshold];
}

- (void)setLoadResourceTimeout:(int)timeout {
    [self.mediaPlayer setLoadResourceTimeout:timeout];
}

/*- (void)setPublishVolume:(int)volume {
    [self.mediaPlayer setPublishVolume:volume];
}

- (void)setPlayVolume:(int)volume {
    [self.mediaPlayer setPlayVolume:volume];
}

- (int)getPublishVolume {
    return [self.mediaPlayer getPublishVolume];
}

- (int)getPlayVolume {
    return [self.mediaPlayer getPlayVolume];
}

- (long)setAudioStream:(long)streamIndex {
    
}

- (long)getAudioStreamCount {
    
}*/

- (void)enableRepeatMode:(BOOL)enable {
    [self.mediaPlayer enableRepeatMode:enable];
}

- (void)setProcessInterval:(long)interval {
    [self.mediaPlayer setProcessInterval:interval];
}

/**
 开始播放
 
 @param index 播放器序号
 */
- (void)onPlayStart:(ZegoMediaPlayerIndex)index {
    
    FlutterResult result = [self.callbackMap objectForKey:KEY_START];
    if(result) {
        result(nil);
        [self.callbackMap removeObjectForKey:KEY_START];
    }
}

/**
 暂停播放
 
 @param index 播放器序号
 */
- (void)onPlayPause:(ZegoMediaPlayerIndex)index {
    
    FlutterResult result = [self.callbackMap objectForKey:KEY_PAUSE];
       if(result) {
           result(nil);
           [self.callbackMap removeObjectForKey:KEY_PAUSE];
       }
}

/**
 恢复播放
 
 @param index 播放器序号
 */
- (void)onPlayResume:(ZegoMediaPlayerIndex)index {
    
    FlutterResult result = [self.callbackMap objectForKey:KEY_RESUME];
       if(result) {
           result(nil);
           [self.callbackMap removeObjectForKey:KEY_RESUME];
       }
}

/**
 播放错误
 
 @param code
 PLAY_ERROR_NOERROR = 0,
 PLAY_ERROR_FILE =  -1，文件格式不支持,
 PLAY_ERROR_PATH =  -2，路径不存在,
 PLAY_ERROR_CODEC = -3, 文件无法解码
 PLAY_ERROR_NO_SUPPORT_STREAM = -4,文件中没有可播放的音视频流
 PLAY_ERROR_DEMUX = -5, 文件解析过程中出现错误
 @param index 播放器序号
 */
- (void)onPlayError:(int)code playerIndex:(ZegoMediaPlayerIndex)index {
    if(_delegate) {
        if([_delegate respondsToSelector:@selector(onPlayError:)])
        [_delegate onPlayError:code];
    }
}

/**
 开始播放视频
 
 @param index 播放器序号
 */
- (void)onVideoBegin:(ZegoMediaPlayerIndex)index {
    
}


/**
 开始播放音频
 
 @param index 播放器序号
 */
- (void)onAudioBegin:(ZegoMediaPlayerIndex)index {
    
}


/**
 播放结束
 
 @param index 播放器序号
 */
- (void)onPlayEnd:(ZegoMediaPlayerIndex)index {
    if(_delegate) {
        if([_delegate respondsToSelector:@selector(onPlayEnd)])
        [_delegate onPlayEnd];
    }
}

/**
 用户停止播放的回调
 
 @param index 播放器序号
 */
- (void)onPlayStop:(ZegoMediaPlayerIndex)index {
    
    FlutterResult result = [self.callbackMap objectForKey:KEY_STOP];
       if(result) {
           result(nil);
           [self.callbackMap removeObjectForKey:KEY_STOP];
       }
}

/**
 网络音乐资源播放不畅，开始尝试缓存数据。
 
 @param index 播放器序号
 @warning 只有播放网络音乐资源才需要关注这个回调
 */
- (void)onBufferBegin:(ZegoMediaPlayerIndex)index {
    if(_delegate) {
        if([_delegate respondsToSelector:@selector(onBufferBegin)])
        [_delegate onBufferBegin];
    }
}

/**
 网络音乐资源可以顺畅播放。
 
 @param index 播放器序号
 @warning 只有播放网络音乐资源才需要关注这个回调
 */
- (void)onBufferEnd:(ZegoMediaPlayerIndex)index {
    if(_delegate) {
        if([_delegate respondsToSelector:@selector(onBufferEnd)])
        [_delegate onBufferEnd];
    }
}

/**
 快进到指定时刻
 
 @param code >=0 成功，其它表示失败
 @param millisecond 实际快进的进度，单位毫秒
 @param index 播放器序号
 */
- (void)onSeekComplete:(int)code when:(long)millisecond playerIndex:(ZegoMediaPlayerIndex)index {
    
    FlutterResult result = [self.callbackMap objectForKey:KEY_SEEK_TO];
       if(result) {
           result(@{@"errorCode": @(code), @"timestamp": @(millisecond)});
           [self.callbackMap removeObjectForKey:KEY_SEEK_TO];
       }
}

/**
 截图
 
 @param image
 @param index 播放器序号
 */
- (void)onSnapshot:(ZEGOImage *)image playerIndex:(ZegoMediaPlayerIndex)index {
    
}

/**
 预加载完成
 
 @param index 播放器序号
 @warning 调用 load 的回调
 */
- (void)onLoadComplete:(ZegoMediaPlayerIndex)index {
    
    FlutterResult result = [self.callbackMap objectForKey:KEY_LOAD];
       if(result) {
           result(nil);
           [self.callbackMap removeObjectForKey:KEY_LOAD];
       }
}

/**
 播放进度回调
 
 @param timestamp 当前播放进度，单位毫秒
 @param index 播放器序号
 @note 同步回调，请不要在回调中处理数据或做其他耗时操作
 */
- (void)onProcessInterval:(long)timestamp playerIndex:(ZegoMediaPlayerIndex)index {
    if(_delegate) {
        if([_delegate respondsToSelector:@selector(onProcessInterval:)])
        [_delegate onProcessInterval:timestamp];
    }
}

@end
