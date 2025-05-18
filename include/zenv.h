// zenv.h
#pragma once
#include <iostream>
#include <vector>
#include "SDL3/SDL.h"
#include "SDL3/SDL_image.h"
#include "SDL3/SDL_ttf.h"

using Window = SDL_Window*;
using Renderer = SDL_Renderer*;
using Font = TTF_Font*;

using Bound = SDL_Rect;
using FBound = SDL_FRect;
using Point = SDL_Point;
using FPoint = SDL_FPoint;
using Color = SDL_Color;
using Vertex = SDL_Vertex;

using Texture = SDL_Texture*;
using Surface = SDL_Surface*;
using Event = SDL_Event;

using windowFlags = SDL_WindowFlags;
using inputProperties = SDL_PropertiesID;