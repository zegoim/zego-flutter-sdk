//
//  ZegoLiveRoomApiDefines-ReliableMessage.h
//  ZegoLiveRoom
//
//  Copyright © 2020年 zego. All rights reserved.
//

#ifndef ZegoLiveRoomApiDefines_RoomExtraInfo_h
#define ZegoLiveRoomApiDefines_RoomExtraInfo_h

/** 可靠业务消息 */
@interface ZegoRoomExtraInfo : NSObject

/** 消息类型 */
@property (nonatomic, copy) NSString    *key;
/** 消息内容 */
@property (nonatomic, copy) NSString    *value;
/** 发消息用户的ID */
@property (nonatomic, copy) NSString    *userID;
/** 发消息用户的名字 */
@property (nonatomic, copy) NSString    *userName;
/** 发送时间 */
@property (nonatomic, assign) unsigned long long  updateTime;

@end

#endif /* ZegoLiveRoomApiDefines_RoomExtraInfo_h */