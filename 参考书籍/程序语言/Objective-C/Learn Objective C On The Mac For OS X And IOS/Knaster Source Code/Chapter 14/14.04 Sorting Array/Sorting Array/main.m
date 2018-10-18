//
//  main.m
//  Sorting Array
//
//  Created by Waqar Malik on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        NSArray *array = [NSArray arrayWithObjects:@"Amir", @"Mishal", @"Irrum", @"Adam", nil];
        NSLog(@"Unsorted Array %@", array);
        NSArray *sortedArray = [array sortedArrayUsingComparator:^(NSString *object1, NSString *object2) {
            return [object1 compare:object2];
        }];
        NSLog(@"Sorted Array %@", sortedArray);
    }
    return 0;
}

