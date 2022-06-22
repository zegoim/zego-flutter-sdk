//
//  ZegoLiveRoomApi.h
//  ZegoLiveRoom
//
//  Copyright © 2017年 zego. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ZegoLiveRoomApiDefines.h"
#import "ZegoLiveRoomApiDefines-IM.h"
#import "zego-api-defines-oc.h"

@protocol ZegoRoomDelegate;
@protocol ZegoLiveEventDelegate;
@protocol ZegoDeviceEventDelegate;
@protocol ZegoAVEngineDelegate;
@protocol ZegoLogInfoDelegate;
@protocol ZegoAudioRouteDelegate;
@protocol ZegoNetTypeDelegate;
@protocol ZegoRealtimeSequentialDataDelegate;

typedef void(^ZegoInitSDKCompletionBlock)(int errorCode);
typedef void(^ZegoLoginCompletionBlock)(int errorCode, NSArray<ZegoStream*> *streamList);
typedef void(^ZegoResponseBlock)(int result, NSString *fromUserID, NSString *fromUserName);
typedef void(^ZegoCustomCommandBlock)(int errorCode, NSString *roomID);
typedef void(^ZegoLogHookBlock)(NSString *logMessage);

typedef void(^ZegoRunLoopObserveBlock)(long taskId, ZegoAPITaskType type, int taskDispatchTime, int taskRunTime, int taskTotalTime);

typedef void(^ZegoExperimentalAPIBlock)(NSString *params);

@interface ZegoLiveRoomApi : NSObject

/**
 ZegoLiveRoom SDK 版本号
 
 @return 版本号
 @discussion 开发者使用本 API 获取 SDK 版本号
 */
+ (NSString *)version;

/**
 版本号2
 
 @return 版本号2
 */
+ (NSString *)version2;

/**
 hook sdk 日志
 
 @param  logHookblk 设置日志hook
 @discussion 1、回调信息为加密信息(需要zego解密工具解密)
             2、设置此日志回调之后sdk 将不会在写日志文件
             3、调用时机,应该最早时机调用此接口 InitSDK之前
 */
+ (void)setLogHook:(ZegoLogHookBlock)logHookblk;

/**
 是否启用测试环境
 
 @param useTestEnv 是否启用测试环境，true 启用，false 不启用。默认为 false
 @discussion 建议在初始化 SDK 前调用。建议开发者在开发阶段设置为测试环境，使用由 Zego 提供的测试环境。上线前需切换为正式环境运营
 */
+ (void)setUseTestEnv:(bool)useTestEnv;

/**
 是否打开调试信息
 
 @param bOnVerbose 是否打开调试信息，true 打开，false 不打开。默认为 false
 @discussion 建议setLogDir后 initSDK前调用 , 启用后,控制台会输出用户调用信息，并且会在SDK日志目录生成 明文的用户可见的日志文件
 */
+ (void)setVerbose:(bool)bOnVerbose;

/**
 设置业务类型
 
 @param type 业务类型，取值 0（直播类型）或 2（实时音视频类型）。默认为 0
 @discussion 确保在创建接口对象前调用
 */
+ (void)setBusinessType:(int)type;

/**
 是否使用聊天室功能
 
 @param bChatRoom 是否使用聊天室功能，true 使用，false 不使用。默认为 false
 @discussion zegoliveroom 自带 IM 功能，随 SDK 初始化。如果要额外使用聊天室，需要启用聊天室功能
 */
//+ (void)setUseChatRoom:(bool)bChatRoom;

/**
 上报日志
 
 @discussion 上传日志到后台便于分析问题
 @discussion 在初始化 SDK 成功后调用
 */
+ (void)uploadLog;

/**
 设置用户 ID 及用户名
 
 @param userID 用户 ID，不可为空
 @param userName 用户名，不可为空
 @return true 成功，false 失败
 @discussion 确保在 loginRoom 前设置成功。userID 和 userName 由业务方自己控制
 */
+ (bool)setUserID:(NSString *)userID userName:(NSString *)userName;


/**
 设置SDK日志大小

 @param size 单个日志大小，单位为字节。取值范围[1 * 1024 * 1024, 100 * 1024 * 1024],推荐 5 * 1024 * 1024 及以上. 当设置为 0 时，不写日志（不推荐，当 SDK 出问题时无法定位原因）
 @discussion 在初始化 SDK 之前调用有效
 @discussion SDK 默认单个日志文件大小为 5M
 */
+ (void)setLogSize:(unsigned int)size;

/**
 设置SDK日志存储目录及大小
 
 @param logDir 日志文件存储路径，如果为空，则存储到默认路径
 @param size 单个日志大小，单位为字节。取值范围[1 * 1024 * 1024, 100 * 1024 * 1024],推荐 5 * 1024 * 1024 及以上. 当设置为 0 时，不写日志（不推荐，当 SDK 出问题时无法定位原因）
 @param subFolder 日志存储子文件夹，当为空时，不创建子文件夹。该文件夹是 logDir 的子目录。
 @discussion 在初始化 SDK 之前调用有效
 */
+ (void)setLogDir:(NSString *)logDir size:(unsigned int)size subFolder:(NSString *)subFolder;

