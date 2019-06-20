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


- (bool)setReliableMessageDelegate:(id<ZegoReliableMessageDelegate>)delegate;

/**
 发送可靠业务消息
 
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

- (void)onRecvReliableMessage:(ZegoReliableMessage *)message room:(NSString *)roomId;

- (void)onUpdateReliableMessageInfo:(NSArray<ZegoReliableMessageInfo *> *)messageInfoList room:(NSString *)roomId;

@end
