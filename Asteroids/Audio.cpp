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
	this->name = "";
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

void Sound::SetVolume(int newVol)
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
	this->name = "";
	if (this->sound != nullptr)
	{
		Mix_FreeMusic(this->sound);
		this->sound = nullptr;
	}
}

bool Music::Load(std::string fileLoc, std::string name)
{
	this->sound = Mix_LoadMUS(fileLoc.c_str());
	if (this->sound != nullptr)
	{
		this->name = name;
		return true;
	}

	Clear();

	return false;
}

void Music::Play()
{
	if (Mix_PlayingMusic() == 0)
		Mix_PlayMusic(this->sound, -1);
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

void Music::SetVolume(int newVol)
{
	Mix_VolumeMusic(newVol);
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
}

void Chunk::Clear()
{
	this->name = "";
	this->channel = -1;
	this->allowOverlayingSound = true;
	if (this->sound != nullptr)
	{
		Mix_FreeChunk(this->sound);
		this->sound = nullptr;
	}

	Mix_ChannelFinished(OnChannelFinish);
}

bool Chunk::Load(std::string fileLoc, std::string name)
{
	sound = Mix_LoadWAV(fileLoc.c_str());
	if (sound != nullptr)
	{
		this->name = name;
		return true;
	}

	Clear();

	return false;
}

void Chunk::Play()
{
	if (!this->allowOverlayingSound)
		if (this->channel == -1)
			return;

	this->channel = Mix_PlayChannel(-1, this->sound, 0);
	if (this->channel >= 0 && this->channel < CHANNEL_LIMIT)
		channelList[this->channel] = this;
}

void Chunk::Pause()
{
	if (this->channel >= 0)
		if (channelList[this->channel] != nullptr)
			Mix_Pause(this->channel);
}

void Chunk::Unpause()
{
	if (this->channel >= 0)
		if (channelList[this->channel] != nullptr)
			Mix_Resume(this->channel);
}

void Chunk::Stop()
{
	if (this->channel >= 0)
		if (channelList[this->channel] != nullptr)
			Mix_HaltChannel(this->channel);
}

void Chunk::SetVolume(int newVol)
{
	Mix_VolumeChunk(sound, newVol);
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