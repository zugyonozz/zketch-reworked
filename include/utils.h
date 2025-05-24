// utils.h
#pragma once
#include "ztype.h"

int hexCharToInt(char c);
Color Hex(const char* hex);
FPoint Point2FPoint(Point& p);
Point FPoint2Point(FPoint& p);
FBound Bound2FBound(Bound& b);
Bound FBound2Bound(FBound& b);
FPoint getMousePos();
bool isHover(Bound b);
bool isOnClick(const Event& e, Bound b);