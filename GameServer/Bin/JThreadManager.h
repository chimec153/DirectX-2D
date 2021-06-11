#pragma once

#include "Server.h"

namespace Server
{
	class JThreadManager
	{
	private:
		JThreadManager();
		~JThreadManager();

	private:
		static JThreadManager* m_pInst;

	public:
		static JThreadManager* GetInst()
		{
			if (!m_pInst)
				m_pInst = new JThreadManager;
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

	private:
		std::unordered_map<std::string, class CThread*>	m_mapThread;

	public:
		template <typename T>
		T* CreateThread(const std::string& strKey)
		{
			T* pThread = new T;

			/*if (!pThread->Init())
			{
				SAFE_RELEASE(pThread);
				return nullptr;
			}*/

			pThread->AddRef();

			m_mapThread.insert(std::make_pair(strKey, pThread));

			return pThread;
		}

	private:
		//class CThread*
	};

}