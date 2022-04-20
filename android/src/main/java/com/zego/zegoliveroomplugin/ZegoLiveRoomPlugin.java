package com.zego.zegoliveroomplugin;

import android.app.Application;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.Build;
import android.view.Surface;
import android.os.Looper;
import android.os.Handler;


import androidx.annotation.RequiresApi;

import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.EventChannel.EventSink;
import io.flutter.plugin.common.PluginRegistry.Registrar;
import io.flutter.view.TextureRegistry;

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import java.util.*;
import java.lang.*;

import com.zego.zegoavkit2.ZegoExternalVideoCapture;
import com.zego.zegoavkit2.ZegoMediaPlayer;
import com.zego.zegoavkit2.ZegoStreamExtraPlayInfo;
import com.zego.zegoavkit2.ZegoVideoCaptureFactory;
import com.zego.zegoavkit2.audioprocessing.ZegoAudioProcessing;
import com.zego.zegoavkit2.audioprocessing.ZegoAudioReverbMode;
import com.zego.zegoavkit2.audioprocessing.ZegoAudioReverbParam;
import com.zego.zegoavkit2.camera.ZegoCamera;
import com.zego.zegoavkit2.entities.ZegoStreamRelayCDNInfo;
import com.zego.zegoavkit2.error.ZegoError;
import com.zego.zegoavkit2.mediaside.IZegoMediaSideCallback;
import com.zego.zegoavkit2.mediaside.ZegoMediaSideInfo;
import com.zego.zegoavkit2.soundlevel.IZegoSoundLevelCallback;
import com.zego.zegoavkit2.soundlevel.ZegoSoundLevelInfo;
import com.zego.zegoavkit2.soundlevel.ZegoSoundLevelMonitor;
import com.zego.zegoavkit2.videofilter.ZegoExternalVideoFilter;
import com.zego.zegoavkit2.videofilter.ZegoVideoFilterFactory;
import com.zego.zegoavkit2.ZegoConstants.PublishChannelIndex;
import com.zego.zegoliveroom.ZegoLiveRoom;
import com.zego.zegoliveroom.callback.IZegoAVEngineCallback;
import com.zego.zegoliveroom.callback.IZegoCustomCommandCallback;
import com.zego.zegoliveroom.callback.IZegoDeviceEventCallback;
import com.zego.zegoliveroom.callback.IZegoEndJoinLiveCallback;
import com.zego.zegoliveroom.callback.IZegoInitSDKCompletionCallback;
import com.zego.zegoliveroom.callback.IZegoLiveEventCallback;
import com.zego.zegoliveroom.callback.IZegoLivePublisherCallback2;
import com.zego.zegoliveroom.callback.IZegoLivePublisherExCallback;
import com.zego.zegoliveroom.callback.IZegoSnapshotCompletionCallback;
import com.zego.zegoliveroom.callback.IZegoUpdatePublishTargetCallback;
import com.zego.zegoliveroom.callback.IZegoLivePlayerCallback2;
import com.zego.zegoliveroom.callback.IZegoLivePublisherCallback;
import com.zego.zegoliveroom.callback.IZegoLoginCompletionCallback;
import com.zego.zegoliveroom.callback.IZegoResponseCallback;
import com.zego.zegoliveroom.callback.IZegoRoomCallback;
import com.zego.zegoliveroom.callback.im.IZegoBigRoomMessageCallback;
import com.zego.zegoliveroom.callback.im.IZegoIMCallback;
import com.zego.zegoliveroom.callback.im.IZegoRoomMessageCallback;
import com.zego.zegoliveroom.constants.ZegoAvConfig;
import com.zego.zegoliveroom.constants.ZegoConstants;
import com.zego.zegoliveroom.constants.ZegoIM;
import com.zego.zegoliveroom.entity.ZegoBigRoomMessage;
import com.zego.zegoliveroom.entity.ZegoPlayStats;
import com.zego.zegoliveroom.entity.ZegoRoomInfo;
import com.zego.zegoliveroom.entity.ZegoRoomMessage;
import com.zego.zegoliveroom.entity.ZegoStreamInfo;
import com.zego.zegoliveroom.entity.ZegoPublishStreamQuality;
import com.zego.zegoliveroom.entity.ZegoPlayStreamQuality;
import com.zego.zegoliveroom.entity.ZegoUser;
import com.zego.zegoliveroom.entity.ZegoUserState;
import com.zego.zegoliveroomplugin.constants.ZegoEventType;
import com.zego.zegoliveroomplugin.module.audioplayer.IZegoAudioPlayerControllerCallback;
import com.zego.zegoliveroomplugin.module.audioplayer.ZegoAudioPlayerController;
import com.zego.zegoliveroomplugin.module.mediaplayer.IZegoMediaPlayerControllerCallback;
import com.zego.zegoliveroomplugin.module.mediaplayer.ZegoMediaPlayerController;

/**
 * ZegoLiveRoomPlugin
 */
public class ZegoLiveRoomPlugin implements MethodCallHandler, EventChannel.StreamHandler, IZegoAudioPlayerControllerCallback, IZegoMediaPlayerControllerCallback {

    /**
     * Plugin registration.
     */

    private static String mPublishMainChl = com.zego.zegoavkit2.ZegoConstants.ZegoVideoDataMainPublishingStream;

    private static ZegoVideoFilterFactory videoFilterFactory;
    private static ZegoVideoCaptureFactory videoCaptureFactory;
    private final Registrar registrar;
    private final TextureRegistry textures;
    private final Context mContext;
    private EventSink mEventSink;

    private ZegoLiveRoom mZegoLiveRoom;
    private ZegoMediaSideInfo mZegoMediaSideInfo;
    private HashMap<String, ZegoViewRenderer> mRenders;
    private boolean mIsEnablePlatformView;

    private int mLogSize = 5 * 1024 * 1024;
    private String mLogPath = null;
//    private ZegoViewRenderer mMediaPlayerRenderer = null;

    private List<ZegoMediaPlayerController> mMediaPlayerControllers = new ArrayList<>();


    private ZegoLiveRoomPlugin(Registrar registrar) {
        this.registrar = registrar;
        this.textures = registrar.textures();
        this.mContext = registrar.context();    //获取应用程序的Context

        this.mZegoLiveRoom = null;
        this.mZegoMediaSideInfo = null;
        this.mIsEnablePlatformView = false;

        this.mRenders = new HashMap<>();
    }

    /**
     * 预存外部视频滤镜工厂
     * <p>
     * 首先实现一个 ZegoVideoFilterFactory ，调用此接口将工厂预存，
     * 然后在 dart 层调用 `enableExternalVideoFilterFactory` 接口，
     * 从而将此预存的工厂设置给 Native ZegoSDK
     *
     * @param factory 外部视频滤镜工厂
     */
    public static void setExternalVideoFilterFactory(ZegoVideoFilterFactory factory) {
        videoFilterFactory = factory;
    }

    /**
     * 预存外部视频采集工厂
     * <p>
     * 首先实现一个 ZegoVideoCaptureFactory ，调用此接口将工厂预存，
     * 然后在 dart 层调用 `enableExternalVideoCaptureFactory` 接口，
     * 从而将此预存的工厂设置给 Native ZegoSDK
     *
     * @param factory 外部视频采集工厂
     */
    public static void setExternalVideoCaptureFactory(ZegoVideoCaptureFactory factory) {
        videoCaptureFactory = factory;
    }

    public static void registerWith(Registrar registrar) {

        ZegoLiveRoomPlugin instance = new ZegoLiveRoomPlugin(registrar);
        final MethodChannel channel = new MethodChannel(registrar.messenger(), "plugins.zego.im/zegoliveroom_plugin");
        channel.setMethodCallHandler(instance);

        final EventChannel eventChannel = new EventChannel(registrar.messenger(), "plugins.zego.im/zegoliveroom_callback");
        eventChannel.setStreamHandler(instance);

        registrar.platformViewRegistry().registerViewFactory("plugins.zego.im/zego_view", ZegoPlatformViewFactory.shareInstance());
    }

