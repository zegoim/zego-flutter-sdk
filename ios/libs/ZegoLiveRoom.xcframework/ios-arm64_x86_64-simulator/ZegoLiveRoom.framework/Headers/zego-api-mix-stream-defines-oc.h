//
//  zego-api-mix-stream-defines-oc.h
//
//  Copyright © 2018年 Zego. All rights reserved.
//

#ifndef zego_api_mix_stream_defines_oc_h
#define zego_api_mix_stream_defines_oc_h

#import "zego-api-defines-oc.h"

/**
 * 字体风格
 */
@interface ZegoFontStyle : NSObject

/**
 * 字体类型。
 * 0 表示思源黑体，1 表示阿里巴巴普惠体，2 表示旁门正道标题体，3 表示站酷快乐体。
 * 默认值：0。
 */
@property (nonatomic, assign) int type;
/**
 * 字体大小，单位 px。默认值：24。取值范围：[12,100]。
 */
@property (nonatomic, assign) int size;
/**
 * 字体颜色，计算公式为为：R + G x 256 + B x 65536，R（红）、G（绿）、B（蓝）的取值范围 [0,255]。默认值：16777215（白色）。取值范围：[0,16777215]。
 */
@property (nonatomic, assign) int color;
/**
 * 字体透明度。默认值：0。取值范围：[0,100]，100 为完全不透明，0 为完全透明。
 */
@property (nonatomic, assign) int transparency;
/**
 * 字体是否有边框。是否必填：否。默认值：false。取值范围：true/false.
 */
@property (nonatomic, assign) BOOL border;
/**
 * 字体边框颜色，计算公式为为：R + G x 256 + B x 65536，R（红）、G（绿）、B（蓝）的取值范围 [0,255]。是否必填：否。默认值：0。取值范围：[0,16777215]。
 */
@property (nonatomic, assign) int borderColor;

@end

/**
 * 文本信息。
 */
@interface ZegoLabelInfo : NSObject

/**
 * 字体风格。
 */
@property (nonatomic, strong) ZegoFontStyle *font;
/**
 * 文本内容，支持设置简体中文、英文、半角，不支持全角。取值范围：最大支持显示100个中文字符，300 个英文字符。
 */
@property (nonatomic, copy) NSString *text;
/**
 * 字体距离输出画布左边框的距离，单位 px。默认值：0。
 */
@property (nonatomic, assign) int left;
/**
 * 字体距离输出画布上边框的距离，单位 px。是否必填：否。默认值：0。
 */
@property (nonatomic, assign) int top;

@end

/**
 为该流的内容设置图片，用于替代视频。复用 layout 布局。
 */
@interface ZegoMixImageInfo : NSObject

/**
 * 图片链接。url为空则显示视频，否则显示图片。
 * 图片大小限制在 1M 以内。
 * 支持 JPG 和 PNG 格式。支持以下 2 种使用方式：
 * URI：将图片提供给 ZEGO 技术支持进行配置，配置完成后会提供图片 URI，例如：preset-id://xxx.jpg。
 * URL：仅支持 HTTP 协议
 */
@property (nonatomic, copy) NSString *url;

@end

/**
 混流输入流配置信息
 */
@interface ZegoMixStreamInput : NSObject

/**
 要混流的单流ID
 */
@property (nonatomic, copy) NSString *streamID;
/**
 混流画布左上角坐标的第二个值，左上角坐标为 (left, top)
 */
@property (nonatomic, assign) int top;
/**
 混流画布左上角坐标的第一个值，左上角坐标为 (left, top)
 */
@property (nonatomic, assign) int left;
/**
 混流画布右下角坐标的第二个值，右下角坐标为 (right, bottom)
 */
@property (nonatomic, assign) int bottom;
/**
 混流画布右下角坐标的第一个值，右下角坐标为 (right, bottom)
 */
@property (nonatomic, assign) int right;
/**
 音浪ID，用于标识用户（比如拉取混流方可根据此标识明确到混流中的单条流是主播/观众/副主播），soundLevelID 必须大于等于 0 且小于等于 4294967295L（即2^32-1）
 */
@property (nonatomic, assign) unsigned int soundLevelID;
/**
 推流内容控制， 0 表示输出的混流包含音视频，1 表示只包含音频，2 表示只包含视频；默认值为 0。
 */
