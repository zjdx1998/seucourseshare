//
//  Engine.m
//  CarParts-Copy
//
//  Created by Waqar Malik on 3/28/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Engine.h"

@implementation Engine

- (id)copyWithZone:(NSZone *)zone
{
	Engine *engineCopy = [[[self class] allocWithZone:zone] init];
	
	return (engineCopy);
	
} // copyWithZone


- (NSString *)description
{
    return (@"I am an engine.  Vrooom!");
} // description

@end // Engine

