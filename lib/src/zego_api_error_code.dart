import 'dart:async';
import 'package:flutter/services.dart';

class ZegoErrorCode {
  static final int kOK = 0;
  static const int kEnd = 1;
  static const int kNoneAppIdError = 10000101;
  static const int kNotLoginError = 10000105;
  static const int kPublishBadNameError = 10000106;
  static const int kInvalidParamError = 10001001;
  static const int kInvalidChannelError = 10001002;
  static const int kNullPointerError = 10001003;
  static const int kNoFreeChannelError = 10001101;
  static const int kFormatUrlError = 10001102;
  static const int kUnmatchStreamIDError = 10002001;
  static const int kUnmatchSeqError = 10002002;
  static const int kNoneSeqError = 10002003;
  static const int kUnmatchStateError = 10002004;
  static const int kRedirectError = 10003001;
  static const int kOutOfMemeryError = 10004001;
  static const int kStartThreadError = 10004002;
  static const int kStartRequestError = 10005001;
  static const int kStartUpdateStreamInfoError = 10006001;
  static const int kNetworkNotConnectError = 11000101;
  static const int kNetworkDnsResolveError = 11000404;
  static const int kNetworkDetectTcpError = 11001001;
  static const int kNetworkDetectTcpConcectError = 11001002;
  static const int kEngineCreateError = 12101001;
  static const int kEngineStatusError = 12101002;
  static const int kEngineStartError = 12101003;
  static const int kDeniedMaxRetryError = 12101004;
  static const int kDeniedDisableSwitchLineError = 12101005;
  static const int kExternalAudioDeviceWasNotEnabledError = 12101101;
  static const int kExternalAudioDeviceEngineError = 12101102;
  static const int kEngineNoPlayDataError = 12102001;
  static const int kEngineNoPublishDataError = 12102002;
  static const int kEngineConnectServerError = 12200001;
  static const int kEngineRtmpHandshakeError = 12200002;
  static const int kEngineRtmpAppConnectError = 12200003;
  static const int kEngineRtmpCreateStreamError = 12200004;
  static const int kEngineRtmpPublishBadNameError = 12200005;
  static const int kEngineServerDisconnectError = 12200006;
  static const int kEngineRtpConnectServerError = 12200100;
  static const int kEngineRtpHelloTimeoutError = 12200101;
  static const int kEngineRtpCreateSessionTimeoutError = 12200102;
  static const int kEngineRtpCreateSessionFailError = 12200103;
  static const int kEngineRtpPlayOrPublishTimeoutError = 12200104;
  static const int kEngineRtpPlayOrPublishDeniedError = 12200105;
  static const int kEngineRtpTimeoutError = 12200106;
  static const int kEngineHttpFlvProtocolError = 12200200;
  static const int kEngineHttpFlvHttpCodeError = 12200201;
  static const int kEngineHttpFlvParseFlvError = 12200202;
  static const int kEngineHttpFlvServerDisconnectError = 12200203;
  static const int kEngineHttpFlvRedirectError = 12200204;
  static const int kPlayStreamNotExistError = 12301004;
  static const int kMediaServerForbidError = 12301011;
  static const int kMediaServerPublishBadNameError = 12301012;
  static const int kMediaServerStopPublishError = 12301014;
  static const int kConfigDecryptError = 20000001;
  static const int kConfigOfflineError = 20000002;
  static const int kConfigDomainError = 20000003;
  static const int kConfigMediaNetworkNoneError = 20000004;
  static const int kConfigMediaNetworkNoUrlError = 20000005;
  static const int kConfigServerCouldntConnectError = 21200007;
  static const int kConfigServerTimeoutError = 21200028;
  static const int kConfigServerSslCaCertError = 21200060;
  static const int kDispatchServerInvalidError = 30000001;
  static const int kDispatchNoIpError = 30000404;
  static const int kDispatchServerCouldntConnectError = 31200007;
  static const int kDispatchServerTimeoutError = 31200028;
  static const int kDispatchServerSslCaCertError = 31200060;
  static const int kDispatchEmptyPublishIpsError = 32001002;
  static const int kDispatchEmptyPlayIpsError = 32001003;
  static const int kDispatchStreamNotExistError = 32001004;
  static const int kLogicServerNoUrlError = 40000404;
  static const int kLogicServerCouldntConnectError = 41200007;
  static const int kLogicServerTimeoutError = 41200028;
  static const int kLogicServerSslCaCertError = 41200060;
  static const int kLiveRoomRequestParamError = 50001001;
  static const int kLiveRoomHBTimeoutError = 50001002;
  static const int kLiveRoomNoPushServerAddrError = 50001003;
  static const int kLiveRoomNoPushCryptoKeyError = 50001004;
  static const int kLiveRoomNoPushTokenError = 50001005;
  static const int kLiveRoomCouldntConnectError = 51200007;
  static const int kLiveRoomTimeoutError = 51200028;
  static const int kLiveRoomSslCaCertError = 51200060;
  static const int kLiveRoomInputParamsError = 52000002;
  static const int kLiveRoomRoomAuthError = 52000101;
  static const int kLiveRoomRoomNotExistError = 52000104;
  static const int kLiveRoomNotLoginError = 52000105;
  static const int kLiveRoomSetStreamInfoError = 52000123;
  static const int kLiveRoomStreamInfoNotExist = 52000138;
  static const int kLiveRoomSessionError = 52000141;
  static const int kLiveRoomQpsLimitError = 52000201;
  static const int kLiveRoomLuaSessionError = 52001001;
  static const int kLiveRoomAddUserError = 52001002;
  static const int kLiveRoomDelUserError = 52001003;
  static const int kLiveRoomAddTransError = 52001009;
  static const int kLiveRoomMaxUserCountError = 52001105;
  static const int kLiveRoomPublishBadNameError = 52001012;
  static const int kLiveRoomRequiredReloginError = 52002001;
  static const int kLiveRoomThirdTokenAuthError = 52002002;
  static const int kLiveRoomNetBrokenTimeoutError = 52002003;
  static const int kRoomConnectError = 60001001;
  static const int kRoomDoHandShakeReqError = 60001002;
  static const int kRoomDoLoginReqError = 60001003;
  static const int kRoomTimeoutError = 60001004;
  static const int kRoomHbTimeoutError = 60001005;
  static const int kRoomStartConnectError = 60001006;
  static const int kRoomInvalidSocketError = 60002001;
  static const int kRoomInvalidRspError = 60003001;
  static const int kRoomDecodeSignError = 62001001;
  static const int kRoomReplayAttacksError = 62002001;
  static const int kRoomLoginCreateUserError = 62010001;
  static const int kRoomLoginSameCreateUserError = 62010002;
  static const int kRoomStatusTimeoutError = 62010005;
  static const int kRoomStatusRspError = 62010006;
  static const int kRoomMultipleLoginKickoutError = 63000001;
  static const int kRoomManualKickoutError = 63000002;
  static const int kRoomSessionErrorKickoutError = 63000003;
  static const int kMixStreamNoneMixConfigError = 80000001;
  static const int kMixStreamCouldntConnectError = 81200007;
  static const int kMixStreamTimeoutError = 81200028;
  static const int kMixStreamSslCaCertError = 81200060;
  static const int kMixStreamFailError = 82000001;
  static const int kMixStreamInputParamsError = 82000002;
  static const int kMixStreamAuthError = 82000003;
  static const int kMixStreamNotExistError = 82000150;
  static const int kMixStreamStartMixError = 82000151;
  static const int kMixStreamStopMixError = 82000152;
  static const int kMixStreamInputFormatError = 82000155;
  static const int kMixStreamOutputFormatError = 82000156;
  static const int kMixStreamNotOpenError = 82000157;
  static const int kMixStreamInputExceedError = 82000158;
  static const int kMixStreamDispatchError = 82000159;
  static const int kMixStreamStopMixOwnerError = 82000160;
  static const int kMixStreamWaterMarkParamError = 82000170;
  static const int kMixStreamWaterMarkImageError = 82000171;
  static const int kMixStreamQpsOverloadError = 82000190;
}

