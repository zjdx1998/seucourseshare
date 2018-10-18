//
//  Car.h
//  CarProperties
//
//  Created by Waqar Malik on 3/30/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

@class Tire;
@class Engine;

@interface Car : NSObject

@property (copy) NSString *name;
@property (strong) Engine *engine;

- (void) setTire: (Tire *) tire
         atIndex: (int) index;

- (Tire *) tireAtIndex: (int) index;

- (void) print;

@end // Car
