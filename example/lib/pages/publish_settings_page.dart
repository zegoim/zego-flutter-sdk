import 'dart:io';

import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:path_provider/path_provider.dart';
import 'package:zegoliveroom_plugin/zegoliveroom_plugin.dart';
import 'package:zegoliveroom_plugin_example/config/zego_config.dart';

class PublishSettingsPage extends StatefulWidget {

  @override
  _PublishSettingsPageState createState() => new _PublishSettingsPageState();
}

class _PublishSettingsPageState extends State<PublishSettingsPage> {

  bool _isPreviewMirror;
  bool _isPublishMirror;

  bool _enableCamera;
  bool _isSetDummyCaptureImage;
  bool _enableVirtualStereo;
  double _virtualStereoAngle;
  bool _enableReverb;
  int _reverbMode;
  String _reverbDetail;
  double _voiceChanger;

  @override
  void initState() {
    // TODO: implement initState
    super.initState();

    _isPreviewMirror = ZegoConfig.getInstance().isPreviewMirror;
    _isPublishMirror = ZegoConfig.getInstance().isPublishMirror;

    _enableCamera = ZegoConfig.getInstance().enableCamera;
    _isSetDummyCaptureImage = ZegoConfig.getInstance().isSetDummyCaptureImage;
    _enableVirtualStereo = ZegoConfig.getInstance().enableVirtualStereo;
    _virtualStereoAngle = ZegoConfig.getInstance().virtualStereoAngle;
    _enableReverb = ZegoConfig.getInstance().enableReverb;
    _reverbMode = ZegoConfig.getInstance().reverbMode;
    _voiceChanger = ZegoConfig.getInstance().voiceChangerValue;

    switch(_reverbMode) {
      case ZegoAudioReverbMode.ZEGO_AUDIO_REVERB_MODE_SOFT_ROOM:
        _reverbDetail = 'Soft Room';
        break;
      case ZegoAudioReverbMode.ZEGO_AUDIO_REVERB_MODE_WARM_CLUB:
        _reverbDetail = 'Warm Club';
        break;
      case ZegoAudioReverbMode.ZEGO_AUDIO_REVERB_MODE_CONCERT_HALL:
        _reverbDetail = 'Concert Hall';
        break;
      case ZegoAudioReverbMode.ZEGO_AUDIO_REVERB_MODE_LARGE_AUDITORIUM:
        _reverbDetail = 'Large Auditorium';
        break;
      default:
        _reverbDetail = 'Unknown';
        break;
    }
  }

  void checkMirrorMode() {
    if(!_isPreviewMirror && !_isPublishMirror) {
      ZegoLiveRoomPublisherPlugin.setVideoMirrorMode(ZegoVideoMirrorMode.ZegoVideoMirrorModePreviewCaptureBothNoMirror);
    } else if(!_isPreviewMirror && _isPublishMirror) {
      ZegoLiveRoomPublisherPlugin.setVideoMirrorMode(ZegoVideoMirrorMode.ZegoVideoMirrorModePreviewNoMirrorPublishMirror);
    } else if(_isPreviewMirror && !_isPublishMirror) {
      ZegoLiveRoomPublisherPlugin.setVideoMirrorMode(ZegoVideoMirrorMode.ZegoVideoMirrorModePreviewMirrorPublishNoMirror);
    } else {
      ZegoLiveRoomPublisherPlugin.setVideoMirrorMode(ZegoVideoMirrorMode.ZegoVideoMirrorModePreviewCaptureBothMirror);
    }
  }
  void onPreviewMirrorValueChanged(bool value) {
    setState(() {
      _isPreviewMirror = value;
      ZegoConfig.getInstance().isPreviewMirror = _isPreviewMirror;
      checkMirrorMode();
    });
  }

  void onPublishMirrorValueChanged(bool value) {
    setState(() {
      _isPublishMirror = value;
      ZegoConfig.getInstance().isPublishMirror = _isPublishMirror;
      checkMirrorMode();
    });
  }

  void onCamaraEnabledChanged(bool value) {
    setState(() {
      _enableCamera = value;
      ZegoConfig.getInstance().enableCamera = _enableCamera;
      ZegoLiveRoomPublisherPlugin.enableCamera(_enableCamera);
    });
  }

