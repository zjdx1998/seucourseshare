#import "Tire.h"


@implementation Tire

- (id) init
{
    if (self = [self initWithPressure: 34
                     treadDepth: 20]) {
    }

    return (self);

} // init


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



- (void) setPressure: (float) p
{
    pressure = p;
} // setPressure


- (float) pressure
{
    return (pressure);
} // pressure


- (void) setTreadDepth: (float) td
{
    treadDepth = td;
} // setTreadDepth


- (float) treadDepth
{
    return (treadDepth);
} // treadDepth


- (NSString *) description
{
    NSString *desc;
    desc = [NSString stringWithFormat:
                         @"Tire: Pressure: %.1f TreadDepth: %.1f",
                     pressure, treadDepth];
    return (desc);

} // description

@end // Tire

