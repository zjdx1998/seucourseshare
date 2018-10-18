//
//  Car.m
//  CarParts
//
//  Created by Waqar Malik on 3/30/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

@class Tire;
@class Engine;

@interface Car : NSObject
{
    Tire *tires[4];
    Engine *engine;
}

- (void) setEngine: (Engine *) newEngine;

- (Engine *) engine;


- (void) setTire: (Tire *) tire
         atIndex: (int) index;

- (Tire *) tireAtIndex: (int) index;

- (void) print;

@end // Car
