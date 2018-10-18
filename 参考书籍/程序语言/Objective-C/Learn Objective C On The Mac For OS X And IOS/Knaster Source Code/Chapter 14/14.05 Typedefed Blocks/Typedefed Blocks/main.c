//
//  main.c
//  Typedefed Blocks
//
//  Created by Waqar Malik on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

typedef double (^MKSampleMultiply2BlockRef)(double c, double d);

int main(int argc, const char * argv[])
{
    MKSampleMultiply2BlockRef multiply2 = ^(double c, double d) { return c * d; };
    printf("%f, %f", multiply2(4, 5), multiply2(5, 2));
    return 0;
}

