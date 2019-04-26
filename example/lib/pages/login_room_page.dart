import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';
import 'package:example/config/zego_config.dart';
import 'package:zegoliveroom_plugin/zego_api_defines.dart';
import 'package:zegoliveroom_plugin/zego_liveroom.dart';
import 'package:zego_permission/zego_permission.dart';
import 'package:example/pages/publish_stream_page.dart';
import 'package:example/pages/play_stream_page.dart';
import 'package:example/ui/zego_ui_tool.dart';

class Authorization {
  final bool camera;
  final bool microphone;

  Authorization(this.camera, this.microphone);
}

class LoginRoomPage extends StatefulWidget {

  final bool isPublish;

  LoginRoomPage(this.isPublish);

  @override
  _LoginRoomPageState createState() => new _LoginRoomPageState();
}

class _LoginRoomPageState extends State<LoginRoomPage> {

  final TextEditingController _controller = new TextEditingController();

  @override
  void initState() {
    // TODO: implement initState
    super.initState();

    if(ZegoConfig.getInstance().roomID.isNotEmpty) {
      _controller.text = ZegoConfig.getInstance().roomID;
    }
  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();
    ZegoLiveRoomPlugin.uninitSDK();
  }

  void onButtonPressed() async {

    if(widget.isPublish) {
      // 登录房间前，先检查权限
      Authorization authorization = await checkAuthorization();
      //权限对象为null，表明当前运行系统下无需进行动态检查权限（如Android 6.0以下系统）
      if (authorization == null) {
        print('loginroom without authorization');
        _loginRoom();
        return;
      }

      //未允许授权，弹窗提示并引导用户开启
      if (!authorization.camera || !authorization.microphone) {

        showSettingsLink();
      }
      //授权完成，允许登录房间
      else {
        _loginRoom();
      }
    } else {

      _loginRoom();
    }


  }

  void _loginRoom() {
    String roomID = _controller.text.trim();

    // 调用登录房间之前，必须先调用setUser
    ZegoLiveRoomPlugin.setUser(ZegoConfig.getInstance().userID, ZegoConfig.getInstance().userName);
    ZegoLiveRoomPlugin.loginRoom(roomID, 'test-room-$roomID', widget.isPublish ? ZegoRoomRole.ROOM_ROLE_ANCHOR : ZegoRoomRole.ROOM_ROLE_AUDIENCE).then((result) {

      if(result.errorCode == 0) {

        print('登录房间成功');
        ZegoConfig.getInstance().roomID = roomID;
        ZegoConfig.getInstance().saveConfig();

        Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) {

          int screenWidthPx = MediaQuery.of(context).size.width.toInt() * MediaQuery.of(context).devicePixelRatio.toInt();
          int screenHeightPx = (MediaQuery.of(context).size.height - MediaQuery.of(context).padding.top - 56.0).toInt() * MediaQuery.of(context).devicePixelRatio.toInt();

          if(widget.isPublish) {

            return PublishStreamPage(screenWidthPx, screenHeightPx);

          } else {

            return PlayStreamPage(screenWidthPx, screenHeightPx);
          }
        }));
      } else {

        ZegoUITool.showAlert(context, '登录房间失败，错误码: ${result.errorCode}');
      }
    });
  }

  // 请求相机、麦克风权限
  Future<Authorization> checkAuthorization() async {
    List<Permission> statusList = await ZegoPermission.getPermissions(
        <PermissionType>[PermissionType.Camera, PermissionType.MicroPhone]);

    if(statusList == null)
      return null;

    PermissionStatus cameraStatus, micStatus;
    for (var permission in statusList) {
      if (permission.permissionType == PermissionType.Camera)
        cameraStatus = permission.permissionStatus;
      if (permission.permissionType == PermissionType.MicroPhone)
        micStatus = permission.permissionStatus;
    }

    bool camReqResult = true, micReqResult = true;
    if (cameraStatus != PermissionStatus.granted ||
        micStatus != PermissionStatus.granted) {

      //不管是第一次询问还是之前已拒绝，都直接请求权限
      if (cameraStatus != PermissionStatus.granted) {
        camReqResult = await ZegoPermission.requestPermission(
            PermissionType.Camera);
      }

      if (micStatus != PermissionStatus.granted) {
        micReqResult = await ZegoPermission.requestPermission(
            PermissionType.MicroPhone);
      }
    }

    return Authorization(camReqResult, micReqResult);
  }

  void showSettingsLink() {
    showDialog(context: context, builder: (BuildContext context) {
      return AlertDialog(
        title: Text('提示'),
        content: Text('请到设置页面开启相机/麦克风权限，否则您将无法体验音视频功能'),
        actions: <Widget>[
          FlatButton(
            child: Text('去设置'),
            onPressed: () {
              Navigator.of(context).pop();
              ZegoPermission.openAppSettings();
            },
          ),
          FlatButton(
            child: Text('取消'),
            onPressed: () {
              Navigator.of(context).pop();
            },
          ),
        ],
      );
    });
  }

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return Scaffold(
      appBar: AppBar(
        title: Text('第二步 登录房间'),
      ),
      body: GestureDetector(

        behavior: HitTestBehavior.translucent,
        onTap: () => FocusScope.of(context).requestFocus(new FocusNode()),

        child: Container(
          padding: const EdgeInsets.symmetric(horizontal: 40.0),
          child: Column(
            children: <Widget>[
              Padding(
                padding: const EdgeInsets.only(bottom: 20.0),
              ),
              Row(
                children: <Widget>[
                  Text('roomID: '),
                ],
              ),
              Padding(
                padding: const EdgeInsets.only(bottom: 10.0),
              ),
              TextField(
                controller: _controller,
                decoration: InputDecoration(
                  contentPadding: const EdgeInsets.only(left: 10.0, top: 12.0, bottom: 12.0),
                  hintText: '请输入房间号',
                  enabledBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                        color: Colors.grey,
                      )
                  ),
                  focusedBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                        color: Color(0xff0e88eb),
                      )
                  ),
                ),
              ),
              Padding(
                padding: const EdgeInsets.only(bottom: 10.0),
              ),
              Text('roomID代表一个房间的标识，需保证房间ID信息的全局唯一，长度不超过 255 bytes 的可打印字符串',
                style: TextStyle(
                    fontSize: 12.0,
                    color: Colors.black45
                ),
                maxLines: 2,
                softWrap: true,
              ),
              Padding(
                padding: const EdgeInsets.only(bottom: 50.0),
              ),
              Container(
                padding: const EdgeInsets.all(0.0),
                decoration: BoxDecoration(
                  borderRadius: BorderRadius.circular(12.0),
                  color: Color(0xff0e88eb),
                ),
                width: 240.0,
                height: 60.0,
                child: CupertinoButton(
                  child: Text('登录房间',
                    style: TextStyle(
                        color: Colors.white
                    ),
                  ),
                  onPressed: onButtonPressed,
                ),
              )
            ],
          ),
        )
      ),
    );
  }

}

