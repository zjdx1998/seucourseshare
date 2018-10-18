//
//  MSCViewController.h
//  CaseTool
//
//  Created by Waqar Malik on 1/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MSCViewController : UIViewController

@property (weak, nonatomic) IBOutlet UITextField *textField;
@property (weak, nonatomic) IBOutlet UILabel *resultsField;

- (IBAction)upppercase;
- (IBAction)lowercase;

@end
