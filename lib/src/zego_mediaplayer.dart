import 'dart:async';
import 'package:flutter/services.dart';
import 'zego_liveroom_event_channel.dart';
import 'zego_api_defines.dart';
import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart';

class ZegoMediaplayer {
  /* Method Channel */
  static const MethodChannel _channel =
      const MethodChannel('plugins.zego.im/zegoliveroom_plugin');

  ///开始播放媒体文件
  ///
  ///@param path 文件路径
  ///@param isRepeat 是否循环播放
  ///@param isAsset 是否为 flutter 的 asset 资源
  static Future<void> start(String path,int playerIndex,
      {bool isRepeat = false, bool isAsset = true}) async {
    return await _channel.invokeMethod(
        'mpStart', {'path': path, 'repeat': isRepeat, 'asset': isAsset,'playerIndex': playerIndex});
  }

  ///创建媒体播放器渲染器
  ///
  ///@param streamID，流ID，用于标识渲染器
  ///@param width，渲染宽度，单位 px (px = viewWidth * devicePixelRatio)
  ///@param height，渲染高度，单位 px (px = viewHeight * devicePixelRatio)
  ///@return textureID, flutter 渲染的纹理ID，非负数。当返回负数代表创建渲染器失败
  ///@discussion 当接口返回 TextureID 后，可通过 [Texture] 控件展示
  ///@discussion 当看到画面带有明显锯齿感时，请检查是否传入的宽高乘上了 devicePixelRatio, 参考[MediaQuery.of(context).devicePixelRatio]
  static Future<int> createMediaPlayerRenderer(int width, int height,int playerIndex) async {
    final int textureID = await _channel.invokeMethod(
        'createMediaPlayerRenderer', {'width': width, 'height': height,'playerIndex':playerIndex});

    return textureID;
  }

  ///更新媒体渲染器的渲染大小
  ///
  ///@param width，渲染宽度，单位 px (px = viewWidth * devicePixelRatio)
  ///@param height，渲染高度，单位 px (px = viewHeight * devicePixelRatio)
  ///@return true 成功，false 失败
  ///@discussion 当需要更新 Texture 渲染控件的大小时，调用本API同步更新渲染器的渲染大小，否则可能会导致图像变形等问题
  ///@discussion 当看到画面带有明显锯齿感时，请检查是否传入的宽高乘上了 devicePixelRatio, 参考[MediaQuery.of(context).devicePixelRatio]
  static Future<void> updateMediaPlayRenderSize(int playerIndex,int width, int height) async {
    return await _channel.invokeMethod(
        'updateMediaPlayRenderSize', {'width': width, 'height': height,'playerIndex': playerIndex});
  }
  

  ///销毁媒体渲染器
  ///
  ///@return true 成功，false 失败
  ///@discussion 释放渲染器资源
  static Future<bool> destroyMediaPlayerRenderer(int playerIndex) async {
    final bool success =
        await _channel.invokeMethod('destroyMediaPlayerRenderer',{'playerIndex': playerIndex});

    return success;
  }

  ///创建拉流 Platform View ，iOS 平台为 UIView，Android 平台 为 SurfaceView
  ///
  ///@param View创建后的回调，viewID 为 Platform View 的唯一标识，请开发者自行管理
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 true 时，调用该API有效，否则会返回错误
  static Widget createMediaPlayerPlatformView(int playerIndex, Function(int viewID) onViewCreated) {
    if(TargetPlatform.iOS == defaultTargetPlatform) {

      return UiKitView(
          key: new ObjectKey(playerIndex),
          viewType: 'plugins.zego.im/zego_view',
          onPlatformViewCreated: (int viewID) {
            if(onViewCreated != null)
              onViewCreated(viewID);
          }
      );
    } else if(TargetPlatform.android == defaultTargetPlatform) {

      return AndroidView(
        key: new ObjectKey(playerIndex),
        viewType: 'plugins.zego.im/zego_view',
        onPlatformViewCreated: (int viewID) {
          if(onViewCreated != null)
            onViewCreated(viewID);
        },
      );
    }

    return null;
  }

  ///取消绑定native的媒体播放器实例的viewID
  ///@param playerIndex 媒体播放器实例索引 ，该参数仅能传入实例索引
  ///@param viewID，调用 [createMediaPlayerPlatformView] 之后返回的 Platform View 的唯一标识，仅在使用 Platform View 渲染时才需要传入该参数
  ///@return 成功，false 失败
  static Future<bool> destroyMediaplayerPlatformView(int playerIndex) async{
    final bool success =
        await _channel.invokeMethod('destroyMediaPlayerPlatformView',{'playerIndex': playerIndex});

    return success;
  }

