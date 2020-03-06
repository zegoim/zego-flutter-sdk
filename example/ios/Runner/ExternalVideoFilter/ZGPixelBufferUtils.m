//
//  ZGPixelBufferUtils.m
//  Runner
//
//  Created by Patrick Fu on 2020/3/5.
//  Copyright © 2020 The Chromium Authors. All rights reserved.
//

#import "ZGPixelBufferUtils.h"

@implementation ZGPixelBufferUtils

+ (nullable CVPixelBufferRef)createPixelBufferWithWidth:(size_t)width height:(size_t)height format:(OSType)format {
    NSDictionary *pixelBufferAttributes = @{(id)kCVPixelBufferIOSurfacePropertiesKey:[NSDictionary dictionary]};
    
    CVPixelBufferRef pixelBuffer;
    CVReturn result = CVPixelBufferCreate(kCFAllocatorDefault, width, height, format, (__bridge CFDictionaryRef)(pixelBufferAttributes), &pixelBuffer);
    
    if (result != kCVReturnSuccess) {
        return nil;
    }
    
    return pixelBuffer;
}

+ (nullable CVPixelBufferPoolRef)createPixelBufferPoolWithWidth:(size_t)width height:(size_t)height format:(OSType)format {
    CFDictionaryRef empty;
    empty = CFDictionaryCreate(kCFAllocatorDefault, NULL, NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    // set CVPixelBufferPool format
    SInt32 cvPixelFormatTypeValue = format;
    CFNumberRef cfPixelFormat = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, (const void*)(&(cvPixelFormatTypeValue)));

    SInt32 cvWidthValue = (SInt32)width;
    CFNumberRef cfWidth = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, (const void*)(&(cvWidthValue)));
    SInt32 cvHeightValue = (SInt32)height;
    CFNumberRef cfHeight = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, (const void*)(&(cvHeightValue)));
    
    CFMutableDictionaryRef attrs;
    attrs = CFDictionaryCreateMutable(kCFAllocatorDefault, 5, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    // set empty IOSurface properties dictionary to use default IOSurface options
    CFDictionarySetValue(attrs, kCVPixelBufferIOSurfacePropertiesKey, empty);
    
    // set the pixel buffer's format
    CFDictionarySetValue(attrs, kCVPixelBufferPixelFormatTypeKey, cfPixelFormat);
    CFDictionarySetValue(attrs, kCVPixelBufferWidthKey, cfWidth);
    CFDictionarySetValue(attrs, kCVPixelBufferHeightKey, cfHeight);
    
    // make the pixel buffer compatible with OpenGL(ES) contexts
    #if TARGET_OS_IOS
    CFDictionarySetValue(attrs, kCVPixelBufferOpenGLESCompatibilityKey, kCFBooleanTrue);
    #elif TARGET_OS_OSX
    CFDictionarySetValue(attrs, kCVPixelBufferOpenGLCompatibilityKey, kCFBooleanTrue);
    #endif

    CVPixelBufferPoolRef pool;

    CVReturn result = CVPixelBufferPoolCreate(kCFAllocatorDefault, NULL, attrs, &pool);

    CFRelease(attrs);
    CFRelease(empty);
    CFRelease(cfPixelFormat);
    CFRelease(cfWidth);
    CFRelease(cfHeight);

    if (result != kCVReturnSuccess) {
        return nil;
    }

    return pool;
}

/// 拷贝 ARGB 类型（单平面模式, 不限排列顺序）的数据到对应数据类型的 PixelBuffer 中
/// @param buffer 裸数据
/// @param pixelbuffer 初始化好的 PixelBuffer
/// @param stride 步长，此处取 stride[0]
/// @param size 图像的宽高
+ (BOOL)copyRGBBuffer:(unsigned char *)buffer to:(CVPixelBufferRef)pixelBuffer strides:(int)strides width:(size_t)width height:(size_t)height {
    if (!pixelBuffer) return NO;
    if (!buffer) return NO;
    
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    
    size_t destStride = CVPixelBufferGetBytesPerRow(pixelBuffer);
    const int srcStride = strides;
    unsigned char *dst = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    for (unsigned int i = 0; i < height; i++, dst += destStride, buffer += srcStride) {
        memcpy(dst, buffer, srcStride);
    }
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    return YES;
}

