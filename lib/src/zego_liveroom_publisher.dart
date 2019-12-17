import 'dart:async';
import 'package:flutter/services.dart';
import 'zego_api_defines.dart';
import 'zego_api_error_code.dart';
import 'zego_liveroom_event_channel.dart';
import 'zego_liveroom.dart';
import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart';

class ZegoLiveRoomPublisherPlugin {
  /* Method Channel */
  static const MethodChannel _channel =
  const MethodChannel('plugins.zego.im/zegoliveroom_plugin');

  ///创建预览渲染器
  ///
  ///@param width，渲染宽度，单位 px (px = viewWidth * devicePixelRatio)
  ///@param height，渲染高度，单位 px (px = viewHeight * devicePixelRatio)
  ///@return textureID, flutter 渲染的纹理ID，非负数。当返回负数代表创建渲染器失败
  ///@discussion 当接口返回 TextureID 后，可通过 [Texture] 控件展示
  ///@discussion 当看到画面带有明显锯齿感时，请检查是否传入的宽高乘上了 devicePixelRatio, 参考[MediaQuery.of(context).devicePixelRatio]
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 false 时，调用该API有效，否则会返回错误
  static Future<int> createPreviewRenderer(int width, int height) async {
    final int textureID = await _channel.invokeMethod('createPreviewRenderer', {
      'width': width,
      'height': height
    });

    return textureID;
  }

  ///设置本地预览视频视图的模式
  ///
  ///@param mode 模式，参考 [ZegoViewMode] 定义。默认 [ZegoViewMode.ZegoVideoViewModeScaleAspectFit]
  ///@return true 成功，false 失败
  ///@discussion 推流开始前调用本 API 进行参数配置
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 false 时，调用该API有效，否则会返回错误
  static Future<bool> setPreviewViewMode(int mode) async {
    final bool success = await _channel.invokeMethod('setPreviewViewMode', {
      'mode': mode
    });

    return success;
  }

  ///更新预览渲染器的渲染大小
  ///
  ///@param width，渲染宽度，单位 px (px = viewWidth * devicePixelRatio)
  ///@param height，渲染高度，单位 px (px = viewHeight * devicePixelRatio)
  ///@return true 成功，false 失败
  ///@discussion 当需要更新 Texture 渲染控件的大小时，调用本API同步更新渲染器的渲染大小，否则可能会导致图像变形等问题
  ///@discussion 当看到画面带有明显锯齿感时，请检查是否传入的宽高乘上了 devicePixelRatio, 参考[MediaQuery.of(context).devicePixelRatio]
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 false 时，调用该API有效，否则会返回错误
  static Future<void> updatePreviewRenderSize(int width, int height) async {
    return await _channel.invokeMethod('updatePreviewRenderSize', {
      'width': width,
      'height': height
    });
  }

  ///销毁预览渲染器
  ///
  ///@return true 成功，false 失败
  ///@discussion 释放渲染器资源
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 false 时，调用该API有效，否则会返回错误
  static Future<bool> destroyPreviewRenderer() async {
    final bool success = await _channel.invokeMethod('destroyPreviewRenderer');
    return success;
  }

  ///创建预览 Platform View ，iOS 平台为 UIView，Android 平台 为 SurfaceView
  ///
  ///@param View创建后的回调，viewID 为 Platform View 的唯一标识，请开发者自行管理
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 true 时，调用该API有效，否则会返回错误
  static Widget createPreviewPlatformView(Function(int viewID) onViewCreated) {
    if(TargetPlatform.iOS == defaultTargetPlatform) {
      
      return UiKitView(
        key: new ObjectKey('preview'),
        viewType: 'plugins.zego.im/zego_view',
        onPlatformViewCreated: (int viewID) {
          if(onViewCreated != null)
            onViewCreated(viewID);
        }
      );
    } else if(TargetPlatform.android == defaultTargetPlatform) {

      return AndroidView(
        key: new ObjectKey('preview'),
        viewType: 'plugins.zego.im/zego_view',
        onPlatformViewCreated: (int viewID) {
          if(onViewCreated != null)
            onViewCreated(viewID);
        },
      );
    }

    return null;
  }

