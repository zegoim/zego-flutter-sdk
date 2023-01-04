//
//  zego-api-data-collect-oc.h
//  ZegoLiveRoom
//
//  Created by Strong on 11/12/2017.
//

#import <Foundation/Foundation.h>

@interface ZegoDataCollect : NSObject

+ (BOOL)reportEventName:(NSString *)name content:(NSString *)content;

@end