/// 拷贝 NV12(YUV420Semi-Planar, 双平面) 类型的数据到对应数据类型的 PixelBuffer 中 (NV21也可用这个方法)
///
/// 内存布局
/// plane 1: Y Y Y Y Y Y Y Y
///          Y Y Y Y Y Y Y Y
/// plane 2: U V U V U V U V (NV21颠倒UV顺序)
///
/// @param buffer 裸数据
/// @param pixelbuffer 初始化好的 PixelBuffer
/// @param stride 步长，此处取 stride[0,1]
/// @param size 图像的宽高
+ (BOOL)copyNV12Buffer:(unsigned char * _Nonnull * _Nonnull)buffer to:(CVPixelBufferRef)pixelBuffer strides:(int *)strides width:(size_t)width height:(size_t)height {
    if (!buffer) return NO;
    
    const unsigned char *yData = buffer[0];
    const unsigned char *uvData = buffer[1];
    
    if (!yData || !uvData) return NO;
    
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    
    size_t destStride = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 0);
    const int yStride = strides[0];
    unsigned char* dst = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    
    for (unsigned int i = 0; i < height; ++i, dst += destStride, yData += yStride) {
        memcpy(dst, yData, yStride);
    }
    
    size_t destStrideUV = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 1);
    const int uvStride = strides[1];
    dst = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
    size_t uvHeight = height >> 1;
    for (unsigned int i = 0; i < uvHeight; ++i, dst += destStrideUV, uvData += uvStride) {
        memcpy(dst, uvData, uvStride);
    }
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    return YES;
}

/// 拷贝 I420(也叫 YU12, YUV420Planar, 三平面) 类型的数据到 PixelBuffer 中 (YV12也可用此方法)
///
/// 内存布局
/// plane 1: Y Y Y Y Y Y Y Y
///          Y Y Y Y Y Y Y Y
/// plane 2: U U U U
/// plane 3: V V V V (YV12为先V后U)
///
/// @param buffer 裸数据
/// @param pixelbuffer 初始化好的 PixelBuffer
/// @param stride 步长，此处取 stride[0,1,2]
/// @param size 图像的宽高
+ (BOOL)copyI420Buffer:(unsigned char * _Nonnull * _Nonnull)buffer to:(CVPixelBufferRef)pixelBuffer strides:(int *)strides width:(size_t)width height:(size_t)height {
    if (!buffer) return NO;
    
    const unsigned char* yData = buffer[0];
    const unsigned char* uData = buffer[1];
    const unsigned char* vData = buffer[2];
    
    if (!yData || !uData || !vData) {
        return NO;
    }
    
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    
    size_t h0 = height;
    size_t d0 = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 0);
    const int yStride = strides[0];
    unsigned char* dst = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    
    for (unsigned int i = 0; i < h0; ++i, dst += d0, yData += yStride) {
        memcpy(dst, yData, yStride);
    }
    
    /// Y:U:V 的数据量 4:1:1；U,V 的高和每行步长分别都是 Y 的一半，所以 U,V 的数据量分别都是 Y 的 1/4
    size_t h1 = (height >> 1);
    size_t d1 = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 1);
    const int uStride = strides[1];
    dst = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
    for (unsigned int i = 0; i < h1; ++i, dst += d1, uData += uStride) {
        memcpy(dst, uData, uStride);
    }
    
    size_t h2 = h1;
    size_t d2 = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 2);
    int vStride = strides[2];
    dst = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 2);
    for (unsigned int i = 0; i < h2; ++i, dst += d2, vData += vStride) {
        memcpy(dst, vData, vStride);
    }
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    return YES;
}