  ///根据viewID，映射到native渲染视频
  ///
  ///@param playerIndex 媒体播放器实例索引 ，该参数仅能传入实例索引
  ///@param viewID，调用 [createMediaPlayerPlatformView] 之后返回的 Platform View 的唯一标识，仅在使用 Platform View 渲染时才需要传入该参数
  ///@return 成功，false 失败
  static Future<bool> startRenderPlatformView(int playerIndex, {@required int viewID}) async {
    print('[Flutter-Dart] startPlayingStream, playerIndex: $playerIndex');
    final bool success = await _channel.invokeMethod('createMediaPlayerPlatformView', {
      'playerIndex': playerIndex,
      'viewID': viewID,
    });
    
    return success;
  }
  

  ///停止播放媒体文件
  ///
  static Future<void> stop(int playerIndex) async {
    return await _channel.invokeMethod('mpStop',{'playerIndex': playerIndex});
  }

  ///暂停播放
  ///
  static Future<void> pause(int playerIndex) async {
    return await _channel.invokeMethod('mpPause',{'playerIndex': playerIndex});
  }

  ///恢复播放音效
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@discussion 当开发者调用了 [pause] 暂停播放音效后，通过调用本接口进行恢复
  static Future<void> resume(int playerIndex) async {
    return await _channel.invokeMethod('mpResume',{'playerIndex': playerIndex});
  }

  ///预加载媒体文件
  ///
  ///@param path 本地路径，支持 flutter asset 资源路径和绝对路径
  static Future<void> preload(String path,int playerIndex,{bool isAsset = true}) async {
    return await _channel
        .invokeMethod('mpLoad', {'path': path, 'asset': isAsset,'playerIndex': playerIndex});
  }

  ///获取播放器当前状态
  ///
  ///@return true 正在播放，false 不在播放
  static Future<bool> isPlaying(int playerIndex) async {
    return await _channel.invokeMethod('mpIsPlaying',{'playerIndex': playerIndex});
  }

  ///设置音量，包括本地播放音量和推流音量
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@param volume 音量，取值范围为 0~100，默认为 50
  static Future<void> setVolume(int volume,int playerIndex) async {
    return await _channel.invokeMethod('mpSetVolume', {'volume': volume,'playerIndex': playerIndex});
  }

  ///跳转至指定的播放进度
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@param timestamp 需要跳转的进度，单位毫秒
  static Future<ZegoMediaplayerSeekResult> seekTo(int timestamp,int playerIndex) async {
    final Map<dynamic, dynamic> ret =
        await _channel.invokeMethod('mpSeekTo', {'timestamp': timestamp,'playerIndex': playerIndex});

    int errorCode = ret['errorCode'];
    int ts = ret['timestamp'];
    int index = ret['playerIndex'];
    return ZegoMediaplayerSeekResult(errorCode, ts);
  }

  ///获取音效文件总时长
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@return timestamp 总时长，单位毫秒；获取失败则返回 0
  static Future<int> getTotalDuration(int playerIndex) async {
    final int timestamp = await _channel.invokeMethod('mpGetDuration',{'playerIndex': playerIndex});

    return timestamp;
  }

  ///获取音效文件当前播放进度
  ///
  ///@param soundID 音效ID，标识当前播放的是哪个音频。取值为非负数
  ///@return timestamp 当前播放进度，单位毫秒；获取失败则返回 0
  static Future<int> getCurrentDuration(int playerIndex) async {
    final int timestamp = await _channel.invokeMethod('mpGetCurrentDuration',{'playerIndex': playerIndex});

    return timestamp;
  }

  ///本地静默播放
  ///
  ///@param mute 是否需要静音本地播放
  static Future<void> muteLocal(bool mute,int playerIndex) async {
    return await _channel.invokeMethod('mpMuteLocal', {'mute': mute,'playerIndex': playerIndex});
  }

  /// 设置播放类型
  ///
  /// @param type ，参考[]
  static Future<void> setPlayerType(int type,int playerIndex) async {
    return await _channel.invokeMethod('mpSetPlayerType', {'type': type,'playerIndex': playerIndex});
  }

  static Future<void> setProcessInterval(int interval,int playerIndex) async {
    return await _channel
        .invokeMethod('mpSetProcessInterval', {'interval': interval,'playerIndex': playerIndex});
  }

