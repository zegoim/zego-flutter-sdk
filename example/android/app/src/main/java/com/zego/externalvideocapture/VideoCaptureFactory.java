package com.zego.externalvideocapture;

import android.content.Context;
 import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import com.zego.zegoavkit2.ZegoVideoCaptureDevice;
import com.zego.zegoavkit2.ZegoVideoCaptureFactory;
import com.zego.zegoliveroom.ZegoLiveRoom;
import com.zego.zegoliveroomplugin.module.mediaplayer.ZegoMediaPlayerController;

import static com.zego.zegoavkit2.ZegoVideoCaptureDevice.PIXEL_FORMAT_BGRA32;

/**
 * 外部采集工厂, 需要通过ZegoLiveRoom.setVideoCaptureFactory()设置到sdk
 * ZGVideoCaptureForMediaPlayer外部采集的入口，定义了创建、销毁 ZegoVideoCaptureDevice 的接口，
 * 向 SDK 提供管理 ZegoVideoCaptureDevice 生命周期的能力。需要调用
 * <strong>
 * 外部采集工厂设置到Zego SDK的时机必须是在 {@link ZegoLiveRoom#initSDK(long, byte[], Context)} 之前。
 * 否则外部采集功能将不生效
 * <p>
 * </strong>
 */
public class VideoCaptureFactory extends ZegoVideoCaptureFactory {

    private static final String TAG = "VideoCaptureFactory";

    private MediaPlayerVideoCapture mDevice;
    private final Handler mUIHandler = new Handler(Looper.getMainLooper());

    public VideoCaptureFactory() {
        mDevice = new MediaPlayerVideoCapture();
    }

    @Override
    protected ZegoVideoCaptureDevice create(String s) {
        Log.i(TAG, "create: s = " + s);
        mUIHandler.post(new Runnable() {
            @Override
            public void run() {
                ZegoMediaPlayerController.getInstance(0).setVideoDataCallback(mDevice, PIXEL_FORMAT_BGRA32);
            }
        });
        return mDevice;
    }

    @Override
    protected void destroy(ZegoVideoCaptureDevice zegoVideoCaptureDevice) {
        mUIHandler.post(new Runnable() {
            @Override
            public void run() {
                ZegoMediaPlayerController.getInstance(0).setVideoDataCallback(null, PIXEL_FORMAT_BGRA32);
            }
        });
    }
}
