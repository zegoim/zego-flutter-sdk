import 'dart:async';
import 'package:flutter/services.dart';

/// SDK 事件回调类型
class ZegoEventType {
  static const int TYPE_ROOM_EVENT = 0;
  static const int TYPE_PUBLISH_EVENT = 1;
  static const int TYPE_PLAY_EVENT = 2;
  static const int TYPE_MEDIA_SIDE_INFO_EVENT = 3;
  static const int TYPE_SOUND_LEVEL_EVENT = 4;
  static const int TYPE_MEDIA_PLAYER_EVENT = 5;
  static const int TYPE_AUDIO_PLAYER_EVENT = 10;
  static const int TYPE_MEDIA_RECORD_EVENT = 11;
}

class ZegoLiveRoomEventChannel {
  /* Event Channel */
  static const EventChannel _eventChannel = 
  const EventChannel('plugins.zego.im/zegoliveroom_callback');

  /* Event Stream */
  static Stream<dynamic> _receivedEvents = _eventChannel.receiveBroadcastStream();

  /// 用于接收native层事件流，开发者无需关注
  static Stream<dynamic> listenRoomEvent() {
    return _receivedEvents
        .map<Map>((dynamic event) => event)
        .where((Map event) => event['type'] == ZegoEventType.TYPE_ROOM_EVENT)
        .map<dynamic>((Map event) => event['method']);
  }

  /// 用于接收native层事件流，开发者无需关注
  static Stream<dynamic> listenPublishEvent() {
    return _receivedEvents
        .map<Map>((dynamic event) => event)
        .where((Map event) => event['type'] == ZegoEventType.TYPE_PUBLISH_EVENT)
        .map<dynamic>((Map event) => event['method']);
  }

  /// 用于接收native层事件流，开发者无需关注
  static Stream<dynamic> listenPlayEvent() {
    return _receivedEvents
        .map<Map>((dynamic event) => event)
        .where((Map event) => event['type'] == ZegoEventType.TYPE_PLAY_EVENT)
        .map<dynamic>((Map event) => event['method']);
  }

  /// 用于接收native层事件流，开发者无需关注
  static Stream<dynamic> listenMediaSideInfoEvent() {
    return _receivedEvents
        .map<Map>((dynamic event) => event)
        .where((Map event) => event['type'] == ZegoEventType.TYPE_MEDIA_SIDE_INFO_EVENT)
        .map<dynamic>((Map event) => event['method']);
  }

  /// 用于接收native层事件流，开发者无需关注
  static Stream<dynamic> listenSoundLevelEvent() {
    return _receivedEvents
        .map<Map>((dynamic event) => event)
        .where((Map event) => event['type'] == ZegoEventType.TYPE_SOUND_LEVEL_EVENT)
        .map<dynamic>((Map event) => event['method']);
  }

  /// 用于接收native层事件流，开发者无需关注
  static Stream<dynamic> listenAudioPlayerEvent() {
    return _receivedEvents
        .map<Map>((dynamic event) => event)
        .where((Map event) => event['type'] == ZegoEventType.TYPE_AUDIO_PLAYER_EVENT)
        .map<dynamic>((Map event) => event['method']);
  }

  /// 用于接收native层事件流，开发者无需关注
  static Stream<dynamic> listenMediaPlayerEvent() {
    return _receivedEvents
        .map<Map>((dynamic event) => event)
        .where((Map event) => event['type'] == ZegoEventType.TYPE_MEDIA_PLAYER_EVENT)
        .map<dynamic>((Map event) => event['method']);
  }

  /// 用于接收native层事件流，开发者无需关注
  static Stream<dynamic> listenMediaRecordEvent() {
    return _receivedEvents
        .map<Map>((dynamic event) => event)
        .where((Map event) => event['type'] == ZegoEventType.TYPE_MEDIA_RECORD_EVENT)
        .map<dynamic>((Map event) => event['method']);
  }
}