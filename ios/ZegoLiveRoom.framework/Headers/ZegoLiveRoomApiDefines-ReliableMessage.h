//
//  ZegoLiveRoomApiDefines-ReliableMessage.h
//  ZegoLiveRoom
//
//  Copyright © 2018年 zego. All rights reserved.
//

#ifndef ZegoLiveRoomApiDefines_ReliableMessage_h
#define ZegoLiveRoomApiDefines_ReliableMessage_h

/** 可靠业务消息 */
@interface ZegoReliableMessage : NSObject

/** 消息类型 */
@property (nonatomic, copy) NSString    *type;
/** 消息序号 */
@property (nonatomic, assign) unsigned int  latestSeq;
/** 消息内容 */
@property (nonatomic, copy) NSString    *content;
/** 发消息用户的ID */
@property (nonatomic, copy) NSString    *fromUserId;
/** 发消息用户的名字 */
@property (nonatomic, copy) NSString    *fromUserName;
/** 发送时间 */
@property (nonatomic, assign) unsigned long long  sendTime;

@end

/** 可靠业务信息 */
@interface ZegoReliableMessageInfo : NSObject

/** 消息类型 */
@property (nonatomic, copy) NSString    *type;
/** 消息序号 */
@property (nonatomic, assign) unsigned int latestSeq;

@end

#endif /* ZegoLiveRoomApiDefines_ReliableMessage_h */
