import 'dart:async';
import 'package:shared_preferences/shared_preferences.dart';
import 'dart:math' show Random;

class ZegoConfig {
  static ZegoConfig _instance = new ZegoConfig._internal();

  static ZegoConfig getInstance() {
    return _instance;
  }

  factory ZegoConfig() {
    return _instance;
  }

  String userID;
  String userName;

  String appID;
  String appSign;

  // 是否使用测试环境
  bool isUseTestEnv;

  String roomID;
  String streamID;

  ZegoConfig._internal() {

    SharedPreferences.getInstance().then((config) {

      this.userID = config.getString('userID') ?? new Random().nextInt(9999999).toString();
      this.userName = config.getString('userName') ?? 'user-$userID';
      this.appID = config.getString('appID') ?? '';
      this.appSign = config.getString('appSign') ?? '';
      this.isUseTestEnv = config.getBool('isUseTestEnv') ?? true;
      this.roomID = config.getString('roomID') ?? '';
      this.streamID = config.getString('streamID') ?? '';
    });
  }

  Future<void> saveConfig() async {

    SharedPreferences config = await SharedPreferences.getInstance();

    config.setString('userID', this.userID);
    config.setString('userName', this.userName);
    config.setString('appID', this.appID);
    config.setString('appSign', this.appSign);
    config.setBool('isUseTestEnv', this.isUseTestEnv);
    config.setString('roomID', this.roomID);
    config.setString('streamID', this.streamID);
  }

}