import 'dart:async';
import 'package:flutter/services.dart';
import 'package:zegoliveroom_plugin/zego_api_defines.dart';

/*class ZegoLiveViewPlugin {
  static const MethodChannel _channel = const MethodChannel('plugins.zego.im/zegoliveroom_plugin');

  static Future<bool> initRenderModule() async {
    final bool success = await _channel.invokeMethod('initRenderModule');

    return success;
  }

  static Future<bool> uninitRenderModule() async {
    final bool success = await _channel.invokeMethod('uninitRenderModule');

    return success;
  }

  static Future<int> createViewRenderer(String streamID, bool isPublisher, int width, int height) async{
    final int textureID = await _channel.invokeMethod('createViewRenderer',
        {'streamID': streamID, 'isPublisher': isPublisher, 'view_width': width, 'view_height': height});

    return textureID;
  }
  
  static Future<bool> setViewMode(String streamID, ZegoViewMode mode) async {
    final bool success = await _channel.invokeMethod('setViewMode',
        {'streamID': streamID, 'mode': mode.index});

    return success;
  }
  
  static Future<bool> updateRenderSize(String streamID, int width, int height) async {
    final bool success = await _channel.invokeMethod('updateRenderSize',
      {'streamID': streamID, 'width': width, 'height': height});

    return success;
  }

  static Future<bool> deleteViewRenderer(String streamID) async {
    final bool success = await _channel.invokeMethod('deleteViewRenderer',
        {'streamID': streamID});
    return success;
  }
}*/