package com.zego.zegoliveroomplugin.module.mediaplayer;

public interface IZegoMediaPlayerControllerCallback {
    void onPlayEnd();

    void onPlayError(int errorCode);

    void onBufferBegin();

    void onBufferEnd();

    void onProcessInterval(long timestamp);

    void onPlayBegin();

    void onPlayPause();

    void onPlayResume();

    void onSeekComplete(int errorCode, long millisecond);

    void onAudioBegin();

    void onVideoBegin();
}
