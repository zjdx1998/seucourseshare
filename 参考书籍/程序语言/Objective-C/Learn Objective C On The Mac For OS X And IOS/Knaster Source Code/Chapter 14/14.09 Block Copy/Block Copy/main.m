//
//  main.m
//  Block Copy
//
//  Created by Waqar Malik on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void (^MKSampleVoidBlockRef)(void);

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        MKSampleVoidBlockRef block1 = ^{
            NSLog(@"Block1");
        };
        block1();
        
        MKSampleVoidBlockRef block2 = ^{
            NSLog(@"Block2");
        };
        block2();
        Block_release(block2);
        
        block2 = Block_copy(block1);
        block2();
    }
    return 0;
}

