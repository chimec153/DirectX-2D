#pragma once	//	이 헤더를 한 번만 포함한다.

#include "../Ref.h"	//	상위 폴더의 레퍼런스 카운트 헤더를 포함한다.
#include "AnimationSequence.h"


typedef struct _tagAnimationTransition	//	애니메이션 전이 구조체를 정의한다.
{
	std::string		strName;	//	이름을 선언한다.
	std::string		strInfoName;	//	
	std::function<bool()>					TransitionFunc;	//	전이 함수를 선언한다.
	class CAnimationSequence* pSequence;
	bool			bAutoTransition;	//	자동 전이 bool 변수를 선언한다.
	std::list<std::function<void()>>			NotifyList;	//	노티파이 함수 리스트를 선언한다.

	_tagAnimationTransition()	//	기본 생성자를 정의한다.
	{
		bAutoTransition = false;	//	자동 전이를 false로 설정한다.
	}
}AnimationTransition, * PAnimationTransition;	//	AnimationTransition으로 정의한다. 포인터는 PAnimationTransition으로 정의한다.

typedef struct _tagAnimationSequenceInfo	//	애니메이션 시퀀스 정보 구조체를 정의한다.
{
	std::string		strName;	//	이름을 선언한다.
	std::shared_ptr<class CAnimationSequence> pSequence;	//	애니메이션 시퀀스 타입의 공유 포인터 변수를 선언한다.
	std::list<PAnimationTransition>		TransitionList;	//	애니메이션 전이 구조체 포인터를 리스트로 갖는다.
}AnimationSequenceInfo, * PAnimationSequenceInfo;	//	AnimationSequenceInfo 로 정의한다. 포인터를 PAnimationSequenceInfo로 정의한다.

typedef struct _tagAnimNotify	//	애니메이션 노티파이 구조체를 선언한다.
{
	std::string		strName;	//	문자열을 선언한다.
	class CAnimationSequence*	pSequence;
	std::vector<std::function<void(float)>>	vecFunction;	//	float변수를 매개변수로 받는 함수 타입의 벡터를 선언한다.
	float		fTime;	//	시간을 선언한다.
	bool		bCall;	//	불렸는지 변수를 선언한다.

	_tagAnimNotify() :	//	기본생성자를 정의한다.
		bCall(false),	//	불렸는지를 false로 초기화한다.
		fTime(0.f)	//	시간을 0으로 초기화한다.
	{
	}
}AnimNotify, * PAnimNotify;	//	AnimNotify로 정의한다. 포인터를 PAnimNotify로 정의한다.

