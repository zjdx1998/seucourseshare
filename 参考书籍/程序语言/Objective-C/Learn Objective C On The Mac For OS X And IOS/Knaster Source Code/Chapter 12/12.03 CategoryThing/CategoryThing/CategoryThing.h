//
//  CategoryThing.m
//  CategoryThing
//
//  Created by Waqar Malik on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CategoryThing : NSObject
{
  NSInteger thing1;
  NSInteger thing2;
  NSInteger thing3;
}
@end // CategoryThing

@interface CategoryThing (Thing1)
- (void)setThing1:(NSInteger)thing1;
- (NSInteger)thing1;
@end // CategoryThing (Thing1)

@interface CategoryThing(Thing2)
- (void)setThing2:(NSInteger)thing2;
- (NSInteger)thing2;
@end // CategoryThing (Thing2)

@interface CategoryThing (Thing3)
- (void)setThing3:(NSInteger)thing3;
- (NSInteger)thing3;
@end // CategoryThing (Thing3)

