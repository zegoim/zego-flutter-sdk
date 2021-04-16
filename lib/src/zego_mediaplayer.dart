import 'dart:async';
import 'package:flutter/services.dart';
import 'zego_liveroom_event_channel.dart';
import 'zego_api_defines.dart';

class ZegoMediaplayer {
  /* Method Channel */
  static const MethodChannel _channel =
      const MethodChannel('plugins.zego.im/zegoliveroom_plugin');

  ///开始播放媒体文件
  ///
  ///@param path 文件路径
  ///@param isRepeat 是否循环播放
  ///@param isAsset 是否为 flutter 的 asset 资源
  static Future<void> start(String path,
      {bool isRepeat = false, bool isAsset = true}) async {
    return await _channel.invokeMethod(
        'mpStart', {'path': path, 'repeat': isRepeat, 'asset': isAsset});
  }

  ///创建媒体播放器渲染器
  ///
  ///@param streamID，流ID，用于标识渲染器
  ///@param width，渲染宽度，单位 px (px = viewWidth * devicePixelRatio)
  ///@param height，渲染高度，单位 px (px = viewHeight * devicePixelRatio)
  ///@return textureID, flutter 渲染的纹理ID，非负数。当返回负数代表创建渲染器失败
  ///@discussion 当接口返回 TextureID 后，可通过 [Texture] 控件展示
  ///@discussion 当看到画面带有明显锯齿感时，请检查是否传入的宽高乘上了 devicePixelRatio, 参考[MediaQuery.of(context).devicePixelRatio]
  static Future<int> createMediaPlayerRenderer(int width, int height) async {
    final int textureID = await _channel.invokeMethod(
        'createMediaPlayerRenderer', {'width': width, 'height': height});

    return textureID;
  }

  ///更新媒体渲染器的渲染大小
  ///
  ///@param width，渲染宽度，单位 px (px = viewWidth * devicePixelRatio)
  ///@param height，渲染高度，单位 px (px = viewHeight * devicePixelRatio)
  ///@return true 成功，false 失败
  ///@discussion 当需要更新 Texture 渲染控件的大小时，调用本API同步更新渲染器的渲染大小，否则可能会导致图像变形等问题
  ///@discussion 当看到画面带有明显锯齿感时，请检查是否传入的宽高乘上了 devicePixelRatio, 参考[MediaQuery.of(context).devicePixelRatio]
  static Future<void> updateMediaPlayRenderSize(int width, int height) async {
    return await _channel.invokeMethod(
        'updateMediaPlayRenderSize', {'width': width, 'height': height});
  }

  ///销毁媒体渲染器
  ///
  ///@return true 成功，false 失败
  ///@discussion 释放渲染器资源
  static Future<bool> destroyMediaPlayerRenderer() async {
    final bool success =
        await _channel.invokeMethod('destroyMediaPlayerRenderer');

    return success;
  }

  ///停止播放媒体文件
  ///
  static Future<void> stop() async {
    return await _channel.invokeMethod('mpStop');
  }

  ///暂停播放
  ///
  static Future<void> pause() async {
    return await _channel.invokeMethod('mpPause');
  }

  ///恢复播放音效
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@discussion 当开发者调用了 [pause] 暂停播放音效后，通过调用本接口进行恢复
  static Future<void> resume() async {
    return await _channel.invokeMethod('mpResume');
  }

  ///预加载媒体文件
  ///
  ///@param path 本地路径，支持 flutter asset 资源路径和绝对路径
  static Future<void> preload(String path, {bool isAsset = true}) async {
    return await _channel
        .invokeMethod('mpLoad', {'path': path, 'asset': isAsset});
  }

  ///设置音量，包括本地播放音量和推流音量
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@param volume 音量，取值范围为 0~100，默认为 50
  static Future<void> setVolume(int volume) async {
    return await _channel.invokeMethod('mpSetVolume', {'volume': volume});
  }

  ///跳转至指定的播放进度
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@param timestamp 需要跳转的进度，单位毫秒
  static Future<ZegoMediaplayerSeekResult> seekTo(int timestamp) async {
    final Map<dynamic, dynamic> ret =
        await _channel.invokeMethod('mpSeekTo', {'timestamp': timestamp});

    int errorCode = ret['errorCode'];
    int ts = ret['timestamp'];
    return ZegoMediaplayerSeekResult(errorCode, ts);
  }

  ///获取音效文件总时长
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@return timestamp 总时长，单位毫秒；获取失败则返回 0
  static Future<int> getTotalDuration() async {
    final int timestamp = await _channel.invokeMethod('mpGetDuration');

    return timestamp;
  }

  ///获取音效文件当前播放进度
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@return timestamp 当前播放进度，单位毫秒；获取失败则返回 0
  static Future<int> getCurrentDuration() async {
    final int timestamp = await _channel.invokeMethod('mpGetCurrentDuration');

    return timestamp;
  }

  ///本地静默播放
  ///
  ///@param mute 是否需要静音本地播放
  static Future<void> muteLocal(bool mute) async {
    return await _channel.invokeMethod('mpMuteLocal', {'mute': mute});
  }

  /// 设置播放类型
  ///
  /// @param type ，参考[]
  static Future<void> setPlayerType(int type) async {
    return await _channel.invokeMethod('mpSetPlayerType', {'type': type});
  }