/**
 设置observe sdk主线程回调 

 @param blk 回调
 @discussion 回调值信息 taskId：sdk任务id  type: 任务类型 taskDispatchTime: 任务调度消耗时间 单位ms taskRunTime: 任务执行耗时 单位ms taskTotalTime: 任务总耗时 单位ms 一般情况只需关注此时间即可
 @discussion 建议每次 初始化SDK前调用, SDK 被反初始化会清除此回调值 
 */
+ (void)setRunLoopObserveDelegate:(ZegoRunLoopObserveBlock)blk;

/**
 设置房间模式

 @param mode 房间模式, 详见 ZegoAPIRoomMode 定义
 @return true 成功, false 失败
 @discussion 默认是单房间模式
 @discussion 必须在初始化SDK前调用。
 @discussion 如果需要开启多房间模式，请与即构技术支持联系配置服务端支持。
 */
+ (bool)setRoomMode:(ZegoAPIRoomMode)mode;

/**
 初始化 SDK
 
 @param appID  Zego 派发的数字 ID, 开发者的唯一标识
 @param appSignature  Zego 派发的签名, 用来校验对应 appID 的合法性
 @return SDK 实例，nil 表示初始化失败
 @discussion 初始化 SDK 时调用。初始化 SDK 失败可能导致 SDK 功能异常
 */
- (instancetype)initWithAppID:(unsigned int)appID appSignature:(NSData*)appSignature;

/**  
 初始化 SDK
 
 @param appID  Zego 派发的数字 ID, 开发者的唯一标识
 @param appSignature  Zego 派发的签名, 用来校验对应 appID 的合法性
 @param blk 回调 block
 @return SDK 实例，nil 表示初始化失败
 @discussion 初始化 SDK 时调用。初始化 SDK 失败可能导致 SDK 功能异常
 */
- (instancetype)initWithAppID:(unsigned int)appID appSignature:(NSData*)appSignature completionBlock:(ZegoInitSDKCompletionBlock)blk;

/**
 设置 room 代理对象
 
 @param roomDelegate 遵循 ZegoRoomDelegate 协议的代理对象
 @return true 成功，false 失败
 @discussion 使用 room 功能，初始化相关视图控制器时需要设置代理对象。未设置代理对象，或对象设置错误，可能导致无法正常收到相关回调
 */
- (bool)setRoomDelegate:(id<ZegoRoomDelegate>) roomDelegate;

/**
 设置房间配置信息
 
 @param audienceCreateRoom 观众是否可以创建房间。true 可以，false 不可以。默认 true
 @param userStateUpdate 用户状态（用户进入、退出房间）是否广播。true 广播，false 不广播。默认 false
 @return true 成功，false 失败
 @discussion 在 userStateUpdate 为 true 的情况下，用户进入、退出房间会触发 [ZegoLiveRoomApi (IM) -onUserUpdate:updateType:] 回调
 @discussion 在登录房间前调用有效，退出房间后失效
 @discussion userStateUpdate为房间属性而非用户属性，设置的是该房间内是否会进行用户状态的广播。如果需要在房间内用户状态改变时，其他用户能收到通知，请为所有用户设置为true；反之，设置为false。设置为true后，方可从OnUserUpdate回调收到用户状态变更通知
 @discussion 不支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启），多房间模式下请使用 [ZegoLiveRoomApi setRoomConfig:userStateUpdate:room:]，指定房间 ID。
 */
- (bool)setRoomConfig:(bool)audienceCreateRoom userStateUpdate:(bool)userStateUpdate;

/**
 设置房间配置信息
 
 @param audienceCreateRoom 观众是否可以创建房间。true 可以，false 不可以。默认 true
 @param userStateUpdate 用户状态（用户进入、退出房间）是否广播。true 广播，false 不广播。默认 false
 @param roomID 房间 ID
 @return true 成功，false 失败
 @discussion 在 userStateUpdate 为 true 的情况下，用户进入、退出房间会触发 [ZegoLiveRoomApi (IM) -onUserUpdate:updateType:] 回调
 @discussion 在登录房间前调用有效，退出房间后失效
 @discussion userStateUpdate为房间属性而非用户属性，设置的是该房间内是否会进行用户状态的广播。如果需要在房间内用户状态改变时，其他用户能收到通知，请为所有用户设置为true；反之，设置为false。设置为true后，方可从OnUserUpdate回调收到用户状态变更通知
 @discussion 支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启）。
 */
- (bool)setRoomConfig:(bool)audienceCreateRoom userStateUpdate:(bool)userStateUpdate room:(NSString *)roomID;

/**
 设置自定义token信息
 
 @warning Deprecated, 请使用 [ZegoLiveRoomApi setToken:room:] 代替
 
 @param thirdPartyToken 第三方传入的token
 @return true 成功，false 失败
 @discussion 使用此方法验证登录及推流时用户的合法性，登录房间前调用，token的生成规则请联系即构。若不需要验证用户合法性，不需要调用此函数。
 @discussion 在登录房间前调用有效，支持在登录房间后重新设置，退出房间后失效
 */
- (bool)setCustomToken:(NSString *)thirdPartyToken;

