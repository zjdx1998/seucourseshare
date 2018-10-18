//
//  main.m
//  Shapes-Green-Circles
//
//  Created by Waqar Malik on 4/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


// --------------------------------------------------
// constants for the different kinds of shapes and their colors

typedef enum {
	kRedColor,
	kGreenColor,
	kBlueColor
} ShapeColor;


// --------------------------------------------------
// Shape bounding rectangle


typedef struct {
	int x, y, width, height;
} ShapeRect;


// --------------------------------------------------
// convert from the ShapeColor enum value to a human-readable name

NSString *colorName (ShapeColor color)
{
	NSString *colorName;
	
	switch (color) {
		case kRedColor:
			colorName = @"red";
			break;
		case kGreenColor:
			colorName = @"green";
			break;
		case kBlueColor:
			colorName = @"blue";
			break;
		default:
			colorName = @"oops! Unexpected color";
			break;
	}
	
	return (colorName);
	
} // colorName


// --------------------------------------------------
// Shapes base class

@interface Shape : NSObject
{
	ShapeColor  fillColor;
	ShapeRect   bounds;
}

- (void) setFillColor: (ShapeColor) fillColor;
- (void) setBounds: (ShapeRect) bounds;
- (void) draw;

@end // Shape


@implementation Shape

- (void) setFillColor: (ShapeColor) c
{
	fillColor = c;
} // setFillColor

- (void) setBounds: (ShapeRect) b
{
	bounds = b;
} // setBounds

- (void) draw
{
} // draw

@end // Shape




// --------------------------------------------------
// All about Triangles

@interface Triangle : Shape
@end // Triangle


@implementation Triangle

- (void) draw
{
	NSLog (@"drawing a triangle at (%d %d %d %d) in %@",
		   bounds.x, bounds.y, 
		   bounds.width, bounds.height,
		   colorName(fillColor));
} // draw

@end // Triangle




// --------------------------------------------------
// All about Circles

@interface Circle : Shape
@end // Circle


@implementation Circle

// I'm new!
- (void) setFillColor: (ShapeColor) c
{
	if (c == kRedColor) {
		c = kGreenColor;
	}
	
	[super setFillColor: c];
	
} // setFillColor


- (void) draw
{
	NSLog (@"drawing a circle at (%d %d %d %d) in %@",
		   bounds.x, bounds.y, 
		   bounds.width, bounds.height,
		   colorName(fillColor));
} // draw

@end // Circle




// --------------------------------------------------
// All about Rectangles

@interface Rectangle : Shape
@end // Rectangle


@implementation Rectangle

- (void) draw
{
	NSLog (@"drawing a rectangle at (%d %d %d %d) in %@",
		   bounds.x, bounds.y, 
		   bounds.width, bounds.height,
		   colorName(fillColor));
} // draw

@end // Rectangle


// --------------------------------------------------
// All about OblateSphereoids

@interface OblateSphereoid : Shape
@end // OblateSphereoid


@implementation OblateSphereoid

- (void) draw
{
	NSLog (@"drawing an egg at (%d %d %d %d) in %@",
		   bounds.x, bounds.y, 
		   bounds.width, bounds.height,
		   colorName(fillColor));
} // draw

@end // OblateSphereoid



// --------------------------------------------------
// Draw the shapes

void drawShapes (id shapes[], int count)
{
	for (int i = 0; i < count; i++)
    {
		id shape = shapes[i];
		[shape draw];
	}
	
} // drawShapes



// --------------------------------------------------
// The main function.  Make the shapes and draw them

int main (int argc, const char * argv[]) 
{
	id shapes[4]; // I'm different!
	
	ShapeRect rect0 = { 0, 0, 10, 30 };
	shapes[0] = [Circle new];
	[shapes[0] setBounds: rect0];
	[shapes[0] setFillColor: kRedColor];
	
	ShapeRect rect1 = { 30, 40, 50, 60 };
	shapes[1] = [Rectangle new];
	[shapes[1] setBounds: rect1];
	[shapes[1] setFillColor: kGreenColor];
	
	ShapeRect rect2 = { 15, 19, 37, 29 };
	shapes[2] = [OblateSphereoid new];
	[shapes[2] setBounds: rect2];
	[shapes[2] setFillColor: kBlueColor];
	
	ShapeRect rect3 = { 47, 32, 80, 50 };	// I'm new!
	shapes[3] = [Triangle new];
	[shapes[3] setBounds: rect3];
	[shapes[3] setFillColor: kRedColor];
	
	drawShapes (shapes, 4); // I'm different!
	
	return (0);
	
} // main
