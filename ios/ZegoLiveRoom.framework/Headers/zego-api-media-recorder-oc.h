//
//  zego-api-media-recorder-oc.h
//  ZegoLiveRoom
//
//  Created by MarkWu on 2018/6/15.
//
#ifndef zego_api_media_recorder_oc_h
#define zego_api_media_recorder_oc_h

#import <Foundation/Foundation.h>
#include "zego-api-defines-oc.h"

/** 媒体录制类型 */
typedef enum : NSUInteger
{
    /** 不录制任何数据 */
    ZEGOAPI_MEDIA_RECORD_NONE = 0,
    /** 只录制音频 */
    ZEGOAPI_MEDIA_RECORD_AUDIO = 1,
    /** 只录制视频 */
    ZEGOAPI_MEDIA_RECORD_VIDEO = 2,
    /** 同时录制音频、视频 */
    ZEGOAPI_MEDIA_RECORD_BOTH = 3
    
} ZegoAPIMediaRecordType;

/** 媒体录制通道 */
typedef enum :  NSUInteger
{
    /** 第一路录制通道，即推流时 ZegoAPIPublishChannelIndex 选择了主推流通道，录制时采用第一路录制通道 */
    ZEGOAPI_MEDIA_RECORD_CHN_MAIN  = 0,
    /** 第二路录制通道，即推流时 ZegoAPIPublishChannelIndex 选择了第二路推流通道，录制时采用第二路录制通道 */
    ZEGOAPI_MEDIA_RECORD_CHN_AUX = 1
    
} ZegoAPIMediaRecordChannelIndex;

/** 媒体录制格式 */
typedef enum : NSUInteger
{
    /** 录制文件为 FLV 格式 */
    ZEGOAPI_MEDIA_RECORD_FLV  = 1,
    /** 录制文件为 MP4 格式 */
    ZEGOAPI_MEDIA_RECORD_MP4  = 2
} ZegoAPIMediaRecordFormat;

@protocol ZegoMediaRecordDelegage <NSObject>
@required
/**
 媒体录制回调
 
 * 设置了媒体录制代理(-setMediaRecordDelegage:)，并启动录制后能收到此回调，通知 App 录制是否启动成功，以及启动录制时设置的录制文件保存路径。
 
 @param errCode 错误码， 0: 录制成功，1：文件存储路径太长，2：初始化 avcontext 失败，3：打开文件失败，4：写文件失败
 @param index 录制通道，详见 enum ZegoAPIMediaRecordChannelIndex。
 @param storagePath 录制文件存储路径
 @see -startRecord:recordType:storagePath:enableStatusUpdate:interval:
 @see -startRecord:recordType:storagePath:enableStatusUpdate:interval:recordFormat:
 */
- (void)onMediaRecord:(int)errCode channelIndex:(ZegoAPIMediaRecordChannelIndex)index storagePath:(NSString *)path;

@optional
/**
 录制信息更新回调
 
 * 设置了媒体录制代理(-setMediaRecordDelegage:)，在媒体录制启动成功后，录制期间会不断收到此回调，此回调的回调频率由启动录制时设置的 interval 参数值决定。
 
 @param index 录制通道，详见 enum ZegoAPIMediaRecordChannelIndex。
 @param path 录制文件存储路径
 @param duration 录制时长，单位是毫秒
 @param size 文件大小，单位是字节
 @see -startRecord:recordType:storagePath:enableStatusUpdate:interval:
 @see -startRecord:recordType:storagePath:enableStatusUpdate:interval:recordFormat:
 @see -startRecord:recordType:storagePath:enableStatusUpdate:interval:recordFormat:isFragment:
 */
- (void)onRecordStatusUpdateFromChannel:(ZegoAPIMediaRecordChannelIndex)index storagePath:(NSString *)path duration:(unsigned int)duration fileSize:(unsigned int)size;

@end

/**
 本地媒体录制，对直播过程中的音视频数据进行录制并存储到本地。
 */
@interface ZegoMediaRecorder : NSObject

/**
 开始录制
 
 * 注意：必须在初始化 SDK之后调用。
 
 @param channelIndex 录制通道，详见 enum ZegoAPIMediaRecordChannelIndex。
 @param recordType 录制类型，详见 enum ZegoAPIMediaRecordType。
 @param storagePath 录制文件存储路径
 @param return true 调用成功，false 调用失败
 @deprecated 请使用 - startRecord:recordType:storagePath:enableStatusUpdate:interval: 或者 - startRecord:recordType:storagePath:enableStatusUpdate:interval:recordFormat:
 */
