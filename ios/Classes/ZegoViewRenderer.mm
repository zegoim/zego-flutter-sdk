#import "ZegoViewRenderer.h"
#import <mutex>
#import <ZegoLog.h>

@interface ZegoViewRenderer()

@property (nonatomic, weak) id<FlutterTextureRegistry> registry;
@property (nonatomic, assign) BOOL isPublisher;
@property (nonatomic, strong)EAGLContext *context;

@end

@implementation ZegoViewRenderer
{
    CVPixelBufferRef m_pInputFrameBuffer;
    CVPixelBufferRef m_pRenderFrameBuffer;
    CVPixelBufferRef m_pTempToCopyFrameBuffer;
    
    dispatch_queue_t  m_opengl_queue;
    GLfloat m_lstVertices[8];
    GLfloat m_lstTexCoord[8];
    CVOpenGLESTextureCacheRef m_pTexCache;
    GLuint m_hProgram;
    GLuint m_hVertexShader;
    GLuint m_hFragShader;
    GLuint m_framebuffer;
    int m_nFrameUniform;
    CVPixelBufferPoolRef m_buffer_pool;
    CVOpenGLESTextureRef m_output_texture;
    std::mutex m_input_buffer_mutex;
    std::mutex m_mutex;
    GLint m_position;
    GLint m_texcoord;
    int m_view_mode;
    
    int m_img_width;
    int m_img_height;
    
    BOOL m_isUseMirror;
    BOOL m_config_changed;
    
    BOOL m_isRendering;
    
    //当有新帧到达或者屏幕大小发生变化时该标志位才会置位YES
    BOOL m_isNewFrameAvailable;
}

@synthesize textureID  = _textureID;

- (instancetype)initWithTextureRegistry:(id<FlutterTextureRegistry>)registry isPublisher:(BOOL)isPublisher viewWidth:(int)width viewHeight:(int)height {
    self = [super init];
    if(self) {
        _registry = registry;
        m_pRenderFrameBuffer = nil;
        m_output_texture = nil;
        m_view_mode = ZegoRendererScaleAspectFit;
        _textureID  = [registry registerTexture:self];
        m_opengl_queue = dispatch_queue_create([[NSString stringWithFormat:@"opengl_queue%lld", _textureID] UTF8String], NULL);
       
        _isPublisher = isPublisher;
        _view_width = width;
        _view_height = height;
        if(_isPublisher)
            m_isUseMirror = YES;
        else
            m_isUseMirror = NO;
        
        m_img_width = 0;
        m_img_height = 0;
        m_config_changed = YES;
        
        m_isNewFrameAvailable = NO;
        
        m_isRendering = NO;
        
        [self createPixelBufferPool:&m_buffer_pool width:_view_width height:_view_height];
        m_pTempToCopyFrameBuffer = nil;
        
        __weak ZegoViewRenderer *weak_ptr = self;
        dispatch_async(m_opengl_queue, ^{
            ZegoViewRenderer *strong_ptr = weak_ptr;
            if(strong_ptr)
                [strong_ptr setupGL];
        });
    }
    
    return self;
}

- (void)setupGL {
    
    /*完全显示，view port坐标是[-1, 1]*/
    m_lstVertices[0] = -1.0;
    m_lstVertices[1] = -1.0;
    
    m_lstVertices[2] = 1.0;
    m_lstVertices[3] = -1.0;
    
    m_lstVertices[4] = -1.0;
    m_lstVertices[5] = 1.0;
    
    m_lstVertices[6] = 1.0;
    m_lstVertices[7] = 1.0;
    

    // * texture
    m_lstTexCoord[0] = 0.0;
    m_lstTexCoord[1] = 0.0;
    
    m_lstTexCoord[2] = 1.0;
    m_lstTexCoord[3] = 0.0;
    
    m_lstTexCoord[4] = 0.0;
    m_lstTexCoord[5] = 1.0;
    
    m_lstTexCoord[6] = 1.0;
    m_lstTexCoord[7] = 1.0;
    
   
    //初始化context
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if(self.context == nil) {
        self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    }
        
    [self compileShaders];
   
}

