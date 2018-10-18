//
//  Tire.h
//  CarProperties
//
//  Created by Waqar Malik on 3/30/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

@interface Tire : NSObject

@property float pressure;
@property float treadDepth;

- (id) initWithPressure: (float) pressure;
- (id) initWithTreadDepth: (float) treadDepth;

- (id) initWithPressure: (float) pressure
       treadDepth: (float) treadDepth;
@end // Tire
