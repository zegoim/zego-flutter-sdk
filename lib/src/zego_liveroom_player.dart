import 'dart:async';
import 'dart:typed_data';
import 'package:flutter/services.dart';
import 'zego_api_defines.dart';
import 'zego_liveroom_event_channel.dart';
import 'zego_liveroom.dart';
import 'zego_api_error_code.dart';
import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart';

class ZegoLiveRoomPlayerPlugin {
  /* Method Channel */
  static const MethodChannel _channel =
  const MethodChannel('plugins.zego.im/zegoliveroom_plugin');

  ///创建拉流渲染器
  ///
  ///@param streamID，流ID，用于标识渲染器
  ///@param width，渲染宽度，单位 px (px = viewWidth * devicePixelRatio)
  ///@param height，渲染高度，单位 px (px = viewHeight * devicePixelRatio)
  ///@return textureID, flutter 渲染的纹理ID，非负数。当返回负数代表创建渲染器失败
  ///@discussion 当接口返回 TextureID 后，可通过 [Texture] 控件展示
  ///@discussion 当看到画面带有明显锯齿感时，请检查是否传入的宽高乘上了 devicePixelRatio, 参考[MediaQuery.of(context).devicePixelRatio]
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 false 时，调用该API有效，否则会返回错误
  static Future<int> createPlayViewRenderer(String streamID, int width, int height) async {
    final int textureID = await _channel.invokeMethod('createPlayViewRenderer', {
      'streamID': streamID,
      'width': width,
      'height': height
    });

    return textureID;
  }

  ///设置观看直播的View的模式
  ///
  ///@param streamID 播放流 ID
  ///@param mode 模式，参考 [ZegoViewMode] 定义。默认 [ZegoViewMode.ZegoRendererScaleAspectFit]
  ///@return true 成功，false 失败
  ///@note 必须在拉流后调用才有效
  ///@discussion 一般在流播放、流新增、全屏切换等其他流尺寸可能变化的场合时调用
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 false 时，调用该API有效，否则会返回错误
  static Future<bool> setViewMode(String streamID, int mode) async {
    final bool success = await _channel.invokeMethod('setViewMode', {
      'streamID': streamID,
      'mode': mode
    });

    return success;
  }

  ///更新拉流渲染器的渲染大小
  ///
  ///@param width，渲染宽度，单位 px (px = viewWidth * devicePixelRatio)
  ///@param height，渲染高度，单位 px (px = viewHeight * devicePixelRatio)
  ///@return true 成功，false 失败
  ///@discussion 当需要更新 Texture 渲染控件的大小时，调用本API同步更新渲染器的渲染大小，否则可能会导致图像变形等问题
  ///@discussion 当看到画面带有明显锯齿感时，请检查是否传入的宽高乘上了 devicePixelRatio, 参考[MediaQuery.of(context).devicePixelRatio]
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 false 时，调用该API有效，否则会返回错误
  static Future<void> updatePlayViewRenderSize(String streamID, int width, int height) async {
    return await _channel.invokeMethod('updatePlayViewRenderSize', {
      'streamID': streamID,
      'width': width,
      'height': height
    });
  }

  ///销毁拉流渲染器
  ///
  ///@return true 成功，false 失败
  ///@discussion 释放渲染器资源
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 false 时，调用该API有效，否则会返回错误
  static Future<bool> destroyPlayViewRenderer(String streamID) async {
    final bool success = await _channel.invokeMethod('destroyPlayViewRenderer', {
      'streamID': streamID
    });

    return success;
  }

