//
//  main.m
//  CarParts-2
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

@interface AllWeatherRadial : Tire
@end // AllWeatherRadial


@implementation AllWeatherRadial

- (NSString *) description
{
	return (@"I am a tire for rain or shine.");
} // description

@end // AllWeatherRadial


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

@interface Slant6 : Engine
@end // Slant6


@implementation Slant6

- (NSString *) description
{
	return (@"I am a slant-6. VROOOM!");
} // description

@end // Slant6




// --------------------------------------------------

@interface Car : NSObject
{
	Engine *engine;
	Tire *tires[4];
}

- (Engine *) engine;

- (void) setEngine: (Engine *) newEngine;

- (Tire *) tireAtIndex: (int) index;

- (void) setTire: (Tire *) tire atIndex: (int) index;

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


- (Engine *) engine
{
	return (engine);
} // engine


- (void) setEngine: (Engine *) newEngine
{
	engine = newEngine;
} // setEngine


- (void) setTire: (Tire *) tire atIndex: (int) index
{
	if (index < 0 || index > 3) {
		NSLog (@"bad index (%d) in setTire:atIndex:", index);
		exit (1);
	}
	
	tires[index] = tire;
	
} // setTire:atIndex:


- (Tire *) tireAtIndex: (int) index
{
	if (index < 0 || index > 3) {
		NSLog (@"bad index (%d) in tireAtIndex:", index);
		exit (1);
	}
	
	return (tires[index]);
	
} // tireAtIndex:



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
	Car *car = [Car new];
	
	for (int i = 0; i < 4; i++) {
		Tire *tire = [AllWeatherRadial new];
		
		[car setTire: tire
			 atIndex: i];
	}
	
	Engine *engine = [Slant6 new];
	[car setEngine: engine];
	
	[car print];
	
	return (0);
	
} // main