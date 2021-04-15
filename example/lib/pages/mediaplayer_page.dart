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

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    ZegoMediaplayer.start("https://storage.zego.im/demo/201808270915.mp4",
        isRepeat: true, isAsset: false);
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
        resizeToAvoidBottomPadding: false,
        appBar: AppBar(
          title: Text("mediaplayer"),
        ),
        body: Stack(
          children: <Widget>[
            Container(
              width: MediaQuery.of(context).size.width,
              height: MediaQuery.of(context).size.height -
                  MediaQuery.of(context).padding.top,
              child: _playTextureID >= 0
                  ? Texture(
                      textureId: _playTextureID,
                    )
                  : null,
            ),
          ],
        ));
  }
}
