package com.zego.zegoliveroomplugin;

import android.graphics.SurfaceTexture;
import android.view.Surface;
import io.flutter.view.TextureRegistry;


public class ZegoViewRenderer {

    private final TextureRegistry.SurfaceTextureEntry textureEntry;

    private final long mTextureID;
    private SurfaceTexture surfaceTexture;
    private Surface surface;

    private int mViewWidth;
    private int mViewHeight;

    private boolean mIsInited;


    public ZegoViewRenderer(TextureRegistry.SurfaceTextureEntry textureEntry,
                            int viewWidth,
                            int viewHeight) {
        this.textureEntry = textureEntry;
        this.mTextureID = textureEntry.id();
        this.surfaceTexture = textureEntry.surfaceTexture();
        this.surfaceTexture.setDefaultBufferSize(viewWidth, viewHeight);
        this.surface = new Surface(this.surfaceTexture);

        this.mViewWidth = viewWidth;
        this.mViewHeight = viewHeight;


        init();
    }

    public void init() {
        mIsInited = true;
    }

    public void release() {
        
        textureEntry.release();
        
        if(surface != null) {
            surface.release();
        }

        mIsInited = false;
    }

    public boolean updateRenderSize(final int width, final int height) {

        if(mViewWidth != width || mViewHeight != height) {
            mViewWidth = width;
            mViewHeight = height;

            surfaceTexture.setDefaultBufferSize(mViewWidth, mViewHeight);
            surface.release();

            surface = new Surface(surfaceTexture);

            return true;
        }

        return false;
    }

    public Surface getSurface() {
        return surface;
    }

    public long getTextureID() {
        return mTextureID;
    }
}
