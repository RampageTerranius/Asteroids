#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <list>

#include "Textures.h"

class TTF
{
public:
	TTF(SDL_Renderer* givenRenderer, std::string newName);
	TTF(SDL_Renderer* givenRenderer, std::string newName, std::string fontLocation, int size);
	~TTF();

	void Clear();
	bool SetFont(std::string fontLocation, int size);
	void SetColor(int r, int g, int b);
	void SetColor(int r, int g, int b, int a);
	void SetText(std::string newText);
	void Draw();
	bool PointIntersectsTexture(SDL_Point point);
	int x, y;
	void SetAnchor(Anchor newAnchor);
	Anchor GetAnchor() { return texture.anchor; };
	
	bool active;
	std::string Name() { return name; }	

	int TexWidth() { return texture.Rect().w; };
	int TexHeight() { return texture.Rect().h; };

private:
	void Update();
	
	std::string name;
	SDL_Renderer* renderer;
	std::string text;
	TTF_Font* font;
	Texture texture;
	SDL_Color color;
};

class TTFs
{
public:
	TTF* CreateTTF(SDL_Renderer* givenRenderer, std::string newName);
	TTF* CreateTTF(SDL_Renderer* givenRenderer, std::string newName, std::string fontLocation, int size);
	void DestroyTTF(TTF* destroyTTF); 
	void DestroyTTF(std::string destroyName);
	TTF* GetTTF(std::string findName);
	void RenderAll();
	void ClearAll();

private:
	std::list<TTF*> ttfList;
};