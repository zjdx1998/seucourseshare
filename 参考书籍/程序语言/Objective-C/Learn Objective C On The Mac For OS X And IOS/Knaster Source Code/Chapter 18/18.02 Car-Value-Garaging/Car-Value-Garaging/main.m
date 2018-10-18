//
//  main.m
//  Car-Value-Garaging
//
//  Created by Waqar Malik on 4/25/12.
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
			  int horsepower) {
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
	for (int i = 0; i < 4; i++) {
		Tire * tire= [[[Tire alloc] init] autorelease];
		[car setTire: tire  atIndex: i];
	}
	
	return (car);
	
} // makeCar


int main (int argc, const char * argv[])
{
	@autoreleasepool
    {
        Garage *garage = [[Garage alloc] init];
        garage.name = @"Joe's Garage";
        
        Car *car;
        car = makeCar (@"Herbie", @"Honda", @"CRX", 1984, 2, 110000, 58);
        [garage addCar: car];
        
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
        
        [garage print];
        
        NSNumber *count;
        count = [garage valueForKeyPath: @"cars.@count"];
        NSLog (@"We have %@ cars", count);
        
        NSNumber *sum;
        sum = [garage valueForKeyPath: @"cars.@sum.mileage"];
        NSLog (@"We have a grand total of %@ miles", sum);
        
        NSNumber *avgMileage;
        avgMileage = [garage valueForKeyPath: @"cars.@avg.mileage"];
        NSLog (@"average is %.2f", [avgMileage floatValue]);
        
        NSNumber *min, *max;
        min = [garage valueForKeyPath: @"cars.@min.mileage"];
        max = [garage valueForKeyPath: @"cars.@max.mileage"];
        NSLog (@"minimax: %@ / %@", min, max);
        
        NSArray *manufacturers;
        manufacturers = [garage valueForKeyPath: @"cars.@distinctUnionOfObjects.make"];
        NSLog (@"makers: %@", manufacturers);
        
        car = [[garage valueForKeyPath: @"cars"] lastObject];
        
        NSArray *keys = [NSArray arrayWithObjects: @"make", @"model", @"modelYear", nil];
        NSDictionary *carValues = [car dictionaryWithValuesForKeys: keys];
        NSLog (@"Car values : %@", carValues);
        
        NSDictionary *newValues =
        [NSDictionary dictionaryWithObjectsAndKeys:
         @"Chevy", @"make",
         @"Nova", @"model",
         [NSNumber numberWithInt:1964], @"modelYear",
         [NSNull null], @"mileage",
         nil];
        [car setValuesForKeysWithDictionary: newValues];
        NSLog (@"car with new values is %@", car);
        
        [car setValue:nil forKey: @"mileage"];
        NSLog (@"Nil miles are %@", car.mileage);
        
        [garage setValue: @"bunny" forKey: @"fluffy"];
        [garage setValue: @"greeble" forKey: @"bork"];
        [garage setValue: [NSNull null] forKey: @"snorgle"];
        [garage setValue: nil forKey: @"gronk"];
        
        NSLog (@"values are %@ %@ %@ and %@",
               [garage valueForKey: @"fluffy"],
               [garage valueForKey: @"bork"],
               [garage valueForKey: @"snorgle"],
               [garage valueForKey: @"gronk"]);
        
        NSLog (@"%@", [garage valueForKey: @"stuff"]);
        
        [garage release];
	}
	return (0);
} // main
