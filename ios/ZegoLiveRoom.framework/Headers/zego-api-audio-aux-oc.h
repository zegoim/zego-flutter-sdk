//
//  zego-api-audio-aux-oc.h
//

#import <Foundation/Foundation.h>
/**
 混音音频数据输入回调（可输入媒体次要信息）
 */
@protocol ZegoAudioAuxDelgate <NSObject>

@optional

/**
 混音音频数据的输入回调，当开启混音后，用户调用该 API 将混音数据传递给 SDK。
 
 * 注意：
 * 1.针对混音数据，目前 SDK 仅支持位深为 16bit，16k、32k、44.1k、48k 采样率，单声道或者双声道的 PCM 音频数据格式。
 * 2.用户根据实际的 PCM 音频填写采样率及声道数。
 * 3.为确保混音效果，请不要在此 API 中执行耗时操作。
 
 @param pData 待混音的音频数据
 @param pDataLen 一次传入的音频数据长度；SDK会提供好长度值，用户按照这个长度写入音频数据即可；如果填写的音频数据长度大于等于 pDataLen，则无需更改 pDataLen 的值;如果填写的音频数据长度小于 pDataLen，将 pDataLen 的值更改为0；当音频最后的尾音不足 SDK 提供的长度值时，又需要向 SDK 传入完整的音频数据，可以用静音数据补齐后再传给 SDK。
 @param pSampleRate 混音数据采样率，支持16k、32k、44.1k、48k
 @param pChannelCount 混音数据声道数，支持1、2
 @see -enableAux:
 */
- (void)onAuxCallback:(void *)pData dataLen:(int *)pDataLen sampleRate:(int *)pSampleRate channelCount:(int *)pChannelCount;

/**
 混音音频数据的输入回调，当开启混音后，用户调用该 API 将混音数据传递给 SDK。
 
 * 注意：
 * 1. 针对混音数据，目前 SDK 仅支持位深为 16bit，16k、32k、44.1k、48k 采样率，单声道或者双声道的 PCM 音频数据格式。
 * 2.用户根据实际的 PCM 音频填写采样率及声道数。
 * 3.为确保混音效果，请不要在此 API 中执行耗时操作。
 * 4.发送媒体次要信息前需要调用 [ZegoMediaSideInfo -setMediaSideFlags:onlyAudioPublish:mediaInfoType:seiSendType:channelIndex:] 开启媒体次要信息传输功能。
 * 5.此接口带媒体次要信息时必须要带有混音音频数据(不能只有媒体次要信息)才能发送媒体次要信息，如果不带混音音频数据则会丢掉媒体次要信息。
 
 @param pData 待混音的音频数据
 @param pDataLen 一次传入的音频数据长度；SDK会提供好长度值，用户按照这个长度写入音频数据即可；如果填写的音频数据长度大于等于 pDataLen，则无需更改 pDataLen 的值;如果填写的音频数据长度小于 pDataLen，将 pDataLen 的值更改为0；当音频最后的尾音不足 SDK 提供的长度值时，又需要向 SDK 传入完整的音频数据，可以用静音数据补齐后再传给 SDK。
 @param pSampleRate 混音数据采样率，支持16k、32k、44.1k、48k
 @param pChannelCount 混音数据声道数，支持1、2
 @param pSideInfo 媒体次要信息数据缓冲区地址,若不需要带媒体次要信息
 @param pSideInfoLength 媒体次要信息数据缓冲区长度
 @param bPacket  媒体次要信息数据是否外部已经打包好包头，YES 表示采用外部打包的包头， NO 表示采用 ZEGO 内部打包的包头。
 @see ZegoAudioAux -enableAux:
 */
- (void)onAuxCallback:(void *)pData dataLen:(int *)pDataLen sampleRate:(int *)pSampleRate channelCount:(int *)pChannelCount sideInfo:(unsigned char*)pSideInfo sideInfoLen:(int*) pSideInfoLength packet:(bool*)pPacket;

@end

/**
 混音配置类
 */
@interface ZegoAudioAux : NSObject

/**
 设置对混音数据的输入回调的监听
 
 * 注意：若不再需要混音，调用 setDelegate:nil 去除回调监听。
 
 @param delegate  实现了 ZegoAudioAuxDelgate 代理的方法，用于混音时向 SDK 传入待混音的音频数据。
 @see ZegoAudioAux -enableAux:
 */
- (void)setDelegate:(id<ZegoAudioAuxDelgate>) delegate;

/**
 混音开关
 
 * 注意：
 * 1. 必须在初始化 SDK 后调用，可在需要混音的任一时间开启混音开关。
 * 2.当开启混音后，SDK 会在 -onAuxCallback:dataLen:sampleRate:channelCount:sideInfo:sideInfoLen:packet: 中获取用于混音的音频数据，即需要开发者在此接口中塞音频数据给 SDK。
 
 @param enable 开启/关闭混音开关，YES 表示开启混音，NO 表示关闭混音；默认为 NO（关闭混音）。
 @return YES 表示调用成功，能收到混音回调；NO 表示调用失败，不能收到混音回调。
 */
- (BOOL)enableAux:(BOOL)enable;

/**
 设置混音音量
 
 @param volume 0~100，默认为 50
 */
- (void)setAuxVolume:(int)volume;

/**
 混音静音开关
 
 * 该接口调用时机无要求，开发者按需调用即可。
 
 @param mute YES: aux 输入播放静音，NO: 不静音。默认 NO
 @return YES 成功，NO 失败
 */
- (BOOL)muteAux:(BOOL)mute;

@end
