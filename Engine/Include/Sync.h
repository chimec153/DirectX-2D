#pragma once

#include "GameEngine.h"

class CSync
{
public:
	CSync(CRITICAL_SECTION* pCrt)
	{
		m_pCrt = pCrt;
		EnterCriticalSection(m_pCrt);
	}

	~CSync()
	{
		LeaveCriticalSection(m_pCrt);
	}

private:
	CRITICAL_SECTION*	m_pCrt;
};