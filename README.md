# zegoliveroom_plugin **(Legacy)**

[![pub package](https://img.shields.io/pub/v/zegoliveroom_plugin.svg)](https://pub.dartlang.org/packages/zegoliveroom_plugin)
![legacy](https://img.shields.io/badge/Legacy%20Project-Will%20be%20deprecated%20soon-yellow)

**[English](https://github.com/zegoim/zego-flutter-sdk/blob/master/README.md) | [中文](https://github.com/zegoim/zego-flutter-sdk/blob/master/README_zh.md)**

Zego's audio and video LiveRoom Flutter SDK **(Legacy)**, provides live video and real-time audio and video services.

Learn more about the solution: [https://www.zego.im](https://www.zego.im)

## ⚠️❗️ This project is a **legacy** SDK and and will be deprecated in the future, only used to support the maintenance for existing users, it is recommended to migrate and upgrade to the new SDK as soon as possible

## **🚀 Please use the brand new [`ZegoExpressEngine Flutter SDK`](https://pub.dev/packages/zego_express_engine)**

- pub: **[https://pub.dev/packages/zego_express_engine](https://pub.dev/packages/zego_express_engine)**

- Github: **[https://github.com/zegoim/zego-express-flutter-sdk](https://github.com/zegoim/zego-express-flutter-sdk)**

- Documents: **[https://doc-en.zego.im/en/5560.html](https://doc-en.zego.im/en/5560.html)**

> 🌞 The new redesigned API is easier to use, updated in sync with the ZEGO native SDK, and has more complete functions!

## 1️⃣ Download and Install Flutter

### **[Flutter Get Started](https://flutter.dev/docs/get-started/install)**

## 2️⃣ Configuration development environment

- Android Studio: `Preferences` -> `Plugins`, search for the `Flutter` plugin to download, configure the first step to download the Flutter SDK path in the plugin.

- VS Code: Search for the `Flutter` extension in the app store and download

After configuring the flutter environment in any of the above development environments, execute the `flutter doctor` on the command line and complete the related undownloaded dependencies according to the prompts.

## 3️⃣ Apply for ZEGO AppID

Log in to [ZEGO Official Website](https://www.zego.im) to register an account, select a scenario according to your actual business needs, and obtain AppID and App Sign for initializing the SDK.

## 4️⃣ Import `zegoliveroom_plugin`

Open the `pubspec.yaml` in the project and add the `zegoliveroom_plugin` dependency:

- ### Depends on `pub` (recommended)

```yaml
dependencies:
  flutter:
    sdk: flutter

  zegoliveroom_plugin: ^1.5.4
```

- ### Depends on git

```yaml
dependencies:
  flutter:
    sdk: flutter

  zegoliveroom_plugin:
    git:
      url: git://github.com/zegoim/zego-flutter-sdk.git
      ref: master
```

After saving the file, execute `flutter pub get`

## 5️⃣ Add device permissions

### Android

Open the file `app/src/main/AndroidManifest.xml`, and add the following contents:

![Add Android Privacy](https://storage.zego.im/sdk-doc/Pics/Android/ZegoLiveRoom/ZegoLiveRoom-IntegrationGuide/3.2-insert_sourceSets_node-4.png)

```xml
    <!-- Permissions required by the SDK -->
    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
    <uses-permission android:name="android.permission.RECORD_AUDIO" />
    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
    <uses-permission android:name="android.permission.CAMERA" />
    <uses-permission android:name="android.permission.BLUETOOTH" />
    <uses-permission android:name="android.permission.MODIFY_AUDIO_SETTINGS" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
    <!-- Permissions required by the App -->
    <uses-permission android:name="android.permission.READ_PHONE_STATE" />
    <uses-permission android:name="android.permission.WAKE_LOCK" />

    <uses-feature
        android:glEsVersion="0x00020000"
        android:required="true" />

    <uses-feature android:name="android.hardware.camera" />
    <uses-feature android:name="android.hardware.camera.autofocus" />
```

> Note: Because Android 6.0 requires dynamic permissions for some of the more important permissions, you cannot apply for static permissions only through the `AndroidMainfest.xml` file. Therefore, you need to refer to the following code (requestPermissions is the Activity method)

```java
String[] permissionNeeded = {
        "android.permission.CAMERA",
        "android.permission.RECORD_AUDIO"};

if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
    if (ContextCompat.checkSelfPermission(this, "android.permission.CAMERA") != PackageManager.PERMISSION_GRANTED ||
        ContextCompat.checkSelfPermission(this, "android.permission.RECORD_AUDIO") != PackageManager.PERMISSION_GRANTED) {
        requestPermissions(permissionNeeded, 101);
    }
}
```

### iOS

Choose the option TARGETS -> Info -> Custom iOS Target Properties

![Add iOS Privacy](https://storage.zego.im/sdk-doc/Pics/iOS/ZegoExpressEngine/Common/privacy-description.png)

Click the + Add button to add camera and microphone permissions.

1. `Privacy - Camera Usage Description`

2. `Privacy - Microphone Usage Description`

After adding permissions, it will be as shown:

![Add iOS Privacy Done](https://storage.zego.im/sdk-doc/Pics/iOS/ZegoExpressEngine/Common/privacy-description-done.png)

> If you use Platform View, you need to add an additional description for iOS, refer to [FAQ-3](#3-ios-uses-platform-view-error-verbose-2platform_view_layercc28-trying-to-embed-a-platform-view-but-the-paintcontext-does-not-support-embedding)

## 6️⃣ Init SDK

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

## 7️⃣ API documentation

[ZegoLiveRoomPlugin API Reference](https://pub.dev/documentation/zegoliveroom_plugin/latest/)

## 8️⃣ FAQ

### 1. Solve the problem of compiling errors of `ZegoLiveRoomApi-AudioIO` and `audio_in_output` modules on iOS platform

Open the iOS project file `Runner. xcworkspace` -> `Build Settings`, change `Compile Sources As` to `Objective-C++`, recompile.

### 2. iOS compiler error: `ld: symbol(s) not found for architecture x86_64`

Compilation with the simulator is not supported, please use real device debugging for best results.

### 3. iOS uses Platform View error: `[VERBOSE-2:platform_view_layer.cc(28)] Trying to embed a platform view but the PaintContext does not support embedding`

Open the iOS project that requires Platform View and add the field `io.flutter.embedded_views_preview` to `Info.plist` with a value of `YES`.

### 4. fatal error: lipo: -extract armv7 specified but fat file: [...] does not contain that architecture

Usually occurs when switching iOS devices, which can be resolved by deleting the "flutter-project-path/build/" and "flutter-project-path/ios/DerivedData/" directories.

### 5. Android building release crashes with `NoClassDefFoundError` when flutter is upgraded to 1.10 or above

Flutter is enabled obfuscation by default in version 1.10 or above. Please add the following line in `app/proguard-rules.pro` to prevent the Zego SDK obfuscation.

```java
-keep class com.zego.**{*;}
```

## 9️⃣ Contributing

We welcome contributions, everyone is welcome to contribute code via pull requests, to help people asking for help, to add to our documentation, or to help out in any other way.
