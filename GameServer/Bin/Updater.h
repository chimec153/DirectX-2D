#pragma once
#include "Server.h"
#include "JRef.h"
namespace Server
{
	class CUpdater :
		public JRef
	{
		friend class CNetServer;

	protected:
		CUpdater(class CNetServer& serv);
		~CUpdater() = 0;

	protected:
		class CNetServer& m_tServer;
		NET_SERVER_TYPE		m_eType;

	public:
		virtual void Update();
		virtual void ServerStart(unsigned int iPort, SERVER_TYPE eType);

	};

}