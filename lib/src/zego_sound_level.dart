import 'dart:async';
import 'package:flutter/services.dart';
import 'zego_liveroom_event_channel.dart';

class ZegoSoundLevelInfo {

  /// 流ID
  final String streamID;
  /// SoundLevel 数值
  final double soundLevel;

  const ZegoSoundLevelInfo(this.streamID, this.soundLevel);
}

class ZegoSoundLevel {
  /* Method Channel */
  static const MethodChannel _channel =
  const MethodChannel('plugins.zego.im/zegoliveroom_plugin');

  static Future<bool> setSoundLevelMonitorCycle(int ms) async {
    final success = await _channel.invokeMethod('setSoundLevelMonitorCycle', {
      'ms': ms
    });

    return success;
  }

  static Future<bool> startSoundLevelMonitor() async {
    final success = await _channel.invokeMethod('startSoundLevelMonitor');
    return success;
  }

  static Future<bool> stopSoundLevelMonitor() async {
    final success = await _channel.invokeMethod('stopSoundLevelMonitor');
    return success;
  }

  static Future<void> registerSoundLevelCallback({
    Function(ZegoSoundLevelInfo soundLevel) onCaptureSoundLevelUpdate,
    Function(List<ZegoSoundLevelInfo> soundLevels) onSoundLevelUpdate
  }) async {

    await _channel.invokeMethod('registerSoundLevelCallback');

    _onCaptureSoundLevelUpdate = onCaptureSoundLevelUpdate;
    _onSoundLevelUpdate = onSoundLevelUpdate;
    
    if(_streamSubscription == null)
      _streamSubscription = ZegoLiveRoomEventChannel.listenSoundLevelEvent().listen(_eventListener);
  }

  static Future<void> unregisterSoundLevelCallback() async {

    await _channel.invokeMethod('unregisterSoundLevelCallback');

    print('unsigner');
    _onCaptureSoundLevelUpdate = null;
    _onSoundLevelUpdate = null;

    _streamSubscription?.cancel();
    _streamSubscription = null;
  }


  static void Function(ZegoSoundLevelInfo soundLevel) _onCaptureSoundLevelUpdate;

  static void Function(List<ZegoSoundLevelInfo> soundLevels) _onSoundLevelUpdate;

  /// 用于接收native层事件流，开发者无需关注
  static StreamSubscription<dynamic> _streamSubscription;
  /// 用于处理native层事件流，开发者无需关注
  static void _eventListener(dynamic data) {

    final Map<dynamic, dynamic> args = data;
    switch (args['name']) {
      case 'onCaptureSoundLevelUpdate':
        if (_onCaptureSoundLevelUpdate != null) {

          String streamID = args['streamID'];
          double soundLevel = args['soundLevel'];

          ZegoSoundLevelInfo info = ZegoSoundLevelInfo(streamID, soundLevel);
          _onCaptureSoundLevelUpdate(info);
        }
        break;
      case 'onSoundLevelUpdate':
        if(_onSoundLevelUpdate != null) {

          List<ZegoSoundLevelInfo> infos = [];
          List<dynamic> soundLevels = args['soundLevels'];
          for(Map<dynamic, dynamic> soundLevelInfo in soundLevels) {
              String streamID = soundLevelInfo['streamID'];
              double soundLevel = soundLevelInfo['soundLevel'];

              ZegoSoundLevelInfo objInfo = ZegoSoundLevelInfo(streamID, soundLevel);
              infos.add(objInfo);
          }

          _onSoundLevelUpdate(infos);
        }
        break;
      default:
        break;
    }
  }


}