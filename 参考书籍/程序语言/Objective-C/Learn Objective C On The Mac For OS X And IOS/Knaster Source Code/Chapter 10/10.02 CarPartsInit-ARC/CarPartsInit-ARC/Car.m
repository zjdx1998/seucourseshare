#import "Car.h"

@implementation Car

- (id) init
{
    if (self = [super init])
    {
        tires = [[NSMutableArray alloc] init];
        
        for (int i = 0; i < 4; i++)
        {
            [tires addObject: [NSNull null]];
        }
    }

    return (self);

} // init


- (Engine *) engine
{
    return (engine);
} // engine


- (void) setEngine: (Engine *) newEngine
{
    engine = newEngine;

} // setEngine


- (void) setTire: (Tire *) tire
         atIndex: (int) index
{
    [tires replaceObjectAtIndex: index
           withObject: tire];

} // setTire:atIndex:


- (Tire *) tireAtIndex: (int) index
{
    Tire *tire;
    tire = [tires objectAtIndex: index];

    return (tire);

} // tireAtIndex:


- (void) print
{
    for (int i = 0; i < 4; i++)
    {
        NSLog (@"%@", [self tireAtIndex: i]);
    }

    NSLog (@"%@", engine);

} // print

@end // Car

