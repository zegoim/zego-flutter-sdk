#import "ZegoLiveRoomPlugin.h"
#import "ZegoRendererController.h"
#import "ZegoPlatformViewFactory.h"
#import "ZegoLog.h"

typedef NS_ENUM(NSUInteger, EVENT_TYPE) {
    TYPE_ROOM_EVENT = 0,
    TYPE_PUBLISH_EVENT,
    TYPE_PLAY_EVENT,
    TYPE_MEDIA_SIDE_INFO_EVENT,
    TYPE_SOUND_LEVEL_EVENT
};

@interface ZegoLiveRoomPlugin()
<ZegoRoomDelegate, ZegoIMDelegate, ZegoLiveEventDelegate, ZegoLivePublisherDelegate, ZegoLivePlayerDelegate, ZegoMediaSideDelegate, ZegoExternalVideoRenderDelegate, ZegoSoundLevelDelegate, FlutterStreamHandler>

@property (nonatomic, strong) ZegoLiveRoomApi *zegoApi;
@property (nonatomic, strong) ZegoMediaSideInfo * mediaSideInfoApi;
@property (nonatomic, strong) ZegoRendererController *renderController;
@property (nonatomic, assign) BOOL isEnablePlatformView;

@property (nonatomic, strong) NSObject<FlutterPluginRegistrar> *registrar;


@end

@implementation ZegoLiveRoomPlugin
{
    volatile FlutterEventSink _eventSink;
}

- (instancetype)initWithRegistrar:(NSObject<FlutterPluginRegistrar> *)registrar {
    if (self = [super init]) {
        
        _zegoApi = nil;
        _mediaSideInfoApi = nil;
        _renderController = nil;
        _isEnablePlatformView = NO;
        
        _registrar = registrar;
    }
    
    return self;
}

- (void)dealloc {
    
    [ZegoPlatformViewFactory release];
    
    if(self.zegoApi)
        self.zegoApi = nil;
    if(self.mediaSideInfoApi)
        self.mediaSideInfoApi = nil;
}

- (bool)numberToBoolValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? false : [number boolValue];
}

- (int)numberToIntValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0 : [number intValue];
}

- (unsigned int)numberToUintValue:(NSNumber *)number {
   
    return [number isKindOfClass:[NSNull class]] ? 0 : [number unsignedIntValue];
}

- (unsigned long)numberToULongValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0 : [number unsignedLongValue];
}

- (float)numberToFolatValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0.f : [number floatValue];
}

NSData* convertStringToSign(NSString* strSign) {
    
    if(strSign == nil || strSign.length == 0)
        return nil;
    
    strSign = [strSign lowercaseString];
    strSign = [strSign stringByReplacingOccurrencesOfString:@" " withString:@""];
    strSign = [strSign stringByReplacingOccurrencesOfString:@"0x" withString:@""];
    NSArray* szStr = [strSign componentsSeparatedByString:@","];
    int nLen = (int)[szStr count];
    nLen = nLen > 32 ? 32 : nLen;
    Byte szSign[32];
    for(int i = 0; i < nLen; i++)
    {
        NSString *strTmp = [szStr objectAtIndex:i];
        if(strTmp.length == 1)
            szSign[i] = toByte(strTmp);
        else
        {
            szSign[i] = toByte([strTmp substringWithRange:NSMakeRange(0, 1)]) << 4 | toByte([strTmp substringWithRange:NSMakeRange(1, 1)]);
        }
    }
    
    NSData *sign = [NSData dataWithBytes:szSign length:32];
    return sign;
}

Byte toByte(NSString* c) {
    NSString *str = @"0123456789abcdef";
    Byte b = [str rangeOfString:c].location;
    return b;
}

- (void)throwSdkNotInitError:(FlutterResult)result ofMethodName:(NSString *)methodName {
    NSString *errorMessage = [NSString stringWithFormat:@"[ERROR]: %@ %@", methodName, @"error because zegoliveroom api is not inited."];
    [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] %@", errorMessage]];
    result([FlutterError errorWithCode:[[NSString stringWithFormat:@"%@_ERROR", methodName] uppercaseString] message:errorMessage details:nil]);
}

- (void)throwNoRendererError:(FlutterResult)result ofMethodName:(NSString *)methodName {
    NSString *errorMessage = [NSString stringWithFormat:@"[ERROR]: %@ %@", methodName, @"error because zego preview or play renderer is null."];
    [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] %@", errorMessage]];
    result([FlutterError errorWithCode:[[NSString stringWithFormat:@"%@_ERROR", methodName] uppercaseString] message:errorMessage details:nil]);
}

- (void)throwNoTextureError:(FlutterResult)result ofMethodName:(NSString *)methodName {
    NSString *errorMessage = [NSString stringWithFormat:@"[ERROR]: %@ %@", methodName, @"error because \'enablePlatformView\' is true. make sure you turn off this api before calling \'initSDK\' when you use texture to render."];
    [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] %@", errorMessage]];
    result([FlutterError errorWithCode:[[NSString stringWithFormat:@"%@_ERROR", methodName] uppercaseString] message:errorMessage details:nil]);
}

- (void)throwNoPlatformViewError:(FlutterResult)result ofMethodName:(NSString *)methodName {
    NSString *errorMessage = [NSString stringWithFormat:@"[ERROR]: %@ %@", methodName, @"error because \'enablePlatformView\' is false. make sure you turn on this api before calling \'initSDK\' when you use platform view to render."];
    [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] %@", errorMessage]];
    result([FlutterError errorWithCode:[[NSString stringWithFormat:@"%@_ERROR", methodName] uppercaseString] message:errorMessage details:nil]);
}


#pragma mark - Handle Flutter CallMethods
- (void)initSDKWithAppID:(unsigned int)appID appSign: (NSString *)appsign result:(FlutterResult)result {
    
    [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] enter init sdk, app id: %u", appID]];
    
    NSData *appSign = convertStringToSign(appsign);
    if(appSign == nil)
        return;
    
    //默认使用外部渲染
    if(!self.isEnablePlatformView) {
    
        [ZegoExternalVideoRender enableExternalVideoRender:YES type:VideoExternalRenderTypeDecodeRgbSeries];
        self.renderController = [[ZegoRendererController alloc] init];
    }
    
    self.zegoApi = [[ZegoLiveRoomApi alloc] initWithAppID: appID appSignature:appSign completionBlock:^(int errorCode){
        
        [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] on init sdk, errorCode: %d", errorCode]];
        
        if(errorCode == 0)
        {
            //设置代理
            [self.zegoApi setRoomDelegate:self];
            [self.zegoApi setPublisherDelegate:self];
            [self.zegoApi setPlayerDelegate:self];
            [self.zegoApi setLiveEventDelegate:self];
            [self.zegoApi setIMDelegate:self];
            
            [[ZegoExternalVideoRender sharedInstance] setExternalVideoRenderDelegate:self];
            
            //初始化媒体次要信息模块
            self.mediaSideInfoApi = [[ZegoMediaSideInfo alloc] init];
            [self.mediaSideInfoApi setMediaSideDelegate:self];
        }
        
        result(@(errorCode));
        
    }];
    
}

#pragma mark - FlutterStreamHandler methods

