//
//  Manager.m
//  Delegation
//
//  Created by Waqar Malik on 3/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Manager.h"

@interface Manager ()
- (void)myWork;
@end

@implementation Manager
@synthesize delegate;

- (void)doWork
{
    [delegate doSomeRequiredWork];
    
    if(YES == [delegate respondsToSelector:@selector(doSomeOptionalWork)])
    {
        [delegate doSomeOptionalWork];
    }
    
    [self myWork];
}

- (void)myWork;
{
    NSLog(@"I am a manager and I am working");
}
@end
