# Change log

## ⚠️❗️ This project is a **legacy** SDK and and will be deprecated in the future, only used to support the maintenance for existing users, it is recommended to migrate and upgrade to the new SDK as soon as possible

## **🚀 Please use the brand new [`ZegoExpressEngine Flutter SDK`](https://pub.dev/packages/zego_express_engine)**


## 1.5.0

1. Add feature of MediaPlayer Rendering and publishing

## 1.4.0

1. Optimization: Limit iOS memory allocation to avoid the problem of untimely memory release in some versions of flutter
2. Update Native SDK to 1.21.3.18

## 1.3.6

1. Update Native SDK to 1.21.1.27

## 1.3.5

1. Update Native SDK to 1.21.1.16

## 1.3.4

1. Add [`takePublishStreamSnapshot`](https://pub.dev/documentation/zegoliveroom_plugin/latest/zegoliveroom_plugin/ZegoLiveRoomPublisherPlugin/takePublishStreamSnapshot.html) and [`takePlayStreamSnapshot`](https://pub.dev/documentation/zegoliveroom_plugin/latest/zegoliveroom_plugin/ZegoLiveRoomPlayerPlugin/takePlayStreamSnapshot.html) APIs

## 1.3.3

1. update native sdk.

2. add `setPlayVolume` API.

## 1.3.2

1. update native ios sdk.

## 1.3.1

1. update native sdk.
2. fix `onPublishStateUpdate` streamInfo is sometimes null.

## 1.3.0

1. update native sdk(2020.10.15).
2. add `switchRoom` API.

## 1.2.18

1. Update project configuration and README
2. Fix a crash in Android example demo

## 1.2.17

1. Fix problem with `setLogConfig` not working

## 1.2.16

1. fix crash when multi thread read/write the input CVPixelBuffer

## 1.2.15

1. fix the input CVPixelBuffer lifecycle issue.

## 1.2.14

1. fix CVPixelBuffer release crash.

## 1.2.13

1. add `setLogConfig` to set log path and log size
2. add `onDeviceError` for Notifying device abnormalities
3. add MediaPlayer module new API(for playing network sources): `setOnlineResourceCache`、`getOnlineResourceCache`、`setBufferThreshold`、`setLoadResourceTimeout`

## 1.2.12

1. add `MediaPlayer`(only audio module)

## 1.2.11

1. Update iOS native SDK to version `200424_175024_release-new-0-gb8ff47533_video_mediaplayer_bn3845_12`

2. Update Android native SDK to version `200424_175024_release-new-0-gb8ff47533_video_mediaplayer_bn3845_12`

## 1.2.10

- Add `setCamZoomFactor`, `getCamMaxZoomFactor` to adjust the camera zoom factor.

## 1.2.9

1. Add `sendBigRoomMessage`, `onRecvBigRoomMessage` to send and receive big room messages.

2. Add `onUpdateOnlineCount` callback to received online user number updates.

## 1.2.8

1. Update iOS native SDK to version `200415_195052_release-new-0-g18e243156_video_mediaplayer_bn3792_12`

2. Update Android native SDK to version `200410_212545_release-new-0-g18e243156_video_mediaplayer_bn3760_12`

## 1.2.7

1. Add log for texture render.

2. Optimize the code when calling `updatePreviewRenderSize` and `updatePlayRenderSize`

## 1.2.6

1. Added `setConfig` for setting additional configuration information.

2. Update iOS/Android native SDK to version `200316_163753_release-new-0-gb76c836cd_video_mediaplayer_bn3590_12`

## 1.2.5

1. Update ios/android native sdk to version `200309_200607_release-new-0-g54f488a0c_video_mediaplayer_bn3549_12`

2. Fix bug of external filter module on Android.

## 1.2.4

- Add external video filter interface and corresponding sample code.

## 1.2.3

- Add `setSharpenFactor` to set the sharpening factor for beauty.

## 1.2.2

- Add `sendRoomMessage`, `onRecvRoomMessage` to send and receive room messages.

## 1.2.1

- Update android native sdk.

## 1.2.0

1. Add `seekTo` `getTotalDuration` `getCurrentDuration` in AudioPlayer.

2. Add `enableLoopback` `setLoopbackVolume` to listen captured voice.

## 1.1.9

- Update iOS/Android native sdk (2020.2.9 hotfix)

## 1.1.8

- Update iOS/Android native sdk (2020.2.9)

## 1.1.7

- Fix bug of init page in example.

## 1.1.6

1. This new version is compatible with AndroidX.

2. Fix the Android example crash in release mode because of the obfuscation.

## 1.1.5

- Fix iOS: use platform view render failed.

## 1.1.4

- Fix AudioPlayer bug.

## 1.1.3

- Add Audio Precessing API: `VirtualStereo`, `Reverb`, `VoiceChanger`

## 1.1.2

1. Fix iOS callback doesn’t work when call `initSDK` failed

2. iOS native framework uses cocoapods dependency instead

## 1.1.1

1. Add audio player module

2. Update texture's API comment

## 1.0.12

- Update iOS/Android native sdk (12.9)

## 1.0.11

1. Add onInnerError

2. Update iOS/Android native sdk

## 1.0.10

1. Add relay CDN methods: `setPublishConfig`, `addPublishTarget`, `deletePublishTarget`, and callback: `onRelayCDNStateUpdate`

2. Fix bug that xcode11 can't run in debug mode

3. Update iOS/Android native sdk

## 1.0.9

- Fix the problem that Android can't receive media sideinfo callback

## 1.0.8

1. Add setVideoMirrorMode API

2. Update iOS/Android native sdk

## 1.0.7

1. Fix android texture render crash into background

2. Fix android startPlayingStream bugs

3. Update android native sdk

## 1.0.6

1. Fix some bug and crash

2. Add log system

## 1.0.5

1. Add peer to peer delay/package lost rate.

2. Add remote user's device state notification(on/off)

3. Add remote user's first frame notification

4. Update native sdk

5. promote stability

## 1.0.4

1. Add sound-level module

2. Add error-code module

3. Update native sdk

## 1.0.3

- Add Platform View API

## 1.0.2

1. Update native sdk

2. Fix crash

3. Refactor api structure

## 1.0.1

- Add format interface

## 1.0.0

- Initial release
