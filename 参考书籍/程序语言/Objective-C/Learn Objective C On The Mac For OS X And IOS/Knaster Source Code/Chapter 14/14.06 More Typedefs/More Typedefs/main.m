//
//  main.m
//  More Typedefs
//
//  Created by Waqar Malik on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void (^MKSampleVoidBlockRef)(void);
typedef void (^MKSampleStringBlockRef)(NSString *);
typedef double (^MKSampleMultiplyBlockRef)(void);
typedef double (^MKSampleMultiply2BlockRef)(double c, double d);

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        NSString *hello = @"Block 0";
        
        MKSampleVoidBlockRef myBlock = ^{ NSLog(@"Hello %@!", hello); };
        MKSampleVoidBlockRef myBlock1 = ^(void){ NSLog(@"Hello Block 1!"); };
        
        myBlock();
        myBlock1();
        MKSampleStringBlockRef myStringBlock = ^(NSString *string){ NSLog(@"Hello %@!", string); };
        
        myStringBlock(@"World");
        
        double a = 10, b = 20;
        MKSampleMultiplyBlockRef multiply = ^(void){ return a * b; };
        NSLog(@"%f", multiply());
        a = 20;
        b = 50;
        NSLog(@"%f", multiply());
        
        MKSampleMultiply2BlockRef multiply2 = ^(double c, double d) { return c * d; };
        NSLog(@"%f, %f", multiply2(4, 5), multiply2(5, 2));
    }
    return 0;
}

