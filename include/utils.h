// utils.h
#pragma once
#include "zenv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fungsi untuk mengkonversi karakter hex ke nilai integer
inline int hexCharToInt(char c);

// Fungsi untuk mengkonversi string hex ke SDL_Color
inline Color Hex(const char* hex);