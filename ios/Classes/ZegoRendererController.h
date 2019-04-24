//
//  ZegoRendererController.h
//  Pods
//
//  Created by 李展鹏@ZEGO on 2018/11/13.
//

#ifndef ZegoRendererController_h
#define ZegoRendererController_h

#import <Foundation/Foundation.h>
#import "ZegoViewRenderer.h"

@interface ZegoPixelBufferPool : NSObject
{
    CVPixelBufferPoolRef _pool;
    int _width;
    int _height;
}

- (CVPixelBufferPoolRef)getBufferPool;
- (int)getWidth;
- (int)getHeight;

@end

@interface ZegoRendererController : NSObject

@property (nonatomic, assign) BOOL isUseFrontCam;

- (BOOL)addRenderer:(ZegoViewRenderer *)renderer ofKey:(NSString *)streamID;
- (BOOL)removeRenderer:(NSString *)streamID;
- (void)removeAllRenderer;
- (ZegoViewRenderer *)getRenderer:(NSString *)streamID;
- (int)getRenderCount;
- (NSDictionary *)getRenderers;

- (BOOL)createPixelBufferPool:(int)width height:(int)height streamID:(NSString* )streamID;
- (BOOL)destroyPixelBufferPool:(NSString *)streamID;
- (void)destroyAllPixelBufferPool;
- (ZegoPixelBufferPool *)getPixelBufferPool:(NSString *)streamID;

- (void)startRendering;
- (void)stopRendering;
- (BOOL)isRendering;

@end

#endif /* ZegoRendererController_h */
