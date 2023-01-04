//
//  zego-api-auto-mix-stream-defines-oc.h
//  ZegoLiveRoom
//
//  Copyright © 2019年 Zego. All rights reserved.
//

#ifndef zego_api_auto_mix_stream_defines_oc_h
#define zego_api_auto_mix_stream_defines_oc_h

#import <Foundation/Foundation.h>


@interface ZegoAutoMixStreamOutputTarget : NSObject
/** 输出目标是否推流地址, true:输出目标是推流地址,false: 输出目标是流名 */
@property (nonatomic, assign) bool isUrl;
/** 输出目标, 不超过 512 字节 */
@property (nonatomic, copy) NSString *target;

@end


/** 混流输出目的地 */
@interface ZegoAutoMixStreamOutput : NSObject
/** 输出目标列表 */
@property (nonatomic, strong) NSArray<ZegoAutoMixStreamOutputTarget*> *targets;
/** 视频码率 */
@property (nonatomic, assign) int bitrate;
/** 视频帧率 */
@property (nonatomic, assign) int fps;
/** 画布 高，视频输出分辨率高 */
@property (nonatomic, assign) int videoOutHeight;
/** 画布宽，视频输出分辨率款 */
@property (nonatomic, assign) int videoOutWidth;
/** 音频通道数 0单身道，1单身道 2双声道 */
@property (nonatomic, assign) int audioChannelCnt;
/** 音频编码 0：HE_AAC  1 AAC_LC 2 MP3 */
@property (nonatomic, assign) int audioEncodeID;
/** 音频码率 */
@property (nonatomic, assign) int audioBitrate;
/** 编码模式 0恒定码率，1恒定质量 */
@property (nonatomic, assign) int encodeMode;
/** 视频质量参数, 有效值范围 0 到 51, 0最好，51最差, 默认23 */
@property (nonatomic, assign) int encodeQua;

@end


/** 混流水印信息 */
@interface ZegoAutoMixStreamWatermark : NSObject

/**
 * 水印图片，只支持png、jpg格式。
 * 此值由 ZEGO 提供，开发者先将图片提供给 ZEGO，ZEGO 设置后再反馈水印图片的设置参数。
 */
@property (nonatomic, copy) NSString *image;
/** 层次 */
@property (nonatomic, assign) int layer;
/** 设置水印在画布上的位置 */
@property (nonatomic, assign) int top;
@property (nonatomic, assign) int left;
@property (nonatomic, assign) int bottom;
@property (nonatomic, assign) int right;

@end


/** 混流配置 */
@interface ZegoAutoMixStreamConfig : NSObject

/**
 * 混流背景颜色，前三个字节为 RGB，即 0xRRGGBBxx。
 * 例如：选取RGB为 #87CEFA 作为背景色，此处写为 0x87CEFA00。
 */
@property (nonatomic, assign) int outputBackgroundColor;
/**
 * 混流背景图，支持预设图片，如 (preset-id://xxx)
 * 此值由 ZEGO 提供，开发者先将背景图提供给 ZEGO，ZEGO 设置后再反馈背景图片的设置参数。
 */
@property (nonatomic, copy) NSString *outputBackgroundImage;
/** 混流水印 */
@property (nonatomic, strong) ZegoAutoMixStreamWatermark *outputWatermark;
/** 是否开启音浪。0 关闭, 1 开启. 默认值为 0 */
@property (nonatomic, assign) int withSoundLevel;
/**
 * 用户自定义数据
 * 注意：pUserData自定义的数据通过媒体次要信息的 onRecvMediaSideInfo 接口回调出来。
 */
@property (nonatomic, strong) NSData * userData;
/** 是否启用单流透传开关 默认不启用（1 启用 0 不启用） */
@property (nonatomic, assign) int bypass;
/** @deprecated 参考 streamAlignmentType */
@property (nonatomic, assign) bool streamAlignment;
/** 
 * 网络时间对齐模式，默认不对齐（0 不对齐, 1 尽量对齐）
 * 尽量对齐指：调用setStreamAlignmentProperty设置的推流用网络时间对齐，其它流不对齐。
 */
@property (nonatomic, assign) int streamAlignmentType;
/** 额外参数, 如果有多个, 通过分号隔开(具体联系即构技术支持 比如 video_encode/sei_mode 可参见数据示例) */
@property (nonatomic, copy) NSString *extra;
/** 混流请求来源 */
@property (nonatomic, copy) NSString *mixSource;
/** 混流输出流列表 */
@property (nonatomic, strong) ZegoAutoMixStreamOutput *output;

@end


/**
 自动混流中，发言者及其说话音量信息。
 */
@interface ZegoSoundLevelInAutoMixedStreamInfo : NSObject

/**
 音浪ID，用于标识用户，对应于 ZegoAutoMixStreamConfig 的 inputStreamList 中的单条输入流信息的 soundLevelID 参数的设置值。
 */
@property (assign) unsigned int soundLevelID;
/**
 音量level
 */
@property (assign) unsigned char soundLevel;
/**
 流 ID
 */
@property (copy) NSString *streamID;
/**
 流发布者 ID
 */
@property (copy) NSString *userID;

@end




#endif /* zego_api_auto_mix_stream_defines_oc_h */
