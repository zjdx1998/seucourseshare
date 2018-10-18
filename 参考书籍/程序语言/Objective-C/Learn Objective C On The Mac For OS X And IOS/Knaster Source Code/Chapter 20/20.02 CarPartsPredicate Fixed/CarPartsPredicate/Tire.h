#import <Cocoa/Cocoa.h>

@interface Tire : NSObject <NSCopying> {  
    float pressure;
    float treadDepth;
}
@property float pressure;
@property float treadDepth;

- (id) initWithPressure: (float) pressure;
- (id) initWithTreadDepth: (float) treadDepth;

- (id) initWithPressure: (float) pressure
       treadDepth: (float) treadDepth;

@end // Tire