/**
 设置鉴权 token 信息
 
 @param token 鉴权 token
 @param roomID 房间 ID
 @return true 成功，false 失败
 @discussion 使用此方法验证登录及推流时用户的合法性，登录房间前调用，token的生成规则请联系即构。若不需要验证用户合法性，不需要调用此函数。
 @discussion 在登录房间前调用有效，支持在登录房间后重新设置，退出房间后失效
 */
- (bool)setToken:(NSString *)token room:(NSString *)roomID;

/**
 设置房间最大在线人数
 
 @param userCount 最大人数
 @return true 成功，false 失败
 @discussion 在登录房间前调用有效，退出房间后失效
 @discussion 不支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启），多房间模式下请使用 [ZegoLiveRoomApi setRoomMaxUserCount:room:]，指定房间 ID。
 */
- (bool)setRoomMaxUserCount:(unsigned int)userCount;

/**
 设置房间最大在线人数
 
 @param userCount 最大人数
 @param roomID 房间 ID
 @return true 成功，false 失败
 @discussion 在登录房间前调用有效，退出房间后失效
 @discussion 支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启）。
 */
- (bool)setRoomMaxUserCount:(unsigned int)userCount room:(NSString *)roomID;

/**
 登录房间
 
 @param roomID 房间 ID，长度不可超过 255 byte
 @param role 成员角色，可取值为 ZEGO_ANCHOR（主播），ZEGO_AUDIENCE（观众），详见 ZegoRole 定义
 @param blk 回调 block
 @return true 成功，false 失败
 @discussion 登录房间成功，才能开始直播。观众登录房间成功后，会在 blk 中返回当前房间的流信息
 */
- (bool)loginRoom:(NSString *)roomID role:(int)role withCompletionBlock:(ZegoLoginCompletionBlock)blk;

/**
 登录房间
 
 @param roomID 房间 ID，长度不可超过 255 byte
 @param roomName 房间名称，可选，长度不可超过 255 byte
 @param role 成员角色，可取值为 ZEGO_ANCHOR（主播），ZEGO_AUDIENCE（观众），详见 ZegoRole 定义
 @param blk 回调 block
 @return true 成功，false 失败
 @discussion 登录房间成功，才能开始直播。观众登录房间成功后，会在 blk 中返回当前房间的流信息
 */
- (bool)loginRoom:(NSString *)roomID roomName:(NSString *)roomName role:(int)role withCompletionBlock:(ZegoLoginCompletionBlock)blk;

/**
 切换房间 调用成功 会停止推拉流 (登录房间成功后，需要快速切换到其它房间时使用,也会停止MultiRoom的拉流) 
 
 @param roomID 房间 ID，长度不可超过 255 byte
 @param roomName 房间名称，可选，长度不可超过 255 byte
 @param role 成员角色，可取值为 ZEGO_ANCHOR（主播），ZEGO_AUDIENCE（观众），详见 ZegoRole 定义
 @param blk 回调 block
 @return true 成功，false 失败
 @discussion 切换房间成功，才能开始直播。观众登录房间成功后，会在 blk 中返回当前房间的流信息
 @discussion 不支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启）。
 */
- (bool)switchRoom:(NSString *)roomID roomName:(NSString *)roomName role:(int)role withCompletionBlock:(ZegoLoginCompletionBlock)blk;

/**
 退出房间
 
 @return true 成功，false 失败
 @discussion 连麦情况下，要 stop 所有的 stream 后，才能执行 logoutRoom。
 @discussion 不支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启）。多房间模式下请使用 [ZegoLiveRoomApi logoutRoom:] 指定房间 ID。
 */
- (bool)logoutRoom;

/**
 退出房间
 
 @param roomID 房间 ID
 @return true 成功，false 失败
 @discussion 连麦情况下，要 stop 所有的 stream 后，才能执行 logoutRoom。
 @discussion 支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启）。
 */
- (bool)logoutRoom:(NSString *)roomID;

/**
 发送自定义信令
 
 * 该 API 可以向指定列表内的用户发送自定义信令，信令内容由用户自定义。发送结果通过 block 回调。
 * 用户可通过代理 [ZegoRoomDelegate -onReceiveCustomCommand:userName:content:roomID:] 方法收到信令。
 * 信令不能保证 100% 可达。
 * 不支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启）。多房间模式下请使用 [ZegoLiveRoomApi sendCustomCommand:content:room:completion:] 指定房间 ID。
 
 @param memberList 发送对象列表
 @param content 消息内容。长度不超过 1024 字节
 @param block 消息发送结果
 @return 发起请求是否成功
 */
- (bool)sendCustomCommand:(NSArray<ZegoUser*> *)memberList content:(NSString *)content completion:(ZegoCustomCommandBlock)block;

/**
 发送自定义信令
 
 * 该 API 可以向指定列表内的用户发送自定义信令，信令内容由用户自定义。发送结果通过 block 回调。
 * 用户可通过代理 [ZegoRoomDelegate -onReceiveCustomCommand:userName:content:roomID:] 方法收到信令。
 * 信令不能保证 100% 可达。
 * 支持多房间模式（通过 [ZegoLiveRoomApi setRoomMode:] 开启）。必须指定房间 ID
 
 @param memberList 发送对象列表
 @param content 消息内容。长度不超过 1024 字节
 @param roomID 房间 ID
 @param block 消息发送结果
 @return 发起请求是否成功
 */
