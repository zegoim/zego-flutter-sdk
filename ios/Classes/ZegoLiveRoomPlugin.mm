#import "ZegoLiveRoomPlugin.h"
#import "ZegoRendererController.h"

static ZegoLiveRoomApi *g_ZegoApi = nil;

//static char* errorMessageWithNotInitSDK = "error because zegoliveroom api is not inited.";
//static char* errorMessageWithNotInitRenderModule = "error because zegorender api is not inited.";
//static char* errorMessageWithNotRenderer = "error because zego live renderer is null.";
//static char* errorMessageWithMediaSideInfo = "media side info flag is false, you should call \"setMediaSideFlags\" first.";

typedef NS_ENUM(NSUInteger, EVENT_TYPE) {
    TYPE_ROOM_EVENT = 0,
    TYPE_PUBLISH_EVENT,
    TYPE_PLAY_EVENT,
    TYPE_MEDIA_SIDE_INFO_EVENT
};

@interface ZegoLiveRoomPlugin() <ZegoRoomDelegate, ZegoIMDelegate, ZegoLiveEventDelegate, ZegoLivePublisherDelegate, ZegoLivePlayerDelegate, ZegoMediaSideDelegate, ZegoExternalVideoRenderDelegate, FlutterStreamHandler>

@property (nonatomic, strong) ZegoMediaSideInfo * media_side_info_api;
@property (nonatomic, strong) NSObject<FlutterPluginRegistrar> *registrar;
@property (nonatomic, strong) ZegoRendererController *renderController;

@end

@implementation ZegoLiveRoomPlugin
{
    volatile FlutterEventSink _eventSink;
}

