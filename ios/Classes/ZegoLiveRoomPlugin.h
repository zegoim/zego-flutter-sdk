#import <Flutter/Flutter.h>
#import <ZegoLiveRoom/ZegoLiveRoom.h>
#import <ZegoLiveRoom/ZegoLiveRoomApi-Publisher2.h>
#import <ZegoLiveRoom/ZegoLiveRoomApi-AudioIO.h>
#import <ZegoLiveRoom/zego-api-media-side-info-oc.h>
#import <ZegoLiveRoom/zego-api-external-video-filter-oc.h>
#import <ZegoLiveRoom/zego-api-external-video-capture-oc.h>
#import <ZegoLiveRoom/zego-api-external-video-render-oc.h>
#import <ZegoLiveRoom/zego-api-sound-level-oc.h>
#import <ZegoLiveRoom/zego-api-error-oc.h>
#import <ZegoLiveRoom/zego-api-audio-processing-oc.h>
#import <OpenGLES/EAGL.h>
#include <OpenGLES/ES2/gl.h>
#import "ZegoViewRenderer.h"

@interface ZegoLiveRoomPlugin : NSObject<FlutterPlugin>

/// 预存外部视频滤镜工厂
/// @discussion 首先实现一个 ZegoVideoFilterFactory ，调用此接口将工厂预存，然后在 dart 层调用 `enableExternalVideoFilterFactory` 接口从而将此预存的工厂设置给 Native ZegoSDK
/// @param factory 外部视频滤镜工厂
+ (void)setExternalVideoFilterFactory:(nullable id<ZegoVideoFilterFactory>)factory;

@end