- (FlutterError* _Nullable)onCancelWithArguments:(id _Nullable)arguments {
    
    [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] onCancel sink, object: %@", arguments]];
    _eventSink = nil;
    return nil;
}

- (FlutterError* _Nullable)onListenWithArguments:(id _Nullable)arguments
                                       eventSink:(nonnull FlutterEventSink)sink {
 
    _eventSink = sink;
    [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] onListen sink: %p, object: %@", _eventSink, arguments]];
    return nil;
}


#pragma mark - ZegoRoomDelegate

- (void)onStreamUpdated:(int)type streams:(NSArray<ZegoStream*> *)streamList roomID:(NSString *)roomID
{
    FlutterEventSink sink = _eventSink;
    [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] onStreamUpdated enter, sink: %p", sink]];
    if(sink) {
        
        NSMutableArray *streamArray = [NSMutableArray array];
        for (ZegoStream *stream in streamList)
        {
            [streamArray addObject:@{@"userID": stream.userID, @"userName": stream.userName, @"streamID": stream.streamID, @"extraInfo": stream.extraInfo}];
        }
        
        NSDictionary *dic = @{@"type": @(TYPE_ROOM_EVENT),
                              @"method": @{@"name": @"onStreamUpdated",
                                           @"updateType": @(type),
                                           @"roomID": roomID,
                                           @"streamList": streamArray}
                              };
        [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] onStreamUpdated, return map: %@", dic]];
        sink(dic);
    }
}


- (void)onStreamExtraInfoUpdated:(NSArray<ZegoStream *> *)streamList roomID:(NSString *)roomID
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        
        NSMutableArray *streamArray = [NSMutableArray array];
        for (ZegoStream *stream in streamList)
        {
            [streamArray addObject:@{@"userID": stream.userID, @"userName": stream.userName, @"streamID": stream.streamID, @"extraInfo": stream.extraInfo}];
        }
        
        sink(@{@"type": @(TYPE_ROOM_EVENT),
               @"method": @{@"name": @"onStreamExtraInfoUpdated",
                            @"roomID": roomID,
                            @"streamList": streamArray}
               });
    }
    
}

//房间连接失败
- (void)onDisconnect:(int)errorCode roomID:(NSString *)roomID
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_ROOM_EVENT),
               @"method": @{@"name": @"onDisconnect",
                            @"errorCode": @(errorCode),
                            @"roomID": roomID}
               });
    }
}

//房间重连成功
- (void)onReconnect:(int)errorCode roomID:(NSString *)roomID
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_ROOM_EVENT),
               @"method": @{@"name": @"onReconnect",
                            @"errorCode": @(errorCode),
                            @"roomID": roomID}
               });
    }
}

//房间连接暂时断开，SDK内部尝试重新连接
- (void)onTempBroken:(int)errorCode roomID:(NSString *)roomID
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_ROOM_EVENT),
               @"method": @{@"name": @"onTempBroken",
                            @"errorCode": @(errorCode),
                            @"roomID": roomID}
               });
    }
}

//用户被踢出房间
- (void)onKickOut:(int)reason roomID:(NSString *)roomID customReason:(NSString *)customReason
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_ROOM_EVENT),
               @"method": @{@"name": @"onKickOut",
                            @"reason": @(reason),
                            @"roomID": roomID,
                            @"customReason": customReason}
               });
    }
}

- (void)onReceiveCustomCommand:(NSString *)fromUserID userName:(NSString *)fromUserName content:(NSString*)content roomID:(NSString *)roomID {
    
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_ROOM_EVENT),
               @"method": @{@"name": @"onReceiveCustomCommand",
                            @"userID": fromUserID,
                            @"userName": fromUserName,
                            @"content": content,
                            @"roomID": roomID}
               });
    }
}

- (void)onUserUpdate:(NSArray<ZegoUserState *> *)userList updateType:(ZegoUserUpdateType)type {
    
    FlutterEventSink sink = _eventSink;
    if(sink) {
        
        NSMutableArray *userArray = [NSMutableArray array];
        for (ZegoUserState *user in userList)
        {
            [userArray addObject:@{@"userID": user.userID, @"userName": user.userName, @"updateFlag": @(user.updateFlag), @"role": @(user.role)}];
        }
        
        sink(@{@"type": @(TYPE_ROOM_EVENT),
               @"method": @{@"name": @"onUserUpdate",
                            @"updateType": @(type),
                            @"userList": userArray}
               });
    }
}

//音视频引擎开始时回调
- (void)onAVEngineStart
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_ROOM_EVENT),
               @"method": @{@"name": @"onAVEngineStart"}
               });
    }
}

//音视频引擎停止时回调
- (void)onAVEngineStop
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_ROOM_EVENT),
               @"method": @{@"name": @"onAVEngineStop"}
               });
    }
}

#pragma mark - ZegoLiveEventDelegate

- (void)zego_onLiveEvent:(ZegoLiveEvent)event info:(NSDictionary<NSString*, NSString*>*)info
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_ROOM_EVENT),
               @"method": @{@"name": @"onLiveEvent",
                            @"event": @(event),
                            @"info": info}
               });
    }
}

#pragma mark - ZegoLivePublisherDelegate

- (void)onPublishStateUpdate:(int)stateCode streamID:(NSString *)streamID streamInfo:(NSDictionary *)info{

    FlutterEventSink sink = _eventSink;
    [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] onPublishStateUpdate enter, sink: %p", sink]];
    if(sink) {
        NSDictionary *dic = @{@"type": @(TYPE_PUBLISH_EVENT),
                              @"method": @{@"name": @"onPublishStateUpdate",
                                           @"stateCode": @(stateCode),
                                           @"streamID": streamID,
                                           @"streamInfo": info}
                              };
        [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] onPublishStateUpdate, return map: %@", dic]];
        sink(dic);
    }
}

- (void)onPublishQualityUpdate:(NSString *)streamID quality:(ZegoApiPublishQuality)quality
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PUBLISH_EVENT),
               @"method": @{@"name": @"onPublishQualityUpdate",
                            @"streamID": streamID,
                            
                            @"acapFps": @(quality.acapFps),
                            @"anetFps": @(quality.afps),
                            @"akbps" : @(quality.akbps),
                            
                            @"vcapFps" : @(quality.cfps),
                            @"vencFps": @(quality.vencFps),
                            @"vnetFps" : @(quality.fps),
                            @"vkbps" : @(quality.kbps),
                            
                            @"rtt" : @(quality.rtt),
                            @"pktLostRate" : @(quality.pktLostRate),
                            
                            @"isHardwareVenc": @(quality.isHardwareVenc),
                            
                            @"width": @(quality.width),
                            @"height": @(quality.height),
                            
                            @"quality" : @(quality.quality)}
               });
    }
}

- (void)onRelayCDNStateUpdate:(NSArray<ZegoAPIStreamRelayCDNInfo *> *)statesInfo streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    if(sink) {
        NSMutableArray *infoArray = [NSMutableArray array];
        for (ZegoAPIStreamRelayCDNInfo *info in statesInfo) {
            [infoArray addObject:@{@"rtmpURL": info.rtmpURL, @"state": @(info.state), @"detail": @(info.detail), @"stateTime": @(info.stateTime)}];
        }
        
        sink(@{@"type": @(TYPE_PUBLISH_EVENT),
               @"method": @{@"name": @"onRelayCDNStateUpdate",
                            @"streamID": streamID,
                            @"statesInfo": infoArray}
               });
    }
}