  static Future<void> setOnlineResourceCache(int time, int size,int playerIndex) async {
    return await _channel
        .invokeMethod('setOnlineResourceCache', {'time': time, 'size': size,'playerIndex': playerIndex});
  }

  static Future<ZegoMediaplayerCacheResult> getOnlineResourceCache(int playerIndex) async {
    final Map<dynamic, dynamic> map =
        await _channel.invokeMethod('getOnlineResourceCache',{'playerIndex': playerIndex});

    ZegoMediaplayerCacheResult result =
        new ZegoMediaplayerCacheResult(map['time'], map['size']);
    return result;
  }

  static Future<void> setBufferThreshold(int threshold,int playerIndex) async {
    return await _channel
        .invokeMethod('setBufferThreshold', {'threshold': threshold,'playerIndex': playerIndex});
  }

  static Future<void> setLoadResourceTimeout(int timeout,int playerIndex) async {
    return await _channel
        .invokeMethod('setLoadResourceTimeout', {'timeout': timeout,'playerIndex': playerIndex});
  }

  ///设置播放文件的音轨
  ///
  ///@param streamIndex 音轨序号，可以通过 getAudioStreamCount 接口获取音轨个数
  static Future<void> setAudioStream(int streamIndex,int playerIndex) async {
    return await _channel
        .invokeMethod('mpSetAudioStream', {'streamIndex': streamIndex,'playerIndex': playerIndex});
  }

  ///获取音轨个数
  ///
  ///@return 音轨个数
  static Future<int> getAudioStreamCount(int playerIndex) async {
    int count = await _channel.invokeMethod('mpGetAudioStreamCount',{'playerIndex': playerIndex});
    return count;
  }

  ///获取推流音量
  ///
  ///@return 推流音量
  static Future<int> getPublishVolume(int playerIndex) async {
    int count = await _channel.invokeMethod('mpGetPublishVolume',{'playerIndex': playerIndex});
    return count;
  }

  ///获取本地播放音量
  ///
  ///@return 本地播放音量
  static Future<int> getPlayVolume(int playerIndex) async {
    int count = await _channel.invokeMethod('mpGetPlayVolume',{'playerIndex': playerIndex});
    return count;
  }

  ///设置推流音量
  ///
  ///@param volume 音量，取值范围[0, 200]，默认 60
  static Future<void> setPublishVolume(int volume,int playerIndex) async {
    return await _channel
        .invokeMethod('mpSetPublishVolume', {'volume': volume,'playerIndex': playerIndex});
  }

  ///设置本地播放音量
  ///
  ///@param volume 音量，取值范围[0, 200]，默认 60
  static Future<void> setPlayVolume(int volume,int playerIndex) async {
    return await _channel.invokeMethod('mpSetPlayVolume', {'volume': volume,'playerIndex': playerIndex});
  }

  ///设置回调对象
  ///
  ///@param onAudioPlayEnd 设置接收 音效播放完成 回调，参考 [_onAudioPlayEnd] 定义
  ///@discussion 开发者只有调用本 API 设置回调对象才能收到相关回调
  static Future<void> registerMediaPlayerCallback(
      {
        Function(int playerIndex) onPlayBegin,
        Function(int playerIndex) onPlayPause,
        Function(int playerIndex) onPlayResume,
        Function(int playerIndex) onPlayEnd,
        Function(int playerIndex) onPlayStop,
        Function(int errorCode,int playerIndex) onPlayError,
        Function(int playerIndex) onBufferBegin,
        Function(int playerIndex) onBufferEnd,
        Function(int playerIndex) onAudioBegin,
        Function(int playerIndex) onVideoBegin,
        Function(int errorCode, int millisecond,int playerIndex) onSeekComplete,
        Function(int timestamp,int playerIndex) onProcessInterval
      }
      ) async {
    await _channel.invokeMethod('registerMediaPlayerCallback');

    _onPlayBegin = onPlayBegin;
    _onPlayPause = onPlayPause;
    _onPlayResume = onPlayResume;
    _onPlayEnd = onPlayEnd;
    _onPlayStop = onPlayStop;
    _onPlayError = onPlayError;
    _onBufferBegin = onBufferBegin;
    _onBufferEnd = onBufferEnd;
    _onAudioBegin = onAudioBegin;
    _onVideoBegin = onVideoBegin;
    _onProcessInterval = onProcessInterval;
    _onSeekComplete = onSeekComplete;

    if (_streamSubscription == null)
      _streamSubscription = ZegoLiveRoomEventChannel.listenMediaPlayerEvent()
          .listen(_eventListener);
  }

