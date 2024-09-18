#include "Textures.h"

#include <SDL_image.h>

#include <cmath>

#include "GameEngine.h"
#include "Debug.h"

// Constructor.
Texture::Texture()
{
	name = "";
	tex = nullptr;
	rect = SDL_Rect();
	anchor = Anchor::Center;
}

// Deconstructor.
Texture::~Texture()
{
	Clear();
}

// destroy this texture and reset it to default. 
void Texture::Clear()
{
	if (tex != nullptr)
	{
		SDL_DestroyTexture(tex);
	}

	tex = nullptr;

	rect.x = 0;
	rect.y = 0;
	rect.h = 0;
	rect.w = 0;	
}

// Load a texture using the given file location and assign it a name.
// Returns true if success, false if failed to load.
bool Texture::Load(std::string fileLoc, std::string newName)
{
	if (tex != nullptr)
	{
		debug.Log("Texture", "Load", "A texture is already loaded in this object");
		return false;
	}

	SDL_Surface* surface;

	// Use base SDL_Image loading function.
	surface = IMG_Load(fileLoc.c_str());
	if (surface == nullptr)
	{
		std::string str = SDL_GetError();
		debug.Log("Texture", "Load", "Failed to load image " + str);
		return false;
	}

	// Setup the default source rect.
	rect.x = 0;
	rect.y = 0;
	rect.w = surface->w;
	rect.h = surface->h;

	// Set the color key for transparency as RGB(255, 0 , 255)
	SDL_SetColorKey(surface, true, SDL_MapRGB(surface->format, 255, 0, 255));

	// Create the new texture.
	tex = SDL_CreateTextureFromSurface(game.GetRenderer().renderer, surface);

	// Clear out the old surface.
	SDL_FreeSurface(surface);

	// If we failed to create the texture using the surface then stop here.
	if (tex == nullptr)
	{
		std::string str = SDL_GetError();
		debug.Log("Texture", "Load", "Failed to create texture from image " + str);
		return false;
	}

	name = newName;

	debug.Log("Texture", "Load", "Success loading texture at location : " + fileLoc);

	return true;
}

// Render the texture to the screen using the given coordinates.
// Renders with no rotation.
bool Texture::Draw(SDL_Renderer* renderer, int x, int y)
{
	return Draw(renderer, 0, x, y);
}

// Render the texture to the screen using the given coordinates and the given rotation.
bool Texture::Draw(SDL_Renderer* renderer, float rotation, int x, int y)
{
	if (renderer != nullptr && HasTexture())
	{
		SDL_Rect tempRect = rect;

		// Prepare the render zones ahead of time.
		tempRect.x = x;
		tempRect.y = y;
		
		// Determine where we are anchoring the texture to from the given X/Y coordinates.
		switch (anchor)
		{
			case Anchor::TopRight:
				tempRect.x -= tempRect.w;
				break;

			case Anchor::Top:
				tempRect.x -= tempRect.w / 2;				
				break;

			case Anchor::TopLeft:
				// Do nothing.
				break;

			case Anchor::Right:
				tempRect.x -= tempRect.w;
				tempRect.y -= tempRect.h / 2;
				break;

			case Anchor::Center:
				tempRect.x -= tempRect.w / 2;
				tempRect.y -= tempRect.h / 2;
			break;

			case Anchor::Left:
				tempRect.y -= tempRect.h / 2;
				break;

			case Anchor::BottomRight:
				tempRect.x -= tempRect.w;
				tempRect.y -= tempRect.h;
				break;

			case Anchor::Bottom:
				tempRect.x -= tempRect.w / 2;
				tempRect.y -= tempRect.h;
				break;

			case Anchor::BottomLeft:				
				tempRect.y -= tempRect.h;
				break;
		}

		// TODO: rework this, surely we dont need to calculate this each time we draw? we should calculate this WHEN we choose to change scale.
		if (scale != 1.0f)
		{
			tempRect.w = static_cast<int>(std::round(tempRect.w * scale));
			tempRect.h = static_cast<int>(std::round(tempRect.h * scale));;
		}

		// Render the texture to the given renderer.
		if (SDL_RenderCopyEx(renderer, tex, NULL, &tempRect, rotation, nullptr, SDL_FLIP_NONE) >= 0)
		{
			// If the texture is asking to render its coordinates then draw a red dot directly where the source location is.
			if (drawGivenCoordinates)
			{
				SDL_SetRenderDrawColor(game.GetRenderer().renderer, 255, 0, 0, 0);
				SDL_RenderDrawPoint(game.GetRenderer().renderer, x, y);
				SDL_SetRenderDrawColor(game.GetRenderer().renderer, game.GetRenderer().renderColor.r, game.GetRenderer().renderColor.g, game.GetRenderer().renderColor.b, game.GetRenderer().renderColor.a);
			}
				
			return true;
		}
	}

	return false;
}

