#include "IOCP.h"
#include "Server.h"
#include "../Thread/ThreadManager.h"
#include "../Thread/ServerThread.h"
#include "User.h"

CIOCP::CIOCP(CServer& core)	:
	CUpdater(core)
{
}

CIOCP::~CIOCP()
{
	closesocket(m_tServSock);

	
}

bool CIOCP::Init(UPDATER_TYPE eType, int iPort, const TCHAR* pIp)
{
	if(!CUpdater::Init(eType, iPort, pIp))
		return false;

	CServerThread* pThread = GET_SINGLE(CThreadManager)->CreateThread<CServerThread>("ServerThread");

	pThread->SetUpdater(this);
	pThread->Start();

	SAFE_RELEASE(pThread);

	return true;
}

void CIOCP::Update()
{
	m_tServSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

	if (m_tServSock == INVALID_SOCKET)
	{
		DWORD iError = GetLastError();
		void* pData = nullptr;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, iError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&pData, 0, NULL);
		std::wstring strError = static_cast<wchar_t*>(pData);
		LocalFree(pData);

		return;
	}

	SOCKADDR_IN tAddr = {};

	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(m_iPort);
	tAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(m_tServSock, reinterpret_cast<sockaddr*>(&tAddr), sizeof(sockaddr)) == SOCKET_ERROR)
	{
		return;
	}

	if (listen(m_tServSock, 0) == SOCKET_ERROR)
	{
		return;
	}

	m_tAccept = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_tServSock), m_tAccept, 0, 10);
	SYSTEM_INFO tSystemInfo = {};

	GetSystemInfo(&tSystemInfo);

	m_pThreadPool = new HANDLE[tSystemInfo.dwNumberOfProcessors * 2];
	memset(m_pThreadPool, 0, sizeof(HANDLE) * tSystemInfo.dwNumberOfProcessors * 2);

	for (DWORD i = 0; i < tSystemInfo.dwNumberOfProcessors * 2; ++i)
	{
		m_pThreadPool[i] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &CIOCP::ThreadFunc, this, 0, NULL));
	}


	m_tCore.SetSocket(m_tServSock);

	for (int i = 0; i < 10; ++i)
	{
		CUser* pUser = m_tCore.CreateAsyncUser(m_tAccept);
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(pUser->GetUserSock()), m_tAccept, reinterpret_cast<ULONG_PTR>( pUser), 0);
	}

	
}

void CIOCP::RecvPacket(POVERPART pPart)
{
	m_tCore.SendOtherUser(pPart->pUser, pPart->tBuf.buf);
}

void CIOCP::SendPacket(CUser* pUser, const char* pPacket)
{
	POVERPART pSendPart = pUser->GetSendPart();

	pSendPart->eType = WORK_TYPE::SEND;
	memcpy_s(pSendPart->pPacket, MSG_SIZE, pPacket, MSG_SIZE);
	pSendPart->tBuf.buf = pSendPart->pPacket;
	pSendPart->tBuf.len = MSG_SIZE;

	if (WSASend(pSendPart->hSocket, &pSendPart->tBuf,1, &pSendPart->iRecvCount,
		pSendPart->iFlag, &pSendPart->tOverlapped, NULL) == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();

		if (iError == WSA_IO_PENDING)
		{
			return;
		}
		else
		{
			ShowError(iError);
		}
	}
}

void CIOCP::ShowError(int iError)
{
	void* pData = nullptr;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, iError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pData), 0, 0);

	std::wstring strError = static_cast<LPWSTR>(pData);

	LocalFree(pData);
}

unsigned __stdcall CIOCP::ThreadFunc(void* pData)
{
	CIOCP* pIOCP = static_cast<CIOCP*>(pData);

	while (pIOCP->m_tCore.IsRunning())
	{
		DWORD dwCount = 0;
		OVERLAPPED* pOverlapped = nullptr;
		CUser* pUser = nullptr;
		bool bResult = GetQueuedCompletionStatus(m_tAccept, &dwCount, reinterpret_cast<PULONG_PTR>(&pUser), &pOverlapped, INFINITE);

		if (!bResult)
		{
			int iError = WSAGetLastError();
			ShowError(iError);
			continue;
		}

		POVERPART pPart = reinterpret_cast<POVERPART>(pOverlapped);

		if (pPart)
		{
			switch (pPart->eType)
			{
			case WORK_TYPE::CONNECT:
			{
				CUser* pUser = pPart->pUser;

				pIOCP->m_tCore.WaitToConnect(pUser);
				pUser->SetState(USER_STATE::CONNECT);

				pPart->eType = WORK_TYPE::RECV;

				if (WSARecv(pPart->hSocket, &pPart->tBuf, 1, 
					&pPart->iRecvCount, &pPart->iFlag, &pPart->tOverlapped, NULL) == SOCKET_ERROR)
				{
					DWORD iError = GetLastError();

					if (iError == WSA_IO_PENDING)
					{
						continue;
					}
					else
					{
						void* pError = nullptr;
						FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
							iError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pError), 0, NULL);

						std::wstring strError = static_cast<LPWSTR>(pError);

						LocalFree(pError);
					}
				}

			}
				break;
			case WORK_TYPE::RECV:
			{
				if (dwCount == 0)
				{
					pIOCP->m_tCore.DeleteUser(pUser);
					continue;
				}
				pIOCP->RecvPacket(pPart);

				if (WSARecv(pPart->hSocket, &pPart->tBuf, 1, &pPart->iRecvCount, &pPart->iFlag, &pPart->tOverlapped, NULL) == SOCKET_ERROR)
				{
					int iError = WSAGetLastError();

					if (iError == WSA_IO_PENDING)
					{
						continue;
					}
				}
			}
				break;
			case WORK_TYPE::SEND:
			{

			}
				break;
			}
		}
	}

	return 0;
}
