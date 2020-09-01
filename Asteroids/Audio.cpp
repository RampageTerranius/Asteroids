#include "Audio.h"
#include "Debug.h"

const int CHANNEL_LIMIT = 32;
std::vector<Chunk*> channelList;

Sound::Sound()
{
	Clear();
}

void Sound::Clear()
{
	name = "";
}

bool Sound::Load(std::string fileLoc, std::string name)
{
	Clear();
	return false;
}

void Sound::Play()
{

}

void Sound::Pause()
{

}

void Sound::Unpause()
{

}

void Sound::Stop()
{

}

Music::Music()
{
	Clear();
}

Music::~Music()
{
	Clear();
}

void Music::Clear()
{
	name = "";
	volume = 128;
	if (sound != nullptr)
	{
		Mix_FreeMusic(sound);
		sound = nullptr;
	}
}

bool Music::Load(std::string fileLoc, std::string newName)
{
	sound = Mix_LoadMUS(fileLoc.c_str());
	if (sound != nullptr)
	{
		name = newName;
		return true;
	}

	Clear();

	return false;
}

void Music::Play()
{
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(sound, -1);
		Mix_VolumeMusic(volume);
	}
	else if (Mix_PausedMusic() == 1)
		Unpause();
	else
		Pause();
}

void Music::Pause()
{
	Mix_PauseMusic();
}

void Music::Unpause()
{
	Mix_ResumeMusic();
}

void Music::Stop()
{
	Mix_HaltMusic();
}

Chunk::Chunk()
{
	Clear();
}

Chunk::~Chunk()
{
	Clear();
}

void OnChannelFinish(int channel)
{
	channelList[channel]->channel = -1;
	channelList[channel] = nullptr;
	Mix_Volume(channel, 128);
}

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

void Chunk::Play()
{
	if (!allowOverlayingSound)
		if (channel == -1)
			return;

	channel = Mix_PlayChannel(-1, sound, 0);
	if (channel >= 0 && channel < CHANNEL_LIMIT)
	{
		Mix_Volume(channel, volume);
		channelList[channel] = this;
	}
}

void Chunk::Pause()
{
	if (channel >= 0)
		if (channelList[channel] != nullptr)
			Mix_Pause(channel);
}

void Chunk::Unpause()
{
	if (channel >= 0)
		if (channelList[channel] != nullptr)
			Mix_Resume(channel);
}

void Chunk::Stop()
{
	if (channel >= 0)
		if (channelList[channel] != nullptr)
			Mix_HaltChannel(channel);
}

Sounds::Sounds()
{
	channelList.clear();
	channelList.resize(0);
	channelList.reserve(CHANNEL_LIMIT);
	channelList.resize(CHANNEL_LIMIT);
}

void Sounds::Cleanup()
{
	for (auto& sound : soundList)
		sound->Clear();

	debug.Log("Sounds", "Cleanup", "Destroyed all sounds");
}

Sound* Sounds::GetSound(std::string name)
{
	for (auto& sound : soundList)
		if (sound->Name() == name)
			return sound;

	return nullptr;
}

Sound* Sounds::CreateSound(std::string fileLoc, std::string name, bool isChunk)
{
	Sound* sound = nullptr;
	if (isChunk)
		sound = new Chunk();
	else
		sound = new Music();

	if (!sound->Load(fileLoc, name))
	{
		delete sound;
		return nullptr;
	}

	soundList.push_back(sound);

	return soundList.back();
}

Sound* Sounds::CreateChunk(std::string fileLoc, std::string name)
{
	return CreateSound(fileLoc, name, true);
}

Sound* Sounds::CreateMusic(std::string fileLoc, std::string name)
{
	return CreateSound(fileLoc, name, false);
}

void Sounds::DeleteSound(std::string name)
{
	int i = 0;
	for (auto& tex : soundList)
		if (tex->Name() == name)
		{
			soundList.erase(soundList.begin() + i);
			break;
		}
		else
			i++;
}