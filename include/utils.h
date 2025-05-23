// utils.h
#pragma once
#include "zenv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hexCharToInt(char c);
Color Hex(const char* hex);
FPoint Point2FPoint(Point& p);
Point FPoint2Point(FPoint& p);
FBound Bound2FBound(Bound& b);
Bound FBound2Bound(FBound& b);