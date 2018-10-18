//
//  main.m
//  PropertyListing
//
//  Created by Waqar Malik on 4/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

static void dateFun (void) {
    NSDate *date = [NSDate date];
    NSLog (@"today is %@", date);
    
    NSDate *yesterday = [NSDate dateWithTimeIntervalSinceNow: -(24 * 60 * 60)];
    NSLog (@"yesterday is %@", yesterday);
} // dateFun


static void dataFun (void) {
    const char *string = "Hi there, this is a C string!";
    NSData *data = [NSData dataWithBytes: string
                                  length: strlen(string) + 1];
    NSLog (@"data is %@", data);
    NSLog (@"%ld byte string is '%s'", [data length], [data bytes]);
} // dataFun


static void atomicCafe (void) {
    NSArray *phrase;
    phrase = [NSArray arrayWithObjects: @"I", @"seem", @"to", @"be", @"a", @"verb", nil];
    [phrase writeToFile: @"/tmp/verbiage.txt"  atomically: YES];
    
    NSArray *phrase2 = [NSArray arrayWithContentsOfFile: @"/tmp/verbiage.txt"];
    NSLog (@"%@", phrase2);
    
} // atomicCafe

void writeCapitols (void)
{
    NSMutableArray *capitols = [NSMutableArray arrayWithCapacity:10];
    NSMutableDictionary *capitol = [NSMutableDictionary dictionaryWithObject:@"Canada" forKey:@"country"];
    [capitol setObject:@"Ottawa" forKey:@"capitol"];
    [capitols addObject:capitol];
    capitol = [NSMutableDictionary dictionaryWithObject:@"Norway" forKey:@"country"];
    [capitol setObject:@"Oslo" forKey:@"capitol"];
    [capitols addObject:capitol];
    
    NSString *error = nil;
    NSData *encodedArray = [NSPropertyListSerialization dataFromPropertyList:capitols 
                                                                      format:NSPropertyListBinaryFormat_v1_0 
                                                            errorDescription:&error];
    [encodedArray writeToFile:@"/tmp/capitols.txt" atomically:YES];
}

static void modifyCapitols(void)
{
    NSData *data = [NSData dataWithContentsOfFile:@"/tmp/capitols.txt"];
    NSPropertyListFormat propertyListFormat = NSPropertyListXMLFormat_v1_0;
    NSString *error = nil;
    NSMutableArray *capitols = [NSPropertyListSerialization propertyListFromData:data
                                                                mutabilityOption:NSPropertyListMutableContainersAndLeaves
                                                                          format:&propertyListFormat
                                                                errorDescription:&error];
    NSLog(@"capitols %@", capitols);
}

int main (int argc, const char * argv[])
{
    @autoreleasepool
    {
        dateFun ();
        dataFun ();
        atomicCafe ();
        writeCapitols();
        modifyCapitols();
    }
    
    return (0);
    
} // main
