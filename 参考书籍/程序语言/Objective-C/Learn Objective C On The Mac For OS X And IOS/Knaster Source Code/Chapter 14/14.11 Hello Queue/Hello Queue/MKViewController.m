//
//  MKViewController.m
//  Hello Queue
//
//  Created by Waqar Malik on 4/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MKViewController.h"

@interface MKViewController ()

@end

void myDispatchFuction(void *argument)
{
    NSLog(@"Serial Task %@", (__bridge NSNumber *)argument);
    NSMutableDictionary *context = (__bridge NSMutableDictionary *)dispatch_get_context(dispatch_get_current_queue());
    NSNumber *value = [context objectForKey:@"value"];
    NSLog(@"value = %@", value);
}

void myFinalizerFunction(void *context)
{
    NSLog(@"myFinalizerFunction");
    NSMutableDictionary *theData = (__bridge_transfer NSMutableDictionary*)context;
    [theData removeAllObjects];
}

@implementation MKViewController
{
    dispatch_queue_t _serial_queue;
    dispatch_queue_t _concurent_queue;
    dispatch_queue_t _main_queue;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
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

- (IBAction)run:(id)sender
{
    if(NULL != _serial_queue)
    {
        dispatch_release(_serial_queue);
    }
    _serial_queue = dispatch_queue_create("com.apress.MySerialQueue1", NULL);
    dispatch_suspend(_serial_queue);
    NSMutableDictionary *myContext = [[NSMutableDictionary alloc] initWithCapacity:5];
    [myContext setObject:@"My Context" forKey:@"title"];
    [myContext setObject:[NSNumber numberWithInt:0] forKey:@"value"];
    dispatch_set_context(_serial_queue, (__bridge_retained void *)myContext);
    dispatch_set_finalizer_f(_serial_queue, &myFinalizerFunction);
    
    _concurent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);

    dispatch_async(_serial_queue,  ^{ NSLog(@"Serial Task 1"); });
    
    dispatch_async(_serial_queue,  ^{
        NSLog(@"Serial Task 2");
        NSMutableDictionary *context = (__bridge NSMutableDictionary *)dispatch_get_context(dispatch_get_current_queue());
        NSNumber *value = [context objectForKey:@"value"];
        NSLog(@"value = %@", value);
        NSInteger number = [value intValue];
        number++;
        NSLog(@"value = %@", value);
        [myContext setValue:[NSNumber numberWithInt:number] forKey:@"value"];
    });
    
    dispatch_async_f(_serial_queue, (__bridge void *)[NSNumber numberWithInt:3], (dispatch_function_t)myDispatchFuction);
    dispatch_async(_serial_queue,  ^{
        NSLog(@"Serial Task 4");
        NSMutableDictionary *myContext = (__bridge NSMutableDictionary *)dispatch_get_context(dispatch_get_current_queue());
        NSNumber *value = [myContext objectForKey:@"value"];
        NSLog(@"value = %@", value);
    });
    
    dispatch_block_t myBlock = ^{ NSLog(@"My Predfined block"); };
    dispatch_async(_serial_queue, myBlock);
    dispatch_resume(_serial_queue);
    
    dispatch_async(_concurent_queue, ^{ NSLog(@"concurrent task 1");});
    dispatch_async(_concurent_queue, ^{ NSLog(@"concurrent task 2");});
    dispatch_async(_concurent_queue, ^{ NSLog(@"concurrent task 3");});
    dispatch_async(_concurent_queue, ^{ NSLog(@"concurrent task 4");});
}
@end
