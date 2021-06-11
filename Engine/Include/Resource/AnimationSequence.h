#pragma once

#include "../Ref.h"

typedef struct _tagKeyFrame
{
	double	dTime;
	Vector3	vPos;
	Vector3 vScale;
	Vector4	qRot;
}KEYFRAME, *PKEYFRAME;

typedef struct _tagBoneKeyFrame
{
	int	iBoneIndex;
	std::vector<PKEYFRAME>	vecKeyFrame;

	_tagBoneKeyFrame() :
		iBoneIndex(-1)
	{
	}

	~_tagBoneKeyFrame()
	{
		SAFE_DELETE_VECLIST(vecKeyFrame);
	}
}BONEKEYFRAME, *PBONEKEYFRAME;

typedef struct _tagAnimationTransition
{
	std::string strName;
	std::string	strSequence;
	class CAnimationSequence* pNextSequence;
	std::vector<std::function<bool()>>	vecTransitionFunc;	
	bool			bAutoTransition;
	std::list<std::function<void()>>	NotifyFuncList;

	_tagAnimationTransition() :
		strName()
		, strSequence()
		, pNextSequence(nullptr)
		, bAutoTransition(false)
	{
	}

	~_tagAnimationTransition()
	{
	}
}ANIMATIONTRANSITION, * PANIMATIONTRANSITION;

class CAnimationSequence	:
	public CRef
{
	friend class CResourceManager;
	friend class CAnimation;

private:
	CAnimationSequence();
	CAnimationSequence(const CAnimationSequence& seq);
	virtual ~CAnimationSequence();

private:
	float	m_fStartTime;
	float	m_fEndTime;
	float	m_fTimeLength;
	float	m_fFrameTime;
	int		m_iStartFrame;
	int		m_iEndFrame;
	int		m_iFrameCount;
	int		m_iTimeMode;
	std::vector<PBONEKEYFRAME>	m_vecBoneKeyFrames;
	bool	m_bLoop;
	ID3D11Buffer* m_pBuffer;
	ID3D11ShaderResourceView* m_pSRV;
	char	m_strFullPath[MAX_PATH];
	std::list<_tagAnimationTransition*>	m_TransitionList;
	float	m_fPlayTime;
	float	m_fPlayScale;
	int		m_iChangeFrame;
	std::vector<float>				m_vecBlendMask;
	ID3D11ShaderResourceView* m_pBlendMaskSRV;
	std::vector<std::function<void()>>	m_vecEndNotify;
	ID3D11Buffer* m_pBlendBuffer;

public:
	void SetStartFrame(int iFrame);
	void SetEndFrame(int iFrame);
	void SetPlayTime(float fTime);
	bool LoadFbxAnimation(const char* pFullPath, bool bLoop = false, bool bRootMotionRemove = false);
	bool CreateSequence(bool bLoop, struct _tagFbxAnimationClip* pClip,bool bRootMotionRemove = false);
	bool CreateSequence(const std::string& strKey, bool bLoop, const TCHAR* pFilePath, 
		const std::string& strPathKey = MESH_PATH, bool bRootMotionRemove = false);
	bool CreateSequenceFullPath(bool bLoop, const TCHAR* pFullPath, bool bRootMotionRemove = false);
	bool CreateSequenceFullPath(bool bLoop, const char* pFullPath, bool bRootMotionRemove = false);
	bool CreateBufferAndSRV(const std::vector<ANIMATIONKEYFRAME>& vecFrames, int iSize);
	void AddSequence(class CAnimationSequence* pSequence);
	void CreateIndentitySequence(int iBone, int iFrame);
	void RotateFrames();
	void SetBlendMask(int iBoneIndex, float fBlend);
	void CreateBlendMaskSRV(int iSize);

public:
	void AddEndCallBack(void(*pFunc)());
	template <typename T>
	void AddEndCallBack(T* pObj, void(T::* pFunc)())
	{
		m_vecEndNotify.push_back(std::bind(pFunc, pObj));
	}
	void EndCallBack();

public:
	void SetShaderResource(int iRegister);
	void ResetShaderResource(int iRegister);
	void SetBlendMaskSRV(int iRegister);

public:
	CAnimationSequence* Clone();

public:
	void Save(FILE* pFile);
	bool SaveFullPath(const char* pFullPath);
	void Load(FILE* pFile, bool bRootMotionRemove = false);
	bool LoadFullPath(const char* pFullPath, bool bRootMotionRemove = false);

public:
	virtual void ShowWindow();
};