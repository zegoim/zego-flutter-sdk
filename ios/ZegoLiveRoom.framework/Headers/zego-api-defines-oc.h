//
//  zego-api-defines-oc.h
//  zegoavkit
//
//  Copyright © 2017年 Zego. All rights reserved.
//

#ifndef zego_api_defines_oc_h
#define zego_api_defines_oc_h

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

#ifdef __cplusplus
#define ZEGO_EXTERN     extern "C"
#else
#define ZEGO_EXTERN     extern
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

typedef unsigned int	uint32;

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
    /** 等比缩放，可能有黑边 */
    ZegoVideoViewModeScaleAspectFit     = 0,
    /** 等比缩放填充整View，可能有部分被裁减 */
    ZegoVideoViewModeScaleAspectFill    = 1,
    /** 填充整个View */
    ZegoVideoViewModeScaleToFill        = 2,
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
}ZegoVideoMirrorMode;

/** 采集旋转角度，逆时针旋转 */
typedef enum {
    /** 旋转 0 度 */
    CAPTURE_ROTATE_0    = 0,
    /** 旋转 90 度 */
    CAPTURE_ROTATE_90   = 90,
    /** 旋转 180 度 */
    CAPTURE_ROTATE_180  = 180,
    /** 旋转 270 度 */
    CAPTURE_ROTATE_270  = 270
} CAPTURE_ROTATE;

/** 视频编解码器. */
typedef enum {
    /** 默认编码, 不支持分层编码 */
    VIDEO_CODEC_DEFAULT = 0,
    /** 分层编码 要达到和VIDEO_CODEC_DEFAULT相同的编码质量，建议码率和VIDEO_CODEC_DEFAULT相比增加20%左右 */
    VIDEO_CODEC_MULTILAYER = 1,
    /** VP8编码 */
    VIDEO_CODEC_VP8 = 2,
} ZegoVideoCodecAvc;

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
     <br>但是在其它 CDN 上转码视频的时候，其它 CDN 基本上不支持提取这种方式打包的信息数据，转码完成后再从其它 CDN 拉流时，可能就丢失了这些次媒体信息。
     <br>ZEGO CDN 转码支持提取此种方式打包的信息数据。
     */
    SideInfoZegoDefined = 0,
    /**
     采用 H264 的 SEI (nalu type = 6,payload type = 243) 类型打包，此类型是 SEI 标准未规定的类型，跟视频编码器或者视频文件中的 SEI 不存在冲突性，用户不需要根据 SEI 的内容做过滤。
     <br>若需要发送 SEI 推荐采用此种类型。
     */
    SeiZegoDefined = 1,
    /**
     采用 H264 的 SEI (nalu type = 6,payload type = 5) 类型打包，H264 标准对于此类型有规定的格式：startcode + nalu type(6) + payload type(5) + len + pay load(uuid + context)+ trailing bits；
     
     <br>因为视频编码器自身会产生 payload type 为 5 的 SEI，或者使用视频文件推流时，视频文件中也可能存在这样的 SEI，所以使用此类型时，用户需要把 uuid + context 当作一段 buffer 塞给次媒体的发送接口；
     
     <br>为了区别视频编码器自身产生的 SEI，所以对 uuid 有格式要求，即 uuid 16字节的前四个字节固定为 'Z' 'E' 'G' 'O' 四个字符（全部大写），后面12字节用户任意填写；
     
     <br>在 SDK 接收端，对于 payload type = 5的 SEI 会根据'ZEGO'字样做过滤，识别出符合要求的 SEI 抛给用户，避免用户收到编码器自身产生的 SEI。
     */
    SeiUserUnregisted = 2
}MediaInfoType;

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
    ZEGO_FILTER_NONE        = 0,
    /**  简洁 */
    ZEGO_FILTER_LOMO        = 1,
    /**  黑白 */
    ZEGO_FILTER_BLACKWHITE  = 2,
    /**  老化 */
    ZEGO_FILTER_OLDSTYLE    = 3,
    /**  哥特 */
    ZEGO_FILTER_GOTHIC      = 4,
    /**  锐色 */
    ZEGO_FILTER_SHARPCOLOR  = 5,
    /**  淡雅 */
    ZEGO_FILTER_FADE        = 6,
    /**  酒红 */
    ZEGO_FILTER_WINE        = 7,
    /**  青柠 */
    ZEGO_FILTER_LIME        = 8,
    /**  浪漫 */
    ZEGO_FILTER_ROMANTIC    = 9,
    /**  光晕 */
    ZEGO_FILTER_HALO        = 10,
    /**  蓝调 */
    ZEGO_FILTER_BLUE        = 11,
    /**  梦幻 */
    ZEGO_FILTER_ILLUSION    = 12,
    /**  夜色 */
    ZEGO_FILTER_DARK        = 13
} ZegoFilter;

