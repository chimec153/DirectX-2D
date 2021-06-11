#pragma once

#include "../Ref.h"
#include "AnimationSequence.h"
#include "Skeleton.h"

typedef struct _tagAnimationNotify
{
	std::string	strName;
	class CAnimationSequence* pSequence;
	std::vector<std::function<void(float)>>	vecNotify;
	std::vector<std::function<void(float,float)>>	vecTimeLineNotify;
	float	fTime;
	float	fEndTime;
	bool	bCall;
	bool	bOnce;
	_tagAnimationNotify() :
		strName()
		, pSequence(nullptr)
		, fTime(-1.f)
		, fEndTime(-1.f)
		, bCall(false)
		, bOnce(true)
	{

	}

	~_tagAnimationNotify()
	{
	}

}ANIMATIONNOTIFY, *PANIMATIONNOTIFY;

class CAnimation	:
	public CRef
{
	friend class CResourceManager;

private:
	CAnimation();
	CAnimation(const CAnimation& ani);
	virtual ~CAnimation();

private:
	std::unordered_map<size_t, class CAnimationSequence*>	m_mapSequence;
	class CAnimationSequence*		m_pSequence;
	class CSkeleton*				m_pSkeleton;
	ID3D11Buffer*					m_pBuffer;
	ID3D11ShaderResourceView*		m_pSrv;
	ID3D11UnorderedAccessView*		m_pUav;
	ID3D11Buffer*					m_pCopyBuffer;
	std::shared_ptr<class CShader>	m_pUpdateShader;
	std::list<class CBoneSocket*>	m_SocketList;
	std::list<PANIMATIONNOTIFY>		m_NotifyList;
	struct _tagAnimationTransition*	m_pTransition;
	float							m_fAnimationTime;
	bool							m_bEnd;
	float							m_fTimeScale;
	ANIMATIONCBUFFER				m_tCBuffer;
	float							m_fTransitionTime;
	float							m_fTransitionTimeLimit;
	std::vector<Matrix>				m_vecFinalBone;
	class CSceneComponent*			m_pOwner;
	bool							m_bStop;
	bool							m_bStart;
	bool							m_bUpdate;
	ID3D11Buffer*					m_pBoneBuffer;
	ID3D11ShaderResourceView*		m_pBoneSRV;
	ID3D11UnorderedAccessView*		m_pBoneUAV;
	class CAnimationSequence*		m_pBlendSequence;
	float							m_fBlendTime;

public:
	void SetOwner(class CSceneComponent* pCom);
	bool HasOwner()	const;
	bool IsStart()	const;
	bool IsEnd()	const;
	class CSkeleton* GetSkeleton()	const;
	void SetTimeScale(float fScale);

public:
	bool AddAnimationSequence(const std::string& strKey, const std::string& strSeq);
	void ChangeSequence(const std::string& strKey);
	void SetPlayTime(const std::string& strKey, float fTime);
	class CAnimationSequence* GetSequence()	const;
	void SetBlendSequence(const std::string& strKey);
	void DeleteBlendSequence();
	class CAnimationSequence* GetBlendSequence()	const;

private:
	class CAnimationSequence* FindSequence(const std::string& strKey);

public:
	bool Init();
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	CAnimation* Clone();

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

public:
	static CAnimation* LoadAnimation(FILE* pFile);

public:
	void SetShaderResource();
	void ResetShaderResource();

public:
	void AddSocket(const std::string& strSocket, const std::string& strBone);
	void SetSocketObject(const std::string& strSocket, const std::shared_ptr<class CGameObject>& pObj);
	void SetSocketObject(const std::string& strSocket, class CGameObject* pObj);
	void SetSocketComponent(const std::string& strSocket, const std::shared_ptr<class CComponent>& pCom);
	void SetSocketOffset(const std::string& strSocket, const Vector3& vOffset);
	void SetSocketRotation(const std::string& strSocket, const Vector3& vRot);
	void DeleteSocketObject(const std::string& strSocket, const std::shared_ptr<class CGameObject>& pObj);
	void DeleteSocketObject(const std::string& strSocket, class CGameObject* pObj);
	void DeleteSocketComponent(const std::string& strSocket, const std::shared_ptr<class CComponent>& pCom);
	void UpdateSocket(float fTime);

private:
	class CBoneSocket* FindSocket(const std::string& strKey);

public:
	void SetSkeleton(const std::string& strBone);
	bool LoadSkeleton(const std::string& strBone, const char* pFileName, const std::string& strPathKey = MESH_PATH);
	bool LoadSkeletonFromFull(const std::string& strBone, const char* pFullPath);

public:
	void AddTransition(const std::string& strTransition, const std::string& strSeq, bool(*pFunc)(),const std::string& strNextSeq);


	template <typename T>
	void AddTransition(const std::string& strTransition, const std::string& strSeq, 
		T* pObj, bool(T::* pFunc)(), const std::string& strNextSeq)
	{
		CAnimationSequence* pSequence = FindSequence(strSeq);

		if (!pSequence)
			return;

		typename::std::list<struct _tagAnimationTransition*>::iterator iter = pSequence->m_TransitionList.begin();
		typename::std::list<struct _tagAnimationTransition*>::iterator iterEnd = pSequence->m_TransitionList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->strName == strTransition)
			{
				SAFE_RELEASE(pSequence);
				return;
			}
		}

		CAnimationSequence* pNextSequence = FindSequence(strNextSeq);

		if (!pNextSequence)
		{
			SAFE_RELEASE(pSequence);
			return;
		}

		struct _tagAnimationTransition* pTransition = new _tagAnimationTransition;

		pTransition->pNextSequence = pNextSequence;
		pTransition->strName = strTransition;
		pTransition->strSequence = strSeq;
		pTransition->vecTransitionFunc.push_back(std::bind(pFunc, pObj));
		
		pSequence->m_TransitionList.push_back(pTransition);

		SAFE_RELEASE(pSequence);
		SAFE_RELEASE(pNextSequence);
	}
	void AutoTransition(const std::string& strTransition, const std::string& strSequence, const std::string& strNext);
	void AddTransitionNotify(const std::string& strTransition, const std::string& strSequence, void(*pFunc)());
	template <typename T>
	void AddTransitionNotify(const std::string& strTransition, const std::string& strSequence, T* pObj, void(T::*pFunc)())
	{
		CAnimationSequence* pSequence = FindSequence(strSequence);

		if (!pSequence)
			return;

		std::list<PANIMATIONTRANSITION>::iterator iter = pSequence->m_TransitionList.begin();
		std::list<PANIMATIONTRANSITION>::iterator iterEnd = pSequence->m_TransitionList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->strName == strTransition)
			{
				(*iter)->NotifyFuncList.push_back(std::bind(pFunc, pObj));
				SAFE_RELEASE(pSequence);
				return;
			}
		}
		SAFE_RELEASE(pSequence);
	}

