package com.zego.zegoliveroomplugin;

import android.util.Log;

final public class ZegoLogJNI {
    static {
        System.loadLibrary("ZegoLog");
    }

    public static void log(String content) {
        Log.d("ZEGO", content);
        ZegoLogJNI.logNotice(content);
    }

    public static native void logNotice(String content);
}
