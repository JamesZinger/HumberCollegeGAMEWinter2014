//
//  RockPaperScissors.m
//  RockPaperScissors
//
//  Created by ROSE JEFFREY D. on 2014-01-29.
//  Copyright (c) 2014 ROSE JEFFREY D. All rights reserved.
//

#import "RockPaperScissors.h"

@implementation RockPaperScissors

- (id)initWithCoder:(NSCoder *)aDecoder
{
    //NSLog(@"START");
    
    self = [super initWithCoder:aDecoder];
    if (self)
    {
        srand(time( NULL ));
        
        CGSize size = CGSizeMake(320, 480);
        
        buttonView = [[UIView alloc] initWithFrame:CGRectMake( 0, 0, size.width, size.height )];
        [buttonView setBackgroundColor:[UIColor lightGrayColor]];
        [self addSubview:buttonView];
        
        float sixtyPercent = size.width * 0.6;
        float twentyPercent = size.width * 0.2;
        
        float twentyFivePercent = size.height * 0.25;
        float thirtyThreePercent = size.height * 0.33;
        
        rockButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [rockButton setFrame:CGRectMake(twentyPercent, twentyFivePercent, sixtyPercent, 40)];
        [rockButton setTitle:@"Rock" forState:UIControlStateNormal];
        [rockButton addTarget:self action:@selector(userSelected:) forControlEvents:UIControlEventTouchUpInside];
        
        paperButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [paperButton setFrame:CGRectMake(twentyPercent, twentyFivePercent * 2, sixtyPercent, 40)];
        [paperButton setTitle:@"Paper" forState:UIControlStateNormal];
        [paperButton addTarget:self action:@selector(userSelected:) forControlEvents:UIControlEventTouchUpInside];
        
        scissorsButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [scissorsButton setFrame:CGRectMake(twentyPercent, twentyFivePercent * 3, sixtyPercent, 40)];
        [scissorsButton setTitle:@"Scissors" forState:UIControlStateNormal];
        [scissorsButton addTarget:self action:@selector(userSelected:) forControlEvents:UIControlEventTouchUpInside];
        
        [buttonView addSubview:rockButton];
        [buttonView addSubview:paperButton];
        [buttonView addSubview:scissorsButton];
        
        resultView = [[UIView alloc] initWithFrame:CGRectMake( 0, 0, size.width, size.height )];
        [resultView setBackgroundColor:[UIColor lightGrayColor]];
        
        resultLabel = [[UILabel alloc] initWithFrame:CGRectMake(twentyPercent, thirtyThreePercent, sixtyPercent, 40)];
        [resultLabel setBackgroundColor:[UIColor whiteColor]];
        [resultLabel setAdjustsFontSizeToFitWidth:YES];
        
        [resultView addSubview:resultLabel];
        
        continueButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [continueButton setFrame:CGRectMake(twentyPercent, thirtyThreePercent * 2, sixtyPercent, 40)];
        [continueButton setTitle:@"Continue" forState:UIControlStateNormal];
        [continueButton addTarget:self action:@selector(continueGame:) forControlEvents:UIControlEventTouchUpInside];
        
        [resultView addSubview:continueButton];
        
        //NSLog(@"WORDS");
    }
    return self;
}

-(void)userSelected:(id)sender
{
    UIButton* selectedButton = (UIButton*)sender;
    NSString* playerSelectionString = [[selectedButton titleLabel] text];
    int computerSelection = random() % 3;
    
    //NSLog(@"Button Clicked");
    
    int playerSelection = 0;
    if ([playerSelectionString isEqualToString:@"Rock"])
    {
        playerSelection = 0;
    }
    else if ([playerSelectionString isEqualToString:@"Paper"])
    {
        playerSelection = 1;
    }
    else
    {
        playerSelection = 2;
    }
    
    NSString* resultString;
    if (playerSelection == computerSelection)
    {
        //NSLog(@"Tie");
        resultString = @"Player tied with the computer.";
    }
    else if (playerSelection == computerSelection + 1 || playerSelection == computerSelection - 2)
    {
        //NSLog(@"Player Wins");
        resultString = @"Player wins!";
    }
    else
    {
        //NSLog(@"Computer Wins");
        resultString = @"Computer wins!";
    }
    
    [resultLabel setText:resultString];
    [self addSubview:resultView];
}

-(void)continueGame:(id)sender
{
    [resultView removeFromSuperview];
    [self addSubview:buttonView];
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
