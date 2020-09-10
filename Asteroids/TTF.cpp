#include "TTF.h"
#include "Debug.h"

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

TTF::TTF(SDL_Renderer* givenRenderer, std::string newName)
{
	name = newName;
	active = true;
	text = "";
	font = nullptr;
	color = { 255, 255 ,255, 0 };
	texture = Texture();
	renderer = givenRenderer;
}

TTF::TTF(SDL_Renderer* givenRenderer, std::string newName, std::string fontLocation, int size)
{
	name = newName;
	active = true;
	text = "";
	color = { 255, 255 ,255, 0 };
	texture = Texture();
	renderer = givenRenderer;

	if (!SetFont(fontLocation, size))
		font = nullptr;
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
	{
		TTF_CloseFont(font);
		font = nullptr;
	}
}

void TTF::SetAnchor(Anchor newAnchor)
{
	texture.anchor = newAnchor;
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

	switch (texture.anchor)
	{
	case Anchor::TopRight:
		rect.x = x - rect.w;
		rect.y = y;
		break;

	case Anchor::Top:
		rect.x = x - (rect.w / 2);
		rect.y = y;
		break;

	case Anchor::TopLeft:
		rect.x = x;
		rect.y = y;
		break;

	case Anchor::Right:
		rect.x = x - rect.w;
		rect.y = y - (rect.h / 2);
		break;

	case Anchor::Center:
		rect.x = x - (rect.w / 2);
		rect.y = y - (rect.h / 2);
		break;

	case Anchor::Left:
		rect.x = x;
		rect.y = y - (rect.h / 2);
		break;

	case Anchor::BottomRight:
		rect.x = x - rect.w;
		rect.y = y - rect.h;		
		break;

	case Anchor::Bottom:
		rect.x = x - (rect.w / 2);		
		rect.y = y - rect.h;
		break;

	case Anchor::BottomLeft:
		rect.x = x;		
		rect.y = y - rect.h;
		break;
	}

	if (SDL_PointInRect(&point, &rect))
		return true;
	else
		return false;
}


TTF* TTFs::CreateTTF(SDL_Renderer* givenRenderer, std::string newName)
{
	TTF* newTTF = new TTF(givenRenderer, newName);
	ttfList.push_back(newTTF);

	debug.Log("TTFs", "CreateTTF", "Pushed new TTF into list.");

	return newTTF;
}

TTF* TTFs::CreateTTF(SDL_Renderer* givenRenderer, std::string newName, std::string fontLocation, int size)
{
	TTF* newTTF = new TTF(givenRenderer, newName, fontLocation, size);
	ttfList.push_back(newTTF);

	debug.Log("TTFs", "CreateTTF", "Pushed new TTF into list.");

	return newTTF;
}

void TTFs::DestroyTTF(TTF* destroyTTF)
{
	for (auto ttf : ttfList)
		if (ttf == destroyTTF)
		{
			ttfList.remove(destroyTTF);
			destroyTTF->Clear();
			delete destroyTTF;
			debug.Log("TTFs", "DestroyTTF", "Removed and destroyed TTF from list");
			return;
		}
}

void TTFs::DestroyTTF(std::string destroyName)
{
	for (auto ttf : ttfList)
		if (destroyName == ttf->Name())
		{
			ttfList.remove(ttf);
			ttf->Clear();
			delete ttf;
			debug.Log("TTFs", "DestroyTTF", "Removed and destroyed TTF from list");
			return;
		}
}

TTF* TTFs::GetTTF(std::string findName)
{
	for (auto ttf : ttfList)
		if (findName == ttf->Name())
			return ttf;

	return nullptr;
}

void TTFs::RenderAll()
{
	for (auto ttf : ttfList)
		if (ttf->active)
			ttf->Draw();
}

void TTFs::ClearAll()
{
	for (auto ttf : ttfList)
	{
		ttf->Clear();
		delete ttf;
	}

	ttfList.clear();

	debug.Log("TTFs", "ClearAll", "Removed and destroyed ALL TTF from list");
}