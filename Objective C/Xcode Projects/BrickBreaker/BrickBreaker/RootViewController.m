//
//  RootViewController.m
//  BrickBreaker
//
//  Created by ROSE JEFFREY D. on 2014-01-29.
//  Copyright (c) 2014 Axon Interactive Inc. All rights reserved.
//

#import "RootViewController.h"

@interface RootViewController ()

@end

@implementation RootViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        [self initializeTimer];
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)startPlaying
{
    
}

-(void)initializeTimer
{
    float interval = 1.0f / 30.0f;
    [NSTimer scheduledTimerWithTimeInterval:interval target:self selector:@selector(update) userInfo:nil repeats:YES];
}

-(void)update
{
    NSLog(@"NaN");
}

@end
