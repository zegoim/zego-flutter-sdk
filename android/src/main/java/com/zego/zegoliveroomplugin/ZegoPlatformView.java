package com.zego.zegoliveroomplugin;

import android.content.Context;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import io.flutter.plugin.platform.PlatformView;

public class ZegoPlatformView implements PlatformView, SurfaceHolder.Callback {

    private SurfaceView mSurfaceView;
    private int mViewID;

    public ZegoPlatformView(Context context, int viewID) {
        this.mViewID = viewID;
        this.mSurfaceView = new SurfaceView(context);
        this.mSurfaceView.getHolder().addCallback(this);

        ZegoLogJNI.log("[ZegoPlatformView] new, viewID: " + viewID + ", context: " + context);

    }

    public SurfaceView getSurfaceView() {
        return mSurfaceView;
    }

    @Override
    public View getView() {
        return mSurfaceView;
    }

    @Override
    public void dispose() {
        ZegoLogJNI.log("[ZegoPlatformView] dispose, viewID: " + mViewID);
        if(mSurfaceView != null) {
            mSurfaceView.getHolder().removeCallback(this);
            Surface surface = mSurfaceView.getHolder().getSurface();
            if(surface != null)
                surface.release();

            mSurfaceView = null;
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        ZegoLogJNI.log("[ZegoPlatformView] onSurfaceCreated, viewID: " + mViewID);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        ZegoLogJNI.log("[ZegoPlatformView] onSurfaceChanged, viewID: " + mViewID + ", size: (" + width + ", " + height + ")" + ", format: " + format);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        ZegoLogJNI.log("[ZegoPlatformView] onSurfaceDestroyed");
    }
}
