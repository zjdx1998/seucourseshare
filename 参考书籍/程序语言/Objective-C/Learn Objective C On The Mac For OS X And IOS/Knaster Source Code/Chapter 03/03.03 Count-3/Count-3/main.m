//
//  main.m
//  Count-3
//
//  Created by Waqar Malik on 4/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[])
{
	int count = 100;
	
	NSLog (@"The numbers from 1 to %d:", count);
	
	for (int i = 1; i <= count; i++)
    {
		NSLog (@"%d\n", i);
	}
    
    return 0;
}

