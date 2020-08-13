#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>

class Sound
{
public:
	Sound();

	virtual void Clear();
	virtual bool Load(std::string fileLoc, std::string name);
	virtual void Play();
	virtual void Pause();
	virtual void Unpause();
	virtual void Stop();

	std::string Name() { return name; }

protected:
	std::string name;
};

class Music : public Sound
{
public:
	Music();
	~Music();

	void Clear();
	bool Load(std::string fileLoc, std::string name);
	void Play();
	void Pause();
	void Unpause();
	void Stop();

private:
	Mix_Music* sound;
};

class Chunk : public Sound
{
public:
	Chunk();
	~Chunk();

	void Clear();
	bool Load(std::string fileLoc, std::string name);
	void Play();
	void Pause();
	void Unpause();
	void Stop();

	int channel;
	bool allowOverlayingSound;

private:
	Mix_Chunk* sound;
};

class Sounds
{
public:
	Sounds();
	void Cleanup();
	Sound* GetSound(std::string name);
	Sound* CreateChunk(std::string fileLoc, std::string name);
	Sound* CreateMusic(std::string fileLoc, std::string name);
	void DeleteSound(std::string name);

private:
	Sound* CreateSound(std::string fileLoc, std::string name, bool isChunk);
	std::vector<Sound*> soundList;
};

extern std::vector<Chunk*> channelList;