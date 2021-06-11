#pragma once	//	�� ����� �� ���� �����Ѵ�.

#include "../Ref.h"	//	���� ������ ���۷��� ī��Ʈ ����� �����Ѵ�.
#include "AnimationSequence.h"


typedef struct _tagAnimationTransition	//	�ִϸ��̼� ���� ����ü�� �����Ѵ�.
{
	std::string		strName;	//	�̸��� �����Ѵ�.
	std::string		strInfoName;	//	
	std::function<bool()>					TransitionFunc;	//	���� �Լ��� �����Ѵ�.
	class CAnimationSequence* pSequence;
	bool			bAutoTransition;	//	�ڵ� ���� bool ������ �����Ѵ�.
	std::list<std::function<void()>>			NotifyList;	//	��Ƽ���� �Լ� ����Ʈ�� �����Ѵ�.

	_tagAnimationTransition()	//	�⺻ �����ڸ� �����Ѵ�.
	{
		bAutoTransition = false;	//	�ڵ� ���̸� false�� �����Ѵ�.
	}
}AnimationTransition, * PAnimationTransition;	//	AnimationTransition���� �����Ѵ�. �����ʹ� PAnimationTransition���� �����Ѵ�.

typedef struct _tagAnimationSequenceInfo	//	�ִϸ��̼� ������ ���� ����ü�� �����Ѵ�.
{
	std::string		strName;	//	�̸��� �����Ѵ�.
	std::shared_ptr<class CAnimationSequence> pSequence;	//	�ִϸ��̼� ������ Ÿ���� ���� ������ ������ �����Ѵ�.
	std::list<PAnimationTransition>		TransitionList;	//	�ִϸ��̼� ���� ����ü �����͸� ����Ʈ�� ���´�.
}AnimationSequenceInfo, * PAnimationSequenceInfo;	//	AnimationSequenceInfo �� �����Ѵ�. �����͸� PAnimationSequenceInfo�� �����Ѵ�.

typedef struct _tagAnimNotify	//	�ִϸ��̼� ��Ƽ���� ����ü�� �����Ѵ�.
{
	std::string		strName;	//	���ڿ��� �����Ѵ�.
	class CAnimationSequence*	pSequence;
	std::vector<std::function<void(float)>>	vecFunction;	//	float������ �Ű������� �޴� �Լ� Ÿ���� ���͸� �����Ѵ�.
	float		fTime;	//	�ð��� �����Ѵ�.
	bool		bCall;	//	�ҷȴ��� ������ �����Ѵ�.

	_tagAnimNotify() :	//	�⺻�����ڸ� �����Ѵ�.
		bCall(false),	//	�ҷȴ����� false�� �ʱ�ȭ�Ѵ�.
		fTime(0.f)	//	�ð��� 0���� �ʱ�ȭ�Ѵ�.
	{
	}
}AnimNotify, * PAnimNotify;	//	AnimNotify�� �����Ѵ�. �����͸� PAnimNotify�� �����Ѵ�.

