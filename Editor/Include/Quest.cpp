#include "Quest.h"
#include "PathManager.h"

std::unordered_map<std::string, CQuest*> CQuest::m_mapQuest = {};

CQuest::CQuest()	:
	m_eState(QUEST_STATE::NONE)
{
}

CQuest::~CQuest()
{
}

CQuest* CQuest::FindQuest(const std::string& strKey)
{
	std::unordered_map<std::string, CQuest*>::iterator iter = m_mapQuest.find(strKey);

	if (iter == m_mapQuest.end())
		return nullptr;

	return iter->second;
}

void CQuest::Destroy()
{
	SAFE_DELETE_MAP(m_mapQuest);
}

void CQuest::LoadQuestInfo(const char* pFilePath, const std::string& strPathKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strPathKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFilePath);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		while (true)
		{
			char strText[MAX_PATH] = {};

			fgets(strText, MAX_PATH, pFile);

			if (!strcmp(strText, ""))
			{
				break;
			}

			if (strText[strlen(strText) - 1] == '\n')
				strText[strlen(strText) - 1] = 0;

			CQuest* pQuest = new CQuest;

			pQuest->m_strName = strText;

			char strDesc[MAX_PATH] = {};

			fgets(strDesc, MAX_PATH, pFile);

			if (!strcmp(strDesc, ""))
			{
				SAFE_DELETE(pQuest);
				break;
			}

			TCHAR pDesc[MAX_PATH] = {};

#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, strDesc, -1, pDesc, static_cast<int>(strlen(strDesc)));
#else
			strcpy_s(pDesc, MAX_PATH, strDesc );
#endif
			if (pDesc[lstrlen(pDesc) - 1] == '\n')
				pDesc[lstrlen(pDesc) - 1] = 0;

			pQuest->m_strDesc = pDesc;

			for (int i = 0; i < static_cast<int>(QUEST_STATE::END); i++)
			{
				char strTalk[MAX_PATH] = {};

				fgets(strTalk, MAX_PATH, pFile);

				if (!strcmp(strTalk, ""))
				{
					SAFE_DELETE(pQuest);
					break;
				}

				if (strTalk[strlen(strTalk) - 1] == '\n')
					strTalk[strlen(strTalk) - 1] = 0;

				TCHAR pTalk[MAX_PATH] = {};
#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, strTalk, -1, pTalk, static_cast<int>(strlen(strTalk)));
#else
				strcpy_s(pTalk, MAX_PATH, strTalk);
#endif

				pQuest->m_pTalk[i] = pTalk;
			}

			if (pQuest)
			{
				m_mapQuest.insert(std::make_pair(pQuest->m_strName, pQuest));
			}
		}


		fclose(pFile);
	}
}
