package com.zego.zegoliveroomplugin;

import android.content.Context;

import java.util.HashMap;

import io.flutter.plugin.common.MessageCodec;
import io.flutter.plugin.common.StandardMessageCodec;
import io.flutter.plugin.platform.PlatformView;
import io.flutter.plugin.platform.PlatformViewFactory;

public class ZegoPlatformViewFactory extends PlatformViewFactory{

    private static ZegoPlatformViewFactory sInstance;

    private HashMap<Integer, ZegoPlatformView> mViews;

    public static ZegoPlatformViewFactory shareInstance() {
        if(sInstance == null) {
            sInstance = new ZegoPlatformViewFactory(StandardMessageCodec.INSTANCE);
            ZegoLogJNI.log("[ZegoPlatformViewFactory] instance init");
        }

        return sInstance;
    }

    private ZegoPlatformViewFactory(MessageCodec<Object> createArgsCodec) {
        super(createArgsCodec);
        mViews = new HashMap<>();
    }

    public void addView(int viewID, ZegoPlatformView view) {
        ZegoLogJNI.log("[ZegoPlatformViewFactory] addView: " + viewID);
        mViews.put(Integer.valueOf(viewID), view);
    }

    public boolean removeView(int viewID) {

        if(!mViews.containsKey(Integer.valueOf(viewID))) {
            ZegoLogJNI.log("[ZegoPlatformViewFactory] removeView failed, no such view: " + viewID);
            return false;
        }

        ZegoLogJNI.log("[ZegoPlatformViewFactory] removeView: " + viewID);
        mViews.remove(Integer.valueOf(viewID));
        return true;
    }

    public ZegoPlatformView getPlatformView(int viewID) {
        ZegoLogJNI.log("[ZegoPlatformViewFactory] getView: " + viewID);
        return mViews.get(Integer.valueOf(viewID));
    }

    @Override
    public PlatformView create(Context context, int viewID, Object args) {
        ZegoLogJNI.log("[ZegoPlatformViewFactory] create view, viewID: " + viewID);
        ZegoPlatformView view = new ZegoPlatformView(context, viewID);
        addView(viewID, view);

        return view;
    }
}
