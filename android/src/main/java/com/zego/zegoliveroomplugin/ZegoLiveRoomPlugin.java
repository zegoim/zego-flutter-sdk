package com.zego.zegoliveroomplugin;

import android.app.Application;
import android.content.Context;
import android.view.Surface;
import android.os.Looper;
import android.os.Handler;


import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.EventChannel.EventSink;
import io.flutter.plugin.common.PluginRegistry.Registrar;
import io.flutter.plugin.common.StandardMessageCodec;
import io.flutter.view.TextureRegistry;

import java.nio.ByteBuffer;
import java.util.*;
import java.lang.*;

import com.zego.zegoavkit2.ZegoStreamExtraPlayInfo;
import com.zego.zegoavkit2.entities.ZegoStreamRelayCDNInfo;
import com.zego.zegoavkit2.error.ZegoError;
import com.zego.zegoavkit2.mediaside.IZegoMediaSideCallback;
import com.zego.zegoavkit2.mediaside.ZegoMediaSideInfo;
import com.zego.zegoavkit2.soundlevel.IZegoSoundLevelCallback;
import com.zego.zegoavkit2.soundlevel.ZegoSoundLevelInfo;
import com.zego.zegoavkit2.soundlevel.ZegoSoundLevelMonitor;
import com.zego.zegoliveroom.ZegoLiveRoom;
import com.zego.zegoliveroom.callback.IZegoAVEngineCallback;
import com.zego.zegoliveroom.callback.IZegoCustomCommandCallback;
import com.zego.zegoliveroom.callback.IZegoEndJoinLiveCallback;
import com.zego.zegoliveroom.callback.IZegoInitSDKCompletionCallback;
import com.zego.zegoliveroom.callback.IZegoLiveEventCallback;
import com.zego.zegoliveroom.callback.IZegoLivePlayerCallback;
import com.zego.zegoliveroom.callback.IZegoLivePublisherExCallback;
import com.zego.zegoliveroom.callback.IZegoUpdatePublishTargetCallback;
import com.zego.zegoliveroom.callback.IZegoLivePlayerCallback2;
import com.zego.zegoliveroom.callback.IZegoLivePublisherCallback;
import com.zego.zegoliveroom.callback.IZegoLoginCompletionCallback;
import com.zego.zegoliveroom.callback.IZegoResponseCallback;
import com.zego.zegoliveroom.callback.IZegoRoomCallback;
import com.zego.zegoliveroom.callback.im.IZegoIMCallback;
import com.zego.zegoliveroom.constants.ZegoAvConfig;
import com.zego.zegoliveroom.constants.ZegoConstants;
import com.zego.zegoliveroom.entity.AuxData;
import com.zego.zegoliveroom.entity.ZegoBigRoomMessage;
import com.zego.zegoliveroom.entity.ZegoConversationMessage;
import com.zego.zegoliveroom.entity.ZegoRoomMessage;
import com.zego.zegoliveroom.entity.ZegoStreamInfo;
import com.zego.zegoliveroom.entity.ZegoPublishStreamQuality;
import com.zego.zegoliveroom.entity.ZegoPlayStreamQuality;
import com.zego.zegoliveroom.entity.ZegoUser;
import com.zego.zegoliveroom.entity.ZegoUserState;


enum EVENT_TYPE {
  TYPE_ROOM_EVENT,
  TYPE_PUBLISH_EVENT,
  TYPE_PLAY_EVENT,
  TYPE_MEDIA_SIDE_INFO_EVENT,
  TYPE_SOUND_LEVEL_EVENT
}

/** ZegoLiveRoomPlugin */
public class ZegoLiveRoomPlugin implements MethodCallHandler, EventChannel.StreamHandler {

  /** Plugin registration. */

  private static String mPublishMainChl = com.zego.zegoavkit2.ZegoConstants.ZegoVideoDataMainPublishingStream;

  private final Registrar registrar;
  private final TextureRegistry textures;
  private final Context mContext;
  private EventSink mEventSink;

  private ZegoLiveRoom mZegoLiveRoom;
  private ZegoMediaSideInfo mZegoMediaSideInfo;
  private HashMap<String, ZegoViewRenderer> mRenders;
  private boolean mIsEnablePlatformView;

  private ZegoLiveRoomPlugin(Registrar registrar) {
    this.registrar = registrar;
    this.textures = registrar.textures();
    this.mContext = registrar.context();    //获取应用程序的Context

    this.mZegoLiveRoom = null;
    this.mZegoMediaSideInfo = null;
    this.mIsEnablePlatformView = false;

    this.mRenders = new HashMap<>();
  }


  public static void registerWith(Registrar registrar) {

    ZegoLiveRoomPlugin instance = new ZegoLiveRoomPlugin(registrar);
    final MethodChannel channel = new MethodChannel(registrar.messenger(), "plugins.zego.im/zegoliveroom_plugin");
    channel.setMethodCallHandler(instance);

    final EventChannel eventChannel = new EventChannel(registrar.messenger(), "plugins.zego.im/zegoliveroom_callback");
    eventChannel.setStreamHandler(instance);

    registrar.platformViewRegistry().registerViewFactory("plugins.zego.im/zego_view", ZegoPlatformViewFactory.shareInstance());
  }

