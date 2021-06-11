#pragma once

#include "JRef.h"
#include "Server.h"

namespace Server
{
	class CUser :
		public JRef
	{
		friend class CNetServer;
		friend class CIOCP;

	private:
		CUser(class CNetServer& serv);
		~CUser();

	private:
		class CNetServer& m_tServer;
		SOCKET		m_tSock;
		SOCKADDR_IN	m_tSockAdr;
		SOCKADDR_IN	m_tSockRemote;
		OVERPART	m_tSend;
		OVERPART	m_tRecv;

	public:
		bool Init();
	};

}