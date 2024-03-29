package com.zego.zegoliveroomplugin.module.mediaplayer;

import android.content.Context;
import android.graphics.Bitmap;

import com.zego.zegoavkit2.IZegoMediaPlayerVideoPlayWithIndexCallback;
import com.zego.zegoavkit2.IZegoMediaPlayerWithIndexCallback;
import com.zego.zegoavkit2.ZegoMediaPlayer;
import com.zego.zegoliveroom.ZegoLiveRoom;
import com.zego.zegoliveroomplugin.ZegoPlatformView;
import com.zego.zegoliveroomplugin.ZegoViewRenderer;
import com.zego.zegoliveroomplugin.utils.ZegoFileHelper;

import java.util.HashMap;

import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.PluginRegistry.Registrar;

public class ZegoMediaPlayerController implements IZegoMediaPlayerWithIndexCallback {

    private static String KEY_START = "START";
    private static String KEY_STOP = "STOP";
    private static String KEY_PAUSE = "PAUSE";
    private static String KEY_RESUME = "RESUME";
    private static String KEY_LOAD = "LOAD";
    private static String KEY_SEEK_TO = "SEEK_TO";

    private static ZegoMediaPlayerController sInstance;
    private ZegoMediaPlayer mPlayer;

    private boolean isPlay = false;

    private HashMap<String, Result> mResultMap;
    //private HashMap<Integer, Result> mStartResultList;
    //private HashMap<Integer, Result> mLoadResultList;

    private IZegoMediaPlayerControllerCallback mCallback = null;
    private IZegoMediaPlayerVideoPlayWithIndexCallback mPlayVideoCallback = null;
    private int mVideoDataFormat;
    public int mediaPlayerIndex;
    private ZegoPlatformView mZegoPlatformView = null;
    private ZegoViewRenderer mZegoViewRenderer = null;

    public ZegoMediaPlayerController(int playerIndex) {
        mResultMap = new HashMap<>();
        mediaPlayerIndex = playerIndex;
        init(playerIndex);
    }

    public void init(int playerIndex) {
        mPlayer = new ZegoMediaPlayer();
        mPlayer.init(ZegoMediaPlayer.PlayerTypeAux, playerIndex);
        mPlayer.setEventWithIndexCallback(this);
        if (mPlayVideoCallback != null) {
            mPlayer.setVideoPlayWithIndexCallback(mPlayVideoCallback, mVideoDataFormat);
        }
    }

    public void setZegoPlatformView(ZegoPlatformView platformView){
        mZegoPlatformView = platformView;
    }

    public ZegoPlatformView getZegoPlatformView(){
        return mZegoPlatformView;
    }

    public ZegoViewRenderer getZegoViewRenderer() {
        return mZegoViewRenderer;
    }

    public void setZegoViewRenderer(ZegoViewRenderer zegoViewRenderer) {
        mZegoViewRenderer = zegoViewRenderer;
    }

    public void uninit() {
        mPlayer.setEventWithIndexCallback(null);
        if (mPlayVideoCallback != null) {
            mPlayer.setVideoPlayWithIndexCallback(null, mVideoDataFormat);
            mPlayVideoCallback = null;
        }
        mPlayer.uninit();
        mPlayer = null;
    }

    public void setVideoDataCallback(final IZegoMediaPlayerVideoPlayWithIndexCallback callback, final int format) {
        mPlayVideoCallback = callback;
        mVideoDataFormat = format;
        if (mPlayer != null) {
            mPlayer.setVideoPlayWithIndexCallback(mPlayVideoCallback, mVideoDataFormat);
        }
    }

    public static ZegoMediaPlayerController getInstance(int playerIndex) {
        if (sInstance == null) {
            Class var0 = ZegoMediaPlayerController.class;
            synchronized (ZegoMediaPlayerController.class) {
                if (sInstance == null) {
                    sInstance = new ZegoMediaPlayerController(playerIndex);
                }
            }
        }
        return sInstance;
    }