  ///创建拉流 Platform View ，iOS 平台为 UIView，Android 平台 为 SurfaceView
  ///
  ///@param View创建后的回调，viewID 为 Platform View 的唯一标识，请开发者自行管理
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 true 时，调用该API有效，否则会返回错误
  static Widget createPlayPlatformView(String streamID, Function(int viewID) onViewCreated) {
    if(TargetPlatform.iOS == defaultTargetPlatform) {

      return UiKitView(
          key: new ObjectKey(streamID),
          viewType: 'plugins.zego.im/zego_view',
          onPlatformViewCreated: (int viewID) {
            if(onViewCreated != null)
              onViewCreated(viewID);
          }
      );
    } else if(TargetPlatform.android == defaultTargetPlatform) {

      return AndroidView(
        key: new ObjectKey(streamID),
        viewType: 'plugins.zego.im/zego_view',
        onPlatformViewCreated: (int viewID) {
          if(onViewCreated != null)
            onViewCreated(viewID);
        },
      );
    }

    return null;
  }

  ///更新播放视图
  ///
  ///@param viewID，调用 [createPlayPlatformView] 之后返回的 Platform View 的唯一标识
  ///@param streamID，流 ID
  ///@discussion 调用 [startPlayingStream] 播放流成功以后，如果要切换流播放 View 或者停止显示流画面，调用该 API 变更
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 true 时，调用该API有效，否则会返回错误
  static Future<void> updatePlayView(int viewID, String streamID) async {
    return await _channel.invokeMethod('updatePlayView', {
      'viewID': viewID,
      'streamID': streamID
    });
  }

  ///设置拉流 Platform View 视频视图的模式
  ///
  ///@param streamID，流 ID
  ///@param mode 模式，参考 [ZegoViewMode] 定义。默认 [ZegoViewMode.ZegoVideoViewModeScaleAspectFit]
  ///@return true 成功，false 失败
  ///@discussion [startPlayingStream] 之后调用本 API 进行参数配置
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 true 时，调用该API有效，否则会返回错误
  static Future<bool> setPlatformViewPlayViewMode(String streamID, int mode) async {
    final bool success = await _channel.invokeMethod('setPlatformViewPlayViewMode', {
      'streamID': streamID,
      'mode': mode
    });

    return success;
  }

  ///销毁拉流 Platform View
  ///
  ///@param viewID，调用 [createPlayPlatformView] 之后返回的 Platform View 的唯一标识
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 true 时，调用该API有效，否则会返回错误
  static Future<bool> removePlayPlatformView(int viewID) async {
    final bool success = await _channel.invokeMethod('removePlayPlatformView', {
      'viewID': viewID
    });

    return success;
  }

  ///播放直播流
  ///
  ///@param streamID 流 ID，该参数仅能传入流 ID，不可在流 ID 后添加播放参数
  ///@param viewID，调用 [createPlayPlatformView] 之后返回的 Platform View 的唯一标识，仅在使用 Platform View 渲染时才需要传入该参数
  ///@param info 多媒体流附加信息，参考 [ZegoStreamExtraPlayInfo] 定义
  ///@return 成功，false 失败
  ///@discussion 播放直播流调用此 API。播放成功后，等待 onPlayStateUpdate 回调
  static Future<bool> startPlayingStream(String streamID, {int viewID, ZegoStreamExtraPlayInfo info}) async {
    _addRoomNoticeLog('[Flutter-Dart] startPlayingStream, streamID: $streamID');
    final bool success = await _channel.invokeMethod('startPlayingStream', {
      'streamID': streamID,
      'viewID': viewID,
      'info': info?.toMap(),
    });

    return success;
  }

  ///停止播放流
  ///
  ///@param streamID 流 ID
  ///@return true 成功，false 失败
  ///@discussion 当有主播停止流推后，会通过 onStreamUpdated 通知房间内用户流删除，用户需要调用此 API 停止播放流。
  static Future<bool> stopPlayingStream(String streamID) async {
    _addRoomNoticeLog('[Flutter-Dart] stopPlayingStream, streamID: $streamID');
    final bool success = await _channel.invokeMethod('stopPlayingStream', {
      'streamID': streamID
    });
    return success;
  }

  ///是否启用扬声器
  ///
  ///@param bEnable true 开启，false 关闭。默认 true
  ///@return true 成功，false 失败
  ///@discussion 设置为关闭后，内置扬声器和耳机均无声音输出
  static Future<bool> enableSpeaker(bool enable) async {
    final bool success = await _channel.invokeMethod('enableSpeaker', {
      'enable': enable
    });

    return success;
  }

