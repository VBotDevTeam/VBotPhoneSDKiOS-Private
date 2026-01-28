// Software License Agreement (BSD License)
//
// Copyright (c) 2010-2024, Deusty, LLC
// All rights reserved.
//
// Redistribution and use of this software in source and binary forms,
// with or without modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// * Neither the name of Deusty nor the names of its contributors may be used
//   to endorse or promote products derived from this software without specific
//   prior written permission of Deusty, LLC.

/**
 * Legacy macros used for 1.9.x backwards compatibility.
 *
 * Imported by default when importing a VBLog.h directly and VB_LEGACY_MACROS is not defined and set to 0.
 **/
#if VB_LEGACY_MACROS

#warning CocoaLumberjack 1.9.x legacy macros enabled. \
Disable legacy macros by importing VBCocoaLumberjack.h or VBLogMacros.h instead of VBLog.h or add `#define VB_LEGACY_MACROS 0` before importing VBLog.h.

#ifndef LOG_LEVEL_DEF
    #define LOG_LEVEL_DEF vbLogLevel
#endif

#define LOG_FLAG_ERROR    VBLogFlagError
#define LOG_FLAG_WARN     VBLogFlagWarning
#define LOG_FLAG_INFO     VBLogFlagInfo
#define LOG_FLAG_DEBUG    VBLogFlagDebug
#define LOG_FLAG_VERBOSE  VBLogFlagVerbose

#define LOG_LEVEL_OFF     VBLogLevelOff
#define LOG_LEVEL_ERROR   VBLogLevelError
#define LOG_LEVEL_WARN    VBLogLevelWarning
#define LOG_LEVEL_INFO    VBLogLevelInfo
#define LOG_LEVEL_DEBUG   VBLogLevelDebug
#define LOG_LEVEL_VERBOSE VBLogLevelVerbose
#define LOG_LEVEL_ALL     VBLogLevelAll

#define LOG_ASYNC_ENABLED YES

#define LOG_ASYNC_ERROR    ( NO && LOG_ASYNC_ENABLED)
#define LOG_ASYNC_WARN     (YES && LOG_ASYNC_ENABLED)
#define LOG_ASYNC_INFO     (YES && LOG_ASYNC_ENABLED)
#define LOG_ASYNC_DEBUG    (YES && LOG_ASYNC_ENABLED)
#define LOG_ASYNC_VERBOSE  (YES && LOG_ASYNC_ENABLED)

#define LOG_MACRO(isAsynchronous, lvl, flg, ctx, atag, fnct, frmt, ...) \
        [VBLog log : isAsynchronous                                     \
             level : lvl                                                \
              flag : flg                                                \
           context : ctx                                                \
              file : __FILE__                                           \
          function : fnct                                               \
              line : __LINE__                                           \
               tag : atag                                               \
            format : (frmt), ## __VA_ARGS__]

#define LOG_MAYBE(async, lvl, flg, ctx, fnct, frmt, ...)                       \
        do { if((lvl & flg) != 0) LOG_MACRO(async, lvl, flg, ctx, nil, fnct, frmt, ##__VA_ARGS__); } while(0)

#define LOG_OBJC_MAYBE(async, lvl, flg, ctx, frmt, ...) \
        LOG_MAYBE(async, lvl, flg, ctx, __PRETTY_FUNCTION__, frmt, ## __VA_ARGS__)

#define VBLogError(frmt, ...)   LOG_OBJC_MAYBE(LOG_ASYNC_ERROR,   LOG_LEVEL_DEF, LOG_FLAG_ERROR,   0, frmt, ##__VA_ARGS__)
#define VBLogWarn(frmt, ...)    LOG_OBJC_MAYBE(LOG_ASYNC_WARN,    LOG_LEVEL_DEF, LOG_FLAG_WARN,    0, frmt, ##__VA_ARGS__)
#define VBLogInfo(frmt, ...)    LOG_OBJC_MAYBE(LOG_ASYNC_INFO,    LOG_LEVEL_DEF, LOG_FLAG_INFO,    0, frmt, ##__VA_ARGS__)
#define VBLogDebug(frmt, ...)   LOG_OBJC_MAYBE(LOG_ASYNC_DEBUG,   LOG_LEVEL_DEF, LOG_FLAG_DEBUG,   0, frmt, ##__VA_ARGS__)
#define VBLogVerbose(frmt, ...) LOG_OBJC_MAYBE(LOG_ASYNC_VERBOSE, LOG_LEVEL_DEF, LOG_FLAG_VERBOSE, 0, frmt, ##__VA_ARGS__)

#endif
