//
//  Things.m
//  12.04 ClassExtension
//
//  Created by Waqar Malik on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Things.h"

@interface Things ()
{
    NSInteger thing4;
}
@property (readwrite, assign) NSInteger thing2;
@property (assign) NSInteger thing3;
@end

@implementation Things
@synthesize thing1;
@synthesize thing2;
@synthesize thing3;

- (void)resetAllValues
{
    self.thing1 = 200;
    self.thing2 = 300;
    self.thing3 = 400;
    thing4 = 5;
}

- (NSString *) description
{
    NSString *desc = [NSString stringWithFormat: @"%d %d %d",
                      thing1, thing2, thing3];
    
    return (desc);
    
} // description
@end
