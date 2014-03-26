//
//  RockPaperScissors.h
//  RockPaperScissors
//
//  Created by ROSE JEFFREY D. on 2014-01-29.
//  Copyright (c) 2014 ROSE JEFFREY D. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface RockPaperScissors : UIView

{
    UIView* buttonView;
    UIButton* rockButton;
    UIButton* paperButton;
    UIButton* scissorsButton;
    UIView* resultView;
    UILabel* resultLabel;
    UIButton* continueButton;
}

-(void)userSelected:(id)sender;
-(void)continueGame:(id)sender;
-(NSString*)getLostTo:(NSString*)selection;
-(NSString*)getWonTo:(NSString*)selection;

@end