    @RequiresApi(api = Build.VERSION_CODES.N)
    @Override
    public void onMethodCall(MethodCall call, final Result result) {

        /* LiveRoom */
        if (call.method.equals("getSdkVersion")) {

            result.success(ZegoLiveRoom.version());

        } else if (call.method.equals("getEngineVersion")) {

            result.success(ZegoLiveRoom.version2());

        } else if (call.method.equals("setVerbose")) {

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            ZegoLiveRoom.setVerbose(enable);
            result.success(null);

        } else if (call.method.equals("uploadLog")) {

            ZegoLiveRoom.uploadLog();
            result.success(null);

        } else if (call.method.equals("getLogDir")) {

            String path = com.zego.zegoavkit2.utils.ZegoLogUtil.getLogPath(mContext);
            result.success(path);

        } else if (call.method.equals("setLogConfig")) {

            mLogPath = call.argument("logPath");
            mLogSize = numberToIntValue((Number) call.argument("logSize"));
            ZegoLogJNI.logNotice("[Flutter-Native] setLogConfig, logSize: " + mLogSize + " logPath: " + mLogPath);
            result.success(null);

        } else if (call.method.equals("initSDK")) {

            long appID = numberToLongValue((Number) call.argument("appID"));
            String strAppSign = call.argument("appSign");
            int playerCount = numberToIntValue((Number) call.argument("playerCount"));

            initSDKWithAppID(appID, strAppSign, result);

        } else if (call.method.equals("uninitSDK")) {

            if (mZegoLiveRoom == null) {

                result.success(false);

            } else {

                if (mZegoMediaSideInfo != null) {
                    mZegoMediaSideInfo.setZegoMediaSideCallback(null);
                    mZegoMediaSideInfo = null;
                }

                ZegoAudioPlayerController.getInstance().uninit();

                //
                if (mMediaPlayerControllers.size()>0){
                    for (ZegoMediaPlayerController mediaPlayerController : mMediaPlayerControllers) {
                        mediaPlayerController.uninit();
                    }
                    mMediaPlayerControllers.clear();
                }

                ZegoLogJNI.logNotice("[Flutter-Native] unInitSDK");
                //反初始化SDK里面会做回调的销毁处理
                result.success(mZegoLiveRoom.unInitSDK());
            }

        } else if (call.method.equals("enablePlatformView")) {

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            mIsEnablePlatformView = enable;
            result.success(null);

        } else if (call.method.equals("setUser")) {

            String userID = call.argument("userID");
            String userName = call.argument("userName");

            boolean success = ZegoLiveRoom.setUser(userID, userName);
            result.success(success);

        } else if (call.method.equals("setUseTestEnv")) {

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            ZegoLiveRoom.setTestEnv(enable);
            result.success(null);

        } else if (call.method.equals("loginRoom")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String roomID = call.argument("roomID");
            String roomName = call.argument("roomName");
            int role = numberToIntValue((Number) call.argument("role"));

            ZegoLogJNI.logNotice("[Flutter-Native] loginRoom enter, roomID: " + roomID + " roomName: " + roomName + " role: " + role);
            boolean success = mZegoLiveRoom.loginRoom(roomID, roomName, role, new IZegoLoginCompletionCallback() {
                @Override
                public void onLoginCompletion(int errorCode, ZegoStreamInfo[] zegoStreamInfos) {

                    ArrayList<HashMap<String, Object>> streamList = new ArrayList<>();
                    for (ZegoStreamInfo streamInfo : zegoStreamInfos) {
                        HashMap<String, Object> map = new HashMap<>();
                        map.put("userID", streamInfo.userID);
                        map.put("userName", streamInfo.userName);
                        map.put("streamID", streamInfo.streamID);
                        map.put("extraInfo", streamInfo.extraInfo);
                        streamList.add(map);
                    }

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("errorCode", errorCode);
                    returnMap.put("streamList", streamList);
                    ZegoLogJNI.logNotice("[Flutter-Native] onLoginRoom, return map: " + returnMap);
                    result.success(returnMap);
                }
            });

            if (!success) {
                result.error(String.format("%s_ERROR", call.method).toUpperCase(), "call \'loginRoom\' failed, please check you call \'setUser\' first", null);
            }


        } else if (call.method.equals("switchRoom")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String roomID = call.argument("roomID");
            String roomName = call.argument("roomName");
            int role = numberToIntValue((Number) call.argument("role"));

            ZegoLogJNI.logNotice("[Flutter-Native] switchRoom enter, roomID: " + roomID + " roomName: " + roomName + " role: " + role);
            boolean success = mZegoLiveRoom.switchRoom(roomID, roomName, role, new IZegoLoginCompletionCallback() {
                @Override
                public void onLoginCompletion(int errorCode, ZegoStreamInfo[] zegoStreamInfos) {

                    ArrayList<HashMap<String, Object>> streamList = new ArrayList<>();
                    for (ZegoStreamInfo streamInfo : zegoStreamInfos) {
                        HashMap<String, Object> map = new HashMap<>();
                        map.put("userID", streamInfo.userID);
                        map.put("userName", streamInfo.userName);
                        map.put("streamID", streamInfo.streamID);
                        map.put("extraInfo", streamInfo.extraInfo);
                        streamList.add(map);
                    }

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("errorCode", errorCode);
                    returnMap.put("streamList", streamList);
                    ZegoLogJNI.logNotice("[Flutter-Native] onSwitchRoom, return map: " + returnMap);
                    result.success(returnMap);
                }
            });

            if (!success) {
                result.error(String.format("%s_ERROR", call.method).toUpperCase(), "call \'switchRoom\' failed.", null);
            }

        } else if (call.method.equals("logoutRoom")) {


            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean success = mZegoLiveRoom.logoutRoom();
            result.success(success);

        } else if (call.method.equals("pauseModule")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int type = numberToIntValue((Number) call.argument("type"));
            mZegoLiveRoom.pauseModule(type);
            result.success(null);

        } else if (call.method.equals("resumeModule")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int type = numberToIntValue((Number) call.argument("type"));
            mZegoLiveRoom.resumeModule(type);
            result.success(null);

        } else if (call.method.equals("enableMicDevice")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            boolean success = mZegoLiveRoom.enableMicDevice(enable);
            result.success(success);

        } else if (call.method.equals("sendRoomMessage")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String content = call.argument("content");

            mZegoLiveRoom.sendRoomMessage(ZegoIM.MessageType.Text, ZegoIM.MessageCategory.Chat, content, new IZegoRoomMessageCallback() {
                @Override
                public void onSendRoomMessage(int errorCode, java.lang.String roomID, long messageID) {
                    HashMap<String, Object> mapResult = new HashMap<>();
                    mapResult.put("errorCode", errorCode);
                    mapResult.put("roomID", roomID);
                    mapResult.put("messageID", messageID);

                    result.success(mapResult);
                }
            });

        } else if (call.method.equals("sendBigRoomMessage")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String content = call.argument("content");

            mZegoLiveRoom.sendBigRoomMessage(ZegoIM.MessageType.Text, ZegoIM.MessageCategory.Chat, content, new IZegoBigRoomMessageCallback() {
                @Override
                public void onSendBigRoomMessage(int errorCode, java.lang.String roomID, String messageID) {
                    HashMap<String, Object> mapResult = new HashMap<>();
                    mapResult.put("errorCode", errorCode);
                    mapResult.put("roomID", roomID);
                    mapResult.put("messageID", messageID);

                    result.success(mapResult);
                }
            });

        } else if (call.method.equals("sendCustomCommand")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ArrayList<HashMap<String, String>> userList = call.argument("users");
            String content = call.argument("content");

            ArrayList<ZegoUser> zegoUserList = new ArrayList<>();
            for (HashMap<String, String> objUser : userList) {
                ZegoUser user = new ZegoUser();
                user.userID = objUser.get("userID");
                user.userName = objUser.get("userName");
                zegoUserList.add(user);
            }

            ZegoUser[] users = zegoUserList.toArray(new ZegoUser[zegoUserList.size()]);
            mZegoLiveRoom.sendCustomCommand(users, content, new IZegoCustomCommandCallback() {
                @Override
                public void onSendCustomCommand(int errorCode, String roomID) {
                    HashMap<String, Object> mapResult = new HashMap<>();
                    mapResult.put("errorCode", errorCode);
                    mapResult.put("roomID", roomID);

                    result.success(mapResult);
                }
            });

        } else if (call.method.equals("setRoomConfig")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean audienceCreate = numberToBoolValue((Boolean) call.argument("audienceCreateRoom"));
            boolean userStateUpdate = numberToBoolValue((Boolean) call.argument("userStateUpdate"));

            mZegoLiveRoom.setRoomConfig(audienceCreate, userStateUpdate);
            result.success(null);
        }
        /* LiveRoom-Publisher */
        else if (call.method.equals("setLatencyMode")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int mode = numberToIntValue((Number) call.argument("mode"));
            mZegoLiveRoom.setLatencyMode(mode);
            result.success(null);

        } else if (call.method.equals("setVideoMirrorMode")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int mode = numberToIntValue((Number) call.argument("mode"));
            boolean success = mZegoLiveRoom.setVideoMirrorMode(mode, 0);
            result.success(success);

        } else if (call.method.equals("setAudioDeviceMode")) {

            int mode = numberToIntValue((Number) call.argument("mode"));
            ZegoLiveRoom.setAudioDeviceMode(mode);
            result.success(null);

        } else if (call.method.equals("startPreview")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean success = mZegoLiveRoom.startPreview();
            result.success(success);

        } else if (call.method.equals("stopPreview")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean success = mZegoLiveRoom.stopPreview();
            result.success(success);

        } else if (call.method.equals("startPublishing")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String streamID = call.argument("streamID");
            String title = call.argument("title");
            int flag = numberToIntValue((Number) call.argument("flag"));
            String extraInfo = call.argument("extraInfo");

            boolean success;
            if (extraInfo != null) {

                success = mZegoLiveRoom.startPublishing(streamID, title, flag, extraInfo);

            } else {

                success = mZegoLiveRoom.startPublishing(streamID, title, flag);
            }

            result.success(success);


        } else if (call.method.equals("stopPublishing")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean success = mZegoLiveRoom.stopPublishing();
            result.success(success);

        } else if (call.method.equals("setVideoCodecId")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int codecID = numberToIntValue((Number) call.argument("codecID"));
            boolean success = mZegoLiveRoom.setVideoCodecId(codecID, 0);
            result.success(success);

        } else if (call.method.equals("updateStreamExtraInfo")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String extraInfo = call.argument("extraInfo");
            boolean success = mZegoLiveRoom.updateStreamExtraInfo(extraInfo);
            result.success(success);

        } else if (call.method.equals("enableMic")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            boolean success = mZegoLiveRoom.enableMic(enable);
            result.success(success);

        } else if (call.method.equals("enableCamera")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            boolean success = mZegoLiveRoom.enableCamera(enable);
            result.success(success);

        } else if (call.method.equals("setFrontCam")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            boolean success = mZegoLiveRoom.setFrontCam(enable);
            result.success(success);

        } else if (call.method.equals("setAVConfig")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int captureWidth = numberToIntValue((Number) call.argument("captureWidth"));
            int captureHeight = numberToIntValue((Number) call.argument("captureHeight"));
            int encodeWidth = numberToIntValue((Number) call.argument("encodeWidth"));
            int encodeHeight = numberToIntValue((Number) call.argument("encodeHeight"));
            int bitrate = numberToIntValue((Number) call.argument("bitrate"));
            int fps = numberToIntValue((Number) call.argument("fps"));

            ZegoAvConfig avConfig = new ZegoAvConfig(ZegoAvConfig.Level.Generic);
            avConfig.setVideoCaptureResolution(captureWidth, captureHeight);
            avConfig.setVideoEncodeResolution(encodeWidth, encodeHeight);
            avConfig.setVideoBitrate(bitrate);
            avConfig.setVideoFPS(fps);

            boolean success = mZegoLiveRoom.setAVConfig(avConfig);
            result.success(success);

        } else if (call.method.equals("requireHardwareEncoder")) {

            boolean require = numberToBoolValue((Boolean) call.argument("bRequire"));
            boolean success = ZegoLiveRoom.requireHardwareEncoder(require);
            result.success(success);

        } else if (call.method.equals("isVideoEncoderSupported")) {

            int codecID = numberToIntValue((Number) call.argument("codecID"));
            boolean success = mZegoLiveRoom.isVideoEncoderSupported(codecID);
            result.success(success);

        } else if (call.method.equals("isVideoDecoderSupported")) {

            int codecID = numberToIntValue((Number) call.argument("codecID"));
            boolean success = mZegoLiveRoom.isVideoDecoderSupported(codecID);
            result.success(success);

        } else if (call.method.equals("enableH265EncodeFallback")) {

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            boolean success = mZegoLiveRoom.enableH265EncodeFallback(enable);
            result.success(success);

        } else if (call.method.equals("enableBeautifying")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int feature = numberToIntValue((Number) call.argument("feature"));

            boolean success = mZegoLiveRoom.enableBeautifying(feature);
            result.success(success);

        } else if (call.method.equals("setPolishStep")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            float step = numberToFloatValue((Number) call.argument("step"));
            boolean success = mZegoLiveRoom.setPolishStep(step);
            result.success(success);

        } else if (call.method.equals("setPolishFactor")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            float factor = numberToFloatValue((Number) call.argument("factor"));
            boolean success = mZegoLiveRoom.setPolishFactor(factor, 0);
            result.success(success);

        } else if (call.method.equals("setWhitenFactor")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            float factor = numberToFloatValue((Number) call.argument("factor"));
            boolean success = mZegoLiveRoom.setWhitenFactor(factor);
            result.success(success);

        } else if (call.method.equals("setSharpenFactor")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            float factor = numberToFloatValue((Number) call.argument("factor"));
            boolean success = mZegoLiveRoom.setSharpenFactor(factor);
            result.success(success);

        } else if (call.method.equals("setFilter")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int filter = numberToIntValue((Number) call.argument("filter"));
            boolean success = mZegoLiveRoom.setFilter(filter);
            result.success(success);

        } else if (call.method.equals("enableTrafficControl")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            int properties = numberToIntValue((Number) call.argument("properties"));

            mZegoLiveRoom.enableTrafficControl(properties, enable);
            result.success(null);

        } else if (call.method.equals("setMinVideoBitrateForTrafficControl")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int bitrate = numberToIntValue((Number) call.argument("bitrate"));
            int mode = numberToIntValue((Number) call.argument("mode"));

            mZegoLiveRoom.setMinVideoBitrateForTrafficControl(bitrate, mode);
            result.success(null);

        } else if (call.method.equals("createPreviewRenderer")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }

            int width = numberToIntValue((Number) call.argument("width"));
            int height = numberToIntValue((Number) call.argument("height"));


            if (mRenders.containsKey(mPublishMainChl)) {
                result.success(-1);
                return;
            }

            ZegoViewRenderer renderer = new ZegoViewRenderer(textures.createSurfaceTexture(), width, height);
            ZegoLogJNI.logNotice("[createPreviewRenderer] view size: " + "(" + width + ", " + height + ")" + "textureID:" + renderer.getTextureID());
            mRenders.put(mPublishMainChl, renderer);
            mZegoLiveRoom.setPreviewView(renderer.getSurface());
            result.success(renderer.getTextureID());

        } else if (call.method.equals("setPreviewViewMode")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }

            if (!mRenders.containsKey(mPublishMainChl)) {
                throwNoRendererError(result, call.method);
                return;
            }

