//
//  zego-api-mediaplayer-defines-oc.h
//  ZegoLiveRoom
//
//  Created by kebo on 2018/8/10.
//

#ifndef zego_api_mediaplayer_defines_oc_h
#define zego_api_mediaplayer_defines_oc_h



/**
 播放器模式
 
 - MediaPlayerTypePlayer: 播放模式
 - MediaPlayerTypeAux: 播放，并且将其混入推流中
 */
typedef NS_ENUM(NSInteger, MediaPlayerType) {
    
    MediaPlayerTypePlayer,
    MediaPlayerTypeAux
};

typedef NS_ENUM(NSInteger, ZegoMediaPlayerVideoPixelFormat) {
    ZegoMediaPlayerVideoPixelFormatUnknown  = 0,
    ZegoMediaPlayerVideoPixelFormatI420     = 1,
    ZegoMediaPlayerVideoPixelFormatNV12     = 2,
    ZegoMediaPlayerVideoPixelFormatNV21     = 3,
    ZegoMediaPlayerVideoPixelFormatBGRA32   = 4,
    ZegoMediaPlayerVideoPixelFormatRGBA32   = 5,
    ZegoMediaPlayerVideoPixelFormatARGB32   = 6,
    ZegoMediaPlayerVideoPixelFormatABGR32   = 7,
};

typedef NS_ENUM(NSInteger, ZegoMediaPlayerIndex) {
    ZegoMediaPlayerIndexFirst   = 0,
    ZegoMediaPlayerIndexSecond  = 1,
    ZegoMediaPlayerIndexThird   = 2,
    ZegoMediaPlayerIndexFourth  = 3,
};

typedef NS_ENUM(NSInteger, ZegoMediaPlayerAudioChannel) {
    ZegoMediaPlayerAudioChannelLeft = 1,
    ZegoMediaPlayerAudioChannelRight = 2,
    ZegoMediaPlayerAudioChannelAll = 3
};

/**
 媒体播放器错误码
 */
typedef NS_ENUM(NSInteger, ZegoMediaPlayerError) {
    /** ok */
    ZegoMediaPlayerErrorOK = 0,
    /** 文件格式不支持 */
    ZegoMediaPlayerErrorFile = -1,
    /** 路径不存在 */
    ZegoMediaPlayerErrorPath = -2,
    /** 文件无法解码 */
    ZegoMediaPlayerErrorCodec = -3,
    /** 文件中没有可播放的音视频流 */
    ZegoMediaPlayerErrorNoSupportStream = -4,
    /** 文件解析过程中出现错误 */
    ZegoMediaPlayerErrorDemux = -5,
    /** 播放掉帧 */
    ZegoMediaPlayerErrorFrameDrop = -6,
    /** 加载资源超时 */
    ZegoMediaPlayerErrorLoadTimeout = -7
};


struct ZegoMediaPlayerVideoDataFormat
{
    int width;
    int height;
    int strides[4];
    int rotation;
    long pts;
    ZegoMediaPlayerVideoPixelFormat pixelFormat;
};

#endif /* zego_api_mediaplayer_defines_oc_h */
