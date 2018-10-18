//
//  Manager.h
//  Delegation
//
//  Created by Waqar Malik on 3/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "WorkerProtocol.h"

@interface Manager : NSObject
@property (weak) id <WorkerProtocol> delegate;

- (void)doWork;
@end