            int mode = numberToIntValue((Number) call.argument("mode"));
            mZegoLiveRoom.setPreviewViewMode(mode);
            result.success(true);

        } else if (call.method.equals("updatePreviewRenderSize")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }

            int width = numberToIntValue((Number) call.argument("width"));
            int height = numberToIntValue((Number) call.argument("height"));

            ZegoViewRenderer renderer = mRenders.get(mPublishMainChl);
            if (renderer == null) {
                throwNoRendererError(result, call.method);
                return;
            }

            mZegoLiveRoom.setPreviewView(null);
            boolean success = renderer.updateRenderSize(width, height);
            //if(success) {
            mZegoLiveRoom.setPreviewView(renderer.getSurface());
            //}
            ZegoLogJNI.logNotice("[updatePreviewRenderSize] width: " + width + " height: " + height + ", textureID: " + renderer.getTextureID());

            result.success(success);

        } else if (call.method.equals("destroyPreviewRenderer")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }

            if (!mRenders.containsKey(mPublishMainChl)) {
                result.success(false);
                return;
            }

            ZegoViewRenderer renderer = mRenders.get(mPublishMainChl);
            if (renderer != null) {
                ZegoLogJNI.logNotice("[destroyPreviewRenderer] textrueID: " + renderer.getTextureID());
                mZegoLiveRoom.setPreviewView(null);
                renderer.release();
                result.success(true);

            } else {

                result.success(false);
            }

            mRenders.remove(mPublishMainChl);

        } else if (call.method.equals("setPreviewView")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (!mIsEnablePlatformView) {
                throwNoPlatformViewError(result, call.method);
                return;
            }

            int viewID = numberToIntValue((Number) call.argument("viewID"));
            ZegoPlatformView view = ZegoPlatformViewFactory.shareInstance().getPlatformView(viewID);
            if (view != null) {
                int width = view.getSurfaceView().getHolder().getSurfaceFrame().width();
                int height = view.getSurfaceView().getHolder().getSurfaceFrame().height();
                if (width == 0 && height == 0) {
                    reportInnerError("[setPreviewView UnexpectedException] view size is zero");
                }
                ZegoLogJNI.logNotice("[SetPreviewView - ZegoPlatformView] view size: " + "(" + width + ", " + height + ")" + "viewID:" + viewID);
            } else {
                ZegoLogJNI.logNotice("[SetPreviewView - ZegoPlatformView] no such view");
            }
            if (view == null) {
                result.success(false);
                return;
            }

            boolean success = mZegoLiveRoom.setPreviewView(view.getSurfaceView());
            result.success(success);

        } else if (call.method.equals("setPlatformViewPreviewViewMode")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (!mIsEnablePlatformView) {
                throwNoPlatformViewError(result, call.method);
                return;
            }

            int mode = numberToIntValue((Number) call.argument("mode"));
            boolean success = mZegoLiveRoom.setPreviewViewMode(mode);

            result.success(success);

        } else if (call.method.equals("removePreviewPlatformView")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (!mIsEnablePlatformView) {
                throwNoPlatformViewError(result, call.method);
                return;
            }

            int viewID = numberToIntValue((Number) call.argument("viewID"));
            boolean success = ZegoPlatformViewFactory.shareInstance().removeView(viewID);

            result.success(success);

        } else if (call.method.equals("setAppOrientation")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int orientation = numberToIntValue((Number) call.argument("orientation"));

            boolean success = mZegoLiveRoom.setAppOrientation(orientation);
            result.success(success);

        } else if (call.method.equals("setPublishConfig")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            HashMap<String, Object> config = new HashMap<>();
            config.put(ZegoConstants.PublishConfig.PUBLISH_CUSTOM_TARGET, call.argument("rtmpURL"));
            mZegoLiveRoom.setPublishConfig(config);

            result.success(true);

        } else if (call.method.equals("addPublishTarget")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String target = call.argument("target");
            String streamID = call.argument("streamID");

            mZegoLiveRoom.addPublishTarget(target, streamID, new IZegoUpdatePublishTargetCallback() {
                @Override
                public void onUpdatePublishTargetState(int errorCode, String streamID) {
                    HashMap<String, Object> mapResult = new HashMap<>();
                    mapResult.put("errorCode", errorCode);
                    mapResult.put("streamID", streamID);

                    result.success(mapResult);
                }
            });

        } else if (call.method.equals("deletePublishTarget")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String target = call.argument("target");
            String streamID = call.argument("streamID");

            mZegoLiveRoom.deletePublishTarget(target, streamID, new IZegoUpdatePublishTargetCallback() {
                @Override
                public void onUpdatePublishTargetState(int errorCode, String streamID) {
                    HashMap<String, Object> mapResult = new HashMap<>();
                    mapResult.put("errorCode", errorCode);
                    mapResult.put("streamID", streamID);

                    result.success(mapResult);
                }
            });

        } else if (call.method.equals("respondJoinLiveReq")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int seq = numberToIntValue((Number) call.argument("seq"));
            int rspResult = numberToIntValue((Number) call.argument("rspResult"));

            boolean success = mZegoLiveRoom.respondJoinLiveReq(seq, rspResult);
            result.success(success);

        } else if (call.method.equals("inviteJoinLive")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String userID = call.argument("userID");

            mZegoLiveRoom.inviteJoinLive(userID, new IZegoResponseCallback() {
                @Override
                public void onResponse(int ret, String fromUserID, String fromUserName) {
                    HashMap<String, Object> mapResult = new HashMap<>();
                    mapResult.put("result", ret);
                    mapResult.put("fromUserID", fromUserID);
                    mapResult.put("fromUserName", fromUserName);

                    result.success(mapResult);
                }
            });

        } else if (call.method.equals("endJoinLive")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String userID = call.argument("userID");

            mZegoLiveRoom.endJoinLive(userID, new IZegoEndJoinLiveCallback() {
                @Override
                public void onEndJoinLive(int errorCode, String roomID) {
                    HashMap<String, Object> mapResult = new HashMap<>();
                    mapResult.put("errorCode", errorCode);
                    mapResult.put("roomID", roomID);

                    result.success(mapResult);
                }
            });

        } else if (call.method.equals("setAudioBitrate")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int bitrate = numberToIntValue((Number) call.argument("bitrate"));

            boolean success = mZegoLiveRoom.setAudioBitrate(bitrate);
            result.success(success);

        } else if (call.method.equals("setAudioChannelCount")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int channels = numberToIntValue((Number) call.argument("channels"));
            mZegoLiveRoom.setAudioChannelCount(channels);
            result.success(true);

        } else if (call.method.equals("enableAEC")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            mZegoLiveRoom.enableAEC(enable);

            result.success(true);

        } else if (call.method.equals("setAECMode")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int mode = numberToIntValue((Number) call.argument("mode"));
            mZegoLiveRoom.setAECMode(mode);
            result.success(null);

        } else if (call.method.equals("enableAGC")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            mZegoLiveRoom.enableAGC(enable);

            result.success(true);

        } else if (call.method.equals("enableVAD")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            mZegoLiveRoom.enableVAD(enable);
            result.success(null);

        } else if (call.method.equals("enableANS")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            boolean success = mZegoLiveRoom.enableNoiseSuppress(enable);

            result.success(success);

        } else if (call.method.equals("enableDTX")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            mZegoLiveRoom.enableDTX(enable);
            result.success(null);

        } else if (call.method.equals("setCamZoomFactor")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            float factor = numberToFloatValue((Number) call.argument("factor"));

            boolean success = ZegoCamera.setCamZoomFactor(factor, PublishChannelIndex.MAIN);

            result.success(success);

        } else if (call.method.equals("getCamMaxZoomFactor")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            float factor = ZegoCamera.getCamMaxZoomFactor(PublishChannelIndex.MAIN);

            result.success(factor);

        } else if (call.method.equals("takePublishStreamSnapshot")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            mZegoLiveRoom.takePreviewSnapshot(new IZegoSnapshotCompletionCallback() {
                @Override
                public void onZegoSnapshotCompletion(Bitmap bitmap) {
                    if (bitmap == null) {
                        result.success(null);
                        return;
                    }

                    ByteArrayOutputStream stream = new ByteArrayOutputStream();
                    bitmap.compress(Bitmap.CompressFormat.PNG, 100, stream);
                    byte[] byteArray = stream.toByteArray();
                    bitmap.recycle();

                    if (byteArray == null || byteArray.length == 0) {
                        result.success(null);
                        return;
                    }

                    result.success(byteArray);
                }
            });

        } else if (call.method.equals("setDummyCaptureImagePath")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoLiveRoom.setDummyCaptureImagePath((String) call.argument("imagePath"), PublishChannelIndex.MAIN);

            result.success(true);
        }
        /* LiveRoom-Player */
        else if (call.method.equals("setViewMode")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }

            String streamID = call.argument("streamID");
            int mode = numberToIntValue((Number) call.argument("mode"));

            boolean success = mZegoLiveRoom.setViewMode(mode, streamID);
            result.success(success);

        } else if (call.method.equals("startPlayingStream")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String streamID = call.argument("streamID");
            HashMap<String, Object> info = call.argument("info");

            Surface surface = null;
            ZegoPlatformView view = null;

            if (mIsEnablePlatformView) {

                // 只有音视频场景下严格检查viewID的合法性，纯音频场景下允许不填viewID（viewID为空）
                if (call.argument("viewID") != null) {
                    int viewID = numberToIntValue((Number) call.argument("viewID"));
                    view = ZegoPlatformViewFactory.shareInstance().getPlatformView(viewID);
                    if (view != null) {
                        int width = view.getSurfaceView().getHolder().getSurfaceFrame().width();
                        int height = view.getSurfaceView().getHolder().getSurfaceFrame().height();
                        if (width == 0 && height == 0) {
                            reportInnerError("[startPlayingStream UnexpectedException] view size is zero");
                        }
                        ZegoLogJNI.logNotice("[StartPlayingStream - ZegoPlatformView] view size: " + "(" + width + ", " + height + ")" + "viewID: " + viewID);
                    } else {
                        ZegoLogJNI.logNotice("[StartPlayingStream - ZegoPlatformView] no such view");
                    }
                    //传入错误的view id
                    if (view == null) {
                        result.success(false);
                        return;
                    }
                }

            } else {

                if (mRenders.containsKey(streamID)) {
                    ZegoViewRenderer renderer = mRenders.get(streamID);
                    surface = renderer.getSurface();
                }//else {

                // 这个位置不能return false，对于纯语音场景会走到这个分支中，不能被过滤掉
                // result.success(false);
                //return;
                //}
            }

            boolean success;


            if (info != null) {

                String params = (String) info.get("params");

                @SuppressWarnings("unchecked")
                ArrayList<String> rtmpUrls = (ArrayList<String>) info.get("rtmpUrls");
                @SuppressWarnings("unchecked")
                ArrayList<String> flvUrls = (ArrayList<String>) info.get("flvUrls");
                ZegoStreamExtraPlayInfo extraPlayInfo = new ZegoStreamExtraPlayInfo();
                extraPlayInfo.params = params;
                extraPlayInfo.rtmpUrls = (String[]) rtmpUrls.toArray(new String[0]);
                extraPlayInfo.flvUrls = (String[]) flvUrls.toArray(new String[0]);

                success = mZegoLiveRoom.startPlayingStream(streamID, mIsEnablePlatformView ? view != null ? view.getSurfaceView() : null : surface, extraPlayInfo);
            } else {

                success = mZegoLiveRoom.startPlayingStream(streamID, mIsEnablePlatformView ? view != null ? view.getSurfaceView() : null : surface);
            }

            result.success(success);


        } else if (call.method.equals("stopPlayingStream")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String streamID = call.argument("streamID");

            boolean success = mZegoLiveRoom.stopPlayingStream(streamID);
            result.success(success);

        } else if (call.method.equals("activateAudioPlayStream")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String streamID = call.argument("streamID");
            boolean active = numberToBoolValue((Boolean) call.argument("active"));

            int errorCode = mZegoLiveRoom.activateAudioPlayStream(streamID, active);
            result.success(errorCode);

        } else if (call.method.equals("activateVideoPlayStream")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String streamID = call.argument("streamID");
            boolean active = numberToBoolValue((Boolean) call.argument("active"));

            int errorCode;
            Number objVideoStreamLayer = call.argument("layer");
            if (objVideoStreamLayer != null) {

                int videoStreamLayer = numberToIntValue(objVideoStreamLayer);
                errorCode = mZegoLiveRoom.activateVideoPlayStream(streamID, active, videoStreamLayer);
            } else {

                errorCode = mZegoLiveRoom.activateVideoPlayStream(streamID, active);
            }

            result.success(errorCode);

        } else if (call.method.equals("requireHardwareDecoder")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean require = numberToBoolValue((Boolean) call.argument("bRequire"));
            boolean success = ZegoLiveRoom.requireHardwareDecoder(require);
            result.success(success);

        } else if (call.method.equals("createPlayViewRenderer")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }

            String streamID = call.argument("streamID");
            int width = numberToIntValue((Number) call.argument("width"));
            int height = numberToIntValue((Number) call.argument("height"));

            if (mRenders.containsKey(streamID)) {
                result.success(-1);
                return;
            }

            ZegoViewRenderer renderer = new ZegoViewRenderer(textures.createSurfaceTexture(), width, height);
            mRenders.put(streamID, renderer);
            ZegoLogJNI.logNotice("[createPlayRenderer] view size: " + "(" + width + ", " + height + ")" + " textureID:" + renderer.getTextureID() + " streamID: " + streamID);
            result.success(renderer.getTextureID());

        } else if (call.method.equals("updatePlayViewRenderSize")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }

            String streamID = call.argument("streamID");
            int width = numberToIntValue((Number) call.argument("width"));
            int height = numberToIntValue((Number) call.argument("height"));

            if (!mRenders.containsKey(streamID)) {
                result.success(false);
                return;
            }


            ZegoViewRenderer renderer = mRenders.get(streamID);
            if (renderer == null) {
                throwNoRendererError(result, call.method);
                return;
            }

            mZegoLiveRoom.updatePlayView(streamID, null);
            boolean success = renderer.updateRenderSize(width, height);
            //if (success) {
            mZegoLiveRoom.updatePlayView(streamID, renderer.getSurface());
            //}
            ZegoLogJNI.logNotice("[updatePlayRenderSize] view size: " + "(" + width + ", " + height + ")" + " textureID:" + renderer.getTextureID() + " streamID: " + streamID);
            result.success(success);

        } else if (call.method.equals("destroyPlayViewRenderer")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }

            String streamID = call.argument("streamID");

            if (!mRenders.containsKey(streamID)) {
                result.success(false);
                return;
            }

            ZegoViewRenderer renderer = mRenders.get(streamID);

            if (renderer != null) {
                ZegoLogJNI.logNotice("[updatePlayRenderSize] textureID:" + renderer.getTextureID() + " streamID: " + streamID);
                //先停止渲染
                mZegoLiveRoom.updatePlayView(streamID, null);

                renderer.release();
                result.success(true);
            } else {

                result.success(false);
            }

            mRenders.remove(streamID);

        } else if (call.method.equals("updatePlayView")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (!mIsEnablePlatformView) {
                throwNoPlatformViewError(result, call.method);
                return;
            }

            int viewID = numberToIntValue((Number) call.argument("viewID"));
            String streamID = call.argument("streamID");

            ZegoPlatformView view = ZegoPlatformViewFactory.shareInstance().getPlatformView(viewID);
            if (view == null) {
                ZegoLogJNI.logNotice("[UpdatePlayView - ZegoPlatformView] no such view");
                result.success(false);
                return;
            }

            mZegoLiveRoom.updatePlayView(streamID, view.getSurfaceView());
            result.success(null);

        } else if (call.method.equals("setPlatformViewPlayViewMode")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (!mIsEnablePlatformView) {
                throwNoPlatformViewError(result, call.method);
                return;
            }

            String streamID = call.argument("streamID");
            int mode = numberToIntValue((Number) call.argument("mode"));

            boolean success = mZegoLiveRoom.setViewMode(mode, streamID);
            result.success(success);


        } else if (call.method.equals("removePlayPlatformView")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (!mIsEnablePlatformView) {
                throwNoPlatformViewError(result, call.method);
                return;
            }

            int viewID = numberToIntValue((Number) call.argument("viewID"));
            boolean success = ZegoPlatformViewFactory.shareInstance().removeView(viewID);
            result.success(success);

        } else if (call.method.equals("requestJoinLive")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            mZegoLiveRoom.requestJoinLive(new IZegoResponseCallback() {
                @Override
                public void onResponse(int ret, String fromUserID, String fromUserName) {
                    HashMap<String, Object> mapResult = new HashMap<>();
                    mapResult.put("result", ret);
                    mapResult.put("fromUserID", fromUserID);
                    mapResult.put("fromUserName", fromUserName);

                    result.success(mapResult);
                }
            });

        } else if (call.method.equals("respondInviteJoinLiveReq")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int seq = numberToIntValue((Number) call.argument("seq"));
            int rspResult = numberToIntValue((Number) call.argument("rspResult"));

            boolean success = mZegoLiveRoom.respondInviteJoinLiveReq(seq, rspResult);
            result.success(success);

        } else if (call.method.equals("setPlayVolume")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int volume = numberToIntValue((Number) call.argument("volume"));
            String streamID = call.argument("streamID");

            boolean success = mZegoLiveRoom.setPlayVolume(volume, streamID);
            result.success(success);

        } else if (call.method.equals("enableSpeaker")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));

            boolean success = mZegoLiveRoom.enableSpeaker(enable);
            result.success(success);

        } else if (call.method.equals("setBuiltInSpeakerOn")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean on = numberToBoolValue((Boolean) call.argument("bOn"));

            boolean success = mZegoLiveRoom.setBuiltInSpeakerOn(on);
            result.success(success);

        } else if (call.method.equals("takePlayStreamSnapshot")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String streamID = call.argument("streamID");

            mZegoLiveRoom.takeSnapshotOfStream(streamID, new IZegoSnapshotCompletionCallback() {
                @Override
                public void onZegoSnapshotCompletion(Bitmap bitmap) {
                    if (bitmap == null) {
                        result.success(null);
                        return;
                    }

                    ByteArrayOutputStream stream = new ByteArrayOutputStream();
                    bitmap.compress(Bitmap.CompressFormat.PNG, 100, stream);
                    byte[] byteArray = stream.toByteArray();
                    bitmap.recycle();

                    if (byteArray == null || byteArray.length == 0) {
                        result.success(null);
                        return;
                    }

                    result.success(byteArray);
                }
            });
        }
        /* Media Side Info */
        else if (call.method.equals("setMediaSideFlags")) {

            if (mZegoLiveRoom == null || mZegoMediaSideInfo == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean start = numberToBoolValue((Boolean) call.argument("start"));
            boolean onlyAudioPublish = numberToBoolValue((Boolean) call.argument("onlyAudioPublish"));

            mZegoMediaSideInfo.setMediaSideFlags(start, onlyAudioPublish, 0);
            result.success(null);

        } else if (call.method.equals("sendMediaSideInfo")) {

            if (mZegoLiveRoom == null || mZegoMediaSideInfo == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String data = call.argument("data");

            ByteBuffer tmpData = ByteBuffer.wrap(data.getBytes());

            ByteBuffer inData = ByteBuffer.allocateDirect(tmpData.capacity());
            for (int i = 0; i < inData.capacity(); i++)
                inData.put(i, tmpData.get(i));

            mZegoMediaSideInfo.sendMediaSideInfo(inData, inData.capacity(), false, 0);
            result.success(null);

        }
        /* Sound Level */
        else if (call.method.equals("registerSoundLevelCallback")) {

            ZegoSoundLevelMonitor.getInstance().setCallback(new IZegoSoundLevelCallback() {
                @Override
                public void onSoundLevelUpdate(ZegoSoundLevelInfo[] zegoSoundLevelInfos) {
                    if (mEventSink != null) {

                        ArrayList<HashMap<String, Object>> soundLevelList = new ArrayList<>();
                        for (ZegoSoundLevelInfo info : zegoSoundLevelInfos) {
                            HashMap<String, Object> objInfo = new HashMap<>();
                            objInfo.put("streamID", info.streamID);
                            objInfo.put("soundLevel", info.soundLevel);

                            soundLevelList.add(objInfo);
                        }

                        HashMap<String, Object> returnMap = new HashMap<>();
                        returnMap.put("type", ZegoEventType.TYPE_SOUND_LEVEL_EVENT);

                        HashMap<String, Object> method = new HashMap<>();
                        method.put("name", "onSoundLevelUpdate");
                        method.put("soundLevels", soundLevelList);

                        returnMap.put("method", method);
                        mEventSink.success(returnMap);
                    }
                }

                @Override
                public void onCaptureSoundLevelUpdate(ZegoSoundLevelInfo zegoSoundLevelInfo) {
                    if (mEventSink != null) {

                        HashMap<String, Object> returnMap = new HashMap<>();
                        returnMap.put("type", ZegoEventType.TYPE_SOUND_LEVEL_EVENT);

                        HashMap<String, Object> method = new HashMap<>();
                        method.put("name", "onCaptureSoundLevelUpdate");
                        method.put("streamID", zegoSoundLevelInfo.streamID);
                        method.put("soundLevel", zegoSoundLevelInfo.soundLevel);

                        returnMap.put("method", method);
                        mEventSink.success(returnMap);
                    }
                }
            });

            result.success(true);

        } else if (call.method.equals("unregisterSoundLevelCallback")) {

            ZegoSoundLevelMonitor.getInstance().setCallback(null);
            result.success(true);

        } else if (call.method.equals("setSoundLevelMonitorCycle")) {

            int ms = numberToIntValue((Number) call.argument("ms"));
            boolean success = ZegoSoundLevelMonitor.getInstance().setCycle(ms);
            result.success(success);

        } else if (call.method.equals("startSoundLevelMonitor")) {

            boolean success = ZegoSoundLevelMonitor.getInstance().start();
            result.success(success);

        } else if (call.method.equals("stopSoundLevelMonitor")) {

            boolean success = ZegoSoundLevelMonitor.getInstance().stop();
            result.success(success);

        }
        /* LiveRoom-AudioPlayer */
        else if (call.method.equals("playAudioEffect")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().playAudioEffect(call, this.registrar, result);

        } else if (call.method.equals("stopAudioEffect")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().stopAudioEffect(call, result);

        } else if (call.method.equals("pauseAudioEffect")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().pauseAudioEffect(call, result);

        } else if (call.method.equals("resumeAudioEffect")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().resumeAudioEffect(call, result);

        } else if (call.method.equals("setAudioEffectVolume")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().setAudioEffectVolume(call, result);

        } else if (call.method.equals("preloadAudioEffect")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().preloadAudioEffect(call, this.registrar, result);

        } else if (call.method.equals("unloadAudioEffect")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().unloadAudioEffect(call, result);

        } else if (call.method.equals("setAllAudioEffectVolume")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().setAllEffectVolume(call, result);

        } else if (call.method.equals("pauseAllAudioEffect")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().pauseAllEffect(result);

        } else if (call.method.equals("resumeAllAudioEffect")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().resumeAllEffect(result);

        } else if (call.method.equals("stopAllAudioEffect")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().stopAllEffect(result);

        } else if (call.method.equals("registerAudioPlayerCallback")) {

            ZegoAudioPlayerController.getInstance().setAudioPlayerEventCallback(this);
            result.success(null);

        } else if (call.method.equals("unregisterAudioPlayerCallback")) {

            ZegoAudioPlayerController.getInstance().setAudioPlayerEventCallback(null);
            result.success(null);
        } else if (call.method.equals("seekToAudio")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().seekTo(call, result);

        } else if (call.method.equals("getTotalDurationAudio")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().getTotalDuration(call, result);

        } else if (call.method.equals("getCurrentDurationAudio")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            ZegoAudioPlayerController.getInstance().getCurrentDuration(call, result);

        }else if (call.method.equals("setCaptureVolume")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }
            int volume = numberToIntValue((Number) call.argument("volume"));
            mZegoLiveRoom.setCaptureVolume(volume);
            result.success(null);

        }
        /* LiveRoom-MediaPlayer */
        else if (call.method.equals("mpStart")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String path = call.argument("path");
            boolean isRepeat = numberToBoolValue((Boolean) call.argument("repeat"));
            boolean isAsset = numberToBoolValue((Boolean) call.argument("asset"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));

            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).start(path, isRepeat, isAsset, this.registrar, result);
            }

        } else if (call.method.equals("mpStop")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));

            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).stop(result);
            }

        } else if (call.method.equals("mpPause")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));

            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).pause(result);
            }

        } else if (call.method.equals("mpResume")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));

            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).resume(result);
            }

        } else if (call.method.equals("mpSeekTo")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            long timestamp = numberToLongValue((Number) call.argument("timestamp"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).seekTo(timestamp,result);
            }

        } else if (call.method.equals("mpGetDuration")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).getTotalDuration(result);
            }

        } else if (call.method.equals("mpGetCurrentDuration")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).getCurrentDuration(result);
            }

        } else if (call.method.equals("mpMuteLocal")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean mute = numberToBoolValue((Boolean) call.argument("mute"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).muteLocal(mute,result);
            }

        } else if (call.method.equals("mpLoad")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            String path = call.argument("path");
            boolean isAsset = numberToBoolValue((Boolean) call.argument("asset"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).preload(path, isAsset, registrar, result);
            }

        } else if (call.method.equals("mpSetVolume")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int vol = numberToIntValue((Number) call.argument("volume"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).setVolume(vol,result);
            }

        } else if (call.method.equals("mpSetPlayerType")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int type = numberToIntValue((Number) call.argument("type"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).setPlayerType(type,result);
            }

        } else if (call.method.equals("mpEnableRepeatMode")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).enableRepeatMode(enable,result);
            }

        } else if (call.method.equals("mpSetProcessInterval")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            long interval = numberToLongValue((Number) call.argument("interval"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).setProcessInterval(interval,result);
            }

        } else if (call.method.equals("setOnlineResourceCache")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int time = numberToIntValue((Number) call.argument("time"));
            int size = numberToIntValue((Number) call.argument("size"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).setOnlineResourceCache(time,size);
            }

            result.success(null);

        } else if (call.method.equals("getOnlineResourceCache")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).getOnlineResourceCache(result);
            }

        }
         else if (call.method.equals("mpSetAudioStream")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }
            int streamIndex = numberToIntValue((Number) call.argument("streamIndex"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).setAudioStream(streamIndex);
            }

            result.success(null);
        }
         else if (call.method.equals("mpGetAudioStreamCount")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).getAudioStreamCount(result);
            }

        }
         else if (call.method.equals("mpGetPublishVolume")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).getPublishVolume(result);
            }

        }
         else if (call.method.equals("mpGetPlayVolume")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }
            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).getPlayVolume(result);
            }

        }
         else if (call.method.equals("mpSetPublishVolume")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }
            int volume = numberToIntValue((Number) call.argument("volume"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).setPublishVolume(volume);
            }

        }

         else if (call.method.equals("mpSetPlayVolume")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }
            int volume = numberToIntValue((Number) call.argument("volume"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).setPlayVolume(volume);
            }

        }


        else if (call.method.equals("setBufferThreshold")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int threshold = numberToIntValue((Number) call.argument("threshold"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).setBufferThreshold(threshold);
            }
            result.success(null);

        } else if (call.method.equals("setLoadResourceTimeout")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int timeout = numberToIntValue((Number) call.argument("timeout"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).setLoadResourceTimeout(timeout);
            }

            result.success(null);

        } else if (call.method.equals("registerMediaPlayerCallback")) {

            if (mMediaPlayerControllers.size() > 0) {
                for (ZegoMediaPlayerController mediaPlayerController : mMediaPlayerControllers) {
                    mediaPlayerController.setMediaPlayerEventCallback(ZegoLiveRoomPlugin.this);
                }
            }
            result.success(null);

        } else if (call.method.equals("unregisterMediaPlayerCallback")) {

            if (mMediaPlayerControllers.size()>0){
                for (ZegoMediaPlayerController mediaPlayerController : mMediaPlayerControllers) {
                    mediaPlayerController.setMediaPlayerEventCallback(null);
                }
            }
            result.success(null);
        } else if (call.method.equals("createMediaPlayerRenderer")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }

            // //废弃创建 PlayerRenderer
            // if(!mIsEnablePlatformView){
            //     throwNoTextureError(result, call.method);
            //     return;
            // }

            int width = numberToIntValue((Number) call.argument("width"));
            int height = numberToIntValue((Number) call.argument("height"));

