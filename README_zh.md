# zegoliveroom_plugin [![pub package](https://img.shields.io/pub/v/zegoliveroom_plugin.svg)](https://pub.dartlang.org/packages/zegoliveroom_plugin)

[English Version](https://github.com/zegoim/zego-flutter-sdk/blob/master/README.md)

即构科技（Zego）音视频Flutter SDK，提供视频直播以及实时音视频服务。

了解更多解决方案：[https://www.zego.im](https://www.zego.im)

## 一、下载 Flutter SDK

参考：[Flutter SDK 下载](https://flutter.dev/docs/get-started/install)

## 二、配置开发环境

1. Android Studio：`Preferences`->`Plugins`，搜索 `Flutter` 插件进行下载，在插件中配置第一步下载好的Flutter SDK路径

2. Visual Code: 在应用商店中搜索 `Flutter` 扩展并下载

以上任一开发环境配置好flutter环境之后，命令行执行 `flutter doctor`，根据提示内容补全相关未下载的依赖项。

## 三、申请Zego App ID

登录[ZEGO 官网](https://www.zego.im)注册账号，根据自身实际业务需求选择场景，获取AppID与App Sign，用于初始化 SDK。

## 四、导入 zegoliveroom_plugin

打开待导入工程中的 `pubspec.yaml`，添加 `zegoliveroom_plugin` 依赖：

* 以pub形式依赖：

```yaml
dependencies:
  flutter:
  sdk: flutter

  zegoliveroom_plugin: ^1.2.6
```

* 以git形式依赖：

```yaml
dependencies:
  flutter:
  sdk: flutter

  zegoliveroom_plugin:
    git:
      url: git://github.com/zegoim/zego-flutter-sdk.git
```

* 以git形式依赖并指定分支：

```yaml
dependencies:
  flutter:
  sdk: flutter

  zegoliveroom_plugin:
    url: git://github.com/zegoim/zego-flutter-sdk.git
    ref: branch_name
```

## 五、初始化 SDK

```Dart
import 'package:flutter/material.dart';
import 'package:zegoliveroom_plugin/zegoliveroom_plugin.dart';

void main() => runApp(new MyApp());

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => new _MyAppState();
}

class _MyAppState extends State<MyApp> {

  // 填入实际从即构官网获取到的AppID
  final int appID = 00000000;
  // 填入实际从即构官网获取到的AppSign
  final String appSign = '0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'
                         '0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'
                         '0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'
                         '0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00';

  String version = '';

  @override
  void initState() {
    super.initState();

    // 获取SDK版本号
    ZegoLiveRoomPlugin.getSdkVersion().then((ver) {

      setState({
        version = ver;
      });

    });

    // 初始化SDK
    ZegoLiveRoomPlugin.initSDK(appID, appSign);
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Zego Plugin example app'),
        ),
        body: Center(
          child: Text('Version: $version'),
        ),
      ),
    );
  }
}
```

更多功能请参考API用法。

## 六、API文档

参考 [ZegoLiveRoomPlugin API Reference](https://pub.dev/documentation/zegoliveroom_plugin/latest/)

## 七、常见问题

#### 1. 解决 iOS 平台下 `ZegoLiveRoomApi-AudioIO`、`audio_in_output` 模块编译错误问题

打开 iOS 工程文件 `Runner. xcworkspace` -> `Build Settings`，将 `Compile Sources As` 改为 `Objective-C++`，重新编译

#### 2. iOS 编译报错：`ld: symbol(s) not found for architecture x86_64`

不支持使用模拟器编译，请使用真机调试以获得最佳效果。

#### 3. iOS 使用 Platform View 时报错：`[VERBOSE-2:platform_view_layer.cc(28)] Trying to embed a platform view but the PaintContext does not support embedding`

打开需要使用 Platform View 的 iOS 工程，在`Info.plist`中添加字段`io.flutter.embedded_views_preview`，其值为`YES`。

#### 4. fatal error: lipo: -extract armv7 specified but fat file: [...] does not contain that architecture

通常在切换 iOS 设备时出现，可通过删除 "flutter-project-path/build/" 和 "flutter-project-path/ios/DerivedData/" 目录解决。

#### 5. flutter 升级至 1.10 或以上时，Android release 下出现 `NoClassDefFoundError` 导致 Crash

flutter 在 1.10 或以上版本默认开启了混淆，请在项目中 `app/proguard-rules.pro` 为 Zego SDK 添加 -keep类的配置防止混淆

```java
-keep class com.zego.**{*;}
```
