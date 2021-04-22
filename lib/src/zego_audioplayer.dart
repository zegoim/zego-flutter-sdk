import 'dart:async';
import 'package:flutter/services.dart';
import 'zego_liveroom_event_channel.dart';

class ZegoAudioplayer {
  /* Method Channel */
  static const MethodChannel _channel =
      const MethodChannel('plugins.zego.im/zegoliveroom_plugin');

  ///播放音效
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@param sourcePath 音效本地路径，若开发者先调用了 [preload]，则无需再从本接口中传入 sourcePath；否则必须填入 sourcePath
  ///@param isAssetPath 是否为 flutter asset 资源路径，若为 true，则应传入 pubspec.yaml 中声明的资源路径（如 assets/zego.mp3）；若为 false，则 SDK 会将其当作绝对路径
  ///@param loopCount 音效播放循环次数，默认为 0，即只播放一次
  ///@param isPublish 是否需要将播放的音效混到推流中推出，若为 true，则拉流方也能听到音效
  ///@retuen errorCode 0 为播放成功，-1 为播放失败
  ///@discussion 最大同时支持播放 12 个音效文件
  static Future<int> play(int soundID,
      {String sourcePath = '',
      bool isAssetPath = true,
      int loopCount = 0,
      bool isPublish = true}) async {
    final Map<dynamic, dynamic> errorMsg =
        await _channel.invokeMethod('playAudioEffect', {
      'soundID': soundID,
      'path': sourcePath,
      'isAssetPath': isAssetPath,
      'loopCount': loopCount,
      'isPublish': isPublish
    });

    int error = errorMsg['errorCode'];
    return error;
  }

  ///停止播放音效
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  static Future<void> stop(int soundID) async {
    return await _channel.invokeMethod('stopAudioEffect', {'soundID': soundID});
  }

  ///暂停播放音效
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  static Future<void> pause(int soundID) async {
    return await _channel
        .invokeMethod('pauseAudioEffect', {'soundID': soundID});
  }

  ///恢复播放音效
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@discussion 当开发者调用了 [pause] 暂停播放音效后，通过调用本接口进行恢复
  static Future<void> resume(int soundID) async {
    return await _channel
        .invokeMethod('resumeAudioEffect', {'soundID': soundID});
  }

  ///预加载音效，避免播放音频时造成的读写文件延迟
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@param sourcePath 音效本地路径，支持 flutter asset 资源路径和绝对路径
  ///@param isAssetPath 是否为 flutter asset 资源路径，若为 true，则应传入 pubspec.yaml 中声明的资源路径（如 assets/zego.mp3）；若为 false，则 SDK 会将其当作绝对路径
  ///@discussion 通过预加载接口导入的音效文件不能超过 30s，否则会调用失败
  ///@discussion 若需要播放超过 30s 的长文件，则直接调用 [start] 播放
  ///@discussion 最大支持预加载 15 个音效
  static Future<int> preload(
    int soundID,
    String sourcePath, {
    bool isAssetPath = true,
  }) async {
    final Map<dynamic, dynamic> errorMsg = await _channel.invokeMethod(
        'preloadAudioEffect',
        {'soundID': soundID, 'path': sourcePath, 'isAssetPath': isAssetPath});

    int error = errorMsg['errorCode'];
    return error;
  }

  ///释放预加载的音效
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@discussion 当开发者调用 [preload] 之后，音效会加载到内存中，开发者需要在适当时机释放内存占用
  static Future<void> unload(int soundID) async {
    return await _channel
        .invokeMethod('unloadAudioEffect', {'soundID': soundID});
  }

  ///设置音效的音量
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@param volume 音量，取值范围为 0~100，默认为 100
  static Future<void> setVolume(int soundID, int volume) async {
    return await _channel.invokeMethod(
        'setAudioEffectVolume', {'soundID': soundID, 'volume': volume});
  }

  ///统一设置所有音效的音量
  ///
  ///@param volume 音量，取值范围为 0~100，默认为 100
  static Future<void> setAllVolume(int volume) async {
    return await _channel
        .invokeMethod('setAllAudioEffectVolume', {'volume': volume});
  }

  ///统一暂停所有音效
  ///
  static Future<void> pauseAll() async {
    return await _channel.invokeMethod('pauseAllAudioEffect');
  }

  ///统一恢复所有音效
  ///
  static Future<void> resumeAll() async {
    return await _channel.invokeMethod('resumeAllAudioEffect');
  }

  ///统一暂停所有音效
  ///
  static Future<void> stopAll() async {
    return await _channel.invokeMethod('stopAllAudioEffect');
  }

