#pragma once
#include "../GameEngine.h"
class CClientSocket
{
private:
	CClientSocket();
	~CClientSocket();

private:
	int					m_iPort;
	const TCHAR*		m_pIP;
	class CUser*		m_pUser;
	SOCKET				m_tClientSock;
	static HANDLE		g_tComport;
	std::function<void(const char*)>	m_PacketFunc;

public:
	class CUser* GetUser()	const;

public:
	bool Init(int iPort, const TCHAR* pIP);
	void SendPacket(const char* pMsg);

	void SetPacketFunc(void(*pFunc)(const char*));
	template <typename T>
	void SetPacketFunc(T* pObj, void(T::* pFunc)(const char*))
	{
		m_PacketFunc = std::bind(pFunc, pObj, std::placeholders::_1);
	}

private:
	static unsigned __stdcall ThreadFunc(void* pData);

private:
	static CClientSocket* m_pInst;

public:
	static CClientSocket* CreateInst()
	{
		if (!m_pInst)
		{
			m_pInst = new CClientSocket;
		}

		return m_pInst;
	}
	static CClientSocket* GetInst()
	{
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst)
		{
			delete m_pInst;
			m_pInst = nullptr;
		}
	}


};

