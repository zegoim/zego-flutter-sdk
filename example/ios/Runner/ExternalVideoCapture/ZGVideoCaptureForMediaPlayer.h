//
//  ZGVideoCaptureForMediaPlayer.h
//  LiveRoomPlayground
//
//  Copyright © 2018年 Zego. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ZegoLiveRoom/zego-api-mediaplayer-oc.h>
#import <ZegoLiveRoom/zego-api-external-video-capture-oc.h>
#import <zegoliveroom_plugin/ZegoMediaplayerController.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZGVideoCaptureForMediaPlayer : NSObject <ZegoVideoCaptureFactory, ZegoMediaPlayerControllerVideoDataDelegate>

@end

NS_ASSUME_NONNULL_END