- (void)compileShaders {
    [EAGLContext setCurrentContext:self.context];
    
    // * disable depth test
    glDisable(GL_DEPTH_TEST);
    
    // * create our texture cache
    CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, self.context, NULL, &m_pTexCache);
    //CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, self.context, NULL, &m_pTexCache2);
    // * create our program
    m_hProgram = glCreateProgram();
    
    // * compile vertex shader
    const static char *strVertexShader = " \
    attribute vec4 position; \
    attribute mediump vec4 texcoord; \
    \
    varying mediump vec2 textureCoordinate; \
    \
    void main() \
    { \
    gl_Position = position; \
    textureCoordinate = texcoord.xy; \
    } \
    ";
    
    m_hVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_hVertexShader, 1, &strVertexShader, 0);
    glCompileShader(m_hVertexShader);
    GLint status;
    glGetShaderiv(m_hVertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        // * compile shader error
        // * to_do: handle error
        NSLog(@"error");
    }
    
    // * compile fragment shader
    const static char *strFragmentShader = " \
    varying highp vec2 textureCoordinate; \
    uniform sampler2D frame; \
    \
    void main() \
    { \
    gl_FragColor = texture2D(frame, textureCoordinate);\
    } \
    ";
    m_hFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_hFragShader, 1, &strFragmentShader, 0);
    glCompileShader(m_hFragShader);
    // * to_do: check status
    if (status != GL_TRUE) {
        NSLog(@"error");
        // * compile shader error
        // * to_do: handle error
    }
    
    // * attach shader to program
    glAttachShader(m_hProgram, m_hVertexShader);
    glAttachShader(m_hProgram, m_hFragShader);
    
    // * link program
    glLinkProgram(m_hProgram);
    glGetProgramiv(m_hProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        // * to_do: handle error
        NSLog(@"error");
    }
    
    // * use program
    glUseProgram(m_hProgram);
    
    // * get uniform index
    m_nFrameUniform = glGetUniformLocation(m_hProgram, "frame");
    
    /* 获取OpenGL为我们分配的两个attribute位置 */
    m_position = glGetAttribLocation(m_hProgram, "position");
    m_texcoord = glGetAttribLocation(m_hProgram, "texcoord");
    
    // * enable attributes
    glEnableVertexAttribArray(m_position);
    glEnableVertexAttribArray(m_texcoord);
    
    
    glGenFramebuffers(1, &m_framebuffer);
}

- (void)updateRenderSize:(CGSize)size {
    //切换大小时注意线程问题，保证线程同步
    
    __weak ZegoViewRenderer *weak_ptr = self;
    dispatch_async(m_opengl_queue, ^{
        ZegoViewRenderer *strong_ptr = weak_ptr;
        if(strong_ptr == nil)
            return;
        
        if(strong_ptr.view_width != size.width || strong_ptr.view_height != size.height) {
            strong_ptr.view_width = size.width;
            strong_ptr.view_height = size.height;
            
            [strong_ptr destroyPixelBufferPool:self->m_buffer_pool];
            [strong_ptr createPixelBufferPool:&self->m_buffer_pool width:strong_ptr.view_width height:strong_ptr.view_height];
            
            self->m_config_changed = YES;
            
        }
        
    });
    
    m_isNewFrameAvailable = YES;
    
}

- (void)setUseMirrorEffect:(BOOL)isUse
{
    __weak ZegoViewRenderer *weak_ptr = self;
    dispatch_async(m_opengl_queue, ^{
        ZegoViewRenderer *strong_ptr = weak_ptr;
        if(strong_ptr == nil)
            return;
        
        if(self->m_isUseMirror == isUse)
            return;
        
        self->m_isUseMirror = isUse;
        self->m_config_changed = YES;
    });
}

