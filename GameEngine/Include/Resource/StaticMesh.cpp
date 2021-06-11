#include "StaticMesh.h"
#include "Material.h"
#include "../Device.h"
#include "Shader.h"
#include "GraphicShader.h"

CStaticMesh::CStaticMesh()
{
	m_eMeshType = MESH_TYPE::MT_STATIC_MESH;
}

CStaticMesh::~CStaticMesh()
{
}

bool CStaticMesh::Init()
{
	return true;
}

bool CStaticMesh::LoadMesh(const std::string& strKey, const TCHAR* strFilePath, const std::string& strRootPath)
{
	return CMesh::LoadMesh(strKey, strFilePath, strRootPath);
}

bool CStaticMesh::LoadMeshFromFull(const std::string& strKey, const TCHAR* strFullPath)
{
	return CMesh::LoadMeshFromFull(strKey, strFullPath);
}

void CStaticMesh::Render(float fTime, int iContainer, int iSubset)
{
	CMesh::Render(fTime, iContainer, iSubset);
}

bool CStaticMesh::CreateMesh(D3D11_PRIMITIVE_TOPOLOGY eTopo, int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage, 
	void* pVtxData, int iIdxCount, int iIdxSize, D3D11_USAGE eIdxUsage,  DXGI_FORMAT eFmt, void* pIdxData,
	D3D11_CPU_ACCESS_FLAG eCpuFlag,
	D3D11_CPU_ACCESS_FLAG eIdxCpuFlag)
{
	return CMesh::CreateMesh(eTopo, iVtxCount, iVtxSize, eVtxUsage, pVtxData,
		iIdxCount, iIdxSize, eIdxUsage, eFmt, pIdxData,eCpuFlag, eIdxCpuFlag);
}

bool CStaticMesh::ConvertFBX(CFBXLoader* pLoader, const char* pFullPath)
{
	return CMesh::ConvertFBX(pLoader, pFullPath);
}

bool CStaticMesh::LoadMesh(const char* pFilePath)
{
	return CMesh::LoadMesh(pFilePath);
}

bool CStaticMesh::SaveMesh(const char* pFilePath)
{
	return CMesh::SaveMesh(pFilePath);
}

bool CStaticMesh::AddVertexBuffer(void* _pVtx, int iSize, int iCount, D3D11_USAGE eUsg, D3D11_PRIMITIVE_TOPOLOGY eTopo, PMESHCONTAINER pContainer)
{
	return CMesh::AddVertexBuffer(_pVtx, iSize, iCount, eUsg, eTopo, pContainer);
}

bool CStaticMesh::AddIndexBuffer(void* pIdxData, int iIdxSize, int iIdxCount, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, PMESHCONTAINER pContainer)
{
	return CMesh::AddIndexBuffer(pIdxData, iIdxSize, iIdxCount, eIdxUsage, eFmt, pContainer);
}

void CStaticMesh::Save(FILE* pFile)
{
	CMesh::Save(pFile);
}

void CStaticMesh::Load(FILE* pFile)
{
	CMesh::Load(pFile);
}
