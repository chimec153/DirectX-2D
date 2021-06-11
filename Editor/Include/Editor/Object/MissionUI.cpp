#include "MissionUI.h"
#include "UI/Panel.h"
#include "UI/TitleBar.h"
#include "UI/UIButton.h"
#include "Scene/Scene.h"
#include "Tati.h"
#include "UI/UFont.h"
#include "../../Quest.h"
#include "Sound/SoundManager.h"

CMissionUI::CMissionUI()	:
	m_iSelect(-1)
	,m_iQuests(0)
{
}

CMissionUI::CMissionUI(const CMissionUI& ui)	:
	CGameObject(ui)
	,m_iSelect(ui.m_iSelect)
	, m_iQuests(ui.m_iQuests)
	, m_QuestList(ui.m_QuestList)
{
}

CMissionUI::~CMissionUI()
{
}

void CMissionUI::AddQuest(CQuest* pQuest)
{
	m_QuestList.push_back(pQuest);

	std::shared_ptr<CComponent> pCom = static_cast<CSceneComponent*>(m_vecButtons[m_iQuests].get())->FindComponentByType<CUFont>();

	TCHAR strName[MAX_PATH] = {};

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, pQuest->GetName().c_str(), -1, strName, static_cast<int>(pQuest->GetName().length()));
#else
	strcpy_s(strName, pQuest->GetName().length()+1, pQuest->GetName().c_str());
#endif

	static_cast<CUFont*>(pCom.get())->SetText(strName);

	m_vecButtons[m_iQuests++]->SetEnable(true);
}

CQuest* CMissionUI::FindQuest(int iIndex)
{
	std::list<CQuest*>::iterator iter = m_QuestList.begin();
	std::list<CQuest*>::iterator iterEnd = m_QuestList.end();

	for (int i=0; iter != iterEnd; ++iter,++i)
	{
		if (iIndex == i)
			return *iter;
	}

	return nullptr;
}

void CMissionUI::DeleteQuest(CQuest* pQuest)
{
	std::list<CQuest*>::iterator iter = m_QuestList.begin();
	std::list<CQuest*>::iterator iterEnd = m_QuestList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == pQuest)
		{
			m_QuestList.erase(iter);
			m_vecButtons[--m_iQuests]->SetEnable(false);			
			return;
		}
	}
}