//            if(mMediaPlayerRenderer == null) {
                ZegoViewRenderer mMediaPlayerRenderer = new ZegoViewRenderer(textures.createSurfaceTexture(), width, height);
                int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));

                if (mMediaPlayerControllers.size() >= playerIndex){
                    mMediaPlayerControllers.get(playerIndex).setZegoViewRenderer(mMediaPlayerRenderer);
                    mMediaPlayerControllers.get(playerIndex).setView(mMediaPlayerRenderer.getSurface());
                }
                ZegoLogJNI.logNotice("[createMediaPlayerRenderer] view size: " + "(" + width + ", " + height + ")" + " textureID:" + mMediaPlayerRenderer.getTextureID());
                result.success(mMediaPlayerRenderer.getTextureID());
//            } else {
//                result.success(mMediaPlayerRenderer.getTextureID());
//            }

        }else if (call.method.equals("createMediaPlayerPlatformView")){
            if (mZegoLiveRoom == null){
                throwSdkNotInitError(result,call.method);
                return;
            }

            if (mIsEnablePlatformView){
                if (call.argument("viewID") != null) {
                    int viewID = numberToIntValue((Number) call.argument("viewID"));
                    int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
                    ZegoPlatformView platformView = ZegoPlatformViewFactory.shareInstance().getPlatformView(viewID);
                    if (platformView != null) {
                        int width = platformView.getSurfaceView().getHolder().getSurfaceFrame().width();
                        int height = platformView.getSurfaceView().getHolder().getSurfaceFrame().height();

                        if (mMediaPlayerControllers.size() >= playerIndex){

                            mMediaPlayerControllers.get(playerIndex).setZegoPlatformView(platformView);
                            mMediaPlayerControllers.get(playerIndex).setView(platformView.getView());
                        }

                        if (width == 0 && height == 0) {
                            reportInnerError("[createMediaPlayerPlatformView UnexpectedException] view size is zero");
                        }
                        ZegoLogJNI.logNotice("[createMediaPlayerPlatformView - ZegoPlatformView] view size: " + "(" + width + ", " + height + ")" + "viewID: " + viewID);
                        result.success(platformView.getView());
                    } else {
                        ZegoLogJNI.logNotice("[createMediaPlayerPlatformView - ZegoPlatformView] no such view");
                    }
                    //传入错误的view id
                    if (platformView == null) {
                        result.success(false);
                        return;
                    }
                }

            }else {
                result.success(null);
            }

        }else if (call.method.equals("destroyMediaPlayerPlatformView")){
            if (mZegoLiveRoom == null){
                throwSdkNotInitError(result,call.method);
                return;
            }

            if (mIsEnablePlatformView){
                int viewID = numberToIntValue((Number) call.argument("viewID"));

                int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
                boolean success = ZegoPlatformViewFactory.shareInstance().removeView(viewID);
                if (mMediaPlayerControllers.size() >= playerIndex){
                    mMediaPlayerControllers.get(playerIndex).setZegoPlatformView(null);
                    mMediaPlayerControllers.get(playerIndex).setView(null);
                }
                ZegoLogJNI.logNotice("[destroyMediaPlayerPlatformView] viewID: " + viewID);
                result.success(success);

            }else {
                result.success(null);
            }



        }else if (call.method.equals("destroyMediaPlayerRenderer")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            if (mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }
            if (!mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.get(playerIndex).getZegoViewRenderer() != null) {
                if (mMediaPlayerControllers.size() >= playerIndex){
                    mMediaPlayerControllers.get(playerIndex).setView(null);
                }
                mMediaPlayerControllers.get(playerIndex).getZegoViewRenderer().release();
                mMediaPlayerControllers.get(playerIndex).setZegoViewRenderer(null);
            }
        } else if (call.method.equals("updateMediaPlayRenderSize")) {
            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }


            if (mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }
            if (!mIsEnablePlatformView) {
                throwNoTextureError(result, call.method);
                return;
            }

            int width = numberToIntValue((Number) call.argument("width"));
            int height = numberToIntValue((Number) call.argument("height"));

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));

            if (mMediaPlayerControllers.get(playerIndex).getZegoViewRenderer() == null) {
                throwNoRendererError(result, call.method);
                return;
            }

            if (mMediaPlayerControllers.size() >= playerIndex){
                mMediaPlayerControllers.get(playerIndex).setView(null);
                boolean success = mMediaPlayerControllers.get(playerIndex).getZegoViewRenderer().updateRenderSize(width, height);
                //if(success) {
                mMediaPlayerControllers.get(playerIndex).setView(mMediaPlayerControllers.get(playerIndex).getZegoViewRenderer().getSurface());
                //}
                ZegoLogJNI.logNotice("[updateMediaPlayRenderSize] width: " + width + " height: " + height + ", textureID: " + mMediaPlayerControllers.get(playerIndex).getZegoViewRenderer().getTextureID());
                result.success(success);

            }else {
                result.success(null);
            }

        }

        /* LiveRoom-AudioIO*/
        else if (call.method.equals("enableAECWhenHeadsetDetected")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            mZegoLiveRoom.enableAECWhenHeadsetDetected(enable);
            result.success(null);

        } else if (call.method.equals("enableLoopback")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            mZegoLiveRoom.enableLoopback(enable);
            result.success(null);

        } else if (call.method.equals("setLoopbackVolume")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            int volume = numberToIntValue((Number) call.argument("volume"));
            mZegoLiveRoom.setLoopbackVolume(volume);
            result.success(null);

        } else if (call.method.equals("enableVirtualStereo")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            int angle = numberToIntValue((Number) call.argument("angle"));

            ZegoAudioProcessing.enableVirtualStereo(enable, angle);
            result.success(null);

        } else if (call.method.equals("enableReverb")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            int mode = numberToIntValue((Number) call.argument("mode"));

            ZegoAudioReverbMode md;
            switch (mode) {
                case 0:
                    md = ZegoAudioReverbMode.SOFT_ROOM;
                    break;
                case 1:
                    md = ZegoAudioReverbMode.WARM_CLUB;
                    break;
                case 2:
                    md = ZegoAudioReverbMode.CONCERT_HALL;
                    break;
                case 3:
                    md = ZegoAudioReverbMode.LARGE_AUDITORIUM;
                    break;
                default:
                    md = ZegoAudioReverbMode.SOFT_ROOM;
                    break;
            }

            ZegoAudioProcessing.enableReverb(enable, md);
            result.success(null);

        } else if (call.method.equals("setReverbParam")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            HashMap<String, Object> param = call.argument("param");
            ZegoAudioReverbParam objParam = new ZegoAudioReverbParam();
            objParam.roomSize = numberToFloatValue((Number) param.get("roomSize"));
            objParam.reverberance = numberToFloatValue((Number) param.get("reverberance"));
            objParam.damping = numberToFloatValue((Number) param.get("damping"));
            objParam.dryWetRatio = numberToFloatValue((Number) param.get("dryWetRatio"));

            ZegoAudioProcessing.setReverbParam(objParam);
            result.success(null);

        } else if (call.method.equals("setVoiceChangerParam")) {

            if (mZegoLiveRoom == null) {
                throwSdkNotInitError(result, call.method);
                return;
            }

            float param = numberToFloatValue((Number) call.argument("param"));
            ZegoAudioProcessing.setVoiceChangerParam(param);
            result.success(null);
        }

        /* External Video Filter */
        else if (call.method.equals("enableExternalVideoFilterFactory")) {
            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            ZegoExternalVideoFilter.setVideoFilterFactory(enable ? videoFilterFactory : null, 0);
            result.success(null);
        } else if (call.method.equals("enableExternalVideoCaptureFactory")) {
            boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
            ZegoExternalVideoCapture.setVideoCaptureFactory(enable ? videoCaptureFactory : null, 0);
            result.success(null);
        } else if (call.method.equals("setConfig")) {
            String config = call.argument("config");
            ZegoLiveRoom.setConfig(config);
            result.success(null);
        }

        /* Error Code */
        else if (call.method.equals("isInitSDKError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isInitSDKError(error);
            result.success(ret);

        } else if (call.method.equals("isNotLoginError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isNotLoginError(error);
            result.success(ret);

        } else if (call.method.equals("isMediaServerNetWorkError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isMediaServerNetWorkError(error);
            result.success(ret);

        } else if (call.method.equals("isLogicServerNetWorkError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isLogicServerNetWrokError(error);
            result.success(ret);

        } else if (call.method.equals("isMixStreamNotExistError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isMixStreamNotExistError(error);
            result.success(ret);

        } else if (call.method.equals("isPlayStreamNotExistError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isPlayStreamNotExistError(error);
            result.success(ret);

        } else if (call.method.equals("isPublishBadNameError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isPublishBadNameError(error);
            result.success(ret);

        } else if (call.method.equals("isPublishForbidError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isPublishForbidError(error);
            result.success(ret);

        } else if (call.method.equals("isPublishStopError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isPublishStopError(error);
            result.success(ret);

        } else if (call.method.equals("isPublishDeniedError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isPublishDeniedError(error);
            result.success(ret);

        } else if (call.method.equals("isPlayForbidError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isPlayForbidError(error);
            result.success(ret);

        } else if (call.method.equals("isPlayDeniedError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isPlayDeniedError(error);
            result.success(ret);

        } else if (call.method.equals("isDNSResolveError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isDNSResolveError(error);
            result.success(ret);

        } else if (call.method.equals("isNetworkUnreachError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isNetworkUnreachError(error);
            result.success(ret);

        } else if (call.method.equals("isHttpProtocolError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isHttpProtocolError(error);
            result.success(ret);

        } else if (call.method.equals("isReqFrequencyLimitError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isReqFrequencyLimitError(error);
            result.success(ret);

        } else if (call.method.equals("isLiveRoomError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isLiveRoomError(error);
            result.success(ret);

        } else if (call.method.equals("isMultiLoginError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isMultiLoginError(error);
            result.success(ret);

        } else if (call.method.equals("isManualKickoutError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isManualKickoutError(error);
            result.success(ret);

        } else if (call.method.equals("isRoomSessionError")) {

            int error = numberToIntValue((Number) call.argument("error"));
            boolean ret = ZegoError.isRoomSessionError(error);
            result.success(ret);

        } else if (call.method.equals("getErrorMsg")) {

            int error = numberToIntValue((Number) call.argument("error"));
            String msg = ZegoError.getErrorMsg(error);
            result.success(msg);

        } else if (call.method.equals("addNoticeLog")) {

            String content = call.argument("content");
            ZegoLogJNI.logNotice(content);

            result.success(null);
        } else if (call.method.equals("isPlaying")) {

            int soundID = numberToIntValue((Number) call.argument("soundID"));
            boolean isPlay = ZegoAudioPlayerController.getInstance().getAudioPlayerState(soundID);
            result.success(isPlay);

        } else if (call.method.equals("mpIsPlaying")) {

            int playerIndex = numberToIntValue((Number) call.argument("playerIndex"));
            if (mMediaPlayerControllers.size() >= playerIndex){
                boolean isPlay = mMediaPlayerControllers.get(playerIndex).getMediaPlayerState();
                result.success(isPlay);
            }else {
                result.success(null);
            }

        } else {

            result.notImplemented();
        }

    }

    @Override
    public void onListen(Object o, EventChannel.EventSink sink) {

        ZegoLogJNI.logNotice("[Flutter-Native] onListen sink: " + sink + " object: " + o);
        mEventSink = sink;
    }

    @Override
    public void onCancel(Object o) {

        ZegoLogJNI.logNotice("[Flutter-Native] onCancel sink, object: " + o);
        mEventSink = null;
    }

    public byte[] convertStringToSign(String strSignKey) throws NumberFormatException {
        String[] keys = strSignKey.split(",");
        if (keys.length != 32) {
            throw new NumberFormatException("App Sign Key Illegal");
        }
        byte[] byteSignKey = new byte[32];
        for (int i = 0; i < 32; i++) {
            int data = Integer.valueOf(keys[i].trim().replace("0x", ""), 16);
            byteSignKey[i] = (byte) data;
        }
        return byteSignKey;
    }

    public void throwSdkNotInitError(Result result, String methodName) {
        String errorMessage = String.format("[ERROR]: %s %s", methodName, "error because zegoliveroom api is not inited.");
        ZegoLogJNI.logNotice("[Flutter-Native] " + errorMessage);
        result.error(String.format("%s_ERROR", methodName).toUpperCase(), errorMessage, null);
    }

    public void throwNoRendererError(Result result, String methodName) {
        String errorMessage = String.format("[ERROR]: %s %s", methodName, "error because zego preview or play renderer is null.");
        ZegoLogJNI.logNotice("[Flutter-Native] " + errorMessage);
        result.error(String.format("%s_ERROR", methodName).toUpperCase(), errorMessage, null);
    }

    public void throwNoTextureError(Result result, String methodName) {
        String errorMessage = String.format("[ERROR]: %s %s", methodName, "error because \'enablePlatformView\' is true. make sure you turn off this api before calling \'initSDK\' when you use texture to render.");
        ZegoLogJNI.logNotice("[Flutter-Native] " + errorMessage);
        result.error(String.format("%s_ERROR", methodName).toUpperCase(), errorMessage, null);
    }

    public void throwNoPlatformViewError(Result result, String methodName) {
        String errorMessage = String.format("[ERROR]: %s %s", methodName, "error because \'enablePlatformView\' is false. make sure you turn on this api before calling \'initSDK\' when you use platform view to render.");
        ZegoLogJNI.logNotice("[Flutter-Native] " + errorMessage);
        result.error(String.format("%s_ERROR", methodName).toUpperCase(), errorMessage, null);
    }

    public void reportInnerError(String message) {
        if (mEventSink != null) {
            HashMap<String, Object> returnMap = new HashMap<>();
            returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

            HashMap<String, Object> method = new HashMap<>();
            method.put("name", "onInnerError");
            method.put("message", message);

            returnMap.put("method", method);
            mEventSink.success(returnMap);
        }
    }

    //Handle Flutter CallMethods

    public void initSDKWithAppID(long appID, String strAppSign, final Result result) {

        if (mZegoLiveRoom == null)
            mZegoLiveRoom = new ZegoLiveRoom();

        //调用其他API前必须调用该函数
        ZegoLiveRoom.setSDKContext(new ZegoLiveRoom.SDKContextEx() {

            @Override
            public long getLogFileSize() {
                ZegoLogJNI.logNotice("[Flutter-Native] getLogFileSize: " + mLogSize);
                return mLogSize;
            }

            @Override
            public String getSubLogFolder() {
                return null;
            }

            @Override
            public com.zego.zegoliveroom.callback.IZegoLogHookCallback getLogHookCallback() {
                return null;
            }

            @Override
            public Application getAppContext() {
                return (Application) mContext;
            }

            @Override
            public String getLogPath() {
                ZegoLogJNI.logNotice("[Flutter-Native] getLogPath: " + mLogPath);
                return mLogPath;
            }

            @Override
            public String getSoFullPath() {
                return null;
            }


        });

        ZegoLogJNI.logNotice("[Flutter-Native] uninit sdk before initsdk next time");
        mZegoLiveRoom.unInitSDK();

        ZegoLogJNI.logNotice("[Flutter-Native] enter init sdk, app id: " + appID);

        mZegoLiveRoom.setZegoRoomCallback(new IZegoRoomCallback() {
            @Override
            public void onStreamUpdated(int type, ZegoStreamInfo[] zegoStreamInfos, String roomID) {
                ZegoLogJNI.logNotice("[Flutter-Native] onStreamUpdate enter, sink: " + mEventSink);
                if (mEventSink != null) {

                    ArrayList<HashMap<String, Object>> streamList = new ArrayList<>();
                    for (ZegoStreamInfo streamInfo : zegoStreamInfos) {
                        HashMap<String, Object> map = new HashMap<>();
                        map.put("userID", streamInfo.userID);
                        map.put("userName", streamInfo.userName);
                        map.put("streamID", streamInfo.streamID);
                        map.put("extraInfo", streamInfo.extraInfo);
                        streamList.add(map);
                    }

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onStreamUpdated");
                    method.put("updateType", type);
                    method.put("roomID", roomID);
                    method.put("streamList", streamList);

                    returnMap.put("method", method);
                    ZegoLogJNI.logNotice("[Flutter-Native] onStreamUpdate, return map: " + returnMap);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onStreamExtraInfoUpdated(ZegoStreamInfo[] zegoStreamInfos, String roomID) {
                if (mEventSink != null) {

                    ArrayList<HashMap<String, Object>> streamList = new ArrayList<>();
                    for (ZegoStreamInfo streamInfo : zegoStreamInfos) {
                        HashMap<String, Object> map = new HashMap<>();
                        map.put("userID", streamInfo.userID);
                        map.put("userName", streamInfo.userName);
                        map.put("streamID", streamInfo.streamID);
                        map.put("extraInfo", streamInfo.extraInfo);
                        streamList.add(map);
                    }

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onStreamExtraInfoUpdated");
                    method.put("roomID", roomID);
                    method.put("streamList", streamList);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onTempBroken(int errorCode, String roomID) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onTempBroken");
                    method.put("roomID", roomID);
                    method.put("errorCode", errorCode);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onRoomInfoUpdated(ZegoRoomInfo zegoRoomInfo, String s) {

            }

            @Override
            public void onReconnect(int errorCode, String roomID) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onReconnect");
                    method.put("roomID", roomID);
                    method.put("errorCode", errorCode);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onDisconnect(int errorCode, String roomID) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onDisconnect");
                    method.put("roomID", roomID);
                    method.put("errorCode", errorCode);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onKickOut(int reason, String roomID, String customReason) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onKickOut");
                    method.put("roomID", roomID);
                    method.put("reason", reason);
                    method.put("customReason", customReason);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onRecvCustomCommand(String userID, String userName, String content, String roomID) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onReceiveCustomCommand");
                    method.put("userID", userID);
                    method.put("userName", userName);
                    method.put("content", content);
                    method.put("roomID", roomID);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onNetworkQuality(String userID, int txQuality, int rxQuality) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onNetworkQuality");
                    method.put("userID", userID);
                    method.put("txQuality", txQuality);
                    method.put("rxQuality", rxQuality);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onTokenWillExpired(String roomID, int remainTimeInSecond) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onTokenWillExpired");
                    method.put("roomID", roomID);
                    method.put("remainTimeInSecond", remainTimeInSecond);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }
        });

        mZegoLiveRoom.setZegoDeviceEventCallback(new IZegoDeviceEventCallback() {
            @Override
            public void onDeviceError(String deviceName, int errorCode) {
                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onDeviceError");
                    method.put("deviceName", deviceName);
                    method.put("errorCode", errorCode);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }
        });

        mZegoLiveRoom.setZegoIMCallback(new IZegoIMCallback() {
            @Override
            public void onUserUpdate(ZegoUserState[] zegoUserStates, int type, String roomID) {
                if (mEventSink != null) {

                    ArrayList<HashMap<String, Object>> userList = new ArrayList<>();
                    for (ZegoUserState userInfo : zegoUserStates) {
                        HashMap<String, Object> map = new HashMap<>();
                        map.put("userID", userInfo.userID);
                        map.put("userName", userInfo.userName);
                        map.put("updateFlag", userInfo.updateFlag);
                        map.put("role", userInfo.roomRole);
                        userList.add(map);
                    }

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onUserUpdate");
                    method.put("updateType", type);
                    method.put("userList", userList);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onRecvRoomMessage(String roomID, ZegoRoomMessage[] messageList) {
                if (mEventSink != null) {

                    ArrayList<HashMap<String, Object>> messageArray = new ArrayList<>();
                    for (ZegoRoomMessage message : messageList) {
                        HashMap<String, Object> map = new HashMap<>();
                        map.put("content", message.content);
                        map.put("fromUserID", message.fromUserID);
                        map.put("fromUserName", message.fromUserName);
                        map.put("messageID", message.messageID);
                        messageArray.add(map);
                    }

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onRecvRoomMessage");
                    method.put("roomID", roomID);
                    method.put("messageList", messageArray);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onUpdateOnlineCount(String roomID, int onlineCount) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onUpdateOnlineCount");
                    method.put("onlineCount", onlineCount);
                    method.put("roomID", roomID);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onRecvBigRoomMessage(String roomID, ZegoBigRoomMessage[] messageList) {
                if (mEventSink != null) {

                    ArrayList<HashMap<String, Object>> messageArray = new ArrayList<>();
                    for (ZegoBigRoomMessage message : messageList) {
                        HashMap<String, Object> map = new HashMap<>();
                        map.put("content", message.content);
                        map.put("fromUserID", message.fromUserID);
                        map.put("fromUserName", message.fromUserName);
                        map.put("messageID", message.messageID);
                        messageArray.add(map);
                    }

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onRecvBigRoomMessage");
                    method.put("roomID", roomID);
                    method.put("messageList", messageArray);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }
        });

        mZegoLiveRoom.setZegoLivePublisherCallback(new IZegoLivePublisherCallback() {
            @Override
            public void onPublishStateUpdate(int stateCode, String streamID, HashMap<String, Object> streamInfo) {
                ZegoLogJNI.logNotice("[Flutter-Native] onPublishStateUpdate enter, sink: " + mEventSink);
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onPublishStateUpdate");
                    method.put("stateCode", stateCode);
                    method.put("streamID", streamID);

                    HashMap<String, Object> info = new HashMap<>();
                    info.put("streamID", streamInfo.get(ZegoConstants.StreamKey.STREAM_ID));
                    String[] rtmpList = (String[]) streamInfo.get(ZegoConstants.StreamKey.RTMP_URL_LIST);
                    String[] flvList = (String[]) streamInfo.get(ZegoConstants.StreamKey.FLV_URL_LIST);
                    String[] hlsList = (String[]) streamInfo.get(ZegoConstants.StreamKey.HLS_URL_LST);
                    info.put("rtmpList", new ArrayList<>(Arrays.asList(rtmpList)));
                    info.put("flvList", new ArrayList<>(Arrays.asList(flvList)));
                    info.put("hlsList", new ArrayList<>(Arrays.asList(hlsList)));
                    method.put("streamInfo", info);

                    returnMap.put("method", method);
                    ZegoLogJNI.logNotice("[Flutter-Native] onPublishStateUpdate, return map: " + returnMap);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onJoinLiveRequest(int seq, String fromUserID, String fromUserName, String roomID) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onJoinLiveRequest");
                    method.put("seq", seq);
                    method.put("fromUserID", fromUserID);
                    method.put("fromUserName", fromUserName);
                    method.put("roomID", roomID);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onPublishQualityUpdate(String streamID, ZegoPublishStreamQuality zegoStreamQuality) {

                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onPublishQualityUpdate");
                    method.put("streamID", streamID);

                    method.put("acapFps", zegoStreamQuality.acapFps);
                    method.put("anetFps", zegoStreamQuality.anetFps);
                    method.put("akbps", zegoStreamQuality.akbps);

                    method.put("vcapFps", zegoStreamQuality.vcapFps);
                    method.put("vencFps", zegoStreamQuality.vencFps);
                    method.put("vnetFps", zegoStreamQuality.vnetFps);
                    method.put("vkbps", zegoStreamQuality.vkbps);

                    method.put("rtt", zegoStreamQuality.rtt);
                    method.put("pktLostRate", zegoStreamQuality.pktLostRate);

                    method.put("isHardwareVenc", zegoStreamQuality.isHardwareVenc);
                    method.put("videoCodecId", zegoStreamQuality.videoCodecId);

                    method.put("width", zegoStreamQuality.width);
                    method.put("height", zegoStreamQuality.height);

                    method.put("quality", zegoStreamQuality.quality);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onCaptureVideoSizeChangedTo(int width, int height) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onCaptureVideoSizeChangedTo");
                    method.put("width", width);
                    method.put("height", height);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onCaptureVideoFirstFrame() {
                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onCaptureVideoFirstFrame");

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }

            }

            @Override
            public void onCaptureAudioFirstFrame() {
                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onCaptureAudioFirstFrame");

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }
        });

        mZegoLiveRoom.setZegoLivePublisherCallback2(new IZegoLivePublisherCallback2() {
            @Override
            public void onCaptureVideoSizeChangedTo(int channelIndex, int width, int height) {
                ZegoLogJNI.logNotice("[Flutter-Native] onCaptureVideoSizeChangedTo enter, sink: " + mEventSink);
                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onCaptureVideoSizeChangedTo");
                    method.put("channelIndex", channelIndex);
                    method.put("width", width);
                    method.put("height", height);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onCaptureVideoFirstFrame(int channelIndex) {
                ZegoLogJNI.logNotice("[Flutter-Native] onCaptureVideoFirstFrame enter, sink: " + mEventSink);
                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onCaptureVideoFirstFrame");
                    method.put("channelIndex", channelIndex);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onPreviewVideoFirstFrame(int channelIndex) {
                ZegoLogJNI.logNotice("[Flutter-Native] onPreviewVideoFirstFrame enter, sink: " + mEventSink);
                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onPreviewVideoFirstFrame");
                    method.put("channelIndex", channelIndex);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onSendLocalAudioFirstFrame(int channelIndex) {
                ZegoLogJNI.logNotice("[Flutter-Native] onSendLocalAudioFirstFrame enter, sink: " + mEventSink);
                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onSendLocalAudioFirstFrame");
                    method.put("channelIndex", channelIndex);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onSendLocalVideoFirstFrame(int channelIndex) {
                ZegoLogJNI.logNotice("[Flutter-Native] onSendLocalVideoFirstFrame enter, sink: " + mEventSink);
                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onSendLocalVideoFirstFrame");
                    method.put("channelIndex", channelIndex);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onVideoEncoderError(int codecID, int errorCode, int channelIndex) {
                ZegoLogJNI.logNotice("[Flutter-Native] onVideoEncoderError enter, sink: " + mEventSink);
                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onVideoEncoderError");
                    method.put("codecID", codecID);
                    method.put("errorCode", errorCode);
                    method.put("channelIndex", channelIndex);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onVideoEncoderChanged(int fromCodecID, int toCodecID, int channelIndex) {
                ZegoLogJNI.logNotice("[Flutter-Native] onVideoEncoderChanged enter, sink: " + mEventSink);
                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onVideoEncoderChanged");
                    method.put("fromCodecID", fromCodecID);
                    method.put("toCodecID", toCodecID);
                    method.put("channelIndex", channelIndex);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }
        });

        mZegoLiveRoom.setZegoLivePublisherExCallback(new IZegoLivePublisherExCallback() {
            @Override
            public void onRelayCDNStateUpdate(ZegoStreamRelayCDNInfo[] stateInfo, String streamID) {
                ZegoLogJNI.logNotice("[Flutter-Native] onRelayCDNStateUpdate enter, sink: " + mEventSink);
                if (mEventSink != null) {
                    ArrayList<HashMap<String, Object>> infoList = new ArrayList<>();
                    for (ZegoStreamRelayCDNInfo info : stateInfo) {
                        HashMap<String, Object> map = new HashMap<>();
                        map.put("datail", info.detail);
                        map.put("rtmpURL", info.rtmpURL);
                        map.put("state", info.state);
                        map.put("stateTime", info.stateTime);
                        infoList.add(map);
                    }

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PUBLISH_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onRelayCDNStateUpdate");
                    method.put("stateInfo", infoList);
                    method.put("streamID", streamID);

                    returnMap.put("method", method);
                    ZegoLogJNI.logNotice("[Flutter-Native] onRelayCDNStateUpdate, return map: " + returnMap);
                    mEventSink.success(returnMap);
                }
            }
        });

        mZegoLiveRoom.setZegoLivePlayerCallback(new IZegoLivePlayerCallback2() {
            @Override
            public void onPlayStateUpdate(int stateCode, String streamID) {
                ZegoLogJNI.logNotice("[Flutter-Native] onPlayStateUpdate enter, sink: " + mEventSink);
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PLAY_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onPlayStateUpdate");
                    method.put("stateCode", stateCode);
                    method.put("streamID", streamID);

                    returnMap.put("method", method);
                    ZegoLogJNI.logNotice("[Flutter-Native] onPlayStateUpdate, return map: " + returnMap);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onPlayQualityUpdate(String streamID, ZegoPlayStreamQuality zegoStreamQuality) {

                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PLAY_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onPlayQualityUpdate");
                    method.put("streamID", streamID);

                    method.put("vnetFps", zegoStreamQuality.vnetFps);
                    method.put("vdjFps", zegoStreamQuality.vdjFps);
                    method.put("vdecFps", zegoStreamQuality.vdecFps);
                    method.put("vrndFps", zegoStreamQuality.vrndFps);
                    method.put("vkbps", zegoStreamQuality.vkbps);

                    method.put("anetFps", zegoStreamQuality.anetFps);
                    method.put("adjFps", zegoStreamQuality.adjFps);
                    method.put("adecFps", zegoStreamQuality.adecFps);
                    method.put("arndFps", zegoStreamQuality.arndFps);
                    method.put("akbps", zegoStreamQuality.akbps);

                    method.put("audioBreakRate", zegoStreamQuality.audioBreakRate);
                    method.put("videoBreakRate", zegoStreamQuality.videoBreakRate);
                    method.put("rtt", zegoStreamQuality.rtt);
                    method.put("p2pRtt", zegoStreamQuality.peerToPeerDelay);
                    method.put("pktLostRate", zegoStreamQuality.pktLostRate);
                    method.put("p2pPktLostRate", zegoStreamQuality.peerToPeerPktLostRate);
                    method.put("quality", zegoStreamQuality.quality);
                    method.put("delay", zegoStreamQuality.delay);

                    method.put("isHardwareVdec", zegoStreamQuality.isHardwareVdec);

                    method.put("width", zegoStreamQuality.width);
                    method.put("height", zegoStreamQuality.height);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onInviteJoinLiveRequest(int seq, String fromUserID, String fromUserName, String roomID) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PLAY_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onInviteJoinLiveRequest");
                    method.put("seq", seq);
                    method.put("fromUserID", fromUserID);
                    method.put("fromUserName", fromUserName);
                    method.put("roomID", roomID);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onRecvEndJoinLiveCommand(String fromUserID, String fromUserName, String roomID) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PLAY_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onRecvEndJoinLiveCommand");
                    method.put("fromUserID", fromUserID);
                    method.put("fromUserName", fromUserName);
                    method.put("roomID", roomID);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onVideoSizeChangedTo(String streamID, int width, int height) {

                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PLAY_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onVideoSizeChangedTo");
                    method.put("streamID", streamID);
                    method.put("width", width);
                    method.put("height", height);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }

            }

            @Override
            public void onRemoteCameraStatusUpdate(String streamID, int status, int reason) {

                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PLAY_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onRemoteCameraStatusUpdate");
                    method.put("streamID", streamID);
                    method.put("status", status);
                    method.put("reason", reason);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onRemoteMicStatusUpdate(String streamID, int status, int reason) {

                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PLAY_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onRemoteMicStatusUpdate");
                    method.put("streamID", streamID);
                    method.put("status", status);
                    method.put("reason", reason);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onRemoteSpeakerStatusUpdate(String streamID, int status, int reason) {

            }

            @Override
            public void onRecvRemoteAudioFirstFrame(String streamID) {

                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PLAY_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onRecvRemoteAudioFirstFrame");
                    method.put("streamID", streamID);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onRecvRemoteVideoFirstFrame(String streamID) {

                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PLAY_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onRecvRemoteVideoFirstFrame");
                    method.put("streamID", streamID);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onRenderRemoteVideoFirstFrame(String streamID) {

                if (mEventSink != null) {
                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_PLAY_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onRenderRemoteVideoFirstFrame");
                    method.put("streamID", streamID);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onVideoDecoderError(int codecID, int errorCode, String streamID) {

            }

            @Override
            public void onPlayStatsUpdate(ZegoPlayStats stats) {

            }
        });

        mZegoLiveRoom.setZegoLiveEventCallback(new IZegoLiveEventCallback() {
            @Override
            public void onLiveEvent(int event, HashMap<String, String> info) {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onLiveEvent");
                    method.put("event", event);
                    method.put("info", info);

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }
        });

        mZegoLiveRoom.setZegoAVEngineCallback(new IZegoAVEngineCallback() {
            @Override
            public void onAVEngineStart() {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onAVEngineStart");

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }

            @Override
            public void onAVEngineStop() {
                if (mEventSink != null) {

                    HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_ROOM_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onAVEngineStop");

                    returnMap.put("method", method);
                    mEventSink.success(returnMap);
                }
            }
        });

        byte[] appSign = convertStringToSign(strAppSign);
        boolean success = mZegoLiveRoom.initSDK(appID, appSign, new IZegoInitSDKCompletionCallback() {

            @Override
            public void onInitSDK(int errorCode) {

                ZegoLogJNI.logNotice("[Flutter-Native] on init sdk, errorCode: " + errorCode);
                result.success(errorCode);
            }
        });

        if (!success)
            result.success(false);

        ZegoAudioPlayerController.getInstance().init();
//        ZegoMediaPlayerController.getInstance().init();

        mMediaPlayerControllers.clear();
        for (int i=0;i<=3;i++){
            ZegoMediaPlayerController mediaPlayerController = new ZegoMediaPlayerController(i);
            mMediaPlayerControllers.add(mediaPlayerController);
        }

        mZegoMediaSideInfo = new ZegoMediaSideInfo();
        mZegoMediaSideInfo.setZegoMediaSideCallback(new IZegoMediaSideCallback() {
            @Override
            public void onRecvMediaSideInfo(String streamID, ByteBuffer data, int dataLen) {

                if (mEventSink != null) {

                    if (dataLen == 0) {
                        return;
                    }

                    int mediaType = (data.get(0) & 0xFF) << 24 | (data.get(1) & 0xFF) << 16 | (data.get(2) & 0xFF) << 8 | (data.get(3) & 0xFF);

                    byte[] tempBuffer;
                    if (mediaType == 1001 || mediaType == 1002) {

                        tempBuffer = new byte[dataLen - 4];
                        for (int i = 0; i < dataLen - 4; i++) {
                            tempBuffer[i] = data.get(i + 4);
                        }

                    } else {

                        tempBuffer = new byte[dataLen - 5];
                        for (int i = 0; i < dataLen - 5; i++) {
                            tempBuffer[i] = data.get(i + 5);
                        }

                    }


                    String strData = new String(tempBuffer);

                    final HashMap<String, Object> returnMap = new HashMap<>();
                    returnMap.put("type", ZegoEventType.TYPE_MEDIA_SIDE_INFO_EVENT);

                    HashMap<String, Object> method = new HashMap<>();
                    method.put("name", "onRecvMediaSideInfo");
                    method.put("streamID", streamID);
                    method.put("data", strData);

                    returnMap.put("method", method);
                    Handler mainHandler = new Handler(Looper.getMainLooper());
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            mEventSink.success(returnMap);
                        }
                    });
                }
            }
        });


    }

    @Override
    public void onAudioPlayEnd(int soundID) {
        if (mEventSink != null) {

            HashMap<String, Object> returnMap = new HashMap<>();
            returnMap.put("type", ZegoEventType.TYPE_AUDIO_PLAYER_EVENT);

            HashMap<String, Object> method = new HashMap<>();
            method.put("name", "onAudioPlayEnd");
            method.put("soundID", soundID);

            returnMap.put("method", method);
            mEventSink.success(returnMap);
        }
    }

    @Override
    public void onAudioPlayBegin(int soundID,int errorCode) {
        HashMap<String, Object> returnMap = new HashMap<>();
        returnMap.put("type", ZegoEventType.TYPE_AUDIO_PLAYER_EVENT);

        HashMap<String, Object> method = new HashMap<>();
        method.put("name", "onAudioPlayBegin");
        method.put("soundID",soundID);
        method.put("errorCode", errorCode);

        returnMap.put("method", method);
        mEventSink.success(returnMap);
    }

    @Override
    public void onAudioLoad(int soundID,int errorCode) {
        HashMap<String, Object> returnMap = new HashMap<>();
        returnMap.put("type", ZegoEventType.TYPE_AUDIO_PLAYER_EVENT);

        HashMap<String, Object> method = new HashMap<>();
        method.put("name", "onAudioLoad");
        method.put("soundID", soundID);
        method.put("errorCode", errorCode);

        returnMap.put("method", method);
        mEventSink.success(returnMap);
    }

    @Override
    public void onAudioLoadComplete(int soundID) {
        HashMap<String,Object> returnMap = new HashMap<>();
        returnMap.put("type",ZegoEventType.TYPE_AUDIO_PLAYER_EVENT);

        HashMap<String,Object> method = new HashMap<>();
        method.put("name","onAudioLoadComplete");
        method.put("soundID",soundID);

        returnMap.put("method",method);
        mEventSink.success(returnMap);
    }

    @Override
    public void onPlayEnd(int playerIndex) {
        if (mEventSink != null) {

            HashMap<String, Object> returnMap = new HashMap<>();
            returnMap.put("type", ZegoEventType.TYPE_MEDIA_PLAYER_EVENT);

            HashMap<String, Object> method = new HashMap<>();
            method.put("name", "onPlayEnd");
            method.put("playerIndex", playerIndex);

            returnMap.put("method", method);
            mEventSink.success(returnMap);
        }
    }

    @Override
    public void onPlayError(int errorCode,int playerIndex) {
        if (mEventSink != null) {

            HashMap<String, Object> returnMap = new HashMap<>();
            returnMap.put("type", ZegoEventType.TYPE_MEDIA_PLAYER_EVENT);

            HashMap<String, Object> method = new HashMap<>();
            method.put("name", "onPlayError");
            method.put("errorCode", errorCode);
            method.put("playerIndex", playerIndex);

            returnMap.put("method", method);
            mEventSink.success(returnMap);
        }
    }

    @Override
    public void onBufferBegin(int playerIndex) {
        if (mEventSink != null) {

            HashMap<String, Object> returnMap = new HashMap<>();
            returnMap.put("type", ZegoEventType.TYPE_MEDIA_PLAYER_EVENT);

            HashMap<String, Object> method = new HashMap<>();
            method.put("name", "onBufferBegin");
            method.put("playerIndex", playerIndex);

            returnMap.put("method", method);
            mEventSink.success(returnMap);
        }
    }

    @Override
    public void onBufferEnd(int playerIndex) {
        if (mEventSink != null) {

            HashMap<String, Object> returnMap = new HashMap<>();
            returnMap.put("type", ZegoEventType.TYPE_MEDIA_PLAYER_EVENT);

            HashMap<String, Object> method = new HashMap<>();
            method.put("name", "onBufferEnd");
            method.put("playerIndex", playerIndex);

            returnMap.put("method", method);
            mEventSink.success(returnMap);
        }
    }

    @Override
    public void onProcessInterval(long timestamp,int playerIndex) {
        if (mEventSink != null) {

            final HashMap<String, Object> returnMap = new HashMap<>();
            returnMap.put("type", ZegoEventType.TYPE_MEDIA_PLAYER_EVENT);

            HashMap<String, Object> method = new HashMap<>();
            method.put("name", "onProcessInterval");
            method.put("timestamp", timestamp);
            method.put("playerIndex", playerIndex);

            returnMap.put("method", method);
            Handler mainHandler = new Handler(Looper.getMainLooper());
            mainHandler.post(new Runnable() {
                @Override
                public void run() {
                    mEventSink.success(returnMap);
                }
            });

        }
    }

    @Override
    public void onPlayBegin(int playerIndex) {
        HashMap<String, Object> returnMap = new HashMap<>();
        returnMap.put("type", ZegoEventType.TYPE_MEDIA_PLAYER_EVENT);

        HashMap<String, Object> method = new HashMap<>();
        method.put("name", "onPlayBegin");
        method.put("playerIndex", playerIndex);

        returnMap.put("method", method);
        mEventSink.success(returnMap);
    }

    @Override
    public void onPlayStop(int playerIndex) {
        HashMap<String, Object> returnMap = new HashMap<>();
        returnMap.put("type", ZegoEventType.TYPE_MEDIA_PLAYER_EVENT);

        HashMap<String, Object> method = new HashMap<>();
        method.put("name", "onPlayStop");
        method.put("playerIndex", playerIndex);

        returnMap.put("method", method);
        mEventSink.success(returnMap);
    }

    @Override
    public void onPlayPause(int playerIndex) {
        HashMap<String, Object> returnMap = new HashMap<>();
        returnMap.put("type", ZegoEventType.TYPE_MEDIA_PLAYER_EVENT);

        HashMap<String, Object> method = new HashMap<>();
        method.put("name", "onPlayPause");
        method.put("playerIndex", playerIndex);

        returnMap.put("method", method);
        mEventSink.success(returnMap);
    }

    @Override
    public void onPlayResume(int playerIndex) {
        HashMap<String, Object> returnMap = new HashMap<>();
        returnMap.put("type", ZegoEventType.TYPE_MEDIA_PLAYER_EVENT);

        HashMap<String, Object> method = new HashMap<>();
        method.put("name", "onPlayResume");
        method.put("playerIndex", playerIndex);
        returnMap.put("method", method);
        mEventSink.success(returnMap);
    }

    @Override
    public void onSeekComplete(int error, long millisecond,int playerIndex) {
        HashMap<String, Object> returnMap = new HashMap<>();
        returnMap.put("type", ZegoEventType.TYPE_MEDIA_PLAYER_EVENT);

        HashMap<String, Object> method = new HashMap<>();
        method.put("name", "onSeekComplete");
        method.put("error",error);
        method.put("millisecond", millisecond);
        method.put("playerIndex", playerIndex);

        returnMap.put("method", method);
        mEventSink.success(returnMap);
    }

    @Override
    public void onAudioBegin(int playerIndex) {
        HashMap<String, Object> returnMap = new HashMap<>();
        returnMap.put("type", ZegoEventType.TYPE_MEDIA_PLAYER_EVENT);

        HashMap<String, Object> method = new HashMap<>();
        method.put("name", "onAudioBegin");
        method.put("playerIndex", playerIndex);

        returnMap.put("method", method);
        mEventSink.success(returnMap);
    }

    @Override
    public void onVideoBegin(int playerIndex) {
        HashMap<String, Object> returnMap = new HashMap<>();
        returnMap.put("type", ZegoEventType.TYPE_MEDIA_PLAYER_EVENT);

        HashMap<String, Object> method = new HashMap<>();
        method.put("name", "onVideoBegin");
        method.put("playerIndex", playerIndex);

        returnMap.put("method", method);
        mEventSink.success(returnMap);
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

    private float numberToFloatValue(Number number) {

        return number != null ? number.floatValue() : .0f;
    }
}
