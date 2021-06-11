#include "SoundManager.h"
#include "../PathManager.h"

DEFINITION_SINGLE(CSoundManager)

CSoundManager::CSoundManager()	:
	m_pSystem(nullptr)
{
}

CSoundManager::~CSoundManager()
{
	std::unordered_map<std::string, PSOUNDINFO>::iterator	iter;
	for (iter = m_mapSound.begin(); iter != m_mapSound.end(); ++iter)
	{
		iter->second->pSound->release();
		SAFE_DELETE(iter->second);
	}

	m_pSystem->close();
	m_pSystem->release();
}

bool CSoundManager::Init()
{
	System_Create(&m_pSystem);
	m_pSystem->init(1024, FMOD_INIT_NORMAL, NULL);

	m_pSystem->getMasterChannelGroup(&m_pMasterGroup);

	CreateChannelGroup("UI");
	CreateChannelGroup("BGM");
	CreateChannelGroup("Effect");

	return true;
}

void CSoundManager::Update(float fTime)
{
	m_pSystem->update();
}

bool CSoundManager::LoadSound(const std::string& strGroup, 
	const std::string& strKey, bool bLoop, const char* pFileName, 
	const std::string& strPathKey)
{
	if (FindSound(strKey))	//	매개변수 strKey로 사운드 정보 구조체를 찾는다.
		return true;	//	사운드 정보 구조체가 있을 경우 true를 반환한다.

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strPathKey);	//	경로 관리자에서 멀티바이트 경로를 찾는다. 매개변수로 strPathKey를 전달한다.

	std::string strFullPath;	//	문자열을 선언한다.
	if (pPath)	//	경로 문자열 포인터가 nullptr이 아닐 경우다.
		strFullPath = pPath;	//	멀티 바이트 경로로 문자열을 설정한다.

	strFullPath += pFileName;	//	문자열에 매개변수 pFileName을 더해서 문자열에 저장한다.

	FMOD_MODE	eMode = FMOD_LOOP_NORMAL;	//	FMOD의 모드형 변수를 선언하고 FMOD_LOOP_NORMAL로 설정한다.
	if (!bLoop)	//	매개변수 bLoop가 false일 경우다.
		eMode = FMOD_3D;	//	FMOD 모드를 FMOD_DEFAULT로 설정한다.

	PSOUNDINFO	pSoundInfo = new SOUNDINFO;	// 사운드 정보 구조체를 생성한다.

	pSoundInfo->bLoop = bLoop;	//	사운드 정보 구조체의 루프를 매개변수 bLoop로 설정한다.

	pSoundInfo->pGroup = FindChannelGroup(strGroup);	

	FMOD_RESULT hr = m_pSystem->createSound(strFullPath.c_str(), eMode, NULL, &pSoundInfo->pSound);	//	시스템의 소리를 생성하는 함수를 호출한다. 매개변수로 문자열,FMOD형 변수, NUUL, 사운드 정보의 사운드 클래스 포인터를 전달한다.                                                                                                                                                                                                    

	m_mapSound.insert(std::make_pair(strKey, pSoundInfo));

	return true;
}

bool CSoundManager::Play(const std::string& strKey)
{
	PSOUNDINFO pSound = FindSound(strKey);	

	if (!pSound)							
		return false;		

	FMOD_RESULT tResult = m_pSystem->playSound(pSound->pSound, pSound->pGroup, false, &pSound->pChannel);
	pSound->pChannel->setVolume(pSound->fVol);
	return true;
}

void CSoundManager::Stop(const std::string& strKey)
{
	PSOUNDINFO pSound = FindSound(strKey);

	if (!pSound)
		return;

	bool bPlaying = false;

	pSound->pChannel->isPlaying(&bPlaying);

	if (bPlaying)
	{
		pSound->pChannel->stop();
	}
}

void CSoundManager::Vol(const std::string& strKey, float fVol)
{
	PSOUNDINFO pInfo = FindSound(strKey);

	if (!pInfo)
		return;

	if (fVol > 1.f)
		fVol = 1.f;

	else if (fVol < 0.f)
		fVol = 0.f;

	pInfo->fVol = fVol;
	
	m_pSystem->playSound(pInfo->pSound, pInfo->pGroup, true, &pInfo->pChannel);
	pInfo->pChannel->setVolume(pInfo->fVol);
	pInfo->pChannel->setPaused(false);
}

PSOUNDINFO CSoundManager::FindSound(const std::string& strKey)
{
	std::unordered_map<std::string, PSOUNDINFO>::iterator iter = m_mapSound.find(strKey);
	if (iter == m_mapSound.end())
		return NULL;	

	return iter->second;
}

bool CSoundManager::CreateChannelGroup(const std::string& strKey)
{
	FMOD::ChannelGroup* pGroup = FindChannelGroup(strKey);

	if (pGroup)
		return false;

	FMOD_RESULT tResult =m_pSystem->createChannelGroup(strKey.c_str(), &pGroup);

	if (tResult == FMOD_OK)
	{
		m_pMasterGroup->addGroup(pGroup,false, 0);

		m_mapChannelGroup.insert(std::make_pair(strKey, pGroup));
		return true;
	}

	return false;
}

FMOD::ChannelGroup* CSoundManager::FindChannelGroup(const std::string& strKey)
{
	std::unordered_map<std::string, FMOD::ChannelGroup*>::iterator iter = m_mapChannelGroup.find(strKey);

	if (iter == m_mapChannelGroup.end())
		return nullptr;

	return iter->second;
}
