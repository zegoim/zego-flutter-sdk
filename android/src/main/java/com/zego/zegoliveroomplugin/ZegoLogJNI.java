package com.zego.zegoliveroomplugin;

final public class ZegoLogJNI {
    static {
        System.loadLibrary("ZegoLog");
    }

    public static native void logNotice(String content);
}
