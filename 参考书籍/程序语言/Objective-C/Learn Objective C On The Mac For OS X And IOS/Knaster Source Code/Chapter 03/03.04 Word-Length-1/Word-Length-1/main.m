//
//  main.m
//  Word-Length-1
//
//  Created by Waqar Malik on 4/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[])
{
	const char *words[4] = { "aardvark", "abacus", 
        "allude", "zygote" };
	int wordCount = 4;
	
	for (int i = 0; i < wordCount; i++)
    {
		NSLog (@"%s is %lu characters long", words[i], strlen(words[i]));
	}
    return 0;
}

