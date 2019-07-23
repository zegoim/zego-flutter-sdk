#ifndef zego_api_error_oc
#define zego_api_error_oc

#import <Foundation/Foundation.h>

typedef enum : NSUInteger {

 kOK                            = 0,
 kEnd                           = 1,

 kNoneAppIdError                = 10000101,
 kNotLoginError                 = 10000105,
 kPublishBadNameError           = 10000106,

 kInvalidParamError             = 10001001,
 kInvalidChannelError           = 10001002,
 kNullPointerError              = 10001003,
 kNoFreeChannelError            = 10001101,
 kFormatUrlError                = 10001102,
 kUnmatchStreamIDError          = 10002001,
 kUnmatchSeqError               = 10002002,
 kNoneSeqError                  = 10002003,
 kUnmatchStateError             = 10002004,
 kRedirectError                 = 10003001,
 kOutOfMemeryError              = 10004001,
 kStartThreadError              = 10004002,
 kStartRequestError             = 10005001,
 kStartUpdateStreamInfoError    = 10006001,

 kNetworkNotConnectError        = 11000101,
 kNetworkDnsResolveError        = 11000404,

 kEngineCreateError             = 12101001,
 kEngineStatusError             = 12101002,
 kEngineStartError              = 12101003,
 kDeniedMaxRetryError           = 12101004,
 kDeniedDisableSwitchLineError  = 12101005,

 kEngineNoPlayDataError         = 12102001,
 kEngineNoPublishDataError      = 12102002,

 kEngineConnectServerError      = 12200001,
 kEngineRtmpHandshakeError      = 12200002,
 kEngineRtmpAppConnectError     = 12200003,
 kEngineRtmpCreateStreamError   = 12200004,
 kEngineRtmpPublishBadNameError = 12200005,
 kEngineServerDisconnectError   = 12200006,
 
 kEngineRtpConnectServerError   = 12200100,
 kEngineRtpHelloTimeoutError    = 12200101,
 kEngineRtpCreateSessionTimeoutError = 12200102,
 kEngineRtpCreateSessionFailError    = 12200103,
 kEngineRtpPlayOrPublishTimeoutError = 12200104,
 kEngineRtpPlayOrPublishDeniedError  = 12200105,
 kEngineRtpTimeoutError         = 12200106,
 kEngineHttpFlvProtocolError    = 12200200,
 kEngineHttpFlvHttpCodeError    = 12200201,
 kEngineHttpFlvParseFlvError    = 12200202,
 kEngineHttpFlvServerDisconnectError = 12200203,
 kEngineHttpFlvRedirectError    = 12200204,

 kPlayStreamNotExistError       = 12301004,
 kMediaServerForbidError        = 12301011,
 kMediaServerPublishBadNameError= 12301012,
 kMediaServerStopPublishError   = 12301014,

 kConfigDecryptError            = 20000001,
 kConfigOfflineError            = 20000002,
 kConfigDomainError             = 20000003,
 kConfigMediaNetworkNoneError   = 20000004,
 kConfigMediaNetworkNoUrlError  = 20000005,
 kConfigServerCouldntConnectError = 21200007,
 kConfigServerTimeoutError      = 21200028,
 kConfigServerSslCaCertError    = 21200060,


 kDispatchServerInvalidError    = 30000001,
 kDispatchNoIpError             = 30000404,
 kDispatchServerCouldntConnectError = 31200007,
 kDispatchServerTimeoutError    = 31200028,
 kDispatchServerSslCaCertError  = 31200060,
 kDispatchEmptyPublishIpsError  = 32001002,
 kDispatchEmptyPlayIpsError     = 32001003,
 kDispatchStreamNotExistError   = 32001004,

 kLogicServerNoUrlError         = 40000404,
 kLogicServerCouldntConnectError= 41200007,
 kLogicServerTimeoutError       = 41200028,
 kLogicServerSslCaCertError     = 41200060,

 kLiveRoomRequestParamError     = 50001001,
 kLiveRoomHBTimeoutError        = 50001002,
 kLiveRoomNoPushServerAddrError = 50001003,
 kLiveRoomNoPushCryptoKeyError  = 50001004,
 kLiveRoomNoPushTokenError      = 50001005,
 kLiveRoomCouldntConnectError   = 51200007,
 kLiveRoomTimeoutError          = 51200028,
 kLiveRoomSslCaCertError        = 51200060,
 kLiveRoomInvalidRspError       = 51400003,
 kLiveRoomInputParamsError      = 52000002,
 kLiveRoomRoomAuthError         = 52000101,
 kLiveRoomRoomNotExistError     = 52000104,
 kLiveRoomNotLoginError         = 52000105,
 kLiveRoomSetStreamInfoError    = 52000123,
 kLiveRoomStreamInfoNotExist    = 52000138,
 kLiveRoomSessionError          = 52000141,
 kLiveRoomQpsLimitError         = 52000201,
 kLiveRoomLuaSessionError       = 52001001,
 kLiveRoomAddUserError          = 52001002,
 kLiveRoomDelUserError          = 52001003,
 kLiveRoomAddTransError         = 52001009,
 kLiveRoomMaxUserCountError     = 52001105,
 kLiveRoomPublishBadNameError   = 52001012,
 kLiveRoomRequiredReloginError  = 52002001,
 kLiveRoomThirdTokenAuthError   = 52002002,
 kLiveRoomNetBrokenTimeoutError = 52002003,

 kRoomConnectError              = 60001001,
 kRoomDoHandShakeReqError       = 60001002,
 kRoomDoLoginReqError           = 60001003,
 kRoomTimeoutError              = 60001004,
 kRoomHbTimeoutError            = 60001005,
 kRoomStartConnectError         = 60001006,
 kRoomInvalidSocketError        = 60002001,
 kRoomInvalidRspError           = 60003001,
 kRoomDecodeSignError           = 62001001,
 kRoomReplayAttacksError        = 62002001,
 kRoomLoginCreateUserError      = 62010001,
 kRoomLoginSameCreateUserError  = 62010002,
 kRoomStatusTimeoutError        = 62010005,
 kRoomStatusRspError            = 62010006,
 kRoomMultipleLoginKickoutError = 63000001,
 kRoomManualKickoutError        = 63000002,
 kRoomSessionErrorKickoutError  = 63000003,

 //混流错误码定义
 kMixStreamNoneMixConfigError   = 80000001, //没有混流配置信息
 kMixStreamCouldntConnectError  = 81200007, //混流服务连接失败
 kMixStreamTimeoutError         = 81200028, //混流服务响应超时
 kMixStreamSslCaCertError       = 81200060, //混流服务证书错误
 kMixStreamFailError            = 82000001, //混流失败
 kMixStreamInputParamsError     = 82000002, //混流请求包解析错误
 kMixStreamAuthError            = 82000003, //混流鉴权失败
 kMixStreamNotExistError        = 82000150, //混流的输入流不存在
 kMixStreamStartMixError        = 82000151, //启动混流失败
 kMixStreamStopMixError         = 82000152, //停止混流失败
 kMixStreamInputFormatError     = 82000155, //输入分辨率格式错误
 kMixStreamOutputFormatError    = 82000156, //输出分辨率格式错误
 kMixStreamNotOpenError         = 82000157, //混流没开
 kMixStreamInputExceedError     = 82000158, //混流输入流数超限
 kMixStreamDispatchError        = 82000159, //混流Dispatch失败
 kMixStreamStopMixOwnerError    = 82000160, //非创建者停止混流错误
 kMixStreamWaterMarkParamError  = 82000170, //混流水印参数错误
 kMixStreamWaterMarkImageError  = 82000171, //混流水印image错误
 kMixStreamQpsOverloadError     = 82000190, //混流请求qps超限错误

} ZegoErrorCode;

