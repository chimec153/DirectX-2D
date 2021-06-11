#include "LootingWindow.h"
#include "UI/UIImage.h"
#include "Component/ColliderRect.h"
#include "UI/UIButton.h"
#include "../Item.h"
#include "UI/Icon.h"
#include "Inventory3D.h"
#include "Scene/Scene.h"
#include "Sound/SoundManager.h"

CLootingWindow::CLootingWindow()	:
	m_pInven(nullptr)
	, m_pLootItem()
	, m_iCount(0)
{
}

CLootingWindow::CLootingWindow(const CLootingWindow& ui)	:
	CGameObject(ui)
	, m_pInven(ui.m_pInven)
	, m_iCount(ui.m_iCount)
{
	size_t iSize = sizeof(CItem*) * MAX_LOOTING;
	memcpy_s(m_pLootItem, iSize, ui.m_pLootItem, iSize);

	for (int i = 0; i < MAX_LOOTING; i++)
	{
		char strSlot[MAX_PATH] = {};
		sprintf_s(strSlot, MAX_PATH, "Slot%d", i);

		m_pSlot[i] = FindComponent(strSlot);
		static_cast<CUIButton*>(m_pSlot[i].get())->SetButtonCallBack(
			BUTTON_STATE::PRESS, this, &CLootingWindow::ButtonPress);
	}
}

CLootingWindow::~CLootingWindow()
{
}