  ///设置预览 Platform View 视频视图
  ///
  ///@param viewID，调用 [createPreviewPlatformView] 之后返回的 Platform View 的唯一标识
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 true 时，调用该API有效，否则会返回错误
  static Future<bool> setPreviewView(int viewID) async {
    final bool success = await _channel.invokeMethod('setPreviewView', {
      'viewID': viewID
    });

    return success;
  }


  ///设置预览 Platform View 视频视图的模式
  ///
  ///@param mode 模式，参考 [ZegoViewMode] 定义。默认 [ZegoViewMode.ZegoVideoViewModeScaleAspectFit]
  ///@return true 成功，false 失败
  ///@discussion 推流开始前调用本 API 进行参数配置
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 true 时，调用该API有效，否则会返回错误
  static Future<bool> setPlatformViewPreviewViewMode(int mode) async {
    final bool success = await _channel.invokeMethod('setPlatformViewPreviewViewMode', {
      'mode': mode
    });

    return success;
  }

  ///销毁预览 Platform View
  ///
  ///@param viewID，调用 [createPreviewPlatformView] 之后返回的 Platform View 的唯一标识
  ///@discussion 只有当 [ZegoLiveRoomPlugin.enablePlatformView] 传值为 true 时，调用该API有效，否则会返回错误
  static Future<bool> removePreviewPlatformView(int viewID) async {
    final bool success = await _channel.invokeMethod('removePreviewPlatformView', {
      'viewID': viewID
    });

    return success;
  }

  ///启动本地预览
  ///
  ///@return true 成功，false 失败
  ///@discussion 启动本地预览前，要先创建推流渲染器并设置到 Texture 控件中，然后调用本 API 才能看到图像
  static Future<bool> startPreview() async {
    final bool success = await _channel.invokeMethod('startPreview');
    return success;
  }

  ///结束本地预览
  ///
  ///@return true 成功，false 失败
  ///@discussion 建议停止推流，或本地预览结束后，调用该 API 停止本地预览
  static Future<bool> stopPreview() async {
    final bool success = await _channel.invokeMethod('stopPreview');
    return success;
  }

  ///开始发布直播
  ///
  ///@param streamID 流 ID
  ///@param title 直播名称
  ///@param flag 直播属性，参考 [ZegoPublishFlag] 定义
  ///@param extraInfo 流附加信息, 最大为 1024 字节
  ///@return true 成功，false 失败
  ///@discussion 发布直播成功后，等待 onPublishStateUpdate 通知
  static Future<bool> startPublishing(String streamID, String title, int flag, {String extraInfo}) async {
    _addRoomNoticeLog('[Flutter-Dart] startPublishing, streamID: $streamID, title: $title, flag: $flag');
    final bool success = await _channel.invokeMethod('startPublishing', {
      'streamID': streamID,
      'title': title,
      'flag': flag,
      'extraInfo': extraInfo
    });

    return success;
  }

  ///停止直播
  ///
  ///@return true 成功，false 失败
  ///@discussion 停止直播
  static Future<bool> stopPublishing() async {
    _addRoomNoticeLog('[Flutter-Dart] stopPublishing');
    final bool success = await _channel.invokeMethod('stopPublishing');

    return success;
  }

  ///更新流附加信息
  ///
  ///@param extraInfo 流附加信息, 最大为 1024 字节
  ///@return true 成功，false 失败
  ///@discussion 更新流附加信息成功后，除调用方外，同一房间内的其他人会收到 onStreamExtraInfoUpdated 通知
  static Future<bool> updateStreamExtraInfo(String extraInfo) async {
    final bool success = await _channel.invokeMethod('updateStreamExtraInfo', {
      'extraInfo': extraInfo
    });

    return success;
  }

  ///设置延迟模式
  ///
  ///@param mode 延迟模式，参考 [ZegoLatencyMode] 定义。默认 [ZegoLatencyMode.ZEGO_LATENCY_MODE_NORMAL]
  ///@discussion 在推流前调用
  static Future<void> setLatencyMode(int mode) async {
    return await _channel.invokeMethod('setLatencyMode', {
      'mode': mode
    });
  }

