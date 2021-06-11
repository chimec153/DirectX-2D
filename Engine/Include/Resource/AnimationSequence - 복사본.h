#pragma once

#include "../Ref.h"

typedef struct _tagKeyFrame	//	Ű������ ����ü�� �����Ѵ�.
{
	double	dTime;	//	�ð��� ��Ÿ���� double�� ������ �����Ѵ�.
	Vector3	vPos;	//	��ġ�� ��Ÿ���� 3���� ���� ����ü�� �����Ѵ�.
	Vector3	vScale;	//	ũ�⸦ ��Ÿ���� 3���� ���� ����ü�� �����Ѵ�.
	Vector4	vRot;	//	ȸ���� ��Ÿ���� 4���� ���� ����ü�� �����Ѵ�.
}KEYFRAME, * PKEYFRAME;

typedef struct _tagBoneKeyFrame	//	�� Ű������ ����ü�� �����Ѵ�.
{
	int		iBoneIndex;	//	���ε����� �����Ѵ�.
	std::vector<PKEYFRAME>	vecKeyFrame;	//	Ű������ ������ ���͸� �����Ѵ�.
	int			iRefCount;	//	���۷��� ī��Ʈ�� �����Ѵ�.

	_tagBoneKeyFrame() :	//	�⺻ �����ڸ� �����Ѵ�.
		iRefCount(1)	//	���۷��� ī��Ʈ�� 1�� �ʱ�ȭ�Ѵ�.
	{
	}

	~_tagBoneKeyFrame()	//	�Ҹ��ڸ� �����Ѵ�.
	{
		SAFE_DELETE_VECLIST(vecKeyFrame);	//	Ű������ ���͸� �����Ѵ�.
	}
}BONEKEYFRAME, * PBONEKEYFRAME;	//	BONEKEYFRAME���� �����Ѵ�. �����ʹ� PBONEKEYFRAME���� �����Ѵ�.

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