- (bool)sendCustomCommand:(NSArray<ZegoUser*> *)memberList content:(NSString *)content room:(NSString *)roomID completion:(ZegoCustomCommandBlock)block;

/**
 设置直播事件代理对象
 
 * 注意：该代理用于监听直播事件回调，设置代理对象后在 [ZegoLiveEventDelegate -zego_onLiveEvent:info:] 中获取直播事件。

 @param liveEventDelegate 遵循 ZegoLiveEventDelegate 协议的代理对象，SDK 内部会弱引用该对象。
 @return true 表示调用成功，false 表示调用失败。
 */
- (bool)setLiveEventDelegate:(id<ZegoLiveEventDelegate>)liveEventDelegate;

/**
 设置音视频设备错误回调代理对象
 
 @param deviceEventDelegate 遵循 ZegoDeviceEventDelegate 协议的代理对象
 @return true 成功，false 失败
 @discussion 开发者获取音视频设备错误，需要先设置此代理对象。未设置代理对象，或对象设置错误，可能导致无法正常收到相关回调
 */
- (bool)setDeviceEventDelegate:(id<ZegoDeviceEventDelegate>)deviceEventDelegate;

/**
 设置是否允许SDK使用麦克风设备
 
 * 调用时机为引擎创建后的任意时刻。
 * 接口由于涉及对设备的操作，极为耗时，不建议随便调用，只在真正需要让出麦克风给其他应用的时候才调用。
 
 @param enable YES 表示允许使用麦克风，NO 表示禁止使用麦克风，此时如果SDK在占用麦克风则会立即释放。
 @return YES 调用成功，NO 调用失败
 */
- (BOOL)enableMicDevice:(BOOL)enable;

#if TARGET_OS_IPHONE
/**
 暂停指定模块
 
 * 用于需要暂停指定模块的场合，例如来电时暂停音频模块。
 
 * 注意：
 * 1.可以在初始化后任意时间调用。
 * 2.暂停指定模块后，注意在合适时机下调用 [ZegoLiveRoomApi -resumeModule:] 恢复模块。

 @param moduleType 模块类型, 参考 ZegoAPIModuleType。
 */
- (void)pauseModule:(int)moduleType;

/**
 恢复指定模块
 
 * 用于暂停模块后需要恢复模块的场合，例如来电结束后恢复音频模块。
 
 * 注意：可以在初始化后任意时间调用。

 @param moduleType 模块类型, 参考 ZegoAPIModuleType。
 */
- (void)resumeModule:(int)moduleType;

/**
 设置音频路由回调
 
 @param delegate 回调
 */
- (bool)setAudioRouteDelegate:(id<ZegoAudioRouteDelegate>)delegate;

/**
 获取当前引擎状态
 @return 详见 ZegoAPIEngineState
 */
- (ZegoAPIEngineState)getEngineState;

#endif

#if TARGET_OS_OSX

/**
 设置视频设备
 
 @param deviceId 设备 Id
 @return true 成功，false 失败
 @discussion 本接口用于 Mac PC 端的业务开发
 */
+ (bool)setVideoDevice:(NSString *)deviceId;

/**
 设置音频设备
 
 @param deviceId 设备 Id
 @param deviceType 设备类型
 @return true 成功，false 失败
 @discussion 本接口用于 Mac PC 端的业务开发
 */
+ (bool)setAudioDevice:(NSString *)deviceId type:(ZegoAPIAudioDeviceType)deviceType;

/**
 获取音频设备列表
 
 @param deviceType 设备类型
 @return 设备信息列表
 */
- (NSArray<ZegoAPIDeviceInfo *> *)getAudioDeviceList:(ZegoAPIAudioDeviceType)deviceType;


/**
 获取视频设备支持分辨率列表 mac平台需要在initsdk 回调后方可调用
 
 @param deviceId 设备id
 @return 设备支持分辨率列表
 */
- (NSArray<ZegoAPIDeviceVideoCapabilityInfo *> *)getVideoDevCapabilityList:(NSString *)deviceId;

/**
 获取视频设备列表
 
 @return 设备信息列表
 */
- (NSArray<ZegoAPIDeviceInfo *> *)getVideoDeviceList;

/**
 系统声卡声音采集开关，开启会将第三方播放器的声音混入推流中的声音。
 单独开启此功能不会将 sdk播放的声音如：拉流的声音和媒体播放器的声音混入
 调用时机为initSDK后调用，可以在推流过程中更改设置
 
 @param enable 是否打开
 */
- (void)enableMixSystemPlayout:(bool)enable;

/**
 设置系统声卡采集的音量 
 
 @param  volume 音量, 有效范围[0, 200], 默认为 100
 @return true 表示设置成功, false 表示失败
 @note 在 SDK 生命周期内有效
 */
- (bool)setSystemPlayoutVolumeBeforeMixing:(int)volume;

/**
 获取麦克风音量
 
 @param deviceId 设备ID
 @return -1: 获取失败 0 ~100 麦克风音量
 @discussion 切换麦克风后需要重新获取麦克风音量
 */
