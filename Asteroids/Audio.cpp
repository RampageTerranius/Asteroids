#include "Audio.h"

#include "Debug.h"

const int CHANNEL_LIMIT = 32;
std::vector<Chunk*> channelList;

// Base constructor.
Sound::Sound()
{
	Clear();
}

// Clear the sounds data.
void Sound::Clear()
{
	name = "";
	volume = 128;
}

// Base sound can not load sounds, always clear and report a failure.
bool Sound::Load(std::string fileLoc, std::string name)
{
	Clear();
	return false;
}

// Music source.
// Single audio source used primarily for playing music.

// Constructor.
Music::Music()
{
	Clear();
}

// Deconstructor.
Music::~Music()
{
	Clear();
}

// Reset the object to default states.
void Music::Clear()
{
	Sound::Clear();
	if (sound != nullptr)
	{
		Mix_FreeMusic(sound);
		sound = nullptr;
	}
}

// Load a sound into the object.
bool Music::Load(std::string fileLoc, std::string newName)
{
	// Attempt to load the file.
	sound = Mix_LoadMUS(fileLoc.c_str());
	if (sound != nullptr)
	{
		name = newName;
		return true;
	}

	// If failed to load, clear music.
	Clear();
	return false;
}

// Play music using internal SDL music functions.
void Music::Play()
{
	// First check if a music file is already playing.
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(sound, -1);
		Mix_VolumeMusic(volume);
	}
	// If it is already playing then either pause or unpause the music instead.
	else if (Mix_PausedMusic() == 1)
	{
		Unpause();
	}
	else
	{
		Pause();
	}
}

// Pause currently playing music.
void Music::Pause()
{
	Mix_PauseMusic();
}

// Unpause currently playing music.
void Music::Unpause()
{
	Mix_ResumeMusic();
}

// Stop the currently playing music.
void Music::Stop()
{
	Mix_HaltMusic();
}

// Audio Chunk.
// Used with short sounds that can overlap each other or play multiple times.
// Constructor.
Chunk::Chunk()
{
	Clear();
}

// Deconstructor.
Chunk::~Chunk()
{
	Clear();
}

// Used by Chunk::Clear to clean up channel list upon sound ending.
void OnChannelFinish(int channel)
{
	// When the chunk finishes playing reset its channel back to -1 so that we know its complete and we can replay it.
	channelList[channel]->channel = -1;
	channelList[channel] = nullptr;
	Mix_Volume(channel, 128);
}

// Clear the chunk and recover the channel it is on.
void Chunk::Clear()
{
	name = "";
	channel = -1;
	allowOverlayingSound = true;
	volume = 128;
	if (sound != nullptr)
	{
		Mix_FreeChunk(sound);
		sound = nullptr;
	}

	Mix_ChannelFinished(OnChannelFinish);
}

// Load a file into the chunk and prepare it for use.
bool Chunk::Load(std::string fileLoc, std::string newName)
{
	sound = Mix_LoadWAV(fileLoc.c_str());
	if (sound != nullptr)
	{
		name = newName;
		return true;
	}

	Clear();

	return false;
}

// Assign a channel and play the sound.
void Chunk::Play()
{
	// If the sound is already playing and you are not allowed to overlay the sound then do not play the sound.
	if (!allowOverlayingSound && channel != -1)
	{
		return;
	}

	// Assign a channel to the sound and attempt to play it.
	channel = Mix_PlayChannel(-1, sound, 0);
	if (channel >= 0 && channel < CHANNEL_LIMIT)
	{
		Mix_Volume(channel, volume);
		channelList[channel] = this;
	}
}

// Pause the chunk on its channel.
void Chunk::Pause()
{
	if (channel >= 0)
	{
		if (channelList[channel] != nullptr)
		{
			Mix_Pause(channel);
		}
	}
}

// Unpause the chunk on its channel.
void Chunk::Unpause()
{
	if (channel >= 0)
	{
		if (channelList[channel] != nullptr)
		{
			Mix_Resume(channel);
		}
	}
}

// Stop the chunk playing on the channel it is on.
void Chunk::Stop()
{
	if (channel >= 0)
	{
		if (channelList[channel] != nullptr)
		{		
			Mix_HaltChannel(channel);
		}
	}
}

// Sound list.
// Used by game states to store a list of all loaded sounds.

// Constructor
Sounds::Sounds()
{
	// Make sure the vector is empty.
	channelList.clear();
	channelList.resize(0);

	// Reserve and resize the channel to the max channel limit.
	channelList.reserve(CHANNEL_LIMIT);
	channelList.resize(CHANNEL_LIMIT);
}

// Go through the entire sound list and clear all sounds.
// Mainly used during shutdown but can be used to clear all current sounds.
void Sounds::Cleanup()
{
	for (auto& sound : soundList)
	{
		sound->Clear();
	}

	debug.Log("Sounds", "Cleanup", "Destroyed all sounds");
}

// Search the lsit for a sound with the given name.
// Returns the sound if it exists, nullptr if not.
// Sound is returned as a base Sound object, you must type cast it to the required type (Chunk or Music)
Sound* Sounds::GetSound(std::string name)
{
	for (auto& sound : soundList)
	{
		if (sound->Name() == name)
		{
			return sound;
		}
	}

	return nullptr;
}


// Private function, Create a sound from the given location with a name and determine if its a chunk or not.
// Returns a base Sound object, is type casted by public functions.
Sound* Sounds::CreateSound(std::string fileLoc, std::string name, bool isChunk)
{
	Sound* sound = nullptr;
	// Check if we need to make this a chunk or a music object.
	if (isChunk)
	{
		sound = new Chunk();
	}
	else
	{
		sound = new Music();
	}

	// Attempt to load the sound.
	if (!sound->Load(fileLoc, name))
	{
		delete sound;
		return nullptr;
	}

	// IF success push the sound into the list and return a pointer to it.
	soundList.push_back(sound);
	return soundList.back();
}

// Create a chunk of audio from the file location and assigns it a name in the list.
Chunk* Sounds::CreateChunk(std::string fileLoc, std::string name)
{
	return (Chunk*)CreateSound(fileLoc, name, true);
}

// Create music from the file location and assigns it a name in the list.
Music* Sounds::CreateMusic(std::string fileLoc, std::string name)
{
	return (Music*)CreateSound(fileLoc, name, false);
}

// Search the entire audio file list and delete the first file foudn with that name.
void Sounds::DeleteSound(std::string name)
{
	int i = 0;
	for (auto& tex : soundList)
	{
		// Iterate over the array and look for any audio file with the given name.
		if (tex->Name() == name)
		{
			soundList.erase(soundList.begin() + i);
			break;
		}
		else
		{
			i++;
		}
	}
}