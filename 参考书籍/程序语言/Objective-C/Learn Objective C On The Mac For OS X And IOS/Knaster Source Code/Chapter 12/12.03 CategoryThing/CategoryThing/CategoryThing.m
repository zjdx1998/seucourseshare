//
//  CategoryThing.m
//  CategoryThing
//
//  Created by Waqar Malik on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "CategoryThing.h"

@implementation CategoryThing

- (NSString *)description
{
    NSString *desc = [NSString stringWithFormat: @"%d %d %d",
                      thing1, thing2, thing3];
    return (desc);

} // description

@end // CategoryThing
