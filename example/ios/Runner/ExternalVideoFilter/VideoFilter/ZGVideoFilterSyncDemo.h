//
//  ZGVideoFilterSyncDemo.h
//  LiveRoomPlayground-iOS
//
//  Created by Paaatrick on 2019/7/30.
//  Copyright © 2019 Zego. All rights reserved.
//



#import <Foundation/Foundation.h>

#if TARGET_OS_OSX
#import <ZegoLiveRoomOSX/zego-api-external-video-filter-oc.h>
#elif TARGET_OS_IOS
#import <ZegoLiveRoom/zego-api-external-video-filter-oc.h>
#endif

NS_ASSUME_NONNULL_BEGIN

/**
 同步类型外部滤镜实现
 */
@interface ZGVideoFilterSyncDemo : NSObject<ZegoVideoFilter, ZegoVideoFilterDelegate>

@end

NS_ASSUME_NONNULL_END