  ///手机内置扬声器开关
  ///
  ///@param bOn true 打开，false 关闭。默认 true
  ///@return true 成功，false 失败
  ///@discussion 设置为关闭后，扬声器无声音，耳机仍有声音输出
  ///@discussion 在推流之前设置, 且 setAudioDeviceMode 设置为 [ZegoAudioDeviceMode.ZEGO_AUDIO_DEVICE_MODE_COMMUNICATION] 或 [ZegoAudioDeviceMode.ZEGO_AUDIO_DEVICE_MODE_COMMUNICATION2] 时有效
  static Future<bool> setBuiltInSpeakerOn(bool bOn) async {
    final bool success = await _channel.invokeMethod('setBuiltInSpeakerOn', {
      'bOn': bOn
    });

    return success;
  }

  ///开关硬件解码
  ///
  ///@param bRequire true 打开，false 关闭。默认 false
  ///@return true 成功，false 失败
  ///@discussion 如果要打开，需要在拉流前设置。打开硬编硬解开关需后台可控，避免碰到版本升级或者硬件升级时出现硬编硬解失败的问题
  static Future<bool> requireHardwareDecoder(bool bRequire) async {
    final bool success = await _channel.invokeMethod('requireHardwareDecoder', {
      'bRequire': bRequire
    });

    return success;
  }

  ///拉流是否接收音频数据
  ///
  ///@param streamID 播放流 ID
  ///@param active true 接收，false 不接收
  ///@return 0 成功，否则失败
  ///@discussion 仅拉 UDP 流有效，必须在拉流后调用才有效
  static Future<int> activateAudioPlayStream(String streamID, bool active) async {
    final int errorCode = await _channel.invokeMethod('activateAudioPlayStream', {
      'streamID': streamID,
      'active': active
    });

    return errorCode;
  }

  ///拉流是否接收视频数据
  ///
  ///@param streamID 播放流 ID
  ///@param active true 接收，false 不接收
  ///@param videoLayer 视频分层类型，参考 [ZegoVideoStreamLayer] 定义
  ///@return 0 成功，否则失败
  ///@discussion 仅拉 UDP 流有效，必须在拉流后调用才有效
  static Future<int> activateVideoPlayStream(String streamID, bool active, {int videoStreamLayer}) async {
    final int errorCode = await _channel.invokeMethod('activateVideoPlayStream', {
      'streamID': streamID,
      'active': active,
      'layer': videoStreamLayer
    });

    return errorCode;
  }

  ///请求连麦
  ///
  ///@return 连麦结果，连麦成功后，将会返回连麦结果，参考 [ZegoResponseResult] 定义
  ///@discussion 观众请求连麦后，主播会收到 onJoinLiveRequest 回调
  static Future<ZegoResponseResult> requestJoinLive() async {
    final Map<dynamic, dynamic> mapResult = await _channel.invokeMethod('requestJoinLive');

    ZegoResponseResult result = new ZegoResponseResult(
        mapResult['result'],
        mapResult['fromUserID'],
        mapResult['fromUserName']
    );

    return result;
  }

  ///回应主播端的邀请连麦请求
  ///
  ///@param seq 连麦请求序列号，标识当次连麦请求
  ///@param rspResult 回应，0 为同意
  ///@return true 成功，false 失败
  ///@discussion 一般在 onInviteJoinLiveRequest 中调用本 API 回应邀请连麦请求
  static Future<bool> respondInviteJoinLiveReq(int seq, int rspResult) async {
    final bool success = await _channel.invokeMethod('respondInviteJoinLiveReq', {
      'seq': seq,
      'rspResult': rspResult
    });

    return success;
  }

