#include "LightManager.h"
#include "Component/Light.h"
#include "UI/imgui/imgui.h"

std::unordered_map<std::string, class CLight*> CLightManager::m_mapLight = {};
CLight* CLightManager::m_pMainLight = nullptr;

DEFINITION_SINGLE(CLightManager)

CLightManager::CLightManager()
{

}

CLightManager::~CLightManager()
{

}

void CLightManager::SetShader()
{
	std::unordered_map<std::string, class CLight*>::iterator iter = m_mapLight.begin();
	std::unordered_map<std::string, class CLight*>::iterator iterEnd = m_mapLight.end();

	for (; iter != iterEnd;++iter)
	{
		iter->second->SetShader();
	}
}

bool CLightManager::Init()
{
	return true;
}

void CLightManager::AddLight(class CLight* pLight)
{
	if (pLight)
	{
		m_mapLight.insert(std::make_pair(pLight->GetName(), pLight));

		if (!m_pMainLight)
			m_pMainLight = pLight;
	}
}

void CLightManager::DeleteLight(class CLight* light)
{
	if (m_mapLight.empty())
		return;

	std::unordered_map<std::string, class CLight*>::const_iterator iter = m_mapLight.find(light->GetName());

	if (iter == m_mapLight.end())
		return;

	if (iter->second == m_pMainLight)
	{
		iter = m_mapLight.erase(iter);

		if (m_mapLight.empty())
		{
			m_pMainLight = nullptr;
		}
		else
		{
			m_pMainLight = iter->second;
		}

		return;
	}

	m_mapLight.erase(iter);
}

class CLight* CLightManager::FindLight(const std::string& strKey) const
{
	std::unordered_map<std::string, class CLight*>::const_iterator iter = m_mapLight.find(strKey);

	if (iter == m_mapLight.end())
		return nullptr;

	return iter->second;
}

CLight* CLightManager::GetMainLight() const
{
	return m_pMainLight;
}

void CLightManager::SetMainLight(CLight* pLight)
{
	m_pMainLight = pLight;
}

void CLightManager::SetMainLight(const std::string& strKey)
{
	CLight* pLight = FindLight(strKey);

	if (!pLight)
		return;

	m_pMainLight = pLight;
}

void CLightManager::Render(float fTime)
{
	std::unordered_map<std::string, CLight*>::iterator iter = m_mapLight.begin();
	std::unordered_map<std::string, CLight*>::iterator iterEnd = m_mapLight.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->Render(fTime);
	}
}

void CLightManager::ShowWindow()
{
	if (ImGui::Begin("LightManager"))
	{
		std::vector<const char*> vecLight;

		std::unordered_map<std::string, CLight*>::iterator iter = m_mapLight.begin();
		std::unordered_map<std::string, CLight*>::iterator iterEnd = m_mapLight.end();

		for (; iter != iterEnd; ++iter)
		{
			vecLight.push_back(iter->first.c_str());
		}

		static int iSel = -1;
		if (!vecLight.empty())
		{
			ImGui::ListBox("Lights", &iSel, &vecLight[0], static_cast<int>(vecLight.size()));
		}

		if (iSel != -1 && iSel < vecLight.size())
		{
			class CLight* pLight = FindLight(vecLight[iSel]);
			if (pLight)
			{
				pLight->ShowWindow();
			}
		}
	}
	ImGui::End();

}
