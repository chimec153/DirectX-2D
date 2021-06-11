#include "PathManager.h"

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{

}

CPathManager::~CPathManager()
{
	SAFE_DELETE_MAP(m_mapPath);
}

bool CPathManager::Init()
{
	TCHAR strRootPath[MAX_PATH] = {};

	GetModuleFileName(0, strRootPath, MAX_PATH);

	for (int i = lstrlen(strRootPath)-1; i > 0; --i)
	{
		if (strRootPath[i] == '\\' || strRootPath[i] == '/')
		{
			memset(strRootPath + i + 1, 0, (lstrlen(strRootPath) - i - 1) * sizeof(TCHAR));

			break;
		}
	}

	PPath pPath = new Path;

	lstrcpy(pPath->pPath, strRootPath);

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, 0, pPath->pPath, -1, pPath->pMultibytePath, lstrlen(pPath->pPath), NULL, NULL);
#else
	strcpy(pPath->pMultibytePath, pPath->pPath);
#endif

	m_mapPath.insert(std::make_pair(ROOT_PATH, pPath));

	AddPath(TEXTURE_PATH, TEXT("Resource\\Texture\\"));
	AddPath(DATA_PATH, TEXT("Data\\"));
	AddPath(SOUND_PATH, TEXT("Sound\\"));
	AddPath(SHADER_PATH, TEXT("Resource\\Shader\\"));
	AddPath(RESOURCE_PATH, TEXT("Resource\\"));
	AddPath(MESH_PATH, TEXT("Resource\\Mesh\\"));

	return true;
}

void CPathManager::AddPath(const std::string& strName, const TCHAR* pPath, const std::string& strRootPath)
{
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pRootPath = FindPath(strRootPath);

	if (pRootPath)
		lstrcpy(strFullPath, pRootPath);

	lstrcat(strFullPath, pPath);

	PPath pNewPath = new Path;

	lstrcpy(pNewPath->pPath, strFullPath);

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, 0, pNewPath->pPath, -1, pNewPath->pMultibytePath, lstrlen(pNewPath->pPath), NULL, NULL);
#else
	strcpy(pNewPath->pMultibytePath, pNewPath->pPath);
#endif

	m_mapPath.insert(std::make_pair(strName, pNewPath));
}

const TCHAR* CPathManager::FindPath(const std::string& strName)
{
	std::unordered_map<std::string, PPath>::iterator iter = m_mapPath.find(strName);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second->pPath;
}

const char* CPathManager::FindMultiBytePath(const std::string& strName)
{
	std::unordered_map<std::string, PPath>::iterator iter = m_mapPath.find(strName);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second->pMultibytePath;
}
