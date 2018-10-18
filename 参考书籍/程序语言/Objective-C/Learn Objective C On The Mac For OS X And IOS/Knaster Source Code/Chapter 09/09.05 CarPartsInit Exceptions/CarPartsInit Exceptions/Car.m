#import "Car.h"

@implementation Car

- (id) init
{
    if (self = [super init])
    {
        tires = [[NSMutableArray alloc] init];

        for(int i = 0; i < 4; i++)
        {
            [tires addObject:[NSNull null]];
        }
    }

    return (self);

} // init

- (void)dealloc
{
    [tires release];
    [engine release];

    [super dealloc];

} // dealloc


- (Engine *)engine
{
    return (engine);
} // engine


- (void)setEngine:(Engine *)newEngine
{
    [newEngine retain];
    [engine release];

    engine = newEngine;

} // setEngine


- (void)setTire:(Tire *)tire atIndex:(int)index
{
    @try {
        [tires replaceObjectAtIndex:index withObject:tire];
    } @catch (NSException *exception) {
        NSLog(@"%@", [exception reason]);
    }

} // setTire:atIndex:


- (Tire *)tireAtIndex:(int)index
{
    Tire *tire = nil;
    @try {
        tire = [tires objectAtIndex:index];
    } @catch (NSException *exception) {
        NSLog(@"%@", [exception reason]);
    }

    return (tire);

} // tireAtIndex:


- (void)print
{
    for(Tire *tire in tires)
    {
        NSLog (@"%@", tire);
    }

    NSLog(@"%@", engine);

} // print

@end // Car

