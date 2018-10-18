//
//  main.m
//  CarPartsInit-GC
//
//  Created by Waqar Malik on 4/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Car.h"
#import "Slant6.h"
#import "AllWeatherRadial.h"

int main(int argc, const char * argv[])
{
    Car *car = [[Car alloc] init];
	
	for (int i = 0; i < 4; i++)
    {
		AllWeatherRadial *tire;
		
		tire = [[AllWeatherRadial alloc] init];
		
		[car setTire: tire
			 atIndex: i];
	}
	
	Engine *engine = [[Slant6 alloc] init];
	[car setEngine: engine];
	
	[car print];
    
    return 0;
}

