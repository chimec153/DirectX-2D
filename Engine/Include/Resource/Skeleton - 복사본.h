#pragma once

#include "../Ref.h"

typedef struct _tagBone	//	본 구조체를 정의한다.
{
	std::string		strName;	//	이름을 선언한다.
	int				iDepth;	//	깊이를 선언한다.
	int				iParentIndex;	//	부모 인덱스를 선언한다.
	Matrix		matOffset;	//	오프셋 행렬을 선언한다.
	Matrix		matBone;	//	본 행렬을 선언한다.
	int				iRefCount;	//	레퍼런스 카운트를 선언한다.

	_tagBone() :	//	기본생성자를 정의한다.
		iRefCount(1)	//	레퍼런스 카운트를 1로 초기화한다.
	{
	}
}BONE, * PBONE;	//	BONE으로 정의한다. 포인터를 PBONE으로 정의한다.

class CSkeleton :	//	스켈레톤 클래스를 정의한다.
	public CRef	//	레퍼런스 카운트를 상속받는다.
{
	friend class CResourceManager;	//	리소스 관리자의 private멤버 접근이 되게한다.
	friend class CAnimation;	//	애니메이션 클래스의 private멤버 접근이 되게한다.
	friend class CAnimationSequence;	//	애니메이션 시퀀스 클래스의 private멤버 접근이 되게한다.
	friend class CAnimationMesh;	//	애니메이션 매쉬 클래스의 private 멤버 접근이 되게한다.
	friend class CMesh;	//	매쉬 클래스의 private멤버 접근이 되게한다.

public:	//	외부에서 접근이 되는 멤버들이다.
	CSkeleton();	//	기본 생성자를 선언한다.
	CSkeleton(const CSkeleton& skeleton);	//	복사 생성자를 선언한다. 매개변수로 값의 변경이 안되는 스켈레톤 클래스 변수를 참조형으로 받는다.
	~CSkeleton();	//	소멸자를 선언한다.

private:	//	외부에서 접근이 안되는 멤버들이다.
	std::vector<PBONE>						m_vecBones;	//	본구조체 포인터 타입의 벡터 변수를 선언한다.
	ID3D11Texture2D* m_pBoneTex;	//	다이렉트 2차원 텍스쳐 포인터를 선언한다.
	ID3D11ShaderResourceView* m_pBoneSRV;	//	다이렉트 셰이더 리소스 뷰 포인터를 선언한다.
	PSTRUCTUREDBUFFER	m_pOffsetBuffer;

public:	//	외부에서 접근이 되는 멤버들이다.
	size_t GetBoneCount()	const;	//	size_t 변수를 반환하는 본개수를 얻는 함수를 선언한다. 클래스 멤버의 값의 변경이 안되는 함수다.
	PBONE GetBone(int iIndex = 0)	const;	//	본구조체 포인터를 반환하는 본을 얻는 함수를 선언한다. 매개변수로 정수형 변수를 받는다. 클래스 멤버의 값의 변경이 안되는 함수다.
	PBONE GetBone(const std::string& strName)	const;	//	본구조체 포인터를 반환하는 본을 얻는 함수다 . 매개변수로 값의 변경이 안되는 문자열 참조형 변수를 받는다. 클래스 멤버의 값의 변경이 안되는 함수다.
	int GetBoneIndex(const std::string& strName)	const;	//	정수형 변수를 반환하는 본 인덱스를 얻는 함수를 선언한다. 매개변수로 값의 변경이 안되는 문자열 참조형 변수를 받는다.  클래스 멤버의 값의 변경이 안되는 함수다.
	const Matrix& GetBoneMatrix(const std::string& strName)	const;	// 값의 변경이 안되는 행렬 참조형을 반환하는 본 행렬을 얻는 함수를 선언한다. 매개변수로 값의 변경이 안되는 문자열 참조형 변수를 받는다. 클래스 멤버의 값의 변경이 안되는 함수다.
	const Matrix& GetBoneMatrix(int iIndex)	const;	//	값의 변경이 안되는 행렬 공용체 참조형 변수를 반환하는 본 행렬을 얻는 함수를 선언한다. 매개변수로 정수형 변수를 받는다.  클래스 멤버의 값의 변경이 안되는 함수다.

public:	//	외부에서 접근이 되는 멤버들이다.
	void AddBone(PBONE pBone);	//	본을 추가하는 함수를 선언한다. 매개변수로 본구조체 포인터를 받는다.
	bool CreateBoneTexture();	//	bool 변수를 반환하는 본 텍스쳐를 생성하는 함수를 선언한다.
	bool UpdateBoneTexture(const std::vector<Matrix>& vecBoneMarix);	//	bool 변수를 반환하는 본텍스쳐를 갱신하는 함수를 선언한다. 매개변수로 값의 변경이 안되는 행렬 공용체 타입의 벡터를 참조형으로 받는다.
	bool SaveSkeleton(const char* pFileName, const std::string& strPathName = MESH_PATH);	//	bool 변수를 반환하는 스켈레톤을 저장하는 함수를 선언한다. 매개변수로 값의 변경이 안되는 문자 포인터, 값의 변경이 안되는 문자열 참조형 변수를 받는다.
	bool SaveSkeletonFullPath(const char* pFullPath);	//	bool 변수를 반환하는 스켈레톤을 전체경로로 저장하는 함수를 선언한다. 매개변수로 값의 변경이 안되는 문자 포인터를 받는다.
	bool LoadSkeleton( const std::string& strName, const char* pFileName, const std::string& strPathName = MESH_PATH);	//	bool 변수를 반환하는 스켈레톤을 불러오는 함수를 선언한다. 매개변수로 값의 변경이 안되는 문자열 참조형, 값의 변경이 안되는 문자 포인터, 값의 변경이 안되는 문자열 참조형 변수를 받는다.
	bool LoadSkeletonFullPath( const std::string& strName, const char* pFullPath);	//	bool 변수를 반환하는 스켈레톤을 전체경로로 로드하는 함수를 선언한다. 매개변수로 값의 변경이 안되는 문자열 참조형, 값의 변경이 안되는 문자 포인터를 받는다.
	void SetShader();	//	SRV를 셰이더에 묶는 함수를 선언한다.
	void ResetShader();
	void Update(float fTime, const Matrix& matWorld);	//	갱신함수를 선언한다. 매개변수로 float변수, 값의 변경이 안되는 행렬 공용체 참조형 변수를 받는다.
	std::shared_ptr<CSkeleton> Clone();	//	스켈레톤 포인터를 반환하는 팩토리 함수를 선언한다.
	void ShowWindow();
};

