//
//  main.m
//  CarPartsInit Exceptions
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
    @autoreleasepool 
    {
        Car *car = [[Car alloc] init];
        
        for (int i = 0; i < 6; i++)
        {
            AllWeatherRadial *tire;
            
            tire = [[AllWeatherRadial alloc] init];
            [car setTire:tire atIndex:i];
            
            [tire release];
        }
        
        Engine *engine = [[Slant6 alloc] init];
        [car setEngine:engine];
        
        [car print];
        [car release];
    }
    return 0;
}

