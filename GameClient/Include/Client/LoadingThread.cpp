//#include "pch.h"
#include "LoadingThread.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "GameMode/MainGameMode.h"

CLoadingThread::CLoadingThread()	:
	m_bIsDone(false)
	, m_pScene(nullptr)
	, m_iProgress(0)
{
}

CLoadingThread::~CLoadingThread()
{
}

void CLoadingThread::Run()
{
	m_pScene = GET_SINGLE(CSceneManager)->CreateScene();

	m_pScene->SetGameMode<CMainGameMode>();

	m_bIsDone = true;
}
