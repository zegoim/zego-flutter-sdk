
/// 房间成员角色
class ZegoRoomRole {

  /// 主播
  static const int ROOM_ROLE_ANCHOR = 1;

  /// 观众
  static const int ROOM_ROLE_AUDIENCE = 2;

}

/// 房间用户更新属性
class ZegoUserUpdateFlag {

  /// 新增
  static const int USER_ADDED = 1;

  /// 删除
  static const int USER_DELETED = 2;

}

/// 房间用户更新类型
class ZegoUserUpdateType {

  /// 全量更新
  static const int UPDATE_TOTAL = 1;

  /// 增量更新
  static const int UPDATE_INCREASE = 2;

}

/// 房间用户属性
class ZegoUser {

  /// 用户ID
  final String userID;

  /// 用户名
  final String userName;

  const ZegoUser(this.userID, this.userName);

  Map<String, String> toMap() {
    return {
      'userID': userID,
      'userName': userName
    };
  }
}

/// 房间用户详细属性
class ZegoUserInfo{

  /// 用户ID
  final String userID;

  /// 用户名
  final String userName;

  /// 房间用户更新属性，参考 [ZegoUserUpdateFlag] 定义
  final int flag;

  /// 房间成员角色，参考 [ZegoRoomRole] 定义
  final int role;

  const ZegoUserInfo(this.userID, this.userName, this.flag, this.role);
}

/// 推流通道标记
class ZegoPublishChannel {

  /// 主推流通道
  static const String kZegoVideoDataMainPublishingStream = '';

  /// 辅推流通道
  static const String kZegoVideoDataAuxPublishingStream = ' ';

}

/// 发布直播的模式
class ZegoPublishFlag {

  /// 连麦模式
  static const int ZEGO_JOIN_PUBLISH = 0;

  /// 混流模式
  static const int ZEGO_MIX_STREAM = 1 << 1;

  /// 单主播模式
  static const int ZEGO_SINGLE_ANCHOR = 1 << 2;

}

/// 流属性
class ZegoStreamInfo {

  /// 用户ID
  final String userID;

  /// 用户名
  final String userName;

  /// 流ID
  final String streamID;

  /// 流附加信息
  final String extraInfo;

  const ZegoStreamInfo(this.userID, this.userName, this.streamID, this.extraInfo);
}

/// 多媒体流附加信息
class ZegoStreamExtraPlayInfo {

  /// 流参数
  final String params;

  /// rtmp 地址
  final List<String> rtmpUrls;

  /// flv 地址
  final List<String> flvUrls;

  const ZegoStreamExtraPlayInfo(this.params, this.rtmpUrls, this.flvUrls);

  Map<String, dynamic> toMap() {
    return {
      'params': params,
      'rtmpUrls': rtmpUrls,
      'flvUrls': flvUrls
    };
  }
}

/// 流变更类型
class ZegoStreamUpdateType {

  /// 流新增
  static const int STREAM_ADD = 2001;

  /// 流删除
  static const int STREAM_DELETE = 2002;

}

class ZegoAVPresetType {

  /// 超低质量
  static const int AVCONFIG_VERYLOW = 0;

  /// 低质量
  static const int AVCONFIG_LOW = 1;

  /// 标准质量
  static const int AVCONFIG_GENERIC = 2;

  /// 高质量
  static const int AVCONFIG_HIGH = 3;

  /// 超高质量
  static const int AVCONFIG_VERYHIGH = 4;

  /// 极高质量
  static const int AVCONFIG_SUPERHIGH = 5;

}

/// 视频参数设置
class ZegoAVConfig {

  /// 视频采集分辨率-宽
  int videoCaptureResolutionWidth;

  /// 视频采集分辨率-高
  int videoCaptureResolutionHeight;

  /// 视频编码输出分辨率-宽
  int videoEncodeResolutionWidth;

  /// 视频编码输出分辨率-高
  int videoEncodeResolutionHeight;

  /// 视频帧率
  int fps;

  /// 视频码率
  int bitrate;

  /// 预设级别，参考 [ZegoAVPresetType] 定义
  int avPresetType;