  static Future<void> setProcessInterval(int interval) async {
    return await _channel
        .invokeMethod('mpSetProcessInterval', {'interval': interval});
  }

  static Future<void> setOnlineResourceCache(int time, int size) async {
    return await _channel
        .invokeMethod('setOnlineResourceCache', {'time': time, 'size': size});
  }

  static Future<ZegoMediaplayerCacheResult> getOnlineResourceCache() async {
    final Map<dynamic, dynamic> map =
        await _channel.invokeMethod('getOnlineResourceCache');

    ZegoMediaplayerCacheResult result =
        new ZegoMediaplayerCacheResult(map['time'], map['size']);
    return result;
  }

  static Future<void> setBufferThreshold(int threshold) async {
    return await _channel
        .invokeMethod('setBufferThreshold', {'threshold': threshold});
  }

  static Future<void> setLoadResourceTimeout(int timeout) async {
    return await _channel
        .invokeMethod('setLoadResourceTimeout', {'timeout': timeout});
  }

  ///设置播放文件的音轨
  ///
  ///@param streamIndex 音轨序号，可以通过 getAudioStreamCount 接口获取音轨个数
  static Future<void> setAudioStream(int streamIndex) async {
   return await _channel
        .invokeMethod('mpSetAudioStream', {'streamIndex': streamIndex});

  }

  ///获取音轨个数
  ///
  ///@return 音轨个数
  static Future<int> getAudioStreamCount() async {
    int count = await _channel.invokeMethod('mpGetAudioStreamCount');
    return count;
  }

  ///获取推流音量
  ///
  ///@return 推流音量
  static Future<int> getPublishVolume() async {
    int count = await _channel.invokeMethod('mpGetPublishVolume');
    return count;
  }

  ///获取本地播放音量
  ///
  ///@return 本地播放音量
  static Future<int> getPlayVolume() async {
    int count = await _channel.invokeMethod('mpGetPlayVolume');
    return count;
  }

  ///设置推流音量
  ///
  ///@param volume 音量，取值范围[0, 200]，默认 60
  static Future<void> setPublishVolume(int volume) async {
    return await _channel
        .invokeMethod('mpSetPublishVolume', {'volume': volume});
  }

  ///设置本地播放音量
  ///
  ///@param volume 音量，取值范围[0, 200]，默认 60
  static Future<void> setPlayVolume(int volume) async {
    return await _channel.invokeMethod('mpSetPlayVolume', {'volume': volume});
  }

  ///设置回调对象
  ///
  ///@param onAudioPlayEnd 设置接收 音效播放完成 回调，参考 [_onAudioPlayEnd] 定义
  ///@discussion 开发者只有调用本 API 设置回调对象才能收到相关回调
  static Future<void> registerMediaPlayerCallback(
      {Function() onPlayEnd,
      Function(int errorCode) onPlayError,
      Function() onBufferBegin,
      Function() onBufferEnd,
      Function(int timestamp) onProcessInterval}) async {
    await _channel.invokeMethod('registerMediaPlayerCallback');

    _onPlayEnd = onPlayEnd;
    _onPlayError = onPlayError;
    _onBufferBegin = onBufferBegin;
    _onBufferEnd = onBufferEnd;
    _onProcessInterval = onProcessInterval;

    if (_streamSubscription == null)
      _streamSubscription = ZegoLiveRoomEventChannel.listenMediaPlayerEvent()
          .listen(_eventListener);
  }

  ///销毁回调对象
  ///
  ///@discussion 当开发者不再需要接收回调时，必须显式调用本 API 销毁回调对象
  static Future<void> unregisterMediaPlayerCallback() async {
    await _channel.invokeMethod('unregisterMediaPlayerCallback');

    _onPlayEnd = null;
    _onPlayError = null;
    _onBufferBegin = null;
    _onBufferEnd = null;
    _onProcessInterval = null;

    _streamSubscription?.cancel();
    _streamSubscription = null;
  }

  /// 播放完成事件回调
  ///
  ///@discussion 当音频播放完毕后，会收到此回调
  ///@discussion 开发者必须调用 [registerMediaPlayerCallback] 且设置 onAudioPlayEnd 对象参数之后才能收到该回调
  static void Function() _onPlayEnd;

  static void Function(int errorCode) _onPlayError;

  static void Function() _onBufferBegin;

  static void Function() _onBufferEnd;

  static void Function(int timestamp) _onProcessInterval;

  /// 用于接收native层事件流，开发者无需关注
  static StreamSubscription<dynamic> _streamSubscription;

  /// 用于处理native层事件流，开发者无需关注
  static void _eventListener(dynamic data) {
    final Map<dynamic, dynamic> args = data;
    switch (args['name']) {
      case 'onPlayEnd':
        if (_onPlayEnd != null) {
          _onPlayEnd();
        }
        break;
      case 'onPlayError':
        if (_onPlayError != null) {
          int error = args['errorCode'];
          _onPlayError(error);
        }
        break;
      case 'onBufferBegin':
        if (_onBufferBegin != null) {
          _onBufferBegin();
        }
        break;
      case 'onBufferEnd':
        if (_onBufferEnd != null) {
          _onBufferEnd();
        }
        break;
      case 'onProcessInterval':
        if (_onProcessInterval != null) {
          int timestamp = args['timestamp'];
          _onProcessInterval(timestamp);
        }
        break;
      default:
        break;
    }
  }
}
