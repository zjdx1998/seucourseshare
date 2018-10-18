//
//  main.m
//  Delegation
//
//  Created by Waqar Malik on 3/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Manager.h"
#import "Worker1.h"
#import "Worker2.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        Manager *manager = [[Manager alloc] init];
        Worker1 *worker1 = [[Worker1 alloc] init];
        manager.delegate = worker1;
        [manager doWork];
        
        Worker2 *worker2 = [[Worker2 alloc] init];
        manager.delegate = worker2;
        [manager doWork];
    }
    return 0;
}