  /// Sets the stream playback volume.
  ///
  /// This function is used to set the playback volume of the stream. Need to be called after calling startPlayingStream.
  /// You need to reset after [stopPlayingStream] and [startPlayingStream].
  ///
  /// [volume] Volume percentage. The value ranges from 0 to 200, and the default value is 100.
  /// [streamID] Stream ID. Set volume for all streams playing by set streamID as nil or empty.
  static Future<bool> setPlayVolume(int volume, String streamID) async {
    return await _channel.invokeMethod('setPlayVolume', {
      'volume': volume,
      'streamID': streamID
    });
  }

  /// Take a snapshot of the playing stream.
  ///
  /// Please call this function after calling [startPlayingStream]
  /// If calling this function when not in playing stream, it will not return or may be return null.
  ///
  /// [streamID] Stream ID to be snapshot
  /// - Returns Results of take play stream snapshot, may be null, or it may not return at all
  static Future<MemoryImage> takePlayStreamSnapshot(String streamID) async {

    final Uint8List pngImageBytes = await _channel.invokeMethod('takePlayStreamSnapshot', {
      'streamID': streamID
    });

    if (pngImageBytes == null) {
      return null;
    }

    MemoryImage image = MemoryImage(pngImageBytes);
    return image;
  }

  ///设置回调对象
  ///
  ///@param onPlayStateUpdate 设置接收 播放流事件 回调，参考 [_onPlayStateUpdate] 定义
  ///@param onPlayQualityUpdate 设置接收 观看质量更新 回调，参考 [_onPlayQualityUpdate] 定义
  ///@param onVideoSizeChangedTo 设置接收 视频宽高变化通知 回调，参考 [_onVideoSizeChangedTo] 定义
  ///@param onInviteJoinLiveRequest 设置接收 主播端的邀请连麦请求 回调，参考 [_onInviteJoinLiveRequest] 定义
  ///@param onRecvEndJoinLiveCommad 设置接收 结束连麦信令 回调，参考 [_onRecvEndJoinLiveCommand] 定义
  ///@param onRemoteCameraStatusUpdate 设置接收 远端摄像头状态 回调，参考 [_onRemoteCameraStatusUpdate] 定义
  ///@param onRemoteMicStatusUpdate 设置接收 远端麦克风状态 回调，参考 [_onRemoteMicStatusUpdate] 定义
  ///@param onRecvRemoteAudioFirstFrame 设置接收 远端音频的首帧 回调，参考 [_onRecvRemoteAudioFirstFrame] 定义
  ///@param onRecvRemoteVideoFirstFrame 设置接收 远端视频的首帧 回调，参考 [_onRecvRemoteVideoFirstFrame] 定义
  ///@param onRenderRemoteVideoFirstFrame 设置接收 远端视频渲染首帧 回调，参考 [_onRenderRemoteVideoFirstFrame] 定义
  ///@discussion 开发者只有调用本 API 设置回调对象才能收到相关回调
  static void registerPlayerCallback({
    Function(int stateCode, String streamID) onPlayStateUpdate,
    Function(String streamID, ZegoPlayStreamQuality quality) onPlayQualityUpdate,
    Function(String streamID, int width, int height) onVideoSizeChangedTo,
    Function(int seq, String fromUserID, String fromUserName, String roomID) onInviteJoinLiveRequest,
    Function(String fromUserID, String fromUserName, String roomID) onRecvEndJoinLiveCommand,
    Function(int status, String streamID, int reason) onRemoteCameraStatusUpdate,
    Function(int status, String streamID, int reason) onRemoteMicStatusUpdate,
    Function(String streamID) onRecvRemoteAudioFirstFrame,
    Function(String streamID) onRecvRemoteVideoFirstFrame,
    Function(String streamID) onRenderRemoteVideoFirstFrame
  }) {

    _onPlayStateUpdate = onPlayStateUpdate;
    _onPlayQualityUpdate = onPlayQualityUpdate;
    _onVideoSizeChangedTo = onVideoSizeChangedTo;
    _onInviteJoinLiveRequest = onInviteJoinLiveRequest;
    _onRecvEndJoinLiveCommand = onRecvEndJoinLiveCommand;
    _onRemoteCameraStatusUpdate = onRemoteCameraStatusUpdate;
    _onRemoteMicStatusUpdate = onRemoteMicStatusUpdate;
    _onRecvRemoteAudioFirstFrame = onRecvRemoteAudioFirstFrame;
    _onRecvRemoteVideoFirstFrame = onRecvRemoteVideoFirstFrame;
    _onRenderRemoteVideoFirstFrame = onRenderRemoteVideoFirstFrame;

    _addRoomNoticeLog('[Flutter-Dart] registerPlayerCallback, init player stream subscription');
    _streamSubscription = ZegoLiveRoomEventChannel.listenPlayEvent().listen(_eventListener, onError: (error) {
      PlatformException exception = error;
      _addRoomNoticeLog('[Flutter-Dart] player stream subscription listen error: ${exception.message??'no error message'}');
    });

  }