- (BOOL)startRecord:(ZegoAPIMediaRecordChannelIndex)channelIndex recordType:(ZegoAPIMediaRecordType)recordType storagePath:(NSString *)storagePath;

/**
 开始录制
 
 * 1.必须在初始化 SDK 之后调用, 默认的生成的录制文件格式为 FLV。
 * 2.如果开启了录制信息更新回调（enableStatusUpdate 参数值为 YES），请实现 -onRecordStatusUpdateFromChannel:index storagePath:duration:fileSize: 回调。
 * 3.设置了录制回调代理(-setMediaRecordDelegage:)后，才能收到媒体录制是否成功的通知。
 * 注意：
 * 1.在开始录制之前需要启动采集，调用 SDK 的 startPreview 或者 startPublishing 启动采集设备，麦克风、摄像头等。
 * 2.若需要录制 ZEGOAPI_MEDIA_RECORD_AUDIO 类型媒体，在开始录制之前必须开启麦克风或者带麦克风的摄像头，不开启则不能正常返回录制的音频文件；同理，录制 ZEGOAPI_MEDIA_RECORD_VIDEO 类型媒体，必须开启摄像头，录制 ZEGOAPI_MEDIA_RECORD_BOTH 类型媒体，麦克风和摄像头都需要开启。
 
 @param channelIndex 录制通道，详见 ZegoAPIMediaRecordChannelIndex。
 @param recordType 录制类型，，详见 ZegoAPIMediaRecordType；如果填写的类型是 ZEGOAPI_MEDIA_RECORD_NONE，实际上 SDK 会采用 ZEGOAPI_MEDIA_RECORD_BOTH。
 @param storagePath 录制文件存储路径，存储的路径必须是应用有权限读写的，如果传入目录路径， - onMediaRecord:channelIndex:storagePath: 回调会返回写文件失败。
 @param enable  是否开启录制信息更新回调；YES: 开启，NO: 关闭；开启后，在已设置媒体录制代理后会收到 - onRecordStatusUpdateFromChannel:index storagePath:duration:fileSize: 回调。
 @param interval 录制文件时状态回调的间隔，单位毫秒，有效范围：[1000,10000]；此参数决定了 - onRecordStatusUpdateFromChannel:index storagePath:duration:fileSize: 的回调频率。
 @param return true 调用成功，false 调用失败
 @see - stopRecord:
 */
- (BOOL)startRecord:(ZegoAPIMediaRecordChannelIndex)channelIndex recordType:(ZegoAPIMediaRecordType)recordType storagePath:(NSString *)storagePath enableStatusUpdate:(BOOL)enable interval:(int)interval;

/**
 开始录制
 
 * 1.必须在初始化 SDK 之后调用, 默认的生成的录制文件格式为 FLV。
 * 2.如果开启了录制信息更新回调（enableStatusUpdate 参数值为 YES），请实现 -onRecordStatusUpdateFromChannel:index storagePath:duration:fileSize: 回调。
 * 3.设置了录制回调代理(-setMediaRecordDelegage:)后，才能收到媒体录制是否成功的通知。
 * 注意：
 * 1.在开始录制之前需要启动采集，调用 SDK 的 startPreview 或者 startPublishing 启动采集设备，麦克风、摄像头等。
 * 2.若需要录制 ZEGOAPI_MEDIA_RECORD_AUDIO 类型媒体，在开始录制之前必须开启麦克风或者带麦克风的摄像头，不开启则不能正常返回录制的音频文件；同理，录制 ZEGOAPI_MEDIA_RECORD_VIDEO 类型媒体，必须开启摄像头，录制 ZEGOAPI_MEDIA_RECORD_BOTH 类型媒体，麦克风和摄像头都需要开启。
 
 @param channelIndex 录制通道，详见 enum ZegoAPIMediaRecordChannelIndex。
 @param recordType 录制类型，详见 enum ZegoAPIMediaRecordType，如果填写的类型是 ZEGOAPI_MEDIA_RECORD_NONE，实际上 SDK 会采用 ZEGOAPI_MEDIA_RECORD_BOTH。
 @param storagePath 录制文件存储路径，存储的路径必须是应用有权限读写的，如果传入目录路径， - onMediaRecord:channelIndex:storagePath: 回调会返回写文件失败。
 @param enable  是否开启录制信息更新回调；YES: 开启，NO: 关闭；开启后，在已设置媒体录制代理后（ - setMediaRecordDelegage:）会收到 - onRecordStatusUpdateFromChannel:index storagePath:duration:fileSize: 回调。
 @param interval 录制文件时状态回调的间隔，单位毫秒，有效范围：[1000,10000]，此参数决定了 - onRecordStatusUpdateFromChannel:index storagePath:duration:fileSize: 的回调频率。
 @param recordFromat 录制文件的文件格式，详见 enum ZegoAPIMediaRecordFormat。
 @param return true 调用成功，false 调用失败
 @see - stopRecord:
 */
