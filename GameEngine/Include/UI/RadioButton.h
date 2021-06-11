#pragma once
#include "UIControl.h"
#include "../Resource/Texture.h"

enum class RADIO_STATE
{
	OFF,
	ON,
	END
};

class CRadioButton :
	public CUIControl
{
	friend class CGameObject;

public:
	CRadioButton();
	CRadioButton(const CRadioButton& ui);
	virtual ~CRadioButton();

private:
	std::vector<std::shared_ptr<CTexture>>		m_vecTex;
	RADIO_STATE									m_eState;
	std::list<class CRadioButton*>				m_ButtonList;
	std::vector<std::function<void(float)>>		m_CallBack[(int)RADIO_STATE::END];
	std::string									m_vecSound[(int)RADIO_STATE::END];
	std::shared_ptr<CComponent>			m_pCC;

public:
	RADIO_STATE GetState()	const;
	void SetState(RADIO_STATE eState);
	void SetTexture(RADIO_STATE eState, std::shared_ptr<CTexture> pTex);
	void SetTexture(RADIO_STATE eState, const std::string& strKey);
	void AddButton(class CRadioButton* pBtn);
	void DeleteButton(class CRadioButton* pBtn);
	void SetSound(RADIO_STATE eState, const std::string& strKey);
	void Play(float fTime);
	bool HasButton(class CRadioButton* pBtn);

public:
	void SetCallBack(RADIO_STATE eState, void(*pFunc)(float));

	template <typename T>
	void SetCallBack(RADIO_STATE eState, T* pObj, void(T::* pFunc)(float))
	{
		m_CallBack[(int)eState].push_back(std::bind(pFunc, pObj, std::placeholders::_1));
	}

	void Call(RADIO_STATE eState, float fTime);

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