  ZegoAVConfig(int presetType) {

    presetType < ZegoAVPresetType.AVCONFIG_VERYLOW || presetType > ZegoAVPresetType.AVCONFIG_SUPERHIGH
        ?
    this.avPresetType = ZegoAVPresetType.AVCONFIG_GENERIC :
    this.avPresetType = presetType;

    
    switch (this.avPresetType){
      case ZegoAVPresetType.AVCONFIG_VERYLOW:
        /// 分辨率：180 x 320
        this.videoCaptureResolutionWidth = 180;
        this.videoCaptureResolutionHeight = 320;
        this.videoEncodeResolutionWidth = 180;
        this.videoEncodeResolutionHeight = 320;
        /// 帧率：15fps
        this.fps = 15;
        /// 码率：300kbps
        this.bitrate = 300 * 1000;
        break;

      case ZegoAVPresetType.AVCONFIG_LOW:
        /// 分辨率：270 x 480
        this.videoCaptureResolutionWidth = 270;
        this.videoCaptureResolutionHeight = 480;
        this.videoEncodeResolutionWidth = 270;
        this.videoEncodeResolutionHeight = 480;
        /// 帧率：15fps
        this.fps = 15;
        /// 码率：400kbps
        bitrate = 400 * 1000;
        break;
        
      case ZegoAVPresetType.AVCONFIG_GENERIC:
        /// 分辨率：360 x 640
        this.videoCaptureResolutionWidth = 360;
        this.videoCaptureResolutionHeight = 640;
        this.videoEncodeResolutionWidth = 360;
        this.videoEncodeResolutionHeight = 640;
        /// 帧率：15fps
        this.fps = 15;
        /// 码率 600kbps
        this.bitrate = 600 * 1000;
        break;

      case ZegoAVPresetType.AVCONFIG_HIGH:
        /// 分辨率：540 x 960
        this.videoCaptureResolutionWidth = 540;
        this.videoCaptureResolutionHeight = 960;
        this.videoEncodeResolutionWidth = 540;
        this.videoEncodeResolutionHeight = 960;
        /// 帧率：15fps
        this.fps = 15;
        /// 码率：1200kbps
        this.bitrate = 1200 * 1000;
        break;
        
      case ZegoAVPresetType.AVCONFIG_VERYHIGH:
        /// 分辨率：720 x 1280
        this.videoCaptureResolutionWidth = 720;
        this.videoCaptureResolutionHeight = 1280;
        this.videoEncodeResolutionWidth = 720;
        this.videoEncodeResolutionHeight = 1280;
        /// 帧率：15fps
        this.fps = 15;
        /// 码率：1500kbps
        this.bitrate = 1500 * 1000;
        break;

      case ZegoAVPresetType.AVCONFIG_SUPERHIGH:
        /// 分辨率：1080 x 1920
        this.videoCaptureResolutionWidth = 1080;
        this.videoCaptureResolutionHeight = 1920;
        this.videoEncodeResolutionWidth = 1080;
        this.videoEncodeResolutionHeight = 1920;
        /// 帧率：15fps
        this.fps = 15;
        /// 码率：3000kbps
        this.bitrate = 3000 * 1000;
        break;
    }
  }
}

/// 渲染视频视图的模式
class ZegoViewMode {

  ///等比缩放，可能有黑边
  static const int ZegoRendererScaleAspectFit = 0;

  ///等比缩放填充整个view
  static const int ZegoRendererScaleAspectFill = 1;

  ///填充整个View，图像可能会变形
  static const int ZegoRendererScaleToFill = 2;

}

/// 设备模块类型
class ZegoApiModule {

  /// 音频采集播放设备
  static const int ZEGOAPI_MODULE_AUDIO = 0x4 | 0x8;

}


/// 延迟模式
class ZegoLatencyMode {

  /// 普通延迟模式
  static const int ZEGO_LATENCY_MODE_NORMAL = 0;

  /// 低延迟模式，无法用于 RTMP 流
  static const int ZEGO_LATENCY_MODE_LOW = 1;

  /// 普通延迟模式，最高码率可达 192K
  static const int ZEGO_LATENCY_MODE_NORMAL2 = 2;