  void onSetDummyCaptureImageChanged(bool value) async {
    setState(() {
      _isSetDummyCaptureImage = value;
      ZegoConfig.getInstance().isSetDummyCaptureImage= _isSetDummyCaptureImage;
    });

    if (_isSetDummyCaptureImage) {
      Directory directory = await getApplicationDocumentsDirectory();
      String imagePath = directory.path +  "/zegologo.jpg";
      print("🧪 SetDummyCaptureImagePath: " + imagePath);
      ByteData data = await rootBundle.load("resources/images/zegologo.jpg");
      List<int> bytes = data.buffer.asUint8List(data.offsetInBytes, data.lengthInBytes);
      await File(imagePath).writeAsBytes(bytes);
      ZegoLiveRoomPublisherPlugin.setDummyCaptureImagePath("file:" + imagePath);
    } else {
      ZegoLiveRoomPublisherPlugin.setDummyCaptureImagePath("");
    }
  }

  void onVirtualEnableChanged(bool value) {
    setState(() {
      _enableVirtualStereo = value;
      ZegoLiveRoomAudioPlugin.enableVirtualStereo(_enableVirtualStereo, _virtualStereoAngle.toInt());
      ZegoConfig.getInstance().enableVirtualStereo = _enableVirtualStereo;
      if(_enableVirtualStereo) {
        /// 开启虚拟立体声前需要设置双声道编码
        ZegoLiveRoomPublisherPlugin.setAudioChannelCount(2);
      } else {
        ZegoLiveRoomPublisherPlugin.setAudioChannelCount(1);
      }
    });
  }
  void onVirtualStereoAngle(double value) {
    setState(() {
      _virtualStereoAngle = value;
      ZegoLiveRoomAudioPlugin.enableVirtualStereo(_enableVirtualStereo, _virtualStereoAngle.toInt());
      ZegoConfig.getInstance().virtualStereoAngle = _virtualStereoAngle;
    });
  }

  void onReverbEnableChanged(bool value) {
    setState(() {
      _enableReverb = value;
      ZegoLiveRoomAudioPlugin.enableReverb(_enableReverb, _reverbMode);
      ZegoConfig.getInstance().enableReverb = _enableReverb;
    });
  }

  void onSelectReverbMode(String action) {

      switch(action) {
        case 'SOFTROOM':
          ZegoLiveRoomAudioPlugin.enableReverb(_enableReverb, ZegoAudioReverbMode.ZEGO_AUDIO_REVERB_MODE_SOFT_ROOM);
          setState(() {
            _reverbDetail = 'Soft Room';
          });
          ZegoConfig.getInstance().reverbMode = ZegoAudioReverbMode.ZEGO_AUDIO_REVERB_MODE_SOFT_ROOM;
          break;
        case 'WARMCLUB':
          ZegoLiveRoomAudioPlugin.enableReverb(_enableReverb, ZegoAudioReverbMode.ZEGO_AUDIO_REVERB_MODE_WARM_CLUB);
          setState(() {
            _reverbDetail = 'Warn Club';
          });
          ZegoConfig.getInstance().reverbMode = ZegoAudioReverbMode.ZEGO_AUDIO_REVERB_MODE_WARM_CLUB;
          break;
        case 'CONCERTHALL':
          ZegoLiveRoomAudioPlugin.enableReverb(_enableReverb, ZegoAudioReverbMode.ZEGO_AUDIO_REVERB_MODE_CONCERT_HALL);
          setState(() {
            _reverbDetail = 'Concert Hall';
          });
          ZegoConfig.getInstance().reverbMode = ZegoAudioReverbMode.ZEGO_AUDIO_REVERB_MODE_CONCERT_HALL;
          break;
        case 'LARGEAUDITORIUM':
          ZegoLiveRoomAudioPlugin.enableReverb(_enableReverb, ZegoAudioReverbMode.ZEGO_AUDIO_REVERB_MODE_LARGE_AUDITORIUM);
          setState(() {
            _reverbDetail = 'Large Auditorium';
          });
          ZegoConfig.getInstance().reverbMode = ZegoAudioReverbMode.ZEGO_AUDIO_REVERB_MODE_LARGE_AUDITORIUM;
          break;
        default:
          break;
      }

  }

