#ifndef zego_api_audio_device_oc
#define zego_api_audio_device_oc

#import <Foundation/Foundation.h>


@interface ZegoAudioDevice : NSObject

/**
 设置音频设备采集时是否开启双声道模式。
  
  @param captureType 0: 使用单声道进行采集; ≠0: 始终使用双声道进行采集
  @attention 初始化 SDK 之后，启用音视频功能前调用（如预览画面StartPreview、推流StartPublishing之前调用）
  @attention 设置始终生效，直到重新通过此 API 修改该值或者反初始化 SDK
 */
+ (void)enableAudioCaptureStereo:(int)captureType;

@end

#endif
