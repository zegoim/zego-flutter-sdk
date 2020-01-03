package com.zego.zegoliveroomplugin.module.audioplayer;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.MediaPlayer;
import android.os.Handler;
import android.os.Looper;

import com.zego.zegoavkit2.audioplayer.IZegoAudioPlayerCallback;
import com.zego.zegoavkit2.audioplayer.ZegoAudioPlayer;
import com.zego.zegoliveroomplugin.utils.ZegoFileHelper;

import java.io.IOException;
import java.util.HashMap;

import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.PluginRegistry.Registrar;

public class ZegoAudioPlayerController implements IZegoAudioPlayerCallback {

    private static ZegoAudioPlayerController sInstance;
    private ZegoAudioPlayer mPlayer;

    private HashMap<Integer, Result> mStartResultList;
    private HashMap<Integer, Result> mLoadResultList;

    private IZegoAudioPlayerControllerCallback mCallback = null;

    public ZegoAudioPlayerController() {
        mStartResultList = new HashMap<>();
        mLoadResultList = new HashMap<>();
    }

    public void init() {
        mPlayer = new ZegoAudioPlayer();
        mPlayer.setCallback(this);
    }

    public void uninit() {
        mPlayer.setCallback(null);
        mPlayer = null;
    }

    public static ZegoAudioPlayerController getInstance() {
        if (sInstance == null) {
            Class var0 = ZegoAudioPlayerController.class;
            synchronized(ZegoAudioPlayerController.class) {
                if (sInstance == null) {
                    sInstance = new ZegoAudioPlayerController();
                }
            }
        }
        return sInstance;
    }

    public void playAudioEffect(MethodCall call, Registrar registrar, Result result) {

        int soundID = numberToIntValue((Number) call.argument("soundID"));
        String path = call.argument("path");
        boolean isAssetPath = numberToBoolValue((Boolean) call.argument("isAssetPath"));
        int loopCount = numberToIntValue((Number) call.argument("loopCount"));
        boolean isPublish = numberToBoolValue((Boolean) call.argument("isPublish"));

        if(isAssetPath) {

            if(path!= null && !path.isEmpty()) {
                String loopUpKey = registrar.lookupKeyForAsset(path);
                playEffectAsync(registrar.context(), loopUpKey, soundID, loopCount, isPublish);
            } else {
                mPlayer.playEffect("", soundID, loopCount, isPublish);
            }

        } else {
            mPlayer.playEffect(path, soundID, loopCount, isPublish);
        }

        mStartResultList.put(Integer.valueOf(soundID), result);
    }

    public void stopAudioEffect(MethodCall call, Result result) {

        int soundID = numberToIntValue((Number) call.argument("soundID"));
        mPlayer.stopEffect(soundID);
        result.success(null);
    }

    public void pauseAudioEffect(MethodCall call, Result result) {

        int soundID = numberToIntValue((Number) call.argument("soundID"));
        mPlayer.pauseEffect(soundID);
        result.success(null);
    }

    public void resumeAudioEffect(MethodCall call, Result result) {

        int soundID = numberToIntValue((Number) call.argument("soundID"));
        mPlayer.resumeEffect(soundID);
        result.success(null);
    }

    public void preloadAudioEffect(MethodCall call, Registrar registrar, Result result) {

        int soundID = numberToIntValue((Number) call.argument("soundID"));
        String path = call.argument("path");
        boolean isAssetPath = numberToBoolValue((Boolean) call.argument("isAssetPath"));

        if(isAssetPath) {
            String loopUpKey = registrar.lookupKeyForAsset(path);
            preloadEffectAsync(registrar.context(), loopUpKey, soundID);
        } else {
            mPlayer.preloadEffect(path, soundID);
        }

        mLoadResultList.put(Integer.valueOf(soundID), result);
    }

    public void unloadAudioEffect(MethodCall call, Result result) {

        int soundID = numberToIntValue((Number) call.argument("soundID"));
        mPlayer.unloadEffect(soundID);
        result.success(null);
    }

    public void setAudioEffectVolume(MethodCall call, Result result) {

        int soundID = numberToIntValue((Number) call.argument("soundID"));
        int vol = numberToIntValue((Number) call.argument("volume"));
        mPlayer.setVolume(soundID, vol);
        result.success(null);
    }

    public void setAllEffectVolume(MethodCall call, Result result) {

        int vol = numberToIntValue((Number) call.argument("volume"));
        mPlayer.setVolumeAll(vol);
        result.success(null);
    }

    public void pauseAllEffect(Result result) {
        mPlayer.pauseAll();
        result.success(null);
    }

    public void resumeAllEffect(Result result) {
        mPlayer.resumeAll();
        result.success(null);
    }

    public void stopAllEffect(Result result) {
        mPlayer.stopAll();
        result.success(null);
    }

    public void setAudioPlayerEventCallback(IZegoAudioPlayerControllerCallback callback) {
        mCallback = callback;
    }

    private void playEffectAsync(final Context context, final String fileName, final int soundID, final int loopCount, final boolean isPublish) {
        new Thread() {
            @Override
            public void run() {
                final String path = ZegoFileHelper.copyAssetsFile2Phone(context, fileName);
                // copy完成
                mPlayer.playEffect(path, soundID, loopCount, isPublish);

            }
        }.start();

    }

    private void preloadEffectAsync(final Context context, final String fileName, final int soundID) {
        new Thread(){
            @Override
            public void run() {
                final String path = ZegoFileHelper.copyAssetsFile2Phone(context, fileName);
                // copy完成
                mPlayer.preloadEffect(path, soundID);
            }
        }.start();
    }


    @Override
    public void onPlayEffect(int soundID, int errorCode) {

        Result result = mStartResultList.get(Integer.valueOf(soundID));
        if(result != null) {
            HashMap<String, Object> retMap = new HashMap<>();
            retMap.put("errorCode", errorCode);
            retMap.put("soundID", soundID);
            result.success(retMap);

            mStartResultList.remove(Integer.valueOf(soundID));
        }
    }

    @Override
    public void onPlayEnd(int soundID) {
        if(mCallback != null) {
            mCallback.onAudioPlayEnd(soundID);
        }
    }

    @Override
    public void onPreloadEffect(int soundID, int errorCode) {

        Result result = mLoadResultList.get(Integer.valueOf(soundID));
        if(result != null) {
            HashMap<String, Object> retMap = new HashMap<>();
            retMap.put("errorCode", errorCode);
            retMap.put("soundID", soundID);
            result.success(retMap);

            mLoadResultList.remove(Integer.valueOf(soundID));
        }
    }

    @Override
    public void onPreloadComplete(int i) {

    }

    private boolean numberToBoolValue(Boolean number) {

        return number != null ? number.booleanValue() : false;
    }

    private int numberToIntValue(Number number) {

        return number != null ? number.intValue() : 0;
    }
}
