//
//  main.m
//  test
//
//  Created by tangapi on 15/1/29.
//  Copyright (c) 2015年 UC. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

#include "glog_mgr.h"

int main(int argc, char * argv[]) {
    
    LogCfg log_cfg;
    log_cfg.cfg_file_path = "/Users/tangapi/Desktop/zhaotao/open_log/Client_Log_Config.cfg";
    log_cfg.program_name = "uc_client";
    log_cfg.log_name = "A_client.log";
    
    GlogMrg::get_instance().init(log_cfg);
    
    for (int i = 0; i < 40000000; i++)
    {
        LOG(INFO) << "The info Valus is " << i;
        LOG(WARNING) << "The warning Valus is " << i;
        LOG(ERROR) << "The error Valus is " << i;
        LOG_IF(INFO, true) << "The condition Valus is " << i;
    }
    
    GlogMrg::get_instance().uninit();
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