+ (BOOL)create32BGRAPixelBufferPool:(CVPixelBufferPoolRef*)pool width:(int)width height:(int)height {
    CFDictionaryRef empty; // empty value for attr value.
    CFMutableDictionaryRef attrs;
    
    empty = CFDictionaryCreate(kCFAllocatorDefault,
                               NULL, NULL, 0,
                               &kCFTypeDictionaryKeyCallBacks,
                               &kCFTypeDictionaryValueCallBacks); // our empty IOSurface properties dictionary
    
    SInt32 cvPixelFormatTypeValue = kCVPixelFormatType_32BGRA;
    CFNumberRef cfPixelFormat = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, (const void*)(&(cvPixelFormatTypeValue)));
    
    SInt32 cvWidthValue = width;
    CFNumberRef cfWidth = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, (const void*)(&(cvWidthValue)));
    SInt32 cvHeightValue = height;
    CFNumberRef cfHeight = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, (const void*)(&(cvHeightValue)));
    
    attrs = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                      4,
                                      &kCFTypeDictionaryKeyCallBacks,
                                      &kCFTypeDictionaryValueCallBacks);
    
    CFDictionarySetValue(attrs, kCVPixelBufferIOSurfacePropertiesKey, empty);
    CFDictionarySetValue(attrs, kCVPixelBufferPixelFormatTypeKey, cfPixelFormat);
    CFDictionarySetValue(attrs, kCVPixelBufferWidthKey, cfWidth);
    CFDictionarySetValue(attrs, kCVPixelBufferHeightKey, cfHeight);
    
    CVReturn ret = CVPixelBufferPoolCreate(kCFAllocatorDefault, nil, attrs, pool);
    
    CFRelease(attrs);
    CFRelease(empty);
    CFRelease(cfPixelFormat);
    CFRelease(cfWidth);
    CFRelease(cfHeight);
    
    if (ret != kCVReturnSuccess) {
        return NO;
    }
    
    return YES;
}

+ (BOOL)createI420PixelBufferPool:(CVPixelBufferPoolRef*)pool width:(int)width height:(int)height {
    CFDictionaryRef empty; // empty value for attr value.
    CFMutableDictionaryRef attrs;
    
    empty = CFDictionaryCreate(kCFAllocatorDefault,
                               NULL, NULL, 0,
                               &kCFTypeDictionaryKeyCallBacks,
                               &kCFTypeDictionaryValueCallBacks); // our empty IOSurface properties dictionary
    
    SInt32 cvPixelFormatTypeValue = kCVPixelFormatType_420YpCbCr8PlanarFullRange;
    CFNumberRef cfPixelFormat = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, (const void*)(&(cvPixelFormatTypeValue)));
    
    SInt32 cvWidthValue = width;
    CFNumberRef cfWidth = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, (const void*)(&(cvWidthValue)));
    SInt32 cvHeightValue = height;
    CFNumberRef cfHeight = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, (const void*)(&(cvHeightValue)));
    
    attrs = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                      5,
                                      &kCFTypeDictionaryKeyCallBacks,
                                      &kCFTypeDictionaryValueCallBacks);
    
    CFDictionarySetValue(attrs, kCVPixelBufferIOSurfacePropertiesKey, empty);
    CFDictionarySetValue(attrs, kCVPixelBufferPixelFormatTypeKey, cfPixelFormat);
    CFDictionarySetValue(attrs, kCVPixelBufferWidthKey, cfWidth);
    CFDictionarySetValue(attrs, kCVPixelBufferHeightKey, cfHeight);
#if TARGET_OS_IOS
    CFDictionarySetValue(attrs, kCVPixelBufferOpenGLESCompatibilityKey, kCFBooleanTrue);
#elif TARGET_OS_OSX
    CFDictionarySetValue(attrs, kCVPixelBufferOpenGLCompatibilityKey, kCFBooleanTrue);
#endif
    
    CVReturn ret = CVPixelBufferPoolCreate(kCFAllocatorDefault, nil, attrs, pool);
    
    CFRelease(attrs);
    CFRelease(empty);
    CFRelease(cfPixelFormat);
    CFRelease(cfWidth);
    CFRelease(cfHeight);
    
    if (ret != kCVReturnSuccess) {
        return NO;
    }
    
    return YES;
}

