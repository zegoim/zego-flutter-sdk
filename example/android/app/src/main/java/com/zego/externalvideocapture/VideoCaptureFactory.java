package com.zego.externalvideocapture;

import android.content.Context;
import android.util.Log;
import com.zego.zegoavkit2.ZegoVideoCaptureDevice;
import com.zego.zegoavkit2.ZegoVideoCaptureFactory;
import com.zego.zegoliveroom.ZegoLiveRoom;

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

    public VideoCaptureFactory(MediaPlayerVideoCapture device) {
        mDevice = device;
    }

    @Override protected ZegoVideoCaptureDevice create(String s) {
        Log.i(TAG, "create: s = " + s);
        return mDevice;
    }

    @Override protected void destroy(ZegoVideoCaptureDevice zegoVideoCaptureDevice) {
        mDevice = null;
    }

    public MediaPlayerVideoCapture getDevice() {
        return mDevice;
    }
}
