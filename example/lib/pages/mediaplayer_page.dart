import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter_seekbar/flutter_seekbar.dart';

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

  int mediaPlayerFirst = 0;
  int mediaPlayerSecond = 1;
  int mediaPlayerThird = 2;
  int mediaPlayerFourth = 3;
  List<Timer> _timers;
  bool isSeek1 = false;
  bool isSeek2 = false;
  bool isSeek3 = false;
  bool isSeek4 = false;
  List<Widget> mediaViews = [];
  List<int> mediaViews2 = [];


  final List<String> urls = [
    'https://storage.zego.im/demo/201808270915.mp4',
    'https://vd2.bdstatic.com//mda-mfr74ncascyftr97//cae_h264//1624684012872717635//mda-mfr74ncascyftr97.mp4',
    'https://vd2.bdstatic.com//mda-jhiah1vrpwck38um//mda-jhiah1vrpwck38um.mp4',
    'https://vd3.bdstatic.com//mda-mfk9r09y30pqe90v//360p//h264//1624258357133951364//mda-mfk9r09y30pqe90v.mp4'
  ];

  Widget mediaFirstView;
  Widget mediaSecondView;
  Widget mediaThirdView;
  Widget mediaFourthView;
  final int MediaPlayerCout = 4;

  List<MediaPlayerInfo> mediaPlayerInfos = [];

  @override
  void initState() {
    // TODO: implement initState
    super.initState();

    _createPlatformView();

    ZegoMediaplayer.registerMediaPlayerCallback(
      onPlayBegin: onPlayBegin,
      onSeekComplete: onSeekComplete,
      onPlayPause: onPlayPause,
      onPlayResume: onPlayResume,
      onPlayStop: onPlayStop,
      onProcessInterval: onProcessInterval,
      onBufferEnd: onBufferEnd,
      onVideoBegin: onVideoBegin,
      onPlayEnd: onPlayEnd,
      onPlayError: onPlayError,
      onAudioBegin: onAudioBegin,
      onBufferBegin: onBufferBegin,
    );
  }
    void _createPlatformView() {

    if(ZegoConfig.getInstance().enablePlatformView){

      for(int i=0;i<MediaPlayerCout;i++){
        mediaFirstView = ZegoMediaplayer.createMediaPlayerPlatformView((viewID) => {
          {
            print("platformViewID:$viewID"),
            setState(() {
              ZegoMediaplayer.startRenderPlatformView(playerIndex: i, viewID: viewID);
            })
          }
        }, playerIndex: i);

        ZegoMediaplayer.start(urls[i], playerIndex: i,isRepeat: false,isAsset: false);
        ZegoMediaplayer.setPlayerType(
            ZegoMediaPlayerType.ZEGO_MEDIA_PLAYER_TYPE_AUX_WITH_STREAM, playerIndex: i);
        ZegoMediaplayer.setProcessInterval(1000, playerIndex: i);

        mediaViews.add(mediaFirstView);
        MediaPlayerInfo mediaPlayerInfo = MediaPlayerInfo(i, PlayerState.play,
          preView: Container(
              width: 400,
              height: 250,
              child: mediaViews[i]!=null?mediaViews[i]:Container(child: Text("视图未创建"))
          ),
        );
        mediaPlayerInfos.add(mediaPlayerInfo);
      }

    }else {
      ZegoMediaplayer.createMediaPlayerRenderer(400, 250).then((value) => {
        mediaViews2.add(value),
        mediaPlayerInfos.add(MediaPlayerInfo(0, PlayerState.play,
          preView: Container(
              width: 400,
              height: 250,
              child: mediaViews2[0]!=null ? Texture(textureId: mediaViews2[0]) : Container(child: Text("视图未创建"))
          ),
        )),
        ZegoMediaplayer.start(urls[0], isRepeat: false,isAsset: false),
        ZegoMediaplayer.setPlayerType(
            ZegoMediaPlayerType.ZEGO_MEDIA_PLAYER_TYPE_AUX_WITH_STREAM),
        ZegoMediaplayer.setProcessInterval(1000),
      });

      ZegoMediaplayer.createMediaPlayerRenderer(400, 250, playerIndex: 1).then((value) => {
        mediaViews2.add(value),
        mediaPlayerInfos.add(MediaPlayerInfo(1, PlayerState.play,
          preView: Container(
              width: 400,
              height: 250,
              child: mediaViews2[1]!=null ? Texture(textureId: mediaViews2[1]) : Container(child: Text("视图未创建"))
          ),
        )),
        ZegoMediaplayer.start(urls[1], playerIndex: 1,isRepeat: false,isAsset: false),
        ZegoMediaplayer.setPlayerType(
            ZegoMediaPlayerType.ZEGO_MEDIA_PLAYER_TYPE_AUX_WITH_STREAM,playerIndex: 1),
        ZegoMediaplayer.setProcessInterval(1000, playerIndex: 1),
      });

      ZegoMediaplayer.createMediaPlayerRenderer(400, 250, playerIndex: 2).then((value) => {
        mediaViews2.add(value),
        mediaPlayerInfos.add(MediaPlayerInfo(2, PlayerState.play,
          preView: Container(
              width: 400,
              height: 250,
              child: mediaViews2[2]!=null ? Texture(textureId: mediaViews2[2]) : Container(child: Text("视图未创建"))
          ),
        )),
        ZegoMediaplayer.start(urls[2], playerIndex: 2,isRepeat: false,isAsset: false),
        ZegoMediaplayer.setPlayerType(
            ZegoMediaPlayerType.ZEGO_MEDIA_PLAYER_TYPE_AUX_WITH_STREAM,playerIndex: 1),
        ZegoMediaplayer.setProcessInterval(1000, playerIndex: 2),
      });

      ZegoMediaplayer.createMediaPlayerRenderer(400, 250,playerIndex: 3).then((value) => {
        mediaViews2.add(value),
        mediaPlayerInfos.add(MediaPlayerInfo(3, PlayerState.play,
          preView: Container(
              width: 400,
              height: 250,
              child: mediaViews2[3]!=null ? Texture(textureId: mediaViews2[3]) : Container(child: Text("视图未创建"))
          ),
        )),
        ZegoMediaplayer.start(urls[3], playerIndex: 3,isRepeat: false,isAsset: false),
        ZegoMediaplayer.setPlayerType(
            ZegoMediaPlayerType.ZEGO_MEDIA_PLAYER_TYPE_AUX_WITH_STREAM, playerIndex: 3),
        ZegoMediaplayer.setProcessInterval(1000, playerIndex: 3),
      });
    }

      ZegoLiveRoomPublisherPlugin.startPublishing("1", "", ZegoPublishFlag.ZEGO_JOIN_PUBLISH);

  }


  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();
    if(ZegoConfig.getInstance().enablePlatformView){
      ZegoMediaplayer.destroyMediaplayerPlatformView();
      ZegoMediaplayer.destroyMediaplayerPlatformView(playerIndex: 1);
      ZegoMediaplayer.destroyMediaplayerPlatformView(playerIndex: 2);
      ZegoMediaplayer.destroyMediaplayerPlatformView(playerIndex: 3);
    }else{
      ZegoMediaplayer.destroyMediaPlayerRenderer();
      ZegoMediaplayer.destroyMediaPlayerRenderer(playerIndex: 1);
      ZegoMediaplayer.destroyMediaPlayerRenderer(playerIndex: 2);
      ZegoMediaplayer.destroyMediaPlayerRenderer(playerIndex: 3);
    }
    ZegoMediaplayer.unregisterMediaPlayerCallback();
    ZegoLiveRoomPlugin.logoutRoom();
  }

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    // _initMediaPLayerInfo(context);

    return Scaffold(
        resizeToAvoidBottomInset: false,
        appBar: AppBar(
          title: Text("mediaplayer"),
        ),
        body: mediaPlayerInfos.length>0 ? Container(
          child: Stack(
            children :[
              SingleChildScrollView(
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: <Widget>[

                    ///第一个播放器
                    Text("MediaPlayer1"),
                    Stack(
                      children: [
                        Container(
                            child: mediaPlayerInfos.length>0 ? mediaPlayerInfos[0].preView : Container()
                        ),
                        Container(
                          color: Colors.black26,
                          margin: EdgeInsets.only(left: 0,top: MediaQuery.of(context).size.width * 0.6),
                          padding: EdgeInsets.all(13),
                          child: mediaPlayerInfos.length>0 ?SeekBar(
                            progresseight: 5,
                            value: mediaPlayerInfos[0].getCurrentProgress,
                            max: mediaPlayerInfos[0].getDuration,
                            onValueChanged: (value){
                              mediaPlayerInfos[0].isSeek = true;
                              ZegoMediaplayer.seekTo((value.progress*mediaPlayerInfos[0].getDuration).toInt()).then((value) =>
                              {
                                mediaPlayerInfos[0].isSeek = false
                              });
                            },
                          ) : Container(),
                        )
                      ],
                    ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: [
                        ElevatedButton(
                            onPressed: () {
                              playMediaPlayer(0);
                            },
                            child: mediaPlayerInfos[0].playerState == PlayerState.play
                                ? Text("暂停")
                                : Text("播放")),
                        ElevatedButton(
                            onPressed: () {
                              stopMediaPlayer(0);
                            },
                            child: mediaPlayerInfos[0].playerState == PlayerState.stop
                                ? Text("开始")
                                : Text("停止")
                        ),
                      ],
                    ),
                    Container(
                      width: MediaQuery.of(context).size.width,
                      height: 2,
                      color: Colors.black26,
                    ),

                    ///第二个播放器
                    Text("MediaPlayer2"),
                    Stack(
                      children: [
                        Container(
                            child: mediaPlayerInfos.length>1 ? mediaPlayerInfos[1].preView : Container()
                        ),
                        Container(
                          color: Colors.black26,
                          margin: EdgeInsets.only(left: 0,top: MediaQuery.of(context).size.width * 0.6),
                          padding: EdgeInsets.all(13),
                          child: mediaPlayerInfos.length>1 ?SeekBar(
                            progresseight: 5,
                            value: mediaPlayerInfos[1].getCurrentProgress,
                            max: mediaPlayerInfos[1].getDuration,
                            onValueChanged: (value){
                              mediaPlayerInfos[3].isSeek = true;
                              ZegoMediaplayer.seekTo((value.progress*mediaPlayerInfos[1].getDuration).toInt(), playerIndex: 1).then((value) => {
                                mediaPlayerInfos[3].isSeek = false
                              });

                            },
                          ) : Container(),
                        )
                      ],
                    ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: [
                        ElevatedButton(
                            onPressed: () {
                              playMediaPlayer(1);
                            },
                            child: mediaPlayerInfos[1].playerState == PlayerState.play
                                ? Text("暂停")
                                : Text("播放")),
                        ElevatedButton(
                            onPressed: () {
                              stopMediaPlayer(1);
                            },
                            child: mediaPlayerInfos[1].playerState == PlayerState.stop
                                ? Text("开始")
                                : Text("停止")
                        ),
                      ],
                    ),
                    Container(
                      width: MediaQuery.of(context).size.width,
                      height: 2,
                      color: Colors.black26,
                    ),
                    ///第三个播放器
                    Text("MediaPlayer3"),
                    Stack(
                      children: [
                        Container(
                            child: mediaPlayerInfos.length>2? mediaPlayerInfos[2].preView : Container()
                        ),
                        Container(
                          color: Colors.black26,
                          margin: EdgeInsets.only(left: 0,top: MediaQuery.of(context).size.width * 0.6),
                          padding: EdgeInsets.all(13),
                          child: mediaPlayerInfos.length>2 ?SeekBar(
                            progresseight: 5,
                            value: mediaPlayerInfos[2].getCurrentProgress,
                            max: mediaPlayerInfos[2].getDuration,
                            onValueChanged: (value){
                              mediaPlayerInfos[2].isSeek = true;
                              ZegoMediaplayer.seekTo((value.progress*mediaPlayerInfos[2].getDuration).toInt(), playerIndex: 2).then((value) =>
                              {
                                mediaPlayerInfos[2].isSeek = false
                              });
                            },
                          ) : Container(),
                        )
                      ],
                    ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: [
                        ElevatedButton(
                            onPressed: () {
                              playMediaPlayer(2);
                            },
                            child: mediaPlayerInfos[2].playerState == PlayerState.play
                                ? Text("暂停")
                                : Text("播放")),
                        ElevatedButton(
                            onPressed: () {
                              stopMediaPlayer(2);
                            },
                            child: mediaPlayerInfos[2].playerState == PlayerState.stop
                                ? Text("开始")
                                : Text("停止")
                        ),
                      ],
                    ),
                    Container(
                      width: MediaQuery.of(context).size.width,
                      height: 2,
                      color: Colors.black26,
                    ),

                    ///第四个播放器
                    Text("MediaPlayer4"),
                    Stack(
                      children: [
                        Container(
                            child: mediaPlayerInfos.length>3? mediaPlayerInfos[3].preView : Container()
                        ),
                        Container(
                          color: Colors.black26,
                          margin: EdgeInsets.only(left: 0,top: MediaQuery.of(context).size.width * 0.6),
                          padding: EdgeInsets.all(13),
                          child: mediaPlayerInfos.length>3 ?SeekBar(
                            progresseight: 5,
                            value: mediaPlayerInfos[3].getCurrentProgress,
                            max: mediaPlayerInfos[3].getDuration,
                            onValueChanged: (value){

                              mediaPlayerInfos[3].isSeek = true;
                              print("onValueChanged${value}  mediaPlayerInfos[3].getDuration:${mediaPlayerInfos[3].getDuration}");
                              ZegoMediaplayer.seekTo((value.progress * mediaPlayerInfos[3].getDuration).toInt(), playerIndex: 3).then((value) =>
                              {
                                mediaPlayerInfos[3].isSeek = false
                              });

                            },
                          ) : Container(),
                        )
                      ],
                    ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: [
                        ElevatedButton(
                            onPressed: () {
                              playMediaPlayer(3);
                            },
                            child: mediaPlayerInfos[3].playerState == PlayerState.play
                                ? Text("暂停")
                                : Text("播放")),
                        ElevatedButton(
                            onPressed: () {
                              stopMediaPlayer(3);
                            },
                            child: mediaPlayerInfos[3].playerState == PlayerState.stop
                                ? Text("开始")
                                : Text("停止")
                        ),
                      ],
                    ),
                    Container(
                      width: MediaQuery.of(context).size.width,
                      height: 2,
                      color: Colors.black26,
                    )

                  ],
                ),
              ),
            ]
          ),
        ):Container(child: Center(child: Text("视图加载中")),));
  }

  void onPlayBegin(int playerIndex) {
    print("onPlayBegin $playerIndex");
    ZegoMediaplayer.seekTo(5000,playerIndex: playerIndex);
  }

  void onSeekComplete(int errorCode, int millisecond,int playerIndex) {
    print("onSeekComplete $playerIndex");
    print(millisecond);
  }

  void onPlayPause(int playerIndex) {
    print("onPlayPause $playerIndex");
    print("播放暂停$playerIndex");
  }

  void onPlayResume(int playerIndex) {
    print("onPlayResume $playerIndex");
    print("播放恢复$playerIndex");
  }

  void onPlayStop(int playerIndex) {
    print("onPlayStop $playerIndex");
    print("播放停止");
    setState(() {
      mediaPlayerInfos[playerIndex].playerState = PlayerState.stop;
    });

  }

  void onBufferBegin(int playerIndex){
    print("onBufferBegin $playerIndex");

  }

  void onPlayEnd(int playerIndex){
    print("onPlayEnd $playerIndex");

  }

  void onPlayError(int errerCode,int playerIndex){
    print("onPlayError $playerIndex");

  }

  void onAudioBegin(int playerIndex){
    print("onAudioBegin $playerIndex");

  }


  void onProcessInterval(int timestamp,int playerIndex){
    // print("第$playerIndex个播放器：时间间隔：$timestamp");
    if(!mediaPlayerInfos[playerIndex].isSeek){
      setState(() {
        mediaPlayerInfos[playerIndex].currentProgress = timestamp.toDouble();
      });
   }
  }

  void onBufferEnd(int playerIndex){
    print("onBufferEnd $playerIndex");
  }
  void onVideoBegin(int playerIndex){
    print("onVideoBegin $playerIndex");

    ZegoMediaplayer.getTotalDuration(playerIndex: playerIndex).then((value) => {
      print("时长$value"),
      setState(() {
        mediaPlayerInfos[playerIndex].setToatalDuration(value.toDouble());
      })

    });
  }

  void stopMediaPlayer(int playerIndex){
    setState(() {
      if(mediaPlayerInfos[playerIndex].playerState == PlayerState.stop){
        ZegoMediaplayer.start(urls[playerIndex],playerIndex: playerIndex,isRepeat: false,isAsset: false);
        mediaPlayerInfos[playerIndex].playerState = PlayerState.play;
      }else{
        ZegoMediaplayer.stop(playerIndex: playerIndex);
        mediaPlayerInfos[playerIndex].playerState = PlayerState.stop;
      }
    });
  }
  void playMediaPlayer(int playerIndex){
    setState(() {
      if(mediaPlayerInfos[playerIndex].playerState == PlayerState.play){
        ZegoMediaplayer.pause(playerIndex: playerIndex);
        mediaPlayerInfos[playerIndex].playerState = PlayerState.puase;
      }else{
        ZegoMediaplayer.resume(playerIndex: playerIndex);
        mediaPlayerInfos[playerIndex].playerState = PlayerState.play;
      }
    });
  }

}

class MediaPlayerInfo{
  int playerIndex;
  PlayerState playerState;
  Widget preView;
  double TotalDuration = 100;
  double currentProgress = 0;
  Timer timer;
  double get getDuration => TotalDuration;
  double get getCurrentProgress => currentProgress;
  bool isSeek = false;
  MediaPlayerInfo(this.playerIndex,this.playerState,{@required this.preView});

  setToatalDuration(double durstiom){
    TotalDuration = durstiom;
  }

}

enum PlayerState{
  play,
  puase,
  stop
}

typedef ProgressUpdate = void Function(double duration);
class OnProgressUpdateCallback{
  ProgressUpdate onProgressUbdate;
  OnProgressUpdateCallback({this.onProgressUbdate});
}
