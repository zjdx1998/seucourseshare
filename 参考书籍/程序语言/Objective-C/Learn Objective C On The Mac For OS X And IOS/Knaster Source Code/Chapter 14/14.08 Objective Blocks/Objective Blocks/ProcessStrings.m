//
//  ProcessStrings.m
//  Objective Blocks
//
//  Created by Waqar Malik on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ProcessStrings.h"

@implementation ProcessStrings
@synthesize theString = _theString;

- (void)testMyString
{
    NSString *string1 = ^{
        return [_theString stringByAppendingString:_theString];
    };
    
    NSString *localObject = _theString;
    NSString *string2 = ^{
        return [localObject stringByAppendingString:localObject];
    };
}
@end
