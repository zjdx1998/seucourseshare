
#import <Foundation/Foundation.h>

@class Tire;
@class Engine;

@interface Car : NSObject
{
    NSMutableArray *tires;
    Engine *engine;
}

- (void)setEngine:(Engine *)newEngine;

- (Engine *)engine;

- (void)setTire:(Tire *)tire atIndex:(int)index;

- (Tire *)tireAtIndex:(int)index;

- (void)print;

@end // Car
