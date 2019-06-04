//
//  ZegoPlatformView.m
//  Pods-Runner
//
//  Created by 李展鹏@ZEGO on 2019/5/30.
//

#import "ZegoPlatformView.h"

@interface ZegoPlatformView()

@property (nonatomic, strong) UIView *uiView;
@property (nonatomic, assign) int64_t viewID;

@end

@implementation ZegoPlatformView

- (instancetype)initWithRect:(CGRect)rect viewID:(int64_t) viewID {
    self = [super init];
    if(self) {
        _uiView = [[UIView alloc] initWithFrame:rect];
        _uiView.backgroundColor = [UIColor blackColor];
        _viewID = viewID;
    }
    
    return self;
}

- (UIView *)getUIView {
    return self.uiView;
}

# pragma mark - Flutter Platform View Delegate

- (UIView *)view {
    return self.uiView;
}


@end
