package com.zego.zegoliveroomplugin.module.audioplayer;

public interface IZegoAudioPlayerControllerCallback {
    void onAudioPlayEnd(int soundID);
    void onAudioPlayBegin(int soundID,int errorCode);
    void onAudioLoad(int soundID,int errorCode);
    void onAudioLoadComplete(int soundID);
}
