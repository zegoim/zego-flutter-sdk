package com.zego.zegoliveroomplugin.module.mediaplayer;

public interface IZegoMediaPlayerControllerCallback {
    void onPlayEnd(int playerIndex);

    void onPlayError(int errorCode,int playerIndex);

    void onBufferBegin(int playerIndex);

    void onBufferEnd(int playerIndex);

    void onProcessInterval(long timestamp,int playerIndex);

    void onPlayBegin(int playerIndex);

    void onPlayStop(int playerIndex);

    void onPlayPause(int playerIndex);

    void onPlayResume(int playerIndex);

    void onSeekComplete(int errorCode, long millisecond,int playerIndex);

    void onAudioBegin(int playerIndex);

    void onVideoBegin(int playerIndex);
}
