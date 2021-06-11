#pragma once

#include "../GameEngine.h"

class CResourceManager
{
private:
	class CMesh* m_pDefaultMesh2D;
	class CMesh* m_pDefaultMeshUI;
	std::shared_ptr<class CTexture> m_pDefaultTexUI;
	std::unordered_map<std::string, class CMesh*>									m_mapMesh;
	std::unordered_map<std::string, std::shared_ptr<class CMaterial>>				m_mapMaterial;
	std::unordered_map<std::string, std::shared_ptr<class CTexture>>				m_mapTexture;
	std::unordered_map<std::string, class CSample*>									m_mapSample;
	std::unordered_map<std::string, std::shared_ptr<class CAnimation2DSequence>>	m_mapAni2DSequence;
	std::unordered_map<std::string, class CAnimationSequence*>						m_mapSequence;
	std::unordered_map<std::string, class CSkeleton*>								m_mapSkeleton;
	CRITICAL_SECTION* m_pCRT;

public:
	class CMesh* GetDefaultMesh()	const;
	class CMesh* GetDefaultUIMesh()	const;
	std::shared_ptr<class CTexture> GetDefaultTexture()	const;

public:
	bool Init();

	class CMesh* FindMesh(const std::string& strName);
	bool CreateMesh(const std::string& strTag, MESH_TYPE eType, D3D11_PRIMITIVE_TOPOLOGY eTopo, int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage,
		void* pVtxData, int iIdxCount = 0, int iIdxSize = 0, D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN, void* pIdxData = nullptr,
		D3D11_CPU_ACCESS_FLAG eCpuFlag = static_cast<D3D11_CPU_ACCESS_FLAG>(0),
		D3D11_CPU_ACCESS_FLAG eIdxCpuFlag = static_cast<D3D11_CPU_ACCESS_FLAG>(0));
	bool LoadMesh(MESH_TYPE eType, const std::string& strKey, const TCHAR* pFilePath, const std::string& strPathKey = MESH_PATH, bool bLoop = false, int iIndex = -1);
	bool LoadMeshFromFull(MESH_TYPE eType, const std::string& strKey, const TCHAR* pFullPath);
	void DeleteMesh(const std::string& strTag);

public:
	std::shared_ptr<class CMaterial> FindMaterial(const std::string& strName);
	std::shared_ptr<class CMaterial> CreateMaterial(const std::string& strName);

public:
	std::shared_ptr<class CTexture> FindTexture(const std::string& strName);
	bool LoadTexture(const std::string& strTag,
		const TCHAR* pFileName,
		const std::string& strRootPath = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const std::string& strTag,
		const TCHAR* pFullPath);
	bool LoadTexture(const std::string& strTag,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& strRootPath = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const std::string& strTag,
		const std::vector<const TCHAR*>& vecFullPath);
	bool LoadTextureArray(const std::string& strTag,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& strRootPath = TEXTURE_PATH);
	bool LoadTextureArrayFromFullPath(const std::string& strTag,
		const std::vector<const TCHAR*>& vecFullPath);
	std::shared_ptr<class CTexture> LoadTexture(FILE* pFile);

public:
	class CSample* CreateSample(const std::string& strTag, D3D11_FILTER eFilter,
		D3D11_TEXTURE_ADDRESS_MODE eUMode = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE eVMode = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE eWMode = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_COMPARISON_FUNC eFunc = D3D11_COMPARISON_NEVER,
		int iMaxanisotropy = 0,
		float pColor[4] = nullptr);
	class CSample* FindSample(const std::string& strTag);

public:
	bool CreateAni2DSequence(const std::string& strTag, std::shared_ptr<class CTexture> pTex, float fPlayRate = 1.f, float fMaxTime = 1.f);
	bool CreateAni2DSequence(const std::string& strTag, const std::string& strTex, float fPlayRate = 1.f, float fMaxTime = 1.f);
	bool LoadAni2DSequence(const TCHAR* pFileName, const std::string& strPathName = RESOURCE_PATH);
	std::shared_ptr<class CAnimation2DSequence> LoadAni2DSeqFromFullPath(const TCHAR* pFileName);
	void SaveAni2DSeqFromFullPath(const std::string& strTag, const TCHAR* pFileName);
	void AddFrame(const std::string& strTag, const Vector2& vStart, const Vector2& vEnd);
	std::shared_ptr<class CAnimation2DSequence> FindAni2DSequence(const std::string& strTag);
	void AddAni2DNotify(const std::string& strSeq, const std::string& strNot, int iFrame);

public:
	bool LoadAnimationSequence(const std::string& strKey, bool bLoop, const TCHAR* pFilePath, const std::string& strPathKey = MESH_PATH, bool bRootMotionRemove = false);
	bool LoadAnimationSequence(const std::string& strKey, bool bLoop, struct _tagFbxAnimationClip* pClip);
	class CAnimationSequence* FindSequence(const std::string& strKey);
	bool LoadAnimationSequenceFullPath(const std::string& strKey, bool bLoop, const TCHAR* pFullPath);
	void AddAnimationSequence(const std::string& strSrc, const std::string& strDest, const char* pFilePath = nullptr, const std::string& strPathKey = MESH_PATH);

public:
	bool LoadSkeleton(const std::string& strKey, const char* pFilePath, const std::string& strPathKey = MESH_PATH);
	bool LoadSkeletonFullPath(const std::string& strKey, const char* pFullPath);
	void SetSkeleton(const std::string& strKey, class CSkeleton* pSkeleton);
	class CSkeleton* FindSkeleton(const std::string& strKey);
	class CSkeleton* LoadSkeleton(FILE* pFile);
	void AddSkeleton(const std::string& strDest, const std::string& strSrc, const char* pFilePath = nullptr, const std::string& strPathKey = MESH_PATH);

public:
	void CreateSphere(const std::string& strName, float radius, UINT sliceCount, UINT stackCount);

public:
	void ShowWindow();
	const std::string ShowMesh();

	DECLARE_SINGLE(CResourceManager)
};

