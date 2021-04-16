//
//  ZegoMediaplayerController.mm
//  Pods-Runner
//
//  Created by 李展鹏@ZEGO on 2020/4/30.
//

#import "ZegoMediaplayerController.h"
#import "ZegoViewRenderer.h"
#import "ZegoLog.h"

#import <sys/time.h>
#import <memory>
#import <thread>

/** 对 MediaPlayer 数据进行渲染时，表示 MediaPlayer 本地预览 */
NSString *kZegoVideoDataMediaPlayerStream = @"kZegoVideoDataMediaPlayerStream";

static NSString * const KEY_START = @"start";
static NSString * const KEY_STOP = @"stop";
static NSString * const KEY_PAUSE = @"pause";
static NSString * const KEY_RESUME = @"resume";
static NSString * const KEY_LOAD = @"load";
static NSString * const KEY_SEEK_TO = @"seek_to";

static id<ZegoMediaPlayerControllerVideoDataDelegate> mVideoDataDelegate = nil;
static ZegoMediaPlayerVideoPixelFormat mFormat = ZegoMediaPlayerVideoPixelFormatBGRA32;

@interface ZegoMediaPlayerController() <ZegoMediaPlayerEventWithIndexDelegate, ZegoMediaPlayerVideoPlayWithIndexDelegate>{
    CVPixelBufferPoolRef pool_;
    int video_width_;
    int video_height_;
}


@property (nonatomic, strong) ZegoMediaPlayer* mediaPlayer;
@property (nonatomic, strong) NSMutableDictionary* callbackMap;
@property (nonatomic, strong) ZegoRendererController *renderController;
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
    if (mVideoDataDelegate) {
        [_mediaPlayer setVideoPlayWithIndexDelegate:self format:mFormat];
    }
}

- (void)uninitObject {
    if ([_renderController removeRenderer:kZegoVideoDataMediaPlayerStream]) {
        if([self.renderController getRenderCount] == 0) {

            if([self.renderController isRendering])
                [self.renderController stopRendering];
        }

        [_renderController destroyPixelBufferPool:kZegoVideoDataMediaPlayerStream];
    };
    _renderController = nil;
    [_mediaPlayer setVideoPlayWithIndexDelegate:nil format:mFormat];
    [_mediaPlayer setEventWithIndexDelegate:nil];
    [_mediaPlayer uninit];
    _mediaPlayer = nil;
}

- (void)setDelegate:(id<ZegoMediaPlayerControllerDelegate>)delegate {
    _delegate = delegate;
}

- (void)setVideoDataDelegate:(id<ZegoMediaPlayerControllerVideoDataDelegate>)videoDataDelegate withFormat:(ZegoMediaPlayerVideoPixelFormat)format {
    mVideoDataDelegate = videoDataDelegate;
    mFormat = format;
}

- (void)setRenderController:(ZegoRendererController *)rndController {
    _renderController = rndController;
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

- (void)setPublishVolume:(int)volume {
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
    return [self.mediaPlayer setAudioStream:streamIndex];
}

- (long)getAudioStreamCount {
    return [self.mediaPlayer getAudioStreamCount];
}

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

typedef void (*CFTypeDeleter)(CFTypeRef cf);
#define MakeCFTypeHolder(ptr) std::unique_ptr<void, CFTypeDeleter>(ptr, CFRelease)

/**
 播放器视频数据回调
 
 @param data 视频数据
 @param size 数据长度
 @param format 数据类型
 @param index 播放器index
 */
- (void)onPlayVideoData:(const char *)data size:(int)size format:(struct ZegoMediaPlayerVideoDataFormat)format playerIndex:(ZegoMediaPlayerIndex)index {
    // 时间戳
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    unsigned long long timeMS = (unsigned long long)(tv_now.tv_sec) * 1000 + tv_now.tv_usec / 1000;
    // 构造 buffer
    CVPixelBufferRef pixelBuffer = [self createInputBufferWithWidth:format.width height:format.height stride:format.strides[0]];
    if (pixelBuffer == NULL) return;
    
    auto holder = MakeCFTypeHolder(pixelBuffer);
    
    CVReturn cvRet = CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    if (cvRet != kCVReturnSuccess) return;
    
    size_t destStride = CVPixelBufferGetBytesPerRow(pixelBuffer);
    unsigned char *dest = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    unsigned char *src = (unsigned char *)data;
    for (int i = 0; i < format.height; i++) {
        memcpy(dest, src, format.strides[0]);
        src += format.strides[0];
        dest += destStride;
    }
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    // 进行渲染
    if(![self.renderController isRendering]) {
        [ZegoLog logNotice:@"[onPlayVideoData] render controller is not rendering, ignore"];
        return;
    }

    // 通过固定的 key 拿到在 plugin 中创建的 Render
    ZegoViewRenderer *renderer = [self.renderController getRenderer:kZegoVideoDataMediaPlayerStream];
    [renderer setSrcFrameBuffer:pixelBuffer processBuffer:nil];
    
    // 回调出去
    if ([mVideoDataDelegate respondsToSelector:@selector(onPlayerVideoFrame:timeStamp:)]) {
        [mVideoDataDelegate onPlayerVideoFrame:pixelBuffer timeStamp:timeMS];
    }
}

#pragma mark- Private
- (void)createPixelBufferPool {
    NSDictionary *pixelBufferAttributes = @{
                                            (id)kCVPixelBufferOpenGLCompatibilityKey: @(YES),
                                            (id)kCVPixelBufferWidthKey: @(video_width_),
                                            (id)kCVPixelBufferHeightKey: @(video_height_),
                                            (id)kCVPixelBufferIOSurfacePropertiesKey: [NSDictionary dictionary],
                                            (id)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_32BGRA)
                                            };
    
    CFDictionaryRef ref = (__bridge CFDictionaryRef)pixelBufferAttributes;
    CVReturn ret = CVPixelBufferPoolCreate(nil, nil, ref, &pool_);
    if (ret != kCVReturnSuccess) {
        return ;
    }
}

- (CVPixelBufferRef)createInputBufferWithWidth:(int)width height:(int)height stride:(int)stride {
    if (video_width_ != width || video_height_ != height) {
        if (video_height_ && video_width_) {
            CVPixelBufferPoolFlushFlags flag = 0;
            CVPixelBufferPoolFlush(pool_, flag);
            CFRelease(pool_);
            pool_ = nil;
        }
        
        video_width_ = width;
        video_height_ = height;
        [self createPixelBufferPool];
    }
    
    CVPixelBufferRef pixelBuffer;
    CVReturn ret = CVPixelBufferPoolCreatePixelBuffer(nil, pool_, &pixelBuffer);
    if (ret != kCVReturnSuccess)
        return nil;
    
    return pixelBuffer;
}

@end
