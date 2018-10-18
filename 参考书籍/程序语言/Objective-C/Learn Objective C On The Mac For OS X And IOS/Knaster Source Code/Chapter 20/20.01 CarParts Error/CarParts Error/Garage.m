//
//  Garage.m
//  15.01 Car-Value-Coding
//
//  Created by markd on 8/16/08.
//  Copyright 2008 Length-O-Words.com. All rights reserved.
//

#import "Garage.h"

@implementation Garage

@synthesize name;
@synthesize cars;

- (void) addCar: (Car *) car {
	if (cars == nil) {
		cars = [[NSMutableArray alloc] init];
	}
	[cars addObject: car];
	
} // addCar


- (void) dealloc
{
	[name release];
	[cars release];
	[stuff release];
	[super dealloc];
} // dealloc


- (void) print {
	NSLog (@"%@:", name);
	
	for (Car *car in cars)
    {
		NSLog (@"    %@", car);
	}
	
} // print


- (void) setValue: (id) value  forUndefinedKey: (NSString *) key
{
	if (stuff == nil) {
		stuff = [[NSMutableDictionary alloc] init];
	}
	[stuff setValue: value forKey: key];
} // setValueForUndefinedKey


- (id) valueForUndefinedKey:(NSString *)key
{
	id value = [stuff valueForKey: key];
	return (value);
} // valueForUndefinedKey


@end  // Car