+ (BOOL)createNV12PixelBufferPool:(CVPixelBufferPoolRef*)pool width:(int)width height:(int)height {
    CFDictionaryRef empty; // empty value for attr value.
    CFMutableDictionaryRef attrs;
    
    empty = CFDictionaryCreate(kCFAllocatorDefault,
                               NULL, NULL, 0,
                               &kCFTypeDictionaryKeyCallBacks,
                               &kCFTypeDictionaryValueCallBacks); // our empty IOSurface properties dictionary
    
    SInt32 cvPixelFormatTypeValue = kCVPixelFormatType_420YpCbCr8BiPlanarFullRange;
    CFNumberRef cfPixelFormat = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, (const void*)(&(cvPixelFormatTypeValue)));
    
    SInt32 cvWidthValue = width;
    CFNumberRef cfWidth = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, (const void*)(&(cvWidthValue)));
    SInt32 cvHeightValue = height;
    CFNumberRef cfHeight = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, (const void*)(&(cvHeightValue)));
    
    attrs = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                      5,
                                      &kCFTypeDictionaryKeyCallBacks,
                                      &kCFTypeDictionaryValueCallBacks);
    
    CFDictionarySetValue(attrs, kCVPixelBufferIOSurfacePropertiesKey, empty);
    CFDictionarySetValue(attrs, kCVPixelBufferPixelFormatTypeKey, cfPixelFormat);
    CFDictionarySetValue(attrs, kCVPixelBufferWidthKey, cfWidth);
    CFDictionarySetValue(attrs, kCVPixelBufferHeightKey, cfHeight);
#if TARGET_OS_IOS
    CFDictionarySetValue(attrs, kCVPixelBufferOpenGLESCompatibilityKey, kCFBooleanTrue);
#elif TARGET_OS_OSX
    CFDictionarySetValue(attrs, kCVPixelBufferOpenGLCompatibilityKey, kCFBooleanTrue);
#endif
    
    CVReturn ret = CVPixelBufferPoolCreate(kCFAllocatorDefault, nil, attrs, pool);
    
    CFRelease(attrs);
    CFRelease(empty);
    CFRelease(cfPixelFormat);
    CFRelease(cfWidth);
    CFRelease(cfHeight);
    
    if (ret != kCVReturnSuccess) {
        return NO;
    }
    
    return YES;
}

+ (void)destroyPixelBufferPool:(CVPixelBufferPoolRef*)pool {
    CVPixelBufferPoolRelease(*pool);
    *pool = nil;
}

+ (BOOL)copyPixelBufferFrom:(CVPixelBufferRef)src to:(CVPixelBufferRef)dst {
    BOOL ret = YES;
    
    CVPixelBufferLockBaseAddress(src, kCVPixelBufferLock_ReadOnly);
    
    unsigned char* pb = (unsigned char*)CVPixelBufferGetBaseAddressOfPlane(src, 0);
    int height = (int)CVPixelBufferGetHeight(src);
    int stride = (int)CVPixelBufferGetBytesPerRow(src);
    int size = (int)CVPixelBufferGetDataSize(src);
    
    while (1) {
        CVReturn cvRet = CVPixelBufferLockBaseAddress(dst, 0);
        if (cvRet != kCVReturnSuccess) {
            ret = NO;
            break;
        }
        
        int dst_height = (int)CVPixelBufferGetHeight(dst);
        int dst_stride = (int)CVPixelBufferGetBytesPerRow(dst);
        int dst_size = (int)CVPixelBufferGetDataSize(dst);
        
        if (stride == dst_stride && dst_size == size) {
            unsigned char* temp = (unsigned char*)CVPixelBufferGetBaseAddressOfPlane(dst, 0);
            memcpy(temp, pb, size);
        } else {
            int copy_height = height > dst_height ? dst_height : height;
            int copy_stride = stride > dst_stride ? dst_stride : stride;
            
            unsigned char* offset_dst = (unsigned char*)CVPixelBufferGetBaseAddressOfPlane(dst, 0);
            unsigned char* offset_src = pb;
            for (int i = 0; i < copy_height; i++) {
                memcpy(offset_dst, offset_src, copy_stride);
                offset_src += stride;
                offset_dst += dst_stride;
            }
        }
        
        CVPixelBufferUnlockBaseAddress(dst, 0);
        break;
    }
    
    CVPixelBufferUnlockBaseAddress(src, kCVPixelBufferLock_ReadOnly);
    return ret;
}


@end
