#include "Shop.h"
#include "UI/TitleBar.h"
#include "UI/Panel.h"
#include "UI/UIButton.h"
#include "Scene/Scene.h"
#include "Editor/Object/Tanara.h"
#include "UI/UIImage.h"
#include "Editor/Item.h"
#include "Editor/Object/Player3D.h"
#include "Editor/Object/Inventory3D.h"
#include "UI/UFont.h"
#include "Sound/SoundManager.h"

CShop::CShop()	:
	m_pItems()
	, m_iSelect(-1)
{
}

CShop::CShop(const CShop& ui)	:
	CGameObject(ui)
	, m_iSelect(ui.m_iSelect)
{
	memcpy_s(m_pItems, sizeof(CItem*) * MAX_SHOP, ui.m_pItems, sizeof(CItem*) * MAX_SHOP);
}

CShop::~CShop()
{
}

void CShop::SetItem(int iSlot, const std::string& strKey)
{
	if (iSlot >= MAX_SHOP)
		return;

	CItem* pItem = CItem::FindItem(strKey);

	if (!pItem)
		return;

	m_pItems[iSlot] = pItem;

	const Vector2 vIndex = pItem->GetIndex();

	char strImage[MAX_PATH] = {};

	sprintf_s(strImage, "Item%d", iSlot);

	std::shared_ptr<CComponent> pCom = FindComponent(strImage);
	
	static_cast<CUIImage*>(pCom.get())->SetTexture(pItem->GetTextureName());
	static_cast<CUIImage*>(pCom.get())->SetUV(
		Vector2(48.f * vIndex.x, 48.f * vIndex.y), 
		Vector2(48.f * (vIndex.x+1.f), 48.f * (vIndex.y+1.f)));

	char strGold[MAX_PATH] = {};

	sprintf_s(strGold, "Gold%d", iSlot);

	pCom = FindComponent(strGold);

	TCHAR strMoney[MAX_PATH] = {};

	swprintf_s(strMoney, L"%d", pItem->GetPrice());

	static_cast<CUFont*>(pCom.get())->SetText(strMoney);

	char strName[MAX_PATH] = {};

	sprintf_s(strName, "Name%d", iSlot);

	TCHAR strItem[MAX_PATH] = {};

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, pItem->GetName().c_str(), -1, strItem, static_cast<int>(pItem->GetName().length()));
#else
	strcpy_s(strItem, MAX_PATH, pItem->GetName().c_str());
#endif
	pCom = FindComponent(strName);
	static_cast<CUFont*>(pCom.get())->SetText(strItem);
}

void CShop::BuyItem(int iSlot)
{
	if (iSlot >= MAX_SHOP || 
		iSlot < 0)
		return;

	if (!m_pItems[iSlot])
		return;

	CLayer* pLayer = m_pScene->FindLayer(ALBEDO_LAYER);

	if (pLayer)
	{
		std::shared_ptr<CGameObject> pPlayer = pLayer->FindObj("Player");

		if (!pPlayer)
		{
			pPlayer = m_pScene->FindLayer(ALPHA_LAYER)->FindObj("Player");
		}

		if (pPlayer)
		{
			int iGold = static_cast<CPlayer3D*>(pPlayer.get())->GetGold();

			int iPrice = m_pItems[iSlot]->GetPrice();
			if (iGold < iPrice)
			{
				GET_SINGLE(CSoundManager)->Play("SYSMSG_2002");
				return;
			}

			CLayer* pUILayer = m_pScene->FindLayer(UI_LAYER);

			std::shared_ptr<CGameObject> pInventory = pUILayer->FindObj("Inventory");

			if (pInventory)
			{
				if (static_cast<CInventory3D*>(pInventory.get())->AddItem(m_pItems[iSlot]))
				{
					GET_SINGLE(CSoundManager)->Play("SYSMSG_2001");
					static_cast<CPlayer3D*>(pPlayer.get())->AddGold(-iPrice);
				}
			}

		}
	}
}

