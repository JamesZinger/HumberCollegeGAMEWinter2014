//
//  RootViewController.h
//  BrickBreaker
//
//  Created by ROSE JEFFREY D. on 2014-01-29.
//  Copyright (c) 2014 Axon Interactive Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface RootViewController : UIViewController
{
    UIImageView* paddle;
}

@property (strong, nonatomic) IBOutlet UILabel* livesLabel;
@property (strong, nonatomic) IBOutlet UILabel* scoreLabel;

@property (strong, nonatomic) IBOutlet UIImageView* ballImageView;
@property (strong, nonatomic) IBOutlet UIImageView* paddleImageView;

// Functions
-(void)startPlaying;
-(void)initializeTimer;
-(void)update;

@end
