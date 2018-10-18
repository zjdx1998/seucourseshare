//
//  main.c
//  Square
//
//  Created by Waqar Malik on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    int (^square_block)(int number) =  ^(int number) {return (number * number);};
    int result = square_block(5);
    printf("Result = %d\n", result);
    return 0;
}

