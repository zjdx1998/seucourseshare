//
//  Car.m
//  CarParts-Copy
//
//  Created by Waqar Malik on 3/28/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Car.h"
#import "Engine.h"
#import "Tire.h"

@implementation Car
{
    NSMutableArray *tires;
}

@synthesize name = appellation;
@synthesize engine;

- (id) init
{
    if (self = [super init]) {

		self.name = @"Car";
        tires = [[NSMutableArray alloc] init];

        for (NSInteger i = 0; i < 4; i++)
        {
            [tires addObject: [NSNull null]];
        }
    }

    return (self);

} // init


- (id)copyWithZone:(NSZone *)zone
{
	Car *carCopy = [[[self class] allocWithZone:zone] init];
	
	carCopy.name = self.name;
	
	Engine *engineCopy = [engine copy];
	carCopy.engine = engineCopy;
	
	for(NSInteger i = 0; i < 4; i++)
    {
		Tire *tireCopy = [[self tireAtIndex:i] copy];
		
		[carCopy setTire:tireCopy atIndex:i];
	}
	
	return (carCopy);
	
} // copyWithZone

- (void)setTire:(Tire *)tire atIndex:(NSInteger)index
{
    [tires replaceObjectAtIndex:index withObject:tire];

} // setTire:atIndex:


- (Tire *)tireAtIndex:(NSInteger)index
{
    Tire *tire = [tires objectAtIndex:index];

    return (tire);

} // tireAtIndex:


- (void) print
{
	NSLog (@"%@ has:", self.name);
    for (NSInteger i = 0; i < 4; i++)
    {
        NSLog (@"%@", [self tireAtIndex:i]);
    }

    NSLog (@"%@", engine);

} // print

@end // Car