  ///设置预览和推流镜像
  ///
  ///@param mode 镜像模式，参考 [ZegoVideoMirrorMode] 定义。默认 [ZegoVideoMirrorMode.ZegoVideoMirrorModePreviewMirrorPublishNoMirror]
  ///@discussion 预览镜像只对前置摄像头有效，后置无效。
  ///@return true 成功，false 失败
  static Future<void> setVideoMirrorMode(int mode) async {
    final bool success = await _channel.invokeMethod('setVideoMirrorMode', {
      'mode': mode
    });

    return success;
  }

  ///开启麦克风
  ///
  ///@param enable true 打开，false 关闭。默认 true
  ///@return true 成功，false 失败
  ///@discussion 推流时可调用本 API 进行参数配置
  static Future<bool> enableMic(bool enable) async {
    final bool success = await _channel.invokeMethod('enableMic', {
      'enable' : enable
    });

    return success;
  }

  ///开启视频采集
  ///
  ///@param enable true 打开，false 关闭。默认 true
  ///@return true 成功，false 失败
  ///@discussion 推流时可调用本 API 进行参数配置
  static Future<bool> enableCamera(bool enable) async {
    final bool success = await _channel.invokeMethod('enableCamera', {
      'enable': enable
    });

    return success;
  }

  ///是否使用前置摄像头
  ///
  ///@param bFront true 使用，false 不使用。默认 true
  ///@return true 成功，false 失败
  ///@discussion 推流时可调用本 API 进行参数配置
  static Future<bool> setFrontCam(bool bFront) async {
    final bool success = await _channel.invokeMethod('setFrontCam', {
      'enable' : bFront
    });

    return success;
  }

  ///设置视频配置
  ///
  ///@param config 配置参数（视频编码输出分辨率、视频采集分辨率、视频帧率、视频码率），参考 [ZegoAVConfig] 定义，默认值为 [ZegoAVPresetType.AVCONFIG_GENERIC]
  ///@return true 成功，false 失败
  ///@discussion 推流开始前调用本 API 进行视频采集参数配置
  static Future<bool> setAVConfig(ZegoAVConfig config) async {
    final bool success = await _channel.invokeMethod('setAVConfig', {
      'captureWidth': config.videoCaptureResolutionWidth,
      'captureHeight': config.videoCaptureResolutionHeight,
      'encodeWidth': config.videoEncodeResolutionWidth,
      'encodeHeight': config.videoEncodeResolutionHeight,
      'bitrate': config.bitrate,
      'fps': config.fps
    });

    return success;
  }

  ///响应连麦请求
  ///
  ///@param seq 连麦请求序列号，标识当次连麦请求
  ///@param rspResult 响应结果，0 表示同意连麦
  ///@return true 成功，false 失败
  ///@discussion 主播端通过 onJoinLiveRequest 收到观众连麦申请，再调用本 API 响应
  static Future<bool> respondJoinLiveReq(int seq, int rspResult) async {
    final bool success = await _channel.invokeMethod('respondJoinLiveReq', {
      'seq': seq,
      'rspResult': rspResult
    });

    return success;
  }

  ///邀请连麦
  ///
  ///@param userID 准备邀请的用户 ID
  ///@return 连麦请求响应结果，参考 [ZegoResponseResult] 定义
  ///@discussion 主播邀请连麦成功后，被邀请的观众收到 onInviteJoinLiveRequest 通知
  static Future<ZegoResponseResult> inviteJoinLive(String userID) async {
    final Map<dynamic, dynamic> mapResult = await _channel.invokeMethod('inviteJoinLive', {
      'userID': userID
    });

    ZegoResponseResult result = new ZegoResponseResult(
        mapResult['result'],
        mapResult['fromUserID'],
        mapResult['fromUserName']
    );

    return result;
  }

  ///结束连麦
  ///
  ///@param userId 指定UserId停止连麦
  ///@return 结束连麦结果，参考 [ZegoEndJoinLiveResult] 定义
  static Future<ZegoEndJoinLiveResult> endJoinLive(String userID) async {
    final Map<dynamic, dynamic> mapResult = await _channel.invokeMethod('endJoinLive', {
      'userID': userID
    });

    ZegoEndJoinLiveResult result = new ZegoEndJoinLiveResult(
        mapResult['errorCode'],
        mapResult['roomID']
    );

    return result;
  }

