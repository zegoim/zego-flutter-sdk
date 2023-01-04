//
//  zego-api-defines-oc.h
//  zegoavkit
//
//  Copyright © 2017年 Zego. All rights reserved.
//

#ifndef zego_api_defines_oc_h
#define zego_api_defines_oc_h

#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>

#ifdef __cplusplus
#define ZEGO_EXTERN extern "C"
#else
#define ZEGO_EXTERN extern
#endif

/** 流ID，值为 NSString */
ZEGO_EXTERN NSString *const kZegoStreamIDKey;
/** 混流ID，值为 NSString */
ZEGO_EXTERN NSString *const kZegoMixStreamIDKey;

/** 流信息列表项 */
/** rtmp 播放 url 列表，值为 <NSArrayNSString *> */
ZEGO_EXTERN NSString *const kZegoRtmpUrlListKey;
/** hls 播放 url 列表，值为 <NSArrayNSString *> */
ZEGO_EXTERN NSString *const kZegoHlsUrlListKey;
/** flv 播放 url 列表，值为 <NSArrayNSString *> */
ZEGO_EXTERN NSString *const kZegoFlvUrlListKey;

/** 设备项 */
/** 摄像头设备 */
ZEGO_EXTERN NSString *const kZegoDeviceCameraName;
/** 麦克风设备 */
ZEGO_EXTERN NSString *const kZegoDeviceMicrophoneName;
/** 音频设备*/
ZEGO_EXTERN NSString *const kZegoDeviceAudioName;

/** 混流输出格式，值为 NSNumber，可选 {0, 1} */
ZEGO_EXTERN NSString *const kMixStreamAudioOutputFormat;

/** 自定义转推 RTMP 地址 */
ZEGO_EXTERN NSString *const kPublishCustomTarget;

/** 单主播模式下，自定义推流 RTMP 地址 */
ZEGO_EXTERN NSString *const kPublishCDNTarget;

/** AudioSession相关配置信息的key, 值为 NSString */
ZEGO_EXTERN NSString *const kZegoConfigKeepAudioSesionActive;

typedef unsigned int uint32;

/** 配置返回错误类型 */
typedef enum {
    /** 分辨率 */
    FLAG_RESOLUTION = 0x1,
    /** 帧率 */
    FLAG_FPS = 0x2,
    /** 比特率 */
    FLAG_BITRATE = 0x4
} SetConfigReturnType;

/** 本地预览视频视图的模式 */
typedef enum {
    /** 等比缩放，可能有黑边，SDK 默认值 */
    ZegoVideoViewModeScaleAspectFit = 0,
    /** 等比缩放填充整View，可能有部分被裁减 */
    ZegoVideoViewModeScaleAspectFill = 1,
    /** 填充整个View */
    ZegoVideoViewModeScaleToFill = 2,
} ZegoVideoViewMode;

typedef enum {
    /** 预览启用镜像，推流不启用镜像 */
    ZegoVideoMirrorModePreviewMirrorPublishNoMirror = 0,
    /** 预览启用镜像，推流启用镜像 */
    ZegoVideoMirrorModePreviewCaptureBothMirror = 1,
    /** 预览不启用镜像，推流不启用镜像 */
    ZegoVideoMirrorModePreviewCaptureBothNoMirror = 2,
    /** 预览不启用镜像，推流启用镜像 */
    ZegoVideoMirrorModePreviewNoMirrorPublishMirror = 3
} ZegoVideoMirrorMode;

/** 采集旋转角度，逆时针旋转 */
typedef enum {
    /** 旋转 0 度 */
    CAPTURE_ROTATE_0 = 0,
    /** 旋转 90 度 */
    CAPTURE_ROTATE_90 = 90,
    /** 旋转 180 度 */
    CAPTURE_ROTATE_180 = 180,
    /** 旋转 270 度 */
    CAPTURE_ROTATE_270 = 270
} CAPTURE_ROTATE;

/** 视频编解码器. */
typedef enum {
    /** 默认编码, 不支持分层编码 */
    VIDEO_CODEC_DEFAULT = 0,
    /** 分层编码 要达到和VIDEO_CODEC_DEFAULT相同的编码质量，建议码率和VIDEO_CODEC_DEFAULT相比增加20%左右 */
    VIDEO_CODEC_MULTILAYER = 1,
    /** VP8编码 */
    VIDEO_CODEC_VP8 = 2,
    /** H265编码 */
    VIDEO_CODEC_H265 = 3,
    /** 未知编码 */
    VIDEO_CODEC_UNKNOWN = 100

} ZegoVideoCodecAvc;

/** 视频编解码器. */
typedef enum {
    /** 软件或硬件 */
    VIDEO_CODEC_BACKEND_ANY = 0,
    /** 软件 */
    VIDEO_CODEC_BACKEND_SOFTWARE = 1,
    /** 硬件 */
    VIDEO_CODEC_BACKEND_HARDWARE = 2

} ZegoVideoCodecBackend;

/** 流的资源类型. */
typedef enum {
    /** CDN */
    RESOURCE_TYPE_CDN = 0,
    /** RTC */
    RESOURCE_TYPE_RTC = 1,
    /** L3 */
    RESOURCE_TYPE_L3 = 2

} ZegoResourceType;

/** 视频分层类型 */
typedef enum {
    /**< 根据网络状态选择图层  */
    VideoStreamLayer_Auto = -1,
    /**< 指定拉基本层（小分辨率） */
    VideoStreamLayer_BaseLayer = 0,
    /**< 指定拉扩展层（大分辨率)  */
    VideoStreamLayer_ExtendLayer = 1
} VideoStreamLayer;

/**
 MediaInfo类型
 */
typedef enum {
    /**
     ZEGO 定义的打包类型，跟视频编码器产生的信息不存兼容性问题。
     <br>但是在其它 CDN 上转码视频的时候，其它 CDN 基本上不支持提取这种方式打包的信息数据，转码完成后再从其它 CDN
     拉流时，可能就丢失了这些次媒体信息。 <br>ZEGO CDN 转码支持提取此种方式打包的信息数据。
     */
    SideInfoZegoDefined = 0,
    /**
     采用 H264 的 SEI (nalu type = 6,payload type = 243) 类型打包，此类型是 SEI 标准未规定的类型，跟视频编码器或者视频文件中的 SEI
     不存在冲突性，用户不需要根据 SEI 的内容做过滤。 <br>若需要发送 SEI 推荐采用此种类型。
     */
    SeiZegoDefined = 1,
    /**
     采用 H264 的 SEI (nalu type = 6,payload type = 5) 类型打包，H264 标准对于此类型有规定的格式：startcode + nalu type(6) + payload type(5)
     + len + pay load(uuid + context)+ trailing bits；

     <br>因为视频编码器自身会产生 payload type 为 5 的 SEI，或者使用视频文件推流时，视频文件中也可能存在这样的
     SEI，所以使用此类型时，用户需要把 uuid + context 当作一段 buffer 塞给次媒体的发送接口；

     <br>为了区别视频编码器自身产生的 SEI，所以对 uuid 有格式要求，即 uuid 16字节的前四个字节固定为 'Z' 'E' 'G' 'O'
     四个字符（全部大写），后面12字节用户任意填写；

     <br>在 SDK 接收端，对于 payload type = 5的 SEI 会根据'ZEGO'字样做过滤，识别出符合要求的 SEI 抛给用户，避免用户收到编码器自身产生的
     SEI。
     */
    SeiUserUnregisted = 2
} MediaInfoType;

