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
	if (FindSound(strKey))	//	�Ű����� strKey�� ���� ���� ����ü�� ã�´�.
		return true;	//	���� ���� ����ü�� ���� ��� true�� ��ȯ�Ѵ�.

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strPathKey);	//	��� �����ڿ��� ��Ƽ����Ʈ ��θ� ã�´�. �Ű������� strPathKey�� �����Ѵ�.

	std::string strFullPath;	//	���ڿ��� �����Ѵ�.
	if (pPath)	//	��� ���ڿ� �����Ͱ� nullptr�� �ƴ� ����.
		strFullPath = pPath;	//	��Ƽ ����Ʈ ��η� ���ڿ��� �����Ѵ�.

	strFullPath += pFileName;	//	���ڿ��� �Ű����� pFileName�� ���ؼ� ���ڿ��� �����Ѵ�.

	FMOD_MODE	eMode = FMOD_LOOP_NORMAL;	//	FMOD�� ����� ������ �����ϰ� FMOD_LOOP_NORMAL�� �����Ѵ�.
	if (!bLoop)	//	�Ű����� bLoop�� false�� ����.
		eMode = FMOD_3D;	//	FMOD ��带 FMOD_DEFAULT�� �����Ѵ�.

	PSOUNDINFO	pSoundInfo = new SOUNDINFO;	// ���� ���� ����ü�� �����Ѵ�.

	pSoundInfo->bLoop = bLoop;	//	���� ���� ����ü�� ������ �Ű����� bLoop�� �����Ѵ�.

	pSoundInfo->pGroup = FindChannelGroup(strGroup);	

	FMOD_RESULT hr = m_pSystem->createSound(strFullPath.c_str(), eMode, NULL, &pSoundInfo->pSound);	//	�ý����� �Ҹ��� �����ϴ� �Լ��� ȣ���Ѵ�. �Ű������� ���ڿ�,FMOD�� ����, NUUL, ���� ������ ���� Ŭ���� �����͸� �����Ѵ�.                                                                                                                                                                                                    

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
