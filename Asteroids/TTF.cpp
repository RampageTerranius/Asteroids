#include "TTF.h"
#include "Debug.h"

#include <string>

//sdl
#include <SDL.h>
#include <SDL_ttf.h>

TTF::TTF()
{
	text = "";
	font = nullptr;
	color = { 255, 255 ,255, 0 };
	texture = nullptr;
}

//cleans up any in use fonts and surfaces, should be called on closing program
void TTF::Clear()
{
	if (texture != nullptr)
		SDL_DestroyTexture(texture);

	if (font != nullptr)
		TTF_CloseFont(font);
}

//set the currently in use font
bool TTF::SetFont(std::string fontLocation, int size)
{
	if (font != nullptr)
		TTF_CloseFont(font);

	font = TTF_OpenFont(fontLocation.c_str(), size);
	if (font != nullptr)
	{
		debug.Log("TTF", "SetFont", "TTF object loaded font into memory at : " + fontLocation + " at font size : " + std::to_string(size));
		return true;
	}
	else
	{
		debug.Log("TTF", "SetFont", "Failed to load font : " + fontLocation + " at font size : " + std::to_string(size));
		return false;
	}
}

void TTF::SetColor(int r, int g, int b)
{
	color.r = r;
	color.g = g;
	color.b = b;
};

//set the text to show, creates a surface with the given text
void TTF::SetText(SDL_Renderer* renderer, std::string newText)
{
	if (font != nullptr)
	{
		//if we already have the same message then stop here
		if (text == newText)
			return;
		else
			text = newText;

		//delete the old texture if it exists
		if (texture != nullptr)
			SDL_DestroyTexture(texture);

		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_FreeSurface(surface);
	}
}

//draws the surface to the given renderer
void TTF::Draw(SDL_Renderer* renderer, int x, int y, bool centerImage)
{
	//make sure we have data to work with
	if (renderer != nullptr && texture != nullptr && font != nullptr)
	{
		//prepare the render zones ahead of time
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		
		SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);

		if (centerImage)
		{
			rect.x -= rect.w / 2;
			rect.y -= rect.h / 2;
		}

		//render the texture of the words to the given renderer
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	}
	else
		debug.Log("TTF", "Draw", "Failed to draw TTF surface with text :" + text);
}