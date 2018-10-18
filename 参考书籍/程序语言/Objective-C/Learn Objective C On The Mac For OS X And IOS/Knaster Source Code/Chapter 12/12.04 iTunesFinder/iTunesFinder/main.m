//
//  main.m
//  iTunesFinder
//
//  Created by Waqar Malik on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ITunesFinder.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        NSNetServiceBrowser *browser = [[NSNetServiceBrowser alloc] init];
        
        ITunesFinder *finder = [[ITunesFinder alloc] init];
        
        [browser setDelegate:finder];
        [browser searchForServicesOfType:@"_daap._tcp"
                                inDomain:@"local."];
        
        NSLog (@"begun browsing");
        
        [[NSRunLoop currentRunLoop] run];
    }
    return 0;
}

