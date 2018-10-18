//
//  SelectorTester.m
//  Selectors
//
//  Created by Waqar Malik on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "SelectorTester.h"

@implementation SelectorTester

- (void)runSelectors
{
    [self performSelector:@selector(myBackgroundMehod1)];
    [self performSelector:@selector(myBackgroundMethod2:) withObject:@"Hello Selector"];
    NSLog(@"Done performing selectors");
}

- (void)myBackgroundMehod1
{
    @autoreleasepool
    {
        NSLog(@"myBackgroundMehod1");
    }
}

- (void)myBackgroundMethod2:(id)object
{
    @autoreleasepool
    {
        NSLog(@"myBackgroundMethod2 %@", object);
    }
}

@end
