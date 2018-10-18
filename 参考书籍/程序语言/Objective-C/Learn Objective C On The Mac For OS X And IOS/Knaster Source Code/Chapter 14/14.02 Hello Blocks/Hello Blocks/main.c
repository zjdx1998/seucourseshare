//
//  main.c
//  Hello Blocks
//
//  Created by Waqar Malik on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    void (^theBlock)() = ^{ printf("Hello Blocks!\n"); };
    theBlock();
    return 0;
}