  void onVoiceChangerChanged(double value) {
    setState(() {
      _voiceChanger = value;
      ZegoLiveRoomAudioPlugin.setVoiceChangerParam(_voiceChanger);
      ZegoConfig.getInstance().voiceChangerValue = _voiceChanger;
    });
  }

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return Scaffold(
      appBar: AppBar(
        title: Text('设置'),
      ),
      body: Container(
        child: ListView(
          children: <Widget>[
            /// 镜像模式
            Container(
              padding: EdgeInsets.symmetric(horizontal: 20.0),
              decoration: BoxDecoration(
                border: Border(
                  bottom: BorderSide(
                    color: Colors.black26
                  )
                )
              ),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: <Widget>[
                  Text(
                    '镜像模式',
                    style: TextStyle(
                        fontSize: 20.0
                    ),
                  ),
                  Column(
                    children: <Widget>[
                      Row(
                        children: <Widget>[
                          Text(
                              '是否预览镜像'
                          ),
                          CupertinoSwitch(
                            value: _isPreviewMirror,
                            onChanged: onPreviewMirrorValueChanged,
                          )
                        ],
                      ),
                      Row(
                        children: <Widget>[
                          Text(
                              '是否推流镜像'
                          ),
                          CupertinoSwitch(
                            value: _isPublishMirror,
                            onChanged: onPublishMirrorValueChanged,
                          )
                        ],
                      ),
                    ],
                  )
                ],
              ),
            ),
            /// enable camera
            Container(
              padding: EdgeInsets.symmetric(horizontal: 20.0),
              decoration: BoxDecoration(
                border: Border(
                  bottom: BorderSide(
                    color: Colors.black26
                  )
                )
              ),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: <Widget>[
                  Text(
                    'EnableCamera',
                    style: TextStyle(
                      fontSize: 20.0
                    ),
                  ),
                  CupertinoSwitch(
                    value: _enableCamera,
                    onChanged: onCamaraEnabledChanged,
                  )
                ],
              ),
            ),
            /// set dummy capture image
            Container(
              padding: EdgeInsets.symmetric(horizontal: 20.0),
              decoration: BoxDecoration(
                border: Border(
                  bottom: BorderSide(
                    color: Colors.black26
                  )
                )
              ),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: <Widget>[
                  Text(
                    'SetDummyCaptureImage',
                    style: TextStyle(
                      fontSize: 20.0
                    ),
                  ),
                  CupertinoSwitch(
                    value: _isSetDummyCaptureImage,
                    onChanged: onSetDummyCaptureImageChanged,
                  )
                ],
              ),
            ),
            /// 虚拟立体声
            Container(
              decoration: BoxDecoration(
                  border: Border(
                      bottom: BorderSide(
                          color: Colors.black26
                      )
                  )
              ),
              padding: EdgeInsets.symmetric(horizontal: 20.0),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: <Widget>[
                  Text(
                    '虚拟立体声',
                    style: TextStyle(
                        fontSize: 20.0
                    ),
                  ),
                  Column(
                    children: <Widget>[
                      CupertinoSwitch(
                        value: _enableVirtualStereo,
                        onChanged: onVirtualEnableChanged,
                      ),
                      CupertinoSlider(
                        value: _virtualStereoAngle,
                        max: 180.0,
                        min: 0.0,
                        onChanged: onVirtualStereoAngle,
                      )
                    ],
                  )
                ],
              ),
            ),
            /// 混响
            Container(
              decoration: BoxDecoration(
                  border: Border(
                      bottom: BorderSide(
                          color: Colors.black26
                      )
                  )
              ),
              padding: EdgeInsets.symmetric(horizontal: 20.0, vertical: 10.0),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: <Widget>[
                  Text(
                    '混响',
                    style: TextStyle(
                        fontSize: 20.0
                    ),
                  ),

                  Column(
                    children: <Widget>[
                      CupertinoSwitch(
                        value: _enableReverb,
                        onChanged: onReverbEnableChanged,
                      ),
                      PopupMenuButton(
                        child: Text(_reverbDetail, style: TextStyle(color: CupertinoColors.activeBlue, fontSize: 18.0),),
                        itemBuilder: (BuildContext context) {
                          return <PopupMenuItem<String>>[
                            PopupMenuItem<String>(child: Text('Soft Room'), value: 'SOFTROOM',),
                            PopupMenuItem<String>(child: Text('Warn Club'), value: 'WARMCLUB',),
                            PopupMenuItem<String>(child: Text('Concert Hall'), value: 'CONCERTHALL',),
                            PopupMenuItem<String>(child: Text('Large Auditorium'), value: 'LARGEAUDITORIUM',),
                          ];
                        },
                        onSelected: onSelectReverbMode
                      )
                    ],
                  )
                ],
              ),
            ),
            /// 变声
            Container(
              decoration: BoxDecoration(
                  border: Border(
                      bottom: BorderSide(
                          color: Colors.black26
                      )
                  )
              ),
              padding: EdgeInsets.symmetric(horizontal: 20.0, vertical: 10.0),
              child: Row(
                children: <Widget>[
                  Text(
                    '变声',
                    style: TextStyle(
                        fontSize: 20.0
                    ),
                  ),
                  CupertinoSlider(
                    value: _voiceChanger,
                    max: 8.0,
                    min: -8.0,
                    onChanged: onVoiceChangerChanged,
                  )
                ],
              ),
            )
          ],
        ),
      ),
    );
  }

}