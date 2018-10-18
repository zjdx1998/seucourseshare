//
//  main.c
//  Local Variables
//
//  Created by Waqar Malik on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    int value = 6;
    int (^multiply_block)(int number) =  ^(int number) {return (value * number);};
    int result = multiply_block(7);
    printf("Result = %d\n", result);
    return 0;
}