@property (nonatomic, assign) int contentControl;
/**
 输入流音量, 有效值范围 [0, 200], 默认值 100
 */
@property (nonatomic, assign) int volume;
/**
 是否为焦点语音，true表示突出该流的音频，false表示不突出该流的音频，默认为false
 */
@property (nonatomic, assign) BOOL audioFocus;
/**
 声音的朝向。-1表示关闭，有效值[0,360]。默认为 -1
 */
@property (nonatomic, assign) int audioDirection;
/** 
 * <p>视频画面渲染模式。
 * <br>0 表示填充模式，等比缩放填充整个区域，可能有部分被裁剪；
 * <br>1 表示适应模式，等比缩放填充区域，如果缩放后比例与设置的尺寸不匹配，多余部分显示为透明。
 * <br>默认为0。
 * </p>
 * */
@property (nonatomic, assign) int renderMode;
/**
 * 文字水印
 */
@property (nonatomic, strong) ZegoLabelInfo *label;

/**
 * 图片信息
 * 为该流的内容设置图片，用于替代视频。
 * 复用 layout 布局。
 */
@property (nonatomic, strong) ZegoMixImageInfo *imageInfo;

/**
* 视频画面圆角半径，单位 px，不超过 [layout] 参数设置的视频画面的宽高。默认为 0。
*/
@property (nonatomic, assign) unsigned int cornerRadius;
/**
 *  原点在左上角，top/bottom/left/right 定义如下：
 *
 *  (left, top)-----------------------
 *  |                                |
 *  |                                |
 *  |                                |
 *  |                                |
 *  -------------------(right, bottom)
 */

@end

/**
 混流输出目的地
 */
@interface ZegoMixStreamOutput : NSObject

/**
 isUrl 参数为 YES 时，则此值为 Url；否则为流名。
 */
@property (nonatomic, copy) NSString *target;
/**
 输出流是否为 URL
 */
@property (nonatomic, assign) BOOL isUrl;
/**
 输出流视频编码格式
 
 * 可选值：0 - H.264编码，3 - H.265编码，默认0
 */
@property (nonatomic, assign) int videoCodec;
/**
 输出流视频码率
 
 * 输出码率控制模式参数设置为CBR恒定码率时，此设置值生效
 * 该参数若被设置，会覆盖外层的nOutputBitrate值
 * 视频码率值范围：(0M,10M]，此参数单位是 bps，1M = 1 * 1000 * 1000 bps
 */
@property (nonatomic, assign) int videoBitrate;

/**
 输出流视频编码规格
 
 * 默认是 ZEGOAPI_ENCODE_PROFILE_MAIN
 * 详见 ZegoAPIEncodeProfile 定义
 */
@property (nonatomic, assign) ZegoAPIEncodeProfile encodeProfile;
/**
 输出流编码延时，有效值范围 [0,2000]，单位毫秒，默认值为 0
 */
@property (nonatomic, assign) int encodeLatency;

@end

/**
 混流水印信息
 */
@interface ZegoMixStreamWatermark : NSObject
/**
 * 水印图片 ，只支持png、jpg格式
 
 * 此值由 ZEGO 提供，开发者先将图片提供给 ZEGO，ZEGO 设置后再反馈水印图片的设置参数。
 */
@property (nonatomic, copy) NSString *image;
/**
 混流画布左上角坐标的第一个值，即左上角坐标为 (left, top)
 */
@property (nonatomic, assign) int left;
/**
 混流画布左上角坐标的第二个值，左上角坐标为 (left, top)
 */
@property (nonatomic, assign) int top;
/**
 混流画布右下角坐标的第一个值，右下角坐标为 (right, bottom)
 */
@property (nonatomic, assign) int right;
/**
 混流画布右下角坐标的第二个值，右下角坐标为 (right, bottom)
 */
@property (nonatomic, assign) int bottom;

@end

/**
 混流配置
 */
@interface ZegoMixStreamConfig : NSObject

/**
 混流输出视频帧率，值范围：[1,30]，根据网络情况设定该值，帧率越高画面越流畅。
 */
