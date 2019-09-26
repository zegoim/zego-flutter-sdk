#ifndef zego_api_external_audio_device_oc_h
#define zego_api_external_audio_device_oc_h

#import <Foundation/Foundation.h>

#import "ZegoAudioCapture.h"
#import "zego-api-defines-oc.h"
#import "zego-api-audio-frame-oc.h"


/** 辅助推流通道音频采集源类别 */
typedef enum ZegoAPIAuxPublishChannelAudioSrcType
{
    kZegoAPIAuxPublishChannelAudioSrcTypeNone                      = -1, /** 无声 */
    
    kZegoAPIAuxPublishChannelAudioSrcTypeSameAsMainPublishChannel  = 0,  /** 推流的辅通道使用的音频数据跟主通道一样 */
    
    kZegoAPIAuxPublishChannelAudioSrcTypeExternalCapture           = 1, /** 推流辅通道采用单独的音频数据，即与主通道不同的音频数据 */

    KZegoAPIAuxPublishChannelAudioSrcTypePlayer                    = 2, /** 推流的辅通道使用的音频数据来自于媒体播放器 */
} ZegoAPIAuxPublishChannelAudioSrcType;


/**
  提供音频外部采集和外部渲染的功能
 */
@interface ZegoExternalAudioDevice : NSObject

/**
 开/关外部音频设备，即开启音频外部采集和外部渲染功能（App 实现音频的采集和播放）
 
 * 一般在以下情况使用音频外部采集、音频外部渲染的功能：
 * 1.客户需要从现有音频流、音频文件或者定制的采集系统中获得采集后输入给 SDK 进行传输。
 * 2.客户有自己对 PCM 输入源做特殊的音效处理的需求，在音效处理后输入给 SDK 进行传输。
 * 3.在客户有自己的渲染需求时使用音频外部渲染的功能，比如对拉取到的原始 PCM 数据做特殊应用或者处理后再渲染。
 
 * 注意：
 * 1.必须在初始化 SDK 之前设置。
 * 2.开发者采用音频外部采集和渲染后，SDK 内部不负责声音增强、噪音抑制、回音消除等功能，需要用户自己实现。
 * 3.音频外部采集和外部渲染是一体的，如果开发者采用了外部渲染，则在有需要的情况下，应当自己实现外部采集；同理，采用了外部采集，在有需要的情况下，应当自己实现外部渲染。
 
 @param enable true 表示开启音频外部采集和外部渲染功能，false 表示关闭音频外部采集和外部渲染功能。
 @see +setAudioSrcForAuxiliaryPublishChannel:
 @see +startCapture:
 @see +startRender
 */
+ (void)enableExternalAudioDevice:(bool)enable;

/**
 选择辅助推流通道的音频采集源

 * 需要同时推两路流时使用。
 * 注意：必须在初始化 SDK 之后，推流之前设置。
 
 @param type 辅助推流通道音频采集源类别，详见 enum ZegoAPIAuxPublishChannelAudioSrcType
 @return 详见 enum ZegoAPIErrorCode，默认值为 kZegoAPIAuxPublishChannelAudioSrcTypeNone(无声)
 */
+ (int)setAudioSrcForAuxiliaryPublishChannel:(ZegoAPIAuxPublishChannelAudioSrcType)type;

/**
 通知 SDK 开始进行音频的外部采集

 * 注意：建议在 startPublishing 之前调用；如果在推流之后调用，会损失从 startPublishing 到startCapture 这段时间的音频数据。
 
 @param publishChannel 选定推流通道，详见 enum ZegoAPIPublishChannelIndex。
 @return 详见 enum ZegoAPIErrorCode
 @see +stopCapture:
 @see +onRecordAudioFrame:channel:
 */
+ (int)startCapture:(ZegoAPIPublishChannelIndex)publishChannel;

/**
 通知 SDK 结束音频的外部采集

 * 注意：建议在 stopPublishing 之后调用；如果在停止推流之前调用，会损失从stopCapture 到 stopPublishing 这段时间的音频数据。
 
 @param publishChannel 选定推流通道，详见 enum ZegoAPIPublishChannelIndex。
 @return 详见 enum ZegoAPIErrorCode
 */
+ (int)stopCapture:(ZegoAPIPublishChannelIndex)publishChannel;

/**
 把采集到的音频数据塞给 SDK

 * 准备好音频帧数据后调用此 API 将音频数据塞给 SDK。
 
 @param audioFrame 采集到的音频帧，详见 interface ZegoAPIAudioFrame。
 @param publishChannel 选定推流通道，详见 enum ZegoAPIPublishChannelIndex。
 @return 详见 enum ZegoAPIErrorCode
 @see + enableExternalAudioDevice:
 @see + startCapture:
 */
+ (int)onRecordAudioFrame:(ZegoAPIAudioFrame*)audioFrame channel:(ZegoAPIPublishChannelIndex)publishChannel;

/**
 通知 SDK App 开始播放音频

 @return 详见 enum ZegoAPIErrorCode
 @see + stopRender
 */
+ (int)startRender;

/**
 通知 SDK App 结束播放音频

 @return 详见 enum ZegoAPIErrorCode
 */
+ (int)stopRender;

/**
 从 SDK 获取可渲染的音频数据

 * 在需要渲染音频数据时调用以获取待渲染的音频数据，调用频率一般是20ms或10ms。
 
 @param audioFrame 得到的音频数据，SDK 根据本地设置的音频采样率、帧数据类型将相应的音频数据写入到 App 提供的地址；详见 interface ZegoAPIAudioFrame。
 @return 详见 enum ZegoAPIErrorCode
 @see + enableExternalAudioDevice:
 @see + startRender
 */
+ (int)onPlaybackAudioFrame:(ZegoAPIAudioFrame*)audioFrame;


/**
 获取 IAudioDataInOutput 对象
 
 @return IAudioDataInOutput 对象，参考 IAudioDataInOutput 类型
 @discussion 必须在InitSDK之后调用。
 @warning Deprecated 不建议使用，请使用 ZegoExternalAudioDevice 其它方法。
 */
+ (AVE::IAudioDataInOutput *)getIAudioDataInOutput;
 
@end

#endif /* zego_api_external_audio_device_oc_h */

