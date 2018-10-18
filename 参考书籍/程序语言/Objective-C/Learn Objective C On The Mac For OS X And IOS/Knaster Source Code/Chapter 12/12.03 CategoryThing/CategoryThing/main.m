//
//  main.m
//  CategoryThing
//
//  Created by Waqar Malik on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CategoryThing.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        CategoryThing *thing = [[CategoryThing alloc] init];
        
        [thing setThing1: 5];
        [thing setThing2: 23];
        [thing setThing3: 42];
        
        NSLog (@"Things are %@", thing);
    }
    return 0;
}

