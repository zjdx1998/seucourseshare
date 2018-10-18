#import <Cocoa/Cocoa.h>

@class Tire;
@class Engine;

@interface Car : NSObject <NSCopying> {
	NSString *name;
    NSMutableArray *tires;
    Engine *engine;
	
	NSString *make;
	NSString *model;
	int modelYear;
	int numberOfDoors;
	float mileage;
}

@property (readwrite, copy) NSString *name;
@property (readwrite, retain) Engine *engine;
@property (readwrite, copy) NSString *make;
@property (readwrite, copy) NSString *model;
@property (readwrite) int modelYear;
@property (readwrite) int numberOfDoors;
@property (readwrite) float mileage;

- (void) setTire: (Tire *) tire
         atIndex: (int) index;
- (Tire *) tireAtIndex: (int) index;

- (void) print;

@end // Car
