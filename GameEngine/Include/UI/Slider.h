#pragma once
#include "UIControl.h"

enum class SLIDER_TYPE
{
	HORIZONTAL,
	VERTICAL
};

class CSlider :
	public CUIControl
{
	friend class CGameObject;

public:
	CSlider();
	CSlider(const CSlider& ui);
	virtual ~CSlider();

private:
	std::vector<std::shared_ptr<CTexture>>	m_vecTexture;
	std::vector<Vector4>					m_vecColor;
	BUTTON_STATE							m_eState;
	std::vector<std::function<void(float)>>	m_vecCallBack[(int)BUTTON_STATE::END];
	SLIDER_TYPE								m_eSliderType;
	float									m_fScroll;
	Vector3									m_vStartPos;
	float									m_fLength;
	float									m_fPercent;
	std::shared_ptr<CComponent>				m_pRC;
	std::shared_ptr<CComponent>				m_pTopButton;
	std::shared_ptr<CComponent>				m_pBackButton;
	std::shared_ptr<CComponent>				m_pBotButton;

public:
	void SetCallBack(BUTTON_STATE eState, void(*pFunc)(float));

	template <typename T>
	void SetCallBack(BUTTON_STATE eState, T* pObj, void(T::* pFunc)(float))
	{
		m_vecCallBack[(int)eState].push_back(std::bind(pFunc, pObj, std::placeholders::_1));
	}

	void SetTexture(BUTTON_STATE eState, const std::string& strKey);
	void SetTexture(BUTTON_STATE eState, std::shared_ptr<CTexture> pTex);
	void SetColor(BUTTON_STATE eState, const Vector4& vColor);
	void SetSliderType(SLIDER_TYPE eType);
	SLIDER_TYPE GetSliderType()	const;

	void Call(float fTime);

public:
	void SetLength(float fLength);
	float GetPercent()	const;
	void AddSlider(float f);
	void SubSlider(float f);
	void JumpSlider(float f);

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