@property (nonatomic, assign) int outputFps;
/**
 * 混流输出码率控制模式，0 表示 CBR 恒定码率，1 表示 CRF 恒定质量，默认值为 0。
 
 * CRF 恒定质量 表示保证视频的清晰度在固定水平上，因此若采用此控制模式，码率会根据网速的变化波动。
 
 * 比如游戏类直播时，为了让观众看到比较流畅的操作类画面会使用恒定质量模式，提升视频质量。
 */
@property (nonatomic, assign) int outputRateControlMode;
/**
 * 混流输出视频码率，输出码率控制模式参数设置为 CBR恒定码率 时此设置值生效。
 
 * 视频码率值范围：(0M,10M]，此参数单位是 bps，1M = 1 * 1000 * 1000 bps
 * 若设置了 ZegoMixStreamOutput.videoBitrate，则本参数会被忽略
 * @warning Deprecated, 请使用 ZegoMixStreamOutput.videoBitrate 代替
 */
@property (nonatomic, assign) int outputBitrate;
/**
 * 混流输出质量，输出码率控制模式参数设置为 CRF恒定质量 时此设置值有效，有效值范围 [0,51]，默认值是 23。
 
 * 若想视频质量好点，在23的基础上降低质量值测试调整。
 
 * 若想文件大小小一点，在23的基础上升高质量值测试调整；
 
 * 以 x 值下的文件大小为例， x + 6 值下的文件大小是 x 值下文件大小的一半，x - 6 值下的文件大小是 x 值下文件大小的两倍。
 */
@property (nonatomic, assign) int outputQuality;
/**
 * 混流输出音频码率，码率范围值是[10000, 192000]。
 
 * 若音频编码格式采用 默认音频编码--即 outputAudioConfig 参数填 0，采用 1/2声道时，对应的建议码率值是 48k/64k，可根据需要在此基础上调整。
 
 * 若音频编码格式采用 可选音频编码--即 outputAudioConfig 参数填 1，采用 1/2声道时，对应的建议码率值是 80k/128k，可根据需要在此基础上调整。
 */
@property int outputAudioBitrate;
/**
 * 混流输出视频分辨率，不确定用什么分辨率时可采用16:9的规格设置。
 
 * 此参数中的宽必须大于等于 输入流列表中所有输入流中最大的分辨率宽，即right布局值；此参数中的高必须大于等于 输入流列表中所有输入流中最大的分辨率高，即bottom布局值；且输入流的布局位置不能超出此参数规定的范围。
 */
@property (nonatomic, assign) CGSize outputResolution;
/**
 * 混流输出音频编码格式，可选值为 0--默认编码，1--可选编码；默认值为 0。
 
 * 0--默认编码：在低码率下，编码后的音质要明显好于 1--可选编码，在码率较大后，达到128kbps及以上，两种编码后的音质近乎相同。
 
 * 1--可选编码：优点在于低复杂性，能兼容更多的设备播放；但是目前经过 0--默认编码 编码后的音频不能正常播放的情况很少。
 */
@property (nonatomic, assign) int outputAudioConfig;
/**
 混流输入流列表，SDK 根据输入流列表中的流进行混流。
 */
@property (nonatomic, strong) NSMutableArray<ZegoMixStreamInput*> *inputStreamList;
/**
 混流输出流列表
 */
@property (nonatomic, strong) NSMutableArray<ZegoMixStreamOutput*> *outputList;
/**
 * 用户自定义数据
 
 * 注意：userData自定义的数据通过媒体次要信息的 -onRecvMediaSideInfo:ofStream: 接口回调出来。
 */
@property (nonatomic, strong) NSData* userData;
/**
 混流声道数，1-单声道，2-双声道，默认为单声道。
 */
@property (nonatomic, assign) int channels;
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
/**
 是否开启音浪。YES：开启，NO：关闭；默认值是NO。
 */
@property (nonatomic, assign) BOOL withSoundLevel;
/**
 * 混音模式，0 表示 raw 模式，1 表示 focus 模式，默认值为 0。
 
 * raw模式：普通模式，未开启焦点语音功能。
 
 * focus模式：开启焦点语音功能，当未指定流（ZegoMixStreamInput.audioFocus = false）时，会自动突出4路流的音频。
 
 * focus模式：开启焦点语音功能，当指定流（ZegoMixStreamInput.audioFocus = true，最多同时4路流）时，如果小于4路流，会自动挑选其他流凑够4路，突出这4路流的音频。
 */
