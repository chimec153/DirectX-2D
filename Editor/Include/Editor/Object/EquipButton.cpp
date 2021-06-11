#include "EquipButton.h"
#include "UI/UIButton.h"
#include "Scene/Scene.h"
#include "Device.h"
#include "../Editor3D.h"
#include "Player3D.h"

CEquipButton::CEquipButton()
{
	m_iObjType = static_cast<int>(EDITOR_OBJECT_TYPE::EQUIPBUTTON);
}

CEquipButton::CEquipButton(const CEquipButton& ui) :
	CGameObject(ui)
{

}
CEquipButton::~CEquipButton()
{

}

bool CEquipButton::Init()
{
	if (!CGameObject::Init())
		return false;

	Resolution tRS = RESOLUTION;

	std::shared_ptr<CComponent> pButton = CreateComponent<CUIButton>("Equip Button");

	SetRootComponent(pButton);

	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::OFF, "BLUIEquip_I2C");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::ON, "BLUIEquip_I2C");
	static_cast<CUIButton*>(pButton.get())->SetTexture(BUTTON_STATE::PRESS, "BLUIEquip_I2C");
	static_cast<CUIButton*>(pButton.get())->SetColor(BUTTON_STATE::OFF, Vector4(0.5f, 0.5f, 0.5f, 0.5f));
	static_cast<CUIButton*>(pButton.get())->SetColor(BUTTON_STATE::ON, Vector4(0.85f, 0.85f, 0.85f, 0.85f));
	static_cast<CUIButton*>(pButton.get())->SetColor(BUTTON_STATE::PRESS, Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CUIButton*>(pButton.get())->SetUV(Vector2(45.f, 9.f), Vector2(97.f, 73.f));
	static_cast<CUIButton*>(pButton.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CUIButton*>(pButton.get())->SetWorldScale(52.f, 64.f, 0.f);
	static_cast<CUIButton*>(pButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(pButton.get())->SetCallBack(BUTTON_STATE::PRESS, this, &CEquipButton::ButtonPress);
	static_cast<CUIButton*>(pButton.get())->SetWorldPos(-static_cast<float>(tRS.iWidth) / 2.f + 125.f, static_cast<float>(tRS.iHeight)/2.f - 50.f, 0.f);

	return true;
}

void CEquipButton::Start()
{
	CGameObject::Start();
}

void CEquipButton::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CEquipButton::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CEquipButton::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CEquipButton::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CEquipButton::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CEquipButton::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CEquipButton::Clone()
{
	return std::shared_ptr<CGameObject>(new CEquipButton(*this));
}

void CEquipButton::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CEquipButton::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CEquipButton::ButtonPress(float fTime)
{
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
			static_cast<CPlayer3D*>(pPlayer.get())->Equip(fTime);
		}
	}
}