- (void)onCaptureAudioFirstFrame {
    
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PUBLISH_EVENT),
               @"method": @{@"name": @"onCaptureAudioFirstFrame"}
               });
    }
}

- (void)onCaptureVideoFirstFrame {
    
    if(!self.isEnablePlatformView) {
        
        BOOL isUseFrontCam = [self.renderController isUseFrontCam];
        ZegoViewRenderer *preview = [self.renderController getRenderer:kZegoVideoDataMainPublishingStream];
        if(preview) {
        
            [preview setUseMirrorEffect:isUseFrontCam];
        }
    }
    
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PUBLISH_EVENT),
               @"method": @{@"name": @"onCaptureVideoFirstFrame"}
               });
    }
}

- (void)onCaptureVideoSizeChangedTo:(CGSize)size
{
    int width = size.width;
    int height = size.height;
    
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PUBLISH_EVENT),
               @"method": @{@"name": @"onCaptureVideoSizeChangedTo",
                            @"width": @(width),
                            @"height": @(height)}
               });
    }
}

/**
 收到连麦请求
 */
- (void)onJoinLiveRequest:(int)seq fromUserID:(NSString *)userId fromUserName:(NSString *)userName roomID:(NSString *)roomID
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PUBLISH_EVENT),
               @"method": @{@"name": @"onJoinLiveRequest",
                            @"seq": @(seq),
                            @"fromUserID": userId,
                            @"fromUserName": userName,
                            @"roomID": roomID}
               });
    }
}

#pragma mark - ZegoLivePlayerDelegate

- (void)onPlayStateUpdate:(int)stateCode streamID:(NSString *)streamID
{
    FlutterEventSink sink = _eventSink;
    [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] onPlayStateUpdate enter, sink: %p", sink]];
    if(sink) {
        NSDictionary *dic = @{@"type": @(TYPE_PLAY_EVENT),
                              @"method": @{@"name": @"onPlayStateUpdate",
                                           @"stateCode": @(stateCode),
                                           @"streamID": streamID}
                              };
        [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] onPlayStateUpdate, return map: %@", dic]];
        sink(dic);
    }
}

- (void)onPlayQualityUpate:(NSString *)streamID quality:(ZegoApiPlayQuality)quality
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type" : @(TYPE_PLAY_EVENT),
               @"method": @{@"name" : @"onPlayQualityUpdate",
                            @"streamID" : streamID,
                            
                            @"vnetFps" : @(quality.fps),
                            @"vdjFps": @(quality.vdjFps),
                            @"vdecFps": @(quality.vdecFps),
                            @"vrndFps": @(quality.vrndFps),
                            @"vkbps" : @(quality.kbps),
                            
                            @"anetFps": @(quality.afps),
                            @"adjFps":@(quality.adjFps),
                            @"adecFps":@(quality.adecFps),
                            @"arndFps":@(quality.arndFps),
                            @"akbps" : @(quality.akbps),
                            
                            @"audioBreakRate" : @(quality.audioBreakRate),
                            @"videoBreakRate":@(quality.videoBreakRate),
                            @"rtt" : @(quality.rtt),
                            @"p2pRtt": @(quality.peerToPeerDelay),
                            @"pktLostRate" : @(quality.pktLostRate),
                            @"p2pPktLostRate": @(quality.peerToPeerPktLostRate),
                            @"quality" : @(quality.quality),
                            @"delay" : @(quality.delay),
                            
                            @"isHardwareVdec": @(quality.isHardwareVdec),
                            
                            @"width": @(quality.width),
                            @"height": @(quality.height)
                            }
               });
    }
}

- (void)onVideoSizeChangedTo:(CGSize)size ofStream:(NSString *)streamID
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PLAY_EVENT),
               @"method": @{@"name": @"onVideoSizeChangedTo",
                            @"streamID": streamID,
                            @"width": @((int)size.width),
                            @"height": @((int)size.height)
                            }
               });
    }
}

/**
 收到主播端的邀请连麦请求
 */
- (void)onInviteJoinLiveRequest:(int)seq fromUserID:(NSString *)userId fromUserName:(NSString *)userName roomID:(NSString *)roomID
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PLAY_EVENT),
               @"method": @{@"name": @"onInviteJoinLiveRequest",
                            @"seq": @(seq),
                            @"fromUserID": userId,
                            @"fromUserName": userName,
                            @"roomID": roomID
                            }
               });
    }
}

/**
 收到结束连麦信令
 */
- (void)onEndJoinLiveCommad:(NSString *)fromUserId userName:(NSString *)fromUserName roomID:(NSString *)roomID
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PLAY_EVENT),
               @"method": @{@"name": @"onRecvEndJoinLiveCommand",
                            @"fromUserID": fromUserId,
                            @"fromUserName": fromUserName,
                            @"roomID": roomID
                            }
               });
    }
}

/**
 远端摄像头状态通知
 */
- (void)onRemoteCameraStatusUpdate:(int)status ofStream:(NSString *)streamID reason:(int)reason {
    
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PLAY_EVENT),
               @"method": @{@"name": @"onRemoteCameraStatusUpdate",
                            @"status": @(status),
                            @"streamID": streamID,
                            @"reason": @(reason)
                            }
               });
    }
}

/**
 远端麦克风状态通知
 */
- (void)onRemoteMicStatusUpdate:(int)status ofStream:(NSString *)streamID reason:(int)reason {
    
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PLAY_EVENT),
               @"method": @{@"name": @"onRemoteMicStatusUpdate",
                            @"status": @(status),
                            @"streamID": streamID,
                            @"reason": @(reason)
                            }
               });
    }
}

/**
 接收到远端音频的首帧通知
 */

- (void)onRecvRemoteAudioFirstFrame:(NSString *)streamID {
    
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PLAY_EVENT),
               @"method": @{@"name": @"onRecvRemoteAudioFirstFrame",
                            @"streamID": streamID,
                            }
               });
    }
}

/**
 接收到远端视频的首帧通知
 */
- (void)onRecvRemoteVideoFirstFrame:(NSString *)streamID {
    
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PLAY_EVENT),
               @"method": @{@"name": @"onRecvRemoteVideoFirstFrame",
                            @"streamID": streamID,
                            }
               });
    }
}

/**
 远端视频渲染首帧通知
 */
- (void)onRenderRemoteVideoFirstFrame:(NSString *)streamID {
    
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_PLAY_EVENT),
               @"method": @{@"name": @"onRenderRemoteVideoFirstFrame",
                            @"streamID": streamID,
                            }
               });
    }
}

#pragma mark - ZegoMediaSideDelegate

