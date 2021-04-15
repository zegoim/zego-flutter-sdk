package com.zego.externalvideocapture;

import android.os.Build;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import com.zego.zegoavkit2.IZegoMediaPlayerVideoPlayWithIndexCallback;
import com.zego.zegoavkit2.ZegoVideoCaptureDevice;
import com.zego.zegoavkit2.ZegoVideoDataFormat;
import com.zego.zegoliveroom.constants.ZegoVideoViewMode;
import java.nio.ByteBuffer;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

public class MediaPlayerVideoCapture extends ZegoVideoCaptureDevice implements
    IZegoMediaPlayerVideoPlayWithIndexCallback {

    private static final String TAG = "VideoCapture";

    // 因为生命周期回调和mediaPlayer回调是不同线程, 使用原子类
    AtomicBoolean runState = new AtomicBoolean(false);

    private Client mClient = null;

    volatile PixelBuffer pixelBuffer = null;

    private int mMaxBufferSize = 0;

    public MediaPlayerVideoCapture() {
    }

    /**
     * 开发者在 allocateAndStart 中获取到 client
     *
     * @param client 用于通知 SDK 采集结果, 包括设置采集的view模式
     */
    @Override
    protected void allocateAndStart(Client client) {
        Log.e(TAG, "allocateAndStart");
        mClient = client;
        mClient.setFillMode(ZegoVideoViewMode.ScaleToFill);
    }

    /**
     * 用于释放销毁一些资源
     */
    @Override
    protected void stopAndDeAllocate() {
        Log.e(TAG, "stopAndDeAllocate");
        mClient.destroy();
        mClient = null;
    }

    /**
     * 启用采集，sdk开始推流或开始预览的时候会调用该
     * 方法通知到开发者
     *
     * @return
     */
    @Override
    protected int startCapture() {
        Log.e(TAG, "startCapture");
        // sdk开始采集
        runState.set(true);
        return 0;
    }

    /**
     * 停止采集，sdk停止推流或停止预览时会调用该方法
     * 通知到开发者
     *
     * @return
     */
    @Override
    protected int stopCapture() {
        Log.e(TAG, "stopCapture");
        // sdk停止采集, 此时需要修改状态
        runState.set(false);
        return 0;
    }

    @Override
    protected int supportBufferType() {
        // sdk外部采集的方式, 使用内存模式
        return PIXEL_BUFFER_TYPE_MEM;
    }

    // mediaPlayer视频帧回调处理
    @Override
    public void onPlayVideoData(byte[] bytes, int size, ZegoVideoDataFormat f, int index) {
        // 获取buffer, 重复利用一个buff
        if (runState.get()) {
            long now;
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
                now = SystemClock.elapsedRealtimeNanos();
            } else {
                now = TimeUnit.MILLISECONDS.toNanos(SystemClock.elapsedRealtime());
            }

            // 视频帧传给ZEGO sdk
            PixelBuffer pixelBuffer = obtainPixelBuffer(bytes, size, f);
            mClient.onByteBufferFrameCaptured(pixelBuffer.buffer, size, pixelBuffer.format, now, 1000000000);
        }
    }

    private synchronized PixelBuffer obtainPixelBuffer(final byte[] bytes, int size, final ZegoVideoDataFormat format) {
        // buff大小变化才进行创建
        if (pixelBuffer == null || size != mMaxBufferSize) {
            if (pixelBuffer != null) {
                pixelBuffer.buffer.clear();
            } else {
                pixelBuffer = new PixelBuffer();
                pixelBuffer.format = new VideoCaptureFormat();
            }
 
            // TODO 创建buff要用allocateDirect方式创建, sdk才能正常收到数据
            pixelBuffer.buffer = ByteBuffer.allocateDirect(size);
            mMaxBufferSize = size;

            // TODO: 4/9/21
            // 设置编解码分辨率
            // ZGManager.getInstance().setZegoAvConfig(width, height);
        } else {
            pixelBuffer.buffer.clear();
        }

        pixelBuffer.buffer.put(bytes, 0, size);
        pixelBuffer.format.width = format.width;
        pixelBuffer.format.height = format.height;
        pixelBuffer.format.strides = format.strides;
        pixelBuffer.format.rotation = 0;
        pixelBuffer.format.pixel_format = PIXEL_FORMAT_BGRA32;

        return pixelBuffer;
    }

    private static class PixelBuffer {
        public ByteBuffer buffer;
        public VideoCaptureFormat format;
    }

    /* -------------以下是目前不需要关注的回调------------- */

    @Override
    protected int setFrameRate(int i) {
        return 0;
    }

    @Override
    protected int setResolution(int i, int i1) {
        return 0;
    }

    @Override
    protected int setFrontCam(int i) {
        return 0;
    }

    @Override
    protected int setView(View view) {
        return 0;
    }

    @Override
    protected int setViewMode(int i) {
        return 0;
    }

    @Override
    protected int setViewRotation(int i) {
        return 0;
    }

    @Override
    protected int setCaptureRotation(int i) {
        return 0;
    }

    @Override
    protected int startPreview() {
        return 0;
    }

    @Override
    protected int stopPreview() {
        return 0;
    }

    @Override
    protected int enableTorch(boolean b) {
        return 0;
    }

    @Override
    protected int takeSnapshot() {
        return 0;
    }

    @Override
    protected int setPowerlineFreq(int i) {
        return 0;
    }
}
