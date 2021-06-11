#include "LoadingThread3D.h"
#include "Scene/SceneManager.h"
#include "GameMode/MainMode.h"

CLoadingThread3D::CLoadingThread3D()	:
	m_bComplete(false)
	,m_pScene(nullptr)
	, m_pFilePath(nullptr)
{
}

CLoadingThread3D::~CLoadingThread3D()
{
}

void CLoadingThread3D::SetWork(void(*pFunc)())
{
	m_Work = std::bind(pFunc);
}

void CLoadingThread3D::Run()
{
	if (m_Work)
	{
		m_Work();
	}
	else
	{
		CMainMode::LoadSequence();
		CMainMode::LoadTexture();
	}

	m_bComplete = true;
}
