//
//  ZegoRendererController.m
//  connectivity
//
//  Created by 李展鹏@ZEGO on 2018/11/13.
//

#import "ZegoRendererController.h"
#import <mutex>

@implementation ZegoPixelBufferPool

- (instancetype)initWithPool:(CVPixelBufferPoolRef)pool width:(int)width height:(int)height {
    self = [super init];
    if(self) {
        _pool = pool;
        _width = width;
        _height = height;
    }
    
    return self;
}

- (CVPixelBufferPoolRef)getBufferPool {
    return _pool;
}

- (int)getWidth {
    return _width;
}

- (int)getHeight {
    return _height;
}

@end

@interface ZegoRendererController()

@property (nonatomic, assign) BOOL isRendering;
@property (strong) NSMutableDictionary<NSString *, ZegoViewRenderer *> *renders;
@property (strong) NSMutableDictionary<NSString * , id>* pools;
@property(readonly, nonatomic) CADisplayLink* displayLink;

@end

@implementation ZegoRendererController
{
    std::mutex mutex;
}

-(instancetype) init {
    self = [super init];
    if(self) {
        _renders = [[NSMutableDictionary alloc] init];
        _pools = [[NSMutableDictionary alloc] init];

        _isRendering = NO;
        _isUseFrontCam = YES;
        
    }
    
    return self;
}

- (BOOL)addRenderer:(ZegoViewRenderer *)renderer ofKey:(NSString *)streamID {
    if([self isStreamExist:streamID])
        return NO;
    
    [self.renders setObject:renderer forKey:streamID];
    return YES;
}

- (BOOL)removeRenderer:(NSString *)streamID {
    if(![self isStreamExist:streamID])
        return NO;
    
    ZegoViewRenderer *renderer = [self.renders objectForKey:streamID];
    [renderer releaseRenderer:nil];
    [self.renders removeObjectForKey:streamID];
    
    return YES;
}

- (void)removeAllRenderer {
    for(NSString *key in self.renders) {
        ZegoViewRenderer *renderer = [self.renders objectForKey:key];
        [renderer releaseRenderer:nil];
    }
    
    [self.renders removeAllObjects];
}

- (ZegoViewRenderer *)getRenderer:(NSString *)streamID {
    if(![self isStreamExist:streamID])
        return nil;
    
    return [self.renders objectForKey:streamID];
}

- (int)getRenderCount {
    return (int)[self.renders count];
}

