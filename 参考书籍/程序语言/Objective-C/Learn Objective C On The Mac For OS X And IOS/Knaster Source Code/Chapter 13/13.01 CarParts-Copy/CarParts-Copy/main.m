//
//  main.m
//  CarParts-Copy
//
//  Created by Waqar Malik on 3/28/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "Car.h"
#import "Slant6.h"
#import "AllWeatherRadial.h"

int main (int argc, const char * argv[])
{
    @autoreleasepool
    {
        Car *car = [[Car alloc] init];
        car.name = @"Herbie";
        
        for (NSInteger i = 0; i < 4; i++)
        {
            AllWeatherRadial *tire;
            tire = [[AllWeatherRadial alloc] init];
            
            [car setTire: tire atIndex: i];
        }
        
        Slant6 *engine = [[Slant6 alloc] init];
        car.engine = engine;
        
        [car print];
        
        Car *carCopy = [car copy];
        [carCopy print];
        
	}
	return (0);
} // main
