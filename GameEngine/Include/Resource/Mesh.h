#pragma once

#include "../Ref.h"

class CMesh :
	public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;

protected:
	CMesh();
	virtual ~CMesh() = 0;

protected:
	Vector3		m_vMin;
	Vector3		m_vMax;
	MESH_TYPE	m_eMeshType;
	PVertex		m_pInstBuffer;
	int			m_iCount;
	std::vector<PMESHCONTAINER>	m_vecMeshContainer;
	std::vector<VERTEX3D>	m_vecVtx;
	std::vector<UINT>	m_vecIdx;

public:
	MESH_TYPE GetMeshType()	const;
	void SetMaterial(std::shared_ptr<class CMaterial> pMaterial, int iContainer = 0, int iSubset = 0);
	std::shared_ptr<class CMaterial> GetMaterial(int iContainer = 0, int iSubset = 0)	const;
	int GetInstCount()	const;
	const std::vector<PMESHCONTAINER>* GetMeshContainer()	const;
	const std::vector<VERTEX3D>* GetVecVtxCopy()	const;
	const std::vector<UINT>* GetVecIdxCopy()	const;
	void SetShader(const std::string& strKey, int iContainer = 0, int iSubset = 0);

public:
	Vector3 GetMax()	const
	{
		return m_vMax;
	}

	Vector3 GetMin()	const
	{
		return m_vMin;
	}

public:
	bool Init();
	virtual void Render(float fTime, int iContainer = 0, int iSubset = 0) = 0;
	void RenderIndex(float fTime, int iCount, int iStart, int iContainer = 0, int iSubset = 0);
	virtual void RenderInstancing(InstancingBuffer* pInstBuffer, int iCount, int iSize, float fTime);
	virtual void RenderInstancing(InstancingBuffer* pInstBuffer, int iCount, float fTime, int iContainer = 0, int iSubset = 0);
	virtual void RenderParticle(int iCount, float fTime);
	virtual bool CreateMesh(D3D11_PRIMITIVE_TOPOLOGY eTopo, int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage, void* pVtxData,
		int iIdxCount = 0, int iIdxSize = 0, D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN, void* pIdxData = nullptr, D3D11_CPU_ACCESS_FLAG eCpuFlag = static_cast<D3D11_CPU_ACCESS_FLAG>(0), D3D11_CPU_ACCESS_FLAG eIdxCpuFlag = static_cast<D3D11_CPU_ACCESS_FLAG>(0));
	bool CreateInstBuffer(int iSize, int iCount);
	void AddInstanceData(const PINSTANCINGDATA pData);
	void ChangeMeshBuffer(const std::vector<VERTEX3D>& vecVtx);
	void ChangeMeshBuffer(void* pData, int iSize);

protected:
	bool ConvertFBX(class CFBXLoader* pLoader, const char* pFullPath, bool bLoop = false, int iIndex = -1);
	bool LoadMesh(const char* pFilePath);
public:
	bool SaveMesh(const char* pFilePath);

protected:
	bool AddVertexBuffer(void* pVtx, int iSize, int iCount, D3D11_USAGE eUsg, D3D11_PRIMITIVE_TOPOLOGY eTopo, PMESHCONTAINER pContainer);
	bool AddIndexBuffer(void* pIdx, int iSize, int iCount, D3D11_USAGE eUsg, DXGI_FORMAT eFmt, PMESHCONTAINER pContainer);

public:
	virtual bool LoadMesh(const std::string& strKey, const TCHAR* strFilePath, const std::string& strRootPath = MESH_PATH, bool bLoop = false, int iIndex = -1);
	virtual bool LoadMeshFromFull(const std::string& strKey, const TCHAR* strFullPath, bool bLoop = false, int iIndex = -1);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual void ShowWindow();

public:
	void DeleteIndexBuffer(int iContainer = 0, int iSubset = 0, int iIndex = -1);
	void AddContainer(class CMesh* pMesh, int iContainer = -1, int iSubset = -1);
	void ChangeContainer(class CMesh* pMesh, int iDestContainer, int iContainer);

public:
	virtual class CMesh* Clone();
};