//
//  ITunesFinder.m
//  iTunesFinder
//
//  Created by Waqar Malik on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ITunesFinder.h"

@implementation ITunesFinder

- (void) netServiceBrowser:(NSNetServiceBrowser *) b
            didFindService:(NSNetService *) service
                moreComing:(BOOL) moreComing
{
    [service resolveWithTimeout:10];

    NSLog (@"found one! Name is %@", [service name]);

} // didFindService


- (void) netServiceBrowser:(NSNetServiceBrowser *) b
          didRemoveService:(NSNetService *) service
                moreComing:(BOOL) moreComing
{
    [service resolveWithTimeout:10];

    NSLog (@"lost one! Name is %@", [service name]);

} // didRemoveService

@end // ITunesFinder
