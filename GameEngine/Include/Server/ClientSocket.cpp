#include "ClientSocket.h"
#include "User.h"
#include "IOCP.h"

HANDLE CClientSocket::g_tComport;
CClientSocket* CClientSocket::m_pInst = nullptr;

CClientSocket::CClientSocket()	:
	m_iPort(0)
	, m_pIP(nullptr)
	, m_pUser(nullptr)
	, m_tClientSock(0)
{
}

CClientSocket::~CClientSocket()
{
	SAFE_RELEASE(m_pUser);
}

CUser* CClientSocket::GetUser() const
{
	if (m_pUser)
	{
		m_pUser->AddRef();
	}

	return m_pUser;
}

bool CClientSocket::Init(int iPort, const TCHAR* pIP)
{
	m_iPort = iPort;
	m_pIP = pIP;

	SYSTEM_INFO tInfo = {};

	GetSystemInfo(&tInfo);
	m_pUser = new CUser;

	if (!m_pUser->AsyncInit())
	{
		SAFE_RELEASE(m_pUser);
		return false;
	}

	m_pUser->m_tRecvAddr.sin_family = AF_INET;
	m_pUser->m_tRecvAddr.sin_port = htons(iPort);
	InetPton(AF_INET, pIP, &m_pUser->m_tRecvAddr.sin_addr.s_addr);

	if (WSAConnect(m_pUser->m_tUserSock, reinterpret_cast<sockaddr*>(&m_pUser->m_tRecvAddr),
		static_cast<int>(sizeof(sockaddr)), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();

		if (iError == WSA_IO_PENDING)
		{

		}
		else
		{
			CIOCP::ShowError(iError);
		}
	}
	else
	{
		if (m_pUser)
		{
			m_pUser->SetState(USER_STATE::CONNECT);
		}

		m_pUser->m_pRecvPart->tBuf.buf = m_pUser->m_pRecvPart->pPacket;
		m_pUser->m_pRecvPart->tBuf.len = MSG_SIZE;

		if (WSARecv(m_pUser->m_tUserSock, &m_pUser->m_pRecvPart->tBuf, 1,
			&m_pUser->m_pRecvPart->iRecvCount, &m_pUser->m_pRecvPart->iFlag,
			&m_pUser->m_pRecvPart->tOverlapped, NULL) == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();

			if (iError == WSA_IO_PENDING)
			{

			}
			else
			{
				CIOCP::ShowError(iError);
			}

		}
	}

	g_tComport = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	for (DWORD i = 0; i < tInfo.dwNumberOfProcessors * 2; ++i)
	{
		_beginthreadex(NULL, 0, &CClientSocket::ThreadFunc, this, 0, 0);
		
	}

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_pUser->GetRecvPart()->hSocket), 
		g_tComport, reinterpret_cast<ULONG_PTR>(m_pUser), 0);

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_pUser->GetSendPart()->hSocket), 
		g_tComport, reinterpret_cast<ULONG_PTR>(m_pUser), 0);
	return true;
}

void CClientSocket::SendPacket(const char* pMsg)
{
	POVERPART pSendPart = m_pUser->GetSendPart();

	memset(pSendPart->pPacket, 0, MSG_SIZE);
	memcpy_s(pSendPart->pPacket, MSG_SIZE, pMsg, MSG_SIZE);
	pSendPart->tBuf.buf = pSendPart->pPacket;
	pSendPart->tBuf.len = MSG_SIZE;

	if (WSASend(pSendPart->hSocket, &pSendPart->tBuf, 1, &pSendPart->iRecvCount,
		pSendPart->iFlag, &pSendPart->tOverlapped, NULL) == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();

		if (iError == WSA_IO_PENDING)
		{
			return;
		}

		CIOCP::ShowError(iError);
	}
}

void CClientSocket::SetPacketFunc(void(*pFunc)(const char*))
{
	m_PacketFunc = std::bind(pFunc, std::placeholders::_1);
}

unsigned __stdcall CClientSocket::ThreadFunc(void* pData)
{
	CClientSocket* pSocket = reinterpret_cast<CClientSocket*>(pData);

	while (true)
	{
		if (pSocket)
		{
			DWORD iRecv = 0;
			OVERLAPPED* pOverlapped = nullptr;
			CUser* pUser = nullptr;
			BOOL bResult = GetQueuedCompletionStatus(g_tComport, &iRecv, reinterpret_cast<PULONG_PTR>(&pUser), &pOverlapped, INFINITE);

			if (!bResult)
			{
				int iError = WSAGetLastError();
				CIOCP::ShowError(iError);
				continue;
			}

			OVERPART* pPart = reinterpret_cast<OVERPART*>(pOverlapped);

			if (pPart)
			{
				switch (pPart->eType)
				{
				case WORK_TYPE::RECV:
				{
					if (!iRecv)
					{
						return 0;
					}

					if (pSocket->m_PacketFunc)
					{
						pSocket->m_PacketFunc(pPart->tBuf.buf);
					}

					if (WSARecv(pPart->hSocket, &pPart->tBuf, 1, &pPart->iRecvCount, &pPart->iFlag, &pPart->tOverlapped, NULL) == SOCKET_ERROR)
					{
						int iError = WSAGetLastError();

						if (iError == WSA_IO_PENDING)
						{
							continue;
						}

						CIOCP::ShowError(iError);
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
	}

	return 0;
}