void CLootingWindow::CreateItem(class CItem* pItem)
{
	m_pSlot[m_iCount]->SetEnable(true);
	m_pLootItem[m_iCount] = pItem;

	Vector2 vIndex = {};
	const char* pName = nullptr;

	if (pItem)
	{
		vIndex = pItem->GetIndex();
		pName = pItem->GetTextureName().c_str();
	}

	std::shared_ptr<CComponent> pIcon = CreateComponent<CIcon>("Item");

	static_cast<CUIButton*>(m_pSlot[m_iCount].get())->AddChild(pIcon);

	if (pName)
	{
		if (!static_cast<CIcon*>(pIcon.get())->SetTexture(pName))
			assert(false);
	}
	static_cast<CIcon*>(pIcon.get())->SetUV(Vector2(48.f * vIndex.x, 48.f * vIndex.y),
		Vector2(48.f * (vIndex.x + 1.f), 48.f * (vIndex.y + 1.f)));
	static_cast<CIcon*>(pIcon.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CIcon*>(pIcon.get())->SetWorldScale(48.f, 48.f, 0.f);
	static_cast<CIcon*>(pIcon.get())->SetRelativePosY(0.f);
	static_cast<CIcon*>(pIcon.get())->SetZOrder(3);
	m_iCount++;
}

void CLootingWindow::Looting(int iSlotIndex)
{
	if (m_iCount < iSlotIndex)
		return;

	if (m_pInven)
	{
		if (!static_cast<CInventory3D*>(m_pInven.get())->AddItem(m_pLootItem[iSlotIndex]))
			return;

		GET_SINGLE(CSoundManager)->Play("Button_Normal_02");

		size_t iCount = MAX_LOOTING - iSlotIndex - 1;
		size_t iSize = iCount * sizeof(m_pLootItem[iSlotIndex]);

		if (iSlotIndex != MAX_LOOTING - 1)
		{
			memcpy_s(m_pLootItem + iSlotIndex, iSize,
				m_pLootItem +iSlotIndex + 1, iSize);
		}

		m_pLootItem[MAX_LOOTING - 1] = nullptr;

		std::shared_ptr<CComponent> pIcon =
			static_cast<CUIButton*>(m_pSlot[iSlotIndex].get())->FindComponent("Item");

		pIcon->Destroy();

		m_pSlot[iSlotIndex]->Destroy();

		for (int i = iSlotIndex; i < MAX_LOOTING-1; i++)
		{
			if (!m_pSlot[i + 1])
				break;

			m_pSlot[i] = m_pSlot[i + 1];
			static_cast<CSceneComponent*>(m_pSlot[i].get())->AddRelativePosX(-48.f);
		}

		m_pSlot[MAX_LOOTING - 1] = nullptr;

		m_iCount--;
	}
}

bool CLootingWindow::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Button_Normal_02", false,
		"Button\\Button_Normal_02.ogg");

	if (!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pWindow = CreateComponent<CUIImage>("LootingWindow");

	SetRootComponent(pWindow);

	static_cast<CUIImage*>(pWindow.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pWindow.get())->SetWorldScale(477.f, 121.f, 1.f);
	static_cast<CUIImage*>(pWindow.get())->SetSize(Vector2(1024.f, 128.f));
	static_cast<CUIImage*>(pWindow.get())->SetUV(Vector2(), Vector2(477.f, 121.f));
	static_cast<CUIImage*>(pWindow.get())->SetTexture("BLUILooting_I4");
	static_cast<CUIImage*>(pWindow.get())->SetZOrder(5);

	std::shared_ptr<CComponent> pBack = CreateComponent<CUIImage>("ItemBack");

	static_cast<CSceneComponent*>(pWindow.get())->AddChild(pBack);

	static_cast<CUIImage*>(pBack.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pBack.get())->SetWorldScale(354.f, 56.f, 1.f);
	static_cast<CUIImage*>(pBack.get())->SetSize(Vector2(1024.f, 128.f));
	static_cast<CUIImage*>(pBack.get())->SetUV(Vector2(477.f,3.f), Vector2(831.f, 59.f));
	static_cast<CUIImage*>(pBack.get())->SetTexture("BLUILooting_I4");
	static_cast<CUIImage*>(pBack.get())->SetZOrder(4);

	for (int i = 0; i < MAX_LOOTING; i++)
	{
		char strSlot[MAX_PATH] = {};

		sprintf_s(strSlot, "Slot%d", i);

		m_pSlot[i] = CreateComponent<CUIButton>(strSlot);

		static_cast<CSceneComponent*>(pBack.get())->AddChild(m_pSlot[i]);

		static_cast<CUIButton*>(m_pSlot[i].get())->SetPivot(0.5f, 0.5f, 0.f);
		static_cast<CUIButton*>(m_pSlot[i].get())->SetWorldScale(48.f, 48.f, 1.f);
		static_cast<CUIButton*>(m_pSlot[i].get())->SetSize(Vector2(1024.f, 128.f));
		static_cast<CUIButton*>(m_pSlot[i].get())->SetUV(Vector2(833.f, 0.f), Vector2(883.f, 49.f));
		static_cast<CUIButton*>(m_pSlot[i].get())->SetTexture(BUTTON_STATE::OFF,"BLUILooting_I4");
		static_cast<CUIButton*>(m_pSlot[i].get())->SetTexture(BUTTON_STATE::ON, "BLUILooting_I4");
		static_cast<CUIButton*>(m_pSlot[i].get())->SetTexture(BUTTON_STATE::PRESS, "BLUILooting_I4");
		static_cast<CUIButton*>(m_pSlot[i].get())->SetRelativePos((i - MAX_LOOTING /2) * 48.f, 0.f, 0.f);
		static_cast<CUIButton*>(m_pSlot[i].get())->SetEnable(false);
		static_cast<CUIButton*>(m_pSlot[i].get())->SetZOrder(2);

		static_cast<CUIButton*>(m_pSlot[i].get())->SetButtonCallBack(
			BUTTON_STATE::PRESS, this, &CLootingWindow::ButtonPress);
	}

	return true;
}

void CLootingWindow::Start()
{
	CGameObject::Start();

	CLayer* pLayer = m_pScene->FindLayer(UI_LAYER);

	if (pLayer)
	{
		m_pInven = pLayer->FindObj("Inventory");
	}
}

void CLootingWindow::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_iCount == 0)
	{
		if (m_LootingCallBack)
		{
			m_LootingCallBack();
			Destroy();
		}
	}
}

void CLootingWindow::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CLootingWindow::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CLootingWindow::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CLootingWindow::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CLootingWindow::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CLootingWindow::Clone()
{
	return std::shared_ptr<CGameObject>(new CLootingWindow(*this));
}

void CLootingWindow::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CLootingWindow::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CLootingWindow::ButtonPress(CUIButton* pButton)
{
	//if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
	{
		for (int i = 0; i < MAX_LOOTING; i++)
		{
			CUIButton* _pButton = static_cast<CUIButton*>(m_pSlot[i].get());

			if (pButton == _pButton)
			{
				Looting(i);
				break;
			}
		}
	}
}