/**
 SEI发送类型
 */
typedef enum {
    /**
     SEI 单帧发送，此种发送方式下，ffmpeg 解码会产生类似“此帧无视频”的警告，可能会导致一些 CDN 兼容性问题，例如转码失败等。
     */
    SeiSendSingleFrame = 0,
    /**
     SEI 随视频帧(I, B, P)发送，推荐采用此类型。
     */
    SeiSendInVideoFrame = 1
} SeiSendType;

/** 远程视图序号 */
typedef enum {
    /** 第一个远程视图 */
    RemoteViewIndex_First = 0,
    /** 第二个远程视图 */
    RemoteViewIndex_Second = 1,
    /** 第三个远程视图 */
    RemoteViewIndex_Third = 2
} RemoteViewIndex;

/** 滤镜特性 */
typedef enum : NSUInteger {
    /**  不使用滤镜 */
    ZEGO_FILTER_NONE = 0,
    /**  简洁 */
    ZEGO_FILTER_LOMO = 1,
    /**  黑白 */
    ZEGO_FILTER_BLACKWHITE = 2,
    /**  老化 */
    ZEGO_FILTER_OLDSTYLE = 3,
    /**  哥特 */
    ZEGO_FILTER_GOTHIC = 4,
    /**  锐色 */
    ZEGO_FILTER_SHARPCOLOR = 5,
    /**  淡雅 */
    ZEGO_FILTER_FADE = 6,
    /**  酒红 */
    ZEGO_FILTER_WINE = 7,
    /**  青柠 */
    ZEGO_FILTER_LIME = 8,
    /**  浪漫 */
    ZEGO_FILTER_ROMANTIC = 9,
    /**  光晕 */
    ZEGO_FILTER_HALO = 10,
    /**  蓝调 */
    ZEGO_FILTER_BLUE = 11,
    /**  梦幻 */
    ZEGO_FILTER_ILLUSION = 12,
    /**  夜色 */
    ZEGO_FILTER_DARK = 13
} ZegoFilter;

/** 美颜特性 */
typedef enum : NSUInteger {
    /**  无美颜 */
    ZEGO_BEAUTIFY_NONE = 0,
    /**  磨皮 */
    ZEGO_BEAUTIFY_POLISH = 1,
    /**  全屏美白，一般与磨皮结合使用：ZEGO_BEAUTIFY_POLISH | ZEGO_BEAUTIFY_WHITEN */
    ZEGO_BEAUTIFY_WHITEN = 1 << 1,
    /**  皮肤美白 */
    ZEGO_BEAUTIFY_SKINWHITEN = 1 << 2,
    /**  锐化 */
    ZEGO_BEAUTIFY_SHARPEN = 1 << 3
} ZegoBeautifyFeature;

/**
 混流图层信息
 */
@interface ZegoMixStreamInfo : NSObject

/**
 要混流的单流ID
 */
@property (copy) NSString *streamID;
/**
 混流画布左上角坐标的第二个值，左上角坐标为 (left, top)
 */
@property int top;
/**
 混流画布左上角坐标的第一个值，左上角坐标为 (left, top)
 */
@property int left;
/**
 混流画布右下角坐标的第二个值，右下角坐标为 (right, bottom)
 */
@property int bottom;
/**
 混流画布右下角坐标的第一个值，右下角坐标为 (right, bottom)
 */
@property int right;
/**
 音浪ID，用于标识用户（比如拉取混流方可根据此标识明确到混流中的单条流是主播/观众/副主播），soundLevelID 必须大于等于 0 且小于等于
 4294967295L（即2^32-1）
 */
@property unsigned int soundLevelID;
/**
 推流内容控制， 0 表示输出的混流包含音视频，1 表示只包含音频，2 表示只包含视频；默认值为 0。
 */
@property int contentControl;
/**
 输入流音量, 有效值范围 [0, 200], 默认值 100
 */
@property int volume;
/**
 是否为焦点语音，默认为否
 */
@property BOOL audioFocus;

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
 混流配置
 */
@interface ZegoCompleteMixStreamConfig : NSObject

/**
 outputIsUrl 为 YES时，则此值为 Url；否则为流名。
 */
@property (copy) NSString *outputStream;
/**
 输出流是否为 URL
 */
@property BOOL outputIsUrl;
/**
 混流输出视频帧率，值范围：[1,30]，根据网络情况设定该值，帧率越高画面越流畅。
 */
@property int outputFps;
/**
 混流输出视频码率，输出码率控制模式参数设置为 CBR恒定码率 时此设置值生效。
 <br>视频码率值范围：(0M,10M]，此参数单位是 bps，1M = 1 * 1000 * 1000 bps
 */
@property int outputBitrate;
/**
 混流输出音频码率，码率范围值是[10000, 192000]。
 <br>若音频编码格式采用 默认音频编码--即 outputAudioConfig 参数填 0，采用 1/2声道时，对应的建议码率值是 48k/64k，可根据需要在此基础上调整。
 <br>若音频编码格式采用 可选音频编码--即 outputAudioConfig 参数填 1，采用 1/2声道时，对应的建议码率值是 80k/128k，可根据需要在此基础上调整。
 */
@property int outputAudioBitrate;
/**
 混流输出视频分辨率，不确定用什么分辨率时可采用16:9的规格设置。
 <br>此参数中的宽必须大于等于 输入流列表中所有输入流中最大的分辨率宽，即right布局值；此参数中的高必须大于等于
 输入流列表中所有输入流中最大的分辨率高，即bottom布局值；且输入流的布局位置不能超出此参数规定的范围。
 */
@property CGSize outputResolution;
/**
 混流输出音频编码格式，可选值为 0--默认编码，1--可选编码；默认值为 0。
 <br>0--默认编码：在低码率下，编码后的音质要明显好于 1--可选编码，在码率较大后，达到128kbps及以上，两种编码后的音质近乎相同。
 <br>1--可选编码：优点在于低复杂性，能兼容更多的设备播放；但是目前经过 0--默认编码 编码后的音频不能正常播放的情况很少。
 */
