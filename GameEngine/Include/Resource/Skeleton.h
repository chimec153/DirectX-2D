#pragma once

#include "../Ref.h"

typedef struct _tagBone
{
	std::string	strName;
	int		iBoneIndex;
	int		iParentIndex;
	Matrix	matOffset;
	Matrix	matBone;
	int		iDepth;

	_tagBone() :
		strName()
		, iBoneIndex(-1)
		, iParentIndex(-1)
		, matOffset()
		, matBone()
		, iDepth(-1)
	{

	}
}BONE, *PBONE;

class CSkeleton	:
	public CRef
{
	friend class CResourceManager;
	friend class CMesh;

private:
	CSkeleton();
	CSkeleton(const CSkeleton& skel);
	virtual ~CSkeleton();

private:
	int	m_iBoneCount;
	std::vector<PBONE>	m_vecBones;
	ID3D11Buffer* m_pBuffer;
	ID3D11ShaderResourceView* m_pSRV;

public:
	void AddBone(const PBONE pBone);
	const PBONE GetBone(int iIdx)	const;
	const PBONE GetBone(const std::string& strKey)	const;
	int GetBoneCount()	const;
	int GetBoneIndex(const std::string& strKey)	const;

public:
	bool SaveSkeleton(const char* pFilePath,const std::string& strPathKey = MESH_PATH);
	bool SaveSkeletonMultiByte(const char* pFullPath);
	void Save(FILE* pFile);
	bool LoadSkeleton(const std::string& strKey, const char* pFilePath, const std::string& strPathKey = MESH_PATH);
	bool LoadSkeletonMultiByte(const std::string& strKey, const char* pFullPath);
	void Load(FILE* pFile);
	void AddSkeleton(CSkeleton* pSkeleton);


public:
	bool CreateBuffer(const std::vector<Matrix>& vecOffset);
	void SetShaderResource(int iRegister);
	void ResetShaderResource(int iRegister);

public:
	CSkeleton* Clone();

public:
	virtual void ShowWindow();
};

// 랜즈플레어
// 물 랜더링
// 충돌 레이캐스팅
// 라이트맵