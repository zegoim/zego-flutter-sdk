//
//  ZegoLiveRoomApiDefines.h
//  ZegoLiveRoom
//
//  Copyright © 2017年 zego. All rights reserved.
//

#ifndef ZegoLiveRoomApiDefines_h
#define ZegoLiveRoomApiDefines_h

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#define ZEGOView UIView
#define ZEGOImage UIImage
#elif TARGET_OS_OSX
#import <AppKit/AppKit.h>
#define ZEGOView NSView
#define ZEGOImage NSImage
#endif

#import "zego-api-defines-oc.h"

#ifdef __cplusplus
#define ZEGO_EXTERN     extern "C"
#else
#define ZEGO_EXTERN     extern
#endif


/** 成员角色 */
typedef enum
{
    /** 主播 */
    ZEGO_ANCHOR = 1,
    /** 观众 */
    ZEGO_AUDIENCE = 2,
} ZegoRole;

/** 流变更类型 */
typedef enum
{
    /** 新增流 */
    ZEGO_STREAM_ADD     = 2001,
    /** 删除流 */
    ZEGO_STREAM_DELETE  = 2002,
} ZegoStreamType;

/** 流变更类型 */
typedef enum
{
    /** 无效值 */
    ZEGO_STREAM_DELETE_NOT_SUPPORT = -1,  
    /** 推流用户,主动关闭 */
    ZEGO_STREAM_DELETE_NORMAL = 0, 
    /** 推流用户, 心跳超时,导致流关闭 */
    ZEGO_STREAM_DELETE_HEARTBEAT_TIMEOUT =1, 
    /** 推流用户,重复登录,导致流关闭 */
    ZEGO_STREAM_DELETE_REPEAT_LOGIN = 2,  
    /** 推流用户被T,导致流关闭 */
    ZEGO_STREAM_DELETE_KICKOUT = 3, 
    /** 推流用户掉线,导致流关闭*/       
    ZEGO_STREAM_DELETE_OFFLINE = 4,  
    /** 该流被服务器关闭*/   
    ZEGO_STREAM_DELETE_SERVER = 100,
    /** 未知原因 (一般是由于SDK 未正常收到流删除推送导致)*/  
    ZEGO_STREAM_DELETE_UNKOWN = 1000, 
} ZegoAPIStreamDeleteReason;

/** 网络质量 */
typedef enum
{
    /** 未知 */
    ZEGO_LIVE_QUALITY_UNKNOWN     = -1,
    /** 极好 */
    ZEGO_LIVE_QUALITY_EXCELLENT   = 0,
    /** 好 */
    ZEGO_LIVE_QUALITY_GOOD        = 1,
    /** 中等 */
    ZEGO_LIVE_QUALITY_MIDDLE      = 2,
    /** 差 */
    ZEGO_LIVE_QUALITY_POOR        = 3,
    /** 极差 */
    ZEGO_LIVE_QUALITY_DIE         = 4,
} ZegoLiveQuality;

/** 流信息 */
@interface ZegoStream : NSObject

/** 用户 ID */
@property (nonatomic, copy) NSString *userID;
/** 用户名 */
@property (nonatomic, copy) NSString *userName;
/** 流 ID */
@property (nonatomic, copy) NSString *streamID;
/** 流附加信息 */
@property (nonatomic, copy) NSString *extraInfo;
/** 流在房间中的序号 */
@property (nonatomic, assign) int streamNID;
/** 流删除原因,当前流删除原因 (只在sdk 回调onStreamUpdated 且type类型为流删除时有效)*/
@property (nonatomic, assign) ZegoAPIStreamDeleteReason streamDeleteReason;
@end

/** 房间信息 */
@interface ZegoAPIRoomInfo : NSObject
/** 房间的sessionid */
@property (nonatomic, assign) unsigned long long roomSessionID;

@end


typedef void(^ZegoSnapshotCompletionBlock)(ZEGOImage* img);

/**
 推流参数
 */
@interface ZegoAPIPublishStreamParams : NSObject
/** 流 ID */
@property (nonatomic, copy) NSString *streamID;
/** 流标题 */
@property (nonatomic, copy) NSString *streamTitle;
/** 推流附加参数 */
@property (nonatomic, copy) NSString *publishParams;
/** 关联的房间 ID. 开启 enableMultiRoom 之后有效, 当用户退出该房间时,会停止拉这条流 */
@property (nonatomic, copy) NSString *roomID;
/** 直播属性，参考 ZegoApiPublishFlag 定义 */
@property (nonatomic, assign) int publishFlag;
/** 推流通道号 */
@property (nonatomic, assign) ZegoAPIPublishChannelIndex channelIndex;
/** 推流是否同步网络时间,仅RTC流有效，与setStreamAlignmentProperty 接口配合使用, 用于混流时各路流的对齐, 常用于KTV场景。*/
@property (nonatomic, assign) int forceSynchronousNetworkTime;

@end

/**
 拉流参数
 */
@interface ZegoAPIPlayStreamParams : NSObject
/** 需要播放的用户流 ID */
@property (nonatomic, copy) NSString *streamID;
/** 用来渲染播放视频的视图，设置为空时不显示视频，只播放音频 */
@property (nonatomic, strong) ZEGOView *view;
/** 多媒体流附加信息 */
@property (nonatomic, copy) ZegoAPIStreamExtraPlayInfo* extraInfo;
/** 关联的房间 ID. 开启 enableMultiRoom 之后有效, 当用户退出该房间时,会停止拉这条流 */
@property (nonatomic, copy) NSString *roomID;

@end

/**
 房间模式
 */
typedef NS_ENUM(NSInteger, ZegoAPIRoomMode)
{
    /** 单房间模式 */
    ZEGOAPI_ROOM_MODE_SINGLE_ROOM = 0,
    /** 多房间模式 */
    ZEGOAPI_ROOM_MODE_MULTI_ROOM = 1,
};


#endif /* ZegoLiveRoomApiDefines_h */