  ///主播方开启美颜功能
  ///
  ///@param feature 美颜特性，参考 [ZegoBeautifyFeature] 定义。默认无美颜
  ///@return true 成功，false 失败
  ///@discussion 推流时可调用本 API 进行参数配置
  static Future<bool> enableBeautifying(int feature) async {
    final bool success = await _channel.invokeMethod('enableBeautifying', {
      'feature' : feature
    });

    return success;
  }

  ///设置美颜磨皮的采样步长
  ///
  ///@param step 采样步长，取值范围[1,16]。默认 4.0
  ///@return true 成功，false 失败
  ///@discussion 推流时可调用本 API 进行参数配置。设置时需确保对应美颜特性开启
  static Future<bool> setPolishStep(double step) async {
    final bool success = await _channel.invokeMethod('setPolishStep', {
      'step' : step
    });

    return success;
  }

  ///设置美颜采样颜色阈值
  ///
  ///@param factor 采样颜色阈值，取值范围[0,16]。默认 4.0
  ///@return true 成功，false 失败
  ///@discussion 推流时可调用本 API 进行参数配置。设置时需确保对应美颜特性开启
  static Future<bool> setPolishFactor(double factor) async {
    final bool success = await _channel.invokeMethod('setPolishFactor', {
      'factor' : factor
    });

    return success;
  }

  ///设置美颜美白的亮度修正参数
  ///
  ///@param factor 亮度修正参数，取值范围[0,1]，值越大亮度越暗。默认 0.5
  ///@return true 成功，false 失败
  ///@discussion 推流时可调用本 API 进行参数配置。设置时需确保对应美颜特性开启
  static Future<bool> setWhitenFactor(double factor) async {
    final bool success = await _channel.invokeMethod('setWhitenFactor', {
      'factor' : factor
    });

    return success;
  }

  ///设置滤镜
  ///
  ///@param filter 滤镜种类，参考 [ZegoFilter] 定义。默认不使用滤镜
  ///@return true 成功，false 失败
  ///@discussion 推流时可调用本 API 进行参数配置
  static Future<bool> setFilter(int filter) async {
    final bool success = await _channel.invokeMethod('setFilter', {
      'filter': filter
    });

    return success;
  }

  ///硬件编码开关
  ///
  ///@param bRequire true 开启，false 关闭。默认 false
  ///@return true 成功，false 失败
  ///@discussion 如果要打开，需要在推流前设置。打开硬编硬解开关需后台可控，避免碰到版本升级或者硬件升级时出现硬编硬解失败的问题
  static Future<bool> requireHardwareEncoder(bool bRequire) async {
    final bool success = await _channel.invokeMethod('requireHardwareEncoder', {
      'bRequire': bRequire
    });

    return success;
  }

  ///是否开启流量控制
  ///
  ///@param enable true 开启；false 关闭。默认开启流量控制，property 为 [ZegoTrafficControlProperty.ZEGOAPI_TRAFFIC_CONTROL_ADAPTIVE_FPS]
  ///@param properties 流量控制属性 (帧率，分辨率）可以多选, 参考 [ZegoTrafficControlProperty] 定义
  ///@discussion enable设置为false时，properties参数会被忽略
  ///@discussion 在推流前调用，在纯 UDP 方案才可以调用此接口
  static Future<void> enableTrafficControl(bool bEnable, int properties) async {
    return await _channel.invokeMethod('enableTrafficControl', {
      'enable' : bEnable,
      'properties': properties
    });
  }

  ///设置TrafficControl视频码率最小值
  ///
  ///@param bitrate 码率，单位为bps
  ///@param mode 码率控制模式，参考 [ZegoTrafficControlMinVideoBitrateMode] 定义
  ///@discussion InitSDK 之后调用有效
  ///@discussion 设置一个在traffic control中video码率的一个最小值，当网络不足以发送这个最小值的时候视频会被卡住，而不是以低于该码率继续发送。
  ///@discussion 初始化SDK后默认情况下没有设置改值，即尽可能的保持视频流畅，InitSDK之后可以随时修改，未重新InitSDK之前如果需要取消该设置值的限制可以设置为0
  static Future<void> setMinVideoBitrateForTrafficControl(int bitrate, int mode) async {
    return await _channel.invokeMethod('setMinVideoBitrateForTrafficControl', {
      'bitrate': bitrate,
      'mode': mode
    });
  }