@property int outputAudioConfig;
/**
 混流输入流列表，SDK 根据输入流列表中的流进行混流。
 */
@property (strong) NSMutableArray<ZegoMixStreamInfo *> *inputStreamList;
/**
 用户自定义数据
 <b>注意：</b>1. userData自定义的数据通过媒体次要信息的 -onRecvMediaSideInfo:ofStream: 接口回调出来。
 */
@property NSData *userData;
/**
 混流声道数，1-单声道，2-双声道，默认为单声道。
 */
@property int channels;
/**
 混流背景颜色，前三个字节为 RGB，即 0xRRGGBBxx。
 <br>例如：选取RGB为 #87CEFA 作为背景色，此处写为 0x87CEFA00。
 */
@property int outputBackgroundColor;
/**
 混流背景图，支持预设图片，如 (preset-id://xxx)
 <br>此值由 ZEGO 提供，开发者先将背景图提供给 ZEGO，ZEGO 设置后再反馈背景图片的设置参数。
 */
@property (copy) NSString *outputBackgroundImage;
/**
 是否开启音浪。YES：开启，NO：关闭；默认值是NO。
 */
@property BOOL withSoundLevel;
/**
 扩展信息，备用。
 */
@property int extra;
@end

/** 视频编码码率控制策略 */
typedef enum {
    /** 平均码率 */
    ZEGOAPI_RC_ABR = 0,
    /** 恒定码率 */
    ZEGOAPI_RC_CBR,
    /** 可变码率 */
    ZEGOAPI_RC_VBR,
    /** 恒定质量 */
    ZEGOAPI_RC_CRF,
} ZegoAPIVideoEncoderRateControlStrategy;

/** 发布直播的模式 */
enum ZegoAPIPublishFlag {
    /**  连麦模式，直播流会推到即构服务器，然后转推到CDN，默认连麦者之间从即构服务器拉流，观众从 CDN 拉流 */
    ZEGOAPI_JOIN_PUBLISH = 0,
    ZEGO_JOIN_PUBLISH = ZEGOAPI_JOIN_PUBLISH,
    /**  混流模式，同连麦模式，有混流需求时使用 */
    ZEGOAPI_MIX_STREAM = 1 << 1,
    ZEGO_MIX_STREAM = ZEGOAPI_MIX_STREAM,
    /**  单主播模式，直播流会直接推到CDN，不经过即构服务器 */
    ZEGOAPI_SINGLE_ANCHOR = 1 << 2,
    ZEGO_SINGLE_ANCHOR = ZEGOAPI_SINGLE_ANCHOR,
};

typedef enum ZegoAPIPublishFlag ZegoApiPublishFlag;

/** 设备模块类型 */
enum ZegoAPIModuleType {
    /** 音频采集播放设备 */
    ZEGOAPI_MODULE_AUDIO = 0x4 | 0x8,
};

/** 推流质量 */
typedef struct {
    /** 视频采集帧率(fps) */
    double cfps;
    /** 视频编码帧率(fps) */
    double vencFps;
    /** 视频网络发送帧率(fps) */
    double fps;
    /** 视频码率(kbps) */
    double kbps;

    /** 音频采集帧率(fps) */
    double acapFps;
    /** 音频网络发送帧率(fps) */
    double afps;
    /** 音频码率(kbps) */
    double akbps;

    /** 本机到即构服务器的往返时延(ms) */
    int rtt;
    /** 发送丢包(0~255)，数值越大丢包越高，丢包率 = pktLostRate/255 */
    int pktLostRate;
    /** 本机综合网络质量(0~3)，分别对应优、良、中、差 */
    int quality;

    /** 是否开启硬件编码 */
    bool isHardwareVenc;
    /** 视频编码格式 参考ZegoVideoCodecAvc  */
    int videoCodecId;
    /** 视频宽度 */
    int width;
    /** 视频高度 */
    int height;

    /** 已发送的总字节数，包括音频、视频及媒体次要信息等 */
    double totalBytes;
    /** 已发送的音频字节数 */
    double audioBytes;
    /** 已发送的视频字节数 */
    double videoBytes;

    /** 当前 APP 的 CPU 使用率 */
    double cpuAppUsage;
    /** 当前系统的 CPU 使用率 */
    double cpuTotalUsage;

    /** 当前 APP 的内存使用率 */
    double memoryAppUsage;
    /** 当前系统的内存使用率 */
    double memoryTotalUsage;
    /** 当前 APP 的内存使用量,单位 MB */
    double memoryAppUsed;

} ZegoAPIPublishQuality;

typedef ZegoAPIPublishQuality ZegoApiPublishQuality;

/** 拉流质量 */
typedef struct {
    /** 视频帧率(网络接收) */
    double fps;
    /** 视频帧率(dejitter) */
    double vdjFps;
    /** 视频帧率(解码) */
    double vdecFps;
    /** 视频帧率(渲染) */
    double vrndFps;
    /** 视频码率(kb/s) */
    double kbps;

    /** 音频帧率(网络接收) */
    double afps;
    /** 音频帧率(dejitter) */
    double adjFps;
    /** 音频帧率(解码) */
    double adecFps;
    /** 音频帧率(渲染) */
    double arndFps;
    /** 音频码率(kb/s) */
    double akbps;
    /** 音频卡顿次数 */
    double audioBreakRate;
    /** 视频卡顿次数 */
    double videoBreakRate;

    /** 延时(ms) */
    int rtt;
    /** 丢包率(0~255) */
    int pktLostRate;
    /** 端到端延迟 */
    int peerToPeerDelay;
    /** 端到端丢包率(0~255) */
    int peerToPeerPktLostRate;
    /** 直播质量(0~3) */
    int quality;
    /** 语音延时(ms) */
    int delay;
    /** 音质评分。-1 表示未知。小于 2 表示音质非常差，杂音频现，大量语义丢失，完全无法交流。2 到 2.5
     * 表示音质很差，偶有杂音，部分语义丢失，难以交流。2.5 到 3 表示音质较差，卡顿频繁，需要集中精力才能听清。3 到 3.5
     * 表示音质一般，偶有卡顿，不是非常流畅，需要一点注意力才能听清。3.5 到 4 表示音质较好，偶有音质损伤，单依然清晰。大于 4
     * 表示音质佳，清晰流畅。 */
    double mos;

    /** 是否硬解 */
    bool isHardwareVdec;
    /** 视频解码格式 参考ZegoVideoCodecAvc */
    int videoCodecId;
    /** 视频宽度 */
    int width;
    /** 视频高度 */
    int height;

    /** 已接收的总字节数，包括音频、视频及媒体次要信息等 */
    double totalBytes;
    /** 已接收的音频字节数 */
    double audioBytes;
    /** 已接收的视频字节数 */
    double videoBytes;

    /** 当前 APP 的 CPU 使用率 */
    double cpuAppUsage;
    /** 当前系统的 CPU 使用率 */
    double cpuTotalUsage;

    /** 当前 APP 的内存使用率 */
    double memoryAppUsage;
    /** 当前系统的内存使用率 */
    double memoryTotalUsage;
    /** 当前 APP 的内存使用量,单位 MB */
    double memoryAppUsed;

    /** 音画不同步, 单位毫秒, 小于0表示视频超前音频的毫秒数, 大于0表示视频滞后音频的毫秒数, 等于0表示无差别.
     * 当绝对值小于200，可基本认为音画同步，当绝对值连续10秒大于200可以认为异常 */
    int avTimestampDiff;

    /** 累计音频卡顿次数 */
    unsigned int audioCumulativeBreakCount;
    /** 累计音频卡顿时长(ms) */
    unsigned int audioCumulativeBreakTime;
    /** 累计音频解码时长(ms) */
    unsigned int audioCumulativeDecodeTime;
    /** 累计音频卡顿比例(%) */
    double audioCumulativeBreakRate;
    /** 累计视频卡顿次数 */
    unsigned int videoCumulativeBreakCount;
    /** 累计视频卡顿时长(ms) */
    unsigned int videoCumulativeBreakTime;
    /** 累计视频解码时长(ms) */
    unsigned int videoCumulativeDecodeTime;
    /** 累计视频卡顿比例(%) */
    double videoCumulativeBreakRate;

} ZegoAPIPlayQuality;