@property (nonatomic, assign) int audioMixMode;
/**
 扩展信息，备用。
 */
@property (nonatomic, assign) int extra;
/**
 混流水印
 */
@property (nonatomic, strong) ZegoMixStreamWatermark *watermark;
/**
 * 混流输入为一条流时，是否混流；非必填参数。YES：不对单流混流，混流输出的流属性与单流一致(分辨率，编码格式等)，混流配置中设置的分辨率等配置无效；NO：混单流，混流服务器会对此单流重新编解码，混流输出的流属性与混流配置相同。
 
 * 若需要开启此功能，请与即构技术支持联系。
 */
@property (nonatomic, assign) BOOL singleStreamPassThrough;
/**
 * @deprecated 参考 streamAlignmentType
 */
@property (nonatomic, assign) BOOL streamAlignment;
/**
 * 网络时间对齐模式；非必填参数。0：关闭混流时间对齐功能；1：尽量对齐。
 
 * 尽量对齐指：调用setStreamAlignmentProperty设置的推流用网络时间对齐，其它流不对齐。
 */
@property (nonatomic, assign) int streamAlignmentType;
/**
 * 高级配置选项，适用于某些定制化的需求，格式 "config1=xxx;config2=xxx"。
 * 请与即构技术支持联系了解可支持字段。
 */
@property (nonatomic, copy) NSString *advancedConfig;
/**
 * 混流服务器拉取输入流的 jitter buffer 长度
 * 
 * 推流端网络抖动较大的场景，可以增加 buffer 长度，通过增加延迟的方式降低卡顿。
 * 推流端网络抖动较小的场景，可以减小 buffer 长度，降低延迟。
 * 在实时合唱 KTV 场景下，推流端网络轻微波动可能会导致混流的卡顿，此时观众拉混流的时候，会有比较高概率出现卡顿的问题。
 * 通过调节混流服务器拉流的 jitter buffer 长度，可优化观众端拉混流出现的卡顿问题。
 * 默认不设置，即服务端使用默认的配置值。
 * 只会对新的输入流设置生效，对于已经开始混流的输入流不生效。
 */ 
@property (nonatomic, assign) int minPlayStreamBufferLength;     
@end

/**
 混流输出的单条混流的详细信息
 */
@interface ZegoMixStreamOutputResult : NSObject
/**
 混流流 ID
 */
@property (nonatomic, copy) NSString *streamID;
/**
 拉取混流的 rtmp url 列表
 */
@property (nonatomic, strong) NSMutableArray<NSString *> *rtmpList;
/**
 拉取混流的 hls url 列表
 */
@property (nonatomic, strong) NSMutableArray<NSString *> *hlsList;
/**
 拉取混流的 flv url 列表
 */
@property (nonatomic, strong) NSMutableArray<NSString *> *flvList;

@end


/**
 混流结果
 */
@interface ZegoMixStreamResultEx : NSObject

/**
 混流请求序号，对应于 [ZegoStreamMixer -mixStreamEx:mixStreamID:] 接口的返回值。
 */
@property (nonatomic, assign) int seq;
/**
 不存在的输入流列表
 */
@property (nonatomic, strong) NSMutableArray<NSString *> *nonExistInputList;
/**
 * 输入图片加载失败的流列表
 */
@property (nonatomic, strong) NSMutableArray<NSString *> *imageErrorInputList;
/**
 混流输出流列表，每路输出对应一个 ZegoMixStreamOutputResult 对象。
 */
@property (nonatomic, strong) NSMutableArray<ZegoMixStreamOutputResult *> *outputResultList;

@end


/**
 混流中，发言者及其说话音量信息。
 */
@interface ZegoSoundLevelInMixedStreamInfo : NSObject

/**
 音浪ID，用于标识用户，对应于 ZegoMixStreamConfig 的 inputStreamList 中的单条输入流信息的 soundLevelID 参数的设置值。
 */
@property (nonatomic, assign) unsigned int soundLevelID;
/**
 音量level
 */
@property (nonatomic, assign) unsigned char soundLevel;

@end

#endif /* zego_api_mix_stream_defines_oc_h */
