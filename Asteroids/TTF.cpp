#include "TTF.h"
#include "Debug.h"

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

TTF::TTF(SDL_Renderer* givenRenderer)
{
	text = "";
	font = nullptr;
	color = { 255, 255 ,255, 0 };
	texture = Texture();
	renderer = givenRenderer;
}

TTF::~TTF()
{
	Clear();
}

// Cleans up any in use fonts and surfaces, should be called on closing program.
void TTF::Clear()
{
	if (texture.HasTexture())
		texture.Clear();

	if (font != nullptr)
		TTF_CloseFont(font);
}

void TTF::CenterImage(bool center)
{
	texture.centerTextureOnDraw = center;
}

bool TTF::IsImageCentered()
{
	return texture.centerTextureOnDraw;
}

// Set the currently in use font.
bool TTF::SetFont(std::string fontLocation, int size)
{
	if (font != nullptr)
		TTF_CloseFont(font);

	font = TTF_OpenFont(fontLocation.c_str(), size);
	if (font != nullptr)
	{
		debug.Log("TTF", "SetFont", "TTF object loaded font into memory at : " + fontLocation + " at font size : " + std::to_string(size));
		Update();
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
	if (color.r == r || color.g == g || color.b == b) // Make sure to check if we need to do any updates to begin with.
		return;
	
	color.r = r;
	color.g = g;
	color.b = b;	

	Update();
};

void TTF::Update()
{
	if (font == nullptr || text == "")
		return;

	// Delete the old texture if it exists.
	if (texture.HasTexture())
		texture.Clear();

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);	

	texture.SetTexture(newTexture, text);

	SDL_FreeSurface(surface);
}

// Set the text to show, creates a surface with the given text.
void TTF::SetText(std::string newText)
{
	if (font == nullptr)
		return;

	// If we already have the same message then stop here.
	if (text == newText)
		return;
	else
		text = newText;

	Update();
};

// Draws the surface to the given renderer.
void TTF::Draw()
{
	// Make sure we have data to work with.
	if (!texture.Draw(renderer, x, y))
		debug.Log("TTF", "Draw", "Failed to draw TTF surface with text :" + text);
}

bool TTF::PointIntersectsTexture(SDL_Point point)
{
	if (!texture.HasTexture())
		return false;

	SDL_Rect rect = texture.Rect();

	if (centerImage)
	{
		rect.x = x - (rect.w / 2);
		rect.y = y - (rect.h / 2);

		if (SDL_PointInRect(&point, &rect))
			return true;
	}
	else
	{
		rect.x = x;
		rect.y = y;

		if (SDL_PointInRect(&point, &rect))
			return true;
	}

	return false;
}