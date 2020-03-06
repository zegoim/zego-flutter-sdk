//
//  ZGPixelBufferUtils.h
//  Runner
//
//  Created by Patrick Fu on 2020/3/5.
//  Copyright © 2020 The Chromium Authors. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZGPixelBufferUtils : NSObject

/// 创建 CVPixelBuffer
/// @param width 宽
/// @param height 高
/// @param format PixelBuffer 的格式
/// @return 创建好的 PixelBuffer
+ (nullable CVPixelBufferRef)createPixelBufferWithWidth:(size_t)width height:(size_t)height format:(OSType)format;

/// 创建 CVPixelBufferPool
/// @param width 宽
/// @param height 高
/// @param format PixelBuffer 的格式
/// @return 创建好的 Pool
+ (nullable CVPixelBufferPoolRef)createPixelBufferPoolWithWidth:(size_t)width height:(size_t)height format:(OSType)format;


/// 拷贝 ARGB 类型（单平面模式, 不限排列顺序）的数据到对应数据类型的 PixelBuffer 中
/// @param buffer 裸数据
/// @param pixelbuffer 初始化好的 PixelBuffer
/// @param stride 步长，此处取 stride[0]
/// @param size 图像的宽高
+ (BOOL)copyRGBBuffer:(unsigned char * _Nonnull)buffer to:(CVPixelBufferRef _Nonnull )pixelBuffer strides:(int)strides width:(size_t)width height:(size_t)height;

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
+ (BOOL)copyNV12Buffer:(unsigned char * _Nonnull * _Nonnull)buffer to:(CVPixelBufferRef _Nonnull )pixelBuffer strides:(int *_Nonnull)strides width:(size_t)width height:(size_t)height;

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
+ (BOOL)copyI420Buffer:(unsigned char * _Nonnull * _Nonnull)buffer to:(CVPixelBufferRef _Nonnull )pixelBuffer strides:(int * _Nonnull)strides width:(size_t)width height:(size_t)height;

/// 创建 BGRA32 类型的 PixelBufferPool
/// @param pool CVPixelBufferPoolRef, The newly created pool will be placed here
/// @param width Image Width
/// @param height Image Height
/// @return 是否成功创建 PixelBufferPool
+ (BOOL)create32BGRAPixelBufferPool:(CVPixelBufferPoolRef _Nonnull * _Nonnull)pool width:(int)width height:(int)height;

/// 创建 I420 类型的 PixelBufferPool
/// @param pool CVPixelBufferPoolRef, The newly created pool will be placed here
/// @param width Image Width
/// @param height Image Height
/// @return 是否成功创建 PixelBufferPool
+ (BOOL)createI420PixelBufferPool:(CVPixelBufferPoolRef _Nonnull * _Nonnull)pool width:(int)width height:(int)height;

/// 创建 NV12 类型的 PixelBufferPool
/// @param pool CVPixelBufferPoolRef, The newly created pool will be placed here
/// @param width Image Width
/// @param height Image Height
/// @return 是否成功创建 PixelBufferPool
+ (BOOL)createNV12PixelBufferPool:(CVPixelBufferPoolRef _Nonnull * _Nonnull)pool width:(int)width height:(int)height;

/// 释放 CVPixelBufferPoolRef
/// @param pool 待释放的 CVPixelBufferPoolRef
+ (void)destroyPixelBufferPool:(CVPixelBufferPoolRef _Nonnull * _Nonnull)pool;

/// 拷贝 CVPixelBufferRef
/// @param src Source CVPixelBufferRef
/// @param dst Destination CVPixelBufferRef
+ (BOOL)copyPixelBufferFrom:(CVPixelBufferRef _Nonnull)src to:(CVPixelBufferRef _Nonnull)dst;

@end

NS_ASSUME_NONNULL_END
