#include "SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Shader.h"
#include "../Resource/Mesh.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{
	std::unordered_map<std::string, CMesh*>::iterator iterMesh = m_mapMesh.begin();
	std::unordered_map<std::string, CMesh*>::iterator iterMeshEnd = m_mapMesh.end();

	for (; iterMesh != iterMeshEnd; ++iterMesh)
		GET_SINGLE(CResourceManager)->ReleaseMesh(iterMesh->first);

	std::unordered_map<std::string, CShader*>::iterator iterShader = m_mapShader.begin();
	std::unordered_map<std::string, CShader*>::iterator iterShaderEnd = m_mapShader.end();

	for (; iterShader != iterShaderEnd; ++iterShader)
		GET_SINGLE(CShaderManager)->ReleaseShader(iterShader->first);
}

bool CSceneResource::Init()
{
	return true;
}

CMesh* CSceneResource::FindMesh(const std::string& strName)
{
	std::unordered_map<std::string, class CMesh*>::iterator iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
	{
		CMesh* pMesh = GET_SINGLE(CResourceManager)->FindMesh(strName);

		if (!pMesh)
			return nullptr;

		pMesh->AddRef();

		m_mapMesh.insert(std::make_pair(strName, pMesh));

		return pMesh;
	}

	iter->second->AddRef();

	return iter->second;
}