class CAnimation :	//	애니메이션 클래스를 정의한다.
	public CRef	//	레퍼런스 클래스를 상속받는다.
{
	friend class CAnimationMeshComponent;	//	애니메이션 매쉬 컴포넌트 클래스의 private 멤버접근이 되게한다.
	friend class CMesh;	//	매쉬 클래스의 private 멤버접근이 되게한다.
	friend class CRenderer;

public:	//	외부에서 접근이 되는 멤버다.
	template <typename T>	//	T타입의 클래스 함수다.
	static std::shared_ptr<T> CreateAnimation(const std::string& strName,	//	정적으로 애니메이션을 생성하는 함수를 정의한다. 매개변수로 값의 변경이 안되는 문자열 참조형,
		class CScene* pScene,	//	장면 포인터,
		const std::string& strBoneName = "", const char* pBoneFileName = nullptr,	//	본이름, 본 파일이름, 경로 키를 받는다.
		const std::string& strBonePathName = MESH_PATH)
	{
		std::shared_ptr<T> pAnimation = std::shared_ptr<T>(new T);	//	T타입의 변수를 동적 할당한다.

		pAnimation->m_pScene = pScene;	//	애니메이션의 장면을 매개변수 pScene으로 설정한다.
		pAnimation->SetName(strName);	//	애니메이션의 이름을 매개변수 이름으로 설정한다.

		if (!pAnimation->Init())	//	애니메이션을 초기화한다.
		{
			return nullptr;	//	nullptr를 반환한다.
		}

		if (pBoneFileName)	//	본 파일 이름이 있을 경우다.
		{
			pAnimation->LoadSkeleton(strBoneName, pBoneFileName,	//	애니메이션의 스켈레톤을 로드 하는 함수를 호출한다. 매개변수로 매개변수 본이름, 매개변수 본파일이름, 매개변수 본 경로 키를 전달한다.
				strBonePathName);
		}

		m_mapAni.insert(std::make_pair(strName, pAnimation));

		return pAnimation;
	}
	
	template <typename T>	//	T타입의 클래스 함수다.
		static std::shared_ptr<T> CreateAnimationFromFull(const std::string& strName,	//	정적으로 애니메이션을 생성하는 함수를 정의한다. 매개변수로 값의 변경이 안되는 문자열 참조형,
			class CScene* pScene,	//	장면 포인터,
			const std::string& strBoneName, const char* pBoneFileName)
	{
		std::shared_ptr<T> pAnimation = std::shared_ptr<T>(new T);	//	T타입의 변수를 동적 할당한다.

		pAnimation->m_pScene = pScene;	//	애니메이션의 장면을 매개변수 pScene으로 설정한다.
		pAnimation->SetName(strName);	//	애니메이션의 이름을 매개변수 이름으로 설정한다.

		if (!pAnimation->Init())	//	애니메이션을 초기화한다.
		{
			return nullptr;	//	nullptr를 반환한다.
		}

		if (pBoneFileName)	//	본 파일 이름이 있을 경우다.
		{
			pAnimation->LoadSkeletonFullPath(strBoneName, pBoneFileName);
		}

		m_mapAni.insert(std::make_pair(strName, pAnimation));

		return pAnimation;
	}

public:	//	유도된 클래스에서 접근이 되는 멤버들이다.
	CAnimation();	//	기본생성자를 선언한다.
	CAnimation(const CAnimation& com);	//	복사생성자를 선언한다. 매개변수로 값의 변경이 불가한 애니메이션 참조형 변수를 받는다.
	virtual ~CAnimation();	//	가상으로 소멸자를 선언한다.

private:	//	외부에서 접근이 안되는 멤버들이다.
	std::unordered_map<size_t, std::shared_ptr<class CAnimationSequence>>	m_mapSequence;
	class CAnimationSequence*				m_pSequence;	//	애니메이션 시퀀스 정보 구조체 포인터를 선언한다.
	std::shared_ptr<class CSkeleton>						m_pSkeleton;	//	스켈레톤 포인터를 선언한다.
	class CSceneComponent* m_pOwner;	//	애니메이션을 가지는 장면 컴포넌트를 선언한다.
	Vector3											m_vBlendScale;	//	블랜드 스케일을 선언한다.
	Vector4											m_vBlendRot;	//	블랜드 회전을 선언한다.
	Vector3											m_vBlendPos;	//	블랜드 위치를 선언한다.
	bool													m_bEnd;	//	끝을 나타내는 bool변수를 선언한다.
	float													m_fAnimationGlobalTime;	//	애니메이션 전역 타임을 선언한다.
	float													m_fClipProgress;	//	클립 진행도를 선언한다.
	float													m_fChangeTime;	//	변경 시간을 선언한다.
	float													m_fChangeLimitTime;	//	변경시간 최대값을 선언한다.
	std::vector<PAnimNotify>						m_vecNotify;	//	애니메이션 노티파이 구조체 포인터 타입의 노티파이 벡터를 선언한다.
	PAnimationTransition						m_pTransition;	//	애니메이션 전이 구조체 포인터변수를 선언한다.
	std::list<class CBoneSocket*>				m_SocketList;	//	본 소캣 포인터 타입의 리스트를 선언한다.
	class CScene* m_pScene;	//	장면 포인터를 선언한다.
	bool		m_bStop;
	float		m_fTimeScale;

private:
	ANIMATIONCBUFFER			m_tCBuffer;
	std::shared_ptr<class CShader>	m_pUpdateShader;
	PSTRUCTUREDBUFFER			m_pStructuredBuffer;
	ID3D11Buffer*				m_pAnimBuffer;
	ID3D11UnorderedAccessView*	m_pUAV;
	std::vector<Matrix>			m_vecBone;

public:
	class CAnimationSequence*	GetSequence()	const;

public:
	void SetPlayTime(const std::string& strInfoName, float fPlayTime);
	bool AddAnimationSequence(const std::string& strInfoName, const std::string& strSequenceName);
	void ChangeSequence(const std::string& strKey);
	void AddTransition(const std::string& strTransitionName, const std::string& strName,
		bool(*pFunc)(), const std::string& strNextName);
	void AutoTransition(const std::string& strInfoName, const std::string& strName, const std::string& strNextName);
	template <typename T>
	void AddTransition(const std::string& strTransitionName, const std::string& strName,
		T* pObj, bool(T::* pFunc)(), const std::string& strNextName)
	{
		std::shared_ptr<class CAnimationSequence>	pInfo = FindAnimSequence(strName);

		if (!pInfo)
			return;

		std::shared_ptr<class CAnimationSequence>	pSequence = FindAnimSequence(strNextName);

		if (!pSequence)
			return;

		PAnimationTransition	pTransition = new AnimationTransition;

		pTransition->pSequence = pSequence.get();
		pTransition->TransitionFunc = std::bind(pFunc, pObj);
		pTransition->strName = strTransitionName;
		pTransition->strInfoName = strName;

		pInfo->m_TransitionList.push_back(pTransition);
	}

	void AddTransitionNotify(const std::string& strName, const std::string& strInfoName, void(*pFunc)());
	template <typename T>
	void AddTransitionNotify(const std::string& strName, const std::string& strInfoName, T* pObj,
		void(T::* pFunc)())
	{
		std::shared_ptr<class CAnimationSequence>	pInfo = FindAnimSequence(strInfoName);

		if (!pInfo)
			return;

		auto	iter = pInfo->m_TransitionList.begin();
		auto	iterEnd = pInfo->m_TransitionList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->strName == strName)
			{
				(*iter)->NotifyList.push_back(std::bind(pFunc, pObj));
				break;
			}
		}
	}

	bool LoadSkeleton(const std::string& strName, const char* pFileName,	//	bool 변수를 반환하는 스켈레톤을 로드하는 함수를 선언한다. 매개변수로 값의 변경이 안되는 문자열 참조형, 값의 변경이 안되는 문자 포인터,
		const std::string& strPathName = MESH_PATH);	//	값의 변경이 안되는 문자열 참조형을 받는다.
	bool LoadSkeletonFullPath(const std::string& strName, const char* pFullPath);
	bool AddSocket(const std::string& strSocketName, const std::string& strBoneName);
	bool SetSocketObject(const std::string& strSocketName, const std::shared_ptr<class CGameObject>& pObj);
	bool SetSocketObject(const std::string& strSocketName, const std::shared_ptr<class CComponent>& pCom);
	bool DeleteSocketObject(const std::string& strSocketName, const std::shared_ptr<class CGameObject>& pObj);
	bool DeleteSocketObject(const std::string& strSocketName, const std::shared_ptr<class CComponent>& pCom);
	bool DeleteSocketObject(const std::string& strSocketName);
	bool SetSocketOffset(const std::string& strSocketName, const Vector3& vOffset);
	bool SetSocketRotation(const std::string& strSocketName, const Vector3& vRotation);
	class CBoneSocket* FindSocket(const std::string& strName);

