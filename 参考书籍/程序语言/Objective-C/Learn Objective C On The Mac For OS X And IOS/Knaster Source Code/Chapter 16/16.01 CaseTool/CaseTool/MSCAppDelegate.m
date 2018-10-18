//
//  MSCAppDelegate.m
//  CaseTool
//
//  Created by Waqar Malik on 1/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MSCAppDelegate.h"

@implementation MSCAppDelegate

@synthesize window = _window;
@synthesize textField = _textField;
@synthesize resultsField = _resultsField;

- (id)init
{
	if (nil != (self = [super init]))
    {
		NSLog (@"init: text %@ / results %@", _textField, _resultsField);
	}
	
	return self;	
}

- (void)awakeFromNib
{
	NSLog (@"awake: text %@ / results %@", _textField, _resultsField);
	
	[_textField setStringValue:@"Enter text here"];
	[_resultsField setStringValue:@"Results"];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
}

- (IBAction)uppercase:(id)sender
{
	NSString *original = [_textField stringValue];
	NSString *uppercase = [original uppercaseString];
	[_resultsField setStringValue:uppercase];
}

- (IBAction)lowercase:(id)sender
{
	NSString *original = [_textField stringValue];
	NSString *lowercase = [original lowercaseString];
	[_resultsField setStringValue:lowercase];
}
@end
