import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:zegoliveroom_plugin/zegoliveroom_plugin.dart';
import 'package:zegoliveroom_plugin_example/config/zego_config.dart';
import 'package:zegoliveroom_plugin_example/pages/login_room_page.dart';
import 'package:zegoliveroom_plugin_example/ui/zego_ui_tool.dart';

class InitPage extends StatefulWidget {
  final bool isPublish;
  final bool isMediaplayer;

  InitPage(this.isPublish, {this.isMediaplayer = false});

  @override
  _InitPageState createState() => new _InitPageState();
}

class _InitPageState extends State<InitPage> {
  final TextEditingController _appIDEdController = new TextEditingController();
  final TextEditingController _appSignEdController =
      new TextEditingController();

  String _version;

  @override
  void initState() {
    super.initState();

    if (ZegoConfig.getInstance().appID.isNotEmpty) {
      _appIDEdController.text = ZegoConfig.getInstance().appID;
    }

    if (ZegoConfig.getInstance().appSign.isNotEmpty) {
      _appSignEdController.text = ZegoConfig.getInstance().appSign;
    }

    ZegoLiveRoomPlugin.getSdkVersion().then((version) {
      print('[SDK Version] $version');
      setState(() {
        _version = version;
      });
    });
  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();

    if (ZegoConfig.getInstance().enableExternalVideoFilter) {
      // 释放外部滤镜工厂
      // 需要在 initSDK 前调用（所以释放外部滤镜工厂也是在 unInitSDK 后调用）
      ZegoLiveRoomPlugin.enableExternalVideoFilterFactory(false);
    }
    ZegoLiveRoomPlugin.enableExternalVideoCaptureFactory(false);
  }

