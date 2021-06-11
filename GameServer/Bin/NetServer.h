#pragma once

#include "Server.h"

namespace Server
{
	class CNetServer
	{
	private:
		CNetServer();
		~CNetServer();

	private:
		OVERPART	m_tRecv;
		OVERPART	m_tSend;
		std::unordered_map<class CUser*, SOCKET>	m_mapWait;
		std::unordered_map<class CUser*, SOCKET>	m_mapConnected;
		SOCKET		m_tSock;
		class CUpdater* m_pServer;
		bool		m_bAlive;
		static class CNetServer* m_pInst;

	public:
		void Start(unsigned int iPort, NET_SERVER_TYPE eNetType = NET_SERVER_TYPE::IOCP, SERVER_TYPE eType = SERVER_TYPE::THREAD);
		void End();
		bool IsAlive()	const
		{
			return m_bAlive;
		}
		SOCKET GetAsynSock(unsigned int iPort, unsigned int iCount);
		class CUser* CreateUser();
		class CUser* GetAsynAccept();
		void WaitToConnect(class CUser* pUser);
		const std::list<class CUser*> GetOtherUser(class CUser* pUser)	const;
		const std::list<class CUser*> GetAllUser()	const;

	public:
		static class CNetServer* GetInst()
		{
			if (!m_pInst)
				m_pInst = new CNetServer;
			return m_pInst;
		}
		static void DestroyInst()
		{
			if (m_pInst)
				delete m_pInst;
			m_pInst = nullptr;
		}
	};

}