#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "Textures.h"

class TTF
{
public:
	TTF(SDL_Renderer* givenRenderer);
	void Clear();
	bool SetFont(std::string fontLocation, int size);
	void SetColor(int r, int g, int b);
	void SetText(std::string newText);
	void Draw();
	bool PointIntersectsTexture(SDL_Point point);
	int x, y;
	bool centerImage = false;

private:
	void Update();

	SDL_Renderer* renderer;
	std::string text;
	TTF_Font* font;
	Texture texture;
	SDL_Color color;
};