- (int)getMicDeviceVolume:(NSString *)deviceId;

/**
 设置麦克风音量
 
 @param deviceId 设备ID
 @param volume 音量 0 ~ 100
 */
- (void)setMicDevice:(NSString *)deviceId volume:(int)volume;

/**
 获取扬声器音量
 
 @param deviceId 设备ID
 @return -1: 获取失败 0 ~100 音量
 */
- (int)getSpeakerDeviceVolume:(NSString *)deviceId;

/**
 设置扬声器音量
 
 @param deviceId 设备ID
 @param volume 音量 0 ~ 100
 */
- (void)setSpeakerDevice:(NSString *)deviceId volume:(int)volume;

/**
 获取app中扬声器音量
 
 @param deviceId 设备ID
 @return -1: 获取失败 0 ~100 音量
 */
- (int)getSpeakerSimpleVolume:(NSString *)deviceId;

/**
 设置app中扬声器音量
 
 @param deviceId 设备ID
 @param volume 音量 0 ~ 100
 */
- (void)setSpeaker:(NSString *)deviceId simpleVolume:(int)volume;

/**
 获取扬声器是否静音
 
 @param deviceId 设备ID
 @return true 静音 false 非静音
 */
- (bool)getSpeakerDeviceMute:(NSString *)deviceId;

/**
 设置扬声器静音
 
 @param deviceId 设备ID
 @param mute 是否静音
 */
- (void)setSpeakerDevice:(NSString *)deviceId mute:(bool)mute;

/**
 获取麦克风是否静音
 
 @param deviceId 设备ID
 @return true 静音 false 非静音
 */
- (bool)getMicDeviceMute:(NSString *)deviceId;

/**
 设置麦克风静音
 
 @param deviceId 设备ID
 @param mute 是否静音
 */
- (void)setMicDevice:(NSString *)deviceId mute:(bool)mute;

/**
 获取app中扬声器是否静音
 
 @param deviceId 设备ID
 @return true 静音 false 非静音
 */
- (bool)getSpeakerSimpleMute:(NSString *)deviceId;

/**
 设置app中扬声器是否静音
 
 @param deviceId 设备ID
 @param mute 是否静音
 */
- (void)setSpeaker:(NSString *)deviceId simpleMute:(bool)mute;

/**
 获取默认的视频设备
 
 @return deviceId
 */
- (NSString *)getDefaultVideoDeviceId;

/**
 获取默认的音频设备
 
 @param deviceType 音频类型
 @return deviceId
 */
- (NSString *)getDefaultAudioDeviceId:(ZegoAPIAudioDeviceType)deviceType;

/**
 监听设备的音量变化
 
 @param deviceId 设备ID
 @param deviceType 设备类型
 @return 设置是否成功
 @discussion 设置后如果有音量变化（包括app音量）通过ZegoDeviceEventDelegate::zego_onDevice:error:回调
 */
- (bool)setAudioVolumeNotify:(NSString *)deviceId type:(ZegoAPIAudioDeviceType)deviceType;

/**
 停止监听设备的音量变化
 
 @param deviceId 设备ID
 @param deviceType 设备类型
 @return 设置是否成功
 */
- (bool)stopAudioVolumeNotify:(NSString *)deviceId type:(ZegoAPIAudioDeviceType)deviceType;

/**
 获取当前使用的音频设备信息

 @param deviceType 音频类型
 @return 音频设备信息
*/
- (ZegoAPIDeviceInfo *)getCurrentAudioDevice:(ZegoAPIAudioDeviceType)deviceType;

#endif

/**
 设置 AVEngine 代理对象
 @return true 成功, false 失败
 */
- (bool)setAVEngineDelegate:(id<ZegoAVEngineDelegate>)avEngineDelegate;

/**
 设置配置信息，如果没有特殊说明，必须确保在 InitSDK 前调用
 
 @param config 配置信息，如"keep_audio_session_active=true", 等号后面值的类型要看下面每一项的定义
 
 @discussion 具体配置信息请咨询技术支持
 @discussion "prefer_play_ultra_source", int value(1/0), default: 0. 可在 InitSDK 之后，拉流之前调用
 @discussion "keep_audio_session_active", bool value, default: false. if set true, app need to set the session inactive yourself
 @discussion "enforce_audio_loopback_in_sync", bool value, default: false. enforce audio loopback in synchronous method
 @discussion "audio_session_mix_with_others", bool value, default: true. set AVAudioSessionCategoryOptionMixWithOthers
 @discussion "support_general_mode_below_ios9", bool value, default: false. support general mode below ios 9.0
 @discussion "room_retry_time", uint32 value, default:300S，设置房间异常后自动恢复最大重试时间，SDK尽最大努力恢复，单位为S，SDK默认为300s，设置为0时不重试
 @discussion "av_retry_time", uint32 value, default:300S，设置推拉流异常后自动恢复最大重试时间，SDK尽最大努力恢复，单位为S，SDK默认为300s，设置为0时不重试
 @discussion "play_clear_last_frame", bool value, default false. 停止拉流时，是否清除最后一帧内容
 @discussion "preview_clear_last_frame", bool value, default false. 停止预览时，是否清除最后一帧内容。注意：必须停止预览且setPreviewView设置为空，此配置才能生效。否则，停止预览后，播放控件仍会保留最后一帧
 @discussion "vcap_external_handle_rotation", bool value, default true, 表示在推流端处理旋转；设置为 vcap_external_handle_rotation=false 时，会把旋转值传到拉流端（仅为 UDP 时有效）。这个配置目前只对外部采集的内存模式和 CVPixelBuffer 模式生效
 @discussion "audio_jitter_break_threshold_ms", uint32 value, default:200ms，定义音频卡顿时长，当音频卡顿时长超过该值时，认为存在卡顿，单位为ms，SDK默认为200ms，取值范围[10,2000]
 @discussion "video_jitter_break_threshold_ms", uint32 value, default:500ms，定义视频卡顿时长，当视频卡顿时长超过该值时，认为存在卡顿，单位为ms，SDK默认为500ms，取值范围[500,2000]
 @discussion "transform_http_to_https_on_init", bool value, default false，控制是否在initSDK时使用https替代http
 */
