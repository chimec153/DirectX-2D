#include "DataBase.h"

DEFINITION_SINGLE(CDataBase)

CDataBase::CDataBase()	:
	m_pDataBase(nullptr)
{

}	
CDataBase::~CDataBase()
{
	sqlite3_close(m_pDataBase);
}

bool CDataBase::Init()
{
	sqlite3_open("test.db", &m_pDataBase);

	sqlite3_stmt* stmt;
	const TCHAR* pQuery = 
		L"CREATE TABLE if not exists LOGIN ("\
		L"ID TEXT PRIMARY KEY,"\
		L"PASSWORD TEXT NOT NULL);";

	bool bResult = true;
	int iResult = sqlite3_prepare16_v2(m_pDataBase, pQuery, -1, &stmt, NULL);

	if (sqlite3_step(stmt) != SQLITE_DONE)
	{
		TCHAR strError[MSG_SIZE] = {};

		swprintf_s(strError, L"line %d: %s\n", __LINE__, static_cast<const TCHAR*>(sqlite3_errmsg16(m_pDataBase)));
		OutputDebugString(strError);
		bResult = false;
	}




	return true;
}

bool CDataBase::Login(const TCHAR* pID, const TCHAR* pPW)
{
	sqlite3_stmt* stmt;

	TCHAR strQuery[MSG_SIZE] = {};

	swprintf_s(strQuery, MSG_SIZE, L"SELECT * FROM LOGIN WHERE ID = '%s'", pID);
	sqlite3_prepare16_v2(m_pDataBase, strQuery, -1, &stmt, NULL);

	if (!stmt)
	{
		TCHAR strError[MSG_SIZE] = {};
		swprintf_s(strError, MSG_SIZE, L"line: %d, %s\n", 
			__LINE__, static_cast<const TCHAR*>(sqlite3_errmsg16(m_pDataBase)));
		OutputDebugString(strError);
		return false;
	}
	bool bResult = false;
	bool bDone = false;
	bool bExist = false;
	while (!bDone)
	{
		int iResult = sqlite3_step(stmt);
		switch (iResult)
		{
		case SQLITE_DONE:
		{
			bResult = false;
			bDone = true;

			if (!bExist)
			{
				sqlite3_stmt* stmt2;

				TCHAR _strQuery[MSG_SIZE] = {};

				swprintf_s(_strQuery, MSG_SIZE, L"INSERT INTO LOGIN (ID, PASSWORD) VALUES('%s','%s')", pID, pPW);

				sqlite3_prepare16_v2(m_pDataBase, _strQuery, -1, &stmt2, NULL);

				if (sqlite3_step(stmt2) != SQLITE_DONE)
				{
					TCHAR strError[MSG_SIZE] = {};
					swprintf_s(strError, L"line %d, %s\n", __LINE__, static_cast<const TCHAR*>(sqlite3_errmsg16(m_pDataBase)));
					OutputDebugString(strError);
				}
				else
				{
					bResult = true;
				}
			}

			break;
		}
			break;
		case SQLITE_ROW:
		{
			bExist = true;
			const char* _pPW = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

			char strPW[MSG_SIZE] = {};
#ifdef _UNICODE
			WideCharToMultiByte(CP_ACP, 0, pPW, -1, strPW, lstrlen(pPW), NULL, NULL);
#else
			strcpy_s(strPW, MSG_SIZE, pPW);
#endif

			if (!strcmp(strPW, _pPW))
			{
				bResult = true;
				bDone = true;
				break;
			}
			continue;
		}
			break;
		default:
			break;
		}
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

	return bResult;
}
