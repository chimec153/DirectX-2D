#pragma once

#include "../GameEngine.h"
#include "Updater.h"

class CServer
{
public:
	CServer();
	~CServer();

private:
	class CUpdater* m_pUpdater;
	UPDATER_TYPE	m_eType;
	bool			m_bRun;
	std::unordered_map<class CUser*, SOCKET>	m_mapWait;
	std::unordered_map<class CUser*, SOCKET>	m_mapConnect;
	int			m_iPort;
	const TCHAR* m_pIP;
	SOCKET		m_tServSock;

public:
	bool IsRunning()	const;

public:
	template <typename T>
	bool Init(UPDATER_TYPE eType, int iPort, const TCHAR* pIp = L"")
	{
		m_bRun = true;
		m_eType = eType;
		m_iPort = iPort;
		m_pIP= pIp;

		m_pUpdater = new T(*this);

		if (!m_pUpdater->Init(eType, iPort, pIp))
		{
			SAFE_RELEASE(m_pUpdater);
			return false;
		}


		return true;
	}

public:
	SOCKET AddUser(SOCKET tSock, int iListen);
	SOCKET GetAsyncSocket(int iListen);
	CUser* CreateAsyncUser(HANDLE hComport);
	void WaitToConnect(class CUser* pUser);
	void SetSocket(SOCKET hSocket);
	void SendOtherUser(class CUser* pUser, const char* pMsg);
	void DeleteUser(class CUser* pUser);
};

