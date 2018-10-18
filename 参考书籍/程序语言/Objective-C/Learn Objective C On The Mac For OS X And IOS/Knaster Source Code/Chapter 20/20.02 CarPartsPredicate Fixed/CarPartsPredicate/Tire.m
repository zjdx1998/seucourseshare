#import "Tire.h"


@implementation Tire

@synthesize pressure;
@synthesize treadDepth;

- (id) init
{
    if (self = [self initWithPressure: 34
                     treadDepth: 20]) {
    }

    return (self);

} // init


- (id) copyWithZone: (NSZone *) zone
{
	Tire *tireCopy;
	tireCopy = [[[self class] allocWithZone: zone]
				initWithPressure: pressure
				treadDepth: treadDepth];
	
	return (tireCopy);
	
} // copyWithZone


- (id) initWithPressure: (float) p
{
    if (self = [self initWithPressure: p
                     treadDepth: 20.0]) {
    }

    return (self);

} // initWithPressure


- (id) initWithTreadDepth: (float) td
{
    if (self = [self initWithPressure: 34.0
                     treadDepth: td]) {
    }

    return (self);

} // initWithTreadDepth


- (id) initWithPressure: (float) p
             treadDepth: (float) td
{
    if (self = [super init]) {
        pressure = p;
        treadDepth = td;
    }

    return (self);

} // initWithPressure:treadDepth:


- (NSString *) description
{
    NSString *desc;
    desc = [NSString stringWithFormat:
                         @"Tire: Pressure: %.1f TreadDepth: %.1f",
                     pressure, treadDepth];
    return (desc);

} // description
@end // Tire

