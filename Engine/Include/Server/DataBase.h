#pragma once

#include "../GameEngine.h"

class CDataBase
{
private:
	sqlite3*	m_pDataBase;

public:
	bool Init();
	bool Login(const TCHAR* pID, const TCHAR* pPW);

	DECLARE_SINGLE(CDataBase)
};

