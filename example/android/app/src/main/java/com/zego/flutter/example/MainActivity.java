package com.zego.flutter.example;

import android.os.Bundle;

import io.flutter.app.FlutterActivity;
import io.flutter.plugins.GeneratedPluginRegistrant;

// 使用外部滤镜：导入 ZegoLiveRoomPlugin 以及自己实现的外部滤镜工厂文件
import com.zego.externalvideocapture.MediaPlayerVideoCapture;
import com.zego.externalvideocapture.VideoCaptureFactory;
import com.zego.zegoavkit2.ZegoVideoCaptureFactory;
import com.zego.zegoliveroomplugin.ZegoLiveRoomPlugin;
import com.zego.ExternalVideoFilter.VideoFilterFactoryDemo;
import com.zego.zegoliveroomplugin.module.mediaplayer.ZegoMediaPlayerController;

import static com.zego.zegoavkit2.ZegoVideoCaptureDevice.PIXEL_FORMAT_BGRA32;


public class MainActivity extends FlutterActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


        // 创建一个自己实现的外部视频滤镜工厂，此处为一个 Demo 工厂
        VideoFilterFactoryDemo factory = new VideoFilterFactoryDemo();
        // 选择使用 FilterTypeMem（异步 BGRA32) 类型滤镜
        factory.filterType = VideoFilterFactoryDemo.FilterType.FilterTypeMem;
        // 设给 ZegoLiveRoomPlugin 暂存，当调用 dart 的接口 `enableExternalVideoFilterFactory` 时才真正调用 Native SDK 接口设入
        ZegoLiveRoomPlugin.setExternalVideoFilterFactory(factory);


        ZegoLiveRoomPlugin.setExternalVideoCaptureFactory(new VideoCaptureFactory());

        GeneratedPluginRegistrant.registerWith(this);
    }
}