bool CMissionUI::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Button_Normal", false,
		"Button\\Button_Normal.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Window_QuestClose", false,
		"Button\\Window_QuestClose.ogg");

	if (!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pTitle = CreateComponent<CTitleBar>("MissionTitle");

	SetRootComponent(pTitle);

	static_cast<CTitleBar*>(pTitle.get())->SetTexture("BLUIMission_IC");
	static_cast<CTitleBar*>(pTitle.get())->SetUV(Vector2(6.f, 6.f), Vector2(517.f, 81.f));
	static_cast<CTitleBar*>(pTitle.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CTitleBar*>(pTitle.get())->SetWorldScale(511.f, 75.f, 1.f);
	static_cast<CTitleBar*>(pTitle.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CTitleBar*>(pTitle.get())->SetWorldPos(200.f, 200.f, 0.f);

	std::shared_ptr<CComponent> pTitleFont = static_cast<CTitleBar*>(pTitle.get())->GetFont();

	static_cast<CUFont*>(pTitleFont.get())->SetText(L"Äù½ºÆ®");
	static_cast<CUFont*>(pTitleFont.get())->SetRect(-100.f, 0.f, 100.f, 0.f);
	static_cast<CUFont*>(pTitleFont.get())->SetSize(30.f);

	std::shared_ptr<CComponent> pExitButton = CreateComponent<CUIButton>("ExitButton");

	static_cast<CPanel*>(pTitle.get())->AddChild(pExitButton);

	static_cast<CUIButton*>(pExitButton.get())->SetTexture(BUTTON_STATE::OFF, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pExitButton.get())->SetTexture(BUTTON_STATE::ON, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pExitButton.get())->SetTexture(BUTTON_STATE::PRESS, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pExitButton.get())->SetTexture(BUTTON_STATE::DISABLE, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pExitButton.get())->SetUV(Vector2(90.f, 348.f), Vector2(107.f, 365.f));
	static_cast<CUIButton*>(pExitButton.get())->SetSize(Vector2(1024.f, 512.f));
	static_cast<CUIButton*>(pExitButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(pExitButton.get())->SetWorldScale(17.f, 17.f, 1.f);
	static_cast<CUIButton*>(pExitButton.get())->AddRelativePos(240.f,0.f, 0.f);
	static_cast<CUIButton*>(pExitButton.get())->SetButtonCallBack(BUTTON_STATE::PRESS, this, &CMissionUI::ButtonCallBack);

	std::shared_ptr<CComponent> pPanel = CreateComponent<CPanel>("MissionPanel");

	static_cast<CTitleBar*>(pTitle.get())->AddChild(pPanel);

	static_cast<CPanel*>(pPanel.get())->SetTexture("BLUIMission_IC");
	static_cast<CPanel*>(pPanel.get())->SetUV(Vector2(4.f, 83.f), Vector2(516.f, 795.f));
	static_cast<CPanel*>(pPanel.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CPanel*>(pPanel.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CPanel*>(pPanel.get())->SetWorldScale(512.f, 712.f, 1.f);
	static_cast<CPanel*>(pPanel.get())->AddRelativePosY(-393.5f);
	static_cast<CPanel*>(pPanel.get())->SetZOrder(5);

	std::shared_ptr<CComponent> pButton = CreateComponent<CUIButton>("Mission1");

	m_vecButtons.push_back(pButton);

	static_cast<CPanel*>(pPanel.get())->AddChild(pButton);

	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::OFF, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::ON, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::PRESS, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::DISABLE, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetUV(Vector2(370.f, 319.f), Vector2(486.f, 342.f));
	static_cast<CUIButton*>(pButton.get())->SetSize(Vector2(1024.f, 512.f));
	static_cast<CUIButton*>(pButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(pButton.get())->SetWorldScale(250.f, 38.f, 1.f);
	static_cast<CUIButton*>(pButton.get())->AddRelativePosY(150.f);
	static_cast<CUIButton*>(pButton.get())->SetButtonCallBack(BUTTON_STATE::PRESS, this, &CMissionUI::ButtonCallBack);
	static_cast<CUIButton*>(pButton.get())->SetZOrder(4);

	std::shared_ptr<CComponent> pFont = CreateComponent<CUFont>("Mission1Title");

	static_cast<CUIButton*>(pButton.get())->AddChild(pFont);
	static_cast<CUIButton*>(pButton.get())->SetEnable(false);

	static_cast<CUFont*>(pFont.get())->SetText(L"¿Ü´«¹ÚÀÌ Àâ±â Äù½ºÆ®");
	static_cast<CUFont*>(pFont.get())->SetShadowOffset(Vector3(1.f, -1.f, 0.f));
	static_cast<CUFont*>(pFont.get())->SetShadowColor(0.f, 0.f, 0.f, 1.f);
	static_cast<CUFont*>(pFont.get())->SetRect(-100.f, 0.f, 100.f, 0.f);


	pButton = CreateComponent<CUIButton>("Mission2");

	m_vecButtons.push_back(pButton);

	static_cast<CPanel*>(pPanel.get())->AddChild(pButton);

	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::OFF, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::ON, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::PRESS, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::DISABLE, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetUV(Vector2(370.f, 319.f), Vector2(486.f, 342.f));
	static_cast<CUIButton*>(pButton.get())->SetSize(Vector2(1024.f, 512.f));
	static_cast<CUIButton*>(pButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(pButton.get())->SetWorldScale(250.f, 38.f, 1.f);
	static_cast<CUIButton*>(pButton.get())->AddRelativePosY(100.f);
	static_cast<CUIButton*>(pButton.get())->SetButtonCallBack(BUTTON_STATE::PRESS, this, &CMissionUI::ButtonCallBack);
	static_cast<CUIButton*>(pButton.get())->SetEnable(false); 
	static_cast<CUIButton*>(pButton.get())->SetZOrder(4);

	pFont = CreateComponent<CUFont>("Mission2Title");

	static_cast<CUIButton*>(pButton.get())->AddChild(pFont);

	static_cast<CUFont*>(pFont.get())->SetText(L"Àü¸®Ç° È¹µæ Äù½ºÆ®");
	static_cast<CUFont*>(pFont.get())->SetShadowOffset(Vector3(1.f, -1.f, 0.f));
	static_cast<CUFont*>(pFont.get())->SetShadowColor(0.f, 0.f, 0.f, 1.f);
	static_cast<CUFont*>(pFont.get())->SetRect(-100.f, 0.f, 100.f, 0.f);

	pButton = CreateComponent<CUIButton>("Mission3");

	static_cast<CPanel*>(pPanel.get())->AddChild(pButton);

	m_vecButtons.push_back(pButton);

	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::OFF, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::ON, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::PRESS, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::DISABLE, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetUV(Vector2(370.f, 319.f), Vector2(486.f, 342.f));
	static_cast<CUIButton*>(pButton.get())->SetSize(Vector2(1024.f, 512.f));
	static_cast<CUIButton*>(pButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(pButton.get())->SetWorldScale(250.f, 38.f, 1.f);
	static_cast<CUIButton*>(pButton.get())->AddRelativePosY(50.f);
	static_cast<CUIButton*>(pButton.get())->SetButtonCallBack(BUTTON_STATE::PRESS, this, &CMissionUI::ButtonCallBack);
	static_cast<CUIButton*>(pButton.get())->SetEnable(false);
	static_cast<CUIButton*>(pButton.get())->SetZOrder(4);

	m_pSubWindow = CreateComponent<CPanel>("SubWindowTitle");

	m_pSubWindow->SetEnable(false);

	static_cast<CSceneComponent*>(pPanel.get())->AddChild(m_pSubWindow);

	static_cast<CPanel*>(m_pSubWindow.get())->SetInheritScale(false);
	static_cast<CPanel*>(m_pSubWindow.get())->SetTexture("smoke_r_00045");
	static_cast<CPanel*>(m_pSubWindow.get())->SetUV(Vector2(0.f, 55.f), Vector2(927.f, 757.f));
	static_cast<CPanel*>(m_pSubWindow.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CPanel*>(m_pSubWindow.get())->SetWorldScale(463.5f, 351.f, 1.f);
	static_cast<CPanel*>(m_pSubWindow.get())->SetRelativePosX(487.f);
	static_cast<CPanel*>(m_pSubWindow.get())->SetRelativePosY(180.f);
	static_cast<CPanel*>(m_pSubWindow.get())->SetPivot(0.5f, 0.5f,0.f);
	static_cast<CPanel*>(m_pSubWindow.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pQuestDesc = CreateComponent<CUFont>("QuestDesc");

	static_cast<CSceneComponent*>(m_pSubWindow.get())->AddChild(pQuestDesc);

	static_cast<CUFont*>(pQuestDesc.get())->SetRect(-221.f, 160.f, 221.f, 0.f);

	SetWorldPos(0.f, 400.f, 0.f);
	return true;
}

void CMissionUI::Start()
{
	CGameObject::Start();
}

void CMissionUI::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CMissionUI::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CMissionUI::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CMissionUI::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CMissionUI::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CMissionUI::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CMissionUI::Clone()
{
	return std::shared_ptr<CGameObject>(new CMissionUI(*this));
}

void CMissionUI::Save(FILE * pFile)
{
	CGameObject::Save(pFile);
}

void CMissionUI::Load(FILE * pFile)
{
	CGameObject::Load(pFile);
}

void CMissionUI::ButtonCallBack(class CUIButton* pButton)
{
	std::string strButton = pButton->GetName();

	if (strButton == "ExitButton")
	{
		GET_SINGLE(CSoundManager)->Play("Window_QuestClose");
		SetEnable(false);
	}
	else if (strButton == "Mission1")
	{
		GET_SINGLE(CSoundManager)->Play("Button_Normal");
		if (!m_pSubWindow->IsEnable())
		{
			m_pSubWindow->SetEnable(true);

			std::shared_ptr<CComponent> pCom = static_cast<CUFont*>(m_pSubWindow.get())->FindComponentByType<CUFont>();

			CQuest* pQuest = FindQuest(0);

			if (pQuest)
			{
				static_cast<CUFont*>(pCom.get())->SetText(pQuest->GetDesc().c_str());
			}
		}
		else
		{
			m_pSubWindow->SetEnable(false);
		}
	}
	else if (strButton == "Mission2")
	{
		GET_SINGLE(CSoundManager)->Play("Button_Normal");
		if (!m_pSubWindow->IsEnable())
		{
			m_pSubWindow->SetEnable(true);

			std::shared_ptr<CComponent> pCom = static_cast<CUFont*>(m_pSubWindow.get())->FindComponentByType<CUFont>();

			CQuest* pQuest = FindQuest(1);

			if (pQuest)
			{
				static_cast<CUFont*>(pCom.get())->SetText(pQuest->GetDesc().c_str());
			}
		}
		else
		{
			m_pSubWindow->SetEnable(false);
		}
	}
	else if (strButton == "Mission3")
	{
		GET_SINGLE(CSoundManager)->Play("Button_Normal");
		if (!m_pSubWindow->IsEnable())
		{
			m_pSubWindow->SetEnable(true);

			std::shared_ptr<CComponent> pCom = static_cast<CUFont*>(m_pSubWindow.get())->FindComponentByType<CUFont>();

			CQuest* pQuest = FindQuest(2);

			if (pQuest)
			{
				static_cast<CUFont*>(pCom.get())->SetText(pQuest->GetDesc().c_str());
			}
		}
		else
		{
			m_pSubWindow->SetEnable(false);
		}
	}
}
