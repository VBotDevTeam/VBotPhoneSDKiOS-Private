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
 * Welcome to CocoaLumberjack!
 *
 * The project page has a wealth of documentation if you have any questions.
 * https://github.com/CocoaLumberjack
 *
 * If you're new to the project you may wish to read "Getting Started" at:
 * Documentation/GettingStarted.md
 *
 * Otherwise, here is a quick refresher.
 * There are three steps to using the macros:
 *
 * Step 1:
 * Import the header in your implementation or prefix file:
 *
 * #import <CocoaLumberjack.h>
 *
 * Step 2:
 * Define your logging level in your implementation file:
 *
 * // Log levels: off, error, warn, info, verbose
 * static const VBLogLevel vbLogLevel = VBLogLevelVerbose;
 *
 * Step 2 [3rd party frameworks]:
 *
 * Define your LOG_LEVEL_DEF to a different variable/function than vbLogLevel:
 *
 * // #undef LOG_LEVEL_DEF // Undefine first only if needed
 * #define LOG_LEVEL_DEF myLibLogLevel
 *
 * Define your logging level in your implementation file:
 *
 * // Log levels: off, error, warn, info, verbose
 * static const VBLogLevel myLibLogLevel = VBLogLevelVerbose;
 *
 * Step 3:
 * Replace your NSLog statements with VBLog statements according to the severity of the message.
 *
 * NSLog(@"Fatal error, no dohickey found!"); -> VBLogError(@"Fatal error, no dohickey found!");
 *
 * VBLog works exactly the same as NSLog.
 * This means you can pass it multiple variables just like NSLog.
 **/

#import <Foundation/Foundation.h>

//! Project version number for CocoaLumberjack.
FOUNDATION_EXPORT double VBCocoaLumberjackVersionNumber;

//! Project version string for CocoaLumberjack.
FOUNDATION_EXPORT const unsigned char VBCocoaLumberjackVersionString[];

// Disable legacy macros
#ifndef VB_LEGACY_MACROS
    #define VB_LEGACY_MACROS 0
#endif

// Core
#import "VBLog.h"

// Main macros
#import "VBLogMacros.h"
#import "VBAssertMacros.h"

// Capture ASL
#import "VBASLLogCapture.h"

// Loggers
#import "VBLoggerNames.h"

#import "VBTTYLogger.h"
#import "VBASLLogger.h"
#import "VBFileLogger.h"
#import "VBOSLogger.h"

// Extensions
#import "VBContextFilterLogFormatter.h"
#import "VBContextFilterLogFormatter+Deprecated.h"
#import "VBDispatchQueueLogFormatter.h"
#import "VBMultiFormatter.h"
#import "VBFileLogger+Buffering.h"

// CLI
#import "VBCLIColor.h"

// etc
#import "VBAbstractDatabaseLogger.h"
#import "VBLog+LOGV.h"
#import "VBLegacyMacros.h"
