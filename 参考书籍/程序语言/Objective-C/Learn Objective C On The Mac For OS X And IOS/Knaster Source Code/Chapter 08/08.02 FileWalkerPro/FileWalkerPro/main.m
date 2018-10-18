//
//  main.m
//  FileWalkerPro
//
//  Created by Waqar Malik on 4/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        NSFileManager *manager;
        manager = [NSFileManager defaultManager];
        
        NSString *home;
        home = [@"~" stringByExpandingTildeInPath];
        
        NSMutableArray *files;
        files = [NSMutableArray arrayWithCapacity: 42];
        
        for(NSString *filename in [manager enumeratorAtPath:home])
        {
            if([[filename pathExtension] isEqualTo:@"jpg"])
            {
                [files addObject:filename];
            }		
        }
        
        for(NSString *filename in files)
        {
            NSLog (@"%@", filename);
        }
    }
    return 0;
}

