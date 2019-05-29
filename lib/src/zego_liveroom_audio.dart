import 'dart:async';
import 'package:flutter/services.dart';

class ZegoLiveRoomAudioPlugin {
  /* Method Channel */
  static const MethodChannel _channel =
  const MethodChannel('plugins.zego.im/zegoliveroom_plugin');

  ///耳机插入状态下是否使用回声消除
  ///
  ///@param enable true 使用，false 不使用。默认 true
  ///@discussion InitSDK 之后，在推流之前调用
  static Future<void> enableAECWhenHeadsetDetected(bool enable) async {
    return await _channel.invokeMethod('enableAECWhenHeadsetDetected', {
      'enable': enable
    });
  }
}