  // 初始化SDK
  void onButtonPressed() {
    String strAppID = _appIDEdController.text.trim();
    String strAppSign = _appSignEdController.text.trim();

    if (strAppID.isEmpty || strAppSign.isEmpty) {
      ZegoUITool.showAlert(context, 'AppID 或 AppSign 不能为空');
      return;
    }

    int appID = int.tryParse(strAppID);
    if (appID == null) {
      ZegoUITool.showAlert(context, 'AppID 不合法');
      return;
    }

    // 测试环境 or 正式环境
    ZegoLiveRoomPlugin.setUseTestEnv(ZegoConfig.getInstance().isUseTestEnv);

    // 设置是否开启外部视频滤镜（必须在 initSDK 前调用，并且需要提前在 Native 层调用 `setExternalVideoFilter` 预设保存外部滤镜工厂)
    ZegoLiveRoomPlugin.enableExternalVideoFilterFactory(
        ZegoConfig.getInstance().enableExternalVideoFilter);
    // 设置是否开启外部视频采集 （这里暂时只在 MediaPlayer 部分实现，所以先这么判断）
    ZegoLiveRoomPlugin.enableExternalVideoCaptureFactory(widget.isMediaplayer);

    // 设置是否使用 PlatformView
    ZegoLiveRoomPlugin.enablePlatformView(
        ZegoConfig.getInstance().enablePlatformView);

    ZegoLiveRoomPlugin.setLogConfig(5 * 1024 * 1024);

    // 初始化 SDK
    ZegoLiveRoomPlugin.initSDK(appID, strAppSign).then((errorCode) {
      if (errorCode == 0) {
        ZegoConfig.getInstance().appID = strAppID;
        ZegoConfig.getInstance().appSign = strAppSign;
        ZegoConfig.getInstance().saveConfig();

        Navigator.of(context)
            .push(MaterialPageRoute(builder: (BuildContext context) {
          return LoginRoomPage(widget.isPublish,
              isMediaplayer: widget.isMediaplayer);
        }));
      } else {
        ZegoUITool.showAlert(context, 'sdk 初始化失败');
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return Scaffold(
      resizeToAvoidBottomPadding: false,
      appBar: AppBar(
        title: Text('第一步 初始化SDK'),
      ),
      body: SafeArea(
          child: GestureDetector(
        behavior: HitTestBehavior.translucent,
        onTap: () => FocusScope.of(context).requestFocus(new FocusNode()),
        child: Container(
          padding: const EdgeInsets.symmetric(horizontal: 40.0),
          child: Column(
            children: <Widget>[
              Padding(
                padding: const EdgeInsets.only(top: 20.0),
              ),
              Row(
                children: <Widget>[
                  Text('Version: '),
                  Padding(
                    padding: const EdgeInsets.only(left: 10.0),
                  ),
                  Expanded(
                    child: Text('$_version'),
                  ),
                ],
              ),
              Padding(
                padding: const EdgeInsets.only(top: 20.0),
              ),
              Row(
                children: <Widget>[
                  Text('userID: '),
                  Padding(
                    padding: const EdgeInsets.only(left: 10.0),
                  ),
                  Text(ZegoConfig.getInstance().userID ?? 'unknown'),
                ],
              ),
              Padding(
                padding: const EdgeInsets.only(top: 10.0),
              ),
              Row(
                children: <Widget>[
                  Text('userName: '),
                  Padding(
                    padding: const EdgeInsets.only(left: 10.0),
                  ),
                  Text(ZegoConfig.getInstance().userName ?? 'unknown'),
                ],
              ),
              Padding(
                padding: const EdgeInsets.only(top: 10.0),
              ),
              Row(
                children: <Widget>[
                  Text('AppID: '),
                  GestureDetector(
                    child: Container(
                      width: 40.0,
                      height: 40.0,
                      child: Image(
                          image:
                              AssetImage('resources/images/settings_tips.png')),
                    ),
                    onTap: () {
                      ZegoUITool.showAlert(context,
                          'AppID和AppSign是每家客户的唯一标识，可直接在官网申请，这两个字段建议实现为业务后台下发，方便控制');
                    },
                  ),
                ],
              ),
              TextField(
                controller: _appIDEdController,
                decoration: InputDecoration(
                  contentPadding: const EdgeInsets.only(
                      left: 10.0, top: 12.0, bottom: 12.0),
                  hintText: '请输入AppID',
                  enabledBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                    color: Colors.grey,
                  )),
                  focusedBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                    color: Color(0xff0e88eb),
                  )),
                ),
              ),
              Padding(
                padding: const EdgeInsets.only(top: 10.0),
              ),
              Row(
                children: <Widget>[
                  Text('AppSign: '),
                ],
              ),
              Padding(
                padding: const EdgeInsets.only(top: 10.0),
              ),
              TextField(
                controller: _appSignEdController,
                decoration: InputDecoration(
                  contentPadding: const EdgeInsets.only(
                      left: 10.0, top: 12.0, bottom: 12.0),
                  hintText: '请输入AppSign',
                  enabledBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                    color: Colors.grey,
                  )),
                  focusedBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                    color: Color(0xff0e88eb),
                  )),
                ),
              ),
              Padding(
                padding: const EdgeInsets.only(top: 10.0),
              ),
              Row(
                children: <Widget>[
                  Text('环境 '),
                  Text(
                    '(请选择与AppID对应的环境)',
                    style: TextStyle(fontSize: 12.0),
                  ),
                ],
              ),
              Row(
                children: <Widget>[
                  Checkbox(
                    value: ZegoConfig.getInstance().isUseTestEnv,
                    onChanged: (value) {
                      setState(() {
                        ZegoConfig.getInstance().isUseTestEnv = value;
                        ZegoConfig.getInstance().saveConfig();
                      });
                    },
                  ),
                  Text('测试环境'),
                  Checkbox(
                    value: !ZegoConfig.getInstance().isUseTestEnv,
                    onChanged: (value) {
                      setState(() {
                        ZegoConfig.getInstance().isUseTestEnv = !value;
                        ZegoConfig.getInstance().saveConfig();
                      });
                    },
                  ),
                  Text('正式环境'),
                ],
              ),
              Row(
                children: <Widget>[
                  Checkbox(
                    value: ZegoConfig.getInstance().enablePlatformView,
                    onChanged: (value) {
                      setState(() {
                        ZegoConfig.getInstance().enablePlatformView = value;
                        ZegoConfig.getInstance().saveConfig();
                      });
                    },
                  ),
                  Text('是否使用 Platform View 渲染视图'),
                ],
              ),
// External Video Filter Checkbox 外部滤镜开关
//                  Row(
//                    children: <Widget>[
//                      Checkbox(
//                        value: ZegoConfig.getInstance().enableExternalVideoFilter,
//                        onChanged: (value) {
//                          setState(() {
//
//                            ZegoConfig.getInstance().enableExternalVideoFilter = value;
//                            ZegoConfig.getInstance().saveConfig();
//                          });
//                        },
//                      ),
//                      Text('是否开启外部视频滤镜'),
//                    ],
//                  ),
              Container(
                padding: const EdgeInsets.all(0.0),
                decoration: BoxDecoration(
                  borderRadius: BorderRadius.circular(12.0),
                  color: Color(0xff0e88eb),
                ),
                width: 240.0,
                height: 60.0,
                child: CupertinoButton(
                  child: Text(
                    '初始化SDK',
                    style: TextStyle(color: Colors.white),
                  ),
                  onPressed: onButtonPressed,
                ),
              )
            ],
          ),
        ),
      )),
    );
  }
}
