#include "AppDelegate.h"
#include "GeneratedPluginRegistrant.h"

// 使用外部滤镜：导入 ZegoLiveRoomPlugin.h 以及自己实现的外部滤镜工厂文件
#import <ZegoLiveRoomPlugin.h>
#import "ZGVideoFilterFactoryDemo.h"
#import "ZGVideoCaptureForMediaPlayer.h"
#import "ZGExternalVideoCaptureFactory.h"
#import <zegoliveroom_plugin/ZegoMediaplayerController.h>

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [GeneratedPluginRegistrant registerWithRegistry:self];
    
    
    // 创建一个自己实现的外部视频滤镜工厂，此处为一个 Demo 工厂
    ZGVideoFilterFactoryDemo *factory = [[ZGVideoFilterFactoryDemo alloc] init];
    // 使用异步(BGRA32)类型滤镜
    factory.bufferType = ZegoVideoBufferTypeAsyncPixelBuffer;
    // 设给 ZegoLiveRoomPlugin 暂存，当调用 dart 的接口 `enableExternalVideoFilterFactory` 时才真正调用 Native SDK 接口设入
    [ZegoLiveRoomPlugin setExternalVideoFilterFactory:factory];
    
    // 用于 MediaPlayer 数据回调的 抽象device
    ZGVideoCaptureForMediaPlayer *mpCapture = [[ZGVideoCaptureForMediaPlayer alloc] init];
    // 创建外部采集工厂
    ZGExternalVideoCaptureFactory *capFactory = [[ZGExternalVideoCaptureFactory alloc] initWithDevice:mpCapture];
    // 同滤镜，只是暂存，只有真正调用的时候，才从 native 接入
    [ZegoLiveRoomPlugin setExternalVideoCaptureFactory:capFactory];
    [[ZegoMediaPlayerController instance] setVideoDataDelegate:mpCapture withFormat:ZegoMediaPlayerVideoPixelFormatBGRA32];
    
    
    // Override point for customization after application launch.
    return [super application:application didFinishLaunchingWithOptions:launchOptions];
}

@end
