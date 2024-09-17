#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

#include <string>
#include <vector>

// Base sound class.
// Chunks and Music use this as the parent.
// When calling upon sounds in engine this type is called.
class Sound
{
public:
	Sound();

	virtual void Clear();
	virtual bool Load(std::string fileLoc, std::string newName);
	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Unpause() = 0;
	virtual void Stop() = 0;

	std::string Name() { return name; }

	int volume;

protected:
	std::string name;	
};

// Used for looping audio tracks.
// Best used for music or long sounds.
class Music : public Sound
{
public:
	Music();
	~Music();

	void Clear();
	bool Load(std::string fileLoc, std::string newName);
	void Play();
	void Pause();
	void Unpause();
	void Stop();

private:
	Mix_Music* sound;
};

// Used for simple short sounds.
// Best used with few second long sounds (gun shots, steps etc...)
class Chunk : public Sound
{
public:
	Chunk();
	~Chunk();

	void Clear();
	bool Load(std::string fileLoc, std::string newName);
	void Play();
	void Pause();
	void Unpause();
	void Stop();

	int channel;
	bool allowOverlayingSound;

private:
	Mix_Chunk* sound;
};

// Sound list class.
// Used to store a list of all sounds currently loaded into a state.
class Sounds
{
public:
	Sounds();
	void Cleanup();
	Sound* GetSound(std::string name);
	Chunk* CreateChunk(std::string fileLoc, std::string newName);
	Music* CreateMusic(std::string fileLoc, std::string newName);
	void DeleteSound(std::string name);

private:
	Sound* CreateSound(std::string fileLoc, std::string name, bool isChunk);
	std::vector<Sound*> soundList;
};

extern std::vector<Chunk*> channelList;