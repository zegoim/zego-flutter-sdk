import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';

import 'package:zegoliveroom_plugin/zegoliveroom_plugin.dart';
import 'package:example/config/zego_config.dart';
import 'package:example/pages/publish_settings_page.dart';

class PublishStreamPage extends StatefulWidget {

  final int screenWidthPx;
  final int screenHeightPx;

  PublishStreamPage(this.screenWidthPx, this.screenHeightPx);

  @override
  _PublishStreamPageState createState() => new _PublishStreamPageState();
}

class _PublishStreamPageState extends State<PublishStreamPage> {

  String _title = '';
  int _publishTextureID = -1;
  bool _isPublishing = false;

  int _publishWidth = 0;
  int _publishHeight = 0;
  double _publishFps = 0.0;
  double _publishBitrate = 0.0;

  bool _isUseMic = true;
  bool _isUseFrontCamera = true;

  TextEditingController _controller = new TextEditingController();

  @override
  void initState() {
    // TODO: implement initState
    super.initState();

    _title = '第三步 发起推流';

    if(ZegoConfig.getInstance().streamID.isNotEmpty) {
      _controller.text = ZegoConfig.getInstance().streamID;
    }

    ZegoLiveRoomPublisherPlugin.registerPublisherCallback(
      onPublishStateUpdate: onPublishStateUpdate,
      onPublishQualityUpdate: onPublishQualityUpdate,
    );

    ZegoLiveRoomPublisherPlugin.createPreviewRenderer(widget.screenWidthPx , widget.screenHeightPx).then((textureID) {
      print('创建预览渲染器，ID: $textureID');
      setState(() {
        _publishTextureID = textureID;
      });
    });

    ZegoLiveRoomPublisherPlugin.setPreviewViewMode(ZegoViewMode.ZegoRendererScaleAspectFill);
    ZegoLiveRoomPublisherPlugin.startPreview();

  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();

    if(_isPublishing) {
      ZegoLiveRoomPublisherPlugin.stopPublishing().then((success) {
        print('停止推流${success?'成功':'失败'}');
      });
    }

    ZegoLiveRoomPublisherPlugin.stopPreview();
    ZegoLiveRoomPublisherPlugin.unregisterPublisherCallback();
    ZegoLiveRoomPublisherPlugin.destroyPreviewRenderer().then((success) {
      print('销毁预览渲染器${success?'成功':'失败'}');
    });

    ZegoLiveRoomPlugin.logoutRoom();

  }

  void onPublishButtonPressed() {

    String streamID = _controller.text.trim();

    ZegoLiveRoomPublisherPlugin.startPublishing(streamID, 'flutter-example', ZegoPublishFlag.ZEGO_JOIN_PUBLISH);

  }

  void onCamStateChanged() {

    _isUseFrontCamera = !_isUseFrontCamera;
    ZegoLiveRoomPublisherPlugin.setFrontCam(_isUseFrontCamera);
  }

  void onMicStateChanged() {

    setState(() {
      _isUseMic = !_isUseMic;
      ZegoLiveRoomPublisherPlugin.enableMic(_isUseMic);
    });
  }

  void onVideoMirroModeChanged(int mode) {
    ZegoLiveRoomPublisherPlugin.setVideoMirrorMode(mode);
  }

  void onPublishStateUpdate(int stateCode, String streamID, Map<String, dynamic> info) {

    if(stateCode == 0) {

      setState(() {
        _isPublishing = true;
        _title = '推流中';
      });

      ZegoConfig.getInstance().streamID = streamID;
      ZegoConfig.getInstance().saveConfig();

    } else {

      print('推流失败，错误码: $stateCode');
    }
  }

  void onPublishQualityUpdate(String streamID, ZegoPublishStreamQuality quality) {
    setState(() {
      _publishWidth = quality.width;
      _publishHeight = quality.height;
      _publishFps = quality.fps;
      _publishBitrate = quality.kbps;
    });
  }

