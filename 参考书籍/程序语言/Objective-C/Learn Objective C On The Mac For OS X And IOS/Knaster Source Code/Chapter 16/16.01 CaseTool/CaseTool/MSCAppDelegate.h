//
//  MSCAppDelegate.h
//  CaseTool
//
//  Created by Waqar Malik on 1/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface MSCAppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (weak) IBOutlet NSTextField *textField;
@property (weak) IBOutlet NSTextField *resultsField;

- (IBAction)uppercase:(id)sender;
- (IBAction)lowercase:(id)sender;
@end