  ///销毁回调对象
  ///
  ///@discussion 当开发者不再需要接收回调时，必须显式调用本 API 销毁回调对象
  static void unregisterPlayerCallback() {

    _addRoomNoticeLog('[Flutter-Dart] unregisterPlayerCallback');

    _onPlayStateUpdate = null;
    _onPlayQualityUpdate = null;
    _onVideoSizeChangedTo = null;
    _onInviteJoinLiveRequest = null;
    _onRecvEndJoinLiveCommand = null;
    _onRemoteCameraStatusUpdate = null;
    _onRemoteMicStatusUpdate = null;
    _onRecvRemoteAudioFirstFrame = null;
    _onRecvRemoteVideoFirstFrame = null;
    _onRenderRemoteVideoFirstFrame = null;

    _streamSubscription.cancel().then((_) {
      _streamSubscription = null;
      _addRoomNoticeLog('[Flutter-Dart] player stream subscription cancel success');
    }).catchError((error) {
      PlatformException exception = error;
      _addRoomNoticeLog('[Flutter-Dart] player stream subscription cancel error: ${exception.message??'no error message'}');
    });

  }



  ///播放流事件
  ///
  ///@param stateCode 播放状态码，0 表示拉流成功
  ///@param streamID 流 ID
  ///@discussion 观众调用 [startPlayingStream] 拉流成功后，通过该 API 通知
  ///@discussion 开发者必须调用 [registerPlayerCallback] 且设置 onPlayStateUpdate 对象参数之后才能收到该回调
  ///@note 拉流状态码，详见[ZegoErrorCode]
  static void Function(int stateCode, String streamID) _onPlayStateUpdate;

  ///观看质量更新
  ///
  ///@param streamID 观看流ID
  ///@param quality 拉流质量参数 参考 [ZegoPlayStreamQuality] 定义
  ///@discussion 调用 [startPlayingStream] 后，该 API 会被多次回调。调用者可以在此回调中获取当前的视频质量数据，加以处理
  ///@discussion 开发者必须调用 [registerPlayerCallback] 且设置 onPlayQualityUpdate 对象参数之后才能收到该回调
  static void Function(String streamID, ZegoPlayStreamQuality quality) _onPlayQualityUpdate;

  ///视频宽高变化通知
  ///
  ///@param streamID 流的唯一标识
  ///@param width 视频宽度
  ///@param height 视频高度
  ///@discussion startPlay 后，以下情况下，播放端会收到该通知：
  ///1. SDK 在获取到第一帧数据后
  ///2. 直播过程中视频宽高发生变化。从播放第一条流，到获得第一帧数据，中间可能出现一个短暂的时间差（具体时长取决于当前的网络状态），推荐在进入直播页面时加载一张预览图以提升用户体验，然后在本回调中去掉预览图
  ///@discussion 开发者必须调用 [registerPlayerCallback] 且设置 onVideoSizeChangedTo 对象参数之后才能收到该回调
  static void Function(String streamID, int width, int height) _onVideoSizeChangedTo;

