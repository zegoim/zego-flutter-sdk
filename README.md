# zegoliveroom_plugin [![pub package](https://img.shields.io/pub/v/zegoliveroom_plugin.svg)](https://pub.dartlang.org/packages/zegoliveroom_plugin)

[Chinese Version](https://github.com/zegoim/zego-flutter-sdk/blob/master/README_CN.md)

Zego's audio and video Flutter SDK provides live video and real-time audio and video services.

Learn more about the solution: [https://www.zego.im](https://www.zego.im)

## I. Download Flutter SDK

[https://flutter.dev/docs/get-started/install](https://flutter.dev/docs/get-started/install)

## II. Configuration development environment

1. Android Studio: `Preferences`->`Plugins`, search for the `Flutter` plugin to download, configure the first step to download the Flutter SDK path in the plugin.

2. Visual Code: Search for the `Flutter` extension in the app store and download

After configuring the flutter environment in any of the above development environments, execute the `flutter doctor` on the command line and complete the related undownloaded dependencies according to the prompts.

## III. Apply for Zego App ID

Log in to [ZEGO Official Website](https://www.zego.im) to register an account, select a scenario according to your actual business needs, and obtain AppID and App Sign for initializing the SDK.

## IV. Import zegoliveroom_plugin

Open the `pubspec.yaml` in the project to be imported and add the `zegoliveroom_plugin` dependency:

* Dependent in pub form:

```yaml
dependencies:
  flutter:
    sdk: flutter

  zegoliveroom_plugin: ^1.0.10
```

* Dependent in git form:

```yaml
dependencies:
  flutter:
    sdk: flutter

  zegoliveroom_plugin:
    git:
      url: git://github.com/zegoim/zego-flutter-sdk.git
```

* Dependent in git form and specify the branch:

```yaml
dependencies:
  flutter:
    sdk: flutter

  zegoliveroom_plugin:
    url: git://github.com/zegoim/zego-flutter-sdk.git
    ref: branch_name
```

## V. Init SDK

```Dart
import 'package:flutter/material.dart';
import 'package:zegoliveroom_plugin/zegoliveroom_plugin.dart';

void main() => runApp(new MyApp());

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => new _MyAppState();
}

class _MyAppState extends State<MyApp> {

  // Fill in the AppID actually obtained from the official website
  final int appID = 00000000;
  // Fill in the AppSign actually obtained from the official website
  final String appSign = '0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'
                         '0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'
                         '0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'
                         '0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00';

  String version = '';

  @override
  void initState() {
    super.initState();

    // get SDK version
    ZegoLiveRoomPlugin.getSdkVersion().then((ver) {

      setState({
        version = ver;
      });

    });

    // init SDK
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

Please refer to the API usage for more features.

## VI. API documentation

Reference [ZegoLiveRoomPlugin API Reference](https://pub.dev/documentation/zegoliveroom_plugin/latest/)

## VII. FAQ

#### 1. Solve the problem of compiling errors of `ZegoLiveRoomApi-AudioIO` and `audio_in_output` modules on iOS platform

Open the iOS project file `Runner. xcworkspace` -> `Build Settings`, change `Compile Sources As` to `Objective-C++`, recompile.

#### 2. iOS compiler error: `ld: symbol(s) not found for architecture x86_64`

Compilation with the simulator is not supported, please use real device debugging for best results.

#### 3. iOS uses Platform View error: `[VERBOSE-2:platform_view_layer.cc(28)] Trying to embed a platform view but the PaintContext does not support embedding`

Open the iOS project that requires Platform View and add the field `io.flutter.embedded_views_preview` to `Info.plist` with a value of `YES`.

#### 4. fatal error: lipo: -extract armv7 specified but fat file: [...] does not contain that architecture

Usually occurs when switching iOS devices, which can be resolved by deleting the "flutter-project-path/build/" and "flutter-project-path/ios/DerivedData/" directories.
