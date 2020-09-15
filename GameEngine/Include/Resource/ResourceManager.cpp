#include "ResourceManager.h"
#include "Mesh2D.h"
#include "ShaderManager.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()	:
	m_pDefaultMesh2D(nullptr)
{

}

CResourceManager::~CResourceManager()
{
	SAFE_RELEASE(m_pDefaultMesh2D);
	DESTROY_SINGLE(CShaderManager);
	SAFE_RELEASE_MAP(m_mapMesh);
}

CMesh2D* CResourceManager::GetDefaultMesh() const
{
	if (m_pDefaultMesh2D)
		m_pDefaultMesh2D->AddRef();

	return m_pDefaultMesh2D;
}

bool CResourceManager::Init()
{
	//	���̴� �Ŵ��� �ʱ�ȭ
	if (!GET_SINGLE(CShaderManager)->Init())
		return false;

	m_pDefaultMesh2D = new CMesh2D;

	if (!m_pDefaultMesh2D->CreateMesh())
		return false;

	return true;
}

CMesh* CResourceManager::FindMesh(const std::string& strName)
{
	std::unordered_map<std::string, CMesh*>::iterator iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CResourceManager::ReleaseMesh(const std::string& strName)
{
	std::unordered_map<std::string, CMesh*>::iterator iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
		return;

	if (iter->second->Release() == 0)
		m_mapMesh.erase(iter);
}
