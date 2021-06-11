#pragma once
#include "PrimitiveComponent.h"
#include "../Resource/Animation2DSequence.h"

typedef struct _tagAni2DInfo
{
	std::shared_ptr<CAnimation2DSequence> pSequence;
	float						fPlayRate;
	float						fTime;
	int							iFrame;
	bool						bLoop;
	std::function<void()>		pFunc;

	_tagAni2DInfo() :
		pSequence(nullptr),
		fPlayRate(1.f),
		fTime(0.f),
		iFrame(0),
		bLoop(true)
	{
	}
}Ani2DInfo, * PAni2DInfo;

class CSpriteComponent :
	public CSceneComponent
{
	friend class CGameObject;

public:
	CSpriteComponent();
	CSpriteComponent(const CSpriteComponent& com);
	virtual ~CSpriteComponent();

protected:
	std::unordered_map<std::string, PAni2DInfo>	m_mapAni2D;
	PAni2DInfo									m_pCurrent;
	AniCBuffer									m_tCBuffer;
	bool										m_bAryTex;
	bool										m_bStop;

public:
	void LED(bool bLED);
	int GetFrame()	const;
	PAni2DInfo GetCurrentSeq()	const;
	bool IsArrayTex()	const;
	bool IsStopped()	const;
	const PAni2DInfo& GetCurrentAni()	const;
	const _tagFrameInfo& GetFrameInfo()	const;
	void Stop();
	void Play();
	const std::unordered_map<std::string, PAni2DInfo>& GetAniSequences()	const;
	void SetSize(const Vector2& vSize);
	void SetStart(const Vector2& vSize);
	void SetEnd(const Vector2& vSize);
	void SetImageType(IMAGE_TYPE eType);

public:
	void AddAni2DInfo(const std::string& strTag, const std::string& strSeq, float fPlayRate = 1.f, bool bLoop = true);
	void AddAni2DInfo(const std::string& strTag, std::shared_ptr<CAnimation2DSequence>, float fPlayRate = 1.f, bool bLoop = true);
	PAni2DInfo FindAni2DInfo(const std::string& strTag);
	void ChangeClip(const std::string& strTag);
	void AddNotify(const std::string& strTag, const std::string& strNot, int iFrame);
	void DeleteSprite(const std::string strTag);
	void SetFrame(float fFrame);

public:
	template <typename T>
	void SetFunc(const std::string& strTag, T* pObj, void(T::* pFunc)())
	{
		PAni2DInfo pInfo = FindAni2DInfo(strTag);

		if (!pInfo)
			return;

		pInfo->pFunc = std::bind(pFunc, pObj);
	}

public:
	void CallNotify(const std::string& strTag);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CComponent> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual void ShowWindow() override;

};
