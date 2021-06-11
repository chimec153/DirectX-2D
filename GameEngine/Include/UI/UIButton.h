#pragma once
#include "UIControl.h"

typedef struct _tagButtonInfo
{
	std::shared_ptr<CTexture>	pTex;
	Vector4			vDif;

	_tagButtonInfo()	:
		pTex(nullptr)
	{
		vDif = Vector4::White;
	}

}ButtonInfo, *PButtonInfo;

class CUIButton :
	public CUIControl
{
	friend class CGameObject;

public:
	CUIButton();
	CUIButton(const CUIButton& ui);
	virtual ~CUIButton();

private:
	BUTTON_STATE							m_eState;
	std::vector<ButtonInfo>					m_vecInfo;
	std::vector<std::function<void(float)>>	m_vecCallBack[(int)BUTTON_STATE::END];
	std::vector<std::function<void(CUIButton*)>>	m_vecBtnCallBack[(int)BUTTON_STATE::END];
	std::shared_ptr<CComponent>				m_pRC;

public:
	void SetTexture(BUTTON_STATE eState, std::shared_ptr<CTexture> pTex);
	void SetTexture(BUTTON_STATE eState, const std::string& strKey);
	void SetColor(BUTTON_STATE eState, const Vector4& vColor);
	void SetColor(BUTTON_STATE eState, float r, float g, float b, float a);
	BUTTON_STATE GetState()	const;

	void SetCallBack(BUTTON_STATE eState, void(*pFunc)(float));
	template <typename T>
	void SetCallBack(BUTTON_STATE eState, T* pObj, void(T::* pFunc)(float))
	{
		m_vecCallBack[(int)eState].push_back(std::bind(pFunc, pObj, std::placeholders::_1));
	}

	void SetButtonCallBack(BUTTON_STATE eState, void(*pFunc)(CUIButton*));
	template <typename T>
	void SetButtonCallBack(BUTTON_STATE eState, T* pObj, void(T::* pFunc)(CUIButton*))
	{
		m_vecBtnCallBack[(int)eState].push_back(std::bind(pFunc, pObj, std::placeholders::_1));
	}

	void Call(float fTime);
	void CallButton(CUIButton* pBtn);

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

