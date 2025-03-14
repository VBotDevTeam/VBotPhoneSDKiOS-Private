//
//  VBotEndpoint.h
//  Copyright © 2020 VPMedia. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CocoaLumberjack/CocoaLumberjack.h>
#import <VBotPJSIP/pjsua.h>
#import "VBotEndpointConfiguration.h"

@class VBotAccount;
@class VBotCall;
@class VBotTransportConfiguration;

/**
 *  Possible errors the Endpoint can return.
 */
typedef NS_ENUM(NSInteger, VBotEndpointError) {
    /**
     *  Unable to create the pjsip library.
     */
    VBotEndpointErrorCannotCreatePJSUA,
    /**
     *  Unable to initialize the pjsip library.
     */
    VBotEndpointErrorCannotInitPJSUA,
    /**
     *  Unable to add transport configuration to endpoint.
     */
    VBotEndpointErrorCannotAddTransportConfiguration,
    /**
     *  Unable to start the pjsip library.
     */
    VBotEndpointErrorCannotStartPJSUA,
    /**
     *  Unable to create the thread for pjsip.
     */
    VBotEndpointErrorCannotCreateThread
};
#define VBotEndpointErrorString(VBotEndpointError) [@[@"VBotEndpointErrorCannotCreatePJSUA", @"VBotEndpointErrorCannotInitPJSUA", @"VBotEndpointErrorCannotAddTransportConfiguration", @"VBotEndpointErrorCannotStartPJSUA", @"VBotEndpointErrorCannotCreateThread"] objectAtIndex:VBotEndpointError]

/**
 *  Possible states for the Endpoint.
 */
typedef NS_ENUM(NSInteger, VBotEndpointState) {
    /**
     *  Endpoint not active.
     */
    VBotEndpointStopped,
    /**
     *  Endpoint is starting.
     */
    VBotEndpointStarting,
    /**
     *  Endpoint is running.
     */
    VBotEndpointStarted,
    /**
     *  Endpoint is closing.
     */
    VBotEndpointClosing,
};
#define VBotEndpointStateString(VBotEndpointState) [@[@"VBotEndpointStopped", @"VBotEndpointStarting", @"VBotEndpointStarted"] objectAtIndex:VBotEndpointState]

@interface VBotEndpoint : NSObject

/**
 *  Current state of the endpoint.
 */
@property (nonatomic) VBotEndpointState state;

/**
 *  The pool associated with the endpoint.
 */
@property (readonly) pj_pool_t * _Nullable pjPool;

@property (readwrite) BOOL ipChangeInProgress;

/**
 *  The incomingCallBlock will be called when an incoming call is received by pjsip.
 */
@property (copy, nonatomic) void (^ _Nonnull incomingCallBlock)(VBotCall * _Nullable call);

/**
 * The missedCallBlock will be called when a call is completed elsewhere or the original call hung up.
 */
@property (copy, nonatomic) void(^ _Nonnull missedCallBlock)(VBotCall * _Nullable call);

/**
 The logCallBackBlock will be called when there is a log message to be shown.
 */
@property (copy, nonatomic) void( ^ _Nonnull logCallBackBlock)(DDLogMessage * _Nonnull logMessage);

/**
 *  References to the account that have been added to the endpoint.
 *  To add accounts as reference use the addAccount function.
 *  To remove accounts use the removeAccount function.
 */
@property (readonly, nonatomic) NSArray * _Nullable accounts;

/**
 *  The endpoint configuration that has been set in the configure function for the endpoint.
 */
@property (readonly) VBotEndpointConfiguration * _Nonnull endpointConfiguration;

/**
 *  The shared instance for the endpoint.
 *
 *  @return The singleton instance.
 */
+ (instancetype _Nonnull)sharedEndpoint;

/**
 *  This will remove the pjsua configuration.
 */
- (void)destroyPJSUAInstance;

/**
 *  This will configure the endpoint with pjsua.
 *
 *  @param endpointConfiguration Instance of an end point configuration.
 *  @param error                 Pointer to NSError pointer. Will be set to a NSError instance if cannot start endpoint.
 *
 *  @return BOOL success of configuration.
 */
- (BOOL)startEndpointWithEndpointConfiguration:(VBotEndpointConfiguration  * _Nonnull)endpointConfiguration error:(NSError * _Nullable * _Nullable)error;

/**
 *  This will add the account as reference to the endpoint.
 *
 *  @param account The account that has been added.
 */
- (void)addAccount:(VBotAccount * _Nonnull)account;

/**
 *  This will search for the account given the accountId.
 *
 *  @param accountId ID of the account.
 *
 *  @return VBotAccount Instance if found.
 */
- (VBotAccount * _Nullable)lookupAccount:(NSInteger)accountId;

/**
 *  This will remove the account reference in the endpoint.
 *
 *  @param account The account that needs to be removed.
 */
- (void)removeAccount:(VBotAccount * _Nonnull)account;

/**
 *  Returns an account if it is available.
 *
 *  @param sipAccount NSString the sip username you want to check.
 *
 *  @return VBotAccount instance of the account.
 */
- (VBotAccount * _Nullable)getAccountWithSipAccount:(NSString * _Nonnull)sipAccount;

/**
 *  This will update the codec configuration.
 *
 *  @param codecConfiguration VBotCodecConfiguration Instance of an VBotCodecConfiguration
 */
- (BOOL)updateCodecConfiguration:(VBotCodecConfiguration * _Nonnull)codecConfiguration;
@end