  /// 低延迟模式，无法用于 RTMP 流。相对于 ZEGO_LATENCY_MODE_LOW 而言，CPU 开销稍低
  static const int ZEGO_LATENCY_MODE_LOW2 = 3;

  /// 低延迟模式，无法用于 RTMP 流。支持WebRTC必须使用此模式
  static const int ZEGO_LATENCY_MODE_LOW3 = 4;

  /// 普通延迟模式，使用此模式前先咨询即构技术支持
  static const int ZEGO_LATENCY_MODE_NORMAL3 = 5;

}

/// 设置预览和推流镜像
class ZegoVideoMirrorMode {
    /// 预览启用镜像，推流不启用镜像
    static const int ZegoVideoMirrorModePreviewMirrorPublishNoMirror = 0;
    /// 预览启用镜像，推流启用镜像
    static const int ZegoVideoMirrorModePreviewCaptureBothMirror = 1;
    /// 预览不启用镜像，推流不启用镜像
    static const int ZegoVideoMirrorModePreviewCaptureBothNoMirror = 2;
    /// 预览不启用镜像，推流启用镜像
    static const int ZegoVideoMirrorModePreviewNoMirrorPublishMirror = 3;
}

/// 流量控制属性
class ZegoTrafficControlProperty {

  /// 基本流量控制，只有码率控制，不带自适应帧率和分辨率
  static const int ZEGOAPI_TRAFFIC_CONTROL_BASIC = 0;

  /// 自适应帧率
  static const int ZEGOAPI_TRAFFIC_CONTROL_ADAPTIVE_FPS = 1;

  /// 自适应分辨率
  static const int ZEGOAPI_TRAFFIC_CONTROL_ADAPTIVE_RESOLUTION = 1 << 1;

  /// 音频流量控制
  static const int ZEGOAPI_TRAFFIC_CONTROL_AUDIO_BITRATE = 1 << 2;

}

/// 码率控制模式
class ZegoTrafficControlMinVideoBitrateMode {

  /// 低于设置的最低码率时，停止视频发送
  static const int ZEGOAPI_TRAFFIC_CONTROL_MIN_VIDEO_BITRATE_NO_VIDEO = 0;

  /// 低于设置的最低码率时，视频以极低的频率发送 （不超过2FPS)
  static const int ZEGOAPI_TRAFFIC_CONTROL_MIN_VIDEO_BITRATE_ULTRA_LOW_FPS = 1;

}

/// 视频编码类型
class ZegoVideoCodecAvc {

  /// 不支持分层编码
  static const int VIDEO_CODEC_DEFAULT = 0;

  /// 分层编码 要达到和 VIDEO_CODEC_DEFAULT 相同的编码质量，建议码率和 VIDEO_CODEC_DEFAULT 相比增加20%左右
  static const int VIDEO_CODEC_MULTILAYER = 1;

}

/// 视频分层类型
class ZegoVideoStreamLayer {

  /// 根据网络状态选择图层
  static const int VideoStreamLayer_Auto = -1;

  /// 指定拉基本层（小分辨率）
  static const int VideoStreamLayer_BaseLayer = 0;

  /// 指定拉扩展层（大分辨率)
  static const int VideoStreamLayer_ExtendLayer = 1;

}

/// 音频设备模式
class ZegoAudioDeviceMode {

  /// 开启硬件回声消除，使用通话音量
  static const int ZEGO_AUDIO_DEVICE_MODE_COMMUNICATION = 1;

  /// 关闭硬件回声消除，使用媒体音量
  static const int ZEGO_AUDIO_DEVICE_MODE_GENERAL = 2;

  /// 根据场景自动选择是否开启硬件回声消除，根据场景自动切换
  static const int ZEGO_AUDIO_DEVICE_MODE_AUTO = 3;

  /// 开启系统回声消除，与communication相比，communication2会始终占用麦克风设备
  static const int ZEGO_AUDIO_DEVICE_MODE_COMMUNICATION2 = 4;

}

/// 手机朝向角度
class ZegoAppOrientation {

  static const int ROTATION_0 = 0;

  static const int ROTATION_90 = 1;

  static const int ROTATION_180 = 2;

  static const int ROTATION_270 = 3;

}

/// 美颜特性
class ZegoBeautifyFeature {