typedef ZegoAPIPlayQuality ZegoApiPlayQuality;

/** 延迟模式 */
typedef enum : NSUInteger {
    /** 普通延迟模式 */
    ZEGOAPI_LATENCY_MODE_NORMAL = 0,
    /** 低延迟模式，无法用于 RTMP 流 */
    ZEGOAPI_LATENCY_MODE_LOW,
    /** 普通延迟模式，最高码率可达192K */
    ZEGOAPI_LATENCY_MODE_NORMAL2,
    /** 低延迟模式，无法用于 RTMP 流。相对于 ZEGO_LATENCY_MODE_LOW 而言，CPU 开销稍低 */
    ZEGOAPI_LATENCY_MODE_LOW2,
    /** 低延迟模式，无法用于 RTMP 流。支持WebRTC必须使用此模式 */
    ZEGOAPI_LATENCY_MODE_LOW3,
    /**< 普通延迟模式，使用此模式前先咨询即构技术支持 */
    ZEGOAPI_LATENCY_MODE_NORMAL3,
} ZegoAPILatencyMode;

/** 流量控制属性 */
typedef enum : NSUInteger {
    /**< 基本流量控制，只有码率控制，不带自适应帧率和分辨率 */
    ZEGOAPI_TRAFFIC_CONTROL_BASIC = 0,
    /**< 自适应帧率 */
    ZEGOAPI_TRAFFIC_CONTROL_ADAPTIVE_FPS = 1,
    /** 自适应分辨率*/
    ZEGOAPI_TRAFFIC_CONTROL_ADAPTIVE_RESOLUTION = 1 << 1,

    /** 音频流量控制 */
    ZEGOAPI_TRAFFIC_CONTROL_AUDIO_BITRATE = 1 << 2,

    /**< 废弃 */
    ZEGOAPI_TRAFFIC_NONE = ZEGOAPI_TRAFFIC_CONTROL_BASIC,
    ZEGOAPI_TRAFFIC_FPS = ZEGOAPI_TRAFFIC_CONTROL_ADAPTIVE_FPS,
    ZEGOAPI_TRAFFIC_RESOLUTION = ZEGOAPI_TRAFFIC_CONTROL_ADAPTIVE_RESOLUTION,

} ZegoAPITrafficControlProperty;

/** 开启流量控制视频码率低于设置的视频最低码率时 SDK 推流策略 */
typedef enum : NSUInteger {
    /** 低于设置的最低码率时，停止视频发送，默认 */
    ZEGOAPI_TRAFFIC_CONTROL_MIN_VIDEO_BITRATE_NO_VIDEO = 0,
    /** 低于设置的最低码率时，视频以极低的帧率发送 （不超过3fps) */
    ZEGOAPI_TRAFFIC_CONTROL_MIN_VIDEO_BITRATE_ULTRA_LOW_FPS

} ZegoAPITrafficControlMinVideoBitrateMode;

/** 音频设备模式 */
typedef enum : NSUInteger {
    /** 通话模式, 开启系统前处理模式(包括回声消除、噪声抑制、音量增益) */
    ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION = 1,
    /** 普通模式, 关闭系统前处理 */
    ZEGOAPI_AUDIO_DEVICE_MODE_GENERAL = 2,
    /** 自动模式, 根据场景选择是否开启系统前处理 */
    ZEGOAPI_AUDIO_DEVICE_MODE_AUTO = 3,
    /** 通话模式, 开启系统前处理，与 ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION 相比，该模式会始终占用麦克风设备 */
    ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION2 = 4,
    /** 通话模式, 开启系统前处理，与 ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION 相比，该模式会下麦后释放麦克风，切回媒体音量 */
    ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION3 = 5,
    /** 普通模式2,关闭系统前处理  与ZEGO_AUDIO_DEVICE_MODE_GENERAL 相比，该模式使用麦克风设备后不会释放 */
    ZEGOAPI_AUDIO_DEVICE_MODE_GENERAL2 = 6,
    /** 普通模式3,关闭系统前处理  与ZEGO_AUDIO_DEVICE_MODE_GENERAL 相比，该模式会始终占用麦克风设备 */
    ZEGOAPI_AUDIO_DEVICE_MODE_GENERAL3 = 7,
    /** 通话模式，开启系统前处理，与 ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION 相比，该模式会下麦后释放麦克风，维持通话音量 */
    ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION4 = 8,
} ZegoAPIAudioDeviceMode;

/** 音频录制时，指定音源类型 */
enum ZegoAPIAudioRecordMask {
    /** 关闭音频录制 */
    ZEGOAPI_AUDIO_RECORD_NONE = 0x0,
    /** 打开采集录制，即录制推流端音频 */
    ZEGOAPI_AUDIO_RECORD_CAP = 0x01,
    /** 打开渲染录制，即录制拉流端音频 */
    ZEGOAPI_AUDIO_RECORD_RENDER = 0x02,
    /** 打开采集和渲染混音结果录制，即录制推流、拉流端混音音频 */
    ZEGOAPI_AUDIO_RECORD_MIX = 0x04
};

/** 音频录制配置信息 */
typedef struct {
    /** 启用音频源选择，详细请参考 ZegoAPIAudioRecordMask */
    unsigned int mask;
    /** 采样率 支持 8000, 16000, 22050, 24000, 32000, 44100, 48000 */
    int sampleRate;
    /** 声道数 支持1(单声道) 或 2(双声道) */
    int channels;

} ZegoAPIAudioRecordConfig;

