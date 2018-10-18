//
//  main.m
//  SimpleEncoding
//
//  Created by Waqar Malik on 4/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Thingie : NSObject <NSCoding>
{
    NSString *name;
    int magicNumber;
    float shoeSize;
    NSMutableArray *subThingies;
}

@property (copy) NSString *name;
@property int magicNumber;
@property float shoeSize;
@property (retain) NSMutableArray *subThingies;

- (id)initWithName: (NSString *) n
       magicNumber: (int) mn  
          shoeSize: (float) ss;

@end // Thingie

@implementation Thingie
@synthesize name;
@synthesize magicNumber;
@synthesize shoeSize;
@synthesize subThingies;

- (id)initWithName: (NSString *) n
       magicNumber: (int) mn  
          shoeSize: (float) ss
{
    if (self = [super init])
    {
        self.name = n;
        self.magicNumber = mn;
        self.shoeSize = ss;
        self.subThingies = [NSMutableArray array];
    }
    
    return (self);
}

- (void) dealloc
{
    [name release];
    [subThingies release];
    
    [super dealloc];
} // dealloc


- (void) encodeWithCoder: (NSCoder *) coder
{
    [coder encodeObject: name
                 forKey: @"name"];
    [coder encodeInt: magicNumber
              forKey: @"magicNumber"];
    [coder encodeFloat: shoeSize
                forKey: @"shoeSize"];
    [coder encodeObject: subThingies
                 forKey: @"subThingies"];
    
} // encodeWithCoder


- (id) initWithCoder: (NSCoder *) decoder
{
    if (self = [super init])
    {
        self.name = [decoder decodeObjectForKey: @"name"];
        self.magicNumber = [decoder decodeIntForKey: @"magicNumber"];
        self.shoeSize = [decoder decodeFloatForKey: @"shoeSize"];
        self.subThingies = [decoder decodeObjectForKey: @"subThingies"];
    }
    
    return (self);
    
} // initWithCoder


- (NSString *) description
{
    NSString *description = 
    [NSString stringWithFormat: @"%@: %d/%.1f %@",
     name, magicNumber, shoeSize, subThingies];
    
    return (description);
    
} // description


@end // Thingie


int main (int argc, const char * argv[])
{
    @autoreleasepool
    {
        Thingie *thing1;
        thing1 = [[Thingie alloc]
                  initWithName: @"thing1"
                  magicNumber: 42
                  shoeSize: 10.5];
        
        NSLog (@"some thing: %@", thing1);
        
        NSData *freezeDried;
        freezeDried = [NSKeyedArchiver archivedDataWithRootObject: thing1];
        
        [thing1 release];
        
        thing1 = [NSKeyedUnarchiver unarchiveObjectWithData: freezeDried];
        
        NSLog (@"reconstituted thing: %@", thing1);
        
        Thingie *anotherThing;
        
        anotherThing =  [[[Thingie alloc]
                          initWithName: @"thing2"
                          magicNumber: 23
                          shoeSize: 13.0] autorelease];
        [thing1.subThingies addObject: anotherThing];
        anotherThing =  [[[Thingie alloc]
                          initWithName: @"thing3"
                          magicNumber: 17
                          shoeSize: 9.0] autorelease];
        [thing1.subThingies addObject: anotherThing];
        
        NSLog (@"thing with things: %@", thing1);
        
        freezeDried = [NSKeyedArchiver archivedDataWithRootObject: thing1];
        
        thing1 = [NSKeyedUnarchiver unarchiveObjectWithData: freezeDried];
        NSLog (@"reconstituted multithing: %@", thing1);
        
        [thing1.subThingies addObject: thing1];
        
        // You really don't want to do this...
        // NSLog (@"infinite thinging: %@", thing1);
        
        freezeDried = [NSKeyedArchiver archivedDataWithRootObject: thing1];
        
        thing1 = [NSKeyedUnarchiver unarchiveObjectWithData: freezeDried];
        
    }
    return (0);
    
} // main