  @Override
  public void onMethodCall(MethodCall call, final Result result) {

    /* LiveRoom */
    if (call.method.equals("getSdkVersion")) {

      result.success(ZegoLiveRoom.version());

    } else if (call.method.equals("getEngineVersion")) {

      result.success(ZegoLiveRoom.version2());

    } else if(call.method.equals("setVerbose")) {

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      ZegoLiveRoom.setVerbose(enable);
      result.success(null);

    } else if (call.method.equals("uploadLog")) {

      ZegoLiveRoom.uploadLog();
      result.success(null);

    } else if (call.method.equals("getLogDir")) {

      String path = com.zego.zegoavkit2.utils.ZegoLogUtil.getLogPath(mContext);
      result.success(path);

    } else if (call.method.equals("initSDK")) {

      long appID = numberToLongValue((Number) call.argument("appID"));
      String strAppSign = call.argument("appSign");

      initSDKWithAppID(appID, strAppSign, result);

    } else if (call.method.equals("uninitSDK")) {

      if (mZegoLiveRoom == null) {

        result.success(false);

      } else {

        if(mZegoMediaSideInfo != null) {
          mZegoMediaSideInfo.setZegoMediaSideCallback(null);
          mZegoMediaSideInfo = null;
        }

        ZegoLogJNI.logNotice("[Flutter-Native] unInitSDK");
        //反初始化SDK里面会做回调的销毁处理
        result.success(mZegoLiveRoom.unInitSDK());
      }

    } else if(call.method.equals("enablePlatformView")) {

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      mIsEnablePlatformView = enable;
      result.success(null);

    } else if (call.method.equals("setUser")) {

      String userID = call.argument("userID");
      String userName = call.argument("userName");

      boolean success = ZegoLiveRoom.setUser(userID, userName);
      result.success(success);

    } else if(call.method.equals("setUseTestEnv")) {

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      ZegoLiveRoom.setTestEnv(enable);
      result.success(null);

    } else if (call.method.equals("loginRoom")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      String roomID = call.argument("roomID");
      String roomName = call.argument("roomName");
      int role = numberToIntValue((Number) call.argument("role"));

      ZegoLogJNI.logNotice("[Flutter-Native] loginRoom enter, roomID: " + roomID + " roomName: " + roomName + " role: " + role);
      boolean success = mZegoLiveRoom.loginRoom(roomID, roomName, role, new IZegoLoginCompletionCallback() {
        @Override
        public void onLoginCompletion(int i, ZegoStreamInfo[] zegoStreamInfos) {

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
          returnMap.put("errorCode", i);
          returnMap.put("streamList", streamList);
          ZegoLogJNI.logNotice("[Flutter-Native] onLoginRoom, return map: " + returnMap);
          result.success(returnMap);
        }
      });

      if(!success) {
        result.error(String.format("%s_ERROR", call.method).toUpperCase(), "call \'loginRoom\' failed, please check you call \'setUser\' first", null);
      }


    } else if (call.method.equals("logoutRoom")) {


      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean success = mZegoLiveRoom.logoutRoom();
      result.success(success);

    } else if (call.method.equals("pauseModule")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int type = numberToIntValue((Number) call.argument("type"));
      mZegoLiveRoom.pauseModule(type);
      result.success(null);

    } else if (call.method.equals("resumeModule")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int type = numberToIntValue((Number) call.argument("type"));
      mZegoLiveRoom.resumeModule(type);
      result.success(null);

    } else if(call.method.equals("enableMicDevice")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      boolean success = mZegoLiveRoom.enableMicDevice(enable);
      result.success(success);

    } else if(call.method.equals("sendCustomCommand")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      ArrayList<HashMap<String, String>> userList = call.argument("users");
      String content = call.argument("content");

      ArrayList<ZegoUser> zegoUserList = new ArrayList<>();
      for(HashMap<String, String> objUser: userList) {
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

    }
    else if(call.method.equals("setRoomConfig")) {

      if(mZegoLiveRoom == null) {
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

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int mode = numberToIntValue((Number) call.argument("mode"));
      mZegoLiveRoom.setLatencyMode(mode);
      result.success(null);

    } else if (call.method.equals("setVideoMirrorMode")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int mode = numberToIntValue((Number) call.argument("mode"));
      boolean success = mZegoLiveRoom.setVideoMirrorMode(mode, 0);
      result.success(success);

    } else if(call.method.equals("setAudioDeviceMode")) {

      int mode = numberToIntValue((Number) call.argument("mode"));
      ZegoLiveRoom.setAudioDeviceMode(mode);
      result.success(null);

    } else if (call.method.equals("startPreview")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean success = mZegoLiveRoom.startPreview();
      result.success(success);

    } else if (call.method.equals("stopPreview")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean success = mZegoLiveRoom.stopPreview();
      result.success(success);

    } else if (call.method.equals("startPublishing")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      String streamID = call.argument("streamID");
      String title = call.argument("title");
      int flag = numberToIntValue((Number) call.argument("flag"));
      String extraInfo = call.argument("extraInfo");

      boolean success;
      if(extraInfo != null) {

        success = mZegoLiveRoom.startPublishing(streamID, title, flag, extraInfo);

      } else {

        success = mZegoLiveRoom.startPublishing(streamID, title, flag);
      }

      result.success(success);


    } else if (call.method.equals("stopPublishing")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean success = mZegoLiveRoom.stopPublishing();
      result.success(success);

    } else if(call.method.equals("setVideoCodecId")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int codecID = numberToIntValue((Number) call.argument("codecID"));
      boolean success = mZegoLiveRoom.setVideoCodecId(codecID, ZegoConstants.PublishChannelIndex.MAIN);
      result.success(success);

    } else if (call.method.equals("updateStreamExtraInfo")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      String extraInfo = call.argument("extraInfo");
      boolean success = mZegoLiveRoom.updateStreamExtraInfo(extraInfo);
      result.success(success);

    } else if (call.method.equals("enableMic")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      boolean success = mZegoLiveRoom.enableMic(enable);
      result.success(success);

    } else if (call.method.equals("enableCamera")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      boolean success = mZegoLiveRoom.enableCamera(enable);
      result.success(success);

    } else if (call.method.equals("setFrontCam")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      boolean success = mZegoLiveRoom.setFrontCam(enable);
      result.success(success);

    } else if (call.method.equals("setAVConfig")) {

      if(mZegoLiveRoom == null) {
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

    } else if (call.method.equals("enableBeautifying")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int feature = numberToIntValue((Number) call.argument("feature"));

      boolean success = mZegoLiveRoom.enableBeautifying(feature);
      result.success(success);

    } else if (call.method.equals("setPolishStep")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      float step = numberToFloatValue((Number) call.argument("step"));
      boolean success = mZegoLiveRoom.setPolishStep(step);
      result.success(success);

    } else if (call.method.equals("setPolishFactor")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      float factor = numberToFloatValue((Number) call.argument("factor"));
      boolean success = mZegoLiveRoom.setPolishFactor(factor, 0);
      result.success(success);

    } else if (call.method.equals("setWhitenFactor")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      float factor = numberToFloatValue((Number) call.argument("factor"));
      boolean success = mZegoLiveRoom.setWhitenFactor(factor);
      result.success(success);

    } else if(call.method.equals("setFilter")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int filter = numberToIntValue((Number) call.argument("filter"));
      boolean success = mZegoLiveRoom.setFilter(filter);
      result.success(success);

    } else if (call.method.equals("enableTrafficControl")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      int properties = numberToIntValue((Number) call.argument("properties"));

      mZegoLiveRoom.enableTrafficControl(properties, enable);
      result.success(null);

    } else if(call.method.equals("setMinVideoBitrateForTrafficControl")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int bitrate = numberToIntValue((Number) call.argument("bitrate"));
      int mode = numberToIntValue((Number) call.argument("mode"));

      mZegoLiveRoom.setMinVideoBitrateForTrafficControl(bitrate, mode);
      result.success(null);

    } else if (call.method.equals("createPreviewRenderer")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(mIsEnablePlatformView) {
        throwNoTextureError(result, call.method);
        return;
      }

      int width = numberToIntValue((Number) call.argument("width"));
      int height = numberToIntValue((Number) call.argument("height"));


      if(mRenders.containsKey(mPublishMainChl)) {
        result.success(-1);
        return;
      }

      ZegoViewRenderer renderer = new ZegoViewRenderer(textures.createSurfaceTexture(), width, height);

      mRenders.put(mPublishMainChl, renderer);
      mZegoLiveRoom.setPreviewView(renderer.getSurface());
      result.success(renderer.getTextureID());

    } else if (call.method.equals("setPreviewViewMode")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(mIsEnablePlatformView) {
        throwNoTextureError(result, call.method);
        return;
      }

      if(!mRenders.containsKey(mPublishMainChl)) {
        throwNoRendererError(result, call.method);
        return;
      }

      int mode = numberToIntValue((Number) call.argument("mode"));
      mZegoLiveRoom.setPreviewViewMode(mode);
      result.success(true);

    } else if (call.method.equals("updatePreviewRenderSize")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(mIsEnablePlatformView) {
        throwNoTextureError(result, call.method);
        return;
      }

      int width = numberToIntValue((Number) call.argument("width"));
      int height = numberToIntValue((Number) call.argument("height"));

      ZegoViewRenderer renderer = mRenders.get(mPublishMainChl);
      if(renderer == null) {
        throwNoRendererError(result, call.method);
        return;
      }

      boolean success = renderer.updateRenderSize(width, height);
      if(success) {
        mZegoLiveRoom.setPreviewView(null);
        mZegoLiveRoom.setPreviewView(renderer.getSurface());
      }

      result.success(success);

    } else if (call.method.equals("destroyPreviewRenderer")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(mIsEnablePlatformView) {
        throwNoTextureError(result, call.method);
        return;
      }

      if(!mRenders.containsKey(mPublishMainChl)) {
        result.success(false);
        return;
      }

      ZegoViewRenderer renderer = mRenders.get(mPublishMainChl);
      if(renderer != null) {

        mZegoLiveRoom.setPreviewView(null);
        renderer.release();
        result.success(true);

      } else {

        result.success(false);
      }

      mRenders.remove(mPublishMainChl);

    } else if(call.method.equals("setPreviewView")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(!mIsEnablePlatformView) {
        throwNoPlatformViewError(result, call.method);
        return;
      }

      int viewID = numberToIntValue((Number) call.argument("viewID"));
      ZegoPlatformView view = ZegoPlatformViewFactory.shareInstance().getPlatformView(viewID);
      if(view != null) {
        int width = view.getSurfaceView().getHolder().getSurfaceFrame().width();
        int height = view.getSurfaceView().getHolder().getSurfaceFrame().height();
        if(width == 0 && height == 0) {
          reportInnerError("[setPreviewView UnexpectedException] view size is zero");
        }
        ZegoLogJNI.logNotice("[SetPreviewView - ZegoPlatformView] view size: " + "(" + width + ", " + height + ")" + "viewID:" + viewID);
      }else {
        ZegoLogJNI.logNotice("[SetPreviewView - ZegoPlatformView] no such view");
      }
      if(view == null) {
        result.success(false);
        return;
      }

      boolean success = mZegoLiveRoom.setPreviewView(view.getSurfaceView());
      result.success(success);

    } else if(call.method.equals("setPlatformViewPreviewViewMode")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(!mIsEnablePlatformView) {
        throwNoPlatformViewError(result, call.method);
        return;
      }

      int mode = numberToIntValue((Number) call.argument("mode"));
      boolean success = mZegoLiveRoom.setPreviewViewMode(mode);

      result.success(success);

    } else if(call.method.equals("removePreviewPlatformView")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(!mIsEnablePlatformView) {
        throwNoPlatformViewError(result, call.method);
        return;
      }

      int viewID = numberToIntValue((Number) call.argument("viewID"));
      boolean success = ZegoPlatformViewFactory.shareInstance().removeView(viewID);

      result.success(success);

    } else if(call.method.equals("setAppOrientation")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int orientation = numberToIntValue((Number) call.argument("orientation"));

      boolean success = mZegoLiveRoom.setAppOrientation(orientation);
      result.success(success);

    } else if(call.method.equals("setPublishConfig")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      HashMap<String, Object> config = new HashMap<>();
      config.put(ZegoConstants.PublishConfig.PUBLISH_CUSTOM_TARGET, call.argument("rtmpURL"));
      mZegoLiveRoom.setPublishConfig(config);

      result.success(true);

    } else if(call.method.equals("addPublishTarget")) {

      if(mZegoLiveRoom == null) {
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

    } else if(call.method.equals("deletePublishTarget")) {

      if(mZegoLiveRoom == null) {
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

    } else if(call.method.equals("respondJoinLiveReq")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int seq = numberToIntValue((Number) call.argument("seq"));
      int rspResult = numberToIntValue((Number) call.argument("rspResult"));

      boolean success = mZegoLiveRoom.respondJoinLiveReq(seq, rspResult);
      result.success(success);

    } else if(call.method.equals("inviteJoinLive")) {

      if(mZegoLiveRoom == null) {
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

    } else if(call.method.equals("endJoinLive")) {

      if(mZegoLiveRoom == null) {
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

    } else if(call.method.equals("setAudioBitrate")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int bitrate = numberToIntValue((Number) call.argument("bitrate"));

      boolean success = mZegoLiveRoom.setAudioBitrate(bitrate);
      result.success(success);

    } else if(call.method.equals("enableAEC")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      mZegoLiveRoom.enableAEC(enable);

      result.success(true);

    } else if(call.method.equals("setAECMode")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int mode = numberToIntValue((Number) call.argument("mode"));
      mZegoLiveRoom.setAECMode(mode);
      result.success(null);

    } else if(call.method.equals("enableAGC")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      mZegoLiveRoom.enableAGC(enable);

      result.success(true);

    } else if(call.method.equals("enableVAD")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      mZegoLiveRoom.enableVAD(enable);
      result.success(null);

    } else if(call.method.equals("enableANS")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      boolean success = mZegoLiveRoom.enableNoiseSuppress(enable);

      result.success(success);

    } else if(call.method.equals("enableDTX")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      mZegoLiveRoom.enableDTX(enable);
      result.success(null);
    }
    /* LiveRoom-Player */
    else if(call.method.equals("setViewMode")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(mIsEnablePlatformView) {
        throwNoTextureError(result, call.method);
        return;
      }

      String streamID = call.argument("streamID");
      int mode = numberToIntValue((Number) call.argument("mode"));

      boolean success = mZegoLiveRoom.setViewMode(mode, streamID);
      result.success(success);

    } else if (call.method.equals("startPlayingStream")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      String streamID = call.argument("streamID");
      HashMap<String, Object> info = call.argument("info");

      Surface surface = null;
      ZegoPlatformView view = null;

      if(mIsEnablePlatformView) {

        // 只有音视频场景下严格检查viewID的合法性，纯音频场景下允许不填viewID（viewID为空）
        if(call.argument("viewID") != null) {
          int viewID = numberToIntValue((Number) call.argument("viewID"));
          view = ZegoPlatformViewFactory.shareInstance().getPlatformView(viewID);
          if(view != null) {
            int width = view.getSurfaceView().getHolder().getSurfaceFrame().width();
            int height = view.getSurfaceView().getHolder().getSurfaceFrame().height();
            if(width == 0 && height == 0) {
                reportInnerError("[startPlayingStream UnexpectedException] view size is zero");
            }
            ZegoLogJNI.logNotice("[StartPlayingStream - ZegoPlatformView] view size: " + "(" + width + ", " + height + ")" + "viewID: " + viewID);
          } else {
            ZegoLogJNI.logNotice("[StartPlayingStream - ZegoPlatformView] no such view");
          }
          //传入错误的view id
          if(view == null) {
            result.success(false);
            return;
          }
        }

      } else {

        if(mRenders.containsKey(streamID)) {
          ZegoViewRenderer renderer = mRenders.get(streamID);
          surface = renderer.getSurface();
        }//else {

          // 这个位置不能return false，对于纯语音场景会走到这个分支中，不能被过滤掉
          // result.success(false);
          //return;
        //}
      }

      boolean success;


      if(info != null) {

        String params = (String)info.get("params");

        @SuppressWarnings("unchecked")
        ArrayList<String> rtmpUrls = (ArrayList<String>)info.get("rtmpUrls");
        @SuppressWarnings("unchecked")
        ArrayList<String> flvUrls = (ArrayList<String>)info.get("flvUrls");
        ZegoStreamExtraPlayInfo extraPlayInfo = new ZegoStreamExtraPlayInfo();
        extraPlayInfo.params = params;
        extraPlayInfo.rtmpUrls = (String[])rtmpUrls.toArray(new String[0]);
        extraPlayInfo.flvUrls =  (String[])flvUrls.toArray(new String[0]);

        success = mZegoLiveRoom.startPlayingStream(streamID, mIsEnablePlatformView ? view != null ? view.getSurfaceView() : null : surface, extraPlayInfo);
      } else {

        success = mZegoLiveRoom.startPlayingStream(streamID, mIsEnablePlatformView ? view != null ? view.getSurfaceView() : null : surface);
      }

      result.success(success);



    } else if (call.method.equals("stopPlayingStream")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      String streamID = call.argument("streamID");

      boolean success = mZegoLiveRoom.stopPlayingStream(streamID);
      result.success(success);

    } else if(call.method.equals("activateAudioPlayStream")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      String streamID = call.argument("streamID");
      boolean active = numberToBoolValue((Boolean) call.argument("active"));

      int errorCode = mZegoLiveRoom.activateAudioPlayStream(streamID, active);
      result.success(errorCode);

    } else if(call.method.equals("activateVideoPlayStream")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      String streamID = call.argument("streamID");
      boolean active = numberToBoolValue((Boolean) call.argument("active"));

      int errorCode;
      Number objVideoStreamLayer = call.argument("layer");
      if(objVideoStreamLayer != null) {

        int videoStreamLayer = numberToIntValue(objVideoStreamLayer);
        errorCode = mZegoLiveRoom.activateVideoPlayStream(streamID, active, videoStreamLayer);
      } else {

        errorCode = mZegoLiveRoom.activateVideoPlayStream(streamID, active);
      }

      result.success(errorCode);

    } else if (call.method.equals("requireHardwareDecoder")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean require = numberToBoolValue((Boolean) call.argument("bRequire"));
      boolean success = ZegoLiveRoom.requireHardwareDecoder(require);
      result.success(success);

    } else if (call.method.equals("createPlayViewRenderer")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(mIsEnablePlatformView) {
        throwNoTextureError(result, call.method);
        return;
      }

      String streamID = call.argument("streamID");
      int width = numberToIntValue((Number) call.argument("width"));
      int height = numberToIntValue((Number) call.argument("height"));

      if(mRenders.containsKey(streamID)) {
        result.success(-1);
        return;
      }

      ZegoViewRenderer renderer = new ZegoViewRenderer(textures.createSurfaceTexture(), width, height);
      mRenders.put(streamID, renderer);

      result.success(renderer.getTextureID());

    } else if (call.method.equals("updatePlayViewRenderSize")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(mIsEnablePlatformView) {
        throwNoTextureError(result, call.method);
        return;
      }

      String streamID = call.argument("streamID");
      int width = numberToIntValue((Number) call.argument("width"));
      int height = numberToIntValue((Number) call.argument("height"));

      if(!mRenders.containsKey(streamID)) {
        result.success(false);
        return;
      }


      ZegoViewRenderer renderer = mRenders.get(streamID);
      if(renderer == null) {
        throwNoRendererError(result, call.method);
        return;
      }

      boolean success = renderer.updateRenderSize(width, height);
      if (success) {

        mZegoLiveRoom.updatePlayView(streamID, null);
        mZegoLiveRoom.updatePlayView(streamID, renderer.getSurface());
      }

      result.success(success);

    } else if (call.method.equals("destroyPlayViewRenderer")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(mIsEnablePlatformView) {
        throwNoTextureError(result, call.method);
        return;
      }

      String streamID = call.argument("streamID");

      if(!mRenders.containsKey(streamID)) {
        result.success(false);
        return;
      }

      ZegoViewRenderer renderer = mRenders.get(streamID);

      if(renderer != null) {

        //先停止渲染
        mZegoLiveRoom.updatePlayView(streamID, null);

        renderer.release();
        result.success(true);
      } else {

          result.success(false);
      }

      mRenders.remove(streamID);

    } else if(call.method.equals("updatePlayView")) {

        if(mZegoLiveRoom == null) {
            throwSdkNotInitError(result, call.method);
            return;
        }

        if(!mIsEnablePlatformView) {
            throwNoPlatformViewError(result, call.method);
            return;
        }

        int viewID = numberToIntValue((Number) call.argument("viewID"));
        String streamID = call.argument("streamID");

        ZegoPlatformView view = ZegoPlatformViewFactory.shareInstance().getPlatformView(viewID);
        if(view == null) {
          ZegoLogJNI.logNotice("[UpdatePlayView - ZegoPlatformView] no such view");
          result.success(false);
          return;
        }

        mZegoLiveRoom.updatePlayView(streamID, view.getSurfaceView());
        result.success(null);

    } else if(call.method.equals("setPlatformViewPlayViewMode")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(!mIsEnablePlatformView) {
        throwNoPlatformViewError(result, call.method);
        return;
      }

      String streamID = call.argument("streamID");
      int mode = numberToIntValue((Number) call.argument("mode"));

      boolean success = mZegoLiveRoom.setViewMode(mode, streamID);
      result.success(success);


    } else if(call.method.equals("removePlayPlatformView")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      if(!mIsEnablePlatformView) {
        throwNoPlatformViewError(result, call.method);
        return;
      }

      int viewID = numberToIntValue((Number) call.argument("viewID"));
      boolean success = ZegoPlatformViewFactory.shareInstance().removeView(viewID);
      result.success(success);

    } else if(call.method.equals("requestJoinLive")) {

      if(mZegoLiveRoom == null) {
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

    } else if(call.method.equals("respondInviteJoinLiveReq")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      int seq = numberToIntValue((Number) call.argument("seq"));
      int rspResult = numberToIntValue((Number) call.argument("rspResult"));

      boolean success = mZegoLiveRoom.respondInviteJoinLiveReq(seq, rspResult);
      result.success(success);

    } else if(call.method.equals("enableSpeaker")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));

      boolean success = mZegoLiveRoom.enableSpeaker(enable);
      result.success(success);

    } else if(call.method.equals("setBuiltInSpeakerOn")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean on = numberToBoolValue((Boolean) call.argument("bOn"));

      boolean success = mZegoLiveRoom.setBuiltInSpeakerOn(on);
      result.success(success);

    }
    /* Media Side Info */
    else if (call.method.equals("setMediaSideFlags")) {

      if(mZegoLiveRoom == null || mZegoMediaSideInfo == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean start = numberToBoolValue((Boolean) call.argument("start"));
      boolean onlyAudioPublish = numberToBoolValue((Boolean) call.argument("onlyAudioPublish"));

      mZegoMediaSideInfo.setMediaSideFlags(start, onlyAudioPublish, ZegoConstants.PublishChannelIndex.MAIN);
      result.success(null);

    } else if (call.method.equals("sendMediaSideInfo")) {

      if(mZegoLiveRoom == null || mZegoMediaSideInfo == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      String data = call.argument("data");

      ByteBuffer tmpData = ByteBuffer.wrap(data.getBytes());

      ByteBuffer inData = ByteBuffer.allocateDirect(tmpData.capacity());
      for(int i = 0; i < inData.capacity(); i++)
        inData.put(i, tmpData.get(i));

      mZegoMediaSideInfo.sendMediaSideInfo(inData, inData.capacity(), false, ZegoConstants.PublishChannelIndex.MAIN);
      result.success(null);

    }
    /* Sound Level */
    else if(call.method.equals("registerSoundLevelCallback")) {

      ZegoSoundLevelMonitor.getInstance().setCallback(new IZegoSoundLevelCallback() {
        @Override
        public void onSoundLevelUpdate(ZegoSoundLevelInfo[] zegoSoundLevelInfos) {
          if(mEventSink != null) {

            ArrayList<HashMap<String, Object>> soundLevelList = new ArrayList<>();
            for(ZegoSoundLevelInfo info : zegoSoundLevelInfos) {
              HashMap<String, Object> objInfo = new HashMap<>();
              objInfo.put("streamID", info.streamID);
              objInfo.put("soundLevel", info.soundLevel);

              soundLevelList.add(objInfo);
            }

            HashMap<String, Object> returnMap = new HashMap<>();
            returnMap.put("type", EVENT_TYPE.TYPE_SOUND_LEVEL_EVENT.ordinal());

            HashMap<String, Object> method = new HashMap<>();
            method.put("name", "onSoundLevelUpdate");
            method.put("soundLevels", soundLevelList);

            returnMap.put("method", method);
            mEventSink.success(returnMap);
          }
        }

        @Override
        public void onCaptureSoundLevelUpdate(ZegoSoundLevelInfo zegoSoundLevelInfo) {
          if(mEventSink != null) {

            HashMap<String, Object> returnMap = new HashMap<>();
            returnMap.put("type", EVENT_TYPE.TYPE_SOUND_LEVEL_EVENT.ordinal());

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

    } else if(call.method.equals("unregisterSoundLevelCallback")) {

      ZegoSoundLevelMonitor.getInstance().setCallback(null);
      result.success(true);

    } else if(call.method.equals("setSoundLevelMonitorCycle")) {

      int ms = numberToIntValue((Number) call.argument("ms"));
      boolean success = ZegoSoundLevelMonitor.getInstance().setCycle(ms);
      result.success(success);

    } else if(call.method.equals("startSoundLevelMonitor")) {

      boolean success = ZegoSoundLevelMonitor.getInstance().start();
      result.success(success);

    } else if(call.method.equals("stopSoundLevelMonitor")) {

      boolean success = ZegoSoundLevelMonitor.getInstance().stop();
      result.success(success);

    }
    /* LiveRoom-AudioIO*/
    else if(call.method.equals("enableAECWhenHeadsetDetected")) {

      if(mZegoLiveRoom == null) {
        throwSdkNotInitError(result, call.method);
        return;
      }

      boolean enable = numberToBoolValue((Boolean) call.argument("enable"));
      mZegoLiveRoom.enableAECWhenHeadsetDetected(enable);
      result.success(null);

    }
    /* Error Code */
    else if(call.method.equals("isInitSDKError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isInitSDKError(error);
        result.success(ret);

    } else if(call.method.equals("isNotLoginError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isNotLoginError(error);
        result.success(ret);

    } else if(call.method.equals("isMediaServerNetWorkError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isMediaServerNetWorkError(error);
        result.success(ret);

    } else if(call.method.equals("isLogicServerNetWorkError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isLogicServerNetWrokError(error);
        result.success(ret);

    } else if(call.method.equals("isMixStreamNotExistError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isMixStreamNotExistError(error);
        result.success(ret);

    } else if(call.method.equals("isPlayStreamNotExistError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isPlayStreamNotExistError(error);
        result.success(ret);

    } else if(call.method.equals("isPublishBadNameError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isPublishBadNameError(error);
        result.success(ret);

    } else if(call.method.equals("isPublishForbidError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isPublishForbidError(error);
        result.success(ret);

    } else if(call.method.equals("isPublishStopError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isPublishStopError(error);
        result.success(ret);

    } else if(call.method.equals("isPublishDeniedError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isPublishDeniedError(error);
        result.success(ret);

    } else if(call.method.equals("isPlayForbidError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isPlayForbidError(error);
        result.success(ret);

    } else if(call.method.equals("isPlayDeniedError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isPlayDeniedError(error);
        result.success(ret);

    } else if(call.method.equals("isDNSResolveError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isDNSResolveError(error);
        result.success(ret);

    } else if(call.method.equals("isNetworkUnreachError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isNetworkUnreachError(error);
        result.success(ret);

    } else if(call.method.equals("isHttpProtocolError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isHttpProtocolError(error);
        result.success(ret);

    } else if(call.method.equals("isReqFrequencyLimitError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isReqFrequencyLimitError(error);
        result.success(ret);

    } else if(call.method.equals("isLiveRoomError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isLiveRoomError(error);
        result.success(ret);

    } else if(call.method.equals("isMultiLoginError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isMultiLoginError(error);
        result.success(ret);

    } else if(call.method.equals("isManualKickoutError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isManualKickoutError(error);
        result.success(ret);

    } else if(call.method.equals("isRoomSessionError")) {

        int error = numberToIntValue((Number) call.argument("error"));
        boolean ret = ZegoError.isRoomSessionError(error);
        result.success(ret);

    } else if(call.method.equals("getErrorMsg")) {

        int error = numberToIntValue((Number) call.argument("error"));
        String msg = ZegoError.getErrorMsg(error);
        result.success(msg);

    } else if(call.method.equals("addNoticeLog")) {

      String content = call.argument("content");
      ZegoLogJNI.logNotice(content);

      result.success(null);
    }
    else{

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
    if(mEventSink != null) {
      HashMap<String, Object> returnMap = new HashMap<>();
      returnMap.put("type", EVENT_TYPE.TYPE_ROOM_EVENT.ordinal());

      HashMap<String, Object> method = new HashMap<>();
      method.put("name", "onInnerError");
      method.put("message", message);

      returnMap.put("method", method);
      mEventSink.success(returnMap);
    }
  }

  //Handle Flutter CallMethods

  public void initSDKWithAppID(long appID, String strAppSign, final Result result) {

    if(mZegoLiveRoom == null)
      mZegoLiveRoom = new ZegoLiveRoom();

    //调用其他API前必须调用该函数
    ZegoLiveRoom.setSDKContext(new ZegoLiveRoom.SDKContext() {

      @Override
      public Application getAppContext() {
        return (Application)mContext;
      }

      @Override
      public String getLogPath() {
        return null;
      }

      @Override
      public String getSoFullPath() {
        return null;
      }
    });

    ZegoLogJNI.logNotice("[Flutter-Native] enter init sdk, app id: " + appID);

      mZegoLiveRoom.setZegoRoomCallback(new IZegoRoomCallback() {
          @Override
          public void onStreamUpdated(int type, ZegoStreamInfo[] zegoStreamInfos, String roomID) {
              ZegoLogJNI.logNotice("[Flutter-Native] onStreamUpdate enter, sink: " + mEventSink);
              if(mEventSink != null) {

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
                  returnMap.put("type", EVENT_TYPE.TYPE_ROOM_EVENT.ordinal());

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
              if(mEventSink != null) {

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
                  returnMap.put("type", EVENT_TYPE.TYPE_ROOM_EVENT.ordinal());

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
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_ROOM_EVENT.ordinal());

                  HashMap<String, Object> method = new HashMap<>();
                  method.put("name", "onTempBroken");
                  method.put("roomID", roomID);
                  method.put("errorCode", errorCode);

                  returnMap.put("method", method);
                  mEventSink.success(returnMap);
              }
          }

          @Override
          public void onReconnect(int errorCode, String roomID) {
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_ROOM_EVENT.ordinal());

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
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_ROOM_EVENT.ordinal());

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
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_ROOM_EVENT.ordinal());

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
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_ROOM_EVENT.ordinal());

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
      });

      mZegoLiveRoom.setZegoIMCallback(new IZegoIMCallback() {
          @Override
          public void onUserUpdate(ZegoUserState[] zegoUserStates, int type) {
              if(mEventSink != null) {

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
                  returnMap.put("type", EVENT_TYPE.TYPE_ROOM_EVENT.ordinal());

                  HashMap<String, Object> method = new HashMap<>();
                  method.put("name", "onUserUpdate");
                  method.put("updateType", type);
                  method.put("userList", userList);

                  returnMap.put("method", method);
                  mEventSink.success(returnMap);
              }
          }

          @Override
          public void onRecvRoomMessage(String s, ZegoRoomMessage[] zegoRoomMessages) {

          }

          @Override
          public void onUpdateOnlineCount(String s, int i) {

          }

          @Override
          public void onRecvBigRoomMessage(String s, ZegoBigRoomMessage[] zegoBigRoomMessages) {

          }
      });

      mZegoLiveRoom.setZegoLivePublisherCallback(new IZegoLivePublisherCallback() {
          @Override
          public void onPublishStateUpdate(int stateCode, String streamID, HashMap<String, Object> streamInfo) {
              ZegoLogJNI.logNotice("[Flutter-Native] onPublishStateUpdate enter, sink: " + mEventSink);
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PUBLISH_EVENT.ordinal());

                  HashMap<String, Object> method = new HashMap<>();
                  method.put("name", "onPublishStateUpdate");
                  method.put("stateCode", stateCode);
                  method.put("streamID", streamID);

                  HashMap<String, Object> info = new HashMap<>();
                  streamInfo.put("streamID", streamInfo.get(ZegoConstants.StreamKey.STREAM_ID));
                  streamInfo.put("rtmpList", streamInfo.get(ZegoConstants.StreamKey.RTMP_URL_LIST));
                  streamInfo.put("flvList", streamInfo.get(ZegoConstants.StreamKey.FLV_URL_LIST));
                  streamInfo.put("hlsList", streamInfo.get(ZegoConstants.StreamKey.HLS_URL_LST));
                  method.put("streamInfo", info);

                  returnMap.put("method", method);
                  ZegoLogJNI.logNotice("[Flutter-Native] onPublishStateUpdate, return map: " + returnMap);
                  mEventSink.success(returnMap);
              }
          }

          @Override
          public void onJoinLiveRequest(int seq, String fromUserID, String fromUserName, String roomID) {
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PUBLISH_EVENT.ordinal());

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

              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PUBLISH_EVENT.ordinal());

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

                  method.put("width", zegoStreamQuality.width);
                  method.put("height", zegoStreamQuality.height);

                  method.put("quality", zegoStreamQuality.quality);

                  returnMap.put("method", method);
                  mEventSink.success(returnMap);
              }
          }

          @Deprecated
          @Override
          public AuxData onAuxCallback(int i) {
              return null;
          }

          @Override
          public void onCaptureVideoSizeChangedTo(int width, int height) {
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PUBLISH_EVENT.ordinal());

                  HashMap<String, Object> method = new HashMap<>();
                  method.put("name", "onCaptureVideoSizeChangedTo");
                  method.put("width", width);
                  method.put("height", height);

                  returnMap.put("method", method);
                  mEventSink.success(returnMap);
              }
          }

          @Deprecated
          @Override
          public void onMixStreamConfigUpdate(int i, String s, HashMap<String, Object> hashMap) {

          }

          @Override
          public void onCaptureVideoFirstFrame() {
              if(mEventSink != null) {
                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PUBLISH_EVENT.ordinal());

                  HashMap<String, Object> method = new HashMap<>();
                  method.put("name", "onCaptureVideoFirstFrame");

                  returnMap.put("method", method);
                  mEventSink.success(returnMap);
              }

          }

          @Override
          public void onCaptureAudioFirstFrame() {
              if(mEventSink != null) {
                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PUBLISH_EVENT.ordinal());

                  HashMap<String, Object> method = new HashMap<>();
                  method.put("name", "onCaptureAudioFirstFrame");

                  returnMap.put("method", method);
                  mEventSink.success(returnMap);
              }
          }
      });

      mZegoLiveRoom.setZegoLivePublisherExCallback(new IZegoLivePublisherExCallback() {
        @Override
        public void onRelayCDNStateUpdate(ZegoStreamRelayCDNInfo[] stateInfo, String streamID) {
          ZegoLogJNI.logNotice("[Flutter-Native] onRelayCDNStateUpdate enter, sink: " + mEventSink);
          if(mEventSink != null) {
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
            returnMap.put("type", EVENT_TYPE.TYPE_PUBLISH_EVENT.ordinal());

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
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PLAY_EVENT.ordinal());

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

              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PLAY_EVENT.ordinal());

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
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PLAY_EVENT.ordinal());

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
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PLAY_EVENT.ordinal());

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

              if(mEventSink != null) {
                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PLAY_EVENT.ordinal());

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

              if(mEventSink != null) {
                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PLAY_EVENT.ordinal());

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

              if(mEventSink != null) {
                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PLAY_EVENT.ordinal());

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
          public void onRecvRemoteAudioFirstFrame(String streamID) {

              if(mEventSink != null) {
                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PLAY_EVENT.ordinal());

                  HashMap<String, Object> method = new HashMap<>();
                  method.put("name", "onRecvRemoteAudioFirstFrame");
                  method.put("streamID", streamID);

                  returnMap.put("method", method);
                  mEventSink.success(returnMap);
              }
          }

          @Override
          public void onRecvRemoteVideoFirstFrame(String streamID) {

              if(mEventSink != null) {
                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PLAY_EVENT.ordinal());

                  HashMap<String, Object> method = new HashMap<>();
                  method.put("name", "onRecvRemoteVideoFirstFrame");
                  method.put("streamID", streamID);

                  returnMap.put("method", method);
                  mEventSink.success(returnMap);
              }
          }

          @Override
          public void onRenderRemoteVideoFirstFrame(String streamID) {

              if(mEventSink != null) {
                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_PLAY_EVENT.ordinal());

                  HashMap<String, Object> method = new HashMap<>();
                  method.put("name", "onRenderRemoteVideoFirstFrame");
                  method.put("streamID", streamID);

                  returnMap.put("method", method);
                  mEventSink.success(returnMap);
              }
          }
      });

      mZegoLiveRoom.setZegoLiveEventCallback(new IZegoLiveEventCallback() {
          @Override
          public void onLiveEvent(int event, HashMap<String, String> info) {
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_ROOM_EVENT.ordinal());

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
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_ROOM_EVENT.ordinal());

                  HashMap<String, Object> method = new HashMap<>();
                  method.put("name", "onAVEngineStart");

                  returnMap.put("method", method);
                  mEventSink.success(returnMap);
              }
          }

          @Override
          public void onAVEngineStop() {
              if(mEventSink != null) {

                  HashMap<String, Object> returnMap = new HashMap<>();
                  returnMap.put("type", EVENT_TYPE.TYPE_ROOM_EVENT.ordinal());

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
      public void onInitSDK(int i) {

        ZegoLogJNI.logNotice("[Flutter-Native] on init sdk, errorCode: " + i);
        result.success(i);
      }
    });

    if(!success)
      result.success(false);

    mZegoMediaSideInfo = new ZegoMediaSideInfo();
      mZegoMediaSideInfo.setZegoMediaSideCallback(new IZegoMediaSideCallback() {
          @Override
          public void onRecvMediaSideInfo(String streamID, ByteBuffer data, int dataLen) {

              if(mEventSink != null) {

                  if (dataLen == 0) {
                      return;
                  }

                  int mediaType = (data.get(0) & 0xFF) << 24 | (data.get(1) & 0xFF) << 16 | (data.get(2) & 0xFF) << 8 | (data.get(3) & 0xFF);

                  byte[] tempBuffer;
                  if(mediaType == 1001 || mediaType == 1002) {

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
                  returnMap.put("type", EVENT_TYPE.TYPE_MEDIA_SIDE_INFO_EVENT.ordinal());

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
