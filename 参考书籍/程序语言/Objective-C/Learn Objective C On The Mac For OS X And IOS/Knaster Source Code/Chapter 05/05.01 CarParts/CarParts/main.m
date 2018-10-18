//
//  main.m
//  CarParts
//
//  Created by Waqar Malik on 4/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

// --------------------------------------------------

@interface Tire : NSObject
@end // Tire


@implementation Tire

- (NSString *) description
{
    return (@"I am a tire. I last a while");
} // description

@end // Tire



// --------------------------------------------------

@interface Engine : NSObject
@end // Engine


@implementation Engine

- (NSString *) description
{
	return (@"I am an engine.  Vrooom!");
} // description

@end // Engine


// --------------------------------------------------

@interface Car : NSObject
{
	Engine *engine;
	Tire *tires[4];
}

- (void) print;

@end // Car


@implementation Car

- (id) init
{
	if (self = [super init]) {
		engine = [Engine new];
		
		tires[0] = [Tire new];
		tires[1] = [Tire new];
		tires[2] = [Tire new];
		tires[3] = [Tire new];
	}
	
	return (self);
	
} // init

- (void) print
{
	NSLog (@"%@", engine);
	
	NSLog (@"%@", tires[0]);
	NSLog (@"%@", tires[1]);
	NSLog (@"%@", tires[2]);
	NSLog (@"%@", tires[3]);
	
} // print

@end // Car


// --------------------------------------------------

int main (int argc, const char * argv[]) 
{
	Car *car;
	
	car = [Car new];
	[car print];
	
	return (0);
	
} // main