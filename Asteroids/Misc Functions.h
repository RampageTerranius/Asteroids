#pragma once

#include <SDL.h>
#include <string>

std::string GetEXEPath();
float GetDistance(SDL_Point pointA, SDL_Point pointB);
float RandomFloat(float min, float max);