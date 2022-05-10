import 'dart:async';
import 'package:flutter/services.dart';
import 'zego_api_error_code.dart';
import 'zego_api_defines.dart';
import 'zego_liveroom_event_channel.dart';


class ZegoMediaRecorderPlugin {
  /* Method Channel */
  static const MethodChannel _channel =
  const MethodChannel('plugins.zego.im/zegoliveroom_plugin');

  /// 开始录制
  /// 
  /// @param config 媒体录制配置，详见 [ZegoMediaRecordConfig]
  /// @param channelIndex 录制通道
  /// @discussion 必须在init sdk之后调用
  static Future<bool> startRecord(ZegoMediaRecordConfig config) async {
    return await _channel.invokeMethod('startRecord', {
      'config': config.toMap()
    });
  }

  /// 停止录制
  /// 
  /// @param channelIndex 录制通道
  static Future<bool> stopRecord() async {
    return await _channel.invokeMethod('stopRecord');
  }

  /// 媒体录制回调
  ///
  /// @param errorCode: 错误码，详见 [ZegoMediaRecordErrorCode] 定义
  /// @param channelIndex 录制通道
  /// @param storagePath 录制文件存储路径
  static void Function(int errorCode, String storagePath) _onMediaRecord;

  /// 录制信息更新回调
  /// 
  /// @param storagePath 录制文件存储路径
  /// @param duration 录制时长，单位毫秒
  /// @param fileSize 文件大小，单位字节
  /// @param quality 录制质量
  static void Function(String storagePath, int duration, int fileSize, ZegoPublishStreamQuality quality) _onMediaRecordInfoUpdate;

  /// 用于接收native层事件流，开发者无需关注
  static StreamSubscription<dynamic> _streamSubscription;

  ///设置回调对象
  ///
  ///@discussion 开发者只有调用本 API 设置回调对象才能收到相关回调
  static void registerMediaRecordCallback({
    Function(int errorCode, String storagePath) onMediaRecord,
    Function(String storagePath, int duration, int fileSize, ZegoPublishStreamQuality quality) onMediaRecordInfoUpdate  
  }) {

    _onMediaRecord = onMediaRecord;
    _onMediaRecordInfoUpdate = onMediaRecordInfoUpdate;

    if (_streamSubscription == null)
      _streamSubscription = ZegoLiveRoomEventChannel.listenMediaRecordEvent()
          .listen(_eventListener);
  }

  ///销毁回调对象
  ///
  ///@discussion 当开发者不再需要接收回调时，必须显式调用本 API 销毁回调对象
  static void unregisterMediaRecordCallback() {

    _onMediaRecord = null;
    _onMediaRecordInfoUpdate = null;

    _streamSubscription?.cancel();
    _streamSubscription = null;
  }

  /// 用于处理native层事件流，开发者无需关注
  static void _eventListener(dynamic data) {
    
    final Map<dynamic, dynamic> args = data;
    switch (args['name']) {
      case 'onMediaRecord':
        if (_onMediaRecord != null) {
          int errorCode = args['errorCode'];
          int channelIndex = args['channelIndex'];
          String storagePath = args['storagePath'];
          _onMediaRecord(errorCode, storagePath);
        }
        break;

      case 'onMediaRecordInfoUpdate':
        if (_onMediaRecordInfoUpdate != null) {
          String storagePath = args['storagePath'];
          int duration = args['duration'];
          int fileSize = args['fileSize'];
          final Map<dynamic, dynamic> quality = args['quality'];
          _onMediaRecordInfoUpdate(storagePath, duration, fileSize, 
            ZegoPublishStreamQuality(
              quality['cfps'], 
              quality['vencFps'], 
              quality['fps'], 
              quality['kbps'], 
              quality['acapFps'], 
              quality['afps'], 
              quality['akbps'], 
              quality['rtt'], 
              quality['pktLostRate'] * 1.0 / 255.0,
              quality['quality'], 
              quality['isHardwareVenc'], 
              quality['videoCodecId'],
              quality['width'], 
              quality['height']
            )
          );
        }
        break;
      default:
        break;
    }
  }
}