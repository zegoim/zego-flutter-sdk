//
//  ZegoPlatformView.h
//  Pods
//
//  Created by 李展鹏@ZEGO on 2019/5/30.
//

#ifndef ZegoPlatformView_h
#define ZegoPlatformView_h

#import <Flutter/FlutterPlatformViews.h>

@interface ZegoPlatformView : NSObject<FlutterPlatformView>

- (instancetype)initWithRect:(CGRect)rect viewID:(int64_t) viewID;

- (UIView *)getUIView;

@end

#endif /* ZegoPlatformView_h */
