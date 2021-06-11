#include "CameraManager.h"
#include "Component/Camera.h"
#include "Object/GameObject.h"
#include <iostream>
#include "UI/imgui/imgui.h"

DEFINITION_SINGLE(CCameraManager)

CCameraManager::CCameraManager()	:
	m_tRS()
	, m_pMainCam(nullptr)
	, m_pUICam(nullptr)
{
}

CCameraManager::~CCameraManager()
{
	SAFE_RELEASE(m_pMainCam);
	SAFE_RELEASE(m_pUICam);
}

void CCameraManager::SetMainCam(class CCamera* pCam)
{
	m_pMainCam = pCam;
}
//인공지능 FSM
void CCameraManager::SetMainCam(const std::string& strTag)
{
	m_pMainCam = FindCam(strTag);
}

void CCameraManager::SetUICam(class CCamera* pCam)
{
	m_pUICam = pCam;
}

void CCameraManager::SetUICam(const std::string& strTag)
{
	m_pUICam = FindCam(strTag);
}

void CCameraManager::AddCam(class CCamera* pCam)
{
	if (!pCam)
		return;

	std::string strTag = pCam->GetName();

	class CCamera* pC = FindCam(strTag);

	if (pC)
	{
		return;
	}
	
	m_mapCam.insert(std::make_pair(strTag, pCam));
}

class CCamera* CCameraManager::GetMainCam() const
{
	return m_pMainCam;
}

class CCamera* CCameraManager::GetUICam() const
{
	return m_pUICam;
}

class CCamera* CCameraManager::FindCam(const std::string& strTag)
{
	std::unordered_map<std::string, class CCamera*>::iterator iter = m_mapCam.find(strTag);

	if (iter == m_mapCam.end())
		return nullptr;

	return iter->second;
}

void CCameraManager::EraseCam(const std::string& strTag)
{
	std::unordered_map<std::string, class CCamera*>::iterator iter = m_mapCam.find(strTag);

	if (iter == m_mapCam.end())
		return;

	m_mapCam.erase(iter);
}

void CCameraManager::EraseCam(CCamera* pCam)
{
	if (m_pMainCam == pCam)
	{
		m_pMainCam = nullptr;
	}

	if (m_pUICam == pCam)
	{
		m_pUICam = nullptr;
	}

	std::unordered_map<std::string, class CCamera*>::iterator iter = m_mapCam.begin();
	std::unordered_map<std::string, class CCamera*>::iterator iterEnd = m_mapCam.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->second == pCam)
		{
			m_mapCam.erase(iter);
			return;
		}
	}
}

const Resolution& CCameraManager::GetResolution() const
{
	return m_tRS;
}
bool CCameraManager::Init(const Resolution& tRS)
{
	m_tRS = tRS;

	//m_pMainCam = new CCamera;
	//m_pMainCam->SetName("MainCamera");

	//m_pMainCam->SetCameraType(CAMERA_TYPE::CT_2D);
	//AddCam(m_pMainCam);

	m_pUICam = new CCamera;
	m_pUICam->SetName("UICamera");

	m_pUICam->SetCameraType(CAMERA_TYPE::CT_UI);

	return true;
}

void CCameraManager::Update(float fTime)
{
	if (!m_pMainCam)
	{
		std::unordered_map<std::string, class CCamera*>::iterator iter = m_mapCam.begin();

		if (iter != m_mapCam.end())
		{
			SetMainCam(iter->second);
		}
	}

	//CGameObject* pObj = m_pMainCam->GetObj();

	//if (!pObj)
	if (m_pMainCam)
	{
		m_pMainCam->Update(fTime);
	}
}

void CCameraManager::PostUpdate(float fTime)
{
	if (!m_pMainCam)
	{
		std::unordered_map<std::string, class CCamera*>::iterator iter = m_mapCam.begin();

		if (iter != m_mapCam.end())
		{
			SetMainCam(iter->second);
		}
	}

	if (m_pMainCam)
	{
		CGameObject* pObj = m_pMainCam->GetObj();

		if (!pObj)
		{
			m_pMainCam->PostUpdate(fTime);
		}
	}
}

void CCameraManager::ShowWindow()
{
	if(ImGui::Begin("Camera Manager"))
	{
		if (m_pMainCam)
		{
			m_pMainCam->CCamera::ShowWindow();
		}

		size_t iSize = m_mapCam.size();

		std::unordered_map<std::string, class CCamera*>::iterator iter = m_mapCam.begin();
		std::unordered_map<std::string, class CCamera*>::iterator iterEnd = m_mapCam.end();
		std::vector<const char*> vecName(iSize);
		for (size_t i=0; iter != iterEnd; ++iter, i++)
		{
			vecName[i] = iter->first.c_str();
		}

		static int iSel = -1;

		if (iSize > 0)
		{
			ImGui::ListBox("Cameras", &iSel, &vecName[0], static_cast<int>(iSize));
		}

		if (iSel != -1 && iSel < iSize)
		{
			class CCamera* pCam = FindCam(vecName[iSel]);

			if (pCam)
			{
				pCam->ShowWindow();
			}
		}
	}
	ImGui::End();
}
