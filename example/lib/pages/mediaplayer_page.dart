import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';

import 'package:zegoliveroom_plugin_example/config/zego_config.dart';
import 'package:zegoliveroom_plugin/zegoliveroom_plugin.dart';

class MediaPlayerPage extends StatefulWidget {
  final int screenWidthPx;
  final int screenHeightPx;

  MediaPlayerPage(this.screenWidthPx, this.screenHeightPx);

  @override
  _MediaPlayerPageState createState() => new _MediaPlayerPageState();
}

class _MediaPlayerPageState extends State<MediaPlayerPage> {
  int _playTextureID = -1;
  bool isStop = false;

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    ZegoMediaplayer.registerMediaPlayerCallback(
      onPlayBegin: onPlayBegin,
      onSeekComplete: onSeekComplete,
      onPlayPause: onPlayPause,
      onPlayResume: onPlayResume,
      onPlayStop: onPlayStop,
    );
    ZegoMediaplayer.start("https://storage.zego.im/demo/201808270915.mp4",
        isRepeat: true, pathMode: ZegoPathMode.LOCAL);
    ZegoMediaplayer.setPlayerType(
        ZegoMediaPlayerType.ZEGO_MEDIA_PLAYER_TYPE_AUX_WITH_STREAM);
    ZegoMediaplayer.createMediaPlayerRenderer(
            widget.screenWidthPx, widget.screenHeightPx)
        .then((value) {
      setState(() {
        _playTextureID = value;
      });
    });
    ZegoLiveRoomPublisherPlugin.startPublishing(
        "1", "", ZegoPublishFlag.ZEGO_JOIN_PUBLISH);
  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();
    ZegoMediaplayer.destroyMediaPlayerRenderer();
    ZegoLiveRoomPlugin.logoutRoom();
  }

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return Scaffold(
        resizeToAvoidBottomInset: false,
        appBar: AppBar(
          title: Text("mediaplayer"),
        ),
        body: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Container(
              width: MediaQuery.of(context).size.width,
              height: (MediaQuery.of(context).size.height -
                      MediaQuery.of(context).padding.top) /
                  2,
              child: _playTextureID >= 0
                  ? Texture(
                      textureId: _playTextureID,
                    )
                  : null,
            ),
            Padding(
              padding: const EdgeInsets.only(bottom: 10.0),
            ),
            CupertinoButton(
                color: Color(0xff0e88eb),
                child: Text('pause/resume'),
                onPressed: () {
                  ZegoMediaplayer.isPlaying().then((ret) {
                    ret ? ZegoMediaplayer.pause() : ZegoMediaplayer.resume();
                  });
                }),
            Padding(
              padding: const EdgeInsets.only(bottom: 10.0),
            ),
            CupertinoButton(
                color: Color(0xff0e88eb),
                child: Text('stop'),
                onPressed: () {
                  ZegoMediaplayer.stop();
                }),
          ],
        ));
  }

  void onPlayBegin() {
    print("播放开始");
    ZegoMediaplayer.seekTo(5000);
  }

  void onSeekComplete(int errorCode, int millisecond) {
    print("seek 成功，");
    print(millisecond);
  }

  void onPlayPause() {
    print("播放暂停");
  }

  void onPlayResume() {
    print("播放恢复");
  }

  void onPlayStop() {
    print("播放停止");
  }
}
