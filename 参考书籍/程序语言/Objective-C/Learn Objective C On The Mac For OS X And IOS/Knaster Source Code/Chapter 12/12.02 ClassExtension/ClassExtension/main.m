//
//  main.m
//  ClassExtension
//
//  Created by Waqar Malik on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Things.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        Things *things = [[Things alloc] init];
        things.thing1 = 1;
        NSLog(@"%@", things);
        [things resetAllValues];
        NSLog(@"%@", things);
    }
    return 0;
}

