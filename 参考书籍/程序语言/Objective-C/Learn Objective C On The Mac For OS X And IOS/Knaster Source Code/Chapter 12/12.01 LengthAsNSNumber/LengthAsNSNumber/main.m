//
//  main.m
//  LengthAsNSNumber
//
//  Created by Waqar Malik on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NSString+NumberConvenience.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        NSMutableDictionary *dict = [NSMutableDictionary dictionary];
        
        [dict setObject:[@"hello" lengthAsNumber]
                 forKey:@"hello"];
        
        [dict setObject:[@"iLikeFish" lengthAsNumber]
                 forKey:@"iLikeFish"];
        
        [dict setObject:[@"Once upon a time" lengthAsNumber]
                 forKey:@"Once upon a time"];
        
        NSLog (@"%@", dict);
    }
    return 0;
}
