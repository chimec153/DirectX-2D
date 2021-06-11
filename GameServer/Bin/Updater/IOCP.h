#pragma once
#include "../Updater.h"
namespace Server
{
	class CIOCP :
		public CUpdater
	{
		friend class CNetServer;

	private:
		CIOCP(class CNetServer& serv);
		~CIOCP();

	private:
		static HANDLE		m_tComPort;
		static unsigned int	m_iCount;
		HANDLE* m_pThreadPool;
		unsigned int		m_iPort;
		char				m_pBuf[BUF_SIZE * 5];
		int					m_iIndex;

	public:
		virtual void Update();

	public:
		void ServerStart(unsigned int iPort, SERVER_TYPE eType);

	private:
		static unsigned WINAPI ThreadFunc(void* pData);

	};

}