- (instancetype)initWithRegistrar:(NSObject<FlutterPluginRegistrar> *)registrar {
    if (self = [super init]) {
        
        _registrar = registrar;
    }
    
    return self;
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
    
    } else if([@"uploadLog" isEqualToString:call.method]) {
        
        [ZegoLiveRoomApi uploadLog];
    
    } else if([@"getLogDir" isEqualToString:call.method]) {
        
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
        if([paths count] > 0) {
            NSString *logPath = [NSString stringWithFormat:@"%@/ZegoLogs", paths.firstObject];
            result(logPath);
        }else {
            result(nil);
        }
        
    } else if([@"initSDK" isEqualToString:call.method]) {
        
        //NSNumber *obj_appID = args[@"appID"];
        //NSString *strAppSign = args[@"appSign"];
        //unsigned int appID = [obj_appID unsignedIntValue];
        
        unsigned int appID = [self numberToUintValue:args[@"appID"]];
        NSString *strAppSign = args[@"appSign"];
        
        [self initSDKWithAppID:appID appSign:strAppSign result:result];
        
    } else if([@"uninitSDK" isEqualToString:call.method]){
        
        if(g_ZegoApi == nil) {
            
            result(@NO);
        }
        else {
            [ZegoExternalVideoRender enableExternalVideoRender:NO type:VideoExternalRenderTypeDecodeRgbSeries];
            
            [self.media_side_info_api setMediaSideDelegate:nil];
            self.media_side_info_api = nil;
            
            self.renderController = nil;

            [g_ZegoApi setRoomDelegate:nil];
            [g_ZegoApi setRenderDelegate:nil];
            [g_ZegoApi setPublisherDelegate:nil];
            [g_ZegoApi setPlayerDelegate:nil];
            [g_ZegoApi setLiveEventDelegate:nil];
            [g_ZegoApi setIMDelegate:nil];
            g_ZegoApi = nil;
            result(@YES);
        }
        
    } else if([@"setUser" isEqualToString:call.method]) {
        
        NSString *userID = args[@"userID"];
        NSString *userName = args[@"userName"];
      
        BOOL success = [ZegoLiveRoomApi setUserID:userID userName:userName];
        result(@(success));
        
    } else if([@"setUseTestEnv" isEqualToString:call.method]) {
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        [ZegoLiveRoomApi setUseTestEnv:enable];
        
    } else if([@"loginRoom" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *roomID = args[@"roomID"];
        NSString *roomName = args[@"roomName"];
        int role = [self numberToIntValue:args[@"role"]];
      
        BOOL success = [g_ZegoApi loginRoom:roomID roomName:roomName role:role withCompletionBlock:^(int errorCode, NSArray<ZegoStream *> *streamList) {
            
              
            NSMutableArray *streamArray = [NSMutableArray array];
            for (ZegoStream *stream in streamList) {
                
                    [streamArray addObject:@{@"userID": stream.userID, @"userName": stream.userName, @"streamID": stream.streamID, @"extraInfo": stream.extraInfo}];
            }
            result(@{@"errorCode": @(errorCode), @"streamList": streamArray});
              
          }];
        
        if(!success) {
            result([FlutterError errorWithCode:[[NSString stringWithFormat:@"%@_ERROR", call.method] uppercaseString] message:@"call \'loginRoom\' failed, please check you call \'setUser\' first" details:nil]);
        }
        
    } else if([@"logoutRoom" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        BOOL success = [g_ZegoApi logoutRoom];
        result(@(success));
        
    } else if([@"pauseModule" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSNumber *type = args[@"type"];
        [g_ZegoApi pauseModule:[type intValue]];
        
    } else if([@"resumeModule" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSNumber *type = args[@"type"];
        [g_ZegoApi resumeModule:(int)[type intValue]];
        

    } else if([@"enableMicDevice" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        BOOL success = [g_ZegoApi enableMicDevice: enable];
        result(@(success));
        
    } else if([@"sendCustomCommand" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
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
        
        [g_ZegoApi sendCustomCommand:zegoUserList content:content completion:^(int errorCode, NSString *roomID) {
                
            result(@{@"errorCode": @(errorCode),
                         @"roomID": roomID
                    });
        }];
        
    } else if([@"setRoomConfig" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool audienceCreate = [self numberToBoolValue:args[@"audienceCreateRoom"]];
        bool userStateUpdate = [self numberToBoolValue:args[@"userStateUpdate"]];
        [g_ZegoApi setRoomConfig:audienceCreate userStateUpdate:userStateUpdate];
       
    }
    /* LiveRoom-Publisher */
    else if([@"setLatencyMode" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int mode = [self numberToIntValue:args[@"mode"]];
        [g_ZegoApi setLatencyMode:(ZegoAPILatencyMode)mode];
        
    } else if([@"setAudioDeviceMode" isEqualToString:call.method]) {
        
        int mode = [self numberToIntValue:args[@"mode"]];
        [ZegoLiveRoomApi setAudioDeviceMode:(ZegoAPIAudioDeviceMode)mode];

    } else if([@"startPreview" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        BOOL success = [g_ZegoApi startPreview];
        result(@(success));
       
    } else if([@"stopPreview" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        BOOL success = [g_ZegoApi stopPreview];
        result(@(success));

    } else if([@"startPublishing" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        NSString *title = args[@"title"];
        int flag = [self numberToIntValue:args[@"flag"]];
        NSString *extraInfo = args[@"extraInfo"];
        
        BOOL success;
        if(![extraInfo isKindOfClass:[NSNull class]]) {
            success = [g_ZegoApi startPublishing:streamID title:title flag:flag extraInfo:extraInfo];
        }
        else {
            success = [g_ZegoApi startPublishing:streamID title:title flag:flag];
        }
        
        result(@(success));

    } else if([@"stopPublishing" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        BOOL success = [g_ZegoApi stopPublishing];
        result(@(success));

    } else if([@"setVideoCodecId" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int codecID = [self numberToIntValue:args[@"codecID"]];
        BOOL success = [g_ZegoApi setVideoCodecId:(ZegoVideoCodecAvc)codecID ofChannel:ZEGOAPI_CHN_MAIN];
        result(@(success));

    } else if([@"updateStreamExtraInfo" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *extraInfo = args[@"extraInfo"];
        BOOL success = [g_ZegoApi updateStreamExtraInfo:extraInfo];
        result(@(success));

    } else if([@"enableMic" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        
        BOOL success = [g_ZegoApi enableMic: enable];
        result(@(success));

    } else if([@"enableCamera" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        
        BOOL success = [g_ZegoApi enableCamera:enable];
        result(@(success));

    }
    else if([@"setFrontCam" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        BOOL success = [g_ZegoApi setFrontCam:enable];
        if(success) {
                
            [self.renderController setIsUseFrontCam:enable];
        }
        result(@(success));

    } else if([@"setAVConfig" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
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

        BOOL success = [g_ZegoApi setAVConfig:avConfig];
        result(@(success));

    } else if([@"requireHardwareEncoder" isEqualToString:call.method]) {
        
        bool bRequire = [self numberToBoolValue:args[@"bRequire"]];
        BOOL success = [ZegoLiveRoomApi requireHardwareEncoder:bRequire];
        result(@(success));
        
    } else if([@"enableBeautifying" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int feature = [self numberToIntValue:args[@"feature"]];
        BOOL success = [g_ZegoApi enableBeautifying:feature];
        result(@(success));

    } else if([@"setPolishStep" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        float step = [self numberToFolatValue:args[@"step"]];
        BOOL success = [g_ZegoApi setPolishStep:step];
        result(@(success));

    } else if([@"setPolishFactor" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        float factor = [self numberToFolatValue:args[@"factor"]];
        BOOL success = [g_ZegoApi setPolishFactor:factor];
        result(@(success));

    } else if([@"setWhitenFactor" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        float factor = [self numberToFolatValue:args[@"factor"]];
        BOOL success = [g_ZegoApi setWhitenFactor:factor];
        result(@(success));

    } else if([@"setFilter" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int filter = [self numberToIntValue:args[@"filter"]];
        bool success = [g_ZegoApi setFilter:(ZegoFilter)filter];
        result(@(success));

    } else if([@"enableTrafficControl" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool bEnable = [self numberToBoolValue:args[@"enable"]];
        NSUInteger properties = [self numberToULongValue:args[@"properties"]];
        [g_ZegoApi enableTrafficControl:bEnable properties:properties];

    } else if([@"setMinVideoBitrateForTrafficControl" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int bitrate = [self numberToIntValue:args[@"bitrate"]];
        int mode = [self numberToIntValue:args[@"mode"]];
        [g_ZegoApi setMinVideoBitrateForTrafficControl:bitrate mode:(ZegoAPITrafficControlMinVideoBitrateMode)mode];

    } else if([@"createPreviewRenderer" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
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
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
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
        
        if(g_ZegoApi == nil) {
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

    } else if([@"destroyPreviewRenderer" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
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

    } else if([@"setAppOrientation" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
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
        BOOL success = [g_ZegoApi setAppOrientation:obj_orientation];
        result(@(success));
 
    } else if([@"respondJoinLiveReq" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int seq = [self numberToIntValue:args[@"seq"]];
        int rspResult = [self numberToIntValue:args[@"rspResult"]];
        
        BOOL success = [g_ZegoApi respondJoinLiveReq:seq result:rspResult];
        result(@(success));

    } else if([@"inviteJoinLive" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *userID = args[@"userID"];
        [g_ZegoApi inviteJoinLive:userID responseBlock:^(int ret, NSString *fromUserID, NSString *fromUserName) {
                
            result(@{@"result":@(ret),
                         @"fromUserID": fromUserID,
                         @"fromUserName": fromUserName
                    });
        }];

    } else if([@"endJoinLive" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *userID = args[@"userID"];
        [g_ZegoApi endJoinLive:userID completionBlock:^(int errorCode, NSString *roomID) {
               
            result(@{@"errorCode":@(errorCode),
                         @"roomID":roomID
                    });
        }];

    } else if([@"setAudioBitrate" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int bitrate = [self numberToIntValue:args[@"bitrate"]];
        BOOL success = [g_ZegoApi setAudioBitrate:bitrate];
        result(@(success));
        
    } else if([@"enableAEC" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        bool success = [g_ZegoApi enableAEC:enable];
        result(@(success));
        
    } else if([@"setAECMode" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int mode = [self numberToIntValue:args[@"mode"]];
        [g_ZegoApi setAECMode:(ZegoAPIAECMode)mode];
        
    } else if([@"enableAGC" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        bool success = [g_ZegoApi enableAGC:enable];
        result(@(success));
        
    } else if([@"enableVAD" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        [g_ZegoApi enableVAD:enable];

    } else if([@"enableANS" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        bool success = [g_ZegoApi enableNoiseSuppress:enable];
        result(@(success));
    }
    else if([@"enableDTX" isEqualToString:call.method])
    {
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        [g_ZegoApi enableDTX:enable];

    }
    /* LiveRoom-Player */
    else if([@"startPlayingStream" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        NSDictionary *info = args[@"info"];
        BOOL success;
        
        if(![info isKindOfClass:[NSNull class]]) {
            NSString *params = info[@"params"];
            NSArray *rtmpUrls = info[@"rtmpUrls"];
            NSArray *flvUrls = info[@"flvUrls"];
            
            ZegoAPIStreamExtraPlayInfo *extraPlayInfo = [ZegoAPIStreamExtraPlayInfo new];
            extraPlayInfo.params = params;
            extraPlayInfo.rtmpUrls = rtmpUrls;
            extraPlayInfo.flvUrls = flvUrls;
            
            success = [g_ZegoApi startPlayingStream:streamID inView:nil extraInfo:extraPlayInfo];
        } else {
            
            success = [g_ZegoApi startPlayingStream:streamID inView:nil];
        }
        
        result(@(success));
 
    } else if([@"stopPlayingStream" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        BOOL success = [g_ZegoApi stopPlayingStream:streamID];
        result(@(success));
  
    } else if([@"activateAudioPlayStream" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *streamID = args[@"streamID"];
        bool active = [self numberToBoolValue:args[@"active"]];
        
        int errorCode = [g_ZegoApi activateAudioPlayStream:streamID active:active];
        result(@(errorCode));
 
    } else if([@"activateVideoPlayStream" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
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
            errorCode = [g_ZegoApi activateVideoPlayStream:streamID active:active videoLayer:(VideoStreamLayer)videoStreamLayer];
        } else {
            
            errorCode = [g_ZegoApi activateVideoPlayStream:streamID active:active];
        }
        
        result(@(errorCode));
  
    } else if([@"requireHardwareDecoder" isEqualToString:call.method]) {
        
        bool bRequire = [self numberToBoolValue:args[@"bRequire"]];
        BOOL success = [ZegoLiveRoomApi requireHardwareDecoder:bRequire];
        result(@(success));
        
    } else if([@"createPlayViewRenderer" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
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
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
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
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
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
        
    } else if([@"destroyPlayViewRenderer" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
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
  
    } else if([@"requestJoinLive" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        [g_ZegoApi requestJoinLive:^(int ret, NSString *fromUserID, NSString *fromUserName) {
                
            result(@{@"result":@(ret),
                     @"fromUserID": fromUserID,
                     @"fromUserName": fromUserName
                    });
        }];
        
    } else if([@"respondInviteJoinLiveReq" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        int seq = [self numberToIntValue:args[@"seq"]];
        int rspResult = [self numberToIntValue:args[@"rspResult"]];
        
        BOOL success = [g_ZegoApi respondInviteJoinLiveReq:seq result:rspResult];
        result(@(success));

    } else if([@"enableSpeaker" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        bool success = [g_ZegoApi enableSpeaker:enable];
        result(@(success));
        
    } else if([@"setBuiltInSpeakerOn" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool on = [self numberToBoolValue:args[@"bOn"]];
        bool success = [g_ZegoApi setBuiltInSpeakerOn:on];
        result(@(success));
        
    }
    /* Media Side Info */
    else if([@"setMediaSideFlags" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil || self.media_side_info_api == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool start = [self numberToBoolValue:args[@"start"]];
        bool onlyAudioPublish = [self numberToBoolValue:args[@"onlyAudioPublish"]];
        
        [self.media_side_info_api setMediaSideFlags:start onlyAudioPublish:onlyAudioPublish channelIndex: ZEGOAPI_CHN_MAIN];
        
    } else if([@"sendMediaSideInfo" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil || self.media_side_info_api == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        NSString *strData = args[@"data"];
        
        NSData *data = [strData dataUsingEncoding:NSUTF8StringEncoding];
        [self.media_side_info_api sendMediaSideInfo:data packet:false channelIndex:ZEGOAPI_CHN_MAIN];

    }
    /* LiveRoom-AudioIO*/
    else if([@"enableAECWhenHeadsetDetected" isEqualToString:call.method]) {
        
        if(g_ZegoApi == nil) {
            [self throwSdkNotInitError:result ofMethodName:call.method];
            return;
        }
        
        bool enable = [self numberToBoolValue:args[@"enable"]];
        [g_ZegoApi enableAECWhenHeadsetDetected:enable];
 
    } else {
        
        result(FlutterMethodNotImplemented);
    }
}

- (void)dealloc {
    
    if(g_ZegoApi)
        g_ZegoApi = nil;
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
    
    result([FlutterError errorWithCode:[[NSString stringWithFormat:@"%@_ERROR", methodName] uppercaseString] message:[NSString stringWithFormat:@"[ERROR]: %@ %@", methodName, @"error because zegoliveroom api is not inited."] details:nil]);
}

- (void)throwNoRendererError:(FlutterResult)result ofMethodName:(NSString *)methodName {
    
    result([FlutterError errorWithCode:[[NSString stringWithFormat:@"%@_ERROR", methodName] uppercaseString] message:[NSString stringWithFormat:@"[ERROR]: %@ %@", methodName, @"error because zego preview or play renderer is null."] details:nil]);
}


#pragma mark - Handle Flutter CallMethods
- (void)initSDKWithAppID:(unsigned int)appID appSign: (NSString *)appsign result:(FlutterResult)result {
    
    NSData *appSign = convertStringToSign(appsign);
    if(appSign == nil)
        return;
    
    //默认开启外部渲染绘图
    [ZegoExternalVideoRender enableExternalVideoRender:YES type:VideoExternalRenderTypeDecodeRgbSeries];
    
    self.renderController = [[ZegoRendererController alloc] init];
    
    g_ZegoApi = [[ZegoLiveRoomApi alloc] initWithAppID: appID appSignature:appSign completionBlock:^(int errorCode){
        
        if(errorCode == 0)
        {
            //设置代理
            [g_ZegoApi setRoomDelegate:self];
            [g_ZegoApi setPublisherDelegate:self];
            [g_ZegoApi setPlayerDelegate:self];
            [g_ZegoApi setLiveEventDelegate:self];
            [g_ZegoApi setIMDelegate:self];
            
            [[ZegoExternalVideoRender sharedInstance] setExternalVideoRenderDelegate:self];
            
            //初始化媒体次要信息模块
            self.media_side_info_api = [[ZegoMediaSideInfo alloc] init];
            [self.media_side_info_api setMediaSideDelegate:self];
        }
        
        result(@(errorCode));
        
    }];
}

#pragma mark - FlutterStreamHandler methods

- (FlutterError* _Nullable)onCancelWithArguments:(id _Nullable)arguments {
        
    _eventSink = nil;
    return nil;
}

- (FlutterError* _Nullable)onListenWithArguments:(id _Nullable)arguments
                                       eventSink:(nonnull FlutterEventSink)sink {
 
    _eventSink = sink;
    return nil;
}


#pragma mark - ZegoRoomDelegate

- (void)onStreamUpdated:(int)type streams:(NSArray<ZegoStream*> *)streamList roomID:(NSString *)roomID
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        
        NSMutableArray *streamArray = [NSMutableArray array];
        for (ZegoStream *stream in streamList)
        {
            [streamArray addObject:@{@"userID": stream.userID, @"userName": stream.userName, @"streamID": stream.streamID, @"extraInfo": stream.extraInfo}];
        }
        
        sink(@{@"type": @(TYPE_ROOM_EVENT),
               @"method": @{@"name": @"onStreamUpdated",
                            @"updateType": @(type),
                            @"roomID": roomID,
                            @"streamList": streamArray}
            });
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
- (void)onKickOut:(int)reason roomID:(NSString *)roomID
{
    FlutterEventSink sink = _eventSink;
    if(sink) {
        sink(@{@"type": @(TYPE_ROOM_EVENT),
               @"method": @{@"name": @"onKickOut",
                            @"reason": @(reason),
                            @"roomID": roomID}
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
    if(sink) {
        sink(@{@"type": @(TYPE_PUBLISH_EVENT),
               @"method": @{@"name": @"onPublishStateUpdate",
                            @"stateCode": @(stateCode), 
                            @"streamID": streamID, 
                            @"streamInfo": info}
            });
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

- (void)onCaptureVideoFirstFrame {
    BOOL isUseFrontCam = [self.renderController isUseFrontCam];
    ZegoViewRenderer *preview = [self.renderController getRenderer:kZegoVideoDataMainPublishingStream];
    if(preview) {
        
        [preview setUseMirrorEffect:isUseFrontCam];
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
    if(sink) {
        sink(@{@"type": @(TYPE_PLAY_EVENT), 
               @"method": @{@"name": @"onPlayStateUpdate",
                            @"stateCode": @(stateCode),
                            @"streamID": streamID}
            });
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
                            @"pktLostRate" : @(quality.pktLostRate),
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
                             @"data" : str_data}
               });
    }
}

#pragma mark - ZegoLiveApiRenderDelegate

- (CVPixelBufferRef)onCreateInputBufferWithWidth:(int)width height:(int)height cvPixelFormatType:(OSType)cvPixelFormatType streamID:(NSString *)streamID
{
    if(![self.renderController isRendering])
        return nil;
    
    ZegoPixelBufferPool *pool_ = [self.renderController getPixelBufferPool:streamID];
    
    if(pool_ == nil) {
        [self.renderController createPixelBufferPool:width height:height streamID:streamID];
        pool_ = [self.renderController getPixelBufferPool:streamID];
        
        if(pool_ == nil)
            return nil;
    }
    
    if(width != [pool_ getWidth] || height != [pool_ getHeight]) {
        [self.renderController createPixelBufferPool:width height:height streamID:streamID];
        pool_ = [self.renderController getPixelBufferPool:streamID];
    }
    
    CVPixelBufferRef pixelBuffer;
    CVReturn ret = CVPixelBufferPoolCreatePixelBuffer(nil, [pool_ getBufferPool], &pixelBuffer);
    if (ret != kCVReturnSuccess)
        return nil;
   
    return pixelBuffer;
    
}

- (void)onPixelBufferCopyed:(CVPixelBufferRef)pixelBuffer streamID:(NSString *)streamID
{
    ZegoViewRenderer *renderer = [self.renderController getRenderer:streamID];
    if(renderer == nil) {
        CVBufferRelease(pixelBuffer);
        return;
    }
    
    //Notify the Flutter new pixelBufferRef to be ready.
    [renderer setSrcFrameBuffer:pixelBuffer processBuffer:nil];
}

@end
