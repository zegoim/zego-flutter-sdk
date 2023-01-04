//
//  ZegoLiveRoomApi-RoomExtraInfo.h
//
//  Copyright © 2020年 zego. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ZegoLiveRoomApi.h"
#import "ZegoLiveRoomApiDefines-RoomExtraInfo.h"

@protocol ZegoLiveRoomExtraInfoDelegate;

typedef void(^ZegoSetRoomExtraInfoCompletionBlock)(int errorCode, NSString *roomId,NSString *key);

@interface ZegoLiveRoomApi (RoomExtraInfo)

/**
 设置房间附加信息回调
 
 @param RoomExtraInfoDelegate 遵循 ZegoLiveRoomExtraInfoDelegate 协议的代理对象
 @return true 成功，false 失败
 @discussion 
 */
- (bool)setRoomExtraInfoUpdateDelegate:(id<ZegoLiveRoomExtraInfoDelegate>) RoomExtraInfoDelegate;

/**
 设置房间附加信息内容 登录房间成功后使用

 * 注意：
 * 1.不支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启）。多房间模式下请使用 [ZegoLiveRoomApi setRoomExtraInfo:value:room:completion:] 指定房间 ID。
 
 @param key 设置的key值 不能超过 10字节, 不允许为空字符串, 一个房间内只允许1个消息类型
 @param value 设置的value值 不能超过 128 字节, 允许为空字符串
 @param completionBlock 回调
 @return true 成功，false 失败
 */
- (bool)setRoomExtraInfo:(NSString*)key value:(NSString*)value completion:(ZegoSetRoomExtraInfoCompletionBlock)completionBlock;


/**
 设置房间附加信息内容 登录房间成功后使用
 
 * 注意：
 * 1.支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启）。

 @param key 设置的key值 不能超过 10字节, 不允许为空字符串, 一个房间内只允许1个消息类型
 @param value 设置的value值 不能超过 128 字节, 允许为空字符串
 @param roomId 房间 ID
 @param completionBlock 回调
 @return true 成功，false 失败
 */
- (bool)setRoomExtraInfo:(NSString*)key value:(NSString*)value room:(NSString*)roomId completion:(ZegoSetRoomExtraInfoCompletionBlock)completionBlock;

@end


@protocol ZegoLiveRoomExtraInfoDelegate <NSObject>

@optional

/**
 房间附加信息更新回调 全量回调当前房间内最新的extrainfo信息(注意列表为空时表示数据已被删除)
 
 @param roomId 回调的roomid
 @param roomExtraInfoList 回调的列表
 */
- (void)onRoomExtraInfoUpdated:(NSString *)roomId roomExtraInfoList:(NSArray<ZegoRoomExtraInfo *> *)roomExtraInfoList;

@end