public:
	void CreateNotify(const std::string& strNotify, const std::string& strSequence, float fTime, float fEndTime = -1);
	void AddNotifyFunction(const std::string& strNotify, void(*pFunc)(float));
	template <typename T>
	void AddNotifyFunction(const std::string& strNotify, T* pObj, void(T::* pFunc)(float))
	{
		PANIMATIONNOTIFY pNotify = FindNotify(strNotify);

		if (!pNotify)
			return;

		pNotify->vecNotify.push_back(std::bind(pFunc, pObj, std::placeholders::_1));
	}
	void AddTimeLineNotifyFunction(const std::string& strNotify, void(*pFunc)(float,float));
	template <typename T>
	void AddTimeLineNotifyFunction(const std::string& strNotify, T* pObj, void(T::* pFunc)(float, float))
	{
		PANIMATIONNOTIFY pNotify = FindNotify(strNotify);

		if (!pNotify)
			return;

		pNotify->vecTimeLineNotify.push_back(std::bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2));
	}

private:
	PANIMATIONNOTIFY FindNotify(const std::string& strKey);

public:
	template <typename T>
	static T* CreateAnimation(const std::string& strKey, const std::string& strBone = "",
		const char* pFilePath = "", const std::string& strPathKey = MESH_PATH)
	{
		T* pAni = new T;

		pAni->SetName(strKey);

		if (!pAni->Init())
		{
			SAFE_RELEASE(pAni);
			return nullptr;
		}

		if (pFilePath)
		{
			pAni->LoadSkeleton(strBone, pFilePath, strPathKey);
		}

		return pAni;
	}
	template <typename T>
	static T* CreateAnimationFullPath(const std::string& strKey, const std::string& strBone = "",
		const char* pFullPath = "")
	{
		T* pAni = new T;

		pAni->SetName(strKey);

		if (!pAni->Init())
		{
			SAFE_RELEASE(pAni);
			return nullptr;
		}

		if (pFullPath)
		{
			pAni->LoadSkeletonFromFull(strBone, pFullPath);
		}

		return pAni;
	}

public:
	void AddEndCallBack(const std::string& strSequence, void(pFunc)());

	template <typename T>
	void AddEndCallBack(const std::string& strSequence, T* pObj, void(T::* pFunc)())
	{
		CAnimationSequence* pSequence = FindSequence(strSequence);

		if (!pSequence)
			return;

		pSequence->AddEndCallBack<T>(pObj, pFunc);

		SAFE_RELEASE(pSequence);
	}
	
public:
	void ShowWindow();

public:
	void Pause();
	void CreateIndentirySequence(int iFrame);
};