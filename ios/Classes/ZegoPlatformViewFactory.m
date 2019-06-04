//
//  ZegoPlatformViewFactory.m
//  Pods
//
//  Created by 李展鹏@ZEGO on 2019/5/30.
//

#import "ZegoPlatformViewFactory.h"

static ZegoPlatformViewFactory *g_factory = nil;

@interface ZegoPlatformViewFactory()

@property (nonatomic, strong) NSMutableDictionary<NSNumber*, ZegoPlatformView*> *views;

@end

@implementation ZegoPlatformViewFactory

- (instancetype) init {
    self = [super init];
    if(self) {
        _views = [[NSMutableDictionary alloc] init];
        
    }
    return self;
}

+ (instancetype) shareInstance {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        g_factory = [[ZegoPlatformViewFactory alloc] init];
    });
    
    return g_factory;
}

+ (void)release {
    if(g_factory) {
        g_factory = nil;
    }
}

- (NSObject<FlutterPlatformView>*)createWithFrame:(CGRect)frame
                                   viewIdentifier:(int64_t)viewId
                                        arguments:(id _Nullable)args {
    
    ZegoPlatformView *view = [[ZegoPlatformView alloc] initWithRect:frame viewID:viewId];
    [self addView:view viewID:@(viewId)];
    
    return view;
}

- (BOOL)addView:(ZegoPlatformView *)view viewID:(NSNumber *)viewID {
    if(!viewID || !view)
        return NO;
    
    if(view) {
        [self.views setObject:view forKey:viewID];
    } else {
        [self.views removeObjectForKey:viewID];
    }
    return YES;
}

- (BOOL)removeView:(NSNumber *)viewID {
    if(!viewID)
        return NO;
    
    [self.views removeObjectForKey:viewID];
    return YES;
}

- (ZegoPlatformView *)getPlatformView:(NSNumber *) viewID {
    if(!viewID)
        return nil;
    
    return [self.views objectForKey:viewID];
}

@end
