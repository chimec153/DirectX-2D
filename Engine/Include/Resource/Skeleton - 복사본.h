#pragma once

#include "../Ref.h"

typedef struct _tagBone	//	�� ����ü�� �����Ѵ�.
{
	std::string		strName;	//	�̸��� �����Ѵ�.
	int				iDepth;	//	���̸� �����Ѵ�.
	int				iParentIndex;	//	�θ� �ε����� �����Ѵ�.
	Matrix		matOffset;	//	������ ����� �����Ѵ�.
	Matrix		matBone;	//	�� ����� �����Ѵ�.
	int				iRefCount;	//	���۷��� ī��Ʈ�� �����Ѵ�.

	_tagBone() :	//	�⺻�����ڸ� �����Ѵ�.
		iRefCount(1)	//	���۷��� ī��Ʈ�� 1�� �ʱ�ȭ�Ѵ�.
	{
	}
}BONE, * PBONE;	//	BONE���� �����Ѵ�. �����͸� PBONE���� �����Ѵ�.

class CSkeleton :	//	���̷��� Ŭ������ �����Ѵ�.
	public CRef	//	���۷��� ī��Ʈ�� ��ӹ޴´�.
{
	friend class CResourceManager;	//	���ҽ� �������� private��� ������ �ǰ��Ѵ�.
	friend class CAnimation;	//	�ִϸ��̼� Ŭ������ private��� ������ �ǰ��Ѵ�.
	friend class CAnimationSequence;	//	�ִϸ��̼� ������ Ŭ������ private��� ������ �ǰ��Ѵ�.
	friend class CAnimationMesh;	//	�ִϸ��̼� �Ž� Ŭ������ private ��� ������ �ǰ��Ѵ�.
	friend class CMesh;	//	�Ž� Ŭ������ private��� ������ �ǰ��Ѵ�.

public:	//	�ܺο��� ������ �Ǵ� ������̴�.
	CSkeleton();	//	�⺻ �����ڸ� �����Ѵ�.
	CSkeleton(const CSkeleton& skeleton);	//	���� �����ڸ� �����Ѵ�. �Ű������� ���� ������ �ȵǴ� ���̷��� Ŭ���� ������ ���������� �޴´�.
	~CSkeleton();	//	�Ҹ��ڸ� �����Ѵ�.

private:	//	�ܺο��� ������ �ȵǴ� ������̴�.
	std::vector<PBONE>						m_vecBones;	//	������ü ������ Ÿ���� ���� ������ �����Ѵ�.
	ID3D11Texture2D* m_pBoneTex;	//	���̷�Ʈ 2���� �ؽ��� �����͸� �����Ѵ�.
	ID3D11ShaderResourceView* m_pBoneSRV;	//	���̷�Ʈ ���̴� ���ҽ� �� �����͸� �����Ѵ�.
	PSTRUCTUREDBUFFER	m_pOffsetBuffer;

public:	//	�ܺο��� ������ �Ǵ� ������̴�.
	size_t GetBoneCount()	const;	//	size_t ������ ��ȯ�ϴ� �������� ��� �Լ��� �����Ѵ�. Ŭ���� ����� ���� ������ �ȵǴ� �Լ���.
	PBONE GetBone(int iIndex = 0)	const;	//	������ü �����͸� ��ȯ�ϴ� ���� ��� �Լ��� �����Ѵ�. �Ű������� ������ ������ �޴´�. Ŭ���� ����� ���� ������ �ȵǴ� �Լ���.
	PBONE GetBone(const std::string& strName)	const;	//	������ü �����͸� ��ȯ�ϴ� ���� ��� �Լ��� . �Ű������� ���� ������ �ȵǴ� ���ڿ� ������ ������ �޴´�. Ŭ���� ����� ���� ������ �ȵǴ� �Լ���.
	int GetBoneIndex(const std::string& strName)	const;	//	������ ������ ��ȯ�ϴ� �� �ε����� ��� �Լ��� �����Ѵ�. �Ű������� ���� ������ �ȵǴ� ���ڿ� ������ ������ �޴´�.  Ŭ���� ����� ���� ������ �ȵǴ� �Լ���.
	const Matrix& GetBoneMatrix(const std::string& strName)	const;	// ���� ������ �ȵǴ� ��� �������� ��ȯ�ϴ� �� ����� ��� �Լ��� �����Ѵ�. �Ű������� ���� ������ �ȵǴ� ���ڿ� ������ ������ �޴´�. Ŭ���� ����� ���� ������ �ȵǴ� �Լ���.
	const Matrix& GetBoneMatrix(int iIndex)	const;	//	���� ������ �ȵǴ� ��� ����ü ������ ������ ��ȯ�ϴ� �� ����� ��� �Լ��� �����Ѵ�. �Ű������� ������ ������ �޴´�.  Ŭ���� ����� ���� ������ �ȵǴ� �Լ���.

public:	//	�ܺο��� ������ �Ǵ� ������̴�.
	void AddBone(PBONE pBone);	//	���� �߰��ϴ� �Լ��� �����Ѵ�. �Ű������� ������ü �����͸� �޴´�.
	bool CreateBoneTexture();	//	bool ������ ��ȯ�ϴ� �� �ؽ��ĸ� �����ϴ� �Լ��� �����Ѵ�.
	bool UpdateBoneTexture(const std::vector<Matrix>& vecBoneMarix);	//	bool ������ ��ȯ�ϴ� ���ؽ��ĸ� �����ϴ� �Լ��� �����Ѵ�. �Ű������� ���� ������ �ȵǴ� ��� ����ü Ÿ���� ���͸� ���������� �޴´�.
	bool SaveSkeleton(const char* pFileName, const std::string& strPathName = MESH_PATH);	//	bool ������ ��ȯ�ϴ� ���̷����� �����ϴ� �Լ��� �����Ѵ�. �Ű������� ���� ������ �ȵǴ� ���� ������, ���� ������ �ȵǴ� ���ڿ� ������ ������ �޴´�.
	bool SaveSkeletonFullPath(const char* pFullPath);	//	bool ������ ��ȯ�ϴ� ���̷����� ��ü��η� �����ϴ� �Լ��� �����Ѵ�. �Ű������� ���� ������ �ȵǴ� ���� �����͸� �޴´�.
	bool LoadSkeleton( const std::string& strName, const char* pFileName, const std::string& strPathName = MESH_PATH);	//	bool ������ ��ȯ�ϴ� ���̷����� �ҷ����� �Լ��� �����Ѵ�. �Ű������� ���� ������ �ȵǴ� ���ڿ� ������, ���� ������ �ȵǴ� ���� ������, ���� ������ �ȵǴ� ���ڿ� ������ ������ �޴´�.
	bool LoadSkeletonFullPath( const std::string& strName, const char* pFullPath);	//	bool ������ ��ȯ�ϴ� ���̷����� ��ü��η� �ε��ϴ� �Լ��� �����Ѵ�. �Ű������� ���� ������ �ȵǴ� ���ڿ� ������, ���� ������ �ȵǴ� ���� �����͸� �޴´�.
	void SetShader();	//	SRV�� ���̴��� ���� �Լ��� �����Ѵ�.
	void ResetShader();
	void Update(float fTime, const Matrix& matWorld);	//	�����Լ��� �����Ѵ�. �Ű������� float����, ���� ������ �ȵǴ� ��� ����ü ������ ������ �޴´�.
	std::shared_ptr<CSkeleton> Clone();	//	���̷��� �����͸� ��ȯ�ϴ� ���丮 �Լ��� �����Ѵ�.
	void ShowWindow();
};