    public void start(String path, boolean isRepeat, boolean isAsset, Registrar registrar, Result result) {

        mResultMap.put(KEY_START, result);

        if (isAsset) {

            if (path != null && !path.isEmpty()) {
                String loopUpKey = registrar.lookupKeyForAsset(path);
                playEffectAsync(registrar.context(), loopUpKey, isRepeat);
            } else {
                mPlayer.start("", isRepeat);
            }

        } else {
            mPlayer.start(path, isRepeat);
        }
    }

    public void stop(Result result) {

        mResultMap.put(KEY_STOP, result);
        mPlayer.stop();
    }

    public void pause(Result result) {

        mResultMap.put(KEY_PAUSE, result);
        mPlayer.pause();
    }

    public void resume(Result result) {

        mResultMap.put(KEY_RESUME, result);
        mPlayer.resume();
    }

    public void preload(String path, boolean isAsset, Registrar registrar, Result result) {

        mResultMap.put(KEY_LOAD, result);
        if (isAsset) {
            String loopUpKey = registrar.lookupKeyForAsset(path);
            preloadEffectAsync(registrar.context(), loopUpKey);
        } else {
            mPlayer.load(path);
        }

        //mLoadResultList.put(Integer.valueOf(soundID), result);
    }

    public void setVolume(int volume, Result result) {

        mPlayer.setVolume(volume);
        result.success(null);
    }

    public void setMediaPlayerEventCallback(IZegoMediaPlayerControllerCallback callback) {
        mCallback = callback;
    }

    public void seekTo(long timestamp, Result result) {

        mResultMap.put(KEY_SEEK_TO, result);
        mPlayer.seekTo(timestamp);
        //result.success(null);
    }

    public void getTotalDuration(Result result) {

        result.success(mPlayer.getDuration());
    }

    public void getCurrentDuration(Result result) {

        result.success(mPlayer.getCurrentDuration());
    }

    public void muteLocal(boolean mute, Result result) {
        mPlayer.muteLocal(mute);
        result.success(null);
    }

    public void setPlayerType(int type, Result result) {
        mPlayer.setPlayerType(type);
        result.success(null);
    }

    public void enableRepeatMode(boolean enable, Result result) {
        mPlayer.enableRepeatMode(enable);
        result.success(null);
    }

    public void setProcessInterval(long timestamp, Result result) {
        mPlayer.setProcessInterval(timestamp);
        result.success(null);
    }

    public void setOnlineResourceCache(int time, int size) {
        mPlayer.setOnlineResourceCache(time, size);
    }

    public void setAudioStream(int streamIndex) {
        mPlayer.setAudioStream(streamIndex);
    }

    public void getAudioStreamCount(Result result) {
        result.success(mPlayer.getAudioStreamCount());
    }

    public void getPublishVolume(Result result) {
        result.success(mPlayer.getPublishVolume());
    }

    public void getPlayVolume(Result result) {
        result.success(mPlayer.getPlayVolume());
    }

    public void setPublishVolume(int volume) {
        mPlayer.setPublishVolume(volume);
    }

    public void setPlayVolume(int volume) {
        mPlayer.setPlayVolume(volume);
    }

    public void setView(Object view) {
        mPlayer.setView(view);
    }

    public void getOnlineResourceCache(Result result) {
        ZegoMediaPlayer.CacheStat cacheStat = new ZegoMediaPlayer.CacheStat();
        mPlayer.getOnlineResourceCacheStat(cacheStat);
        HashMap<String, Object> returnMap = new HashMap<>();
        returnMap.put("time", cacheStat.time);
        returnMap.put("size", cacheStat.size);
        result.success(returnMap);
    }

    public void setBufferThreshold(int threshold) {
        mPlayer.setBufferThreshold(threshold);
    }

    public void setLoadResourceTimeout(int timeout) {
        mPlayer.setLoadResourceTimeout(timeout);
    }

    private void playEffectAsync(final Context context, final String fileName, final boolean isRepeat) {
        new Thread() {
            @Override
            public void run() {
                final String path = ZegoFileHelper.copyAssetsFile2Phone(context, fileName);
                // copy完成
                mPlayer.start(path, isRepeat);

            }
        }.start();

    }