+ (void)setConfig:(NSString *)config;

/**
 设置试验性API回调

 @param blk 回调
 */
+ (void)setExperimentalAPICallback:(ZegoExperimentalAPIBlock)blk;

/**
 试验性API
 
 @param params 具体信息请咨询技术支持
 @return 0 成功, 否则失败
 */
+ (int)callExperimentalAPI:(NSString *)params;

/**
 设置 LogInfo 代理对象

 @param delegate 代理对象
 @return true 成功, false 失败
 */
- (bool)setLogInfoDelegate:(id<ZegoLogInfoDelegate>)delegate;

/**
 设置网络类型回调

 @param delegate 代理对象
 */
- (bool)setNetTypeDelegate:(id<ZegoNetTypeDelegate>)delegate;

/**
 发送通过流传输的实时有序数据
 
 @param data 数据，不超过 4096 字节
 @param index 推流通道号，参见 ZegoAPIPublishChannelIndex
 @return 大于 0 表示调用 seq，-1 表示失败
 @attention 必须在 index 通道推流成功后，才可以发送，否则会被丢弃。
 @attention 发送频率不超过每秒 200 次。
 @attention 接收方通过设置 setRealtimeSequentialDataDelegate 获取。
 */
- (int)sendRealtimeSequentialData:(NSData *)data channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置实时有序数据回调
 
 @param delegate 回调接口，详见 ZegoRealtimeSequentialDataDelegate 类。
 */
- (void)setRealtimeSequentialDataDelegate:(id<ZegoRealtimeSequentialDataDelegate>)delegate;

@end


@protocol ZegoRoomDelegate <NSObject>

@optional

/**
用户被踢出房间

@param reason 被踢出原因
@param roomID 房间 ID
@warning Deprecated, 请使用onKickOut:roomID:customReason
 */
- (void)onKickOut:(int)reason roomID:(NSString *)roomID;

/**
 用户被踢出房间
 
 @param reason 被踢出原因
 @param roomID 房间 ID
 @discussion 可在该回调中处理用户被踢出房间后的下一步处理（例如报错、重新登录提示等）
 */
- (void)onKickOut:(int)reason roomID:(NSString *)roomID customReason:(NSString *)customReason;

/**
 与 server 断开通知
 
 @param errorCode 错误码，0 表示无错误
 @param roomID 房间 ID
 @discussion 建议开发者在此通知中进行重新登录、推/拉流、报错、友好性提示等其他恢复逻辑。与 server 断开连接后，SDK 会进行重试，重试失败抛出此错误。请注意，此时 SDK 与服务器的所有连接均会断开
 */
- (void)onDisconnect:(int)errorCode roomID:(NSString *)roomID;

/**
 与 server 重连成功通知
 
 @param errorCode 错误码，0 表示无错误
 @param roomID 房间 ID
 */
- (void)onReconnect:(int)errorCode roomID:(NSString *)roomID;

/**
 与 server 连接中断通知，SDK会尝试自动重连
 
 @param errorCode 错误码，0 表示无错误
 @param roomID 房间 ID
 */
- (void)onTempBroken:(int)errorCode roomID:(NSString *)roomID;

/**
 流信息更新
 
 @param type 更新类型，详见 ZegoStreamType 定义
 @param streamList 直播流列表，列表中包含的是变更流的信息，非房间全部流信息
 @param roomID 房间 ID
 @discussion 房间内增加流、删除流，均会触发此更新。主播推流，自己不会收到此回调，房间内其他成员会收到。建议对流增加和流删除分别采取不同的处理。
 */
- (void)onStreamUpdated:(int)type streams:(NSArray<ZegoStream*> *)streamList roomID:(NSString *)roomID;

/**
 流附加信息更新
 
 @param streamList 附加信息更新的流列表
 @param roomID 房间 ID
 @discussion 主播推流成功后调用 [ZegoLiveRoomApi (Publisher) -updateStreamExtraInfo:] 更新附加信息，在此回调中通知房间内其他成员。调用 [ZegoLiveRoomApi (Publisher) -updateStreamExtraInfo:] 更新信息的调用方，不会收到此回调
 */
