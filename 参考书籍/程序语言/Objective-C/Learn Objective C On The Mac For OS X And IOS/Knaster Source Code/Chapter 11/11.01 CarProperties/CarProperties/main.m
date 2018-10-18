//
//  main.m
//  CarProperties
//
//  Created by Waqar Malik on 3/30/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Car.h"
#import "Slant6.h"
#import "AllWeatherRadial.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        Car *car = [[Car alloc] init];
        car.name = @"Herbie";
        
        for (int i = 0; i < 4; i++)
        {
            AllWeatherRadial *tire;

            tire = [[AllWeatherRadial alloc] init];
            tire.rainHandling = 20+i;
            tire.snowHandling = 28+i;
            NSLog(@"tire %d's handling is %.f %.f", i, tire.rainHandling, tire.snowHandling);

            [car setTire:tire atIndex:i];
        }
        
        car.engine = [[Slant6 alloc] init];;
        
        [car print];
    }
	return (0);
}

