#include "InvenButton.h"
#include "UI/UIButton.h"
#include "Scene/Scene.h"
#include "Device.h"
#include "../Editor3D.h"
#include "Sound/SoundManager.h"

CInvenButton::CInvenButton()
{
	m_iObjType = static_cast<int>(EDITOR_OBJECT_TYPE::INVENTORYBUTTON);
}

CInvenButton::CInvenButton(const CInvenButton& ui)	:
	CGameObject(ui)
{

}
CInvenButton::~CInvenButton()
{

}

bool CInvenButton::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Window_InventoryClose", false,
		"Button\\Window_InventoryClose.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Window_InventoryOpen", false,
		"Button\\Window_InventoryOpen.ogg");

	if (!CGameObject::Init())
		return false;

	Resolution tRS = RESOLUTION;

	std::shared_ptr<CComponent> pButton = CreateComponent<CUIButton>("Inventory Button");

	SetRootComponent(pButton);

	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::OFF, "BLUIInventory_I4F");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::ON, "BLUIInventory_I4F");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::PRESS, "BLUIInventory_I4F");
	static_cast<CUIButton*>(pButton.get())->SetColor(BUTTON_STATE::OFF, Vector4(0.5f, 0.5f, 0.5f, 0.5f));
	static_cast<CUIButton*>(pButton.get())->SetColor(BUTTON_STATE::ON, Vector4(0.85f, 0.85f, 0.85f, 0.85f));
	static_cast<CUIButton*>(pButton.get())->SetColor(BUTTON_STATE::PRESS, Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CUIButton*>(pButton.get())->SetUV(Vector2(43.f, 10.f), Vector2(109.f, 68.f));
	static_cast<CUIButton*>(pButton.get())->SetSize(Vector2(1024.f,1024.f));
	static_cast<CUIButton*>(pButton.get())->SetWorldScale(66.f, 58.f, 0.f);
	static_cast<CUIButton*>(pButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(pButton.get())->SetCallBack(BUTTON_STATE::PRESS, this, &CInvenButton::ButtonPress);
	static_cast<CUIButton*>(pButton.get())->SetWorldPos(-static_cast<float>(tRS.iWidth) / 2.f + 53.f, static_cast<float>(tRS.iHeight) / 2.f - 58.f, 0.f);

	return true;
}

void CInvenButton::Start()
{
	CGameObject::Start();
}

void CInvenButton::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CInvenButton::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CInvenButton::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CInvenButton::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CInvenButton::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CInvenButton::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CInvenButton::Clone()
{
	return std::shared_ptr<CGameObject>(new CInvenButton(*this));
}

void CInvenButton::Save(FILE * pFile)
{
	CGameObject::Save(pFile);
}

void CInvenButton::Load(FILE * pFile)
{
	CGameObject::Load(pFile);
}

void CInvenButton::ButtonPress(float fTime)
{
	CLayer* pLayer = m_pScene->FindLayer(UI_LAYER);

	if (pLayer)
	{
		std::shared_ptr<CGameObject> pInven = pLayer->FindObj("Inventory");

		if (pInven)
		{
			pInven->SetEnable(!pInven->IsEnable());

			if (pInven->IsEnable())
			{
				GET_SINGLE(CSoundManager)->Play("Window_InventoryOpen");
			}
			else
			{
				GET_SINGLE(CSoundManager)->Play("Window_InventoryClose");
			}
		}
	}
}
