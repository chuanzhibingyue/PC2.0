//
//  common_test.h
//  commontest
//
//  Created by jamesoy on 5/18/16.
//  Copyright © 2016 quanshi. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface common_test : NSObject

+ (void)attachToMainThread;
+ (void)postMessageToMainThread;

@end
