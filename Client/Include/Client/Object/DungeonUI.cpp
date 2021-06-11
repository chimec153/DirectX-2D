#include "DungeonUI.h"
#include "UI/Panel.h"
#include "UI/UIButton.h"
#include "Iseulenia.h"
#include "Scene/Scene.h"
#include "../EditorFlags.h"
#include "Sound/SoundManager.h"
#include "UI/UFont.h"

CDungeonUI::CDungeonUI()
{
}

CDungeonUI::CDungeonUI(const CDungeonUI& ui)	:
	CGameObject(ui)
{
}

CDungeonUI::~CDungeonUI()
{
}

bool CDungeonUI::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Button_Open", false,
		"Button\\Button_Open.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Window_UIClose", false,
		"Button\\Window_UIClose.ogg");

	if(!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pPanel = CreateComponent<CPanel>("Panel");

	SetRootComponent(pPanel);

	static_cast<CPanel*>(pPanel.get())->SetTexture("BLUIDungeon_I11");
	static_cast<CPanel*>(pPanel.get())->SetUV(Vector2(1.f, 77.f), Vector2(691.f, 600.f));
	static_cast<CPanel*>(pPanel.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CPanel*>(pPanel.get())->SetZOrder(3);
	static_cast<CPanel*>(pPanel.get())->SetWorldScale(690.f, 523.f, 1.f);
	static_cast<CPanel*>(pPanel.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CPanel*>(pPanel.get())->SetWorldPos(200.f, 0.f, 0.f);

	std::shared_ptr<CComponent> pButton = CreateComponent<CUIButton>("DungeonButton1");

	static_cast<CPanel*>(pPanel.get())->AddChild(pButton);

	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::OFF, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::ON, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::PRESS, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::DISABLE, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetUV(Vector2(845.f, 159.f), Vector2(1010.f, 195.f));
	static_cast<CUIButton*>(pButton.get())->SetSize(Vector2(1024.f, 512.f));
	static_cast<CUIButton*>(pButton.get())->SetButtonCallBack(BUTTON_STATE::PRESS, this, &CDungeonUI::ButtonCallBack);
	static_cast<CUIButton*>(pButton.get())->SetWorldScale(165.f, 36.f, 1.f);
	static_cast<CUIButton*>(pButton.get())->SetZOrder(2);
	static_cast<CUIButton*>(pButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(pButton.get())->AddRelativePosY(25.f);

	std::shared_ptr<CComponent> pFont = CreateComponent<CUFont>("Font");

	static_cast<CUIButton*>(pButton.get())->AddChild(pFont);

	static_cast<CUFont*>(pFont.get())->SetText("터틀레이크");
	static_cast<CUFont*>(pFont.get())->SetRect(-100.f, 0.f, 100.f, 0.f);

	pButton = CreateComponent<CUIButton>("ExitButton");

	static_cast<CPanel*>(pPanel.get())->AddChild(pButton);

	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::OFF, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::ON, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::PRESS, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::DISABLE, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetUV(Vector2(90.f, 348.f), Vector2(107.f, 365.f));
	static_cast<CUIButton*>(pButton.get())->SetSize(Vector2(1024.f, 512.f));
	static_cast<CUIButton*>(pButton.get())->SetButtonCallBack(BUTTON_STATE::PRESS, this, &CDungeonUI::ButtonCallBack);
	static_cast<CUIButton*>(pButton.get())->SetWorldScale(17.f, 17.f, 1.f);
	static_cast<CUIButton*>(pButton.get())->SetZOrder(2);
	static_cast<CUIButton*>(pButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(pButton.get())->AddRelativePos(316.f, 242.f, 0.f);

	return true;
}

void CDungeonUI::Start()
{
	CGameObject::Start();
}

void CDungeonUI::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CDungeonUI::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CDungeonUI::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CDungeonUI::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CDungeonUI::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CDungeonUI::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CDungeonUI::Clone()
{
	return std::shared_ptr<CGameObject>(new CDungeonUI(*this));
}

void CDungeonUI::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CDungeonUI::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CDungeonUI::ButtonCallBack(CUIButton* pButton)
{
	std::string strButton = pButton->GetName();
	if (strButton == "DungeonButton1")
	{
		GET_SINGLE(CSoundManager)->Play("Button_Open");
		CLayer* pLayer = m_pScene->FindLayer(ALPHA_LAYER);

		std::shared_ptr<CGameObject> pNpc = pLayer->FindObj("Iseulenia");

		static_cast<CIseulenia*>(pNpc.get())->Teleport(DUNGEON_TYPE::TURTLE_LAKE);
	}
	else if (strButton == "ExitButton")
	{
		GET_SINGLE(CSoundManager)->Play("Window_UIClose");
		CLayer* pLayer = m_pScene->FindLayer(ALPHA_LAYER);

		std::shared_ptr<CGameObject> pNpc = pLayer->FindObj("Iseulenia");

		static_cast<CIseulenia*>(pNpc.get())->NPCWork();
	}
}
