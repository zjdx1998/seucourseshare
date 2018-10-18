//
//  main.m
//  CarParts-Split
//
//  Created by Waqar Malik on 4/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "Tire.h"	// don't really have to #import this or the next header file.
#import "Engine.h"	// since they're brought in by Slant6.h and AllWeatherRadial.h
#import "Car.h"
#import "Slant6.h"
#import "AllWeatherRadial.h"

int main (int argc, const char * argv[]) 
{
	Car *car = [Car new];
	
	int i;
	for (i = 0; i < 4; i++) {
		Tire *tire = [AllWeatherRadial new];
		
		[car setTire: tire
			 atIndex: i];
	}
	
	Engine *engine = [Slant6 new];
	[car setEngine: engine];
	
	[car print];
	
	return (0);
	
} // main

