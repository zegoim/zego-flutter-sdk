package com.zego.ExternalVideoFilter.VideoFilter;

import android.graphics.SurfaceTexture;
import android.os.Build;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;

import com.zego.zegoavkit2.videofilter.ZegoVideoFilter;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.CountDownLatch;

/**
 * 外部滤镜采用 BUFFER_TYPE_MEM（异步传递 RGBA32 图像数据）方式传递数据给 SDK。
 *
 * Created by robotding on 16/12/3.
 */

public class VideoFilterMemDemo extends ZegoVideoFilter {
    private static final String TAG = "VideoFilterMemDemo";

    // SDK 内部实现 ZegoVideoFilter.Client 协议的对象
    private ZegoVideoFilter.Client mClient = null;

    private HandlerThread mThread = null;
    private volatile Handler mHandler = null;

    // 图像数据信息
    static class PixelBuffer {
        public int width;
        public int height;
        public int stride;
        public long timestamp_100n;
        public ByteBuffer buffer;
    }
    private ArrayList<PixelBuffer> mProduceQueue = new ArrayList<PixelBuffer>();
    private int mWriteIndex = 0;
    private int mWriteRemain = 0;
    private ConcurrentLinkedQueue<PixelBuffer> mConsumeQueue = new ConcurrentLinkedQueue<PixelBuffer>();
    private int mMaxBufferSize = 0;

    private boolean mIsRunning = false;

    @Override
    protected void allocateAndStart(Client client) {
        mClient = client;
        mThread = new HandlerThread("video-filter");
        mThread.start();
        mHandler = new Handler(mThread.getLooper());
        mIsRunning = true;

        mProduceQueue.clear();
        mConsumeQueue.clear();
        mWriteIndex = 0;
        mWriteRemain = 0;
        mMaxBufferSize = 0;
    }

    /**
     * 释放资源
     * 注意：必须调用 client 的 destroy 方法，否则会造成内存泄漏。
     */
    @Override
    protected void stopAndDeAllocate() {
        mIsRunning = false;

        final CountDownLatch barrier = new CountDownLatch(1);
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                barrier.countDown();
            }
        });
        try {
            barrier.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        mHandler = null;

        if (Build.VERSION.SDK_INT >= 18) {
            mThread.quitSafely();
        } else {
            mThread.quit();
        }
        mThread = null;

        mClient.destroy();
        mClient = null;
    }

    /**
     * 指定滤镜的传递数据类型，异步传递 RGBA32 图像数据
     * SDK 需要根据 supportBufferType 返回的类型值创建不同的 client 对象。
     * @return
     */
    @Override
    protected int supportBufferType() {
        return BUFFER_TYPE_MEM;
    }

    /**
     * SDK 通知外部滤镜当前采集图像的宽高并请求内存池下标
     * @param width 采集图像宽
     * @param height 采集图像高
     * @param stride
     * @return
     */
    @Override
    protected synchronized int dequeueInputBuffer(int width, int height, int stride) {
        if (stride * height > mMaxBufferSize) {
            if (mMaxBufferSize != 0) {
                mProduceQueue.clear();
            }

            mMaxBufferSize = stride * height;
            createPixelBufferPool(4);
        }

        if (mWriteRemain == 0) {
            return -1;
        }

        mWriteRemain--;
        return (mWriteIndex + 1) % mProduceQueue.size();
    }

    /**
     * SDK 请求外部滤镜返回对应内存池下标的 ByteBuffer（ByteBuffer.allocateDirect方式分配的）
     * @param index dequeueInputBuffer 返回的内存池下标即 SDK 获得的内存池下标
     * @return
     */
    @Override
    protected synchronized ByteBuffer getInputBuffer(int index) {
        if (mProduceQueue.isEmpty()) {
            return null;
        }
        ByteBuffer buffer = mProduceQueue.get(index).buffer;
        buffer.position(0);
        return buffer;
    }

    /**
     * SDK 抛出图像数据，外部滤镜进行处理
     *
     * @param bufferIndex
     * @param width
     * @param height
     * @param stride
     * @param timestamp_100n
     */
    @Override
    protected synchronized void queueInputBuffer(int bufferIndex, final int width, int height, int stride, long timestamp_100n) {
        if (bufferIndex == -1) {
            return ;
        }

        // 根据 SDK 返回的 bufferIndex 索引取相应的图像数据
        PixelBuffer pixelBuffer = mProduceQueue.get(bufferIndex);
        pixelBuffer.width = width;
        pixelBuffer.height = height;
        pixelBuffer.stride = stride;
        pixelBuffer.timestamp_100n = timestamp_100n;
        pixelBuffer.buffer.limit(height * stride);
        mConsumeQueue.add(pixelBuffer);

        mWriteIndex = (mWriteIndex + 1) % mProduceQueue.size();

        // 切换线程，异步进行前处理
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                if (!mIsRunning) {
                    Log.e(TAG, "already stopped");
                    return ;
                }

                // 取出图像数据
                PixelBuffer pixelBuffer = getConsumerPixelBuffer();
                byte[] bgra32 = new byte[stride * height];
                pixelBuffer.buffer.get(bgra32);

                for (int i = 0; i < (stride * height); i += 4) {
                    int b = bgra32[i];
                    int g = bgra32[i + 1];
                    int r = bgra32[i + 2];
                    bgra32[i] = (byte) (255 - b);
                    bgra32[i + 1] = (byte) (255 - g);
                    bgra32[i + 2] = (byte) (255 - r);
                }

                // 获取buffer下标
                int index = mClient.dequeueInputBuffer(pixelBuffer.width, pixelBuffer.height, pixelBuffer.stride);
                if (index >= 0) {
                    ByteBuffer dst = mClient.getInputBuffer(index);
                    dst.position(0);
//                    pixelBuffer.buffer.position(0);
//                    dst.put(pixelBuffer.buffer);
                    dst.put(bgra32);

                    mClient.queueInputBuffer(index, pixelBuffer.width, pixelBuffer.height, pixelBuffer.stride, pixelBuffer.timestamp_100n);
                }

                returnProducerPixelBuffer(pixelBuffer);
            }
        });
    }

    @Override
    protected SurfaceTexture getSurfaceTexture() {
        return null;
    }

    @Override
    protected void onProcessCallback(int textureId, int width, int height, long timestamp_100n) {

    }

    // 创建存放图像数据的 buffer
    private void createPixelBufferPool(int count) {
        for (int i = 0; i < count; i++) {
            PixelBuffer pixelBuffer = new PixelBuffer();
            pixelBuffer.buffer = ByteBuffer.allocateDirect(mMaxBufferSize);
            mProduceQueue.add(pixelBuffer);
        }

        mWriteRemain = count;
        mWriteIndex = -1;
    }

    private PixelBuffer getConsumerPixelBuffer() {
        if (mConsumeQueue.isEmpty()) {
            return null;
        }
        return mConsumeQueue.poll();
    }

    private synchronized void returnProducerPixelBuffer(PixelBuffer pixelBuffer) {
        if (pixelBuffer.buffer.capacity() == mMaxBufferSize) {
            mWriteRemain++;
        }
    }
}
