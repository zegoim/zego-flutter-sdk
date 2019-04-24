import 'dart:async';
import 'package:flutter/services.dart';
import 'package:zegoliveroom_plugin/zego_liveroom_event_channel.dart';

const String CALLBACK_ID = 'zegoliveroom_callback';

class ZegoMediaSideInfo {
  /* Method Channel */
  static const MethodChannel _channel =
  const MethodChannel('plugins.zego.im/zegoliveroom_plugin');

  ///发送媒体次要信息开关
  ///
  ///@param bStart true 开启, false 关闭
  ///@param bOnlyAudioPublish true 纯音频直播，不传输视频数据, false 音视频直播，传输视频数据
  ///@return true 成功，false 失败
  ///@attention onlyAudioPublish 开关在 start 开关开启时才生效
  ///@attention 必须在InitSDK之后、推流前，设置
  static Future<void> setMediaSideFlags(bool start, bool onlyAudioPublish) async {
    return await _channel.invokeMethod('setMediaSideFlags', {
      'start' : start,
      'onlyAudioPublish' : onlyAudioPublish
    });
  }

  ///发送媒体次要信息
  ///
  ///@param inData 媒体次要信息数据
  ///@return true 成功，false 失败
  ///@attention 必须在推流成功后调用
  static Future<void> sendMediaSideInfo(String data) async {
    return await _channel.invokeMethod('sendMediaSideInfo', {
      'data' : data,
    });
  }

  ///设置回调对象
  ///
  ///@param onRecvMediaSideInfo 设置接收 流媒体次要信息 回调，参考 [_onRecvMediaSideInfo] 定义
  ///@discussion 开发者只有调用本 API 设置回调对象才能收到相关回调
  static void registerMediaSideInfoCallback({
    Function(String streamID, String data) onRecvMediaSideInfo
  }) {

    _onRecvMediaSideInfo = onRecvMediaSideInfo;

    if(_streamSubscription == null) {
      _streamSubscription = ZegoLiveRoomEventChannel.listenMediaSideInfoEvent().listen(_eventListener);
    }

  }

  ///销毁回调对象
  ///
  ///@discussion 当开发者不再需要接收回调时，必须显式调用本 API 销毁回调对象
  static void unregisterMediaSideInfoCallback() {

    _onRecvMediaSideInfo = null;

    _streamSubscription?.cancel();
    _streamSubscription = null;

  }



  ///流媒体次要信息回调
  ///
  ///@param streamID 流ID，标记当前回调的信息属于哪条流
  ///@param data，数据缓冲区，存储流媒体次要信息
  static void Function(String streamID, String data) _onRecvMediaSideInfo;




  /// 用于接收native层事件流，开发者无需关注
  static StreamSubscription<dynamic> _streamSubscription;
  /// 用于处理native层事件流，开发者无需关注
  static void _eventListener(dynamic data) {

    final Map<dynamic, dynamic> args = data;
    switch (args['name']) {
      case 'onRecvMediaSideInfo':
        if (_onRecvMediaSideInfo != null) {

          String streamID = args['streamID'];
          String data = args['data'];

          _onRecvMediaSideInfo(streamID, data);
        }
        break;
        default:
          break;
    }
  }

}
