// types.h
#pragma once
#include "SDL.h"

// object properties
using Bound = SDL_Rect;
using FBound = SDL_FRect;
using Point = SDL_Point;
using FPoint = SDL_FPoint;
using Vertex = SDL_Vertex;
using windowFlags = SDL_WindowFlags;
using inputProperties = SDL_PropertiesID;

// objects
using FColor = SDL_FColor;
using Color = SDL_Color;
using Texture = SDL_Texture*;
using Renderer = SDL_Renderer*;
using Window = SDL_Window*;
using Surface = SDL_Surface*;

// event
using Event = SDL_Event;