/** 推流通道 */
typedef enum : NSUInteger {
    /** 主推流通道，默认*/
    ZEGOAPI_CHN_MAIN = 0,
    /** 第二路推流通道, 默认没有声音，需要指定音频源。*/
    ZEGOAPI_CHN_AUX,
} ZegoAPIPublishChannelIndex;

/** 音频设备类型 */
typedef enum : NSUInteger {
    /**< 输入设备 */
    ZEGOAPI_AUDIO_DEVICE_INPUT = 0,
    /**< 输出设备 */
    ZEGOAPI_AUDIO_DEVICE_OUTPUT = 1,
} ZegoAPIAudioDeviceType;

/** 设备状态 */
typedef enum : NSUInteger {
    /**< 添加设备 */
    ZEGOAPI_DEVICE_ADD = 0,
    /**< 删除设备 */
    ZEGOAPI_DEVICE_DELETE = 1,
} ZegoAPIDeviceState;

/** 设备状态 */
typedef enum : NSInteger {
    /**< 设备状态未知，由于推流端的 SDK 版本低于 6.3.0 */
    ZEGOAPI_DEVICE_UNKNOWN = -1,
    /**< 设备已打开 */
    ZEGOAPI_DEVICE_OPEN = 0,
    /**< 设备已关闭 */
    ZEGOAPI_DEVICE_CLOSE = 1,
} ZegoAPIDeviceStatus;

/** 音量类型 */
typedef enum : NSUInteger {
    /**< 设备音量 */
    ZEGOAPI_VOLUME_ENDPOINT = 0,
    /**< App 音量 */
    ZEGOAPI_VOLUME_SIMPLE = 1,
} ZegoAPIVolumeType;

@interface ZegoAPIDeviceInfo : NSObject
/** 设备ID */
@property (copy) NSString *deviceId;
/** 设备名 */
@property (copy) NSString *deviceName;

@end

@interface ZegoAPIDeviceVideoCapabilityInfo : NSObject
/** 分辨率高 */
@property int height;
/** 分辨率宽 */
@property int width;
/** 帧率 */
@property int fps;

@end

typedef enum : NSUInteger {
    /**< 默认模式,SDK会根据拉流设置的rtmpUrls/flvUrls/shouldSwitchServer参数和即构的后台配置自动选择拉流资源 */
    DEFAULT = 0,
    /**<只从CDN拉流 */
    CDN_ONLY = 1,
    /**< 只从L3拉流,会忽略rtmpUrls/flvUrls/shouldSwitchServer 参数 */
    L3_ONLY = 2,
    /**< 只从RTC拉流 ,会忽略 rtmpUrls/flvUrls/shouldSwitchServer 参数 */
    RTC_ONLY = 3,
    /**< CDN Plus拉流 ,会忽略 rtmpUrls/flvUrls/shouldSwitchServer 参数 */
    CDN_PLUS = 4,
} ZegoAPIStreamResourceMode;

/**
 支持的 CDN 网络协议
 */
typedef NS_ENUM(NSUInteger, ZegoAPICDNProtocol) {
    /**
     TCP
    */
    ZEGOAPI_CDN_PROTOCOL_TCP = 1,
    /**
     QUIC
    */
    ZEGOAPI_CDN_PROTOCOL_QUIC = 2
};

/** 
 可用的 httpdns 服务 
 */
typedef NS_ENUM(NSUInteger, ZegoAPISupportHttpDNS) {

    /** 不使用 */
    ZEGOAPI_SUPPORT_HTTP_DNS_NONE = 0,
    /** 网宿 httpdns */
    ZEGOAPI_SUPPORT_HTTP_DNS_WANGSU = 1,
    /** 腾讯 httpdns */
    ZEGOAPI_SUPPORT_HTTP_DNS_TENCENT = 2,
    /** 阿里云 httpdns */
    ZEGOAPI_SUPPORT_HTTP_DNS_ALIYUN = 3,
};

/**
 自定义拉流 CDN 地址信息
*/
@interface ZegoAPICDNPlayUrlInfo : NSObject
/** 拉流地址 */
@property (nonatomic, copy) NSString *url;
/** 网络协议 */
@property (nonatomic, assign) ZegoAPICDNProtocol protocol;
/** 支持的quic版本列表 */
@property (nonatomic, strong) NSArray<NSString *> *quicVersions;
/** 指定 httpDNS 解析域名 */
@property (nonatomic, assign) ZegoAPISupportHttpDNS httpDNS;

@end

/**
 跨 APP 拉流信息
*/
@interface ZegoAPICrossAppInfo : NSObject
/** 跨app拉流时的目标appid */
@property (nonatomic, assign) unsigned int appid;
/** 拉流所需的Token */
@property (nonatomic, strong) NSData *token;
@end

/**
 多媒体流附加信息
 */
@interface ZegoAPIStreamExtraPlayInfo : NSObject

/** 流参数 */
@property (nonatomic, copy) NSString *params;
/** 解密密钥（支持16/24/32字节） */
@property (nonatomic, strong) NSData *decryptKey;
/** rtmp 地址，使用 TCP 网络协议 */
@property (nonatomic, strong) NSArray<NSString *> *rtmpUrls;
/** flv 地址，使用 TCP 网络协议 */
@property (nonatomic, strong) NSArray<NSString *> *flvUrls;

/** rtmp 地址，可选择 TCP 或者 QUIC 网络协议 */
@property (nonatomic, strong) NSArray<ZegoAPICDNPlayUrlInfo *> *advancedRtmpUrls;
/** flv 地址，可选择 TCP 或者 QUIC 网络协议 */
@property (nonatomic, strong) NSArray<ZegoAPICDNPlayUrlInfo *> *advancedFlvUrls;

/** 连麦时是否切换服务器 */
@property (nonatomic, assign) BOOL shouldSwitchServer;

/** 自定义拉流模式 默认auto */
@property (nonatomic, assign) ZegoAPIStreamResourceMode mode;

/** 源流编码模式 默认VIDEO_CODEC_UNKNOWN */
@property (nonatomic, assign) ZegoVideoCodecAvc videoCodecID;

/** 源流资源类型 默认RESOURCE_TYPE_RTC */
@property (nonatomic, assign) ZegoResourceType sourceResourceType;

/** 跨 APP 拉流信息 */
@property (nonatomic, strong) ZegoAPICrossAppInfo *crossAppInfo;

/** 预配置的编解码器模板 */
@property (nonatomic, assign) int codecTemplateId;

@end

/**
 转推CDN状态
 */

typedef enum : NSUInteger {
    /**< 转推停止 */
    ZEGOAPI_RELAY_STOP = 0,
    /**< 正在转推 */
    ZEGOAPI_RELAY_START = 1,
    /**< 正在重试 */
    ZEGOAPI_RELAY_RETRY = 2,
} ZegoAPIStreamRelayCDNState;