    private void preloadEffectAsync(final Context context, final String fileName) {
        new Thread() {
            @Override
            public void run() {
                final String path = ZegoFileHelper.copyAssetsFile2Phone(context, fileName);
                // copy完成
                mPlayer.load(path);
            }
        }.start();
    }

    @Override
    public void onPlayEnd(int index) {
        if (mCallback != null) {
            mCallback.onPlayEnd(mediaPlayerIndex);
        }
        isPlay = false;
    }

    @Override
    public void onPlayStart(int index) {
        Result result = mResultMap.get(KEY_START);
        if (result != null) {
            result.success(null);
            mResultMap.remove(KEY_START);
        }
        if (mCallback!=null){
            mCallback.onPlayBegin(mediaPlayerIndex);
        }
        isPlay = true;
    }

    @Override
    public void onPlayPause(int index) {
        Result result = mResultMap.get(KEY_PAUSE);
        if (result != null) {
            result.success(null);
            mResultMap.remove(KEY_PAUSE);
        }
        if (mCallback != null){
            mCallback.onPlayPause(mediaPlayerIndex);
        }
        isPlay = false;
    }

    @Override
    public void onPlayStop(int index) {
        Result result = mResultMap.get(KEY_STOP);
        if (result != null) {
            result.success(null);
            mResultMap.remove(KEY_STOP);

        }

        if (mCallback != null){
            mCallback.onPlayStop(mediaPlayerIndex);
        }

        isPlay = false;
    }

    @Override
    public void onPlayResume(int var1) {
        Result result = mResultMap.get(KEY_RESUME);
        if (result != null) {
            result.success(null);
            mResultMap.remove(KEY_RESUME);
        }
        if (mCallback != null){
            mCallback.onPlayResume(mediaPlayerIndex);
        }
        isPlay = true;
    }

    @Override
    public void onPlayError(int error, int index) {
        if (mCallback != null) {
            mCallback.onPlayError(error, mediaPlayerIndex);
        }
        isPlay = false;
    }

    @Override
    public void onVideoBegin(int index) {
        if (mCallback != null){
            mCallback.onVideoBegin(mediaPlayerIndex);
        }
        isPlay = true;
    }

    @Override
    public void onAudioBegin(int index) {
        if (mCallback != null){
            mCallback.onAudioBegin(mediaPlayerIndex);
        }
        isPlay = true;
    }

    @Override
    public void onBufferBegin(int var1) {
        if (mCallback != null) {
            mCallback.onBufferBegin(mediaPlayerIndex);
        }
    }

    @Override
    public void onBufferEnd(int var1) {
        if (mCallback != null) {
            mCallback.onBufferEnd(mediaPlayerIndex);
        }
    }

    @Override
    public void onSeekComplete(int error, long timestamp, int index) {
        Result result = mResultMap.get(KEY_SEEK_TO);
        if (result != null) {
            HashMap<String, Object> retMap = new HashMap<>();
            retMap.put("errorCode", error);
            retMap.put("timestamp", timestamp);
            result.success(retMap);

            mResultMap.remove(KEY_SEEK_TO);
        }
        if (mCallback != null){
            mCallback.onSeekComplete(error,timestamp, mediaPlayerIndex);
        }
        isPlay = true;
    }

    @Override
    public void onSnapshot(Bitmap var1, int var2) {

    }

    @Override
    public void onLoadComplete(int var1) {
        Result result = mResultMap.get(KEY_LOAD);
        if (result != null) {
            result.success(null);
            mResultMap.remove(KEY_LOAD);
        }
        isPlay = true;
    }

    @Override
    public void onProcessInterval(long timestamp, int index) {
        if (mCallback != null) {
            mCallback.onProcessInterval(timestamp, mediaPlayerIndex);
        }
    }

    @Override
    public void onReadEOF(int i) {

    }

    private boolean numberToBoolValue(Boolean number) {

        return number != null ? number.booleanValue() : false;
    }

    private int numberToIntValue(Number number) {

        return number != null ? number.intValue() : 0;
    }

    private long numberToLongValue(Number number) {

        return number != null ? number.longValue() : 0;
    }

    public boolean getMediaPlayerState(){
        return isPlay;
    }
}