  ///收到主播端的邀请连麦请求
  ///
  ///@param seq 连麦请求序列号，标识当次连麦请求
  ///@param fromUserID 来源用户 ID
  ///@param fromUserName 来源用户名
  ///@param roomID 房间 ID
  ///@discussion 主播端调用 inviteJoinLive 邀请观众连麦后，观众端会收到此通知。建议在此通知中，调用 [respondInviteJoinLiveReq] 回应邀请连麦请求
  ///@discussion 开发者必须调用 [registerPlayerCallback] 且设置 onInviteJoinLiveRequest 对象参数之后才能收到该回调
  static void Function(int seq, String fromUserID, String fromUserName, String roomID) _onInviteJoinLiveRequest;

  ///收到结束连麦信令
  ///
  ///@param fromUserID 来源用户 ID
  ///@param fromUserName 来源用户名
  ///@param roomID 房间 ID
  ///@discussion 开发者必须调用 [registerPlayerCallback] 且设置 onRecvEndJoinLiveCommand 对象参数之后才能收到该回调
  static void Function(String fromUserID, String fromUserName, String roomID) _onRecvEndJoinLiveCommand;

  ///远端摄像头状态通知
  ///
  ///@param streamID 流的唯一标识
  ///@param status 参考 [ZegoDeviceStatus]
  ///@discussion 当房间内其他正在推流的用户关闭或开启摄像头时，当前用户可以收到此通知。
  ///@discussion 开发者必须调用 [registerPlayerCallback] 且设置 onRemoteCameraStatusUpdate 对象参数之后才能收到该回调
  static void Function(int status, String streamID, int reason) _onRemoteCameraStatusUpdate;

  ///远端麦克风状态通知
  ///
  ///@param streamID 流的唯一标识
  ///@param status 参考
  ///@discussion 当房间内其他正在推流的用户关闭或开启麦克风时，当前用户可以收到此通知。
  ///@discussion 开发者必须调用 [registerPlayerCallback] 且设置 onRemoteMicStatusUpdate 对象参数之后才能收到该回调
  static void Function(int status, String streamID, int reason) _onRemoteMicStatusUpdate;

  ///接收到远端音频的首帧通知
  ///
  ///@param streamID 流的唯一标识
  ///@discussion 当前用户调用 [startPlayingStream] 接收到其他用户的音频数据首帧时，会收到此通知。
  ///@discussion 开发者必须调用 [registerPlayerCallback] 且设置 onRecvRemoteAudioFirstFrame 对象参数之后才能收到该回调
  static void Function(String streamID) _onRecvRemoteAudioFirstFrame;

  ///接收到远端视频的首帧通知
  ///
  ///@param streamID 流的唯一标识
  ///@discussion 当前用户调用 [startPlayingStream] 接收到其他用户的视频数据首帧时，会收到此通知。
  ///@discussion 开发者必须调用 [registerPlayerCallback] 且设置 onRecvRemoteVideoFirstFrame 对象参数之后才能收到该回调
  static void Function(String streamID) _onRecvRemoteVideoFirstFrame;

  ///远端视频渲染首帧通知
  ///
  ///@param streamID 流的唯一标识
  ///@discussion 当前房间内其他用户渲染视频首帧时，当前用户可以收到此通知。
  ///@discussion 开发者必须调用 [registerPlayerCallback] 且设置 onRenderRemoteVideoFirstFrame 对象参数之后才能收到该回调
  static void Function(String streamID) _onRenderRemoteVideoFirstFrame;

  /// SDK内置日志，开发者无需关注
  static void _addRoomNoticeLog(String content) {

    _channel.invokeMethod('addNoticeLog', {
      'content': content
    });

  }