- (void)startRendering {
    if(self.isRendering)
        return;
    
    _displayLink = [CADisplayLink displayLinkWithTarget:self
                                               selector:@selector(onDisplayLink:)];
    [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    // 30帧刷新
    _displayLink.frameInterval = 2;
    _displayLink.paused = NO;
    
    self.isRendering = YES;
}

- (void)stopRendering {
    if(!self.isRendering)
        return;
    
    self.displayLink.paused = YES;
    [_displayLink invalidate];
    
    self.isRendering = NO;
}


- (BOOL)createPixelBufferPool:(int)width height:(int)height streamID:(NSString *)streamID {
    

    if([self isContainStringKey:self.pools ofKey:streamID]) {
        //如果当前流的内存池已存在，则重新创建
        ZegoPixelBufferPool *pool = [self getPixelBufferPool:streamID];
        if(pool != nil) {
            
            CVPixelBufferPoolRef pool_ = [pool getBufferPool];
            CVPixelBufferPoolFlushFlags flag = 0;
            CVPixelBufferPoolFlush(pool_, flag);
            CFRelease(pool_);
        }
        
        {
            std::lock_guard<std::mutex> lock(mutex);
            [self.pools removeObjectForKey:streamID];
        }
        
    }
    
    NSDictionary *pixelBufferAttributes = [NSDictionary dictionaryWithObjectsAndKeys:
                                           [NSNumber numberWithBool:YES], (id)kCVPixelBufferOpenGLCompatibilityKey,
                                           [NSNumber numberWithInt:width], (id)kCVPixelBufferWidthKey,
                                           [NSNumber numberWithInt:height], (id)kCVPixelBufferHeightKey,
                                           [NSDictionary dictionary], (id)kCVPixelBufferIOSurfacePropertiesKey,
                                           [NSNumber numberWithInt:kCVPixelFormatType_32BGRA], (id)kCVPixelBufferPixelFormatTypeKey,
                                           nil
                                           ];
    CVPixelBufferPoolRef pool_;
    CFDictionaryRef ref = (__bridge CFDictionaryRef)pixelBufferAttributes;
    CVReturn ret = CVPixelBufferPoolCreate(nil, nil, ref, &pool_);
    if (ret != kCVReturnSuccess) {
        return NO;
    }
    
    ZegoPixelBufferPool *objPool = [[ZegoPixelBufferPool alloc] initWithPool:pool_ width:width height:height];
    
    {
        std::lock_guard<std::mutex> lock(mutex);
        [self.pools setObject:objPool forKey:streamID];
    }
    
    return YES;
}

- (BOOL)destroyPixelBufferPool:(NSString *)streamID{
    
    if(![self isContainStringKey:self.pools ofKey:streamID])
        return NO;
    
    ZegoPixelBufferPool *objPool = [self getPixelBufferPool:streamID];
    if(objPool == nil)
        return NO;
    
    CVPixelBufferPoolRef pool_ = [objPool getBufferPool];
    CVPixelBufferPoolFlushFlags flag = 0;
    CVPixelBufferPoolFlush(pool_, flag);
    CFRelease(pool_);
    
    {
        std::lock_guard<std::mutex> lock(mutex);
        [self.pools removeObjectForKey:streamID];
    }
    
    return YES;
}

- (void)destroyAllPixelBufferPool {
    
    
    for(NSString *key in self.pools) {
        ZegoPixelBufferPool *objPool = [self getPixelBufferPool:key];
        if(objPool == nil)
            continue;
        
        CVPixelBufferPoolRef pool_ = [objPool getBufferPool];
        CVPixelBufferPoolFlushFlags flag = 0;
        CVPixelBufferPoolFlush(pool_, flag);
        CFRelease(pool_);
    }
    
    {
        std::lock_guard<std::mutex> lock(mutex);
        [self.pools removeAllObjects];
    }
}

- (ZegoPixelBufferPool *)getPixelBufferPool:(NSString *)streamID {
    if(![self isContainStringKey:self.pools ofKey:streamID])
        return nil;
    
    std::lock_guard<std::mutex> lock(mutex);
    return [self.pools objectForKey:streamID];
}

- (NSDictionary *)getRenderers {
    return self.renders;
}
#pragma mark - private method

- (BOOL)isStreamExist:(NSString *)streamID {
    for(NSString *key in self.renders) {
        if([key isEqualToString:streamID])
            return YES;
    }
    
    return NO;
}

- (BOOL)isContainNumberKey:(NSMutableDictionary *)dic ofKey:(NSNumber *)currentKey {
    for(NSNumber *key in dic) {
        if(key.intValue == currentKey.intValue) {
            return YES;
        }
    }
    
    return NO;
}

- (BOOL)isContainStringKey:(NSMutableDictionary *)dic ofKey:(NSString *)currentKey {
    for(NSString *key in dic) {
        if(key.intValue == currentKey.intValue) {
            return YES;
        }
    }
    
    return NO;
}

- (void)onDisplayLink:(CADisplayLink*)link {
    
    if([self.renders count] == 0)
        return;
    
    for(NSString *key in self.renders) {
        ZegoViewRenderer *renderer = [self.renders objectForKey:key];
        if(renderer == nil)
            continue;
        
        if(![renderer isNewFrameAvailable])
            continue;
        
        [renderer notifyDrawNewFrame];
    }
}

@end
