#pragma once
#include "Object\GameObject.h"
#include "../EditorFlags.h"
class CUI :
	public CGameObject
{
public:
	CUI();
	CUI(const CUI& ui);
	virtual ~CUI();

private:
	int m_iValue;
	int m_iMax;
	Vector2 m_vScaleMax;
	std::shared_ptr<CComponent>	m_pBar;
	std::shared_ptr<CComponent>	m_pMPBar;
	std::shared_ptr<CComponent>	m_pExpBar;
	std::shared_ptr<CComponent>	m_pFont;
	std::shared_ptr<CComponent>	m_pTag;
	std::shared_ptr<CComponent>	m_pSkillIcon[static_cast<int>(SKILL_TYPE::END)];
	class CPlayer3D*	m_pPlayer;
	std::shared_ptr<CComponent>	m_pInventoryButton;
	std::shared_ptr<CComponent>	m_pEquipButton;
	std::shared_ptr<CComponent>	m_pBloodHUD;
	std::shared_ptr<CComponent>	m_pMsgHUD;
	std::shared_ptr<CComponent>	m_pBlinkHUD;
	float				m_fBloodAlpha;
	std::shared_ptr<CComponent>	m_pCastingBar;
	std::shared_ptr<CComponent>	m_pCastingFont;
	std::shared_ptr<CComponent>	m_pCastingBack;
	float						m_fBackAlpha;
	std::shared_ptr<CComponent>	m_pBackImage;
	std::shared_ptr<CComponent>	m_pDieButton;
	std::shared_ptr<CComponent>	m_pChat;
	std::shared_ptr<CComponent>	m_pChatWindowSmall;

public:
	void AddValue(int iValue);
	void SetValue(int iValue);
	void SetMax(int iMax)
	{
		m_iMax = iMax;
	}
	void SetScaleMax(const Vector2& vScale)
	{
		m_vScaleMax = vScale;
	}
	void SetPlayer(class CPlayer3D* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	void SetCastingBar(bool bOn);

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
	void DieButtonCallBack(float fTime);
	void SetCharText(const TCHAR* pText);
	void SetChatWindowSmall(bool bEnable);
};

