#include "TownLoadingThread.h"
#include "Editor/GameMode/Town.h"
#include "Editor/GameMode/MainMode.h"

CTownLoadingThread::CTownLoadingThread()	:
	m_eType(LOADING_TYPE::TEXTURE)
	, m_pFilePath(nullptr)
{
}

CTownLoadingThread::~CTownLoadingThread()
{
}

void CTownLoadingThread::Run()
{
	switch (m_eType)
	{
	case LOADING_TYPE::TEXTURE:
	{
		CTown::LoadTextures(m_pFilePath);
	}
		break;
	case LOADING_TYPE::SEQUENCE:
	{
		CMainMode::LoadSequence(m_pFilePath);
	}
		break;
	case LOADING_TYPE::MESH:
	{
		CTown::LoadMeshs(m_pFilePath);
	}
		break;
	}

	Complete();
}
