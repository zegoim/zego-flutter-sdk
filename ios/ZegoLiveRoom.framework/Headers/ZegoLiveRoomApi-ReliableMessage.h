//
//  ZegoLiveRoomApi-ReliableMessage.h
//  ZegoLiveRoom
//
//  Copyright © 2018年 zego. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ZegoLiveRoomApi.h"
#import "ZegoLiveRoomApiDefines-ReliableMessage.h"

@protocol ZegoReliableMessageDelegate;

typedef void(^ZegoSendReliableMessageCompletionBlock)(int errorCode, NSString *roomId, NSString *msgType, NSUInteger msgSeq);

typedef void(^ZegoGetReliableMessageCompletionBlock)(int errorCode, NSString *roomId, NSArray<ZegoReliableMessage*> *messageList);

@interface ZegoLiveRoomApi (ReliableMessage)


/**
 设置可靠消息代理对象

 * 当需要使用房间内业务消息和业务状态同步功能时，需要使用可靠消息。
 
 * 注意：
 * 1.建议在登录房间之前设置，否则无法进行相关回调。
 * 2.未设置代理对象，或代理对象未实现相关代理方法，会导致无法进行相关回调。
 * 3.SDK 内部弱引用该代理对象。
 
 @param delegate 遵循 ZegoReliableMessageDelegate 协议的代理对象
 @return true 设置成功，false 设置失败
 */
- (bool)setReliableMessageDelegate:(id<ZegoReliableMessageDelegate>)delegate;

/**
 发送可靠业务消息
 
 * 可以调用该 API 发送房间内业务消息和业务状态的同步消息。
 * 调用该 API 后，房间的其他用户可以通过回调 [ZegoReliableMessageDelegate -onRecvReliableMessage:room:] 收到该条消息。
 
 * 注意：
 * 1.在登录房间后调用该 API 才有效。
 * 2.可靠消息是通过版本控制思想来实现的，通过 type 来唯一定义一种消息。服务端通过 -sendReliableMessage:type:latestSeq:completion: 请求的 latestSeq 和服务端维护的该 type 消息的 latestSeq 的比对，来判断该类型消息是否需要同步。如果客户端 latestSeq 小于服务端 latestSeq，则说明客户端的消息版本太低，则回调一个错误码，客户端据此需要去获取最新消息进行同步。如果客户端 latestSeq 大于或等于服务端 latestSeq，则服务端认为可以同步，然后服务端 latestSeq 更新为请求的 latestSeq，并发消息给所有房间内其他成员。因此当开发者需要维护当前 type 消息的本地 latestSeq，当调用同步方法 -sendReliableMessage:type:latestSeq:completion: 回调时传入 latestSeq+1 成功后需要将更新本地 latestSeq，回调失败则需要拉取最新的可靠消息，更新本地 latestSeq。
 
 @param content 可靠业务消息数据，不能超过 2048 字节, 允许为空字符串
 @param type 可靠业务消息类型，不能超过 128 字节, 不允许为空字符串，一个房间内只允许不超过10个不同的消息类型
 @param seq 当前最新的业务消息seq
 @param completionBlock 发送可靠业务消息结果
 @return true 成功，false 失败
 */
- (bool)sendReliableMessage:(NSString *)content type:(NSString *)type latestSeq: (unsigned int)seq completion:(ZegoSendReliableMessageCompletionBlock)completionBlock;


/**
 获取可靠业务消息

 @param messageTypes 可靠业务消息类型列表
 @param completionBlock 获取可靠业务消息结果
 @return true 成功，false 失败
 @discussion messageTypes的个数不能超过10个
 */
- (bool)getReliableMessages:(NSArray<NSString*> *)messageTypes completion:(ZegoGetReliableMessageCompletionBlock)completionBlock;

@end

@protocol ZegoReliableMessageDelegate <NSObject>

@optional

/**
 收到房间内的可靠消息
 
 * 调用 -sendRoomMessage:type:category:priority:completion: 发送消息后，会触发房间内其他用户进行该回调。
 
 @param message 可靠消息
 @param roomId 房间 ID
 */
- (void)onRecvReliableMessage:(ZegoReliableMessage *)message room:(NSString *)roomId;

- (void)onUpdateReliableMessageInfo:(NSArray<ZegoReliableMessageInfo *> *)messageInfoList room:(NSString *)roomId;

@end