- (void)setupVAO:(int)imageWidth height:(int)imageHeight {
    //重新对texcoord数组赋值，负责缩放以及镜像
    
    float fWidthScale = 1.0;
    float fHeightScale = 1.0;
    
    
    m_img_width = imageWidth;
    m_img_height = imageHeight;
    //NSLog(@"setup vao, textureID: %lld, img :(%d, %d), view :(%d, %d),  ", _textureID, imageWidth, imageHeight, self.view_width, self.view_height);
    
    //实际倍数
    float fViewRate = (float)self.view_width / (float)self.view_height;
    float fImageRate = (float)imageWidth / (float)imageHeight;
    
    if (m_view_mode == 0) {
        // * blackbar
        if(fImageRate > fViewRate) {
            float actualHeight = self.view_width / fImageRate;
            fHeightScale = float(actualHeight / self.view_height);
        }else {
            float actualWidth = self.view_height * fImageRate;
            fWidthScale = float(actualWidth / self.view_width);
        }
    } else if(m_view_mode == 1) {
        // * crop
        if(fImageRate > fViewRate) {
            float actualWidth = self.view_height * fImageRate;
            fWidthScale = float(actualWidth / self.view_width);

        }else {
            float actualHeight = self.view_width / fImageRate;
            fHeightScale = float(actualHeight / self.view_height);
            
        }
    } else if(m_view_mode == 2) {
        // * stretch
    }
    
    
    m_lstVertices[0] = -fWidthScale;
    m_lstVertices[1] = -fHeightScale;
    
    m_lstVertices[2] = fWidthScale;
    m_lstVertices[3] = -fHeightScale;
    
    m_lstVertices[4] = -fWidthScale;
    m_lstVertices[5] = fHeightScale;
    
    m_lstVertices[6] = fWidthScale;
    m_lstVertices[7] = fHeightScale;
    
    if(m_isUseMirror) {
        for (int i = 0; i < 4; i++) {
            m_lstVertices[2 * i] = - m_lstVertices[2 * i];
        }
    }
    
    
    m_config_changed = NO;
    // * enable attributes
}

