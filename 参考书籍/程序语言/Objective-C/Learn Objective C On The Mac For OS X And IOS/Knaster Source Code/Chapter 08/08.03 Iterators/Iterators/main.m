//
//  main.m
//  Iterators
//
//  Created by Waqar Malik on 11/17/11.
//  Copyright (c) 2011 Crimson Research, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

int main (int argc, const char * argv[])
{
    @autoreleasepool
    {
        NSLog(@"Using Index");
        NSArray *array = [NSArray arrayWithObjects:@"one", @"two", @"three", @"four", nil];
        NSUInteger i = 0;
        for(i = 0; i < [array count]; i++)
        {
            NSLog (@"index %lu has %@.", i, [array objectAtIndex:i]);
        }
        
        NSLog(@"Using Object Enumerator");
        i = 0;
        NSEnumerator *enumerator = [array objectEnumerator];
        NSString *value = nil;
        while(value = [enumerator nextObject])
        {
            NSLog (@"index %lu has %@.", i, value);
            i++;
        }
        
        NSLog(@"Using Fast Enumeration");
        i = 0;
        for(NSString *value in array)
        {
            NSLog (@"index %lu has %@.", i, value);
            i++;
        }
        
        NSLog(@"Using Blocks");
        [array enumerateObjectsUsingBlock:^(NSString *value, NSUInteger index, BOOL *stop) {
            NSLog (@"index %lu has %@.", index, value);
        }];
    }
    return 0;
}

