#include "ServerThread.h"
#include "../Server/Updater.h"

CServerThread::CServerThread()	:
	m_pUpdater(nullptr)
{
}

CServerThread::~CServerThread()
{
}

void CServerThread::SetUpdater(CUpdater* pUpdater)
{
	m_pUpdater = pUpdater;
}

void CServerThread::Run()
{
	if (m_pUpdater)
	{
		m_pUpdater->Update();
	}
}