  ///设置音效播放进度
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@param timestamp 需要跳转的进度，单位毫秒
  ///@return 是否设置成功，0 为成功，-1 为失败
  static Future<int> seekTo(int soundID, int timestamp) async {
    final int error = await _channel.invokeMethod(
        'seekToAudio', {'soundID': soundID, 'timestamp': timestamp});

    return error;
  }

  ///获取音效文件总时长
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@return timestamp 总时长，单位毫秒；获取失败则返回 0
  static Future<int> getTotalDuration(int soundID) async {
    final int timestamp = await _channel
        .invokeMethod('getTotalDurationAudio', {'soundID': soundID});

    return timestamp;
  }

  ///根据 soundID 获取对应的状态
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@return 当前的播放状态，true 正在播放，false 不在播放
  static Future<bool> isPlaying(int soundID) async {
    final bool ret =
        await _channel.invokeMethod('isPlaying', {'soundID': soundID});
    return ret;
  }

  ///获取音效文件当前播放进度
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@return timestamp 当前播放进度，单位毫秒；获取失败则返回 -1
  static Future<int> getCurrentDuration(int soundID) async {
    final int timestamp = await _channel
        .invokeMethod('getCurrentDurationAudio', {'soundID': soundID});

    return timestamp;
  }

  ///设置回调对象
  ///
  ///@param onAudioPlayEnd 设置接收 音效播放完成 回调，参考 [_onAudioPlayEnd] 定义
  ///@discussion 开发者只有调用本 API 设置回调对象才能收到相关回调
  static Future<void> registerAudioPlayerCallback(
      Function(int soundID) onAudioPlayEnd,
      Function(int soundID, int errorCode) onAudioLoad,
      Function(int soundID, int errorCode) onAudioPlayBegin,
      Function(int soundID) onAudioLoadComplete) async {
    await _channel.invokeMethod('registerAudioPlayerCallback');
    _onAudioLoad = onAudioLoad;
    _onAudioLoadComplete = onAudioLoadComplete;
    _onAudioPlayBegin = onAudioPlayBegin;
    _onAudioPlayEnd = onAudioPlayEnd;

    if (_streamSubscription == null)
      _streamSubscription = ZegoLiveRoomEventChannel.listenAudioPlayerEvent()
          .listen(_eventListener);
  }

  ///销毁回调对象
  ///
  ///@discussion 当开发者不再需要接收回调时，必须显式调用本 API 销毁回调对象
  static Future<void> unregisterAudioPlayerCallback() async {
    await _channel.invokeMethod('unregisterAudioPlayerCallback');

    _onAudioLoad = null;
    _onAudioLoadComplete = null;
    _onAudioPlayBegin = null;
    _onAudioPlayEnd = null;

    _streamSubscription?.cancel();
    _streamSubscription = null;
  }

  /// 播放完成事件回调
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频
  ///@discussion 当音频播放完毕后，会收到此回调
  ///@discussion 开发者必须调用 [registerAudioPlayerCallback] 且设置 onAudioPlayEnd 对象参数之后才能收到该回调
  static void Function(int soundID) _onAudioPlayEnd;

  static void Function(int soundID, int errorCode) _onAudioPlayBegin;

  static void Function(int soundID, int errorCode) _onAudioLoad;

  static void Function(int soundID) _onAudioLoadComplete;

  /// 用于接收native层事件流，开发者无需关注
  static StreamSubscription<dynamic> _streamSubscription;

  /// 用于处理native层事件流，开发者无需关注
  static void _eventListener(dynamic data) {
    final Map<dynamic, dynamic> args = data;
    switch (args['name']) {
      case 'onAudioPlayEnd':
        if (_onAudioPlayEnd != null) {
          int soundID = args['soundID'];

          _onAudioPlayEnd(soundID);
        }
        break;
      case 'onAudioPlayBegin':
        if (_onAudioPlayBegin != null) {
          int soundID = args['soundID'];
          int errorCode = args['errorCode'];

          _onAudioPlayBegin(soundID, errorCode);
        }
        break;
      case 'onAudioLoad':
        if (_onAudioLoad != null) {
          int soundID = args['soundID'];
          int errorCode = args['errorCode'];

          _onAudioLoad(soundID, errorCode);
        }
        break;
      case 'onAudioLoadComplete':
        if (_onAudioLoadComplete != null) {
          int soundID = args['soundID'];

          _onAudioLoadComplete(soundID);
        }
        break;
      default:
        break;
    }
  }
}