  ///设置选用分层编码
  ///
  ///@param codecID 是否选用分层编码，参考 [ZegoVideoCodecAvc] 定义。默认为 [ZegoVideoCodecAvc.VIDEO_CODEC_DEFAULT] 无分层编码
  ///@return true 成功，false 失败
  ///@discussion 设置选用分层编码,在 InitSDK 后，推流前调用有效
  static Future<bool> setVideoCodecId(int codecID) async {
    final bool success = await _channel.invokeMethod('setVideoCodecId', {
      'codecID': codecID
    });

    return success;
  }

  ///设置音频设备模式
  ///
  ///@param mode 模式， 参考 [ZegoAudioDeviceMode] 定义。默认 [ZegoAudioDeviceMode.ZEGO_AUDIO_DEVICE_MODE_AUTO]
  ///@discussion 在 InitSDK 前调用
  static Future<void> setAudioDeviceMode(int mode) async {
    return await _channel.invokeMethod('setAudioDeviceMode', {
      'mode': mode
    });
  }

  ///设置音频码率

  ///@param bitrate 码率
  ///@return true 成功 false 失败
  static Future<bool> setAudioBitrate(int bitrate) async {
    final bool success = await _channel.invokeMethod('setAudioBitrate', {
      'bitrate': bitrate
    });

    return success;
  }

  ///回声消除开关
  ///
  ///@param enable true 打开 false 关闭，默认在 iOS 平台为关闭，在 Android 平台为打开
  ///@return true 成功 false 失败
  ///@discussion 建议在推流前调用设置
  static Future<bool> enableAEC(bool enable) async {
    final bool success = await _channel.invokeMethod('enableAEC', {
      'enable': enable
    });

    return success;
  }

  ///设置回声消除模式
  ///
  ///@param mode 回声消除模式，参考 [ZegoAECMode] 定义，默认值为 [ZegoAECMode.ZEGOAPI_AEC_MODE_ARRGRESSIVE]
  ///@return true 成功，false 失败
  ///@discussion 建议在推流前调用设置
  static Future<void> setAECMode(int mode) async {
    return await _channel.invokeMethod('setAECMode', {
      'mode': mode
    });
  }

  ///音频采集自动增益开关
  ///
  ///@param enable 是否开启，默认关闭
  ///@return true 成功，false 失败
  ///@discussion 建议在推流前调用设置
  static Future<bool> enableAGC(bool enable) async {
    final bool success = await _channel.invokeMethod('enableAGC', {
      'enable': enable
    });

    return success;
  }

  ///是否开启语音活动检测
  ///
  ///@param enable true 开启；false 关闭，默认关闭
  ///@return true 成功，false 失败
  ///@discussion 在有音频的条件下检测到语音时才发送语音包，有效减少流量消耗，降低成本
  ///@discussion 在推流前调用，只有纯 UDP 方案才可以调用此接口
  static Future<void> enableVAD(bool enable) async {
    return await _channel.invokeMethod('enableVAD', {
      'enable': enable
    });
  }

  ///音频采集噪声抑制开关
  ///
  ///@param enable true 开启，false 关闭，默认在 iOS 平台为关闭，在 Android 平台为打开
  ///@return true 成功，false 失败
  static Future<bool> enableANS(bool enable) async {
    final bool success = await _channel.invokeMethod('enableANS', {
      'enable': enable
    });

    return success;
  }

  ///是否开启离散音频包发送
  ///
  ///@param enable true 开启，此时关闭麦克风后，不会发送静音包；false 关闭，此时关闭麦克风后会发送静音包
  ///@return true 成功，false 失败
  ///@discussion 默认状态下，关闭麦克风后会发送静音包
  ///@discussion 在推流前调用，只有纯 UDP 方案才可以调用此接口
  static Future<void> enableDTX(bool enable) async {
    return await _channel.invokeMethod('enableDTX', {
     'enable': enable
   });
  }

