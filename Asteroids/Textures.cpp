#include "Textures.h"
#include "GameEngine.h"
#include "Debug.h"

Texture::Texture()
{
	name = "";
	tex = nullptr;
	rect = SDL_Rect();
}

Texture::~Texture()
{
	Clear();
}

void Texture::Clear()
{
	if (tex != nullptr)
		SDL_DestroyTexture(tex);

	tex = nullptr;

	rect.x = 0;
	rect.y = 0;
	rect.h = 0;
	rect.w = 0;
}

bool Texture::Load(std::string fileLoc, std::string name)
{
	debug.Log("Texture", "Load", "Attempting to load texture at location : " + fileLoc);

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

	// Setup the default source rect
	rect.x = 0;
	rect.y = 0;
	rect.w = surface->w;
	rect.h = surface->h;

	// Set the color key for transparency as RGB(255, 0 , 255)
	SDL_SetColorKey(surface, true, SDL_MapRGB(surface->format, 255, 0, 255));

	// Create the new texture
	tex = SDL_CreateTextureFromSurface(game.GetRenderer().renderer, surface);

	// Clear out the old surface.
	SDL_FreeSurface(surface);

	if (tex == nullptr)
	{
		std::string str = SDL_GetError();
		debug.Log("Texture", "Load", "Failed to create texture from image " + str);
		return false;
	}

	this->name = name;

	debug.Log("Texture", "Load", "Success loading texture at location : " + fileLoc);

	return true;
}

bool Texture::Draw(SDL_Renderer* renderer, int x, int y)
{
	return Draw(renderer, 0, x, y);
}

bool Texture::Draw(SDL_Renderer* renderer, float rotation, int x, int y)
{
	if (renderer != nullptr && HasTexture())
	{
		// Prepare the render zones ahead of time.
		rect.x = x;
		rect.y = y;

		if (this->centerTextureOnDraw)
		{
			rect.x -= rect.w / 2;
			rect.y -= rect.h / 2;
		}

		// Render the texture to the given renderer.
		if (SDL_RenderCopyEx(renderer, tex, NULL, &rect, rotation, nullptr, SDL_FLIP_NONE) >= 0)
			return true;
	}

	return false;
}

bool Texture::HasTexture()
{
	if (this->tex != nullptr)
		return true;
	else
		return false;
}

bool Texture::SetTexture(SDL_Texture* texture, std::string name)
{
	if (texture == nullptr)
		return false;

	if (name.empty())
		return false;

	this->name = name;
	this->tex = texture;

	SDL_QueryTexture(texture, nullptr, nullptr, &this->rect.w, &this->rect.h);

	return true;
}

void Textures::Cleanup()
{
	for (auto it = this->textureList.begin(); it != this->textureList.end();)
		{
			(*it)->Clear();
			delete (*it);
			(*it) = nullptr;
			it = this->textureList.erase(it);
		}	

	debug.Log("Textures", "Cleanup", "Destroyed all textures");
}

Texture* Textures::GetTexture(std::string name)
{
	for (auto& tex : this->textureList)
		if (tex->Name() == name)
			return tex;

	return nullptr;
}

Texture* Textures::CreateTexture(std::string fileLoc, std::string name)
{
	Texture* tex = nullptr;

	// First check if a texture under this name already exists.
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

void Textures::DeleteTexture(std::string name)
{
	int i = 0;

	for (auto& tex : textureList)
		if (tex->Name() == name)
		{
			delete textureList[i];
			textureList.erase(textureList.begin() + i);	
			break;
		}
		else
			i++;
}