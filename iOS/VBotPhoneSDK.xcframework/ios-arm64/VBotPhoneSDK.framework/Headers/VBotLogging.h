//
//  VBotLogging.h
//  Copyright © 2020 VPMedia. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VBCocoaLumberjack.h"

@interface VBotLogging : NSObject

#define VBotLog(flag, fnct, frmt, ...) \
[VBotLogging logWithFlag: flag file:__FILE__ function: fnct line:__LINE__ format:(frmt), ## __VA_ARGS__]

#define VBotLogVerbose(frmt, ...)    VBotLog(VBLogFlagVerbose,    __PRETTY_FUNCTION__, frmt, ## __VA_ARGS__)
#define VBotLogDebug(frmt, ...)      VBotLog(VBLogFlagDebug,      __PRETTY_FUNCTION__, frmt, ## __VA_ARGS__)
#define VBotLogInfo(frmt, ...)       VBotLog(VBLogFlagInfo,       __PRETTY_FUNCTION__, frmt, ## __VA_ARGS__)
#define VBotLogWarning(frmt, ...)    VBotLog(VBLogFlagWarning,    __PRETTY_FUNCTION__, frmt, ## __VA_ARGS__)
#define VBotLogError(frmt, ...)      VBotLog(VBLogFlagError,      __PRETTY_FUNCTION__, frmt, ## __VA_ARGS__)

+ (void) logWithFlag:(VBLogFlag)flag
                file: (const char *_Nonnull)file
            function:(const char*_Nonnull)function
                line:(NSUInteger)line
              format:(NSString * _Nonnull)format, ... NS_FORMAT_FUNCTION(5, 6);
@end