  ///设置手机方向
  ///
  ///@param orientation 手机方向，参考 [ZegoAppOrientation] 定义。
  ///@return true 成功，false 失败
  ///@discussion 本设置用于校正主播输出视频朝向
  static Future<bool> setAppOrientation(int orientation) async {
    final success = await _channel.invokeMethod('setAppOrientation', {
      'orientation': orientation
    });

    return success;
  }

  ///自定义发布直播（推流）配置
  ///
  ///@param  rtmpURL 用户指定的转推 RTMP 地址，非 RTMP 地址可能导致转推失败
  static Future<void> setPublishConfig(String rtmpURL) async {
    return await _channel.invokeMethod('setPublishConfig', {
      'rtmpURL': rtmpURL
    });
  }

  ///添加已发布直播的转推地址
  ///
  ///@param target 添加的转推地址（支持rtmp/avertp）
  ///@param streamID 推流ID
  ///@return true 表示调用成功，false 表示调用失败。
  ///@discussion 注意: 必须在 initSDK 后调用
  static Future<ZegoStreamRelayCDNResult> addPublishTarget(String target, String streamID) async {
    final Map<dynamic, dynamic> mapResult = await _channel.invokeMethod('addPublishTarget', {
      'target': target,
      'streamID': streamID
    });

    ZegoStreamRelayCDNResult result = new ZegoStreamRelayCDNResult(mapResult['errorCode'], mapResult['streamID']);
    return result;
  }

  ///删除转推地址
  ///
  ///@param target 转推地址（支持rtmp/avertp）
  ///@param streamID 推流ID
  ///@return true 表示调用成功，false 表示调用失败。
  ///@discussion 注意: 必须在 initSDK 后调用
  static Future<ZegoStreamRelayCDNResult> deletePublishTarget(String target, String streamID) async {
    final Map<dynamic, dynamic> mapResult = await _channel.invokeMethod('deletePublishTarget', {
      'target': target,
      'streamID': streamID
    });

    ZegoStreamRelayCDNResult result = new ZegoStreamRelayCDNResult(mapResult['errorCode'], mapResult['streamID']);
    return result;
  }

  ///设置回调对象
  ///
  ///@param onPublishStateUpdate 设置接收 推流状态更新 回调，参考 [_onPublishStateUpdate] 定义
  ///@param onPublishQualityUpdate 设置接收 发布质量更新 回调，参考 [_onPublishQualityUpdate] 定义
  ///@param onCaptureVideoSizeChangedTo 设置接收 采集视频的宽度和高度变化 回调，参考 [_onCaptureVideoSizeChangedTo] 定义
  ///@param onCaptureAudioFirstFrame 设置接收 采集音频的首帧 回调，参考 [_onCaptureAudioFirstFrame] 定义
  ///@param onCaptureVideoFirstFrame 设置接收 采集视频的首帧 回调，参考 [_onCaptureVideoFirstFrame] 定义
  ///@param onJoinLiveRequest 设置接收 收到连麦请求 回调，参考[_onJoinLiveRequest] 定义
  ///@discussion 开发者只有调用本 API 设置回调对象才能收到相关回调
  static void registerPublisherCallback({
    Function(int stateCode, String streamID, Map<String, dynamic> info) onPublishStateUpdate,
    Function(String streamID, ZegoPublishStreamQuality quality) onPublishQualityUpdate,
    Function(String streamID, List<ZegoStreamRelayCDNInfo> statesInfo) onRelayCDNStateUpdate,
    Function(int width, int height) onCaptureVideoSizeChangedTo,
    Function() onCaptureAudioFirstFrame,
    Function() onCaptureVideoFirstFrame,
    Function(int seq, String fromUserID, String fromUserName, String roomID) onJoinLiveRequest,
  }) {

    _onPublishStateUpdate = onPublishStateUpdate;
    _onPublishQualityUpdate = onPublishQualityUpdate;
    _onRelayCDNStateUpdate = onRelayCDNStateUpdate;
    _onCaptureVideoSizeChangedTo = onCaptureVideoSizeChangedTo;
    _onCaptureAudioFirstFrame = onCaptureAudioFirstFrame;
    _onCaptureVideoFirstFrame = onCaptureVideoFirstFrame;
    _onJoinLiveRequest = onJoinLiveRequest;

    _addRoomNoticeLog('[Flutter-Dart] registerPublisherCallback, init publisher stream subscription');
    _streamSubscription = ZegoLiveRoomEventChannel.listenPublishEvent().listen(_eventListener, onError: (error) {
      PlatformException exception = error;
      _addRoomNoticeLog('[Flutter-Dart] publisher stream subscription listen error: ${exception.message??'no error message'}');
    });

  }

