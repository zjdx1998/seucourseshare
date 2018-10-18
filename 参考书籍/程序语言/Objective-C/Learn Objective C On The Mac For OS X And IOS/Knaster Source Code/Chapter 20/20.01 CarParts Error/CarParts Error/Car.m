#import "Car.h"

#import "Engine.h"

@implementation Car

@synthesize name;
@synthesize engine;
@synthesize make;
@synthesize model;
@synthesize modelYear;
@synthesize numberOfDoors;
@synthesize mileage;

- (id) init
{
    if (self = [super init]) {

		self.name = @"Car";
        tires = [[NSMutableArray alloc] init];

        for (int i = 0; i < 4; i++) {
            [tires addObject: [NSNull null]];
        }
    }

    return (self);

} // init


- (id) copyWithZone: (NSZone *) zone
{
	Car *carCopy;
	carCopy = [[[self class] 
				allocWithZone: zone]
			   init];
	
	carCopy.name = name;
	carCopy.make = make;
	carCopy.model = model;
	carCopy.numberOfDoors = numberOfDoors;
	carCopy.mileage = mileage;
	
	Engine *engineCopy;
	engineCopy = [[engine copy] autorelease];
	carCopy.engine = engineCopy;
	
	for (int i = 0; i < 4; i++)
    {
		Tire *tireCopy;
		
		tireCopy = [[self tireAtIndex: i] copy];
		[tireCopy autorelease];
		
		[carCopy setTire: tireCopy
				 atIndex: i];
	}
	
	return (carCopy);
	
} // copyWithZone


- (void) dealloc
{
	self.name = nil;
    [tires release];
    [engine release];

    [super dealloc];

} // dealloc


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
	NSString *desc = [self description];
	NSLog (@"%@", desc);
	
} // print


- (NSString *) description {
	NSString *desc;
	desc = [NSString stringWithFormat: 
			@"%@, a %d %@ %@, has %d doors, %.1f miles, %d hp and %d tires",
			name, modelYear, make, model, numberOfDoors, mileage, [engine horsepower],
			[tires count]];

	return desc;
	
} // description


- (void) setNilValueForKey: (NSString *) key {
	if ([key isEqualToString: @"mileage"]) {
		mileage = 0;
	} else {
		[super setNilValueForKey: key];
	}
} // setNilValueForKey

- (void)processMyObject:(id)object error:(NSError **)error
{
}
@end // Car

