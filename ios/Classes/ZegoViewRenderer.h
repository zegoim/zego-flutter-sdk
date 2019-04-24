#import <Flutter/Flutter.h>
#import <OpenGLES/EAGL.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

typedef enum
{
    ZegoRendererScaleAspectFit = 0,    ///< 等比缩放，可能有黑边
    ZegoRendererScaleAspectFill = 1,   ///< 等比缩放填充整View，可能有部分被裁减
    ZegoRendererScaleToFill = 2,       ///< 填充整个View
}ZegoRendererViewMode;

@interface ZegoViewRenderer : NSObject <FlutterTexture>

@property (nonatomic, readonly) int64_t textureID;
@property (nonatomic) int view_width;
@property (nonatomic) int view_height;

- (void)setSrcFrameBuffer:(CVPixelBufferRef)srcFrameBuffer processBuffer:(CVPixelBufferRef)processBuffer;
- (void)setVideoMode:(ZegoRendererViewMode)mode;
- (void)setUseMirrorEffect:(BOOL)isUse;

- (BOOL)isNewFrameAvailable;
- (void)notifyDrawNewFrame;

- (CVPixelBufferRef)getPixelBuffer;
- (void)updateRenderSize:(CGSize)size;
- (instancetype)initWithTextureRegistry:(id<FlutterTextureRegistry>)registry isPublisher:(BOOL)isPublisher viewWidth:(int)width viewHeight:(int)height;

- (void)releaseRenderer:(void(^)(void))callback;


@end
