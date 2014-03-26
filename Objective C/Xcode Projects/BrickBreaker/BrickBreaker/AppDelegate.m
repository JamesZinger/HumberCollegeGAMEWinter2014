//
//  AppDelegate.m
//  BrickBreaker
//
//  Created by ROSE JEFFREY D. on 2014-01-29.
//  Copyright (c) 2014 Axon Interactive Inc. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
    
    // self.rootViewController = [[RootViewController alloc] init];
    self.rootViewController = [[RootViewController alloc] initWithNibName:@"RootViewController" bundle:nil];
    
    self.window.rootViewController = self.rootViewController;
    
    self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
    return YES;
}

@end
