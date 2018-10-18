//
//  WorkerProtocol.h
//  Delegation
//
//  Created by Waqar Malik on 3/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol WorkerProtocol <NSObject>
@optional
- (void)doSomeOptionalWork;

@required
- (void)doSomeRequiredWork;
@end