typedef enum : NSUInteger {
    /**< 无 */
    ZEGOAPI_RELAY_NONE = 0,
    /**< 服务器错误 */
    ZEGOAPI_RELAY_SERVER_ERROR = 8,
    /**< 握手失败 */
    ZEGOAPI_RELAY_HAND_SHAKE_FAILED = 9,
    /**< 接入点错误 */
    ZEGOAPI_RELAY_ACCESS_POINT_ERROR = 10,
    /**< 创建流失败 */
    ZEGOAPI_RELAY_CREATE_STREAM_FAILED = 11,
    /**< BAD NAME */
    ZEGOAPI_RELAY_BAD_NAME = 12,
    /**< CDN服务器主动断开 */
    ZEGOAPI_RELAY_CDN_SERVER_DISCONNECTED = 13,
    /**< 主动断开 */
    ZEGOAPI_RELAY_DISCONNECTED = 14,
    /**< 混流输入流会话关闭, 混流转推CDN时有效 */
    ZEGOAPI_MIXSTREAM_ALL_INPUT_STREAM_CLOSED = 1214,
    /**< 混流输入流全部没有数据, 混流转推CDN时有效 */
    ZEGOAPI_MIXSTREAM_ALL_INPUT_STREAM_NODATA = 1215,
    /**< 混流服务器内部错误，混流转推CDN时有效 */
    ZEGOAPI_MIXSTREAM_SERVER_INTERNAL_ERROR = 1230,
} ZegoAPIStreamRelayCDNDetail;

/**
 转推CDN状态信息
 */
@interface ZegoAPIStreamRelayCDNInfo : NSObject

/** 转推CDN的rtmp地址 */
@property (copy) NSString *rtmpURL;
/** 当前状态 */
@property (assign) ZegoAPIStreamRelayCDNState state;
/** 转推停止或转推重试时的详细原因 */
@property (assign) ZegoAPIStreamRelayCDNDetail detail;
/** 状态改变时的时间 */
@property (assign) unsigned int stateTime;

@end

@interface ZegoAPICodecCapabilityInfo : NSObject
/** 编码格式 */
@property (assign) ZegoVideoCodecAvc codecId;
/** 是否硬编 */
@property (assign) int isHardware;

@end

/**
 拉的所有rtc和l3流的统计数据
 */
@interface ZegoAPIPlayStats : NSObject
/** 下行总码率(kb/s) */
@property (assign) double rxKBitrate;
/** 下行音频码率(kb/s) */
@property (assign) double rxAudioKBitrate;
/** 下行视频码率(kb/s) */
@property (assign) double rxVideoKBitrate;

@end

/**
 ACE 模式
 */
typedef enum : NSUInteger {
    /** 激进模式 */
    ZEGOAPI_AEC_MODE_ARRGRESSIVE,
    /** 中等模式 */
    ZEGOAPI_AEC_MODE_MEDIUM,
    /** 轻度模式 */
    ZEGOAPI_AEC_MODE_SOFT,
} ZegoAPIAECMode;

/**
 ANS 模式
 */
typedef enum : NSUInteger {
    /** 轻度模式 */
    ZEGOAPI_ANS_MODE_LOW = 0,
    /** 中等模式 */
    ZEGOAPI_ANS_MODE_MEDIUM,
    /** 激进模式 */
    ZEGOAPI_ANS_MODE_HIGH,
    /** AI 模式 */
    ZEGOAPI_ANS_MODE_AI,
} ZegoAPIANSMode;

typedef enum : NSInteger {
    /** 一般性错误 */
    ZEGOAPI_DEVICE_ERROR_GENERIC = -1,
    /** 无效设备 ID */
    ZEGOAPI_DEVICE_ERROR_INVALID_ID = -2,
    /** 没有权限 */
    ZEGOAPI_DEVICE_ERROR_NO_AUTHORIZATION = -3,
    /** 采集帧率为0 */
    ZEGOAPI_DEVICE_ERROR_ZERO_FPS = -4,
    /** 设备被占用 */
    ZEGOAPI_DEVICE_ERROR_IN_USE_BY_OTHER = -5,
    /** 设备未插入 */
    ZEGOAPI_DEVICE_ERROR_UNPLUGGED = -6,
    /** 媒体服务无法恢复 */
    ZEGOAPI_DEVICE_ERROR_MEDIA_SERVICES_LOST = -8,
    /** 设备被 Siri 占用 */
    ZEGOAPI_DEVICE_ERROR_IN_USE_BY_SIRI = -9,
    /** 设备采集的声音过低 */
    ZEGOAPI_DEVICE_ERROR_SOUND_LEVEL_TOO_LOW = -10,
    /** 可能是由 iPad 磁吸保护套引起的设备被占用问题 */
    ZEGOAPI_DEVICE_ERROR_MAGNETIC_CASE = -11,
} ZegoAPIDeviceErrorCode;

typedef enum : NSInteger {
    /** 一般性错误 */
    ZEGOAPI_DEVICE_ERROR_REASON_GENERIC = -1,
    /** 无效设备 ID */
    ZEGOAPI_DEVICE_ERROR_REASON_INVALID_ID = -2,
    /** 没有权限 */
    ZEGOAPI_DEVICE_ERROR_REASON_NO_AUTHORIZATION = -3,
    /** 采集帧率为0 */
    ZEGOAPI_DEVICE_ERROR_REASON_ZERO_FPS = -4,
    /** 设备被占用 */
    ZEGOAPI_DEVICE_ERROR_REASON_IN_USE_BY_OTHER = -5,
    /** 设备未插入 */
    ZEGOAPI_DEVICE_ERROR_REASON_UNPLUGGED = -6,
    /** 需要重启系统 */
    ZEGOAPI_DEVICE_ERROR_REASON_REBOOT_REQUIRED = -7,
    /** 媒体服务无法恢复 */
    ZEGOAPI_DEVICE_ERROR_REASON_MEDIA_SERVICES_LOST = -8,
    /** 设备被 Siri 占用 */
    ZEGOAPI_DEVICE_ERROR_REASON_IN_USE_BY_SIRI = -9,
    /** 设备采集的声音过低 */
    ZEGOAPI_DEVICE_ERROR_REASON_SOUND_LEVEL_TOO_LOW = -10,
    /** 可能是由 iPad 磁吸保护套引起的设备被占用问题 */
    ZEGOAPI_DEVICE_ERROR_REASON_MAGNETIC_CASE = -11,
    /** 没有错误 */
    ZEGOAPI_DEVICE_ERROR_REASON_NONE = 0,
    /** 禁用 */
    ZEGOAPI_DEVICE_ERROR_REASON_DISABLED = 2,
    /** 屏蔽采集 */
    ZEGOAPI_DEVICE_ERROR_REASON_MUTE = 3,
    /** 中断 */
    ZEGOAPI_DEVICE_ERROR_REASON_INTERRUPTION = 4,
    /** 在后台 */
    ZEGOAPI_DEVICE_ERROR_REASON_IN_BACKGROUND = 5,
    /** 前台有多个 APP 运行 */
    ZEGOAPI_DEVICE_ERROR_REASON_MULTI_FOREGROUND_APP = 6,
    /** 系统压力过大 */
    ZEGOAPI_DEVICE_ERROR_REASON_SYSTEM_PRESSURE = 7,

} ZegoAPIDeviceErrorReason;

