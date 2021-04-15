//
//  ZGExternalVideoCaptureFactory.h
//  LiveRoomPlayGround
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ZegoLiveRoom/zego-api-external-video-capture-oc.h>

NS_ASSUME_NONNULL_BEGIN

/**
 示例 Demo 的外部视频采集 factory
 */
@interface ZGExternalVideoCaptureFactory : NSObject <ZegoVideoCaptureFactory>
/**
 初始化工厂
 
 @param device 抽象的采集设备
 */
- (instancetype)initWithDevice: (id<ZegoVideoCaptureDevice>)device;

@end

NS_ASSUME_NONNULL_END

