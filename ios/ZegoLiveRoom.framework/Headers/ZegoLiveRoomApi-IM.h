//
//  ZegoLiveRoomApi.h
//  ZegoLiveRoom
//
//  Copyright © 2017年 zego. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ZegoLiveRoomApi.h"
#import "ZegoLiveRoomApiDefines-IM.h"


@protocol ZegoIMDelegate;

typedef void(^ZegoRoomMessageCompletion)(int errorCode, NSString *roomId, unsigned long long messageId);
typedef void(^ZegoBigRoomMessageCompletion)(int errorCode, NSString *roomId, NSString *messageId);

@interface ZegoLiveRoomApi (IM)

/**
 设置 IM 代理对象
 
 * 注意：
 * 1.建议在登录房间之前设置，否则无法进行相关回调。
 * 2.需要使用点赞主播、评论、送礼、房间内成员列表变化通知等 IM 功能时，需要设置该类型代理对象。未设置代理对象，或代理对象未实现相关代理方法，会导致无法进行相关回调。
 * 3.SDK 内部弱引用该代理对象。
 
 @param imDelegate 遵循 ZegoIMDelegate 协议的代理对象
 @return 设置结果。true 成功，false 失败
 */
- (bool)setIMDelegate:(id<ZegoIMDelegate>)imDelegate;

/**
 房间发送广播消息
 
 @param content 消息内容, 不超过 512 字节
 @param type 消息类型，可以自定义
 @param category 消息分类，可以自定义
 @param priority 消息优先级, deprecated, 由 SDK 内部确定优先级
 @param completionBlock 消息发送结果，回调 server 下发的 messageId
 @return true 成功，false 失败
 @discussion 实现点赞主播、评论、送礼物等 IM 功能时，需要调用本 API
 @warning Deprecated 请使用 [ZegoLiveRoomApi sendRoomMessage:type:category:completion:]
 */
- (bool)sendRoomMessage:(NSString *)content type:(ZegoMessageType)type category:(ZegoMessageCategory)category priority:(ZegoMessagePriority)priority completion:(ZegoRoomMessageCompletion)completionBlock;

/**
 房间发送广播消息
 
 * 可以调用该 API 发送点赞主播、评论、送礼物等消息。
 
 * 注意：
 * 1.在登录房间后调用该 API 才有效。
 * 2.调用该 API 后，房间的其他用户可以通过回调 [ZegoIMDelegate -onRecvRoomMessage:messageList:] 收到该条消息。
 
 @param content 消息内容, 不超过 512 字节
 @param type 消息类型，可以自定义。详见 ZegoMessageType
 @param category 消息分类，可以自定义。详见 ZegoMessageCategory
 @param completionBlock 消息发送结果回调。回调信息包含 server 下发的 messageId
 @return true 成功，false 失败
 */
- (bool)sendRoomMessage:(NSString *)content type:(ZegoMessageType)type category:(ZegoMessageCategory)category completion:(ZegoRoomMessageCompletion)completionBlock;

/**
 房间发送不可靠信道的消息
 
 * 用于高并发的场景，消息可能被丢弃，当高并发达到极限时会根据策略丢弃部分消息。
 
 * 注意：
 * 1.在登录房间后调用该 API 才有效。
 * 2.调用该 API 后，房间的其他用户可以通过回调 [ZegoIMDelegate -onRecvBigRoomMessage:messageList:] 收到该条消息。
 
 @param content 消息内容, 不超过 512 字节
 @param type 消息类型，可以自定义。详见 ZegoMessageType
 @param category 消息分类，可以自定义。详见 ZegoMessageCategory
 @param completionBlock 消息发送结果回调。回调信息包含 server 下发的 messageId
 @return true 成功，false 失败
 */
- (bool)sendBigRoomMessage:(NSString *)content type:(ZegoMessageType)type category:(ZegoMessageCategory)category completion:(ZegoBigRoomMessageCompletion)completionBlock;

@end


/**
 即构 IM 代理协议
 */
@protocol ZegoIMDelegate <NSObject>

@optional

/**
 房间成员更新回调
 
 * 注意：
 * 1.必须调用 [ZegoLiveRoomApi +setRoomConfig:userStateUpdate:] 开启用户状态（用户进入、退出房间）广播，当房间成员变化（用户进入、退出房间）时，才会触发此回调。
 
 @param userList 成员更新列表
 @param type  更新类型(增量，全量)，详见 ZegoUserUpdateType
 */
- (void)onUserUpdate:(NSArray<ZegoUserState *> *)userList updateType:(ZegoUserUpdateType)type;

/**
 收到房间的广播消息
 
 * 调用 -sendRoomMessage:type:category:priority:completion: 发送消息后，会触发房间内其他用户进行该回调。
 
 @param roomId 房间 Id
 @param messageList 消息列表，包括消息内容，消息分类，消息类型，发送者等信息
 */
- (void)onRecvRoomMessage:(NSString *)roomId messageList:(NSArray<ZegoRoomMessage*> *)messageList;

/**
 收到在线人数更新
 
 * 默认为 30s 回调一次，开发者可联系即构技术支持，自定义回调频率。
 
 @param onlineCount 在线人数
 @param roomId 房间 Id
 */
- (void)onUpdateOnlineCount:(int)onlineCount room:(NSString *)roomId;

/**
 收到房间的不可靠消息广播
 
 * 调用 -sendBigRoomMessage:type:category:completion: 发送消息后，会触发房间内其他用户进行该回调。
 
 @param roomId 房间 Id
 @param messageList 消息列表，包括消息内容，消息分类，消息类型，发送者等信息
 */
- (void)onRecvBigRoomMessage:(NSString *)roomId messageList:(NSArray<ZegoBigRoomMessage*> *)messageList;

@end