  /// 无美颜
  static const int ZEGO_BEAUTIFY_NONE = 0;

  /// 磨皮
  static const int ZEGO_BEAUTIFY_POLISH = 1;

  /// 全屏美白，一般与磨皮结合使用：ZEGO_BEAUTIFY_POLISH | ZEGO_BEAUTIFY_WHITEN
  static const int ZEGO_BEAUTIFY_WHITEN = 1 << 1;

  /// 皮肤美白
  static const int ZEGO_BEAUTIFY_SKINWHITEN = 1 << 2;

  /// 锐化
  static const int ZEGO_BEAUTIFY_SHARPEN = 1 << 3;

}

/// 滤镜特性
class ZegoFilter {

  /// 不使用滤镜
  static const int ZEGO_FILTER_NONE = 0;

  /// 简洁
  static const int ZEGO_FILTER_LOMO = 1;

  /// 黑白
  static const int ZEGO_FILTER_BLACKWHITE = 2;

  /// 老化
  static const int ZEGO_FILTER_OLDSTYLE = 3;

  /// 哥特
  static const int ZEGO_FILTER_GOTHIC = 4;

  ///  锐色
  static const int ZEGO_FILTER_SHARPCOLOR = 5;

  ///  淡雅
  static const int ZEGO_FILTER_FADE = 6;

  ///  酒红
  static const int ZEGO_FILTER_WINE = 7;

  ///  青柠
  static const int ZEGO_FILTER_LIME = 8;

  ///  浪漫
  static const int ZEGO_FILTER_ROMANTIC = 9;

  ///  光晕
  static const int ZEGO_FILTER_HALO = 10;

  ///  蓝调
  static const int ZEGO_FILTER_BLUE = 11;

  ///  梦幻
  static const int ZEGO_FILTER_ILLUSION = 12;

  ///  夜色
  static const int ZEGO_FILTER_DARK = 13;

}

/// 流事件类型
class ZegoLiveEvent {

  /// 播放直播开始重试
  static const int Play_BeginRetry = 1;

  /// 播放直播重试成功
  static const int Play_RetrySuccess = 2;

  /// 发布直播开始重试
  static const int Publish_BeginRetry = 3;

  /// 发布直播重试成功
  static const int Publish_RetrySuccess = 4;

  /// 拉流临时中断
  static const int Play_TempDisconnected = 5;

  /// 推流临时中断
  static const int Publish_TempDisconnected = 6;

  /// 拉流视频卡顿
  static const int Play_VideoBreak = 7;

}

class ZegoAECMode {

  /// 激进的回声抵消。可能会稍微明显影响音质，但是回声会消除的很干净
  static const int ZEGOAPI_AEC_MODE_ARRGRESSIVE = 0;

  /// 适度的回声抵消。可能会稍微影响一点点音质，但是残留的回声会更少
  static const int ZEGOAPI_AEC_MODE_MEDIUM = 1;

  /// 舒适的回声抵消。回声的抵消基本不会影响声音的音质，会残留一点回声但不影响正常听音
  static const int ZEGOAPI_AEC_MODE_SOFT = 2;

}

class ZegoDeviceStatus {

  /// 设备已打开
  static const int ZEGOAPI_DEVICE_OPEN = 0;

  /// 设备已关闭
  static const int ZEGOAPI_DEVICE_CLOSE = 1;

}



class ZegoPublishStreamQuality {

  /// 视频帧率(采集)
  final double cfps;

  /// 视频帧率(编码)
  final double vencFps;

  /// 视频帧率(网络发送)，推荐参考
  final double fps;

  /// 视频码率(kb/s)，推荐参考
  final double kbps;

  /// 音频帧率(采集)
  final double acapFps;

  /// 音频帧率(网络发送)
  final double afps;

  /// 音频码率(kb/s)
  final double akbps;

  /// 延时(ms)
  final int rtt;

  /// 丢包率(0~1)
  final double pktLostRate;

  /// 质量(0~3)，该质量为综合各种数据之后推算出来的质量参考值，其中 0 为最优，质量以此递减（优、良、中、差）
  final int quality;

  /// 是否硬编
  final bool isHardwareVenc;

