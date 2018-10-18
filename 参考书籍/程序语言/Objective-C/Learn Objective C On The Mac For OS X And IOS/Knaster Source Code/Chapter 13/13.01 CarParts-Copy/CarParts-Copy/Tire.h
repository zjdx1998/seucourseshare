//
//  Tire.h
//  CarParts-Copy
//
//  Created by Waqar Malik on 3/28/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

@interface Tire : NSObject <NSCopying> 
@property (assign) float pressure;
@property (assign) float treadDepth;

- (id)initWithPressure:(float)pressure;
- (id)initWithTreadDepth:(float)treadDepth;

- (id)initWithPressure:(float)pressure treadDepth:(float)treadDepth;

@end // Tire
