//
//  main.m
//  CarPartsPredicate
//
//  Created by Waqar Malik on 2/1/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Car.h"
#import "Engine.h"
#import "Garage.h"
#import "Slant6.h"
#import "Tire.h"

Car *makeCar (NSString *name, NSString *make, NSString *model,
			  int modelYear, int numberOfDoors, float mileage,
			  int horsepower) NS_RETURNS_NOT_RETAINED;

int main (int argc, const char * argv[])
{
    NSAutoreleasePool *pool;
    pool = [[NSAutoreleasePool alloc] init];
	
    Garage *garage = [[Garage alloc] init];
    garage.name = @"Joe's Garage";
	
	Car *car;
	car = makeCar (@"Herbie", @"Honda", @"CRX", 1984, 2, 34000, 58);
	[garage addCar: car];
    
	NSPredicate *predicate;
	predicate = [NSPredicate predicateWithFormat: @"name == 'Herbie'"];
    BOOL match = [predicate evaluateWithObject: car];
    NSLog (@"%s", (match) ? "YES" : "NO");
    
    predicate = [NSPredicate predicateWithFormat: @"engine.horsepower > 150"];
    match = [predicate evaluateWithObject: car];
	NSLog (@"%s", (match) ? "YES" : "NO");
    
    predicate = [NSPredicate predicateWithFormat: @"name == %@", @"Herbie"];
    match = [predicate evaluateWithObject: car];
  	NSLog (@"%s", (match) ? "YES" : "NO");
    
    predicate = [NSPredicate predicateWithFormat: @"%K == %@", @"name", @"Herbie"];
    match = [predicate evaluateWithObject: car];
  	NSLog (@"%s", (match) ? "YES" : "NO");
    
    NSPredicate *predicateTemplate = [NSPredicate predicateWithFormat:@"name == $NAME"];
    NSDictionary *varDict;
    varDict = [NSDictionary dictionaryWithObjectsAndKeys:
               @"Herbie", @"NAME", nil];
    predicate = [predicateTemplate predicateWithSubstitutionVariables: varDict];
    NSLog(@"SNORGLE: %@", predicate);
    match = [predicate evaluateWithObject: car];
  	NSLog (@"%s", (match) ? "YES" : "NO");
    
    
	car = makeCar (@"Badger", @"Acura", @"Integra", 1987, 5, 217036.7, 130);
	[garage addCar: car];
	
	car = makeCar (@"Elvis", @"Acura", @"Legend", 1989, 4, 28123.4, 151);
	[garage addCar: car];
	
	car = makeCar (@"Phoenix", @"Pontiac", @"Firebird", 1969, 2, 85128.3, 345);
	[garage addCar: car];
	
	car = makeCar (@"Streaker", @"Pontiac", @"Silver Streak", 1950, 2, 39100.0, 36);
	[garage addCar: car];
	
	car = makeCar (@"Judge", @"Pontiac", @"GTO", 1969, 2, 45132.2, 370);
	[garage addCar: car];
	
	car = makeCar (@"Paper Car", @"Plymouth", @"Valiant", 1965, 2, 76800, 105);
	[garage addCar: car];
	
    predicate = [NSPredicate predicateWithFormat: @"engine.horsepower > 150"];
    NSArray *cars = [garage cars];
    for (Car *car in [garage cars]) {
        if ([predicate evaluateWithObject: car]) {
            NSLog (@"%@", car.name);
        }
    }
    
    predicate = [NSPredicate predicateWithFormat: @"engine.horsepower > 150"];
    NSArray *results;
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    NSArray *names;
    names = [results valueForKey:@"name"];
    NSLog (@"%@", names);
    
    NSMutableArray *carsCopy = [cars mutableCopy];
    [carsCopy filterUsingPredicate: predicate];
    NSLog (@"%@", carsCopy);
    [carsCopy release];
    
    predicate = [NSPredicate predicateWithFormat: @"engine.horsepower > %d", 50];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    predicateTemplate = [NSPredicate predicateWithFormat: @"engine.horsepower > $POWER"];
    varDict = [NSDictionary dictionaryWithObjectsAndKeys:
               [NSNumber numberWithInt: 150], @"POWER", nil];
    predicate = [predicateTemplate predicateWithSubstitutionVariables: varDict];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    predicate = [NSPredicate predicateWithFormat:
                 @"(engine.horsepower > 50) AND (engine.horsepower < 200)"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"oop %@", results);
    
    predicate = [NSPredicate predicateWithFormat: @"name < 'Newton'"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", [results valueForKey: @"name"]);
    
    predicate = [NSPredicate predicateWithFormat:
                 @"engine.horsepower BETWEEN { 50, 200 }"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    NSArray *betweens = [NSArray arrayWithObjects:
                         [NSNumber numberWithInt: 50], [NSNumber numberWithInt: 200], nil];
    predicate = [NSPredicate predicateWithFormat: @"engine.horsepower BETWEEN %@", betweens];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    predicateTemplate = [NSPredicate predicateWithFormat: @"engine.horsepower BETWEEN $POWERS"];
    varDict = [NSDictionary dictionaryWithObjectsAndKeys: betweens, @"POWERS", nil];
    predicate = [predicateTemplate predicateWithSubstitutionVariables: varDict];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    predicate = [NSPredicate predicateWithFormat: @"name IN { 'Herbie', 'Snugs', 'Badger', 'Flap' }"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", [results valueForKey: @"name"]);
    
    predicate = [NSPredicate predicateWithFormat: @"SELF.name IN { 'Herbie', 'Snugs', 'Badger', 'Flap' }"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", [results valueForKey: @"name"]);
    
    names = [cars valueForKey: @"name"];
    predicate = [NSPredicate predicateWithFormat: @"SELF IN { 'Herbie', 'Snugs', 'Badger', 'Flap' }"];
    results = [names filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    predicate = [NSPredicate predicateWithFormat: @"name BEGINSWITH 'Bad'"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    predicate = [NSPredicate predicateWithFormat: @"name BEGINSWITH 'HERB'"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    predicate = [NSPredicate predicateWithFormat: @"name BEGINSWITH[cd] 'HERB'"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    predicate = [NSPredicate predicateWithFormat: @"name LIKE[cd] '*er*'"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    predicate = [NSPredicate predicateWithFormat: @"name LIKE[cd] '???er*'"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    NSArray *names1 = [NSArray arrayWithObjects: @"Herbie", @"Badger", @"Judge", @"Elvis", nil];
    NSArray *names2 = [NSArray arrayWithObjects: @"Judge", @"Paper Car", @"Badger", @"Phoenix", nil];
    
    predicate = [NSPredicate predicateWithFormat: @"SELF IN %@", names1];
    results = [names2 filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", predicate);
    NSLog (@"%@", results);
    
    //return 0;
    
    predicate = [NSPredicate predicateWithFormat: @"modelYear > 1970"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
	
    predicate = [NSPredicate predicateWithFormat: @"name contains[cd] 'er'"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
    predicate = [NSPredicate predicateWithFormat: @"name beginswith 'B'"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
	
	predicate = [NSPredicate predicateWithFormat: @"%K beginswith %@",
				 @"name", @"B"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"with args : %@", results);
	
	predicateTemplate = [NSPredicate predicateWithFormat: @"name beginswith $NAME"];
	NSDictionary *dict = [NSDictionary 
						  dictionaryWithObjectsAndKeys: @"Bad", @"NAME", nil];
	predicate = [predicateTemplate predicateWithSubstitutionVariables:dict];
	NSLog (@"SNORGLE: %@", predicate);
	
	predicate = [NSPredicate predicateWithFormat: @"name in { 'Badger', 'Judge', 'Elvis' }"];
	results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
	
	predicateTemplate = [NSPredicate predicateWithFormat: @"name in $NAME_LIST"];
	names = [NSArray arrayWithObjects:@"Badger", @"Judge", @"Elvis", nil];
	dict = [NSDictionary 
            dictionaryWithObjectsAndKeys: names, @"NAME_LIST", nil];
	predicate = [predicateTemplate predicateWithSubstitutionVariables:dict];
	results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
	
	predicateTemplate = [NSPredicate predicateWithFormat: @"%K in $NAME_LIST", @"name"];
	dict = [NSDictionary 
            dictionaryWithObjectsAndKeys: names, @"NAME_LIST", nil];
	predicate = [predicateTemplate predicateWithSubstitutionVariables:dict];
	results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
	NSLog (@"xSNORGLE: %@", predicate);
	
	// SELF is optional here.
	predicate = [NSPredicate predicateWithFormat:@"SELF.name in { 'Badger', 'Judge', 'Elvis' }"];
	
	for (Car *car in cars) {
		if ([predicate evaluateWithObject: car]) {
			NSLog (@"SNORK : %@ matches", car.name);
		}
	}
	
	
#if 0
	predicate = [NSPredicate predicateWithFormat: @"ANY engine.horsepower > 200"];
    results = [cars filteredArrayUsingPredicate: predicate];
	NSLog (@"SNORGLE: %@", predicate);
#endif
	
    predicate = [NSPredicate predicateWithFormat: @"engine.horsepower > 200"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
	
    predicate = [NSPredicate predicateWithFormat: @"tires.@sum.pressure > 10"];
    results = [cars filteredArrayUsingPredicate: predicate];
    NSLog (@"%@", results);
    
#if 0
	predicate = [NSPredicate predicateWithFormat: @"ALL engine.horsepower > 30"];
	results = [cars filteredArrayUsingPredicate: predicate];
	NSLog (@"%@", results);
#endif
	
    [garage release];
    
    [pool release];
    
    return (0);
    
} // main

Car *makeCar (NSString *name, NSString *make, NSString *model,
			  int modelYear, int numberOfDoors, float mileage,
			  int horsepower)
{
	Car *car = [[[Car alloc] init] autorelease];
	
	car.name = name;
	car.make = make;
	car.model = model;
	car.modelYear = modelYear;
	car.numberOfDoors = numberOfDoors;
	car.mileage = mileage;
	
	Slant6 *engine = [[[Slant6 alloc] init] autorelease];
	[engine setValue: [NSNumber numberWithInt: horsepower]
			  forKey: @"horsepower"];
	car.engine = engine;
	
	// Make some tires.
	// int i;
	for (int i = 0; i < 4; i++)
    {
		Tire * tire= [[[Tire alloc] init] autorelease];
		[car setTire: tire  atIndex: i];
	}
	
	return (car);
	
} // makeCar