  /// 用于接收native层事件流，开发者无需关注
  static StreamSubscription<dynamic> _streamSubscription;
  /// 用于处理native层事件流，开发者无需关注
  static void _eventListener(dynamic data) {

    final Map<dynamic, dynamic> args = data;
    _addRoomNoticeLog('[Flutter-Dart] player eventListener, method name: ${args['name']}');

    switch (args['name']) {
      case 'onPlayStateUpdate':
        if(_onPlayStateUpdate != null) {

          int stateCode = args['stateCode'];
          String streamID = args['streamID'];

          _onPlayStateUpdate(stateCode, streamID);
        }
        break;
      case 'onPlayQualityUpdate':
        if(_onPlayQualityUpdate != null) {

          String streamID = args['streamID'];

          double fps = args['vnetFps'];
          double vdjFps = args['vdjFps'];
          double vdecFps = args['vdecFps'];
          double vrndFps = args['vrndFps'];
          double kbps = args['vkbps'];

          double afps = args['anetFps'];
          double adjFps = args['adjFps'];
          double adecFps = args['adecFps'];
          double arndFps = args['arndFps'];
          double akbps = args['akbps'];

          double audioBreakRate = args['audioBreakRate'];
          double videoBreakRate = args['videoBreakRate'];

          int rtt = args['rtt'];
          int peerToPeerDelay = args['p2pRtt'];

          int pktLostRate = args['pktLostRate'];
          int peerToPeerPktLostRate = args['p2pPktLostRate'];

          int delay = args['delay'];

          bool isHardwareVdec = args['isHardwareVdec'];

          int width = args['width'];
          int height = args['height'];

          int quality = args['quality'];

          ZegoPlayStreamQuality playStreamQuality = new ZegoPlayStreamQuality(
              fps,
              vdjFps,
              vdecFps,
              vrndFps,
              kbps,
              afps,
              adjFps,
              adecFps,
              arndFps,
              akbps,
              audioBreakRate,
              videoBreakRate,
              rtt,
              peerToPeerDelay,
              pktLostRate * 1.0 / 255.0,
              peerToPeerPktLostRate * 1.0 / 255.0,
              quality,
              delay,
              isHardwareVdec,
              width,
              height
          );

          _onPlayQualityUpdate(streamID, playStreamQuality);

        }
        break;
      case 'onVideoSizeChangedTo':
        if(_onVideoSizeChangedTo != null) {

          String streamID = args['streamID'];
          int width = args['width'];
          int height = args['height'];

          _onVideoSizeChangedTo(streamID, width, height);

        }
        break;
      case 'onInviteJoinLiveRequest':
        if(_onInviteJoinLiveRequest != null) {

          int seq = args['seq'];
          String fromUserID = args['fromUserID'];
          String fromUserName = args['fromUserName'];
          String roomID = args['roomID'];

          _onInviteJoinLiveRequest(seq, fromUserID, fromUserName, roomID);

        }
        break;
      case 'onRecvEndJoinLiveCommand':
        if(_onRecvEndJoinLiveCommand != null) {

          String fromUserID = args['fromUserID'];
          String fromUserName = args['fromUserName'];
          String roomID = args['roomID'];

          _onRecvEndJoinLiveCommand(fromUserID, fromUserName, roomID);
        }
        break;
      case 'onRemoteCameraStatusUpdate':
        if(_onRemoteCameraStatusUpdate != null) {

          int status = args['status'];
          String streamID = args['streamID'];
          int reason = args['reason'];

          _onRemoteCameraStatusUpdate(status, streamID, reason);
        }
        break;
      case 'onRemoteMicStatusUpdate':
        if(_onRemoteMicStatusUpdate != null) {

          int status = args['status'];
          String streamID = args['streamID'];
          int reason = args['reason'];

          _onRemoteMicStatusUpdate(status, streamID, reason);
        }
        break;
      case 'onRecvRemoteAudioFirstFrame':
        if(_onRecvRemoteAudioFirstFrame != null) {

          String streamID = args['streamID'];

          _onRecvRemoteAudioFirstFrame(streamID);
        }
        break;
      case 'onRecvRemoteVideoFirstFrame':
        if(_onRecvRemoteVideoFirstFrame != null) {

          String streamID = args['streamID'];

          _onRecvRemoteVideoFirstFrame(streamID);
        }
        break;
      case 'onRenderRemoteVideoFirstFrame':
        if(_onRenderRemoteVideoFirstFrame != null) {

          String streamID = args['streamID'];

          _onRenderRemoteVideoFirstFrame(streamID);
        }
        break;
      default:
        break;
    }

  }

}