//
//  main.m
//  Selectors
//
//  Created by Waqar Malik on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SelectorTester.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        SelectorTester *tester = [[SelectorTester alloc] init];
        
        [tester runSelectors];
    }
    return 0;
}

