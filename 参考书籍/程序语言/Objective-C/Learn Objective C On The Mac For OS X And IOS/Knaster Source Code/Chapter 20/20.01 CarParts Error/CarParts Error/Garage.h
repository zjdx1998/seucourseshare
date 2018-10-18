//
//  Garage.h
//  15.01 Car-Value-Coding
//
//  Created by markd on 8/16/08.
//  Copyright 2008 Length-O-Words.com. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class Car;

@interface Garage : NSObject {
	NSString *name;
	NSMutableArray *cars;
	NSMutableDictionary *stuff;
}
@property (readonly, retain) NSMutableArray *cars;

@property (readwrite, copy) NSString *name;

- (void) addCar: (Car *) car;

- (void) print;

@end // Garage