- (void)processingData {
    
    [EAGLContext setCurrentContext:self.context];
    
    /* 销毁上一帧的texture缓存 */
    if(m_output_texture) {
        CFRelease(m_output_texture);
        m_output_texture = NULL;
    }
    
    CVPixelBufferRef readInputBuffer = nil;
    {
        std::lock_guard<std::mutex> lock(m_input_buffer_mutex);
        if (m_pInputFrameBuffer) {
            readInputBuffer = m_pInputFrameBuffer;
            CVBufferRetain(readInputBuffer);
        }
    }
    
    if(readInputBuffer == nil /*|| m_pProcessFrameBuffer == nil*/)
        return;

    int width = (int)CVPixelBufferGetWidth(readInputBuffer);
    int height = (int)CVPixelBufferGetHeight(readInputBuffer);

    /* 以image宽高、viewg宽高计算纹理贴图坐标 */
    if(m_config_changed || width != m_img_width || height != m_img_height)
        [self setupVAO:width height:height];

    CVPixelBufferRef processBuffer;
    
    NSDictionary *auxPixelBufferAttributes = [NSDictionary dictionaryWithObjectsAndKeys:
                                           [NSNumber numberWithInt:3], (id)kCVPixelBufferPoolAllocationThresholdKey,
                                           nil
                                           ];
    
    CFDictionaryRef ref = (__bridge CFDictionaryRef)auxPixelBufferAttributes;
    CVReturn ret = CVPixelBufferPoolCreatePixelBufferWithAuxAttributes(nil, m_buffer_pool, ref, &processBuffer);
    if(ret != kCVReturnSuccess)
    {
        [ZegoLog logNotice: [NSString stringWithFormat:@"alloc error: %d", ret]];
        CVBufferRelease(readInputBuffer);
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(self->m_pRenderFrameBuffer) {
                //CVBufferRelease(self->m_pRenderFrameBuffer);
                self->m_pRenderFrameBuffer = nil;
            }
        }
        return;
    }

    /* create input frame texture from sdk */
    CVOpenGLESTextureRef texture_input = NULL;
    [self createTexture:&texture_input FromPixelBuffer:readInputBuffer];

    //create output frame texture to flutter
    [self createTexture:&m_output_texture FromPixelBuffer:processBuffer];

    if (texture_input && m_output_texture) {
        
        //Bind
        glBindTexture(CVOpenGLESTextureGetTarget(m_output_texture), CVOpenGLESTextureGetName(m_output_texture));
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, CVOpenGLESTextureGetName(m_output_texture), 0);
        
        glViewport(0, 0, self.view_width, self.view_height);
        
        // 黑色背景用于填充黑边
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glUseProgram(m_hProgram);
        
        
        glBindTexture(CVOpenGLESTextureGetTarget(texture_input), CVOpenGLESTextureGetName(texture_input));
        glUniform1i(m_nFrameUniform, 0);
        
        
        glEnableVertexAttribArray(m_position);
        glEnableVertexAttribArray(m_texcoord);
        
        glVertexAttribPointer(m_position, 2, GL_FLOAT, 0, 0, m_lstVertices);
        glVertexAttribPointer(m_texcoord, 2, GL_FLOAT, 0, 0, m_lstTexCoord);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFlush();
        
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            //if(self->m_pRenderFrameBuffer)
                //CVBufferRelease(self->m_pRenderFrameBuffer);
            
            self->m_pRenderFrameBuffer = processBuffer;
        }
        
        CFRelease(texture_input);
    }
    
    CVBufferRelease(readInputBuffer);
}

- (void)createTexture:(CVOpenGLESTextureRef *)texture FromPixelBuffer:(CVPixelBufferRef)pixelBuffer {
    int width = (int)CVPixelBufferGetWidth(pixelBuffer);
    int height = (int)CVPixelBufferGetHeight(pixelBuffer);
    
    CVReturn err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                                                                m_pTexCache,
                                                                pixelBuffer,
                                                                NULL,
                                                                GL_TEXTURE_2D,
                                                                GL_RGBA,
                                                                (GLsizei)width,
                                                                (GLsizei)height,
                                                                GL_BGRA,
                                                                GL_UNSIGNED_BYTE,
                                                                0,
                                                                texture);
    if(err != kCVReturnSuccess) {
        CFRelease(*texture);
        return;
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(CVOpenGLESTextureGetTarget(*texture), CVOpenGLESTextureGetName(*texture));
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
}

//接管sdk回调的数据，该数据从ZegoLiveRoomPlugin生成
- (void)setSrcFrameBuffer:(CVPixelBufferRef)srcFrameBuffer processBuffer:(CVPixelBufferRef)processBuffer {
    
    std::lock_guard<std::mutex> lock(m_input_buffer_mutex);
    
    if(self->m_pInputFrameBuffer) {
        CVBufferRelease(self->m_pInputFrameBuffer);
    }
    self->m_pInputFrameBuffer = srcFrameBuffer;
    CVBufferRetain(self->m_pInputFrameBuffer);
    
    m_isNewFrameAvailable = YES;
}

- (void)setVideoMode:(ZegoRendererViewMode)mode {
    
    dispatch_async(m_opengl_queue, ^{
        if(self->m_view_mode == mode)
            return;
        self->m_view_mode = mode;
        
        self->m_config_changed = YES;
    });
}

- (CVPixelBufferRef)getPixelBuffer {
    return m_pRenderFrameBuffer;
}

- (BOOL)isNewFrameAvailable {
    return m_isNewFrameAvailable;
}

- (void)notifyDrawNewFrame {
    [self.registry textureFrameAvailable:self.textureID];
}