public:	//	외부에서 접근이 되는 멤버들이다.
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);	//	가상으로 선언된 갱신함수를 선언한다. 매개변수로 float변수를 받는다.
	virtual void Render(float fTime);
	virtual std::shared_ptr<CAnimation> Clone();
	void ShowWindow();

public:
	bool CreateNotify(const std::string& strSequenceName, const std::string& strNotifyName, float fTime);
	bool AddNotifyFunction(const std::string& strNotifyName, void(*pFunc)(float));
	template <typename T>
	bool AddNotifyFunction(const std::string& strNotifyName,
		T* pObj, void(T::* pFunc)(float))
	{
		auto	iter = m_vecNotify.begin();
		auto	iterEnd = m_vecNotify.end();

		PAnimNotify	pNotify = nullptr;

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->strName == strNotifyName)
			{
				pNotify = *iter;
				break;
			}
		}

		if (!pNotify)
			return false;

		pNotify->vecFunction.push_back(std::bind(pFunc, pObj, std::placeholders::_1));

		return true;
	}

private:
	std::shared_ptr<class CAnimationSequence> FindAnimSequence(const std::string& strName);

public:
	void SetShaderResource();
	void ResetShaderResource();

public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;

public:
	static void SaveAnim(FILE* pFile);
	static void LoadAnim(FILE* pFile);

public:
	static std::unordered_map<std::string, std::shared_ptr<CAnimation>>	m_mapAni;

public:
	void Pause();
	void Play();

};

