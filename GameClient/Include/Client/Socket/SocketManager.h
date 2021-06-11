#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "GameEngine.h"
#include <WinSock2.h>

class CSocketManager
{
private:
	WSADATA		m_tData;
	static bool		m_bMove;
	static Vector2	m_vPos;
	static HANDLE		m_tMutex;
	static bool		m_bLoop;

public:
	bool Init();
	void Move(const Vector2& vPos);
	void Exit();

private:
	static unsigned _stdcall ThreadFunc(void* pData);
	static unsigned _stdcall RecvFunc(void* pData);

	DECLARE_SINGLE(CSocketManager)
};

