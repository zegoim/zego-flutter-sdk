package com.zego.zegoliveroomplugin;

import android.content.Context;
import android.view.SurfaceView;
import android.view.View;

import io.flutter.plugin.platform.PlatformView;

public class ZegoPlatformView implements PlatformView {

    private SurfaceView mSurfaceView;

    public ZegoPlatformView(Context context) {
        this.mSurfaceView = new SurfaceView(context);
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

    }
}
