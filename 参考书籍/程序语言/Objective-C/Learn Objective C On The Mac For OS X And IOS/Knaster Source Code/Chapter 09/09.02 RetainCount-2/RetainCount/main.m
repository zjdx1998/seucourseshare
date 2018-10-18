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

- (id) init
{
	if (self = [super init])
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
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
    RetainTracker *tracker = [RetainTracker new]; // count: 1
	
    [tracker retain]; // count: 2
    [tracker autorelease]; // count: still 2
    [tracker release]; // count: 1
	
    NSLog (@"releasing pool");
    [pool release]; 
    // gets nuked, sends release to tracker
	
    @autoreleasepool
    {
        RetainTracker *tracker2 = [RetainTracker new]; // count: 1
        
        [tracker2 retain]; // count: 2
        [tracker2 autorelease]; // count: still 2
        [tracker2 release]; // count: 1
        
        NSLog (@"auto releasing pool");
    }
    
    return (0);
}