  ///销毁回调对象
  ///
  ///@discussion 当开发者不再需要接收回调时，必须显式调用本 API 销毁回调对象
  static void unregisterPublisherCallback() {

    _addRoomNoticeLog('[Flutter-Dart] unregisterPublisherCallback');

    _onPublishStateUpdate = null;
    _onPublishQualityUpdate = null;
    _onRelayCDNStateUpdate = null;
    _onCaptureVideoSizeChangedTo = null;
    _onCaptureAudioFirstFrame = null;
    _onCaptureVideoFirstFrame = null;
    _onJoinLiveRequest = null;

    _streamSubscription.cancel().then((_) {
      _streamSubscription = null;
      _addRoomNoticeLog('[Flutter-Dart] publisher stream subscription cancel success');
    }).catchError((error) {
      PlatformException exception = error;
      _addRoomNoticeLog('[Flutter-Dart] publisher stream subscription cancel error: ${exception.message??'no error message'}');
    });

  }



  ///推流状态更新
  ///
  ///@param stateCode 状态码
  ///@param streamID 流 ID
  ///@param info 推流信息
  ///@discussion 主播调用 [startPublishing] 推流成功后，通过该 API 通知主播方
  ///@discussion 开发者必须调用 [registerPublisherCallback] 且设置 onPublishStateUpdate 对象参数之后才能收到该回调
  ///@note 推流状态码，详见[ZegoErrorCode]
  static void Function(int stateCode, String streamID, Map<String, dynamic> info) _onPublishStateUpdate;

  ///发布质量更新
  ///
  ///@param streamID streamID 发布流 ID
  ///@param quality 推流质量参数 参考 [ZegoPublishStreamQuality] 定义
  ///@discussion 调用 [startPublishing] 后，该 API 会被多次回调。调用者可以在此回调中获取当前的视频质量数据，加以处理
  ///@discussion 开发者必须调用 [registerPublisherCallback] 且设置 onPublishQualityUpdate 对象参数之后才能收到该回调
  static void Function(String streamID, ZegoPublishStreamQuality quality) _onPublishQualityUpdate;

  ///发布直播转推 CDN 状态信息更新
  ///
  ///@param statesInfo 转推CDN状态信息数组
  ///@param streamID 发布直播的流ID，在用户同时推多路流的情况下需要以此判断本次回调表示的是哪一路流的状态更新
  ///@discussion 可以通过此 API 获取当前发布的直播转推 CDN 的状态更新。
  static void Function(String streamID, List<ZegoStreamRelayCDNInfo> statesInfo) _onRelayCDNStateUpdate;

  ///采集视频的宽度和高度变化通知
  ///
  ///@param width 视频宽度
  ///@param height 视频高度
  ///@discussion 发布直播成功后，当视频尺寸变化时，发布者会收到此回调通知
  ///@discussion 开发者必须调用 [registerPublisherCallback] 且设置 onCaptureVideoSizeChangedTo 对象参数之后才能收到该回调
  static void Function(int width, int height) _onCaptureVideoSizeChangedTo;

  ///采集音频的首帧通知
  ///
  ///@discussion 采集到音频的首帧之后，开发者将收到此回调通知
  ///@discussion 开发者必须调用 [registerPublisherCallback] 且设置 onCaptureAudioFirstFrame 对象参数之后才能收到该回调
  static void Function() _onCaptureAudioFirstFrame;

