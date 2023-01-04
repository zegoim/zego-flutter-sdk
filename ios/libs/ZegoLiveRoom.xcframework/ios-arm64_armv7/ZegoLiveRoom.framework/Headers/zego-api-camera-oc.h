//
//  zego_camera_ios.h
//  zegoavkit
//
//  Created by summeryxia on 2018/04/11.
//  Copyright © 2018年 Zego. All rights reserved.
//

#ifndef zego_api_camera_oc_h
#define zego_api_camera_oc_h

#import <Foundation/Foundation.h>
#import "zego-api-defines-oc.h"

typedef NS_ENUM(NSInteger, ZegoCameraFocusMode) {
    ZegoCameraFocusModeLocked              = 0,
    ZegoCameraFocusModeAutoFocus           = 1,
    ZegoCameraFocusModeContinuousAutoFocus = 2,
};

typedef NS_ENUM(NSInteger, ZegoCameraExposureMode) {
    ZegoCameraExposureModeLocked                            = 0,
    ZegoCameraExposureModeAutoExpose                        = 1,
    ZegoCameraExposureModeContinuousAutoExposure            = 2,
};

@interface ZegoCamera : NSObject

#if TARGET_OS_IPHONE
/**
 设置设备坐标中的曝光参考点
 
 @param point 坐标点的 x 和 y 的取值范围均为 [0.0, 1.0]. 坐标是对应于设备的归一化的坐标, 左上角坐标为 (0.0, 0.0)，右下角坐标为(1.0, 1.0)
 @param index 推流通道
 @attention 必须在初始化 SDK 后调用
 @deprecated 请使用 [setCamExposurePointInPreview:channelIndex:] 代替
 */
+ (BOOL)setCamExposurePoint:(CGPoint)point channelIndex:(ZegoAPIPublishChannelIndex)index __attribute__((__deprecated__));

/**
 设置预览视图中的对焦点
 
 @param point 坐标点的 x 和 y 的取值范围均为 [0.0, 1.0]. 坐标是对应于视图的归一化的坐标，左上角是(0.0, 0.0), 右下角是 (1.0, 1.0)
 @param index 推流通道
 @return true 成功，false 失败
 @attention 每次摄像头重新启动采集，设置都会失效，需要重新设置.
 @attention 如果要使用对焦功能最好关闭人脸识别，目前 iOS 是默认开启的, 通过 setConfig 接口设置 "camera_detect_face=false" 关闭.
 */
+ (BOOL)setCamFocusPointInPreview:(CGPoint)point channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置预览视图中的曝光参考点
 
 @param point 坐标点的 x 和 y 的取值范围均为 [0.0, 1.0]. 坐标是对应于视图的归一化的坐标，左上角是(0.0, 0.0), 右下角是 (1.0, 1.0)
 @param index 推流通道
 @return true 成功，false 失败
 @attention 每次摄像头重新启动采集，设置都会失效，需要重新设置.
 */
+ (BOOL)setCamExposurePointInPreview:(CGPoint)point channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置摄像头变焦倍数
 
 @param zoomFactor 变焦倍数，最小值为1.0，最大值不能超过 getCamMaxZoomFactor: 的返回值
 @param index 推流通道
 @return true 成功，false 失败
 @attention 每次摄像头重新启动采集，设置都会失效，需要重新设置.
 */
+ (BOOL)setCamZoomFactor:(float)zoomFactor channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 获取摄像头最大变焦倍数
 
 @param index 推流通道
 @return 返回最大变焦倍数，1.0为不支持变焦
 */
+ (float)getCamMaxZoomFactor:(ZegoAPIPublishChannelIndex)index;

/**
 设置曝光补偿度
 
 @param value 曝光度，取值范围为[-1,1], -1变黑, 1变亮
 @param index 推流通道
 @return true 成功，false 失败
 @discussion 必须在初始化 SDK 后调用
 */
+ (BOOL)setCamExposureCompensation:(float)value channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置对焦模式
    
 @param mode 对焦模式，参考 ZegoCameraFocusMode
 @param index 推流 channel Index. 默认为主Channel
 @return true 成功，false 失败
 @attention 必须在初始化 SDK 后调用
 @attention 仅对后摄像头生效
 @attention SDK 默认使用 ZegoCameraFocusModeContinuousAutoFocus
*/
+ (BOOL)setCamFocusMode:(ZegoCameraFocusMode)mode channelIndex:(ZegoAPIPublishChannelIndex)index;

/**
 设置曝光模式
    
 @param mode 曝光模式，参考 ZegoCameraExposureMode
 @param index 推流 channel Index. 默认为主Channel
 @return true 成功，false 失败
 @attention 必须在初始化 SDK 后调用
 @attention SDK 默认使用 ZegoCameraExposureModeContinuousAutoExposure
*/
+ (BOOL)setCamExposureMode:(ZegoCameraExposureMode)mode channelIndex:(ZegoAPIPublishChannelIndex)index;

#endif
/**
 开启摄像头自适应帧率

 @note 开启后，SDK 在一定帧率范围内根据环境亮度自动匹配合适的摄像头帧率，提升设置的帧率过高时的画面亮度。
 @note 业务场景：推流端用户设置的帧率偏高，所处环境光照较低，无法正常显示或识别主体的场景。

 @attention 调用时机：在初始化 SDK 后、摄像头启动前调用。
 @attention 注意事项：当 SetVideoFPS 设置帧率值小于期望帧率最小值时，将使用 SetVideoFPS 设置的帧率值。

 @param enable true 开启，false 关闭
 @param minFPS 期望帧率的最小值，最小值为 3，建议 15
 @param maxFPS 期望帧率的最大值，非负值，建议 25
 @param index 推流通道
*/
+ (void)enableCamAdaptiveFPS:(BOOL)enable minFPS:(int)minFPS maxFPS:(int)maxFPS channelIndex:(ZegoAPIPublishChannelIndex)index;

@end

#endif
