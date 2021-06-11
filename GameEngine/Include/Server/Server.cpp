#include "Server.h"
#include "Updater.h"
#include "IOCP.h"
#include "User.h"
#include "DataBase.h"

CServer::CServer()	:
	m_pUpdater(nullptr)
	, m_eType(UPDATER_TYPE::THREAD)
	, m_bRun(false)
	, m_iPort(0)
	,m_pIP(nullptr)
	, m_tServSock(0)
{
}

CServer::~CServer()
{
	m_bRun = false;
	SAFE_RELEASE(m_pUpdater);
	if (m_tServSock)
	{
		closesocket(m_tServSock);
		m_tServSock = 0;
	}

	std::unordered_map<CUser*, SOCKET>::iterator iter = m_mapWait.begin();
	std::unordered_map<CUser*, SOCKET>::iterator iterEnd = m_mapWait.end();

	for (; iter != iterEnd;++iter)
	{
		if (iter->first)
		{
			iter->first->Release();
		}
	}
	m_mapWait.clear();

	iter = m_mapConnect.begin();
	iterEnd = m_mapConnect.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->first)
		{
			iter->first->Release();
		}
	}
	m_mapConnect.clear();
}

bool CServer::IsRunning() const
{
	return m_bRun;
}


SOCKET CServer::AddUser(SOCKET tSock, int iListen)
{
	CUser* pUser = new CUser;
	sockaddr tSockAddr = {};
	int iSize = static_cast<int>(sizeof(sockaddr));
	SOCKET tClientSock = WSAAccept(tSock, &tSockAddr, &iSize, NULL, 0);;
	pUser->Init(tClientSock, m_iPort);

	return tClientSock;
}

SOCKET CServer::GetAsyncSocket(int iListen)
{
	m_tServSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(m_iPort);
	tAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(m_tServSock, reinterpret_cast<sockaddr*>(&tAddr), sizeof(sockaddr)) == SOCKET_ERROR)
	{
		CIOCP::ShowError(GetLastError());
		return 0;
	}

	if (listen(m_tServSock, iListen) == SOCKET_ERROR)
	{
		CIOCP::ShowError(GetLastError());
		return 0;
	}

	return m_tServSock;
}

CUser* CServer::CreateAsyncUser(HANDLE hComport)
{
	CUser* pUser = new CUser;

	SOCKET tSock = pUser->AsyncInit();

	POVERPART pRecvPart = pUser->GetRecvPart();
	pRecvPart->eType = WORK_TYPE::CONNECT;

	u_long mode = 1;
	if (ioctlsocket(m_tServSock, FIONBIO, &mode) == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
	}

	pRecvPart->tBuf.buf = pRecvPart->pPacket;
	pRecvPart->tBuf.len = MSG_SIZE;

	BOOL R = AcceptEx(m_tServSock, pUser->m_tUserSock, pRecvPart->pPacket, 0, sizeof(sockaddr) + 16, sizeof(sockaddr) + 16, &pRecvPart->iRecvCount, &pRecvPart->tOverlapped);
	
	if (!R)
	{
		DWORD iError = WSAGetLastError();

		if (iError == WSA_IO_PENDING)
		{
		}
		else
		{
			void* pData = nullptr;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, iError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pData), 0, NULL);

			std::wstring strError = static_cast<LPWSTR>(pData);

			LocalFree(pData);

			SAFE_RELEASE(pUser);
			assert(false);
			return nullptr;
		}
	}

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_tServSock), hComport, reinterpret_cast<ULONG_PTR>(pUser), 0);
	
	m_mapWait.insert(std::make_pair(pUser, tSock));

	return pUser;
}

void CServer::WaitToConnect(CUser* pUser)
{
	std::unordered_map<CUser*, SOCKET>::iterator iter = m_mapWait.find(pUser);

	if (iter == m_mapWait.end())
		return;

	m_mapConnect.insert(std::make_pair(iter->first, iter->second));

	m_mapWait.erase(iter);
}

void CServer::SetSocket(SOCKET hSocket)
{
	m_tServSock = hSocket;
}

void CServer::SendOtherUser(CUser* pUser, const char* pMsg)
{
	std::unordered_map<CUser*, SOCKET>::iterator iter = m_mapConnect.begin();
	std::unordered_map<CUser*, SOCKET>::iterator iterEnd = m_mapConnect.end();

	for (; iter != iterEnd; ++iter)
	{
		memset(iter->first->m_pSendPart->pPacket, 0, MSG_SIZE);
		memcpy_s(iter->first->m_pSendPart->pPacket, MSG_SIZE, pMsg,MSG_SIZE);

		iter->first->m_pSendPart->tBuf.buf = iter->first->m_pSendPart->pPacket;
		iter->first->m_pSendPart->tBuf.len = MSG_SIZE;

		if (WSASend(iter->second, &iter->first->m_pSendPart->tBuf, 1, &iter->first->m_pSendPart->iRecvCount,
			iter->first->m_pSendPart->iFlag, &iter->first->m_pSendPart->tOverlapped, NULL) == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();

			if (iError == WSA_IO_PENDING)
			{
				continue;
			}

			CIOCP::ShowError(iError);
		}
	}
}

void CServer::DeleteUser(CUser* pUser)
{
	std::unordered_map<CUser*, SOCKET>::iterator iter = m_mapConnect.find(pUser);

	if (iter == m_mapConnect.end())
	{
		iter = m_mapWait.find(pUser);

		if (iter == m_mapWait.end())
		{
			return;
		}
	}

	SAFE_RELEASE(pUser);

	m_mapConnect.erase(iter);
}
