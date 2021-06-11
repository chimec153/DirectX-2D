#include "Updater.h"

HANDLE CUpdater::m_tAccept;
HANDLE* CUpdater::m_pThreadPool = nullptr;

CUpdater::CUpdater(CServer& core)	:
	m_eType(UPDATER_TYPE::THREAD)
	, m_iPort(0)
	, m_pIp(nullptr)
	, m_tServSock(0)
	, m_tCore(core)
{
}

CUpdater::~CUpdater()
{
	CloseHandle(m_tAccept);
	WaitForMultipleObjects(8, m_pThreadPool, true, INFINITE);

	SAFE_DELETE_ARRAY(m_pThreadPool)
}

void CUpdater::WaitToConnect(CUser* pUser)
{
}

bool CUpdater::Init(UPDATER_TYPE eType, int iPort, const TCHAR* pIp)
{
	m_eType = eType;
	m_iPort = iPort;
	m_pIp = pIp;

	return true;
}