- (void)releaseRenderer:(void(^)())callback {
    
    dispatch_async(m_opengl_queue, ^{
        
        [EAGLContext setCurrentContext:self.context];
        
        if(self->m_pInputFrameBuffer){
            CVBufferRelease(self->m_pInputFrameBuffer);
            self->m_pInputFrameBuffer = nil;
        }
        
        if(self->m_pRenderFrameBuffer){
            CVBufferRelease(self->m_pRenderFrameBuffer);
            self->m_pRenderFrameBuffer = nil;
        }
        
        if(self->m_output_texture) {
            CFRelease(self->m_output_texture);
            self->m_output_texture = NULL;
        }
        
        if (self->m_pTexCache) {
            CFRelease(self->m_pTexCache);
            self->m_pTexCache = 0;
        }
        
        if(self->m_framebuffer) {
            glDeleteBuffers(1, &self->m_framebuffer);
        }
        
        if(self->m_hVertexShader) {
            glDeleteShader(self->m_hVertexShader);
        }
        
        if(self->m_hFragShader) {
            glDeleteShader(self->m_hFragShader);
        }
        
        if(self->m_hProgram) {
            glDeleteProgram(self->m_hProgram);
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{
            if(callback)
                callback();
        });
    });
    //释放GPU资源
    [self.registry unregisterTexture:_textureID];
}

//释放资源
-(void)dealloc {
    
    [self destroyPixelBufferPool:m_buffer_pool];
    
    self.context = nil;
    
}

#pragma mark - FlutterTexture
- (CVPixelBufferRef)copyPixelBuffer {
   
    __weak ZegoViewRenderer *weak_ptr = self;
    dispatch_async(m_opengl_queue, ^{
        ZegoViewRenderer *strong_ptr = weak_ptr;
        [strong_ptr processingData];
    });
    
    //CVPixelBufferRef temp = nil;
    
    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_pTempToCopyFrameBuffer != m_pRenderFrameBuffer) {
        
        
        //if(m_pTempToCopyFrameBuffer)
        CVBufferRelease(m_pTempToCopyFrameBuffer);
        m_pTempToCopyFrameBuffer = m_pRenderFrameBuffer;
        
        CVBufferRetain(m_pTempToCopyFrameBuffer);
    }
    
    /*if (m_pRenderFrameBuffer) {
        temp = m_pRenderFrameBuffer;
        CVBufferRetain(temp);
    }*/
    
    m_isNewFrameAvailable = NO;
    return m_pTempToCopyFrameBuffer;
}

- (void)createPixelBufferPool:(CVPixelBufferPoolRef *)pool width:(int)width height:(int)height {
    NSDictionary *pixelBufferAttributes = [NSDictionary dictionaryWithObjectsAndKeys:
                                           [NSNumber numberWithBool:YES], (id)kCVPixelBufferOpenGLCompatibilityKey,
                                           [NSNumber numberWithInt:width], (id)kCVPixelBufferWidthKey,
                                           [NSNumber numberWithInt:height], (id)kCVPixelBufferHeightKey,
                                           [NSDictionary dictionary], (id)kCVPixelBufferIOSurfacePropertiesKey,
                                           [NSNumber numberWithInt:kCVPixelFormatType_32BGRA], (id)kCVPixelBufferPixelFormatTypeKey,
                                           nil
                                           ];
    
    CFDictionaryRef ref = (__bridge CFDictionaryRef)pixelBufferAttributes;
    CVReturn ret = CVPixelBufferPoolCreate(nil, nil, ref, pool);
    if (ret != kCVReturnSuccess) {
        return;
    }
    
}

- (void)destroyPixelBufferPool:(CVPixelBufferPoolRef)pool {
    if(pool == nil)
        return;
    
    CVPixelBufferPoolFlushFlags flag = 0;
    CVPixelBufferPoolFlush(pool, flag);
    CFRelease(pool);
    pool = nil;
    
}

@end