- (BOOL)startRecord:(ZegoAPIMediaRecordChannelIndex)channelIndex recordType:(ZegoAPIMediaRecordType)recordType storagePath:(NSString *)storagePath enableStatusUpdate:(BOOL)enable interval:(int)interval recordFormat:(ZegoAPIMediaRecordFormat)recordFormat;

/**
 开始录制
 
 * 1.必须在初始化 SDK 之后调用, 默认的生成的录制文件格式为 FLV。
 * 2.如果开启了录制信息更新回调（enableStatusUpdate 参数值为 YES），请实现 -onRecordStatusUpdateFromChannel:index storagePath:duration:fileSize: 回调。
 * 3.设置了录制回调代理(-setMediaRecordDelegage:)后，才能收到媒体录制是否成功的通知。
 * 注意：
 * 1.在开始录制之前需要启动采集，调用 SDK 的 startPreview 或者 startPublishing 启动采集设备，麦克风、摄像头等。
 * 2.若需要录制 ZEGOAPI_MEDIA_RECORD_AUDIO 类型媒体，在开始录制之前必须开启麦克风或者带麦克风的摄像头，不开启则不能正常返回录制的音频文件；同理，录制 ZEGOAPI_MEDIA_RECORD_VIDEO 类型媒体，必须开启摄像头，录制 ZEGOAPI_MEDIA_RECORD_BOTH 类型媒体，麦克风和摄像头都需要开启。
 
 @param channelIndex 录制通道，详见 enum ZegoAPIMediaRecordChannelIndex。
 @param recordType 录制类型，详见 enum ZegoAPIMediaRecordType，如果填写的类型是 ZEGOAPI_MEDIA_RECORD_NONE，实际上 SDK 会采用 ZEGOAPI_MEDIA_RECORD_BOTH。
 @param storagePath 录制文件存储路径，存储的路径必须是应用有权限读写的，如果传入目录路径， - onMediaRecord:channelIndex:storagePath: 回调会返回写文件失败。
 @param enable  是否开启录制信息更新回调；YES: 开启，NO: 关闭；开启后，在已设置媒体录制代理后（ - setMediaRecordDelegage:）会收到 - onRecordStatusUpdateFromChannel:index storagePath:duration:fileSize: 回调。
 @param interval 录制文件时状态回调的间隔，单位毫秒，有效范围：[1000,10000]，此参数决定了 - onRecordStatusUpdateFromChannel:index storagePath:duration:fileSize: 的回调频率。
 @param recordFromat 录制文件的文件格式，详见 enum ZegoAPIMediaRecordFormat
 @param isFragment 录制文件是否分片，只在录制文件格式被设置为 MP4 格式时才有效，默认分片间隔为2s。进行分片能保证录制发生异常中断等问题时，已保存的录制文件可以正常播放；注意，这儿的分片是 SDK 内部处理录制文件的一个逻辑概念，而不是将录制文件按间隔时间分成多个小文件存储。
 @param return true 调用成功，false 调用失败
 @see -stopRecord:
 */
- (BOOL)startRecord:(ZegoAPIMediaRecordChannelIndex)channelIndex recordType:(ZegoAPIMediaRecordType)recordType storagePath:(NSString *)storagePath enableStatusUpdate:(BOOL)enable interval:(int)interval recordFormat:(ZegoAPIMediaRecordFormat)recordFormat isFragment:(BOOL)isFragment;

/**
 停止录制
 
 @param channelIndex 录制通道，详见 enum ZegoAPIMediaRecordChannelIndex。
 */
- (void)stopRecord:(ZegoAPIMediaRecordChannelIndex)channelIndex;

/**
 设置录制代理对象
 
 * 设置录制代理后，在启动录制（startRecord）之后才能收到录制回调。
 
 @param delegate 媒体录制代理，详见 delegate ZegoMediaRecordDelegage。
 */
- (void)setMediaRecordDelegage:(id<ZegoMediaRecordDelegage>)delegate;

@end

#endif