/** 美颜特性 */
typedef enum : NSUInteger {
    /**  无美颜 */
    ZEGO_BEAUTIFY_NONE          = 0,
    /**  磨皮 */
    ZEGO_BEAUTIFY_POLISH        = 1,
    /**  全屏美白，一般与磨皮结合使用：ZEGO_BEAUTIFY_POLISH | ZEGO_BEAUTIFY_WHITEN */
    ZEGO_BEAUTIFY_WHITEN        = 1 << 1,
    /**  皮肤美白 */
    ZEGO_BEAUTIFY_SKINWHITEN    = 1 << 2,
    /**  锐化 */
    ZEGO_BEAUTIFY_SHARPEN       = 1 << 3
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
 音浪ID，用于标识用户（比如拉取混流方可根据此标识明确到混流中的单条流是主播/观众/副主播），soundLevelID 必须大于等于 0 且小于等于 4294967295L（即2^32-1）
 */
@property unsigned int soundLevelID;
/**
 推流内容控制， 0 表示输出的混流包含音视频，1 表示只包含音频，2 表示只包含视频；默认值为 0。
 */
@property int contentControl;

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
 <br>此参数中的宽必须大于等于 输入流列表中所有输入流中最大的分辨率宽，即right布局值；此参数中的高必须大于等于 输入流列表中所有输入流中最大的分辨率高，即bottom布局值；且输入流的布局位置不能超出此参数规定的范围。
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
@property (strong) NSMutableArray<ZegoMixStreamInfo*> *inputStreamList;
/**
 用户自定义数据
 <b>注意：</b>1. userData自定义的数据通过媒体次要信息的 -onRecvMediaSideInfo:ofStream: 接口回调出来。
 */
@property NSData* userData;
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
typedef enum
{
    /** 恒定码率 */
    ZEGOAPI_RC_ABR = 0,
    /** 恒定码率 */
    ZEGOAPI_RC_CBR,
    /** 恒定质量,仅用于研究目的 */
    ZEGOAPI_RC_VBR,
    /** 恒定质量 */
    ZEGOAPI_RC_CRF,
} ZegoAPIVideoEncoderRateControlStrategy;

/** 发布直播的模式 */
enum ZegoAPIPublishFlag
{
    /**  连麦模式，直播流会推到即构服务器，然后转推到CDN，默认连麦者之间从即构服务器拉流，观众从 CDN 拉流 */
    ZEGOAPI_JOIN_PUBLISH    = 0,
    ZEGO_JOIN_PUBLISH       = ZEGOAPI_JOIN_PUBLISH,
    /**  混流模式，同连麦模式，有混流需求时使用 */
    ZEGOAPI_MIX_STREAM      = 1 << 1,
    ZEGO_MIX_STREAM         = ZEGOAPI_MIX_STREAM,
    /**  单主播模式，直播流会直接推到CDN，不经过即构服务器 */
    ZEGOAPI_SINGLE_ANCHOR   = 1 << 2,
    ZEGO_SINGLE_ANCHOR      = ZEGOAPI_SINGLE_ANCHOR,
};

typedef enum ZegoAPIPublishFlag ZegoApiPublishFlag;

/** 设备模块类型 */
enum ZegoAPIModuleType
{
    /** 音频采集播放设备 */
    ZEGOAPI_MODULE_AUDIO            = 0x4 | 0x8,
};

/** 推流质量 */
typedef struct
{
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
    /** 视频宽度 */
    int width;
    /** 视频高度 */
    int height;
    
} ZegoAPIPublishQuality;

typedef ZegoAPIPublishQuality ZegoApiPublishQuality;


/** 拉流质量 */
typedef struct
{
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
    
    /** 是否硬解 */
    bool isHardwareVdec;
    /** 视频宽度 */
    int width;
    /** 视频高度 */
    int height;
    
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
    /** 低于设置的最低码率时，视频以极低的帧率发送 （不超过2fps) */
    ZEGOAPI_TRAFFIC_CONTROL_MIN_VIDEO_BITRATE_ULTRA_LOW_FPS
    
} ZegoAPITrafficControlMinVideoBitrateMode;

/** 音频设备模式 */
typedef enum : NSUInteger {
    /** 通话模式, 开启系统回声消除 */
    ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION = 1,
    /** 普通模式, 关闭系统回声消除 */
    ZEGOAPI_AUDIO_DEVICE_MODE_GENERAL = 2,
    /** 自动模式, 根据场景选择是否开启系统回声消除 */
    ZEGOAPI_AUDIO_DEVICE_MODE_AUTO = 3,
    /** 通话模式, 开启系统回声消除，与communication相比，communication2会始终占用麦克风设备 */
    ZEGOAPI_AUDIO_DEVICE_MODE_COMMUNICATION2 = 4,
} ZegoAPIAudioDeviceMode;

/** 音频录制时，指定音源类型 */
enum ZegoAPIAudioRecordMask
{
    /** 关闭音频录制 */
    ZEGOAPI_AUDIO_RECORD_NONE      = 0x0,
    /** 打开采集录制，即录制推流端音频 */
    ZEGOAPI_AUDIO_RECORD_CAP       = 0x01,
    /** 打开渲染录制，即录制拉流端音频 */
    ZEGOAPI_AUDIO_RECORD_RENDER    = 0x02,
    /** 打开采集和渲染混音结果录制，即录制推流、拉流端混音音频 */
    ZEGOAPI_AUDIO_RECORD_MIX       = 0x04
};

/** 音频录制配置信息 */
typedef struct
{
    /** 启用音频源选择，详细请参考 ZegoAVAPIAudioRecordMask */
    unsigned int mask;
    /** 采样率 支持 8000, 16000, 22050, 24000, 32000, 44100, 48000 */
    int sampleRate;
    /** 声道数 支持1(单声道) 或 2(双声道) */
    int channels;
    
} ZegoAPIAudioRecordConfig;

/** 推流通道 */
typedef enum :  NSUInteger {
    /** 主推流通道，默认*/
    ZEGOAPI_CHN_MAIN        =   0,
    /** 第二路推流通道, 无法推出声音*/
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
typedef enum : NSUInteger
{
    /**< 添加设备 */
    ZEGOAPI_DEVICE_ADD = 0,
    /**< 删除设备 */
    ZEGOAPI_DEVICE_DELETE = 1,
} ZegoAPIDeviceState;

/** 设备状态 */
typedef enum : NSUInteger
{
    /**< 设备已打开 */
    ZEGOAPI_DEVICE_OPEN = 0,
    /**< 设备已关闭 */
    ZEGOAPI_DEVICE_CLOSE = 1,
} ZegoAPIDeviceStatus;

/** 音量类型 */
typedef enum : NSUInteger
{
    /**< 设备音量 */
    ZEGOAPI_VOLUME_ENDPOINT = 0,
    /**< App 音量 */
    ZEGOAPI_VOLUME_SIMPLE = 1,
} ZegoAPIVolumeType;

@interface ZegoAPIDeviceInfo : NSObject
/** 设备ID */
@property (copy) NSString* deviceId;
/** 设备名 */
@property (copy) NSString* deviceName;

@end

/**
 多媒体流附加信息
 */
@interface ZegoAPIStreamExtraPlayInfo : NSObject

/** 流参数 */
@property (copy) NSString* params;
/** rtmp 地址 */
@property (strong) NSArray<NSString*>* rtmpUrls;
/** flv 地址 */
@property (strong) NSArray<NSString*>* flvUrls;
/** 连麦时是否切换服务器 */
@property (assign) BOOL shouldSwitchServer;

@end

/**
 混流中，发言者及其说话音量信息。
 */
@interface ZegoSoundLevelInMixedStreamInfo : NSObject

/**
 音浪ID，用于标识用户，对应于 ZegoMixStreamConfig 的 inputStreamList 中的单条输入流信息的 soundLevelID 参数的设置值。
 */
@property (assign) unsigned int soundLevelID;
/**
 音量level
 */
@property (assign) unsigned char soundLevel;

@end

/**
 转推CDN状态
 */

typedef enum : NSUInteger
{
    /**< 转推停止 */
    ZEGOAPI_RELAY_STOP = 0,
    /**< 正在转推 */
    ZEGOAPI_RELAY_START = 1,
    /**< 正在重试 */
    ZEGOAPI_RELAY_RETRY = 2,
} ZegoAPIStreamRelayCDNState;

typedef enum : NSUInteger
{
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

/**
 ACE 模式
 */
typedef enum : NSUInteger
{
    /** 激进模式 */
    ZEGOAPI_AEC_MODE_ARRGRESSIVE,
    /** 中等模式 */
    ZEGOAPI_AEC_MODE_MEDIUM,
    /** 轻度模式 */
    ZEGOAPI_AEC_MODE_SOFT,
} ZegoAPIAECMode;


typedef enum : NSInteger
{
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

} ZegoAPIDeviceErrorCode;

typedef enum : NSInteger
{
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
    /** 媒体服务无法恢复 */
    ZEGOAPI_DEVICE_ERROR_REASON_MEDIA_SERVICES_LOST = -8,
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

}ZegoAPIDeviceErrorReason;

#endif /* zego_api_defines_oc_h */
