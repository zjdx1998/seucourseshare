//
//  Car.h
//  CarParts-Copy
//
//  Created by Waqar Malik on 3/28/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

@class Tire;
@class Engine;

@interface Car : NSObject <NSCopying>
@property (readwrite, copy) NSString *name;
@property (readwrite, strong) Engine *engine;

- (void)setTire:(Tire *)tire atIndex:(NSInteger)index;
- (Tire *)tireAtIndex:(NSInteger)index;

- (void)print;

@end // Car