  ///采集视频的首帧通知
  ///
  ///@discussion 采集到视频的首帧之后，开发者将收到此回调通知
  ///@discussion 开发者必须调用 [registerPublisherCallback] 且设置 onCaptureVideoFirstFrame 对象参数之后才能收到该回调
  static void Function() _onCaptureVideoFirstFrame;

  ///收到连麦请求
  ///
  ///@param seq 连麦请求序列号，标识当次连麦请求
  ///@param fromUserID 来源用户 ID
  ///@param fromUserName 来源用户名
  ///@param roomID 房间 ID
  ///@discussion 观众调用申请连麦后，主播端会收到本通知
  ///@discussion 开发者必须调用 [registerPublisherCallback] 且设置 onJoinLiveRequest 对象参数之后才能收到该回调
  static void Function(int seq, String fromUserID, String fromUserName, String roomID) _onJoinLiveRequest;

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
    _addRoomNoticeLog('[Flutter-Dart] publisher eventListener, method name: ${args['name']}');

    switch (args['name']) {
      case 'onPublishStateUpdate':
        if(_onPublishStateUpdate != null) {

          int stateCode = args['stateCode'];
          String streamID = args['streamID'];
          Map<String, dynamic> info = {};
          Map<dynamic, dynamic> objInfo = args['streamInfo'];
          objInfo.forEach((key, value) {
            info[key] = value;
          });

          _onPublishStateUpdate(stateCode, streamID, info);
        }
        break;
      case 'onPublishQualityUpdate':
        if(_onPublishQualityUpdate != null) {

          String streamID = args['streamID'];

          double vcapfps = args['vcapFps'];
          double vencFps = args['vencFps'];
          double fps = args['vnetFps'];
          double kbps = args['vkbps'];

          double acapFps = args['acapFps'];
          double afps = args['anetFps'];
          double akbps = args['akbps'];

          int rtt = args['rtt'];
          int pktLostRate = args['pktLostRate'];

          bool isHardwareVenc = args['isHardwareVenc'];

          int width = args['width'];
          int height = args['height'];

          int quality = args['quality'];

          ZegoPublishStreamQuality publishStreamQuality = new ZegoPublishStreamQuality(
              vcapfps,
              vencFps,
              fps,
              kbps,
              acapFps,
              afps,
              akbps,
              rtt,
              pktLostRate * 1.0 / 255.0,
              quality,
              isHardwareVenc,
              width,
              height
          );

          _onPublishQualityUpdate(streamID, publishStreamQuality);

        }
        break;
      case 'onRelayCDNStateUpdate':
        if(_onRelayCDNStateUpdate != null) {
          
          String streamID = args['streamID'];
          List<ZegoStreamRelayCDNInfo> statesInfo = [];
          List<dynamic> objList = args['statesInfo'];

          for (Map<dynamic, dynamic> obj in objList) {
            ZegoStreamRelayCDNInfo info = new ZegoStreamRelayCDNInfo(obj['rtmpURL'], obj['state'], obj['detail'], obj['stateTime']);
            statesInfo.add(info);
          }
          _onRelayCDNStateUpdate(streamID, statesInfo);
        }
        break;
      case 'onCaptureVideoSizeChangedTo':
        if(_onCaptureVideoSizeChangedTo != null) {

          int width = args['width'];
          int height = args['height'];

          _onCaptureVideoSizeChangedTo(width, height);

        }
        break;
      case 'onCaptureAudioFirstFrame':
        if(_onCaptureAudioFirstFrame != null) {

          _onCaptureAudioFirstFrame();
        }
        break;
      case 'onCaptureVideoFirstFrame':
        if(_onCaptureVideoFirstFrame != null) {

          _onCaptureVideoFirstFrame();
        }
        break;
      case 'onJoinLiveRequest':
        if(_onJoinLiveRequest != null) {

          int seq = args['seq'];
          String fromUserID = args['fromUserID'];
          String fromUserName = args['fromUserName'];
          String roomID = args['roomID'];

          _onJoinLiveRequest(seq, fromUserID, fromUserName, roomID);

        }
        break;
      default:
        break;
    }

  }
}