#pragma once
#include "Thread/Thread.h"

namespace Server
{
	typedef struct _tagWorkData
	{
		void* pArg;
		std::function<unsigned(void*)>	pFunc;
	}WORKDATA, * PWORKDATA;

	class CServerThread :
		public CThread
	{
		friend class JThreadManager;

	private:
		CServerThread();
		virtual ~CServerThread();

	private:
		std::list<WORKDATA>		m_WorkList;

	public:
		void AddWork(unsigned (*pFunc)(void*), void* pArg);
		void Run();
		void WorkFront();

	};

}