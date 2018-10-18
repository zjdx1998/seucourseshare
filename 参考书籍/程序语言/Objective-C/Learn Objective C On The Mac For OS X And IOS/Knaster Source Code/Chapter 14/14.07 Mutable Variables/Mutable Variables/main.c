//
//  main.c
//  Mutable Variables
//
//  Created by Waqar Malik on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

typedef void (^MKSampleMultiplyBlockRef) (double a, double b);

int main(int argc, const char * argv[])
{
    __block double c = 3;
    MKSampleMultiplyBlockRef multiply = ^(double a, double b) { c = a * b; };
    
    multiply(4, 5);
    printf("c = %f\n", c);
    
    void (^MKSampleBlock)(void) = ^(void) {
        double a = 4;
        double c = 2;
        printf("a * c = %f\n", a * c);
    };
    
    MKSampleBlock();
    return 0;
}