- (void)onRecvMediaSideInfo:(NSData *)data ofStream:(NSString *)streamID
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        
        uint32_t mediaType = ntohl(*(uint32_t*)data.bytes);
        NSData *mediaInfo;
        if(mediaType == 1001 || mediaType == 1002) {
            
            mediaInfo = [data subdataWithRange:NSMakeRange(4, data.length - 4)];
        } else {
            
            mediaInfo = [data subdataWithRange:NSMakeRange(5, data.length - 5)];
        }
        
        NSString * str_data = [[NSString alloc] initWithData:mediaInfo encoding:NSUTF8StringEncoding];
        sink(@{@"type": @(TYPE_MEDIA_SIDE_INFO_EVENT),
               @"method" : @{@"name" : @"onRecvMediaSideInfo",
                             @"streamID": streamID,
                             @"data" : str_data == nil ? @"" : str_data}
               });
    }
}

#pragma mark - ZegoSoundLevelDelegate

- (void)onSoundLevelUpdate:(NSArray<ZegoSoundLevelInfo *> *)soundLevels {
    FlutterEventSink sink = _eventSink;
    if(sink) {
        
        NSMutableArray *soundLevelList = [NSMutableArray array];
        for(ZegoSoundLevelInfo *info in soundLevels) {
            [soundLevelList addObject:@{@"streamID": info.streamID,
                                        @"soundLevel": @(info.soundLevel)
                                        }];
        }
        
        sink(@{@"type": @(TYPE_SOUND_LEVEL_EVENT),
               @"method" : @{@"name" : @"onSoundLevelUpdate",
                             @"soundLevels" : soundLevelList}
               });
    }
}

- (void)onCaptureSoundLevelUpdate:(ZegoSoundLevelInfo *)captureSoundLevel {
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_SOUND_LEVEL_EVENT),
               @"method" : @{@"name" : @"onCaptureSoundLevelUpdate",
                             @"streamID": captureSoundLevel.streamID,
                             @"soundLevel" : @(captureSoundLevel.soundLevel)}
               });
    }
}

#pragma mark - ZegoLiveApiRenderDelegate

- (CVPixelBufferRef)onCreateInputBufferWithWidth:(int)width height:(int)height cvPixelFormatType:(OSType)cvPixelFormatType streamID:(NSString *)streamID
{
    if(![self.renderController isRendering]) {
        [ZegoLog logNotice:@"[onCreateInputBufferWithWidth] render controller is not rendering, ignore"];
        return nil;
    }
    
    ZegoPixelBufferPool *pool_ = [self.renderController getPixelBufferPool:streamID];
    
    if(pool_ == nil) {
        [self.renderController createPixelBufferPool:width height:height streamID:streamID];
        pool_ = [self.renderController getPixelBufferPool:streamID];
        
        if(pool_ == nil)
            return nil;
    }
    
    if(width != [pool_ getWidth] || height != [pool_ getHeight]) {
        [ZegoLog logNotice:[NSString stringWithFormat:@"[onCreateInputBufferWithWidth] resize pixelbuffer pool, old width: %d, old height: %d, new width: %d, new height: %d", [pool_ getWidth], [pool_ getHeight], width, height]];
        
        [self.renderController createPixelBufferPool:width height:height streamID:streamID];
        pool_ = [self.renderController getPixelBufferPool:streamID];
    }
    
    CVPixelBufferRef pixelBuffer;
    CVReturn ret = CVPixelBufferPoolCreatePixelBuffer(nil, [pool_ getBufferPool], &pixelBuffer);
    if (ret != kCVReturnSuccess) {
        [ZegoLog logNotice:[NSString stringWithFormat:@"[onCreateInputBufferWithWidth] pixelbuffer pool creates pixelbuffer failed, error: %d", ret]];
        return nil;
    }
   
    return pixelBuffer;
    
}

- (void)onPixelBufferCopyed:(CVPixelBufferRef)pixelBuffer streamID:(NSString *)streamID
{
    ZegoViewRenderer *renderer = [self.renderController getRenderer:streamID];
    if(renderer == nil) {
        [ZegoLog logNotice:[NSString stringWithFormat:@"[onPixelBufferCopyed] renderer %@ has been released, delete buffer", streamID]];
        CVBufferRelease(pixelBuffer);
        return;
    }
    
    //Notify the Flutter new pixelBufferRef to be ready.
    [renderer setSrcFrameBuffer:pixelBuffer processBuffer:nil];
}

- (void)onSetFlipMode:(int)mode streamID:(NSString *)streamID {
    ZegoViewRenderer *renderer = [self.renderController getRenderer:kZegoVideoDataMainPublishingStream];
    if (renderer == nil) {
        return;
    }
    
    // Need to manually flip the frame when mode == 1
    [renderer setUseMirrorEffect:mode == 1];
}


