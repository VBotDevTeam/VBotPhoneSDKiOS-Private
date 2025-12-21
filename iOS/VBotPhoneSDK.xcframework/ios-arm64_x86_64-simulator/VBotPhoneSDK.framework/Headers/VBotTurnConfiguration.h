//
//  VBotTurnConfiguration.h
//  VBotSIP
//
//  Copyright © 2020 VPMedia. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <VBotPJSIP/pjsua.h>

typedef NS_ENUM(NSUInteger, VBotStunPasswordType) {
    VBotStunPasswordTypePlain = PJ_STUN_PASSWD_PLAIN,
    VBotStunPasswordTypeHashed = PJ_STUN_PASSWD_HASHED,
};

@interface VBotTurnConfiguration : NSObject
@property (nonatomic, assign) BOOL enableTurn;
@property (nonatomic, assign) VBotStunPasswordType passwordType;

@property (nonatomic, strong) NSString * _Nullable server;
@property (nonatomic, strong) NSString * _Nullable username;
@property (nonatomic, strong) NSString * _Nullable password;
@end