- (void)onStreamExtraInfoUpdated:(NSArray<ZegoStream *> *)streamList roomID:(NSString *)roomID;

/**
 收到自定义信令
 
 * 调用 -sendCustomCommand:content:completion: 发送自定义信令后，消息列表中的用户会触发此回调。
 
 @param fromUserID 消息来源 UserID
 @param fromUserName 消息来源 UserName
 @param content 消息内容
 @param roomID 房间 ID
 */
- (void)onReceiveCustomCommand:(NSString *)fromUserID userName:(NSString *)fromUserName content:(NSString*)content roomID:(NSString *)roomID;

/**
 收到房间信息更新
 
 * 房间信息更新，登录成功，或者重连成功后，房间信息变化会收到此回调
 
 @param roomInfo 房间信息 详情参加ZegoAPIRoomInfo
 @param roomID 房间 ID
 */
- (void)onRoomInfoUpdated:(ZegoAPIRoomInfo *)roomInfo  roomID:(NSString *)roomID;

/**
 房间内用户的网络质量回调
 
 @param userID 用户ID，空表示本地用户
 @param txQuality 上行网络质量，详见 ZegoLiveQuality 定义
 @param rxQuality 下行网络质量，详见 ZegoLiveQuality 定义
 @attention 只要发起推流或拉流，就会回调自身的网络质量
 @attention 已 RTC 或 L3 方式拉流，且对应推流用户和本地用户在同一房间内，会收到对应用户的网络质量
 @attention 用户在推流才有上行网络质量，拉流才有下行网络质量，否则网络质量为未知。
 */
- (void)onNetworkQuality:(NSString *)userID txQuality:(int)txQuality rxQuality:(int)rxQuality;

/**
 设置的token即将过期回调
 
 * 登录成功后在设置的token剩余时间小于等于30秒时会收到此回调
 
 @param roomID 房间 ID
 @param timeInSecond 剩余时间 单位为秒
 */
- (void)onTokenWillExpired:(NSString *)roomID remainTime:(int)timeInSecond;

@end

#ifndef Zego_Live_Event
#define Zego_Live_Event

/** 直播事件状态 */
typedef enum : NSUInteger {
    /** 播放直播开始重试 */
    Play_BeginRetry = 1,
    /** 播放直播重试成功 */
    Play_RetrySuccess = 2,
    
    /** 发布直播开始重试 */
    Publish_BeginRetry = 3,
    /** 发布直播重试成功 */
    Publish_RetrySuccess = 4,
    
    /** 拉流临时中断 */
    Play_TempDisconnected = 5,
    /** 推流临时中断 */
    Publish_TempDisconnected = 6,
    
    /** 视频卡顿开始 */
    Play_VideoBreak = 7,
    /** 视频卡顿结束 */
    Play_VideoBreakEnd = 8,
    /** 视频卡顿取消 */
    Play_VideoBreakCancel = 13,
    
    /** 音频卡顿开始 */
    Play_AudioBreak = 9,
    /** 音频卡顿结束 */
    Play_AudioBreakEnd = 10,
    /** 音频卡顿取消 */
    Play_AudioBreakCancel = 14,
    
    /** 注册推流信息失败 */
    PublishInfo_RegisterFailed = 11,
    /** 注册推流信息成功 */
    PublishInfo_RegisterSuccess = 12,
    
} ZegoLiveEvent;

#endif /* Zego_Live_Event */

/**
 直播事件代理
 */
@protocol ZegoLiveEventDelegate <NSObject>

/**
 直播事件回调
 
 * 用于监听流卡顿、推拉流重试操作等 SDK 事件。
 
 * 注意：info 目前的结构为 @{@"StreamID":EventStreamID}。

 @param event 发生的直播事件
 @param info 事件相关信息
 */
- (void)zego_onLiveEvent:(ZegoLiveEvent)event info:(NSDictionary<NSString*, NSString*>*)info;

@optional

/**
 推拉流事件回调
 
 * 用于监听推拉流开始、推拉流重试操作等 SDK 事件，及附加信息。
 
 * 注意：extraInfo 目前包括的键有 "url"表示地址, "streamProtocol"表示流协议，包括rtmp,flv,avertp，hls，webrtc等, "netProtocol"表示网络协议，包括tcp,udp,quic, "resourceType"表示资源类型，包括cdn,rtc,l3,

 @param event 发生的直播事件
 @param streamID 流 ID
 @param extraInfo 附加信息
 */
- (void)zego_onStreamEvent:(ZegoStreamEvent)event stream:(NSString*)streamID extraInfo:(NSDictionary<NSString*, NSString*>*)extraInfo;

@end

@protocol ZegoDeviceEventDelegate <NSObject>

/**
 设备事件回调
 
 @param deviceName 设备类型名称。返回值 kZegoDeviceCameraName/kZegoDeviceMicrophoneName/kZegoDeviceAudioName
 @param errorCode 错误码。 返回值参考 ZegoAPIDeviceErrorCode 定义
 @discussion 调用 [ZegoLiveRoomApi -setDeviceEventDelegate] 设置设备事件代理对象后，在此回调中获取设备状态或错误
 */
- (void)zego_onDevice:(NSString *)deviceName error:(int)errorCode;

