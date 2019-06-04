//
//  ZegoPlatformViewFactory.h
//  Pods
//
//  Created by 李展鹏@ZEGO on 2019/5/30.
//

#ifndef ZegoPlatformViewFactory_h
#define ZegoPlatformViewFactory_h

#import <Flutter/FlutterPlatformViews.h>
#import "ZegoPlatformView.h"

@interface ZegoPlatformViewFactory : NSObject<FlutterPlatformViewFactory>

+ (instancetype) shareInstance;
+ (void) release;

- (BOOL)addView:(ZegoPlatformView *)view viewID:(NSNumber *)viewID;
- (BOOL)removeView:(NSNumber *)viewID;
- (ZegoPlatformView *)getPlatformView:(NSNumber *) viewID;

@end

#endif /* ZegoPlatformViewFactory_h */