  /// 视频宽度
  final int width;

  /// 视频高度
  final int height;

  const ZegoPublishStreamQuality(
      this.cfps,
      this.vencFps,
      this.fps,
      this.kbps,
      this.acapFps,
      this.afps,
      this.akbps,
      this.rtt,
      this.pktLostRate,
      this.quality,
      this.isHardwareVenc,
      this.width,
      this.height
      );
}

class ZegoPlayStreamQuality {

  /// 视频帧率(网络接收)
  final double fps;

  /// 视频帧率(dejitter抗抖动缓冲)，推荐参考
  final double vdjFps;

  /// 视频帧率(解码)
  final double vdecFps;

  /// 视频帧率(渲染)
  final double vrndFps;

  /// 视频码率(kb/s)，推荐参考
  final double kbps;

  /// 音频帧率(网络接收)
  final double afps;

  /// 音频帧率(dejitter)
  final double adjFps;

  /// 音频帧率(解码)
  final double adecFps;

  /// 音频帧率(渲染)
  final double arndFps;

  /// 音频码率(kb/s)
  final double akbps;

  /// 音频播放卡顿次数
  final double audioBreakRate;

  /// 视频播放卡顿次数
  final double videoBreakRate;

  /// 延时(ms)
  final int rtt;

  /// 端到端延时(ms)
  final int peerToPeerDelay;

  /// 丢包率(0~1)
  final double pktLostRate;

  /// 端到端丢包率(0~1)
  final double peerToPeerPktLostRate;

  /// 质量(0~3)，该质量为综合各种数据之后推算出来的质量参考值，其中 0 为最优，质量以此递减（优、良、中、差）
  final int quality;

  /// 语音延时(ms)
  final int delay;

  /// 是否硬解
  final bool isHardwareVdec;

  /// 视频宽度
  final int width;

  /// 视频高度
  final int height;

  const ZegoPlayStreamQuality(
      this.fps,
      this.vdjFps,
      this.vdecFps,
      this.vrndFps,
      this.kbps,
      this.afps,
      this.adjFps,
      this.adecFps,
      this.arndFps,
      this.akbps,
      this.audioBreakRate,
      this.videoBreakRate,
      this.rtt,
      this.peerToPeerDelay,
      this.pktLostRate,
      this.peerToPeerPktLostRate,
      this.quality,
      this.delay,
      this.isHardwareVdec,
      this.width,
      this.height
      );
}

/// 转推CDN状态
class ZegoStreamRelayCDNState {
  /// 转推停止
  static const int ZEGO_STREAM_RELAY_STATE_STOP = 0;
  /// 正在转推
  static const int ZEGO_STREAM_RELAY_STATE_START = 1;
  /// 正在重试
  static const int ZEGO_STREAM_RELAY_STATE_RETRY = 2;
}

/// 转推停止或转推重试时的详细原因
class ZegoStreamRelayCDNDetail {
  /// 无
  static const int ZEGO_STREAM_RELAY_DETAIL_NONE = 0;
  /// 服务器错误
  static const int ZEGO_STREAM_RELAY_DETAIL_SERVER_ERROR = 8;
  /// 握手失败
  static const int ZEGO_STREAM_RELAY_DETAIL_HAND_SHAKE_FAILED = 9;
  /// 接入点错误
  static const int ZEGO_STREAM_RELAY_DETAIL_ACCESS_POINT_ERROR = 10;
  /// 创建流失败
  static const int ZEGO_STREAM_RELAY_DETAIL_CREATE_STREAM_FAILED = 11;
  /// BAD NAME
  static const int ZEGO_STREAM_RELAY_DETAIL_BAD_NAME = 12;
  /// CDN服务器主动断开
  static const int ZEGO_STREAM_RELAY_DETAIL_CDN_SERVER_DISCONNECTED = 13;
  /// 主动断开
  static const int ZEGO_STREAM_RELAY_DETAIL_DISCONNECTED = 14;
  /// 混流输入流会话关闭, 混流转推CDN时有效
  static const int ZEGO_STREAM_RELAY_DETAIL_MIXSTREAM_ALL_INPUT_STREAM_CLOSED = 1214;
  /// 混流输入流全部没有数据, 混流转推CDN时有效
  static const int ZEGO_STREAM_RELAY_DETAIL_MIXSTREAM_ALL_INPUT_STREAM_NODATA = 1215;
  /// 混流服务器内部错误，混流转推CDN时有效
  static const int ZEGO_STREAM_RELAY_DETAIL_MIXSTREAM_SERVER_INTERNAL_ERROR = 1230;
}


