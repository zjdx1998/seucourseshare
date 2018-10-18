//
//  main.m
//  RetainCount
//
//  Created by Waqar Malik on 3/30/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface RetainTracker : NSObject
@end // RetainTracker

@implementation RetainTracker
- (id)init
{
	if(self = [super init])
    {
		NSLog (@"init: Retain count of %lu.", [self retainCount]);
	}
	
	return (self);
} // init


- (void) dealloc
{
	NSLog (@"dealloc called. Bye Bye.");
	[super dealloc];
	
} // dealloc

@end // RetainTracker

int main(int argc, const char * argv[])
{
    RetainTracker *tracker = [RetainTracker new];
    // count: 1
	
    [tracker retain]; // count: 2
    NSLog (@"%lu", [tracker retainCount]);
	
    [tracker retain]; // count: 3
    NSLog (@"%lu", [tracker retainCount]);
	
    [tracker release]; // count: 2
    NSLog (@"%lu", [tracker retainCount]);
	
    [tracker release]; // count: 1
    NSLog (@"%lu", [tracker retainCount]);
	
    [tracker retain]; // count 2
    NSLog (@"%lu", [tracker retainCount]);
	
    [tracker release]; // count 1
    NSLog (@"%lu", [tracker retainCount]);
	
    [tracker release]; // count: 0, dealloc it
	
    return (0);
}

