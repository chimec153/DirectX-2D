#include "ServerIOCP.h"
#include "Server/Server.h"
#include "Server/DataBase.h"
#include "Server/User.h"

CServerIOCP::CServerIOCP(CServer& core)	:
	CIOCP(core)
{
}

CServerIOCP::~CServerIOCP()
{
	DESTROY_SINGLE(CDataBase);
}

bool CServerIOCP::Init(UPDATER_TYPE eType, int iPort, const TCHAR* pIp)
{
	if(!CIOCP::Init(eType, iPort, pIp))
		return false;

	// 데이터 베이스 초기화
	if (!GET_SINGLE(CDataBase)->Init())
	{
		return false;
	}

	return true;
}

void CServerIOCP::Update()
{
	CIOCP::Update();
}

void CServerIOCP::RecvPacket(POVERPART pPart)
{
	EDITOR_HEADER eHeader = static_cast<EDITOR_HEADER>(pPart->tBuf.buf[0]);

	switch (eHeader)
	{
	case EDITOR_HEADER::CHAT:
	{
		m_tCore.SendOtherUser(pPart->pUser, pPart->tBuf.buf);
	}
		break;
	case EDITOR_HEADER::LOGIN:
	{
		RecvLoginPacket(pPart->pUser, &pPart->tBuf.buf[1]);
	}
		break;
	}
}

void CServerIOCP::RecvLoginPacket(CUser* pUser, const char* pPacket)
{
	int idx = 0;
	int iLength = pPacket[idx++];
	TCHAR strID[MSG_SIZE] = {};
	memcpy_s(strID, MSG_SIZE * 2, &pPacket[idx], iLength * 2);

	idx += iLength * 2;
	iLength = pPacket[idx++];
	TCHAR strPW[MSG_SIZE] = {};
	memcpy_s(strPW, MSG_SIZE * 2, &pPacket[idx], iLength * 2);

	char _pPacket[MSG_SIZE] = {};

	_pPacket[0] = static_cast<char>(EDITOR_HEADER::LOGIN);

	if (GET_SINGLE(CDataBase)->Login(strID, strPW))
	{
		pUser->SetState(USER_STATE::LOGIN);

		_pPacket[1] = true;
	}
	else
	{
		_pPacket[1] = false;
	}

	SendPacket(pUser, _pPacket);
}
