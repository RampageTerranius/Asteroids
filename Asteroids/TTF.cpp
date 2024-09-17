#include "TTF.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

#include "Debug.h"

// Constructor, set no font and default size.
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

// Constructor, set font and size.
TTF::TTF(SDL_Renderer* givenRenderer, std::string newName, std::string fontLocation, int size)
{
	name = newName;
	active = true;
	text = "";
	color = { 255, 255 ,255, 0 };
	texture = Texture();
	renderer = givenRenderer;

	if (!SetFont(fontLocation, size))
	{
		font = nullptr;
	}
}

// Deconstructor.
TTF::~TTF()
{
	Clear();
}

// Cleans up any in use fonts and surfaces, should be called on closing program.
void TTF::Clear()
{
	if (texture.HasTexture())
	{
		texture.Clear();
	}

	if (font != nullptr)
	{
		TTF_CloseFont(font);
		font = nullptr;
	}
}

// Set the anchor point for placign the texture.
void TTF::SetAnchor(Anchor newAnchor)
{
	texture.anchor = newAnchor;
}

// Set the currently in use font.
bool TTF::SetFont(std::string fontLocation, int size)
{
	if (font != nullptr)
	{
		debug.Log("TTF", "SetFont", "TTF object already had a font, closing font...");
		TTF_CloseFont(font);
	}

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

// Set the RBG color for this TTF with no alpha.
void TTF::SetColor(int r, int g, int b)
{
	SetColor(r, g, b, 255);
};

// Set the RGBA color for this TTF.
void TTF::SetColor(int r, int g, int b, int a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;

	Update();
};

// Forces the TTF to update its texture.
void TTF::Update()
{
	if (font == nullptr || text == "")
	{
		debug.Log("TTF", "Update", "Attempt to update TTF that is not initialized or has no name");
		return;
	}

	// Delete the old texture if it exists.
	if (texture.HasTexture())
	{
		texture.Clear();
	}

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);	

	texture.SetTexture(newTexture, text);

	SDL_FreeSurface(surface);
}

// Set the text to show, creates a surface with the given text.
void TTF::SetText(std::string newText)
{
	if (font == nullptr)
	{
		return;
	}

	// If we already have the same message then stop here.
	if (text == newText)
	{
		return;
	}
	else
	{
		text = newText;
	}

	Update();
};

// Draws the surface to the given renderer.
void TTF::Draw()
{
	// Make sure we have data to work with.
	if (!texture.Draw(renderer, x, y))
	{
		debug.Log("TTF", "Draw", "Failed to draw TTF surface with text :" + text);
	}
}

// Check if the given point intersects with this TTF texture.
bool TTF::PointIntersectsTexture(SDL_Point point)
{
	if (!texture.HasTexture())
	{
		return false;
	}

	SDL_Rect rect = texture.Rect();

	// Check what anchor type we have and change location considered an interection to compensate.
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

	// SDL_PointInRect returns an SDL_Bool.
	// Internally this is a 0 for false and 1 for true so we can type cast this back to bool.
	return static_cast<bool>(SDL_PointInRect(&point, &rect));
}

// Creates a new TTF object and adds it to the list with the given name.
// Sets no font and default font size.
TTF* TTFs::CreateTTF(SDL_Renderer* givenRenderer, std::string newName)
{
	TTF* newTTF = new TTF(givenRenderer, newName);
	ttfList.push_back(newTTF);

	debug.Log("TTFs", "CreateTTF", "Pushed new TTF into list.");

	return newTTF;
}

// Creates a new TTF object and adds it to the list with the given name.
// Loads the fond at the given location and sets to given font size.
TTF* TTFs::CreateTTF(SDL_Renderer* givenRenderer, std::string newName, std::string fontLocation, int size)
{
	TTF* newTTF = new TTF(givenRenderer, newName, fontLocation, size);
	ttfList.push_back(newTTF);

	debug.Log("TTFs", "CreateTTF", "Pushed new TTF into list.");

	return newTTF;
}

// Searches the TTF list for the given TTF, deletes it then clears it from the list.
void TTFs::DestroyTTF(TTF* destroyTTF)
{
	for (auto ttf : ttfList)
	{
		if (ttf == destroyTTF)
		{
			ttfList.remove(destroyTTF);
			destroyTTF->Clear();
			delete destroyTTF;
			debug.Log("TTFs", "DestroyTTF", "Removed and destroyed TTF from list");
			return;
		}
	}
}

// Searches the TTF list for a TTF with the given name, deletes it then clears it from the list.
void TTFs::DestroyTTF(std::string destroyName)
{
	for (auto ttf : ttfList)
	{
		if (destroyName == ttf->Name())
		{
			ttfList.remove(ttf);
			ttf->Clear();
			delete ttf;
			debug.Log("TTFs", "DestroyTTF", "Removed and destroyed TTF from list");
			return;
		}
	}
}

// Search the list of TTFs for a TTF with the given name.
// Returns any found TTF, returns nullptr if no TTF with given name exists.
TTF* TTFs::GetTTF(std::string findName)
{
	for (auto ttf : ttfList)
	{
		if (findName == ttf->Name())
		{
			return ttf;
		}
	}

	return nullptr;
}

// Iterate over the TTF lsit and render all TTF objects.
void TTFs::RenderAll()
{
	for (auto ttf : ttfList)
	{
		if (ttf->active)
		{
			ttf->Draw();
		}
	}
}

// Iterate over the TTF list, clear and delete all.
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