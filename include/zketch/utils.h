// utils.h
#pragma once
#include "ztype.h"

// Convert string to Color, default color solid black.
// `Parameter`
// - `hex` 
// hex is const char* value with format color hex.
//
// `Return`
// - `Color`
// Color is wrapper of objects { Uint8 r, Uint8 g, Uint8 b, Uint8 a }.
//
// `Format`
// RGB, RGBA, RRGGBB, RRGGBBAA
Color Hex(const char* hex);

// Convert Point to FPoint.
// `Parameter `
// - `p` 
// p is Point& value of current position.
//
// `Return`
// - `FPoint` 
// FPoint is float version of Point.
FPoint Point2FPoint(Point& p);

// Convert FPoint to Point.
// `Parameter `
// - `p` 
// p is FPoint& value of current position.
//
// `Return`
// - `Point` 
// Point is wrapper of objects { int x, int y }.
Point FPoint2Point(FPoint& p);

// Convert Bound to FBound.
// `Parameter `
// - `b` 
// b is Bound& value of current boundary.
//
// `Return`
// - `FBound` 
// FBound is float version of Bound.
FBound Bound2FBound(Bound& b);

// Convert FBound to Bound.
// `Parameter `
// - `b` 
// b is FBound& value of current boundary.
//
// `Return`
// - `Bound` 
// Bound is wrapper of objects { int x, int y, int w,  int h }.
Bound FBound2Bound(FBound& b);

// Convert Color to FColor
// `Parameter`
// - `c` 
// c is Color& value of current color.
//
// `Return`
// - `FColor`
// FColor is float version of color.
FColor Color2FColor(Color& c);

// Convert Color to FColor
// `Parameter`
// - `c` 
// c is FColor& value of current color.
//
// `Return`
// - `Color`
// Color is wrapper of objects { Uint8 r, Uint8 g, Uint8 b, Uint8 a }.
Color FColor2Color(FColor& c);

// Function used to track mouse position
// `Return`
// - `FPoint`
// FPoint is float version of Point.
FPoint getMousePos();

// Function used to check if mouse position in boundary
// `Return`
// - `bool`
// if postion in boundary return True, if not return False.
bool isHover(Bound b);

// Function used to check if mouse click position in boundary
// `Return`
// - `bool`
// if click position in boundary return True, if not return False.
bool isOnClick(const Event& e, Bound b);