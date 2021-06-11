#pragma once

#include "../Ref.h"

typedef struct _tagKeyFrame	//	키프레임 구조체를 정의한다.
{
	double	dTime;	//	시간을 나타내는 double형 변수를 선언한다.
	Vector3	vPos;	//	위치를 나타내는 3차원 벡터 구조체를 선언한다.
	Vector3	vScale;	//	크기를 나타내는 3차원 벡터 구조체를 선언한다.
	Vector4	vRot;	//	회전을 나타내는 4차원 벡터 구조체를 선언한다.
}KEYFRAME, * PKEYFRAME;

typedef struct _tagBoneKeyFrame	//	본 키프레임 구조체를 정의한다.
{
	int		iBoneIndex;	//	본인덱스를 선언한다.
	std::vector<PKEYFRAME>	vecKeyFrame;	//	키프레임 포인터 벡터를 선언한다.
	int			iRefCount;	//	레퍼런스 카운트를 선언한다.

	_tagBoneKeyFrame() :	//	기본 생성자를 정의한다.
		iRefCount(1)	//	레퍼런스 카운트를 1로 초기화한다.
	{
	}

	~_tagBoneKeyFrame()	//	소멸자를 정의한다.
	{
		SAFE_DELETE_VECLIST(vecKeyFrame);	//	키프레임 벡터를 제거한다.
	}
}BONEKEYFRAME, * PBONEKEYFRAME;	//	BONEKEYFRAME으로 정의한다. 포인터는 PBONEKEYFRAME으로 정의한다.

class CAnimationSequence :
	public CRef
{
	friend class CAnimation;
	friend class CResourceManager;

public:
	CAnimationSequence();
	CAnimationSequence(const CAnimationSequence& anim);
	~CAnimationSequence();

private:
	class CAnimation* m_pAnim;
	bool							m_bLoop;
	float							m_fStartTime;
	float							m_fEndTime;
	float							m_fTimeLength;
	float							m_fFrameTime;
	float							m_fPlayTime;
	float							m_fPlayScale;
	int								m_iStartFrame;
	int								m_iEndFrame;
	int								m_iFrameLength;
	int								m_iFrameMode;
	int								m_iChangeFrame;
	bool							m_bEnd;
	std::vector<PBONEKEYFRAME>		m_vecKeyFrame;
	char							m_strFullPath[MAX_PATH];
	PSTRUCTUREDBUFFER				m_pKeyFrameBuffer;
	std::list<PAnimationTransition>	m_TransitionList;

public:
	std::shared_ptr<CAnimationSequence> Clone();
	bool IsSequenceEnd()	const;
	void GetKeyFrame(std::vector<PBONEKEYFRAME>& vecFrame);
	bool LoadFbxAnimation( const char* pFullPath, bool bLoop = false);
	bool Save(const TCHAR* pFileName, const std::string& strPathName = MESH_PATH);
	bool SaveMultibyte(const char* pFileName, const std::string& strPathName = MESH_PATH);
	bool SaveFullPath(const TCHAR* pFullPath);
	bool SaveFullPathMultibyte(const char* pFullPath);
	bool Load( const TCHAR* pFileName, const std::string& strPathName = MESH_PATH);
	bool LoadMultibyte(	const char* pFileName, const std::string& strPathName = MESH_PATH);
	bool LoadFullPath(	const TCHAR* pFullPath);
	bool LoadFullPathMultibyte(const char* pFullPath);
	int GetKeyFrameCount()	const;

public:
	void SetPlayScale(float fScale);
	void SetPlayTime(float fTime);

public:
	void ShowWindow();

public:
	bool CreateSequence(bool bLoop, struct _tagFbxAnimationClip* pClip);
	bool CreateSequence(const std::string& strName, bool bLoop,
		int iStartFrame, int iEndFrame, float fPlayTime,
		const std::vector<PBONEKEYFRAME>& vecFrame);
	bool CreateSequence(const TCHAR* pFullPath);
	bool CreateSequenceMultibyte( const char* pFullPath);
	bool CreateSequence(bool bLoop, const std::string& strName, const TCHAR* pFileName,
		const std::string& strPathName = MESH_PATH);
	void SetShader();
	void SetChangeShader();
	void ResetShader();
	void ResetChangeShader();
};

