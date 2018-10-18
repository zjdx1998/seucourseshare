//
//  NSString+NumberConvenience.m
//  12.01 LengthAsNSNumber
//
//  Created by Waqar Malik on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "NSString+NumberConvenience.h"

@implementation NSString (NumberConvenience)
- (NSNumber *)lengthAsNumber
{
    NSUInteger length = [self length];
    
	return ([NSNumber numberWithUnsignedInt:length]);
	
} // lengthAsNumber
@end
