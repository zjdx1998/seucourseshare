//
//  main.m
//  CarParts
//
//  Created by Waqar Malik on 3/30/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "Tire.h"	// don't really have to #import this or the next header file.
#import "Engine.h"	// since they're brought in by Slant6.h and AllWeatherRadial.h
#import "Car.h"
#import "Slant6.h"
#import "AllWeatherRadial.h"

int main(int argc, const char * argv[])
{
	Car *car = [[Car alloc] init];
	
	for (int i = 0; i < 4; i++)
    {
		Tire *tire = [[AllWeatherRadial alloc] init];
		
		[car setTire:tire atIndex:i];
	}
	
	Engine *engine = [[Engine alloc] init];
	[car setEngine:engine];
	
	[car print];

	return (0);
}

