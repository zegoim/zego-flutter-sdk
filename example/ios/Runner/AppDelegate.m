#include "AppDelegate.h"
#include "GeneratedPluginRegistrant.h"

// 使用外部滤镜：导入 ZegoLiveRoomPlugin.h 以及自己实现的外部滤镜工厂文件
#import <ZegoLiveRoomPlugin.h>
#import "ZGVideoFilterFactoryDemo.h"

// 使用外部采集
// 这里是结合 MediaPlayer 来使用外部采集示例
#import "ZGVideoCaptureForMediaPlayer.h"
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
    
    // 创建一个自己实现的外部采集工厂，此处为一个配合 MediaPlayer 的工厂
    ZGVideoCaptureForMediaPlayer *mpCapture = [[ZGVideoCaptureForMediaPlayer alloc] init];
    // 暂存外部采集工厂
    [ZegoLiveRoomPlugin setExternalVideoCaptureFactory:mpCapture];
    
    // Override point for customization after application launch.
    return [super application:application didFinishLaunchingWithOptions:launchOptions];
}

@end