/**
 设备事件回调
 
 @param deviceName 设备类型名称。返回值 kZegoDeviceCameraName/kZegoDeviceMicrophoneName/kZegoDeviceAudioName
 @param errorCode 错误码。 返回值参考 ZegoAPIDeviceErrorCode 定义
 @param deviceID  出错的设备id (目前仅支持mac设备)
 @discussion 调用 [ZegoLiveRoomApi -setDeviceEventDelegate] 设置设备事件代理对象后，在此回调中获取设备状态或错误
 */
- (void)zego_onDevice:(NSString *)deviceName error:(int)errorCode deviceID:(NSString *)deviceID;

#if TARGET_OS_OSX

@optional

/**
 音频设备改变状态的回调
 
 @param deviceId 设备ID
 @param deviceName 设备名
 @param deviceType 设备类型，参考 zego-api-defines-oc.h 中 ZegoAPIAudioDeviceType 的定义
 @param state   设备状态，参考 zego-api-defines-oc.h 中 ZegoAPIDeviceState 的定义
 @discussion 调用 [ZegoLiveRoomApi -setDeviceEventDelegate] 设置设备事件代理对象后，在此回调中获取音频设备改变状态的信息
 */
- (void)zego_onAudioDevice:(NSString *)deviceId deviceName:(NSString *)deviceName deviceType:(ZegoAPIAudioDeviceType)deviceType changeState:(ZegoAPIDeviceState)state;

/**
 音频设备音量变化的回调
 
 @param deviceId 设备ID
 @param deviceType 设备类型，参考 zego-api-defines-oc.h 中 ZegoAPIAudioDeviceType 的定义
 @param volume 音量，有效值 0 ~ 100
 @param volumeType  音量类型，参考 zego-api-defines-oc.h 中 ZegoAPIVolumeType 的定义
 @discussion 调用 [ZegoLiveRoomApi -setDeviceEventDelegate] 设置设备事件代理对象后，在此回调中获取音频设备音量变化的信息
 */
- (void)zego_onAudioDevice:(NSString *)deviceId deviceType:(ZegoAPIAudioDeviceType)deviceType changeVolume:(uint32_t)volume volumeType:(ZegoAPIVolumeType)volumeType mute:(bool)mute;

/**
 视频设备改变状态的回调
 
 @param deviceId 设备ID
 @param deviceName 设备名
 @param state   设备状态，参考 zego-api-defines-oc.h 中 ZegoAPIDeviceState 的定义
 @discussion 调用 [ZegoLiveRoomApi -setDeviceEventDelegate] 设置设备事件代理对象后，在此回调中获取视频设备改变状态的信息
 */
- (void)zego_onVideoDevice:(NSString *)deviceId deviceName:(NSString *)deviceName changeState:(ZegoAPIDeviceState)deviceState;

/**
 默认音频设备改变状态的回调
 
 @param deviceId 设备ID
 @param deviceType 设备类型，参考 zego-api-defines-oc.h 中 ZegoAPIAudioDeviceType 的定义
 @discussion 调用 [ZegoLiveRoomApi -setDeviceEventDelegate] 设置设备事件代理对象后，在此回调中获取音频设备改变状态的信息
 */
- (void)zego_onAudioDefaultDeviceStateChanged:(NSString *)deviceId deviceType:(ZegoAPIAudioDeviceType)deviceType;

#endif

@end

@protocol ZegoAVEngineDelegate <NSObject>

/**
 音视频引擎开始时回调
 */
- (void)onAVEngineStart;

/**
 音视频引擎停止时回调
 */
- (void)onAVEngineStop;

@end

@protocol ZegoLogInfoDelegate <NSObject>

@optional

/**
 日志将要写满
 */
- (void)onLogWillOverwrite;

/**
 日志上报结果
 
 @param errorCode 错误码
 */
- (void)onLogUploadResult:(int)errorCode;

@end


@protocol ZegoAudioRouteDelegate <NSObject>

/**
 音频路由发生变化
 
 @param audioRoute 当前的音频路由类型
 */
- (void)onAudioRouteChange:(ZegoAPIAudioRoute)audioRoute;

@end

@protocol ZegoNetTypeDelegate <NSObject>

/**
 网络类型变更通知

 @param netType 网络类型, 详见 zego-api-defines-oc.h 中的 ZegoAPINetType
 */
- (void)onNetTypeChange:(ZegoAPINetType)netType;

@end

/**
 实时有序数据回调
 */
@protocol ZegoRealtimeSequentialDataDelegate <NSObject>

/**
 接收实时有序数据的回调
 
 @attention 同步接口，请勿做耗时操作
 @param data 数据
 @param streamID 流 ID
 */
- (void)onRecvRealtimeSequentialData:(NSData *)data streamID:(NSString *)streamID;

@optional

/**
 发送实时有序数据的回调
 
 @param seq 发送序号，和 sendRealtimeSequentialData 的返回值对应
 @param errorCode 错误码，0 表示成功, 10008002 表示 SDK 尚未初始化，12102005 表示推流通道还未推流成功。
 */
- (void)onSendRealtimeSequentialData:(int)seq error:(int)errorCode;

@end




