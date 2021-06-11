#include "NavigationManager.h"
#include "Component/Component.h"

DEFINITION_SINGLE(CNavigationManager)

CNavigationManager::CNavigationManager()
{

}

CNavigationManager::~CNavigationManager()
{

}

std::shared_ptr<class CComponent> CNavigationManager::FindNav(const std::string& strKey)
{
	std::unordered_map < std::string, std::shared_ptr<CComponent>>::iterator iter = m_mapNav.find(strKey);

	if (iter == m_mapNav.end())
		return nullptr;

	return iter->second;
}
