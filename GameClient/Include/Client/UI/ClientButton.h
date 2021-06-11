#pragma once

#include "UI/UIObject.h"
#include "UI/UIButton.h"

class CClientButton :
	public CUIObject
{
	friend class CScene;

public:
	CClientButton();
	CClientButton(const CClientButton& btn);
	virtual ~CClientButton();

private:
	std::function<void(float)>	m_CallBack[(int)BUTTON_STATE::END];
	std::shared_ptr<CComponent>			m_pButton;
	std::shared_ptr<CComponent>				m_pImg;
	std::vector<std::shared_ptr<CTexture>>	m_vecTexture;

public:
	void SetTexture(BUTTON_STATE eState, std::shared_ptr<CTexture> pTex);
	void SetTexture(BUTTON_STATE eState, const std::string& strTag);
	void SetColor(BUTTON_STATE eState, const Vector4& vClr);
	void SetColor(BUTTON_STATE eState, BYTE r, BYTE g, BYTE b, BYTE a);
	void SetColor(BUTTON_STATE eState, float r, float g, float b, float a);
	void SetSound(BUTTON_STATE eStat, SOUND_TYPE eType, const std::string& strKey);
	void SetImgTexture(BUTTON_STATE eStat, std::shared_ptr<CTexture> pTex);
	void SetImgTexture(BUTTON_STATE eStat, const std::string& strKey);
	void ChangeImg(float fTime);
	void SetUIImage();
	class CUIButton* GetButton()	const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CGameObject> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void ColBgn(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);

public:
	void SetClickCallBack(BUTTON_STATE eStat, void(*pFunc)(float));

	template <typename T>
	void SetClickCallBack(T* pObj, BUTTON_STATE eStat, void(T::* pFunc)(float))
	{
		((CUIButton*)m_pButton.get())->SetCallBack<T>(eStat, pObj, pFunc);
	}
};