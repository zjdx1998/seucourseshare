//
//  main.m
//  Objective Blocks
//
//  Created by Waqar Malik on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "ProcessStrings.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        ProcessStrings *myStringProcessor = [[ProcessStrings alloc] init];
        myStringProcessor.theString = @"Hello Objective Blocks!";
        
        [myStringProcessor testMyString];
    }
    return 0;
}

