import 'dart:async';
import 'package:flutter/services.dart';
import 'package:zegoliveroom_plugin/zegoliveroom_plugin.dart';

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

  ///设置虚拟立体声
  ///
  ///@param enable true 开启，false 关闭。默认 false
  ///@param angle 角度，范围 0 ~ 180，90 为正前方，0 和 180 分别对应最右边和最左边
  ///@discussion 必须在 InitSDK 之后调用，且需调用 [ZegoLiveRoomPublisherPlugin.setAudioChannelCount] 设置为双声道编码
  static Future<void> enableVirtualStereo(bool enable, int angle) async {
    return await _channel.invokeMethod('enableVirtualStereo', {
      'enable': enable,
      'angle': angle
    });
  }

  ///设置音频混响
  ///
  ///@param enable true 开启，false 关闭。默认为 false
  ///@param mode 混响模式，参考 [ZegoAudioReverbMode]
  ///@discussion 必须在 InitSDK 之后调用，且推流途中修改 mode 值均能动态生效
  static Future<void> enableReverb(bool enable, int mode) async {
    return await _channel.invokeMethod('enableReverb', {
      'enable': enable,
      'mode': mode
    });
  }

  ///设置音频混响参数值
  ///
  ///@param 混响参数，参考 [ZegoAudioReverbParam]
  ///@discussion 必须在 InitSDK 之后调用，且推流途中修改参数值均能动态生效
  static Future<void> setReverbParam(ZegoAudioReverbParam param) async {
    return await _channel.invokeMethod('setReverbParam', {
      'param': param.toMap()
    });
  }

  ///设置变声器参数
  ///
  ///@param param 变声参数，取值范围为 [-8.0, 8.0] ，默认为 0 （不变声）
  ///@discussion 必须在 InitSDK 之后调用，且仅对采集的声音有效
  ///@note 几种典型的变声效果定义如下：
  ///@note 1. 女声变男生：-3.0
  ///@note 2. 男声变女声：4.0
  ///@note 3. 女声变童声：6.0
  ///@note 4. 男声变童声：8.0
  static Future<void> setVoiceChangerParam(double param) async {
    return await _channel.invokeMethod('setVoiceChangerParam', {
      'param': param
    });
  }
}