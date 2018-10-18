#import "Engine.h"

@implementation Engine

- (id) init {
	if (self = [super init]) {
		horsepower = 145;
	}
	
	return (self);
	
} // init


- (id) copyWithZone: (NSZone *) zone
{
	Engine *engineCopy;
	engineCopy = [[[self class]
				   allocWithZone: zone]
				  init];
	[engineCopy setValue: [NSNumber numberWithInt: horsepower]
		        forKey: @"horsepower"];
	
	return (engineCopy);
	
} // copyWithZone


- (NSString *) description
{
	NSString *description;
	description = [NSString stringWithFormat:
				   @"I am an engine.  Vrooom!  %d HP", horsepower];
	return (description);
	
} // description

@end // Engine

