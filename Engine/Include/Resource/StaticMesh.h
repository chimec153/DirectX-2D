#pragma once
#include "Mesh.h"

class CStaticMesh :
	public CMesh
{
	friend class CResourceManager;
	friend class CSceneResource;

public:
	CStaticMesh();
	~CStaticMesh();

public:
	bool Init();
	virtual bool LoadMesh(const std::string& strKey, const TCHAR* strFilePath, const std::string& strRootPath = MESH_PATH);
	virtual bool LoadMeshFromFull(const std::string& strKey, const TCHAR* strFullPath);
	//virtual void Render(float fTime);
	virtual void Render(float fTime, int iContainer = 0, int iSubset = 0);
	virtual bool CreateMesh(D3D11_PRIMITIVE_TOPOLOGY eTopo, int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage, void* pVtxData,
		int iIdxCount = 0, int iIdxSize = 0, D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN, void* pIdxData = nullptr,
		D3D11_CPU_ACCESS_FLAG eCpuFlag = static_cast<D3D11_CPU_ACCESS_FLAG>(0),
		D3D11_CPU_ACCESS_FLAG eIdxCpuFlag = static_cast<D3D11_CPU_ACCESS_FLAG>(0));

protected:
	bool ConvertFBX(class CFBXLoader* pLoader, const char* pFullPath);
	bool LoadMesh(const char* pFilePath);
	bool SaveMesh(const char* pFilePath);

private:
	bool AddVertexBuffer(void* pVtx, int iSize, int iCount, D3D11_USAGE eUsg, D3D11_PRIMITIVE_TOPOLOGY eTopo, PMESHCONTAINER pContainer);
	bool AddIndexBuffer(void* pIdx, int iSize, int iCount, D3D11_USAGE eUsg, DXGI_FORMAT eFmt,PMESHCONTAINER pContainer);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};