@interface ZegoError : NSObject

/**
 是否初始化SDK错误

 @param error 错误码
 @return 是否初始化SDK错误
 */
+ (bool)isInitSDKError:(int)error;

/**
 是否未登录错误

 @param error 错误码
 @return 是否未登录错误
 */
+ (bool)isNotLoginError:(int)error;

/**
 是否媒体服务器连接失败错误

 @param error 错误码
 @return 是否媒体服务器连接失败错误
 */
+ (bool)isMediaServerNetWorkError:(int)error;

/**
 是否逻辑服务器连接失败错误

 @param error 错误码
 @return 是否逻辑服务器连接失败错误
 */
+ (bool)isLogicServerNetWrokError:(int)error;

/**
 是否混流的流不存在错误

 @param error 错误码
 @return 是否混流的流不存在错误
 */
+ (bool)isMixStreamNotExistError:(int)error;

/**
 是否拉的流不存在错误

 @param error 错误码
 @return 是否拉的流不存在错误
 */
+ (bool)isPlayStreamNotExistError:(int)error;

/**
 是否推流重复流名错误
 
 @param error 错误码
 @return 是否推流重复流名错误
 */
+ (bool)isPublishBadNameError:(int)error;

/**
 是否禁止推流错误

 @param error 错误码
 @return 是否禁止推流错误
 */
+ (bool)isPublishForbidError:(int)error;

/**
 是否停止推流错误

 @param error 错误码
 @return 是否停止推流错误
 */
+ (bool)isPublishStopError:(int)error;


/**
 是否推流Denied错误
 
 @param error 错误码
 @return 是否推流Denied错误
 */
+ (bool)isPublishDeniedError:(int)error;

/**
 是否禁止拉流错误

 @param error 错误码
 @return 是否禁止拉流错误
 */
+ (bool)isPlayForbidError:(int)error;

/**
 是否拉流Denied错误

 @param error 错误码
 @return 是否拉流Denied错误
 */
+ (bool)isPlayDeniedError:(int)error;

/**
 是否DNSResolve错误
 
 @param error 错误码
 @return 是否DNSResolve错误
 */
+ (bool)isDNSResolveError:(int)error;

/**
 是否Network不可达错误
 
 @param error 错误码
 @return 是否Network不可达错误
 */
+ (bool)isNetworkUnreachError:(int)error;

/**
 是否Http协议错误

 @param error 错误码
 @return 是否Http协议错误
 */
+ (bool)isHttpProtocolError:(int)error;

/**
 是否超过服务请求频率错误

 @param error 错误码
 @return 是否超过服务请求频率错误
 */
+ (bool)isReqFrequencyLimitError:(int)error;

/**
 是否访问LiveRoom服务错误

 @param error 错误码
 @return 是否访问LiveRoom服务错误
 */
+ (bool)isLiveRoomError:(int)error;

/**
 是否该账户多点登录被踢出错误

 @param error 错误码
 @return 是否该账户多点登录被踢出错误
 */
+ (bool)isMultiLoginError:(int)error;

/**
 是否该账户被手动踢出错误

 @param error 错误码
 @return 是否该账户被手动踢出错误
 */
+ (bool)isManualKickoutError:(int)error;

/**
 是否房间会话错误

 @param error 错误码
 @return 是否房间会话错误
 */
+ (bool)isRoomSessionError:(int)error;

/**
 获取error msg
 
 @param error 错误码
 @return error msg
 */
+ (NSString *)getErrorMsg:(int)error;

@end

#endif