class CAnimation :	//	�ִϸ��̼� Ŭ������ �����Ѵ�.
	public CRef	//	���۷��� Ŭ������ ��ӹ޴´�.
{
	friend class CAnimationMeshComponent;	//	�ִϸ��̼� �Ž� ������Ʈ Ŭ������ private ��������� �ǰ��Ѵ�.
	friend class CMesh;	//	�Ž� Ŭ������ private ��������� �ǰ��Ѵ�.
	friend class CRenderer;

public:	//	�ܺο��� ������ �Ǵ� �����.
	template <typename T>	//	TŸ���� Ŭ���� �Լ���.
	static std::shared_ptr<T> CreateAnimation(const std::string& strName,	//	�������� �ִϸ��̼��� �����ϴ� �Լ��� �����Ѵ�. �Ű������� ���� ������ �ȵǴ� ���ڿ� ������,
		class CScene* pScene,	//	��� ������,
		const std::string& strBoneName = "", const char* pBoneFileName = nullptr,	//	���̸�, �� �����̸�, ��� Ű�� �޴´�.
		const std::string& strBonePathName = MESH_PATH)
	{
		std::shared_ptr<T> pAnimation = std::shared_ptr<T>(new T);	//	TŸ���� ������ ���� �Ҵ��Ѵ�.

		pAnimation->m_pScene = pScene;	//	�ִϸ��̼��� ����� �Ű����� pScene���� �����Ѵ�.
		pAnimation->SetName(strName);	//	�ִϸ��̼��� �̸��� �Ű����� �̸����� �����Ѵ�.

		if (!pAnimation->Init())	//	�ִϸ��̼��� �ʱ�ȭ�Ѵ�.
		{
			return nullptr;	//	nullptr�� ��ȯ�Ѵ�.
		}

		if (pBoneFileName)	//	�� ���� �̸��� ���� ����.
		{
			pAnimation->LoadSkeleton(strBoneName, pBoneFileName,	//	�ִϸ��̼��� ���̷����� �ε� �ϴ� �Լ��� ȣ���Ѵ�. �Ű������� �Ű����� ���̸�, �Ű����� �������̸�, �Ű����� �� ��� Ű�� �����Ѵ�.
				strBonePathName);
		}

		m_mapAni.insert(std::make_pair(strName, pAnimation));

		return pAnimation;
	}
	
	template <typename T>	//	TŸ���� Ŭ���� �Լ���.
		static std::shared_ptr<T> CreateAnimationFromFull(const std::string& strName,	//	�������� �ִϸ��̼��� �����ϴ� �Լ��� �����Ѵ�. �Ű������� ���� ������ �ȵǴ� ���ڿ� ������,
			class CScene* pScene,	//	��� ������,
			const std::string& strBoneName, const char* pBoneFileName)
	{
		std::shared_ptr<T> pAnimation = std::shared_ptr<T>(new T);	//	TŸ���� ������ ���� �Ҵ��Ѵ�.

		pAnimation->m_pScene = pScene;	//	�ִϸ��̼��� ����� �Ű����� pScene���� �����Ѵ�.
		pAnimation->SetName(strName);	//	�ִϸ��̼��� �̸��� �Ű����� �̸����� �����Ѵ�.

		if (!pAnimation->Init())	//	�ִϸ��̼��� �ʱ�ȭ�Ѵ�.
		{
			return nullptr;	//	nullptr�� ��ȯ�Ѵ�.
		}

		if (pBoneFileName)	//	�� ���� �̸��� ���� ����.
		{
			pAnimation->LoadSkeletonFullPath(strBoneName, pBoneFileName);
		}

		m_mapAni.insert(std::make_pair(strName, pAnimation));

		return pAnimation;
	}

public:	//	������ Ŭ�������� ������ �Ǵ� ������̴�.
	CAnimation();	//	�⺻�����ڸ� �����Ѵ�.
	CAnimation(const CAnimation& com);	//	��������ڸ� �����Ѵ�. �Ű������� ���� ������ �Ұ��� �ִϸ��̼� ������ ������ �޴´�.
	virtual ~CAnimation();	//	�������� �Ҹ��ڸ� �����Ѵ�.

private:	//	�ܺο��� ������ �ȵǴ� ������̴�.
	std::unordered_map<size_t, std::shared_ptr<class CAnimationSequence>>	m_mapSequence;
	class CAnimationSequence*				m_pSequence;	//	�ִϸ��̼� ������ ���� ����ü �����͸� �����Ѵ�.
	std::shared_ptr<class CSkeleton>						m_pSkeleton;	//	���̷��� �����͸� �����Ѵ�.
	class CSceneComponent* m_pOwner;	//	�ִϸ��̼��� ������ ��� ������Ʈ�� �����Ѵ�.
	Vector3											m_vBlendScale;	//	���� �������� �����Ѵ�.
	Vector4											m_vBlendRot;	//	���� ȸ���� �����Ѵ�.
	Vector3											m_vBlendPos;	//	���� ��ġ�� �����Ѵ�.
	bool													m_bEnd;	//	���� ��Ÿ���� bool������ �����Ѵ�.
	float													m_fAnimationGlobalTime;	//	�ִϸ��̼� ���� Ÿ���� �����Ѵ�.
	float													m_fClipProgress;	//	Ŭ�� ���൵�� �����Ѵ�.
	float													m_fChangeTime;	//	���� �ð��� �����Ѵ�.
	float													m_fChangeLimitTime;	//	����ð� �ִ밪�� �����Ѵ�.
	std::vector<PAnimNotify>						m_vecNotify;	//	�ִϸ��̼� ��Ƽ���� ����ü ������ Ÿ���� ��Ƽ���� ���͸� �����Ѵ�.
	PAnimationTransition						m_pTransition;	//	�ִϸ��̼� ���� ����ü �����ͺ����� �����Ѵ�.
	std::list<class CBoneSocket*>				m_SocketList;	//	�� ��Ĺ ������ Ÿ���� ����Ʈ�� �����Ѵ�.
	class CScene* m_pScene;	//	��� �����͸� �����Ѵ�.
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

	bool LoadSkeleton(const std::string& strName, const char* pFileName,	//	bool ������ ��ȯ�ϴ� ���̷����� �ε��ϴ� �Լ��� �����Ѵ�. �Ű������� ���� ������ �ȵǴ� ���ڿ� ������, ���� ������ �ȵǴ� ���� ������,
		const std::string& strPathName = MESH_PATH);	//	���� ������ �ȵǴ� ���ڿ� �������� �޴´�.
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

public:	//	�ܺο��� ������ �Ǵ� ������̴�.
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);	//	�������� ����� �����Լ��� �����Ѵ�. �Ű������� float������ �޴´�.
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

