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

#import <TargetConditionals.h>
#import <Foundation/Foundation.h>
#import <os/log.h>

// Disable legacy macros
#ifndef VB_LEGACY_MACROS
    #define VB_LEGACY_MACROS 0
#endif

#import "VBLog.h"

NS_ASSUME_NONNULL_BEGIN

/// Describes a type that maps CocoaLumberjack log levels to os\_log levels (`os_log_type_t`).
API_AVAILABLE(macos(10.12), ios(10.0), watchos(3.0), tvos(10.0))
VB_SENDABLE
@protocol VBOSLogLevelMapper <NSObject>

/// Maps the given `VBLogFlag` to a `os_log_type_t`.
/// - Parameter logFlag: `VBLogFlag` for which to return the os log type.
- (os_log_type_t)osLogTypeForLogFlag:(VBLogFlag)logFlag;

@end

/// The default os\_log level mapper.
/// Uses the following mapping:
/// - `VBLogFlagError` -> `OS_LOG_TYPE_ERROR`
/// - `VBLogFlagWarning` -> `OS_LOG_TYPE_ERROR`
/// - `VBLogFlagInfo` -> `OS_LOG_TYPE_INFO`
/// - `VBLogFlagDebug` -> `OS_LOG_TYPE_DEBUG`
/// - `VBLogFlagVerbose` -> `OS_LOG_TYPE_DEBUG`
API_AVAILABLE(macos(10.12), ios(10.0), watchos(3.0), tvos(10.0))
VB_SENDABLE
@interface VBOSLogLevelMapperDefault : NSObject <VBOSLogLevelMapper>

- (instancetype)init NS_DESIGNATED_INITIALIZER;

@end

#if TARGET_OS_SIMULATOR
/// An os\_log level mapper that works around the fact that `OS_LOG_TYPE_DEBUG` messages logged in the simulator do not show up in the Console.app.
/// Performs the same mapping as ``VBOSLogLevelMapperDefault``, except that `OS_LOG_TYPE_DEBUG` is raised to `OS_LOG_TYPE_DEFAULT`.
/// See [this thread](https://developer.apple.com/forums/thread/82736?answerId=761544022#761544022) in the Apple Developer Forums.
API_AVAILABLE(macos(10.12), ios(10.0), watchos(3.0), tvos(10.0))
VB_SENDABLE
@interface VBOSLogLevelMapperSimulatorConsoleAppWorkaround : VBOSLogLevelMapperDefault
@end
#endif

/// This class provides a logger for the Apple os\_log facility.
API_AVAILABLE(macos(10.12), ios(10.0), watchos(3.0), tvos(10.0))
VB_SENDABLE
@interface VBOSLogger : VBAbstractLogger <VBLogger>

/// The shared instance using `OS_LOG_DEFAULT`.
@property (nonatomic, class, readonly, strong) VBOSLogger *sharedInstance;

/// The log level mapper, that maps ``VBLogFlag``s to ``os_log_type_t`` for this logger.
@property (nonatomic, strong, readonly) id<VBOSLogLevelMapper> logLevelMapper;

/// The designated initializer, using `VBOSLogLevelMapperDefault`.
/// @param subsystem Desired subsystem in log. E.g. "org.example"
/// @param category Desired category in log. E.g. "Point of interests."
/// @discussion This method requires either both or no parameter to be set. Much like `(String, String)?` in Swift.
///             If both parameters are nil, this method will return a logger configured with `OS_LOG_DEFAULT`.
///             If both parameters are non-nil, it will return a logger configured with `os_log_create(subsystem, category)`.
- (instancetype)initWithSubsystem:(nullable NSString *)subsystem 
                         category:(nullable NSString *)category NS_DESIGNATED_INITIALIZER;

/// Creates an instance that uses `OS_LOG_DEFAULT` and `VBOSLogLevelMapperDefault`.
- (instancetype)init;

/// An initializer that in addition to subsystem and category also allows providing the log level mapper.
/// @param subsystem Desired subsystem in log. E.g. "org.example"
/// @param category Desired category in log. E.g. "Point of interests."
/// @param logLevelMapper The log level mapper to use.
/// @discussion This method requires either both or no parameter to be set. Much like `(String, String)?` in Swift.
///             If both parameters are nil, this method will return a logger configured with `OS_LOG_DEFAULT`.
///             If both parameters are non-nil, it will return a logger configured with `os_log_create(subsystem, category)`
- (instancetype)initWithSubsystem:(nullable NSString *)subsystem
                         category:(nullable NSString *)category
                   logLevelMapper:(id<VBOSLogLevelMapper>)logLevelMapper;
// FIXME: This should actually be the designated initializer, but that would be a breaking change. Adjust in next version bump.

/// Creates an instance that uses `OS_LOG_DEFAULT`.
/// @param logLevelMapper The log level mapper to use.
- (instancetype)initWithLogLevelMapper:(id<VBOSLogLevelMapper>)logLevelMapper;

@end

NS_ASSUME_NONNULL_END
