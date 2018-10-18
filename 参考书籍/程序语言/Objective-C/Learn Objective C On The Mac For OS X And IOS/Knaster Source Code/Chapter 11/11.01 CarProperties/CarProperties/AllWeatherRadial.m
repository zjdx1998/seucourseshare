//
//  AllWeatherRadial.m
//  CarProperties
//
//  Created by Waqar Malik on 3/30/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "AllWeatherRadial.h"

@implementation AllWeatherRadial
@synthesize rainHandling;
@synthesize snowHandling;

- (id) initWithPressure:(float)p
             treadDepth:(float)td
{
    if (self = [super initWithPressure: p
                      treadDepth: td]) {
        rainHandling = 24.7;
        snowHandling = 42.5;
    }

    return (self);

} // initWithPressure:treadDepth

- (NSString *) description
{
    NSString *desc;
    desc = [[NSString alloc] initWithFormat:
            @"AllWeatherRadial: %.1f / %.1f / %.1f / %.1f",
            self.pressure, self.treadDepth, self.rainHandling, self.snowHandling];
    
    return (desc);
    
} // description

@end // AllWeatherRadial