+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
    
    ZegoLiveRoomPlugin* instance = [[ZegoLiveRoomPlugin alloc] initWithRegistrar:registrar];
    /*Create Method Channel.*/
    FlutterMethodChannel* channel = [FlutterMethodChannel
                                     methodChannelWithName:@"plugins.zego.im/zegoliveroom_plugin"
                                     binaryMessenger:[registrar messenger]];
    [registrar addMethodCallDelegate:instance channel:channel];
    
    /*Create Event Channel.*/
    FlutterEventChannel* eventChannel = [FlutterEventChannel
                                         eventChannelWithName:@"plugins.zego.im/zegoliveroom_callback"
                                         binaryMessenger:[registrar messenger]];
    
    
    [eventChannel setStreamHandler:instance];
    [registrar registerViewFactory:[ZegoPlatformViewFactory shareInstance] withId:@"plugins.zego.im/zego_view"];
}

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
    
    NSDictionary *args = call.arguments;
    
    /* LiveRoom */
    if ([@"getSdkVersion" isEqualToString:call.method]) {
        
        result([ZegoLiveRoomApi version]);
        
    } else if([@"getEngineVersion" isEqualToString:call.method]) {
        
        result([ZegoLiveRoomApi version2]);
    
    } else if([@"setVerbose" isEqualToString: call.method]) {
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        [ZegoLiveRoomApi setVerbose: enable];
        result(nil);
    
    } else if([@"uploadLog" isEqualToString:call.method]) {
        
        [ZegoLiveRoomApi uploadLog];
        result(nil);
    
    } else if([@"getLogDir" isEqualToString:call.method]) {
        
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
        if([paths count] > 0) {
            NSString *logPath = [NSString stringWithFormat:@"%@/ZegoLogs", paths.firstObject];
            result(logPath);
        }else {
            result(nil);
        }
        
    } else if([@"initSDK" isEqualToString:call.method]) {
        
        unsigned int appID = [self numberToUintValue:args[@"appID"]];
        NSString *strAppSign = args[@"appSign"];
        
        [self initSDKWithAppID:appID appSign:strAppSign result:result];
        
    } else if([@"uninitSDK" isEqualToString:call.method]){
        
        if(self.zegoApi == nil) {
            [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] repeat uninit sdk, ignore"]];
            result(@NO);
        }
        else {
            
            if(!self.isEnablePlatformView) {
                
                [ZegoExternalVideoRender enableExternalVideoRender:NO type:VideoExternalRenderTypeDecodeRgbSeries];
                self.renderController = nil;
            }
            
            [[ZegoExternalVideoRender sharedInstance] setExternalVideoRenderDelegate:nil];
            
            [self.mediaSideInfoApi setMediaSideDelegate:nil];
            self.mediaSideInfoApi = nil;
            
            [self.zegoApi setRoomDelegate:nil];
            //[self.zegoApi setRenderDelegate:nil];
            [[ZegoExternalVideoRender sharedInstance] setExternalVideoRenderDelegate:nil];
            [self.zegoApi setPublisherDelegate:nil];
            [self.zegoApi setPlayerDelegate:nil];
            [self.zegoApi setLiveEventDelegate:nil];
            [self.zegoApi setIMDelegate:nil];
            self.zegoApi = nil;
            [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] unInitSDK"]];
            result(@YES);
        }
        
    } else if([@"enablePlatformView" isEqualToString:call.method]) {
    
        BOOL enable = [self numberToBoolValue:args[@"enable"]];
        _isEnablePlatformView = enable;
        result(nil);
        
    } else if([@"setUser" isEqualToString:call.method]) {
        
        NSString *userID = args[@"userID"];
        NSString *userName = args[@"userName"];
      
        BOOL success = [ZegoLiveRoomApi setUserID:userID userName:userName];
        result(@(success));
        
    } else if([@"setUseTestEnv" isEqualToString:call.method]) {
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        [ZegoLiveRoomApi setUseTestEnv:enable];
        result(nil);
        
    } else if([@"loginRoom" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *roomID = args[@"roomID"];
        NSString *roomName = args[@"roomName"];
        int role = [self numberToIntValue:args[@"role"]];
        [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] loginRoom enter, roomID: %@, roomName: %@, role: %d", roomID, roomName, role]];
        BOOL success = [self.zegoApi loginRoom:roomID roomName:roomName role:role withCompletionBlock:^(int errorCode, NSArray<ZegoStream *> *streamList) {
            
              
            NSMutableArray *streamArray = [NSMutableArray array];
            for (ZegoStream *stream in streamList) {
                
                    [streamArray addObject:@{@"userID": stream.userID, @"userName": stream.userName, @"streamID": stream.streamID, @"extraInfo": stream.extraInfo}];
            }
            
            NSDictionary *dic = @{@"errorCode": @(errorCode), @"streamList": streamArray};
            [ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] onLoginRoom, return map: %@", dic]];
            result(dic);
              
          }];
        
        if(!success) {
            result([FlutterError errorWithCode:[[NSString stringWithFormat:@"%@_ERROR", call.method] uppercaseString] message:@"call \'loginRoom\' failed, please check you call \'setUser\' first" details:nil]);
        }
        
    } else if([@"logoutRoom" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        BOOL success = [self.zegoApi logoutRoom];
        result(@(success));
        
    } else if([@"pauseModule" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSNumber *type = args[@"type"];
        [self.zegoApi pauseModule:[type intValue]];
        result(nil);
        
    } else if([@"resumeModule" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSNumber *type = args[@"type"];
        [self.zegoApi resumeModule:(int)[type intValue]];
        result(nil);

    } else if([@"enableMicDevice" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        BOOL success = [self.zegoApi enableMicDevice: enable];
        result(@(success));
        
    } else if([@"sendCustomCommand" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSArray *userList = args[@"users"];
        NSString *content = args[@"content"];
        
        NSMutableArray *zegoUserList = [NSMutableArray array];
        for(NSMutableDictionary *objUser in userList) {
            ZegoUser *user = [[ZegoUser alloc] init];
            user.userId = objUser[@"userID"];
            user.userName = objUser[@"userName"];
            [zegoUserList addObject:user];
        }
        
        [self.zegoApi sendCustomCommand:zegoUserList content:content completion:^(int errorCode, NSString *roomID) {
                
            result(@{@"errorCode": @(errorCode),
                         @"roomID": roomID
                    });
        }];
        
    } else if([@"setRoomConfig" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool audienceCreate = [self numberToBoolValue:args[@"audienceCreateRoom"]];
        bool userStateUpdate = [self numberToBoolValue:args[@"userStateUpdate"]];
        [self.zegoApi setRoomConfig:audienceCreate userStateUpdate:userStateUpdate];
        result(nil);
       
    }
    /* LiveRoom-Publisher */
    else if([@"setLatencyMode" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int mode = [self numberToIntValue:args[@"mode"]];
        [self.zegoApi setLatencyMode:(ZegoAPILatencyMode)mode];
        result(nil);
        
    } else if([@"setVideoMirrorMode" isEqualToString:call.method]) {
        
        if (self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
        }
        
        int mode = [self numberToIntValue:args[@"mode"]];
        BOOL success = [self.zegoApi setVideoMirrorMode:(ZegoVideoMirrorMode)mode];
        result(@(success));
        
    } else if([@"setAudioDeviceMode" isEqualToString:call.method]) {
        
        int mode = [self numberToIntValue:args[@"mode"]];
        [ZegoLiveRoomApi setAudioDeviceMode:(ZegoAPIAudioDeviceMode)mode];
        result(nil);

    } else if([@"startPreview" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        BOOL success = [self.zegoApi startPreview];
        result(@(success));
       
    } else if([@"stopPreview" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        BOOL success = [self.zegoApi stopPreview];
        result(@(success));

    } else if([@"startPublishing" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        NSString *title = args[@"title"];
        int flag = [self numberToIntValue:args[@"flag"]];
        NSString *extraInfo = args[@"extraInfo"];
        
        BOOL success;
        if(![extraInfo isKindOfClass:[NSNull class]]) {
            success = [self.zegoApi startPublishing:streamID title:title flag:flag extraInfo:extraInfo];
        }
        else {
            success = [self.zegoApi startPublishing:streamID title:title flag:flag];
        }
        
        result(@(success));

    } else if([@"stopPublishing" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        BOOL success = [self.zegoApi stopPublishing];
        result(@(success));

    } else if([@"setVideoCodecId" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int codecID = [self numberToIntValue:args[@"codecID"]];
        BOOL success = [self.zegoApi setVideoCodecId:(ZegoVideoCodecAvc)codecID ofChannel:ZEGOAPI_CHN_MAIN];
        result(@(success));

    } else if([@"updateStreamExtraInfo" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *extraInfo = args[@"extraInfo"];
        BOOL success = [self.zegoApi updateStreamExtraInfo:extraInfo];
        result(@(success));

    } else if([@"enableMic" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        
        BOOL success = [self.zegoApi enableMic: enable];
        result(@(success));

    } else if([@"enableCamera" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        
        BOOL success = [self.zegoApi enableCamera:enable];
        result(@(success));

    }
    else if([@"setFrontCam" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        BOOL success = [self.zegoApi setFrontCam:enable];
        
        if(success && !self.isEnablePlatformView) {
                
            [self.renderController setIsUseFrontCam:enable];
        }
        result(@(success));

    } else if([@"setAVConfig" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }

        int captureWidth = [self numberToIntValue:args[@"captureWidth"]];
        int captureHeight = [self numberToIntValue:args[@"captureHeight"]];
        int encodeWidth = [self numberToIntValue:args[@"encodeWidth"]];
        int encodeHeight = [self numberToIntValue:args[@"encodeHeight"]];
        int bitrate = [self numberToIntValue:args[@"bitrate"]];
        int fps = [self numberToIntValue:args[@"fps"]];
        
        ZegoAVConfigPreset preset = ZegoAVConfigPreset_Generic;
        ZegoAVConfig *avConfig = [ZegoAVConfig presetConfigOf: preset];
        avConfig.videoCaptureResolution = CGSizeMake(captureWidth, captureHeight);
        avConfig.videoEncodeResolution = CGSizeMake(encodeWidth, encodeHeight);
        avConfig.bitrate = bitrate;
        avConfig.fps = fps;

        BOOL success = [self.zegoApi setAVConfig:avConfig];
        result(@(success));

    } else if([@"requireHardwareEncoder" isEqualToString:call.method]) {
        
        bool bRequire = [self numberToBoolValue:args[@"bRequire"]];
        BOOL success = [ZegoLiveRoomApi requireHardwareEncoder:bRequire];
        result(@(success));
        
    } else if([@"enableBeautifying" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int feature = [self numberToIntValue:args[@"feature"]];
        BOOL success = [self.zegoApi enableBeautifying:feature];
        result(@(success));

    } else if([@"setPolishStep" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        float step = [self numberToFolatValue:args[@"step"]];
        BOOL success = [self.zegoApi setPolishStep:step];
        result(@(success));

    } else if([@"setPolishFactor" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        float factor = [self numberToFolatValue:args[@"factor"]];
        BOOL success = [self.zegoApi setPolishFactor:factor];
        result(@(success));

    } else if([@"setWhitenFactor" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        float factor = [self numberToFolatValue:args[@"factor"]];
        BOOL success = [self.zegoApi setWhitenFactor:factor];
        result(@(success));

    } else if([@"setFilter" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int filter = [self numberToIntValue:args[@"filter"]];
        bool success = [self.zegoApi setFilter:(ZegoFilter)filter];
        result(@(success));

    } else if([@"enableTrafficControl" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool bEnable = [self numberToBoolValue:args[@"enable"]];
        NSUInteger properties = [self numberToULongValue:args[@"properties"]];
        [self.zegoApi enableTrafficControl:bEnable properties:properties];
        result(nil);

    } else if([@"setMinVideoBitrateForTrafficControl" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int bitrate = [self numberToIntValue:args[@"bitrate"]];
        int mode = [self numberToIntValue:args[@"mode"]];
        [self.zegoApi setMinVideoBitrateForTrafficControl:bitrate mode:(ZegoAPITrafficControlMinVideoBitrateMode)mode];
        result(nil);

    } else if([@"createPreviewRenderer" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(self.isEnablePlatformView) {
            [self throwNoTextureError:result ofMethodName:call.method];
            return;
        }
        
        int width = [self numberToIntValue:args[@"width"]];
        int height = [self numberToIntValue:args[@"height"]];
        
        ZegoViewRenderer *renderer = [[ZegoViewRenderer alloc] initWithTextureRegistry:[self.registrar textures] isPublisher:YES viewWidth:width viewHeight:height];
            
        if([self.renderController addRenderer:renderer ofKey:kZegoVideoDataMainPublishingStream]) {
                
            if(![self.renderController isRendering])
                [self.renderController startRendering];
                
            result(@(renderer.textureID));
        }
        else {
            result(@(-1));
        }
        
    } else if([@"setPreviewViewMode" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(self.isEnablePlatformView) {
            [self throwNoTextureError:result ofMethodName:call.method];
            return;
        }
        
        ZegoViewRenderer *renderer = [self.renderController getRenderer:kZegoVideoDataMainPublishingStream];
        if(renderer == nil) {
            [self throwNoRendererError:result ofMethodName:call.method];
            return;
        }
        
        int mode = [self numberToIntValue:args[@"mode"]];
                
        [renderer setVideoMode:(ZegoRendererViewMode)mode];
        result(@(YES));
        
    } else if([@"updatePreviewRenderSize" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        ZegoViewRenderer *renderer = [self.renderController getRenderer:kZegoVideoDataMainPublishingStream];
        if(renderer == nil) {
            [self throwNoRendererError:result ofMethodName:call.method];
            return;
        }
        
        int width = [self numberToIntValue:args[@"width"]];
        int height = [self numberToIntValue:args[@"height"]];
        
        [renderer updateRenderSize:CGSizeMake(width, height)];
        result(nil);

    } else if([@"destroyPreviewRenderer" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(self.isEnablePlatformView) {
            [self throwNoTextureError:result ofMethodName:call.method];
            return;
        }
        
        if([self.renderController removeRenderer:kZegoVideoDataMainPublishingStream]) {
                
            if([self.renderController getRenderCount] == 0) {
                
                if([self.renderController isRendering])
                    [self.renderController stopRendering];
            }
                
            [self.renderController destroyPixelBufferPool:kZegoVideoDataMainPublishingStream];
            result(@(YES));
            
            
        } else {
                
            result(@(NO));
        }

    } else if([@"setPreviewView" isEqualToString:call.method]) {
      
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(!self.isEnablePlatformView) {
            [self throwNoPlatformViewError:result ofMethodName:call.method];
            return;
        }
        
        int viewID = [self numberToIntValue:args[@"viewID"]];
        ZegoPlatformView * view = [[ZegoPlatformViewFactory shareInstance] getPlatformView:@(viewID)];
        if(view) {
            
            [self.zegoApi setPreviewView:[view getUIView]];
            result(@(YES));
        } else {
            result(@(NO));
        }
        
    } else if([@"setPlatformViewPreviewViewMode" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(!self.isEnablePlatformView) {
            [self throwNoPlatformViewError:result ofMethodName:call.method];
            return;
        }
        
        int mode = [self numberToIntValue:args[@"mode"]];
        
        bool success = [self.zegoApi setPreviewViewMode:(ZegoVideoViewMode)mode];
        result(@(success));
        
    } else if([@"removePreviewPlatformView" isEqualToString:call.method]){
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(!self.isEnablePlatformView) {
            [self throwNoPlatformViewError:result ofMethodName:call.method];
            return;
        }
        
        int viewID = [self numberToIntValue:args[@"viewID"]];
        BOOL success = [[ZegoPlatformViewFactory shareInstance]  removeView:@(viewID)];
        result(@(success));
        
    } else if([@"setAppOrientation" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int orientation = [self numberToIntValue:args[@"orientation"]];
        
        UIInterfaceOrientation obj_orientation;
        switch (orientation) {
            case 0:
                obj_orientation = UIInterfaceOrientationPortrait;
                break;
            case 1:
                obj_orientation = UIInterfaceOrientationLandscapeRight;
                break;
            case 2:
                obj_orientation = UIInterfaceOrientationPortraitUpsideDown;
                break;
            case 3:
                obj_orientation = UIInterfaceOrientationLandscapeLeft;
                break;
            default:
                obj_orientation = UIInterfaceOrientationUnknown;
                break;
        }
        BOOL success = [self.zegoApi setAppOrientation:obj_orientation];
        result(@(success));
 
    } else if ([@"setPublishConfig" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *target = args[@"rtmpURL"];
        [self.zegoApi setPublishConfig:@{kPublishCustomTarget: target}];
        result(nil);
    
    } else if ([@"addPublishTarget" isEqualToString:call.method]) {
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        

        NSString *target = args[@"target"];
        NSString *stream_ID = args[@"streamID"];
        [self.zegoApi addPublishTarget:target streamID:stream_ID completion:^(int errorCode, NSString *streamID) {
            NSDictionary *dic = @{@"errorCode": @(errorCode), @"streamID": streamID};
            result(dic);
        }];
    
    } else if ([@"deletePublishTarget" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *target = args[@"target"];
        NSString *stream_ID = args[@"streamID"];
        [self.zegoApi deletePublishTarget:target streamID:stream_ID completion:^(int errorCode, NSString *streamID) {
            NSDictionary *dic = @{@"errorCode": @(errorCode), @"streamID": streamID};
            result(dic);
        }];
    
    } else if([@"respondJoinLiveReq" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int seq = [self numberToIntValue:args[@"seq"]];
        int rspResult = [self numberToIntValue:args[@"rspResult"]];
        
        BOOL success = [self.zegoApi respondJoinLiveReq:seq result:rspResult];
        result(@(success));

    } else if([@"inviteJoinLive" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *userID = args[@"userID"];
        [self.zegoApi inviteJoinLive:userID responseBlock:^(int ret, NSString *fromUserID, NSString *fromUserName) {
                
            result(@{@"result":@(ret),
                         @"fromUserID": fromUserID,
                         @"fromUserName": fromUserName
                    });
        }];

    } else if([@"endJoinLive" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *userID = args[@"userID"];
        [self.zegoApi endJoinLive:userID completionBlock:^(int errorCode, NSString *roomID) {
               
            result(@{@"errorCode":@(errorCode),
                         @"roomID":roomID
                    });
        }];

    } else if([@"setAudioBitrate" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int bitrate = [self numberToIntValue:args[@"bitrate"]];
        BOOL success = [self.zegoApi setAudioBitrate:bitrate];
        result(@(success));
        
    } else if([@"enableAEC" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        bool success = [self.zegoApi enableAEC:enable];
        result(@(success));
        
    } else if([@"setAECMode" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int mode = [self numberToIntValue:args[@"mode"]];
        [self.zegoApi setAECMode:(ZegoAPIAECMode)mode];
        result(nil);
        
    } else if([@"enableAGC" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        bool success = [self.zegoApi enableAGC:enable];
        result(@(success));
        
    } else if([@"enableVAD" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        [self.zegoApi enableVAD:enable];
        result(nil);

    } else if([@"enableANS" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        bool success = [self.zegoApi enableNoiseSuppress:enable];
        result(@(success));
    }
    else if([@"enableDTX" isEqualToString:call.method])
    {
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        [self.zegoApi enableDTX:enable];
        result(nil);

    }
    /* LiveRoom-Player */
    else if([@"startPlayingStream" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        NSDictionary *info = args[@"info"];
        BOOL success;
        
        ZegoPlatformView *view = nil;
        if(self.isEnablePlatformView) {
            int viewID = [self numberToIntValue:args[@"viewID"]];
            view = [[ZegoPlatformViewFactory shareInstance]  getPlatformView:@(viewID)];
            if(view == nil) {
                result(@(NO));
                return;
            }
        }
        
        if(![info isKindOfClass:[NSNull class]]) {
            NSString *params = info[@"params"];
            NSArray *rtmpUrls = info[@"rtmpUrls"];
            NSArray *flvUrls = info[@"flvUrls"];
            
            ZegoAPIStreamExtraPlayInfo *extraPlayInfo = [ZegoAPIStreamExtraPlayInfo new];
            extraPlayInfo.params = params;
            extraPlayInfo.rtmpUrls = rtmpUrls;
            extraPlayInfo.flvUrls = flvUrls;
            
            success = [self.zegoApi startPlayingStream:streamID inView:view ? [view getUIView] : nil extraInfo:extraPlayInfo];
        } else {
            
            success = [self.zegoApi startPlayingStream:streamID inView:view ? [view getUIView] : nil];
        }
        
        result(@(success));
 
    } else if([@"stopPlayingStream" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        BOOL success = [self.zegoApi stopPlayingStream:streamID];
        result(@(success));
  
    } else if([@"activateAudioPlayStream" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        bool active = [self numberToBoolValue:args[@"active"]];
        
        int errorCode = [self.zegoApi activateAudioPlayStream:streamID active:active];
        result(@(errorCode));
 
    } else if([@"activateVideoPlayStream" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        bool active = [self numberToBoolValue:args[@"active"]];
        //int videoStreamLayer = [self numberToIntValue:args[@"layer"]];
        NSNumber *objVideoStreamLayer = args[@"layer"];
        int errorCode;
        
        if(![objVideoStreamLayer isKindOfClass:[NSNull class]]) {
            
            int videoStreamLayer =[self numberToIntValue:objVideoStreamLayer];
            errorCode = [self.zegoApi activateVideoPlayStream:streamID active:active videoLayer:(VideoStreamLayer)videoStreamLayer];
        } else {
            
            errorCode = [self.zegoApi activateVideoPlayStream:streamID active:active];
        }
        
        result(@(errorCode));
  
    } else if([@"requireHardwareDecoder" isEqualToString:call.method]) {
        
        bool bRequire = [self numberToBoolValue:args[@"bRequire"]];
        BOOL success = [ZegoLiveRoomApi requireHardwareDecoder:bRequire];
        result(@(success));
        
    } else if([@"createPlayViewRenderer" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(self.isEnablePlatformView) {
            [self throwNoTextureError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        int width = [self numberToIntValue:args[@"width"]];
        int height = [self numberToIntValue:args[@"height"]];
        
        ZegoViewRenderer *renderer = [[ZegoViewRenderer alloc] initWithTextureRegistry:[self.registrar textures] isPublisher:NO viewWidth:width viewHeight:height];
            
        if([self.renderController addRenderer:renderer ofKey:streamID]) {
                
            if(![self.renderController isRendering])
                [self.renderController startRendering];
                
            result(@(renderer.textureID));
        }
        else {
            result(@(-1));
        }

    } else if([@"setViewMode" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(self.isEnablePlatformView) {
            [self throwNoTextureError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        int mode = [self numberToIntValue:args[@"mode"]];
        
        ZegoViewRenderer *renderer = [self.renderController getRenderer:streamID];
        if(renderer == nil) {
            [self throwNoRendererError:result ofMethodName:call.method];
            return;
        }
        
        [renderer setVideoMode:(ZegoRendererViewMode)mode];
        result(@(YES));

    }else if([@"updatePlayViewRenderSize" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(self.isEnablePlatformView) {
            [self throwNoTextureError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        int width = [self numberToIntValue:args[@"width"]];
        int height = [self numberToIntValue:args[@"height"]];
        
        ZegoViewRenderer *renderer = [self.renderController getRenderer:streamID];
        if(renderer == nil) {
            [self throwNoRendererError:result ofMethodName:call.method];
            return;
        }
                
        [renderer updateRenderSize:CGSizeMake(width, height)];
        result(nil);
        
    } else if([@"destroyPlayViewRenderer" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(self.isEnablePlatformView) {
            [self throwNoTextureError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        if([self.renderController removeRenderer:streamID]) {
                
            if([self.renderController getRenderCount] == 0) {
                
                //当前没有用户推拉流，则停止渲染器并释放资源
                if([self.renderController isRendering])
                    [self.renderController stopRendering];
            }
                    
            [self.renderController destroyPixelBufferPool:streamID];
            result(@(YES));
        } else {
                
            result(@(NO));
        }
  
    } else if([@"updatePlayView" isEqualToString:call.method]) {
      
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(!self.isEnablePlatformView) {
            [self throwNoPlatformViewError:result ofMethodName:call.method];
            return;
        }
        
        int viewID = [self numberToIntValue:args[@"viewID"]];
        NSString *streamID = args[@"streamID"];
        
        ZegoPlatformView *view = [[ZegoPlatformViewFactory shareInstance] getPlatformView:@(viewID)];
        [self.zegoApi updatePlayView:[view getUIView] ofStream:streamID];
        result(nil);
        
    } else if([@"setPlatformViewPlayViewMode" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(!self.isEnablePlatformView) {
            [self throwNoPlatformViewError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        int mode = [self numberToIntValue:args[@"mode"]];
        
        bool success = [self.zegoApi setViewMode:(ZegoVideoViewMode)mode ofStream:streamID];
        result(@(success));
        
    } else if([@"removePlayPlatformView" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        if(!self.isEnablePlatformView) {
            [self throwNoPlatformViewError:result ofMethodName:call.method];
            return;
        }
        
        int viewID = [self numberToIntValue:args[@"viewID"]];
        BOOL success = [[ZegoPlatformViewFactory shareInstance]  removeView:@(viewID)];
        result(@(success));
        
    } else if([@"requestJoinLive" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        [self.zegoApi requestJoinLive:^(int ret, NSString *fromUserID, NSString *fromUserName) {
                
            result(@{@"result":@(ret),
                     @"fromUserID": fromUserID,
                     @"fromUserName": fromUserName
                    });
        }];
        
    } else if([@"respondInviteJoinLiveReq" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int seq = [self numberToIntValue:args[@"seq"]];
        int rspResult = [self numberToIntValue:args[@"rspResult"]];
        
        BOOL success = [self.zegoApi respondInviteJoinLiveReq:seq result:rspResult];
        result(@(success));

    } else if([@"enableSpeaker" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        bool success = [self.zegoApi enableSpeaker:enable];
        result(@(success));
        
    } else if([@"setBuiltInSpeakerOn" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool on = [self numberToBoolValue:args[@"bOn"]];
        bool success = [self.zegoApi setBuiltInSpeakerOn:on];
        result(@(success));
        
    }
    /* Media Side Info */
    else if([@"setMediaSideFlags" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil || self.mediaSideInfoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool start = [self numberToBoolValue:args[@"start"]];
        bool onlyAudioPublish = [self numberToBoolValue:args[@"onlyAudioPublish"]];
        
        [self.mediaSideInfoApi setMediaSideFlags:start onlyAudioPublish:onlyAudioPublish channelIndex: ZEGOAPI_CHN_MAIN];
        result(nil);
        
    } else if([@"sendMediaSideInfo" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil || self.mediaSideInfoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *strData = args[@"data"];
        
        NSData *data = [strData dataUsingEncoding:NSUTF8StringEncoding];
        [self.mediaSideInfoApi sendMediaSideInfo:data packet:false channelIndex:ZEGOAPI_CHN_MAIN];
        result(nil);

    }
    /* LiveRoom-AudioIO*/
    else if([@"enableAECWhenHeadsetDetected" isEqualToString:call.method]) {
        
        if(self.zegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        [self.zegoApi enableAECWhenHeadsetDetected:enable];
        result(nil);
        
    }
    /* SoundLevel */
    else if([@"registerSoundLevelCallback" isEqualToString:call.method]) {
        
        [[ZegoSoundLevel sharedInstance] setSoundLevelDelegate:self];
        result(@(YES));
    } else if([@"unregisterSoundLevelCallback" isEqualToString:call.method]) {
        
        [[ZegoSoundLevel sharedInstance] setSoundLevelDelegate:nil];
        result(@(YES));
    } else if([@"setSoundLevelMonitorCycle" isEqualToString:call.method]) {
        
        unsigned int ms = [self numberToUintValue:args[@"ms"]];
        bool success = [[ZegoSoundLevel sharedInstance] setSoundLevelMonitorCycle:ms];
        result(@(success));
        
    } else if([@"startSoundLevelMonitor" isEqualToString:call.method]) {
        
        bool success = [[ZegoSoundLevel sharedInstance] startSoundLevelMonitor];
        result(@(success));
        
    } else if([@"stopSoundLevelMonitor" isEqualToString:call.method]) {
        
        bool success = [[ZegoSoundLevel sharedInstance] stopSoundLevelMonitor];
        result(@(success));
    }
    /* Error Code */
    else if([@"isInitSDKError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isInitSDKError:error];
        result(@(ret));
        
    } else if([@"isNotLoginError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isNotLoginError:error];
        result(@(ret));
        
    } else if([@"isMediaServerNetWorkError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isMediaServerNetWorkError:error];
        result(@(ret));
        
    } else if([@"isLogicServerNetWorkError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isLogicServerNetWrokError:error];
        result(@(ret));
        
    } else if([@"isMixStreamNotExistError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isMixStreamNotExistError:error];
        result(@(ret));
        
    } else if([@"isPlayStreamNotExistError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isPlayStreamNotExistError:error];
        result(@(ret));
        
    } else if([@"isPublishBadNameError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isPublishBadNameError:error];
        result(@(ret));
        
    } else if([@"isPublishForbidError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isPublishForbidError:error];
        result(@(ret));
        
    } else if([@"isPublishStopError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isPublishStopError:error];
        result(@(ret));
        
    } else if([@"isPublishDeniedError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isPublishDeniedError:error];
        result(@(ret));
        
    } else if([@"isPlayForbidError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isPlayForbidError:error];
        result(@(ret));
        
    } else if([@"isPlayDeniedError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isPlayDeniedError:error];
        result(@(ret));
        
    } else if([@"isDNSResolveError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isDNSResolveError:error];
        result(@(ret));
        
    } else if([@"isNetworkUnreachError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isNetworkUnreachError:error];
        result(@(ret));
        
    } else if([@"isHttpProtocolError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isHttpProtocolError:error];
        result(@(ret));
        
    } else if([@"isReqFrequencyLimitError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isReqFrequencyLimitError:error];
        result(@(ret));
        
    } else if([@"isLiveRoomError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isLiveRoomError:error];
        result(@(ret));
        
    } else if([@"isMultiLoginError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isMultiLoginError:error];
        result(@(ret));
        
    } else if([@"isManualKickoutError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isManualKickoutError:error];
        result(@(ret));
        
    } else if([@"isRoomSessionError" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        bool ret = [ZegoError isRoomSessionError:error];
        result(@(ret));
        
    } else if([@"getErrorMsg" isEqualToString:call.method]) {
        
        int error = [self numberToIntValue:args[@"error"]];
        NSString *msg = [ZegoError getErrorMsg:error];
        result(msg);
        
    }
    else if([@"addNoticeLog" isEqualToString:call.method]) {
        NSString *content = args[@"content"];
        [ZegoLog logNotice:content];
        result(nil);
    }
    else {
        result(FlutterMethodNotImplemented);
    }

}


@end

