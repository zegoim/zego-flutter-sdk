import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';

import 'package:zegoliveroom_plugin_example/config/zego_config.dart';
import 'package:zegoliveroom_plugin/zegoliveroom_plugin.dart';

class PlayStreamPlatformViewPage extends StatefulWidget {

  final int screenWidthPx;
  final int screenHeightPx;

  PlayStreamPlatformViewPage(this.screenWidthPx, this.screenHeightPx);

  @override
  _PlayStreamPlatformViewPageState createState() => new _PlayStreamPlatformViewPageState();
}

class _PlayStreamPlatformViewPageState extends State<PlayStreamPlatformViewPage> {

  String _title = '';
  int _viewID = -1;
  bool _isPlaying = false;

  int _playWidth = 0;
  int _playHeight = 0;
  double _playFps = 0.0;
  double _playBitrate = 0.0;

  bool _isUseSpeaker = true;

  TextEditingController _controller = new TextEditingController();

  Widget _playView;

  @override
  void initState() {
    // TODO: implement initState
    super.initState();

    _title = '第三步 发起拉流';

    if(ZegoConfig.getInstance().streamID.isNotEmpty) {
      _controller.text = ZegoConfig.getInstance().streamID;
    }

    ZegoLiveRoomPlayerPlugin.registerPlayerCallback(
      onPlayStateUpdate: onPlayStateUpdate,
      onPlayQualityUpdate: onPlayQualityUpdate,
    );

  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();

    if(_isPlaying) {
      ZegoLiveRoomPlayerPlugin.stopPlayingStream(ZegoConfig.getInstance().streamID);
    }

    ZegoLiveRoomPlayerPlugin.removePlayPlatformView(_viewID);

    ZegoLiveRoomPlayerPlugin.unregisterPlayerCallback();
    ZegoLiveRoomPlugin.logoutRoom();
  }

  void onPlayButtonPressed() {

    String streamID = _controller.text.trim();

    setState(() {
      _playView = ZegoLiveRoomPlayerPlugin.createPlayPlatformView(streamID, (int viewID) {

        print('platform view created, id: $viewID');
        _viewID = viewID;
        ZegoLiveRoomPlayerPlugin.startPlayingStream(streamID, viewID: _viewID).then((success) {
          ZegoLiveRoomPlayerPlugin.setPlatformViewPlayViewMode(streamID, ZegoViewMode.ZegoRendererScaleAspectFill);
        });

      });
    });

  }

  void onSpeakerStateChanged() {
    setState(() {
      _isUseSpeaker = !_isUseSpeaker;
      ZegoLiveRoomPlayerPlugin.enableSpeaker(_isUseSpeaker);
    });
  }

  void onPlayStateUpdate(int stateCode, String streamID) {

    if(stateCode == 0) {

      setState(() {
        _isPlaying = true;
        _title = '拉流中';
      });

      ZegoConfig.getInstance().streamID = streamID;
      ZegoConfig.getInstance().saveConfig();
    } else {

      print('拉流失败，错误码: $stateCode');
    }
  }

  void onPlayQualityUpdate(String streamID, ZegoPlayStreamQuality quality) {
    setState(() {
      _playWidth = quality.width;
      _playHeight = quality.height;
      _playBitrate = quality.kbps;
      _playFps = quality.vdjFps;
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

                )
              ],
            ),
            Padding(
              padding: const EdgeInsets.only(bottom: 10.0),
            ),
            TextField(
              controller: _controller,

              decoration: InputDecoration(
                  contentPadding: const EdgeInsets.only(left: 10.0, top: 12.0, bottom: 12.0),
                  hintText: '请输入streamID',
                  enabledBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                          color: Colors.grey
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
                child: Text('发起拉流',
                  style: TextStyle(
                      color: Colors.white
                  ),
                ),
                onPressed: onPlayButtonPressed,
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
              Text('分辨率:  $_playWidth x $_playHeight',
                style: TextStyle(
                    color: Colors.white
                ),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('码率:  ${_playBitrate.toStringAsFixed(2)} kb/s',
                style: TextStyle(
                    color: Colors.white
                ),
              ),
            ],
          ),
          Row(
            children: <Widget>[
              Text('帧率:  ${_playFps.toStringAsFixed(2)}',
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
                  image: _isUseSpeaker
                      ? AssetImage('resources/images/bottom_microphone_on_icon.png')
                      : AssetImage('resources/images/bottom_microphone_off_icon.png'),
                ),
                onPressed: onSpeakerStateChanged,
              ),
            ],
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return Scaffold(
        resizeToAvoidBottomInset: false,
        appBar: AppBar(
          title: Text(_title),
        ),
        body: Stack(
          children: <Widget>[
            Container(
              width: MediaQuery.of(context).size.width,
              height: MediaQuery.of(context).size.height - MediaQuery.of(context).padding.top,
              child: _playView,
            ),
            _isPlaying ? showPublishingToolPage() : showPreviewToolPage(),
          ],
        )
    );
  }

}