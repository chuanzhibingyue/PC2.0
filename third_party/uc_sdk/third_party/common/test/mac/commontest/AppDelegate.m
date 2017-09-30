//
//  AppDelegate.m
//  commontest
//
//  Created by jamesoy on 5/18/16.
//  Copyright © 2016 quanshi. All rights reserved.
//

#import "AppDelegate.h"
#import "common_test.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
   
    [common_test attachToMainThread];
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        [common_test postMessageToMainThread];
    });
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end
