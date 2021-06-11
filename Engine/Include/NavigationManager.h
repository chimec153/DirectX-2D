#pragma once

#include "GameEngine.h"
#include "Component/Component.h"

class CNavigationManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<class CComponent>>	m_mapNav;

public:
	template <typename T>
	std::shared_ptr<class CComponent> CreateNav(const std::string& strKey)
	{
		std::shared_ptr<CComponent> pCom = FindNav(strKey);

		if (pCom)
			return nullptr;

		pCom = std::shared_ptr<CComponent>(new T);

		if (!pCom->Init())
		{
			return false;
		}

		m_mapNav.insert(std::make_pair(strKey, pCom));

		return pCom;
	}

public:
	std::shared_ptr<class CComponent> FindNav(const std::string& strKey);

	DECLARE_SINGLE(CNavigationManager)
};

