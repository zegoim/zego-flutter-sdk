//
//  ZegoLog.m
//  
//
//  Created by lizhanpeng@ZEGO on 2019/8/8.
//

#import "ZegoLog.h"
#include "../../zego_defines.h"

@implementation ZegoLog

+ (void)logNotice:(NSString *)content {
    zego_liveroom_custom_log(content.UTF8String);
}

@end
