//
//  MSCViewController.m
//  CaseTool
//
//  Created by Waqar Malik on 1/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MSCViewController.h"

@implementation MSCViewController
@synthesize textField;
@synthesize resultsField;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if(nil != self)
    {
        NSLog (@"init: text %@ / results %@", textField, resultsField);
    }
    return self;
}

- (void)awakeFromNib
{
	NSLog(@"awake: text %@ / results %@", textField, resultsField);    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
	[super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
	NSLog (@"viewDidLoad: text %@ / results %@", textField, resultsField);
	
    [textField setPlaceholder:@"Enter text here"];
    resultsField.text = @"Results";
}

- (void)viewDidUnload
{
    [self setTextField:nil];
    [self setResultsField:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
	[super viewWillDisappear:animated];
}

- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}

- (IBAction)upppercase
{
	NSString *original = textField.text;
	NSString *uppercase = [original uppercaseString];
    resultsField.text = uppercase;
}

- (IBAction)lowercase
{
	NSString *original = textField.text;
	NSString *lowercase = [original lowercaseString];
    resultsField.text = lowercase;
}
@end
