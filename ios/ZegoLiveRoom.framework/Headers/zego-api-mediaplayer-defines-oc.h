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
    ZegoMediaPlayerIndexFirst  = 0,
    ZegoMediaPlayerIndexSecond = 1,
    ZegoMediaPlayerIndexThird  = 2,
    ZegoMediaPlayerIndexFourth = 3,
};

struct ZegoMediaPlayerVideoDataFormat
{
    int width;
    int height;
    int strides[4];
    int rotation;
    ZegoMediaPlayerVideoPixelFormat pixelFormat;
};

#endif /* zego_api_mediaplayer_defines_oc_h */
