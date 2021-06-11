#include "User.h"

CUser::CUser()	:
	m_tUserSock(0)
	, m_tSendAddr()
	, m_tRecvAddr()
	, m_pSendPart(nullptr)
	, m_pRecvPart(nullptr)
	, m_eState(USER_STATE::WAIT)
{
}

CUser::~CUser()
{
	SAFE_DELETE_ARRAY(m_pRecvPart->pPacket);
	SAFE_DELETE_ARRAY(m_pSendPart->pPacket);
	if (m_tUserSock)
	{
		closesocket(m_tUserSock);
	}

	SAFE_DELETE(m_pSendPart);
	SAFE_DELETE(m_pRecvPart);
}

POVERPART CUser::GetSendPart() const
{
	return m_pSendPart;
}

POVERPART CUser::GetRecvPart() const
{
	return m_pRecvPart;
}

SOCKET CUser::GetUserSock() const
{
	return m_tUserSock;
}

void CUser::SetState(USER_STATE eState)
{
	m_eState = eState;
}

USER_STATE CUser::GetState() const
{
	return m_eState;
}

bool CUser::Init(SOCKET tSock, int iPort, const TCHAR* pIP)
{
	m_tUserSock = tSock;

	m_pSendPart = new OVERPART;

	m_pSendPart->hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

	if (m_pSendPart->hSocket == INVALID_SOCKET)
	{
		DWORD iError =  GetLastError();

		void* pData = nullptr;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, iError, 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pData), 0, NULL);
		std::wstring strError = static_cast<TCHAR*>(pData);
		LocalFree(pData);
		SAFE_DELETE(m_pSendPart);
		return false;
	}

	m_pSendPart->eType = WORK_TYPE::SEND;
	m_pSendPart->pUser = this;

	m_pSendPart->tAddr.sin_port = htons(iPort);
	m_pSendPart->tAddr.sin_family = AF_INET;

	InetPton(m_pSendPart->tAddr.sin_family, pIP, &m_pSendPart->tAddr.sin_addr.s_addr);

	m_pSendPart->pPacket = new char[MSG_SIZE];
	memset(m_pSendPart->pPacket, 0, MSG_SIZE);
	m_pSendPart->tBuf.buf = m_pSendPart->pPacket;
	m_pSendPart->tBuf.len = MSG_SIZE;
	m_pSendPart->iCount = 0;
	m_pSendPart->iRecvCount = 0;
	m_pSendPart->iFlag = 0;
	memset(&m_pSendPart->tOverlapped, 0, sizeof(OVERLAPPED));

	if (WSAConnect(m_pSendPart->hSocket, reinterpret_cast<sockaddr*>(&m_pSendPart->tAddr), 
		sizeof(sockaddr), &m_pSendPart->tBuf, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		SAFE_DELETE(m_pSendPart);
		return false;
	}

	m_pRecvPart = new OVERPART;

	m_pRecvPart->eType = WORK_TYPE::RECV;
	m_pRecvPart->hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

	if (m_pRecvPart->hSocket == INVALID_SOCKET)
	{
		int iError = WSAGetLastError();

		void* pData = nullptr;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, iError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pData), 0, NULL);

		std::wstring strError = static_cast<LPWSTR>(pData);
		LocalFree(pData);
		SAFE_DELETE(m_pSendPart);
		SAFE_DELETE(m_pRecvPart);
		return false;
	}
	m_pRecvPart->pUser = this;
	m_pRecvPart->tAddr.sin_port = htons(iPort);
	m_pRecvPart->tAddr.sin_family = AF_INET;
	m_pRecvPart->pPacket = new char[MSG_SIZE];
	m_pRecvPart->tBuf.buf = m_pRecvPart->pPacket;
	m_pRecvPart->tBuf.len = MSG_SIZE;

	InetPton(AF_INET, pIP, &m_pRecvPart->tAddr.sin_addr.s_addr);

	if (WSAConnect(m_pRecvPart->hSocket, reinterpret_cast<sockaddr*>(&m_pRecvPart->tAddr), 
		sizeof(sockaddr), &m_pRecvPart->tBuf, NULL, NULL, NULL) == INVALID_SOCKET)
	{
		SAFE_DELETE(m_pSendPart);
		SAFE_DELETE(m_pRecvPart);
		return false;
	}

	m_pRecvPart->pPacket = new char[MSG_SIZE];
	m_pRecvPart->tBuf.buf = m_pRecvPart->pPacket;
	m_pRecvPart->tBuf.len = MSG_SIZE;
	m_pRecvPart->iCount = 0;
	m_pRecvPart->iRecvCount = 0;
	m_pRecvPart->iFlag = 0;
	memset(&m_pRecvPart->tOverlapped, 0, sizeof(OVERLAPPED));

	if (WSARecv(m_pRecvPart->hSocket, &m_pRecvPart->tBuf, 
		1, &m_pRecvPart->iRecvCount, 
		&m_pRecvPart->iFlag, &m_pRecvPart->tOverlapped, NULL) == SOCKET_ERROR)
	{
		DWORD iError = GetLastError();

		if (iError == WSA_IO_PENDING)
		{
			return true;
		}
		void* pData = nullptr;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, 
			iError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pData), 0, NULL);

		std::wstring strError = static_cast<TCHAR*>(pData);

		LocalFree(pData);

		return false;
	}

	return true;
}

SOCKET CUser::AsyncInit()
{
	m_tUserSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (m_tUserSock == INVALID_SOCKET)
	{
		assert(false);
		return 0;
	}

	m_pSendPart = new OVERPART;
	m_pRecvPart = new OVERPART;
	m_pSendPart->eType = WORK_TYPE::SEND;
	m_pRecvPart->eType = WORK_TYPE::RECV;
	m_pSendPart->hSocket = m_tUserSock;
	m_pRecvPart->hSocket = m_tUserSock;
	memset(&m_pSendPart->tOverlapped, 0, sizeof(OVERLAPPED));
	memset(&m_pRecvPart->tOverlapped, 0, sizeof(OVERLAPPED));
	m_pSendPart->pPacket = new char[MSG_SIZE];
	m_pRecvPart->pPacket = new char[MSG_SIZE];
	memset(m_pSendPart->pPacket, 0, MSG_SIZE);
	memset(m_pRecvPart->pPacket, 0, MSG_SIZE);
	m_pSendPart->iFlag = 0;
	m_pSendPart->iCount = 0;
	m_pSendPart->iRecvCount = 0;
	m_pRecvPart->iFlag = 0;
	m_pRecvPart->iCount = 0;
	m_pRecvPart->iRecvCount = 0;
	m_pSendPart->pUser = this;
	m_pRecvPart->pUser = this;

	return m_tUserSock;
}
