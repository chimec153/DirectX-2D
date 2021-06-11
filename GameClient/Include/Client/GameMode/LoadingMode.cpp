//#include "pch.h"
#include "LoadingMode.h"
#include "../LoadingThread.h"
#include "Thread/ThreadManager.h"
#include "../UI/Br.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"

CLoadingMode::CLoadingMode()	:
	m_pBar(nullptr),
	m_pThread(nullptr)
{
}

CLoadingMode::~CLoadingMode()
{
	SAFE_RELEASE(m_pThread);
}

bool CLoadingMode::Init()
{
	if (!CGameMode::Init())
		return false;

	GET_SINGLE(CResourceManager)->LoadTexture("Back", TEXT("back.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("Bar", TEXT("bar.png"));

	m_pBar = m_pScene->CreateObject<CBr>("LoadingBar", m_pScene->FindLayer("UI"));

	m_pThread = GET_SINGLE(CThreadManager)->CreateThread<CLoadingThread>("LoadingThread");

	m_pThread->Start();

	return true;
}

void CLoadingMode::Update(float fTime)
{
	CGameMode::Update(fTime);

	if (m_pThread->IsDone())
	{
		CScene* pScene = m_pThread->GetScene();

		GET_SINGLE(CThreadManager)->DeleteThread("LoadingThread");

		GET_SINGLE(CSceneManager)->SetNextScene(pScene);
	}
}
