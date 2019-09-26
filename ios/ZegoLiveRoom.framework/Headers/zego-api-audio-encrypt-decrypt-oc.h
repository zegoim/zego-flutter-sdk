#ifndef zego_api_audio_encrypt_decrypt_oc
#define zego_api_audio_encrypt_decrypt_oc

#import <Foundation/Foundation.h>

/**
 音频加解密回调
 */
@protocol ZegoAudioEncryptDecryptDelegate <NSObject>
@required
/**
 音频加密、解密回调
 
 * 注意：
 * 1.开启音频加解密功能并成功设置音频加解密代理后，推流成功或者拉流成功都能收到此回调。
 * 2.当业务方需要同时使用音频加密、解密功能时，在此回调中同时处理音频加解密，使用此回调中的 streamID 来区分推流、拉流。
 
 @param streamID 标识当前回调的音频数据属于哪条流，即区分是推流、拉流流名
 @param inData  SDK 回调给 App 的音频数据, 推流时是用于加密的音频数据，拉流时是用于解密的音频数据
 @param inDataLen SDK 回调给 App 的音频数据的长度（单位：byte）
 @param outData App 将加密（推流）或者解密（拉流）后的音频数据回传给 SDK
 @param outDataLen App 实际回传给 SDK 的音频数据大小（单位：byte)
 @param maxOutBufLen App 回传数据的缓冲区的最大长度（单位：byte），即 inDataLen < maxOutBufLen
 @see +enableAudioEncryptDecrypt:
 @see -setAudioEncryptDecryptDelegage:
 **/
- (void)onAudioEncryptDecrypt:(NSString *)streamID inData:(const unsigned char *)pInData inDataLen:(int)inDataLen outData:(unsigned char *)pOutData outDataLen:(int *)pOutDataLen maxOutBufLen:(int)maxOutBufLen;

@end

/**
 提供音频加解密功能
 */
@interface ZegoAudioEncryptDecrypt : NSObject

/**
 设置音频加解密代理对象

 * 注意：
 * 1.开启音频加解密功能后，必须设置音频加解密代理才能接收到音频数据。
 * 2.成功推流后能收到音频加密回调，成功拉流后能收到音频解密回调。
 
 @param delegate 音频加密、解密回调，详细请查看 ZegoAudioEncryptDecryptDelegate 协议
 */
- (void)setAudioEncryptDecryptDelegage:(id<ZegoAudioEncryptDecryptDelegate>)delegate;

/**
 开/关音频加密、解密功能
 
 * 业务方需要防止其付费的音视频产品被免费传播，或者保护版权的问题可以开启此功能。
 * 推流时将编码后的音频数据加密，再发送到云端；拉流时将加密的音频数据解密，再进行解码、渲染。
 * 注意：
 * 1.必须在 init SDK 之前调用。
 * 2.开启音频加解密功能后，需要设置音频加解密代理(-setAudioEncryptDecryptDelegage:)才能收到 -onAudioEncryptDecrypt:inData:inDataLen:outData:outDataLen:maxOutBufLen: 音频加解密回调。
 
 @param enable：开启，false 关闭
  */
+ (void)enableAudioEncryptDecrypt:(BOOL)enable;

@end

#endif