class ZegoMediaRecordErrorCode {
  /// 成功
  static const int success = 0;
  /// 路径太长
  static const int pathTooLong = 1;
  /// 初始化 avcontext 失败
  static const int initFailed = 2;
  /// 打开文件失败
  static const int openFileFailed = 3;
  /// 写文件头失败
  static const int writeHeaderFailed = 4;
  /// 开始录制
  static const int recordBegin = 5;
  /// 停止录制
  static const int recordEnd = 6;
  /// 剩余空间不够
  static const int noSpace = 7;
  /// 文件句柄异常
  static const int badFileDesc = 8;
  /// IO 异常
  static const int ioError = 9;
}

class ZegoErrorHandler {
  /* Method Channel */
  static const MethodChannel _channel =
  const MethodChannel('plugins.zego.im/zegoliveroom_plugin');

  static Future<bool> isInitSDKError(int error) async {
    return await _channel.invokeMethod('isInitSDKError', {
      'error': error
    });
  }

  static Future<bool> isNotLoginError(int error) async {
    return await _channel.invokeMethod('isNotLoginError', {
      'error': error
    });
  }

  static Future<bool> isMediaServerNetWorkError(int error) async {
    return await _channel.invokeMethod('isMediaServerNetWorkError', {
      'error': error
    });
  }