  ///销毁回调对象
  ///
  ///@discussion 当开发者不再需要接收回调时，必须显式调用本 API 销毁回调对象
  static Future<void> unregisterMediaPlayerCallback() async {
    await _channel.invokeMethod('unregisterMediaPlayerCallback');

    _onPlayBegin = null;
    _onPlayPause = null;
    _onPlayResume = null;
    _onPlayEnd = null;
    _onPlayStop = null;
    _onPlayError = null;
    _onBufferBegin = null;
    _onBufferEnd = null;
    _onAudioBegin = null;
    _onVideoBegin = null;
    _onProcessInterval = null;
    _onSeekComplete = null;

    _streamSubscription?.cancel();
    _streamSubscription = null;
  }

  /// 播放完成事件回调
  ///
  ///@discussion 当音频播放完毕后，会收到此回调
  ///@discussion 开发者必须调用 [registerMediaPlayerCallback] 且设置 onAudioPlayEnd 对象参数之后才能收到该回调
  static void Function(int playerIndex) _onPlayBegin;

  static void Function(int playerIndex) _onPlayPause;

  static void Function(int playerIndex) _onPlayResume;

  static void Function(int playerIndex) _onPlayEnd;

  static void Function(int playerIndex) _onPlayStop;

  static void Function(int errorCode,int playerIndex) _onPlayError;

  static void Function(int playerIndex) _onBufferBegin;

  static void Function(int playerIndex) _onBufferEnd;

  static void Function(int errorCode, int millisecond,int playerIndex) _onSeekComplete;

  static void Function(int playerIndex) _onAudioBegin;

  static void Function(int playerIndex) _onVideoBegin;

  static void Function(int timestamp,int playerIndex) _onProcessInterval;

  /// 用于接收native层事件流，开发者无需关注
  static StreamSubscription<dynamic> _streamSubscription;

  /// 用于处理native层事件流，开发者无需关注
  static void _eventListener(dynamic data) {
    final Map<dynamic, dynamic> args = data;
    switch (args['name']) {
      case 'onPlayBegin':
        if (_onPlayBegin != null) {
          int playerIndex = args['playerIndex'];
          _onPlayBegin(playerIndex);
        }
        break;
      case 'onPlayPause':
        if (_onPlayPause != null) {
          int playerIndex = args['playerIndex'];
          _onPlayPause(playerIndex);
        }
        break;
      case 'onPlayResume':
        if (_onPlayResume != null) {
          int playerIndex = args['playerIndex'];
          _onPlayResume(playerIndex);
        }
        break;
      case 'onPlayEnd':
        if (_onPlayEnd != null) {
          int playerIndex = args['playerIndex'];
          _onPlayEnd(playerIndex);
        }
        break;
      case 'onPlayStop':
        if (_onPlayStop != null) {
          int playerIndex = args['playerIndex'];
          _onPlayStop(playerIndex);
        }
        break;
      case 'onPlayError':
        if (_onPlayError != null) {
          int error = args['errorCode'];
          int playerIndex = args['playerIndex'];
          _onPlayError(error,playerIndex);
        }
        break;
      case 'onBufferBegin':
        if (_onBufferBegin != null) {
          int playerIndex = args['playerIndex'];
          _onBufferBegin(playerIndex);
        }
        break;
      case 'onBufferEnd':
        if (_onBufferEnd != null) {
          int playerIndex = args['playerIndex'];
          _onBufferEnd(playerIndex);
        }
        break;
      case 'onSeekComplete':
        if (_onSeekComplete != null) {
          int errCode = args['errorCode'];
          int millisecond = args['millisecond'];
          int playerIndex = args['playerIndex'];
          _onSeekComplete(errCode, millisecond,playerIndex);
        }
        break;
      case 'onAudioBegin':
        if (_onAudioBegin != null) {
          int playerIndex = args['playerIndex'];
          _onAudioBegin(playerIndex);
        }
        break;
      case 'onVideoBegin':
        if (_onVideoBegin != null) {
          int playerIndex = args['playerIndex'];
          _onVideoBegin(playerIndex);
        }
        break;
      case 'onProcessInterval':
        if (_onProcessInterval != null) {
          int timestamp = args['timestamp'];
          int playerIndex = args['playerIndex'];
          _onProcessInterval(timestamp,playerIndex);
        }
        break;
      default:
        break;
    }
  }
}
