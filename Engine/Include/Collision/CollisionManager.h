#pragma once

#include "../GameEngine.h"

class CCollisionManager
{
private:
	std::vector<PCollisionChannel>						m_vecChannel;
	std::unordered_map<std::string, PCollisionProfile>	m_mapProfile;

public:
	bool Init();

public:
	bool CreateChannel(const std::string& strTag, COLLISION_CHANNEL_TYPE eType);
	bool CreateProfile(const std::string& strTag, COLLISION_CHANNEL eChannel, COLLISION_CHANNEL_TYPE eType);
	void SetProfileChannelType(const std::string& strTag, COLLISION_CHANNEL eChannel, COLLISION_CHANNEL_TYPE eType);

public:
	PCollisionProfile FindProfile(const std::string& strTag);

	DECLARE_SINGLE(CCollisionManager)
};