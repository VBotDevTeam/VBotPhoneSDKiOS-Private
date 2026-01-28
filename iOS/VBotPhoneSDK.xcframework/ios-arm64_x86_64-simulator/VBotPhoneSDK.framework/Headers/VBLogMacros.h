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

// Disable legacy macros
#ifndef VB_LEGACY_MACROS
    #define VB_LEGACY_MACROS 0
#endif

#import "VBLog.h"

/**
 * The constant/variable/method responsible for controlling the current log level.
 **/
#ifndef LOG_LEVEL_DEF
    #define LOG_LEVEL_DEF vbLogLevel
#endif

/**
 * Whether async should be used by log messages, excluding error messages that are always sent sync.
 **/
#ifndef LOG_ASYNC_ENABLED
    #define LOG_ASYNC_ENABLED YES
#endif

/**
 * These are the two macros that all other macros below compile into.
 * These big multiline macros makes all the other macros easier to read.
 **/
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

#define LOG_MACRO_TO_VBLOG(ddlog, isAsynchronous, lvl, flg, ctx, atag, fnct, frmt, ...) \
        [vblog log : isAsynchronous                                     \
             level : lvl                                                \
              flag : flg                                                \
           context : ctx                                                \
              file : __FILE__                                           \
          function : fnct                                               \
              line : __LINE__                                           \
               tag : atag                                               \
            format : (frmt), ## __VA_ARGS__]

/**
 * Define version of the macro that only execute if the log level is above the threshold.
 * The compiled versions essentially look like this:
 *
 * if (logFlagForThisLogMsg & vbLogLevel) { execute log message }
 *
 * When LOG_LEVEL_DEF is defined as vbLogLevel.
 *
 * As shown further below, Lumberjack actually uses a bitmask as opposed to primitive log levels.
 * This allows for a great amount of flexibility and some pretty advanced fine grained logging techniques.
 *
 * Note that when compiler optimizations are enabled (as they are for your release builds),
 * the log messages above your logging threshold will automatically be compiled out.
 *
 * (If the compiler sees LOG_LEVEL_DEF/vbLogLevel declared as a constant, the compiler simply checks to see
 *  if the 'if' statement would execute, and if not it strips it from the binary.)
 *
 * We also define shorthand versions for asynchronous and synchronous logging.
 **/
#define LOG_MAYBE(async, lvl, flg, ctx, tag, fnct, frmt, ...) \
        do { if(((NSUInteger)lvl & (NSUInteger)flg) != 0) LOG_MACRO(async, lvl, flg, ctx, tag, fnct, frmt, ##__VA_ARGS__); } while(0)

#define LOG_MAYBE_TO_VBLOG(ddlog, async, lvl, flg, ctx, tag, fnct, frmt, ...) \
        do { if(((NSUInteger)lvl & (NSUInteger)flg) != 0) LOG_MACRO_TO_VBLOG(ddlog, async, lvl, flg, ctx, tag, fnct, frmt, ##__VA_ARGS__); } while(0)

/**
 * Ready to use log macros with no context or tag.
 **/
#define VBLogError(frmt, ...)   LOG_MAYBE(NO,                LOG_LEVEL_DEF, VBLogFlagError,   0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define VBLogWarn(frmt, ...)    LOG_MAYBE(LOG_ASYNC_ENABLED, LOG_LEVEL_DEF, VBLogFlagWarning, 0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define VBLogInfo(frmt, ...)    LOG_MAYBE(LOG_ASYNC_ENABLED, LOG_LEVEL_DEF, VBLogFlagInfo,    0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define VBLogDebug(frmt, ...)   LOG_MAYBE(LOG_ASYNC_ENABLED, LOG_LEVEL_DEF, VBLogFlagDebug,   0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define VBLogVerbose(frmt, ...) LOG_MAYBE(LOG_ASYNC_ENABLED, LOG_LEVEL_DEF, VBLogFlagVerbose, 0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)

#define VBLogErrorToVBLog(ddlog, frmt, ...)   LOG_MAYBE_TO_VBLOG(ddlog, NO,                LOG_LEVEL_DEF, VBLogFlagError,   0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define VBLogWarnToVBLog(ddlog, frmt, ...)    LOG_MAYBE_TO_VBLOG(ddlog, LOG_ASYNC_ENABLED, LOG_LEVEL_DEF, VBLogFlagWarning, 0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define VBLogInfoToVBLog(ddlog, frmt, ...)    LOG_MAYBE_TO_VBLOG(ddlog, LOG_ASYNC_ENABLED, LOG_LEVEL_DEF, VBLogFlagInfo,    0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define VBLogDebugToVBLog(ddlog, frmt, ...)   LOG_MAYBE_TO_VBLOG(ddlog, LOG_ASYNC_ENABLED, LOG_LEVEL_DEF, VBLogFlagDebug,   0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define VBLogVerboseToVBLog(ddlog, frmt, ...) LOG_MAYBE_TO_VBLOG(ddlog, LOG_ASYNC_ENABLED, LOG_LEVEL_DEF, VBLogFlagVerbose, 0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