/** 多路混音模式 */
typedef NS_ENUM(NSInteger, ZegoAPIAudioMixMode) {
    /** 关闭效果 */
    ZEGOAPI_AUDIO_MIX_MODE_RAW = 0,
    /** 聚焦效果 */
    ZEGOAPI_AUDIO_MIX_MODE_FOCUSED = 1
};

/** 音频路由类型 */
typedef NS_ENUM(NSInteger, ZegoAPIAudioRoute) {
    /** 扬声器 */
    ZEGOAPI_AUDIO_ROUTE_SPEAKER = 0,
    /** 耳机 */
    ZEGOAPI_AUDIO_ROUTE_HEADSET,
    /** 蓝牙 */
    ZEGOAPI_AUDIO_ROUTE_BLUETOOTH,
    /** 听筒 */
    ZEGOAPI_AUDIO_ROUTE_RECEIVER,
    /** USB 音频设备 */
    ZEGOAPI_AUDIO_ROUTE_USB_AUDIO,
    /** air play */
    ZEGOAPI_AUDIO_ROUTE_AIR_PLAY

};

/** 网络类型 */
typedef NS_ENUM(NSInteger, ZegoAPINetType) {
    /** 无网络 */
    ZEGOAPI_NT_NONE = 0,
    /** 网线 */
    ZEGOAPI_NT_LINE = 1,
    /**  WIFI */
    ZEGOAPI_NT_WIFI = 2,
    /** 2G */
    ZEGOAPI_NT_2G = 3,
    /** 3G */
    ZEGOAPI_NT_3G = 4,
    /** 4G */
    ZEGOAPI_NT_4G = 5,
    /** 5G */
    ZEGOAPI_NT_5G = 6,
    /** 未知类型 */
    ZEGOAPI_NT_UNKNOWN = 32
};

/** sdk 任务类型*/
typedef NS_ENUM(NSInteger, ZegoAPITaskType) {
    /** 正常任务 */
    ZEGOAPI_TASK_NORMAL = 1,
    /** 延时类任务(如定时器) */
    ZEGOAPI_TASK_DELAY = 2,
};

/** 编解码器错误码 */
typedef NS_ENUM(NSInteger, ZegoAPICodecError) {
    /** 成功 */
    ZEGOAPI_CODEC_ERROR_NONE = 0,
    /** 不支持 */
    ZEGOAPI_CODEC_ERROR_NOT_SUPPORT = -1,
    /** 失败 */
    ZEGOAPI_CODEC_ERROR_FAILED = -2,
    /** 软解性能不足 */
    ZEGOAPI_CODEC_ERROR_LOW_FPS = -3,
};

/** 触发流量控制的因素 */
typedef NS_ENUM(NSInteger, ZegoAPITrafficControlFocusOn) {
    /** 只关注本地网络 */
    ZEGOAPI_TRAFFIC_CONTROL_FOCUS_ON_LOCAL_ONLY = 0,
    /** 关注本地网络, 同时也兼顾远端网络,目前只在 1v1 场景下有效 */
    ZEGOAPI_TRAFFIC_CONTROL_FOCUS_ON_REMOTE = 1,
};

/** 视频数据来源，例如想设置辅路推流视频数据直接复制主路视频数据，则可以设置为 VIDEO_SRC_MAIN_PUBLISH_CHN */
typedef NS_ENUM(NSInteger, ZegoVideoSourceType) {
    /** 默认采集逻辑，即如果有设置视频外部采集工厂，则使用外部采集，否则使用内部采集 */
    VIDEO_SRC_DEFAULT = 0,
    /** 不启动采集，即没有视频数据 */
    ZEGOAPI_VIDEO_SRC_NONE = 1,
    /** 视频源来自于摄像头 */
    ZEGOAPI_VIDEO_SRC_CAMERA = 2,
    /** 视频源来自于外部采集。当没有设置外部采集工厂时，将自动修正为 VIDEO_SRC_CAMERA */
    ZEGOAPI_VIDEO_SRC_EXTERNAL_CAPTURE = 3,
    /** 视频源来自于主路推流。当推主路流时，不能设置为此值 */
    ZEGOAPI_VIDEO_SRC_MAIN_PUBLISH_CHN = 4,
};

/** 音频数据来源，例如想设置辅路推流音频数据直接复制主路音频数据，则可以设置为 AUDIO_SRC_MAIN_PUBLISH_CHN */
typedef NS_ENUM(NSInteger, ZegoAudioSourceType) {
    /** 不启动采集，即没有音频数据 */
    AUDIO_SRC_NONE = 0,
    /** 音频源来自于麦克风 */
    AUDIO_SRC_MIC = 1,
    /** 音频源来自于外部采集 */
    AUDIO_SRC_EXTERNAL_CAPTURE = 2,
    /** 音频源来自于媒体采集 */
    AUDIO_SRC_MEDIA_CAPTURE = 3,
    /** 音频源来自于主路推流。当推主路流时，不能设置为此值 */
    AUDIO_SRC_MAIN_PUBLISH_CHN = 4,
    /** 音频源来自于媒体播放器 */
    AUDIO_SRC_PLAYER = 5,
};

/** 视频编码模式 */
typedef NS_ENUM(NSInteger, ZegoAPIVideoEncodeMode) {
    /** 低延时 */
    ZEGOAPI_VIDEO_ENCODE_MODE_LOW_DELAY = 0,
    /** 低码率 */
    ZEGOAPI_VIDEO_ENCODE_MODE_LOW_BITRATE = 1,
};

/**
语音检测结果类型
*/
typedef NS_ENUM(NSInteger, ZegoAPIAudioVADType) {
    /** 噪声 */
    ZEGOAPI_AUDIO_VAD_TYPE_NOISE = 0,
    /** 人声 */
    ZEGOAPI_AUDIO_VAD_TYPE_SPEECH = 1,
};

/**
 视频编码规格
 */