  Widget showPreviewToolPage() {
    return GestureDetector(

      behavior: HitTestBehavior.translucent,
      onTap: () => FocusScope.of(context).requestFocus(new FocusNode()),

      child: Container(
        padding: const EdgeInsets.symmetric(horizontal: 30.0),
        child: Column(
          children: <Widget>[
            Padding(
              padding: const EdgeInsets.only(top: 50.0),
            ),
            Row(
              children: <Widget>[
                Text('streamID: ',
                  style: TextStyle(
                    color: Colors.white
                  ),
                )
              ],
            ),
            Padding(
              padding: const EdgeInsets.only(bottom: 10.0),
            ),
            TextField(
              controller: _controller,
              style: TextStyle(
                color: Colors.white
              ),
              decoration: InputDecoration(
                  contentPadding: const EdgeInsets.only(left: 10.0, top: 12.0, bottom: 12.0),
                  hintText: '请输入streamID',
                  hintStyle: TextStyle(
                    color: Color.fromRGBO(255, 255, 255, 0.8)
                  ),
                  enabledBorder: OutlineInputBorder(
                    borderSide: BorderSide(
                      color: Colors.white
                    )
                  ),
                  focusedBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                          color: Color(0xff0e88eb)
                      )
                  )
              ),
            ),
            Padding(
              padding: const EdgeInsets.only(bottom: 10.0),
            ),
            Text('每个用户的流名必须保持唯一，也就是流名必须全局唯一，sdk推拉流都是基于流ID来标识。长度不超过 255 bytes, 且不要包含特殊字符。',
              style: TextStyle(
                color: Colors.white
              ),
            ),
            Padding(
              padding: const EdgeInsets.only(bottom: 30.0),
            ),
            Container(
              padding: const EdgeInsets.all(0.0),
              decoration: BoxDecoration(
                borderRadius: BorderRadius.circular(12.0),
                color: Color(0xee0e88eb),
              ),
              width: 240.0,
              height: 60.0,
              child: CupertinoButton(
                child: Text('发起推流',
                  style: TextStyle(
                      color: Colors.white
                  ),
                ),
                onPressed: onPublishButtonPressed,
              ),
            )
          ],
        ),
      ),
    );
  }

  Widget showPublishingToolPage() {
    return Container(
      padding: EdgeInsets.only(left: 10.0, right: 10.0, bottom: MediaQuery.of(context).padding.bottom + 20.0),
      child: Column(
        children: <Widget>[
          Padding(
            padding: const EdgeInsets.only(top: 10.0),
          ),
          Row(
            children: <Widget>[
              Text('roomID:  ${ZegoConfig.getInstance().roomID}',
                style: TextStyle(
                    color: Colors.white
                ),
              ),
            ],
          ),

          Row(
            children: <Widget>[
              Text('streamID:  ${ZegoConfig.getInstance().streamID}',
                style: TextStyle(
                  color: Colors.white
                ),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('分辨率:  ${_publishWidth} x ${_publishHeight}',
                style: TextStyle(
                    color: Colors.white
                ),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('码率:  ${_publishBitrate.toStringAsFixed(2)} kb/s',
                style: TextStyle(
                    color: Colors.white
                ),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('帧率:  ${_publishFps.toStringAsFixed(2)}',
                style: TextStyle(
                    color: Colors.white
                ),
              ),
            ],
          ),
          Expanded(
            child: Padding(padding: const EdgeInsets.only(top: 10.0)),
          ),
          Row(
            children: <Widget>[
              CupertinoButton(
                padding: const EdgeInsets.all(0.0),
                pressedOpacity: 1.0,
                borderRadius: BorderRadius.circular(
                    0.0),
                child: Image(
                  width: 44.0,
                  image:AssetImage('resources/images/bottom_switchcamera_icon.png'),
                ),
                onPressed: onCamStateChanged,
              ),
              Padding(
                padding: const EdgeInsets.only(left: 10.0),
              ),
              CupertinoButton(
                padding: const EdgeInsets.all(0.0),
                pressedOpacity: 1.0,
                borderRadius: BorderRadius.circular(
                    0.0),
                child: Image(
                  width: 44.0,
                  image: _isUseMic
                      ? AssetImage('resources/images/bottom_microphone_on_icon.png')
                      : AssetImage('resources/images/bottom_microphone_off_icon.png'),
                ),
                onPressed: onMicStateChanged,
              ),
              Padding(
                padding: const EdgeInsets.only(left: 10.0),
              )
            ],
          ),
        ],
      ),
    );
  }

  void onSettingsButtonClicked() {
    Navigator.push(context, MaterialPageRoute(builder: (BuildContext context) {
      return PublishSettingsPage();
    },fullscreenDialog: true));
  }

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return Scaffold(
      resizeToAvoidBottomPadding: false,
      appBar: AppBar(
        title: Text(_title),
      ),
      floatingActionButton: CupertinoButton(
          child: Text(
            '设置',
            style: TextStyle(
              color: Colors.white
            ),
          ),
          onPressed: onSettingsButtonClicked
      ),
      body: Stack(
        children: <Widget>[
          Container(
            width: MediaQuery.of(context).size.width,
            height: MediaQuery.of(context).size.height - MediaQuery.of(context).padding.top,
            child: _publishTextureID >= 0 ? Texture(textureId: _publishTextureID,) : null,
          ),
          _isPublishing ? showPublishingToolPage() : showPreviewToolPage(),
        ],
      )
    );
  }

}