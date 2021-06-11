#pragma once

#include "../GameEngine.h"
#include "../fmod.hpp"

typedef struct _tagSoundInfo
{
	FMOD::Sound*	pSound;
	bool			bLoop;
	float			fVol;
	FMOD::Channel*	pChannel;
	FMOD::ChannelGroup* pGroup;

	_tagSoundInfo() :
		pSound(nullptr),
		bLoop(false),
		fVol(1.f),
		pChannel(nullptr),
		pGroup(nullptr)
	{

	}
}SOUNDINFO, *PSOUNDINFO;

class CSoundManager
{
private:
	FMOD::System*									m_pSystem;
	std::unordered_map<std::string, PSOUNDINFO>		m_mapSound;
	FMOD::ChannelGroup*								m_pMasterGroup;
	std::unordered_map<std::string, FMOD::ChannelGroup*>	m_mapChannelGroup;

public:
	bool Init();
	void Update(float fTime);

	bool LoadSound(const std::string& strGroup, const std::string& strKey, bool bLoop, const char* pFileName, 
		const std::string& strPathKey = SOUND_PATH);
	bool Play(const std::string& strKey);
	void Stop(const std::string& strKey);
	void Vol(const std::string& strKey, float fVol);
	PSOUNDINFO FindSound(const std::string& strKey);
	bool CreateChannelGroup(const std::string& strKey);
	FMOD::ChannelGroup* FindChannelGroup(const std::string& strKey);
	DECLARE_SINGLE(CSoundManager)
};