bool CShop::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "SYSMSG_2001", false,
		"Button\\SYSMSG_2001.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "SYSMSG_2002", false,
		"Button\\SYSMSG_2002.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Button_Tap", false,
		"Button\\Button_Tap.ogg");

	if(!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pTitle = CreateComponent<CTitleBar>("Title");

	SetRootComponent(pTitle);

	static_cast<CTitleBar*>(pTitle.get())->SetTexture("BLUIShop_I6");
	static_cast<CTitleBar*>(pTitle.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CTitleBar*>(pTitle.get())->SetUV(Vector2(1.f, 0.f), Vector2(881.f, 75.f));
	static_cast<CTitleBar*>(pTitle.get())->SetWorldScale(880.f, 75.f, 1.f);
	static_cast<CTitleBar*>(pTitle.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CTitleBar*>(pTitle.get())->SetWorldPos(400.f, 350.f, 0.f);
	static_cast<CTitleBar*>(pTitle.get())->SetZOrder(5);
	std::shared_ptr<CComponent> pTitleFont = static_cast<CTitleBar*>(pTitle.get())->GetFont();

	static_cast<CUFont*>(pTitleFont.get())->SetText(L"무기 상점");
	static_cast<CUFont*>(pTitleFont.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::CEN);
	static_cast<CUFont*>(pTitleFont.get())->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
	static_cast<CUFont*>(pTitleFont.get())->SetRect(-70.f, 0.f, 70.f, 0.f);

	std::shared_ptr<CComponent> pPanel = CreateComponent<CPanel>("Panel");

	static_cast<CTitleBar*>(pTitle.get())->AddChild(pPanel);

	static_cast<CPanel*>(pPanel.get())->SetZOrder(5);
	static_cast<CPanel*>(pPanel.get())->SetTexture("BLUIShop_I6");
	static_cast<CPanel*>(pPanel.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CPanel*>(pPanel.get())->SetUV(Vector2(0.f, 77.f), Vector2(881.f, 757.f));
	static_cast<CPanel*>(pPanel.get())->SetWorldScale(881.f, 680.f, 1.f);
	static_cast<CPanel*>(pPanel.get())->SetRelativePosY(-377.5f);
	static_cast<CPanel*>(pPanel.get())->SetPivot(0.5f, 0.5f, 0.f);

	std::shared_ptr<CComponent> pExitButton = CreateComponent<CUIButton>("Exit");

	static_cast<CTitleBar*>(pTitle.get())->AddChild(pExitButton);

	static_cast<CUIButton*>(pExitButton.get())->SetTexture(BUTTON_STATE::OFF, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pExitButton.get())->SetTexture(BUTTON_STATE::ON, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pExitButton.get())->SetTexture(BUTTON_STATE::PRESS, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pExitButton.get())->SetTexture(BUTTON_STATE::DISABLE, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pExitButton.get())->SetSize(Vector2(1024.f, 512.f));
	static_cast<CUIButton*>(pExitButton.get())->SetUV(Vector2(90.f, 348.f), Vector2(107.f, 365.f));
	static_cast<CUIButton*>(pExitButton.get())->SetWorldScale(17.f, 17.f, 1.f);
	static_cast<CUIButton*>(pExitButton.get())->SetRelativePos(400.f, 0.f, 0.f);
	static_cast<CUIButton*>(pExitButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(pExitButton.get())->SetButtonCallBack(BUTTON_STATE::PRESS, this, &CShop::ButtonCallBack);
	static_cast<CUIButton*>(pExitButton.get())->SetZOrder(4);

	std::shared_ptr<CComponent> pButton = CreateComponent<CUIButton>("Buy");

	static_cast<CPanel*>(pPanel.get())->AddChild(pButton);

	
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::OFF, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::ON, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::PRESS, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::DISABLE, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetSize(Vector2(1024.f, 512.f));
	static_cast<CUIButton*>(pButton.get())->SetUV(Vector2(445.f, 247.f), Vector2(574.f, 281.f));
	static_cast<CUIButton*>(pButton.get())->SetWorldScale(129.f, 34.f, 1.f);
	static_cast<CUIButton*>(pButton.get())->SetRelativePos(-66.f, -322.f, 0.f);
	static_cast<CUIButton*>(pButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(pButton.get())->SetButtonCallBack(BUTTON_STATE::PRESS, this, &CShop::ButtonCallBack);
	static_cast<CUIButton*>(pButton.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pFont = CreateComponent<CUFont>("BuyFont");

	static_cast<CUIButton*>(pButton.get())->AddChild(pFont);

	static_cast<CUFont*>(pFont.get())->SetText("구매");
	static_cast<CUFont*>(pFont.get())->SetRect(-150.f, 0.f, 100.f, 0.f);

	pButton = CreateComponent<CUIButton>("Sell");

	static_cast<CPanel*>(pPanel.get())->AddChild(pButton);

	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::OFF, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::ON, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::PRESS, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::DISABLE, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(pButton.get())->SetSize(Vector2(1024.f, 512.f));
	static_cast<CUIButton*>(pButton.get())->SetUV(Vector2(574.f, 247.f), Vector2(702.f, 281.f));
	static_cast<CUIButton*>(pButton.get())->SetWorldScale(128.f, 34.f, 1.f);
	static_cast<CUIButton*>(pButton.get())->SetRelativePos(59.5f, -322.f, 0.f);
	static_cast<CUIButton*>(pButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(pButton.get())->SetButtonCallBack(BUTTON_STATE::PRESS, this, &CShop::ButtonCallBack);
	static_cast<CUIButton*>(pButton.get())->SetZOrder(1);

	pFont = CreateComponent<CUFont>("SellFont");

	static_cast<CUIButton*>(pButton.get())->AddChild(pFont);

	static_cast<CUFont*>(pFont.get())->SetText("판매");
	static_cast<CUFont*>(pFont.get())->SetRect(-50.f, 0.f, 100.f, 0.f);

	std::shared_ptr<CComponent> pImage = CreateComponent<CUIImage>("Select");

	static_cast<CPanel*>(pPanel.get())->AddChild(pImage);

	static_cast<CUIImage*>(pImage.get())->SetTexture("UI2_Worldmap_lock_gray_Img");
	static_cast<CUIImage*>(pImage.get())->SetSize(Vector2(1024.f, 512.f));
	static_cast<CUIImage*>(pImage.get())->SetUV(Vector2(693.f, 82.f), Vector2(866.f, 123.f));
	static_cast<CUIImage*>(pImage.get())->SetWorldScale(400.f, 60.f, 1.f);
	static_cast<CUIImage*>(pImage.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pImage.get())->SetEnable(false);

	for (int i = 0; i < 8; i++)
	{
		char strSlot[MAX_PATH] = {};

		sprintf_s(strSlot, "Slot%d", i);

		std::shared_ptr<CComponent> pButton = CreateComponent<CUIButton>(strSlot);

		static_cast<CPanel*>(pPanel.get())->AddChild(pButton);

		static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::OFF, "UI2_Worldmap_lock_gray_Img");
		static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::ON, "UI2_Worldmap_lock_gray_Img");
		static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::PRESS, "UI2_Worldmap_lock_gray_Img");
		static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::DISABLE, "UI2_Worldmap_lock_gray_Img");
		static_cast<CUIButton*>(pButton.get())->SetSize(Vector2(1024.f, 512.f));
		static_cast<CUIButton*>(pButton.get())->SetUV(Vector2(801.f, 3.f), Vector2(981.f, 39.f));
		static_cast<CUIButton*>(pButton.get())->SetWorldScale(400.f, 60.f, 1.f);
		static_cast<CUIButton*>(pButton.get())->SetRelativePos(-220.f, 270.f - i * 68.f, 0.f);
		static_cast<CUIButton*>(pButton.get())->SetPivot(0.5f, 0.5f, 0.f);
		static_cast<CUIButton*>(pButton.get())->SetButtonCallBack(BUTTON_STATE::PRESS, this, &CShop::ButtonCallBack);
		static_cast<CUIButton*>(pButton.get())->SetZOrder(4);

		char strItem[MAX_PATH] = {};

		sprintf_s(strItem, "Item%d", i);

		std::shared_ptr<CComponent> pImage = CreateComponent<CUIImage>(strItem);

		static_cast<CUIButton*>(pButton.get())->AddChild(pImage);

		static_cast<CUIImage*>(pImage.get())->SetTexture("icon_sheet_076");
		static_cast<CUIImage*>(pImage.get())->SetSize(Vector2(960.f, 480.f));
		static_cast<CUIImage*>(pImage.get())->SetUV(Vector2(576.f, 240.f), Vector2(624.f, 288.f));
		static_cast<CUIImage*>(pImage.get())->SetWorldScale(48.f, 48.f, 1.f);
		static_cast<CUIImage*>(pImage.get())->SetRelativePos(-170.f, 0.f, 0.f);
		static_cast<CUIImage*>(pImage.get())->SetPivot(0.5f, 0.5f, 0.f);

		pImage = CreateComponent<CUIImage>("MoneyIcon");

		static_cast<CUIButton*>(pButton.get())->AddChild(pImage);

		static_cast<CUIImage*>(pImage.get())->SetTexture("32_UI2_Waypoint_Manager_Start_icon");
		static_cast<CUIImage*>(pImage.get())->SetSize(Vector2(1024.f, 1024.f));
		static_cast<CUIImage*>(pImage.get())->SetUV(Vector2(131.f, 865.f), Vector2(147.f, 881.f));
		static_cast<CUIImage*>(pImage.get())->SetWorldScale(16.f, 16.f, 1.f);
		static_cast<CUIImage*>(pImage.get())->SetRelativePos(-130.f, 0.f, 0.f);
		static_cast<CUIImage*>(pImage.get())->SetPivot(0.5f, 0.5f, 0.f);

		char strGoldFont[MAX_PATH] = {};

		sprintf_s(strGoldFont, "Gold%d", i);

		std::shared_ptr<CComponent> pFont = CreateComponent<CUFont>(strGoldFont);

		static_cast<CUIButton*>(pButton.get())->AddChild(pFont);

		static_cast<CUFont*>(pFont.get())->SetText(L"000");
		static_cast<CUFont*>(pFont.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::CEN);
		static_cast<CUFont*>(pFont.get())->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
		static_cast<CUFont*>(pFont.get())->SetRelativePosX(-110.f);
		static_cast<CUFont*>(pFont.get())->SetRect(0.f, 5.f, 100.f, 0.f);

		char strItemNameFont[MAX_PATH] = {};

		sprintf_s(strItemNameFont, "Name%d", i);

		std::shared_ptr<CComponent> pItemNameFont = CreateComponent<CUFont>(strItemNameFont);

		static_cast<CUIButton*>(pButton.get())->AddChild(pItemNameFont);

		static_cast<CUFont*>(pItemNameFont.get())->SetText(L"None");
		static_cast<CUFont*>(pItemNameFont.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::CEN);
		static_cast<CUFont*>(pItemNameFont.get())->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
		static_cast<CUFont*>(pItemNameFont.get())->SetRelativePosX(-20.f);
		static_cast<CUFont*>(pItemNameFont.get())->SetRect(0.f, 5.f, 200.f, 0.f);
	}

	return true;
}

void CShop::Start()
{
	CGameObject::Start();
}

void CShop::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CShop::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CShop::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CShop::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CShop::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CShop::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CShop::Clone()
{
	return std::shared_ptr<CGameObject>(new CShop(*this));
}

void CShop::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CShop::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CShop::ButtonCallBack(CUIButton* pButton)
{
	const std::string& strName = pButton->GetName();

	if (strName == "Exit")
	{
		CLayer* pLayer = m_pScene->FindLayer(ALBEDO_LAYER);

		std::shared_ptr<CGameObject> pTanara = pLayer->FindObj("Tanara");
		if (pTanara)
		{
			static_cast<CTanara*>(pTanara.get())->Close();
		}
	}

	else if (strName == "Buy")
	{
		BuyItem(m_iSelect);
	}
	else
	{
		GET_SINGLE(CSoundManager)->Play("Button_Tap");
		const char* pName = strName.c_str();

		char* pContext = nullptr;

		char* pResult = strtok_s(const_cast<char*>(pName), "Slot", &pContext);

		m_iSelect = atoi(pResult);

		std::shared_ptr<CComponent> pCom = FindComponent("Select");
		pCom->SetEnable(true);
		static_cast<CUIImage*>(pCom.get())->SetRelativePos(-220.f, 270.f - m_iSelect * 68.f, 0.f);
	}
}
