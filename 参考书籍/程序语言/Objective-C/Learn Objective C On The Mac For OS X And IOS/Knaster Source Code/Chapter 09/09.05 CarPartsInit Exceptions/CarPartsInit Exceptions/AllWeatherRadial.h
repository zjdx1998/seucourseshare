#import <Foundation/Foundation.h>
#import "Tire.h"

@interface AllWeatherRadial : Tire
{
    float rainHandling;
    float snowHandling;
}

- (void) setRainHandling: (float) rainHanding;
- (float) rainHandling;

- (void) setSnowHandling: (float) snowHandling;
- (float) snowHandling;

@end // AllWeatherRadial