typedef NS_ENUM(NSInteger, ZegoAPIEncodeProfile) {
    /**
     Baseline 级别的视频编码规格，解码消耗较低，画面效果较差，一般用于低阶或需要额外容错的应用。
     */
    ZEGOAPI_ENCODE_PROFILE_BASELINE = 0,
    /**
     Main 级别的视频编码规格，解码消耗比 Baseline 稍高，画面效果也较好，一般用于主流消费类电子产品。
     */
    ZEGOAPI_ENCODE_PROFILE_MAIN = 1,
    /**
     High 级别的视频编码规格，解码消耗比 Main 高，画面效果好，一般用于广播及视频碟片存储，高清电视。
     */
    ZEGOAPI_ENCODE_PROFILE_HIGH = 2,
};

/**
 视频朝向模式
 */
typedef NS_ENUM(NSUInteger, ZegoOrientationMode) {
    /**
     * @brief 自定义模式，默认为该模式，推流端用户需要通过 [SetAppOrientation] 设置朝向，通过 [SetVideoConfig] 设置视频分辨率控制视频比例。
     */
    ZEGOAPI_ORIENTATION_MODE_CUSTOM = 0,
    /**
     * @brief 自适应模式，拉流端视频方向自动垂直地面向上，推流端用户不再需要通过 [SetAppOrientation] 设置朝向。
     * @attention 推流端和拉流端都需要设置为拉流自适应模式。
     * @attention 不支持媒体播放器、云录制、本地录制，不支持通过 CDN 推拉流场景。
     * @attention SDK 在该模式下会根据设备实际朝向自动调换编码分辨率的宽高。
     */
    ZEGOAPI_ORIENTATION_MODE_ADAPTION = 1,
    /**
     * @brief 对齐模式。以 Status Bar 作为参考，拉流端的视频方向与推流端预览的视频方向一致。SDK 在拉流端会以 Status Bar 作为参考对图像进行旋转，旋转角度和推流端预览的旋转角度一致。推流端用户不再需要通过 [SetAppOrientation] 设置朝向，不再需要通过 [SetVideoConfig] 设置视频分辨率控制视频比例。
     * @attention 不支持媒体播放器、云录制、本地录制，不支持通过 CDN 推拉流场景。
     * @attention SDK 在该模式下会根据 Status Bar 实际位置自动调换编码分辨率的宽高。
     */
    ZEGOAPI_ORIENTATION_MODE_ALIGNMENT = 2,
    /**
     * @brief 固定比例模式。以 Status Bar 作为参考，拉流端的视频方向与推流端预览的视频方向一致，且视频分辨率与编码分辨率保持一致，推流端用户不再需要通过 [SetAppOrientation] 设置朝向。
     */
    ZEGOAPI_ORIENTATION_MODE_FIXED_RESOLUTION_RATIO = 3
};

/**  */
typedef NS_ENUM(NSUInteger, ZegoStreamEvent) {
    /** 开始推流 */
    ZEGOAPI_STREAM_EVENT_PUBLISH_START = 100,
    /** 推流成功 */
    ZEGOAPI_STREAM_EVENT_PUBLISH_SUCCESS = 101,
    /** 推流失败 */
    ZEGOAPI_STREAM_EVENT_PUBLISH_FAIL = 102,
    /** 开始重试推流 */
    ZEGOAPI_STREAM_EVENT_RETRY_PUBLISH_START = 103,
    /** 重试推流成功 */
    ZEGOAPI_STREAM_EVENT_RETRY_PUBLISH_SUCCESS = 104,
    /** 重试推流失败 */
    ZEGOAPI_STREAM_EVENT_RETRY_PUBLISH_FAIL = 105,
    /** 推流结束 */
    ZEGOAPI_STREAM_EVENT_PUBLISH_END = 106,
    /** 开始拉流 */
    ZEGOAPI_STREAM_EVENT_PLAY_START = 200,
    /** 拉流成功 */
    ZEGOAPI_STREAM_EVENT_PLAY_SUCCESS = 201,
    /** 拉流失败 */
    ZEGOAPI_STREAM_EVENT_PLAY_FAIL = 202,
    /** 开始重试拉流 */
    ZEGOAPI_STREAM_EVENT_RETRY_PLAY_START = 203,
    /** 重试拉流成功 */
    ZEGOAPI_STREAM_EVENT_RETRY_PLAY_SUCCESS = 204,
    /** 重试拉流失败 */
    ZEGOAPI_STREAM_EVENT_RETRY_PLAY_FAIL = 205,
    /** 拉流结束 */
    ZEGOAPI_STREAM_EVENT_PLAY_END = 206,
};

/**
自定义直推 CDN 地址信息
*/
@interface ZegoAPICDNPublishTarget : NSObject
/** 推流地址 */
@property (nonatomic, copy) NSString *url;
/** 支持的协议列表，按照顺序尝试 */
@property (nonatomic, strong) NSArray<NSNumber *> *protocol_list;
/** 支持的 QUIC 版本列表，如果支持 QUIC 协议，需要填写此信息 */
@property (nonatomic, strong) NSArray<NSString *> *quic_version_list;

@end

typedef NS_ENUM(NSUInteger, ZegoLowlightEnhanceMode) {
    /** 关闭低照度增强,采集画面亮度不变 */
    ZEGO_LOWLIGHT_ENHANCE_MODE_OFF = 0,
    /** 开启低照度增强，采集画面亮度增强 */
    ZEGO_LOWLIGHT_ENHANCE_MODE_ON = 1,
    /** 自动低照度增强，采集画面亮度增强值随环境亮度动态调整 */
    ZEGO_LOWLIGHT_ENHANCE_MODE_AUTO = 2
};

/// 视频配置偏好
typedef NS_ENUM(NSUInteger, ZegoVideoConfigPreference) {
    /** 流畅优先 */
    ZEGO_VIDEO_CONFIG_PREFERENCE_SMOOTH = 0,
    /** 清晰优先 */
    ZEGO_VIDEO_CONFIG_PREFERENCE_CLEAR = 1,
    /** 均衡模式 */
    ZEGO_VIDEO_CONFIG_PREFERENCE_BALANCED = 2
};

/** 自适应视频配置 */
@interface ZegoAdaptiveVideoConfig : NSObject

/// 自适应视频配置偏好，0 流畅优先，1 清晰优先，2 均衡模式。默认为 2。
@property (nonatomic, assign) ZegoVideoConfigPreference preference;

/// 动态调整允许的最小分辨率宽，编码宽不会低于此设定值，单位 px。默认为 240。
@property (nonatomic, assign) int minEncodeWidth;

/// 动态调整允许的最小分辨率高，编码高不会低于此设定值，单位 px。默认为 240。
@property (nonatomic, assign) int minEncodeHeight;

/// 动态调整允许的最小视频帧率，编码帧率不会低于此设定值，单位 fps。默认为 10。
@property (nonatomic, assign) int minFPS;

@end

#endif /* zego_api_defines_oc_h */