// Return is there is a texture or not.
bool Texture::HasTexture()
{
	if (tex != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Set the given sdl_texture and set the name for this texture object.
bool Texture::SetTexture(SDL_Texture* texture, std::string newName)
{
	if (texture == nullptr)
	{
		return false;
	}

	if (newName.empty())
	{
		return false;
	}

	name = newName;
	tex = texture;

	ResetImageDimensions();

	return true;
}

// Query and set the width/height of the texture to default.
void Texture::ResetImageDimensions()
{
	SDL_QueryTexture(tex, nullptr, nullptr, &rect.w, &rect.h);
}

// Set the width and height of the texture by hand.
void Texture::SetWidthHeight(int w, int h)
{
	rect.w = w;
	rect.h = h;
}

// Set the scale of the image.
void Texture::SetScale(float newScale)
{
	if (newScale > 0.0)
	{
		scale = newScale;
	}
}

// cleanup and remove all textures from the texture list
void Textures::Cleanup()
{
	for (int i = 0; i < textureList.size(); i++)
	{
		textureList.at(i)->Clear();
		delete textureList.at(i);
	}

	textureList.clear();

	debug.Log("Textures", "Cleanup", "Destroyed all textures");
}

// Iterate over the texture list and return the first texture found with the given name.
// Returns nullptr if no texture is found.
Texture* Textures::GetTexture(std::string name)
{
	for (auto& tex : textureList)
	{
		if (tex->Name() == name)
		{
			return tex;
		}
	}

	return nullptr;
}

// Creates a new texture and adds it to the list.
// loads an image from the given location and assigning a name to the texture.
// Returns a pointer to the texture created.
Texture* Textures::CreateTexture(std::string fileLoc, std::string name)
{
	Texture* tex = nullptr;

	// First check if a texture under this name already exists, if it does return it instead.
	tex = GetTexture(name);

	if (tex != nullptr)
	{			
		debug.Log("Textures", "CreateTexture", "A texture with the name of " + name + " already exists. Returning existing texture instead.");
		return tex;
	}

	// If it doesnt exist lets create it instead.
	tex = new Texture();

	if (!tex->Load(fileLoc, name))
	{
		delete tex;
		return nullptr;
	}

	textureList.push_back(tex);

	return textureList.back();
}

// Adds a given texture to the texture list giving it the given name.
// Returns true on success, false when given texture doesnt exist.
bool Textures::AddTexture(SDL_Texture* texture, std::string name)
{
	if (texture == nullptr)
	{
		debug.Log("Textures", "AddTexture", "given texture was nullptr, can not add a blank texture");
		return false;
	}

	Texture* tex = new Texture();

	tex->SetTexture(texture, name);

	textureList.push_back(tex);

	return true;
}

// Iterate over the list of textures and delete the first texture with the given name.
void Textures::DeleteTexture(std::string name)
{
	int i = 0;

	for (auto& tex : textureList)
	{
		if (tex->Name() == name)
		{
			delete textureList[i];
			textureList.erase(textureList.begin() + i);
			break;
		}
		else
		{
			i++;
		}
	}
}