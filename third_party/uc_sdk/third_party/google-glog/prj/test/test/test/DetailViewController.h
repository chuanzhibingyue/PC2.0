//
//  DetailViewController.h
//  test
//
//  Created by tangapi on 15/1/29.
//  Copyright (c) 2015年 UC. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DetailViewController : UIViewController

@property (strong, nonatomic) id detailItem;
@property (weak, nonatomic) IBOutlet UILabel *detailDescriptionLabel;

@end

