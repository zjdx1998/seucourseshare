//
//  MKViewController.m
//  Objective Queue
//
//  Created by Waqar Malik on 4/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MKViewController.h"

@interface MKViewController ()

@end

@implementation MKViewController
{
    __strong NSOperationQueue *_operationQueue;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

- (NSOperation *)operationWithData:(id)data
{
    return [[NSInvocationOperation alloc] initWithTarget:self selector:@selector(myWorkerMethod:) object:data];
}

// This is the method that does the actual work
- (void)myWorkerMethod:(id)data
{
    NSLog(@"My Worker Method %@", data);
}

- (IBAction)invokeOperation:(id)sender 
{
    NSBlockOperation *blockOperation = [NSBlockOperation blockOperationWithBlock:^{
        NSLog(@"My Block Operation");
    }];
    
    _operationQueue = [[NSOperationQueue alloc] init];
    _operationQueue.maxConcurrentOperationCount = 5;
    [_operationQueue addOperation:blockOperation];
    [_operationQueue addOperation:[self operationWithData:@"My Sample Data"]];
    [_operationQueue addOperationWithBlock:^{
        NSLog(@"Inline Operation with Block");
    }];
    [blockOperation addExecutionBlock:^{
        NSLog(@"Adding execution block to existing block");
    }];
}
@end
