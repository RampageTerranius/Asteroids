#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>


class TTF
{
public:
	TTF();
	void Clear();
	bool SetFont(std::string fontLocation, int size);
	void SetColor(int r, int g, int b);
	void SetText(SDL_Renderer* renderer, std::string newText);
	void Draw(SDL_Renderer* renderer, int x, int y, bool centerImage);

private:
	std::string text;
	TTF_Font* font;
	SDL_Texture* texture;
	SDL_Color color;
};