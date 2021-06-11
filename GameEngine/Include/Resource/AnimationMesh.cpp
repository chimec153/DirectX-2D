#include "AnimationMesh.h"

CAnimationMesh::CAnimationMesh()
{
	m_eMeshType = MESH_TYPE::MT_ANIMATION_MESH;
}

CAnimationMesh::CAnimationMesh(const CAnimationMesh& mesh)
{
}

CAnimationMesh::~CAnimationMesh()
{
}

bool CAnimationMesh::Init()
{
	return true;
}

void CAnimationMesh::Render(float fTime)
{
}

void CAnimationMesh::Render(float fTime, int iContainer, int iSubset)
{
	CMesh::Render(fTime, iContainer, iSubset);
}

void CAnimationMesh::RenderInstancing(InstancingBuffer* pIBuffer, int iCount, int iSize, float fTime)
{
	CMesh::RenderInstancing(pIBuffer, iCount, iSize, fTime);
}

bool CAnimationMesh::ConvertFBX(CFBXLoader* pLoader, const char* pFullPath)
{
	return CMesh::ConvertFBX(pLoader, pFullPath);
}

bool CAnimationMesh::LoadMesh(const char* pFilePath)
{
	return CMesh::LoadMesh(pFilePath);
}

bool CAnimationMesh::SaveMesh(const char* pFilePath)
{
	return CMesh::SaveMesh(pFilePath);
}

bool CAnimationMesh::AddVertexBuffer(void* pVtx, int iSize, int iCount, D3D11_USAGE eUsg, D3D11_PRIMITIVE_TOPOLOGY eTopo, PMESHCONTAINER pContainer)
{
	return CMesh::AddVertexBuffer(pVtx, iSize, iCount, eUsg, eTopo, pContainer);
}

bool CAnimationMesh::AddIndexBuffer(void* pIdxData, int iIdxSize, int iIdxCount, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, PMESHCONTAINER pContainer)
{
	return CMesh::AddIndexBuffer(pIdxData, iIdxSize, iIdxCount, eIdxUsage, eFmt, pContainer);
}

bool CAnimationMesh::LoadMesh(const std::string& strKey, const TCHAR* strFilePath, const std::string& strRootPath)
{
	return CMesh::LoadMesh(strKey, strFilePath, strRootPath);
}

bool CAnimationMesh::LoadMeshFromFull(const std::string& strKey, const TCHAR* strFullPath)
{
	return CMesh::LoadMeshFromFull(strKey, strFullPath);
}

void CAnimationMesh::Save(FILE* pFile)
{
	CMesh::Save(pFile);
}

void CAnimationMesh::Load(FILE * pFile)
{
	CMesh::Load(pFile);
}

class CMesh* CAnimationMesh::Clone()
{
	return new CAnimationMesh(*this);
}
