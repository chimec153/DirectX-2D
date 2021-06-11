#include "CollisionManager.h"

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	SAFE_DELETE_MAP(m_mapProfile);
	SAFE_DELETE_VECLIST(m_vecChannel);
}

bool CCollisionManager::Init()
{
	CreateChannel("Base", COLLISION_CHANNEL_TYPE::CCT_COL);
	CreateChannel("Player", COLLISION_CHANNEL_TYPE::CCT_COL);
	CreateChannel("Monster", COLLISION_CHANNEL_TYPE::CCT_COL);
	CreateChannel("NPC", COLLISION_CHANNEL_TYPE::CCT_COL);
	CreateChannel("Custom1", COLLISION_CHANNEL_TYPE::CCT_COL);

	CreateProfile("Base", COLLISION_CHANNEL::CC_BASE, COLLISION_CHANNEL_TYPE::CCT_COL);
	CreateProfile("Player", COLLISION_CHANNEL::CC_PLAYER, COLLISION_CHANNEL_TYPE::CCT_COL);
	CreateProfile("Monster", COLLISION_CHANNEL::CC_MONSTER, COLLISION_CHANNEL_TYPE::CCT_COL);
	CreateProfile("NPC", COLLISION_CHANNEL::CC_NPC, COLLISION_CHANNEL_TYPE::CCT_COL);
	CreateProfile("Custom1", COLLISION_CHANNEL::CC_CUSTOM1, COLLISION_CHANNEL_TYPE::CCT_COL);

	//SetProfileChannelType("Player", COLLISION_CHANNEL::CC_MONSTER, COLLISION_CHANNEL_TYPE::CCT_IGN);
	SetProfileChannelType("Player", COLLISION_CHANNEL::CC_PLAYER, COLLISION_CHANNEL_TYPE::CCT_IGN);
	SetProfileChannelType("Player", COLLISION_CHANNEL::CC_CUSTOM1, COLLISION_CHANNEL_TYPE::CCT_IGN);
	SetProfileChannelType("Custom1", COLLISION_CHANNEL::CC_CUSTOM1, COLLISION_CHANNEL_TYPE::CCT_IGN);
	SetProfileChannelType("Custom1", COLLISION_CHANNEL::CC_PLAYER, COLLISION_CHANNEL_TYPE::CCT_IGN);

	return true;
}

bool CCollisionManager::CreateChannel(const std::string& strTag, COLLISION_CHANNEL_TYPE eType)
{
	PCollisionChannel pChannel = new CollisionChannel;

	pChannel->strTag = strTag;
	pChannel->eType = eType;
	pChannel->eChannel = COLLISION_CHANNEL(m_vecChannel.size());

	m_vecChannel.push_back(pChannel);

	std::unordered_map<std::string, PCollisionProfile>::iterator iter = m_mapProfile.begin();
	std::unordered_map<std::string, PCollisionProfile>::iterator iterEnd = m_mapProfile.end();

	for (; iter != iterEnd; ++iter)
		iter->second->vecChannel.push_back(*pChannel);

	return true;
}

bool CCollisionManager::CreateProfile(const std::string& strTag, COLLISION_CHANNEL eChannel, COLLISION_CHANNEL_TYPE eType)
{
	PCollisionProfile pProfile = FindProfile(strTag);

	if (pProfile)
		return false;

	pProfile = new CollisionProfile;

	pProfile->eChannel = eChannel;
	pProfile->strTag = strTag;

	size_t iSize = m_vecChannel.size();

	for (int i = 0; i < iSize; ++i)
		pProfile->vecChannel.push_back(*m_vecChannel[i]);

	pProfile->vecChannel[(int)eChannel].eType = eType;

	m_mapProfile.insert(std::make_pair(strTag, pProfile));

	return true;
}

void CCollisionManager::SetProfileChannelType(const std::string& strTag, COLLISION_CHANNEL eChannel, COLLISION_CHANNEL_TYPE eType)
{
	PCollisionProfile pProfile = FindProfile(strTag);

	if (!pProfile)
		return;

	pProfile->vecChannel[(int)eChannel].eType = eType;	
}

PCollisionProfile CCollisionManager::FindProfile(const std::string& strTag)
{
	std::unordered_map<std::string, PCollisionProfile>::iterator iter = m_mapProfile.find(strTag);

	if (iter == m_mapProfile.end())
		return nullptr;

	return iter->second;
}
