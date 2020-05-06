package com.zego.zegoliveroomplugin.module.mediaplayer;

public interface IZegoMediaPlayerControllerCallback {
    void onPlayEnd();

    void onPlayError(int errorCode);

    void onBufferBegin();

    void onBufferEnd();

    void onProcessInterval(long timestamp);
}
