//
//  Things.h
//  12.04 ClassExtension
//
//  Created by Waqar Malik on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Things : NSObject
@property (assign) NSInteger thing1;
@property (readonly, assign) NSInteger thing2;

- (void)resetAllValues;
@end
