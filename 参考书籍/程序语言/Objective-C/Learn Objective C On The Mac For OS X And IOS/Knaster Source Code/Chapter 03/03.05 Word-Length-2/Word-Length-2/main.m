//
//  main.m
//  Word-Length-2
//
//  Created by Waqar Malik on 4/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[])
{
	const char *words[4] = { "Joe-Bob \"Handyman\" Brown",
		"Jacksonville \"Sly\" Murphy",
		"Shinara Bain", 
		"George \"Guitar\" Books" };
	int wordCount = 4;
	
	for (int i = 0; i < wordCount; i++)
    {
		NSLog (@"%s is %lu characters long", words[i], strlen(words[i]));
	}
    return 0;
}