/// 转推CDN状态信息
class ZegoStreamRelayCDNInfo {
  
  /// 转推CDN的rtmp地址
  final String rtmpURL;

  /// 当前转推状态
  final ZegoStreamRelayCDNState state;

  /// 转推停止或转推重试时的详细原因
  final ZegoStreamRelayCDNDetail detail;

  /// 状态改变时的时间
  final int stateTime;

  const ZegoStreamRelayCDNInfo(this.rtmpURL, this.state, this.detail, this.stateTime);
}

/// 添加/删除转推 CDN 结果
class ZegoStreamRelayCDNResult {

  /// 错误码，0为无错误
  final int errorCode;

  /// 转推的流 ID
  final String streamID;

  const ZegoStreamRelayCDNResult(this.errorCode, this.streamID);
}

/// 登录房间结果
class ZegoLoginRoomResult {

  /// 错误码，0为无错误
  final int errorCode;

  /// 流列表
  final List<ZegoStreamInfo> streamList;

  const ZegoLoginRoomResult(this.errorCode, this.streamList);
}

/// 房间消息
class ZegoRoomMessage {
  final String content;

  final String fromUserID;

  final String fromUserName;

  final int messageID;

  const ZegoRoomMessage(this.content, this.fromUserID, this.fromUserName, this.messageID);
}

/// 房间消息发送结果
class ZegoSendRoomMessageResult {
  final int errorCode;

  final String roomID;

  final int messageID;

  const ZegoSendRoomMessageResult(this.errorCode, this.roomID, this.messageID);
}

/// 自定义信令发送结果
class ZegoCustomCommandResult {

  /// 错误码，0为无错误
  final int errorCode;

  /// 房间ID
  final String roomID;

  const ZegoCustomCommandResult(this.errorCode, this.roomID);
}

/// 连麦响应结果
class ZegoResponseResult {

  /// 响应结果
  final int result;

  /// 来源用户的ID
  final String fromUserID;

  /// 来源用户名
  final String fromUserName;

  const ZegoResponseResult(this.result, this.fromUserID, this.fromUserName);
}

/// 结束连麦响应结果
class ZegoEndJoinLiveResult {

  /// 错误码，0为无错误
  final int errorCode;

  /// 房间ID
  final String roomID;

  const ZegoEndJoinLiveResult(this.errorCode, this.roomID);
}

class ZegoAudioReverbMode {

  static const int ZEGO_AUDIO_REVERB_MODE_SOFT_ROOM = 0;

  static const int ZEGO_AUDIO_REVERB_MODE_WARM_CLUB = 1;

  static const int ZEGO_AUDIO_REVERB_MODE_CONCERT_HALL = 2;

  static const int ZEGO_AUDIO_REVERB_MODE_LARGE_AUDITORIUM = 3;
}

class ZegoAudioReverbParam {

  /// 房间大小，取值范围[0.0, 1.0]，用于控制产生混响"房间"的大小，房间越大，混响越强
  double roomSize;
  
  /// 余响，取值范围[0.0, 0.5]，用于控制混响的拖尾长度
  double reverberance;

  /// 混响阻尼， 取值范围[0.0， 2.0]，控制混响的衰减程度，阻尼越大，衰减越大
  double damping;

  /// 干湿比，取值范围 >= 0.0。 控制混响与直达声和早期反射声之间的比例，干(dry)的部分默认定为1，当干湿比设为较小时，湿(wet)的比例较大，此时混响较强
  double dryWetRatio;

  ZegoAudioReverbParam(this.roomSize, this.reverberance, this.damping, this.dryWetRatio);

  Map<String, dynamic> toMap() {
    return {
      'roomSize': roomSize,
      'reverberance': reverberance,
      'damping': damping,
      'dryWetRatio': dryWetRatio
    };
  }
}
