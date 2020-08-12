#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>

class Texture
{
public:
	Texture();
	~Texture();

	void Clear();
	bool Draw(SDL_Renderer* renderer, float rotation, int x, int y);
	bool Draw(SDL_Renderer* renderer, int x, int y);
	bool Load(std::string fileLoc, std::string newName);
	bool SetTexture(SDL_Texture* texture, std::string newName);
	bool HasTexture();

	std::string Name() { return name; };
	SDL_Rect Rect() { return rect; };
	void SetWidthHeight(int w, int h);
	void SetScale(float newScale);
	void ResetImageDimensions();
	SDL_Point Center() { SDL_Point centerPoint; centerPoint.y = static_cast<int> (round(static_cast<float> (rect.h / 2))); centerPoint.x = static_cast<int> (round(static_cast<float> (rect.w / 2))); return centerPoint; }

	bool centerTextureOnDraw = true;	

private:
	float scale = 1.0;
	std::string name;
	SDL_Texture* tex;
	SDL_Rect rect;	
};

class Textures
{
private:
	std::vector<Texture*> textureList;

public:
	void Cleanup();
	Texture* GetTexture(std::string name);
	Texture* CreateTexture(std::string fileLoc, std::string name);
	bool AddTexture(SDL_Texture* texture, std::string name);
	void DeleteTexture(std::string name);
};
