#include "SocketManager.h"
#include "Scene/SceneManager.h"
#include "../Object/Player.h"
#include "Engine.h"

DEFINITION_SINGLE(CSocketManager)

bool CSocketManager::m_bMove = false;
Vector2 CSocketManager::m_vPos = {};
HANDLE CSocketManager::m_tMutex = {};
bool CSocketManager::m_bLoop = true;

CSocketManager::CSocketManager()	:
	m_tData()
{
	
}

CSocketManager::~CSocketManager()
{
	Exit();
	CloseHandle(m_tMutex);
}

bool CSocketManager::Init()
{
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, NULL);

	m_tMutex = CreateMutex(NULL, false, NULL);
	
	return true;
}

void CSocketManager::Move(const Vector2& vPos)
{
	WaitForSingleObject(m_tMutex, INFINITE);
	m_bMove = true;

	m_vPos = vPos;
	ReleaseMutex(m_tMutex);
}

void CSocketManager::Exit()
{
	m_bLoop = false;
}

unsigned _stdcall CSocketManager::ThreadFunc(void* pData)
{
	WSADATA tData = {};

	WSAStartup(MAKEWORD(2, 2), &tData);

	sockaddr_in tClntAddr = {};
	SOCKET tClnt = {};

	tClntAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	tClntAddr.sin_family = AF_INET;
	tClntAddr.sin_port = htons(atoi("2738"));

	tClnt = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(tClnt, (sockaddr*)& tClntAddr, (int)sizeof(sockaddr)) != SOCKET_ERROR)
	{
		ULONG lAddr = tClntAddr.sin_addr.s_addr;
		char strAddr[8] = {};
		memcpy(strAddr, &lAddr, 4);
		//send(tClnt, strAddr, 8, 0);
		while (m_bLoop)
		{
			WaitForSingleObject(m_tMutex, INFINITE);
			if (m_bMove)
			{
				m_bMove = false;
				ReleaseMutex(m_tMutex);
				char strMsg[8] = {};
				
				float fX = m_vPos.x;
				float fY = m_vPos.y;

				memcpy(strMsg, &fX, 4);
				memcpy(strMsg+4, &fY, 4);
				send(tClnt, strMsg, 8, 0);

				char strRecv[8] = {};
				int iCount = recv(tClnt, strRecv, 8, 0);
				if (iCount > 0)
				{
					Vector2 vMove = {};

					memcpy(&vMove.x, strRecv, 4);
					memcpy(&vMove.y, strRecv + 4, 4);

					std::shared_ptr<CGameObject> pPlayer = GET_SINGLE(CSceneManager)->GetScene()->GetGameMode()->GetPlayer();

					((CPlayer*)pPlayer.get())->Move(vMove);
				}
			}

			else
			{
				ReleaseMutex(m_tMutex);
			}
		}

		//send(tClnt, "exit", 8, 0);
		lAddr = tClntAddr.sin_addr.s_addr;
		memset(strAddr, 0, 8);
		memcpy(strAddr, &lAddr, 4);
		//send(tClnt, strAddr, 8, 0);
	}

	closesocket(tClnt);

	WSACleanup();

	return 0;
}

unsigned _stdcall CSocketManager::RecvFunc(void* pData)
{
	return 0;
}
