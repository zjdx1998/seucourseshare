//
//  main.m
//  Car-Value-Coding
//
//  Created by Waqar Malik on 4/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Foundation/Foundation.h>
#import "Car.h"
#import "Slant6.h"
#import "AllWeatherRadial.h"

Car *makeCustomizedCopy (Car *car, NSString *name, NSString *make, NSString *model,
						 int modelYear, int numberOfDoors, float mileage) {
	Car *copy = [[car copy] autorelease];
	copy.name = name;
	copy.make = make;
	copy.model = model;
	copy.modelYear = modelYear;
	copy.mileage = mileage;
	
	return (copy);
	
} // makeCustomizedCopy


int main (int argc, const char * argv[])
{
	@autoreleasepool
    {
        Car *car = [[[Car alloc] init] autorelease];
        car.name = @"Herbie";
        car.make = @"Honda";
        car.model = @"CRX";
        car.modelYear = 1984;
        car.numberOfDoors = 2;
        car.mileage = 110000;
        
        for (int i = 0; i < 4; i++)
        {
            AllWeatherRadial *tire;
            tire = [[AllWeatherRadial alloc] init];
            [car setTire: tire atIndex: i];
            [tire release];
        }
        
        Slant6 *engine = [[[Slant6 alloc] init] autorelease];
        car.engine = engine;
        
        NSLog (@"horsepower is %@", [engine valueForKey: @"horsepower"]);
        [engine setValue: [NSNumber numberWithInt: 150]
                  forKey: @"horsepower"];
        NSLog (@"horsepower is %@", [engine valueForKey: @"horsepower"]);
        
        NSLog (@"Car is %@", car);
        
        NSString *name = [car valueForKey:@"name"];
        NSLog (@"%@", name);
        NSLog (@"make is %@", [car valueForKey:@"make"]);
        NSLog (@"model year is %@", [car valueForKey: @"modelYear"]);
        
        [car setValue: @"Harold" forKey: @"name"];
        NSLog (@"new car name is %@", [car name]);
        
        [car setValue: [NSNumber numberWithFloat: 25062.4]
               forKey: @"mileage"];
        NSLog (@"new mileage is %.1f", [car mileage]);
        
        [car setValue: [NSNumber numberWithInt: 155]
           forKeyPath: @"engine.horsepower"];
        NSLog (@"horsepower is %@", [car valueForKeyPath: @"engine.horsepower"]);
        
        NSArray *pressures = [car valueForKeyPath: @"tires.pressure"];
        NSLog (@"pressures %@", pressures);
	}
	
	return (0);
	
} // main
