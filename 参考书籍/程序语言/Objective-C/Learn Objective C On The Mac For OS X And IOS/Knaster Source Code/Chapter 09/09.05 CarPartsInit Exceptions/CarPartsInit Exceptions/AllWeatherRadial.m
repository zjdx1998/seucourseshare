#import "AllWeatherRadial.h"

@implementation AllWeatherRadial

- (id)initWithPressure:(float)p treadDepth:(float)td
{
    if(self = [super initWithPressure:p treadDepth:td])
    {
        rainHandling = 23.7;
        snowHandling = 42.5;
    }

    return (self);

} // initWithPressure:treadDepth


- (void) setRainHandling: (float) rh
{
    rainHandling = rh;
} // setRainHandling


- (float) rainHandling
{
    return (rainHandling);
} // rainHandling


- (void) setSnowHandling: (float) sh
{
    snowHandling = sh;
} // setSnowHandling


- (float) snowHandling
{
    return (snowHandling);
} // snowHandling


- (NSString *) description
{
    NSString *desc;
    desc = [[NSString alloc] initWithFormat:
                   @"AllWeatherRadial: %.1f / %.1f / %.1f / %.1f",
               [self pressure], [self treadDepth],
               [self rainHandling], 
               [self snowHandling]];

    return (desc);

} // description

@end // AllWeatherRadial

