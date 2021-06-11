#pragma once
#include "Mesh.h"
class CAnimationMesh :
	public CMesh
{
	friend class CResourceManager;

public:
	CAnimationMesh();
	CAnimationMesh(const CAnimationMesh& mesh);
	virtual ~CAnimationMesh();

public:
	bool Init();
	virtual void Render(float fTime);
	virtual void Render(float fTime, int iContainer = 0, int iSubset = 0);
	virtual void RenderInstancing(InstancingBuffer* pIBuffer, int iCount, int iSize, float fTime);

protected:
	bool ConvertFBX(class CFBXLoader* pLoader, const char* pFullPath);
	bool LoadMesh(const char* pFilePath);
	bool SaveMesh(const char* pFilePath);

protected:
	bool AddVertexBuffer(void* pVtx, int iSize, int iCount, D3D11_USAGE eUsg, D3D11_PRIMITIVE_TOPOLOGY eTopo, PMESHCONTAINER pContainer);
	bool AddIndexBuffer(void* pIdx, int iSize, int iCount, D3D11_USAGE eUsg, DXGI_FORMAT eFmt, PMESHCONTAINER pContainer);

public:
	virtual bool LoadMesh(const std::string& strKey, const TCHAR* strFilePath, const std::string& strRootPath = MESH_PATH);
	virtual bool LoadMeshFromFull(const std::string& strKey, const TCHAR* strFullPath);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual class CMesh* Clone();
};