  static Future<bool> isLogicServerNetWorkError(int error) async {
    return await _channel.invokeMethod('isLogicServerNetWorkError', {
      'error': error
    });
  }

  static Future<bool> isMixStreamNotExistError(int error) async {
    return await _channel.invokeMethod('isMixStreamNotExistError', {
      'error': error
    });
  }

  static Future<bool> isPlayStreamNotExistError(int error) async {
    return await _channel.invokeMethod('isPlayStreamNotExistError', {
      'error': error
    });
  }

  static Future<bool> isPublishBadNameError(int error) async {
    return await _channel.invokeMethod('isPublishBadNameError', {
      'error': error
    });
  }

  static Future<bool> isPublishForbidError(int error) async {
    return await _channel.invokeMethod('isPublishForbidError', {
      'error': error
    });
  }

  static Future<bool> isPublishStopError(int error) async {
    return await _channel.invokeMethod('isPublishStopError', {
      'error': error
    });
  }

  static Future<bool> isPublishDeniedError(int error) async {
    return await _channel.invokeMethod('isPublishDeniedError', {
      'error': error
    });
  }

  static Future<bool> isPlayForbidError(int error) async {
    return await _channel.invokeMethod('isPlayForbidError', {
      'error': error
    });
  }

  static Future<bool> isPlayDeniedError(int error) async {
    return await _channel.invokeMethod('isPlayDeniedError', {
      'error': error
    });
  }

  static Future<bool> isDNSResolveError(int error) async {
    return await _channel.invokeMethod('isDNSResolveError', {
      'error': error
    });
  }

  static Future<bool> isNetworkUnreachError(int error) async {
    return await _channel.invokeMethod('isNetworkUnreachError', {
      'error': error
    });
  }

  static Future<bool> isHttpProtocolError(int error) async {
    return await _channel.invokeMethod('isHttpProtocolError', {
      'error': error
    });
  }

  static Future<bool> isReqFrequencyLimitError(int error) async {
    return await _channel.invokeMethod('isReqFrequencyLimitError', {
      'error': error
    });
  }

  static Future<bool> isLiveRoomError(int error) async {
    return await _channel.invokeMethod('isLiveRoomError', {
      'error': error
    });
  }

  static Future<bool> isMultiLoginError(int error) async {
    return await _channel.invokeMethod('isMultiLoginError', {
      'error': error
    });
  }

  static Future<bool> isManualKickoutError(int error) async {
    return await _channel.invokeMethod('isManualKickoutError', {
      'error': error
    });
  }

  static Future<bool> isRoomSessionError(int error) async {
    return await _channel.invokeMethod('isRoomSessionError', {
      'error': error
    });
  }

  static Future<String> getErrorMsg(int error) async {
    return await _channel.invokeMethod('getErrorMsg', {
      'error': error
    });
  }

}