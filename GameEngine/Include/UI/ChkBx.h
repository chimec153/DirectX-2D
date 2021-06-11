#pragma once
#include "UIControl.h"

enum class CHECK_STATE
{
	OFF,
	ON,
	END
};

class CChkBx :
	public CUIControl
{
	friend class CGameObject;

public:
	CChkBx();
	CChkBx(const CChkBx& ui);
	virtual ~CChkBx();

private:
	std::vector<std::shared_ptr<CTexture>>		m_vecTex;
	bool										m_bOn;
	std::vector<std::function<void(float)>>		m_CallBack[(int)CHECK_STATE::END];
	std::vector<std::function<void(CChkBx*)>>	m_BoxCallBack[(int)CHECK_STATE::END];
	std::string									m_vecSound[(int)CHECK_STATE::END];
	std::shared_ptr<CComponent>					m_pRC;

public:
	bool IsOn()	const;
	void Check(bool bOn);
	void SetTexture(CHECK_STATE eState, const std::string& strKey);
	void SetTexture(CHECK_STATE eState, std::shared_ptr<CTexture> pTex);
	void SetSound(CHECK_STATE eState, const std::string& strKey);
	void Play();
	std::string GetSound(CHECK_STATE eStat)	const;


public:
	void SetCallBack(CHECK_STATE eState, void(*pFunc)(float));

	template <typename T>
	void SetCallBack(CHECK_STATE eState, T* pObj, void(T::* pFunc)(float))
	{
		m_CallBack[(int)eState].push_back(std::bind(pFunc, pObj, std::placeholders::_1));
	}
	void SetBoxCallBack(CHECK_STATE eState, void(*pFunc)(CChkBx*));

	template <typename T>
	void SetBoxCallBack(CHECK_STATE eState, T* pObj, void(T::* pFunc)(CChkBx*))
	{
		m_BoxCallBack[(int)eState].push_back(std::bind(pFunc, pObj, std::placeholders::_1));
	}
	void Call(float);
	void BoxCall(CChkBx* pBx);

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
	void ColEnter(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);

};

