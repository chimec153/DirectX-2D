#include "Equip.h"
#include "UI/TitleBar.h"
#include "UI/Panel.h"
#include "UI/UIImage.h"
#include "Input.h"
#include "Component/ColliderRect.h"
#include "Scene/Scene.h"
#include "Inventory3D.h"
#include "../Item.h"
#include "../Object/Player3D.h"
#include "Component/Renderer.h"
#include "RenderManager.h"
#include "Device.h"
#include "Resource/Animation.h"
#include "UI/UFont.h"
#include "Resource/Mesh.h"
#include "Resource/ResourceManager.h"
#include "Sound/SoundManager.h"

CEquip::CEquip()	:
	m_pPanel()
	, m_pEquipIcons()
	, m_ppItems()
	, m_iSelect(-1)
{
	m_iObjType = static_cast<int>(EDITOR_OBJECT_TYPE::EQUIP);
}
CEquip::CEquip(const CEquip& ui)	:
	CGameObject(ui)
	, m_pPanel()
	, m_pEquipIcons()
	, m_ppItems()
	, m_iSelect(-1)
{

}
CEquip::~CEquip()
{

}
void CEquip::DeleteIcon(int iType)
{
	if (iType >= static_cast<int>(EQUIP_TYPE::END))
		return;

	static_cast<CSceneComponent*>(m_pEquipSlots[iType].get())->DeleteChild(m_pEquipIcons[iType]);
	m_pEquipIcons[iType] = nullptr;
	m_ppItems[iType] = nullptr;
}
void CEquip::SetItem(int iType)
{
	CLayer* pLayer = m_pScene->FindLayer(UI_LAYER);

	std::shared_ptr<CGameObject> pInven = pLayer->FindObj("Inventory");

	if (!pInven)
		return;

	int iSelect = static_cast<CInventory3D*>(pInven.get())->GetSelect();

	if (iSelect == -1)
		return;

	CItem* pItem = static_cast<CInventory3D*>(pInven.get())->GetItemIndex(iSelect);

	if (!pItem)
		return;

	if (iType == -1 || static_cast<int>(pItem->GetType()) != iType)
		return;

	std::shared_ptr<CComponent> pIcon = static_cast<CInventory3D*>(pInven.get())->GetItemIcon(iSelect);

	if (!pIcon)
		return;

	static_cast<CSceneComponent*>(pIcon.get())->SetRelativePos(0.f, 0.f, 0.f);
	static_cast<CSceneComponent*>(pIcon.get())->SetInheritScale(false);

	std::shared_ptr<CGameObject> pPlayer = m_pScene->FindLayer(ALBEDO_LAYER)->FindObj("Player");

	if (!pPlayer)
		return;

	int iDestContainer = -1;

	switch (static_cast<EQUIP_TYPE>(iType))
	{
	case EQUIP_TYPE::WEAPON:
	{
		GET_SINGLE(CSoundManager)->Play("Equip_Staff");
		std::shared_ptr<CGameObject> pWeapon = static_cast<CPlayer3D*>(pPlayer.get())->GetWeapon();

		if (pWeapon)
		{
			std::shared_ptr<CComponent> pRoot = pWeapon->GetRootComponent();

			static_cast<CSceneComponent*>(pRoot.get())->SetMesh(pItem->GetMeshName());
			static_cast<CSceneComponent*>(pRoot.get())->AddRenderState("OutLineObject");

			CAnimation* pAni = static_cast<CPlayer3D*>(pPlayer.get())->GetAni();

			pAni->SetSocketOffset("Weapon", pItem->GetSocketOffset());
			pAni->SetSocketRotation("Weapon", pItem->GetSocketRotation());

		}

		const std::shared_ptr<CComponent> pCom = static_cast<CPlayer3D*>(pPlayer.get())->GetWeaponOutLine();

		if (pCom)
		{
			static_cast<CSceneComponent*>(pCom.get())->SetMesh(pItem->GetMeshName());
			static_cast<CSceneComponent*>(pCom.get())->SetShader("OutLineShader");
			static_cast<CSceneComponent*>(pCom.get())->SetWorldScale(1.05f, 1.05f, 1.05f);
			static_cast<CSceneComponent*>(pCom.get())->AddRenderState("OutLine");
		}
	}
	break;
	case EQUIP_TYPE::SHIELD:
		break;
	case EQUIP_TYPE::HELMET:
		GET_SINGLE(CSoundManager)->Play("Equip_Leather");
		iDestContainer = 9;
		break;
	case EQUIP_TYPE::SHOULDER:
		GET_SINGLE(CSoundManager)->Play("Equip_Leather");
		iDestContainer = 5;
		break;
	case EQUIP_TYPE::UPPER:
		GET_SINGLE(CSoundManager)->Play("Equip_LeatherBody");
		iDestContainer = 0;
		break;
	case EQUIP_TYPE::BELT:
		GET_SINGLE(CSoundManager)->Play("Equip_Leather");
		iDestContainer = 1;
		break;
	case EQUIP_TYPE::GLOVE:
		GET_SINGLE(CSoundManager)->Play("Equip_LeatherHand");
		iDestContainer = 3;
		break;
	case EQUIP_TYPE::LOWER:
		GET_SINGLE(CSoundManager)->Play("Equip_Leather");
		iDestContainer = 4;
		break;
	case EQUIP_TYPE::BOOTS:
		GET_SINGLE(CSoundManager)->Play("Equip_LeatherFeet");
		iDestContainer = 2;
		break;
	}

	if ( iDestContainer != -1)
	{
		std::shared_ptr<CComponent> pMesh = static_cast<CPlayer3D*>(pPlayer.get())->GetMesh();
		std::shared_ptr<CComponent> pOutMesh = static_cast<CPlayer3D*>(pPlayer.get())->GetOutLine();

		class CMesh* _pMesh = static_cast<CSceneComponent*>(pMesh.get())->GetMesh();
		class CMesh* _pOutMesh = static_cast<CSceneComponent*>(pOutMesh.get())->GetMesh();

		class CMesh* pSrcMesh = GET_SINGLE(CResourceManager)->FindMesh(pItem->GetMeshName());

		int iContainer = pItem->GetContainer();

		_pMesh->ChangeContainer(pSrcMesh, iDestContainer, iContainer);
		_pOutMesh->ChangeContainer(pSrcMesh, iDestContainer, iContainer);

		static_cast<CSceneComponent*>(pMesh.get())->SetMesh(_pMesh);
		static_cast<CSceneComponent*>(pOutMesh.get())->SetMesh(_pOutMesh);
	}

	if (m_pEquipIcons[iType])
	{
		static_cast<CSceneComponent*>(m_pEquipSlots[iType].get())->DeleteChild(m_pEquipIcons[iType]);
	}

	static_cast<CInventory3D*>(pInven.get())->DeleteItem(iSelect);

	if (m_ppItems[iType])
	{
		static_cast<CInventory3D*>(pInven.get())->AddItem(m_ppItems[iType], m_pEquipIcons[iType]);
	}

	m_pEquipIcons[iType] = pIcon;

	m_ppItems[iType] = pItem;
	static_cast<CSceneComponent*>(m_pEquipSlots[iType].get())->AddChild(m_pEquipIcons[iType]);


}
bool CEquip::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Equip_Iron", false,
		"Equip_Iron.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Equip_Lance", false,
		"Equip_Lance.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Equip_Leather", false,
		"Equip_Leather.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Equip_LeatherBody", false,
		"Equip_LeatherBody.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Equip_LeatherFeet", false,
		"Equip_LeatherFeet.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Equip_LeatherHand", false,
		"Equip_LeatherHand.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Equip_Staff", false,
		"Equip_Staff.ogg");

	if (!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pTitle = CreateComponent<CTitleBar>("EquipTitle");

	SetRootComponent(pTitle);

	static_cast<CTitleBar*>(pTitle.get())->SetTexture("BLUIEquip_I2C");
	static_cast<CTitleBar*>(pTitle.get())->SetUV(Vector2(), Vector2(956.f, 82.f));
	static_cast<CTitleBar*>(pTitle.get())->SetWorldScale(956.f, 82.f, 1.f);
	static_cast<CTitleBar*>(pTitle.get())->SetPivot(0.5f, 0.5f, 0.f);

	std::shared_ptr<CComponent> pTitleFont = static_cast<CTitleBar*>(pTitle.get())->GetFont();

	static_cast<CUFont*>(pTitleFont.get())->SetText(L"¿Â∫Ò");
	static_cast<CUFont*>(pTitleFont.get())->SetSize(30.f);
	static_cast<CUFont*>(pTitleFont.get())->SetRect(-100.f, 0.f, 100.f, 0.f);

	std::shared_ptr<CComponent> pPanel = CreateComponent<CPanel>("EquipPanel");

	static_cast<CTitleBar*>(pTitle.get())->AddChild(pPanel);

	static_cast<CPanel*>(pPanel.get())->SetTexture("BLUIEquip_I2C");
	static_cast<CPanel*>(pPanel.get())->SetUV(Vector2(0.f, 82.f), Vector2(956.f, 757.f));
	static_cast<CPanel*>(pPanel.get())->SetWorldScale(956.f, 757.f, 1.f);
	static_cast<CPanel*>(pPanel.get())->SetRelativePosY(-417.f);
	static_cast<CPanel*>(pPanel.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CPanel*>(pPanel.get())->SetZOrder(2);

	std::shared_ptr<CComponent> pIconBoots = CreateComponent<CUIImage>("BootsSlot");

	static_cast<CPanel*>(pPanel.get())->AddChild(pIconBoots);

	static_cast<CUIImage*>(pIconBoots.get())->SetTexture("BLUIEquip_I2C");
	static_cast<CUIImage*>(pIconBoots.get())->SetUV(Vector2(957.f, 53.f), Vector2(1007.f, 103.f));
	static_cast<CUIImage*>(pIconBoots.get())->SetWorldScale(50.f, 50.f, 1.f);
	static_cast<CUIImage*>(pIconBoots.get())->SetRelativePos(-225.f, -120.f, 1.f);
	static_cast<CUIImage*>(pIconBoots.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pIconBoots.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pBootsCollider = CreateComponent<CColliderRect>("BootsSlotBody");

	static_cast<CUIImage*>(pIconBoots.get())->AddChild(pBootsCollider);

	static_cast<CColliderRect*>(pBootsCollider.get())->SetExtend(50.f, 50.f);
	static_cast<CColliderRect*>(pBootsCollider.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	static_cast<CColliderRect*>(pBootsCollider.get())->SetUI();
	static_cast<CColliderRect*>(pBootsCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_INIT, this, &CEquip::ColInit);
	static_cast<CColliderRect*>(pBootsCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_STAY, this, &CEquip::ColStay);
	static_cast<CColliderRect*>(pBootsCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_LAST, this, &CEquip::ColEnd);

	std::shared_ptr<CComponent> pShoulderImage = CreateComponent<CUIImage>("ShoulderSlot");

	static_cast<CPanel*>(pPanel.get())->AddChild(pShoulderImage);

	static_cast<CUIImage*>(pShoulderImage.get())->SetTexture("BLUIEquip_I2C");
	static_cast<CUIImage*>(pShoulderImage.get())->SetUV(Vector2(957.f, 261.f), Vector2(1007.f, 311.f));
	static_cast<CUIImage*>(pShoulderImage.get())->SetWorldScale(50.f, 50.f, 1.f);
	static_cast<CUIImage*>(pShoulderImage.get())->SetRelativePos(-420.f, 200.f, 1.f);
	static_cast<CUIImage*>(pShoulderImage.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pShoulderImage.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pShoulderCollider = CreateComponent<CColliderRect>("ShoulderSlotBody");

	static_cast<CUIImage*>(pShoulderImage.get())->AddChild(pShoulderCollider);

	static_cast<CColliderRect*>(pShoulderCollider.get())->SetExtend(50.f, 50.f);
	static_cast<CColliderRect*>(pShoulderCollider.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	static_cast<CColliderRect*>(pShoulderCollider.get())->SetUI();
	static_cast<CColliderRect*>(pShoulderCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_INIT, this, &CEquip::ColInit);
	static_cast<CColliderRect*>(pShoulderCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_STAY, this, &CEquip::ColStay);
	static_cast<CColliderRect*>(pShoulderCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_LAST, this, &CEquip::ColEnd);

	std::shared_ptr<CComponent> pLowerImage = CreateComponent<CUIImage>("LowerSlot");

	static_cast<CPanel*>(pPanel.get())->AddChild(pLowerImage);

	static_cast<CUIImage*>(pLowerImage.get())->SetTexture("BLUIEquip_I2C");
	static_cast<CUIImage*>(pLowerImage.get())->SetUV(Vector2(957.f, 313.f), Vector2(1007.f, 363.f));
	static_cast<CUIImage*>(pLowerImage.get())->SetWorldScale(50.f, 50.f, 1.f);
	static_cast<CUIImage*>(pLowerImage.get())->SetRelativePos(-420.f, -20.f, 1.f);
	static_cast<CUIImage*>(pLowerImage.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pLowerImage.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pLowerCollider = CreateComponent<CColliderRect>("LowerSlotBody");

	static_cast<CUIImage*>(pLowerImage.get())->AddChild(pLowerCollider);

	static_cast<CColliderRect*>(pLowerCollider.get())->SetExtend(50.f, 50.f);
	static_cast<CColliderRect*>(pLowerCollider.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	static_cast<CColliderRect*>(pLowerCollider.get())->SetUI();
	static_cast<CColliderRect*>(pLowerCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_INIT, this, &CEquip::ColInit);
	static_cast<CColliderRect*>(pLowerCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_STAY, this, &CEquip::ColStay);
	static_cast<CColliderRect*>(pLowerCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_LAST, this, &CEquip::ColEnd);

	std::shared_ptr<CComponent> pShieldImage = CreateComponent<CUIImage>("ShieldSlot");

	static_cast<CPanel*>(pPanel.get())->AddChild(pShieldImage);

	static_cast<CUIImage*>(pShieldImage.get())->SetTexture("BLUIEquip_I2C");
	static_cast<CUIImage*>(pShieldImage.get())->SetUV(Vector2(957.f, 365.f), Vector2(1007.f, 415.f));
	static_cast<CUIImage*>(pShieldImage.get())->SetWorldScale(50.f, 50.f, 1.f);
	static_cast<CUIImage*>(pShieldImage.get())->SetRelativePos(-30.f, -20.f, 1.f);
	static_cast<CUIImage*>(pShieldImage.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pShieldImage.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pShieldCollider = CreateComponent<CColliderRect>("ShieldSlotBody");

	static_cast<CUIImage*>(pShieldImage.get())->AddChild(pShieldCollider);

	static_cast<CColliderRect*>(pShieldCollider.get())->SetExtend(50.f, 50.f);
	static_cast<CColliderRect*>(pShieldCollider.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	static_cast<CColliderRect*>(pShieldCollider.get())->SetUI();
	static_cast<CColliderRect*>(pShieldCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_INIT, this, &CEquip::ColInit);
	static_cast<CColliderRect*>(pShieldCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_STAY, this, &CEquip::ColStay);
	static_cast<CColliderRect*>(pShieldCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_LAST, this, &CEquip::ColEnd);

	std::shared_ptr<CComponent> pGloveImage = CreateComponent<CUIImage>("GloveSlot");

	static_cast<CPanel*>(pPanel.get())->AddChild(pGloveImage);

	static_cast<CUIImage*>(pGloveImage.get())->SetTexture("BLUIEquip_I2C");
	static_cast<CUIImage*>(pGloveImage.get())->SetUV(Vector2(957.f, 625.f), Vector2(1007.f, 675.f));
	static_cast<CUIImage*>(pGloveImage.get())->SetWorldScale(50.f, 50.f, 1.f);
	static_cast<CUIImage*>(pGloveImage.get())->SetRelativePos(-30.f, 100.f, 1.f);
	static_cast<CUIImage*>(pGloveImage.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pGloveImage.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pGloveCollider = CreateComponent<CColliderRect>("GloveSlotBody");

	static_cast<CUIImage*>(pGloveImage.get())->AddChild(pGloveCollider);

	static_cast<CColliderRect*>(pGloveCollider.get())->SetExtend(50.f, 50.f);
	static_cast<CColliderRect*>(pGloveCollider.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	static_cast<CColliderRect*>(pGloveCollider.get())->SetUI();
	static_cast<CColliderRect*>(pGloveCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_INIT, this, &CEquip::ColInit);
	static_cast<CColliderRect*>(pGloveCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_STAY, this, &CEquip::ColStay);
	static_cast<CColliderRect*>(pGloveCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_LAST, this, &CEquip::ColEnd);

	std::shared_ptr<CComponent> pBeltImage = CreateComponent<CUIImage>("BeltSlot");

	static_cast<CPanel*>(pPanel.get())->AddChild(pBeltImage);

	static_cast<CUIImage*>(pBeltImage.get())->SetTexture("BLUIEquip_I2C");
	static_cast<CUIImage*>(pBeltImage.get())->SetUV(Vector2(957.f, 677.f), Vector2(1007.f, 727.f));
	static_cast<CUIImage*>(pBeltImage.get())->SetWorldScale(50.f, 50.f, 1.f);
	static_cast<CUIImage*>(pBeltImage.get())->SetRelativePos(-30.f, 40.f, 1.f);
	static_cast<CUIImage*>(pBeltImage.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pBeltImage.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pBeltCollider = CreateComponent<CColliderRect>("BeltSlotBody");

	static_cast<CUIImage*>(pBeltImage.get())->AddChild(pBeltCollider);

	static_cast<CColliderRect*>(pBeltCollider.get())->SetExtend(50.f, 50.f);
	static_cast<CColliderRect*>(pBeltCollider.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	static_cast<CColliderRect*>(pBeltCollider.get())->SetUI();
	static_cast<CColliderRect*>(pBeltCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_INIT, this, &CEquip::ColInit);
	static_cast<CColliderRect*>(pBeltCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_STAY, this, &CEquip::ColStay);
	static_cast<CColliderRect*>(pBeltCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_LAST, this, &CEquip::ColEnd);

	std::shared_ptr<CComponent> pUpperImage = CreateComponent<CUIImage>("UpperSlot");

	static_cast<CPanel*>(pPanel.get())->AddChild(pUpperImage);

	static_cast<CUIImage*>(pUpperImage.get())->SetTexture("BLUIEquip_I2C");
	static_cast<CUIImage*>(pUpperImage.get())->SetUV(Vector2(780.f, 768.f), Vector2(830.f, 818.f));
	static_cast<CUIImage*>(pUpperImage.get())->SetWorldScale(50.f, 50.f, 1.f);
	static_cast<CUIImage*>(pUpperImage.get())->SetRelativePos(-420.f, 40.f, 1.f);
	static_cast<CUIImage*>(pUpperImage.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pUpperImage.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pUpperCollider = CreateComponent<CColliderRect>("UpperSlotBody");

	static_cast<CUIImage*>(pUpperImage.get())->AddChild(pUpperCollider);

	static_cast<CColliderRect*>(pUpperCollider.get())->SetExtend(50.f, 50.f);
	static_cast<CColliderRect*>(pUpperCollider.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	static_cast<CColliderRect*>(pUpperCollider.get())->SetUI();
	static_cast<CColliderRect*>(pUpperCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_INIT, this, &CEquip::ColInit);
	static_cast<CColliderRect*>(pUpperCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_STAY, this, &CEquip::ColStay);
	static_cast<CColliderRect*>(pUpperCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_LAST, this, &CEquip::ColEnd);

	std::shared_ptr<CComponent> pHelmetImage = CreateComponent<CUIImage>("HelmetSlot");

	static_cast<CPanel*>(pPanel.get())->AddChild(pHelmetImage);

	static_cast<CUIImage*>(pHelmetImage.get())->SetTexture("BLUIEquip_I2C");
	static_cast<CUIImage*>(pHelmetImage.get())->SetUV(Vector2(468.f, 768.f), Vector2(518.f, 818.f));
	static_cast<CUIImage*>(pHelmetImage.get())->SetWorldScale(50.f, 50.f, 1.f);
	static_cast<CUIImage*>(pHelmetImage.get())->SetRelativePos(-225.f, 285.f, 1.f);
	static_cast<CUIImage*>(pHelmetImage.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pHelmetImage.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pHelmetCollider = CreateComponent<CColliderRect>("HelmetSlotBody");

	static_cast<CUIImage*>(pHelmetImage.get())->AddChild(pHelmetCollider);

	static_cast<CColliderRect*>(pHelmetCollider.get())->SetExtend(50.f, 50.f);
	static_cast<CColliderRect*>(pHelmetCollider.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	static_cast<CColliderRect*>(pHelmetCollider.get())->SetUI();
	static_cast<CColliderRect*>(pHelmetCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_INIT, this, &CEquip::ColInit);
	static_cast<CColliderRect*>(pHelmetCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_STAY, this, &CEquip::ColStay);
	static_cast<CColliderRect*>(pHelmetCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_LAST, this, &CEquip::ColEnd);

	std::shared_ptr<CComponent> pWeaponImage = CreateComponent<CUIImage>("WeaponSlot");

	static_cast<CPanel*>(pPanel.get())->AddChild(pWeaponImage);

	static_cast<CUIImage*>(pWeaponImage.get())->SetTexture("BLUIEquip_I2C");
	static_cast<CUIImage*>(pWeaponImage.get())->SetUV(Vector2(957.f, 573.f), Vector2(1007.f, 623.f));
	static_cast<CUIImage*>(pWeaponImage.get())->SetWorldScale(50.f, 50.f, 1.f);
	static_cast<CUIImage*>(pWeaponImage.get())->SetRelativePos(-420.f, 145.f, 1.f);
	static_cast<CUIImage*>(pWeaponImage.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pWeaponImage.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pWeaponCollider = CreateComponent<CColliderRect>("WeaponSlotBody");

	static_cast<CUIImage*>(pWeaponImage.get())->AddChild(pWeaponCollider);

	static_cast<CColliderRect*>(pWeaponCollider.get())->SetExtend(50.f, 50.f);
	static_cast<CColliderRect*>(pWeaponCollider.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	static_cast<CColliderRect*>(pWeaponCollider.get())->SetUI();
	static_cast<CColliderRect*>(pWeaponCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_INIT, this, &CEquip::ColInit);
	static_cast<CColliderRect*>(pWeaponCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_STAY, this, &CEquip::ColStay);
	static_cast<CColliderRect*>(pWeaponCollider.get())->SetCallBack<CEquip>(COLLISION_STATE::CS_LAST, this, &CEquip::ColEnd);

	m_pEquipSlots[static_cast<int>(EQUIP_TYPE::WEAPON)] = pWeaponImage;
	m_pEquipSlots[static_cast<int>(EQUIP_TYPE::SHIELD)] = pShieldImage;
	m_pEquipSlots[static_cast<int>(EQUIP_TYPE::HELMET)] = pHelmetImage;
	m_pEquipSlots[static_cast<int>(EQUIP_TYPE::SHOULDER)] = pShoulderImage;
	m_pEquipSlots[static_cast<int>(EQUIP_TYPE::UPPER)] = pUpperImage;
	m_pEquipSlots[static_cast<int>(EQUIP_TYPE::BELT)] = pBeltImage;
	m_pEquipSlots[static_cast<int>(EQUIP_TYPE::GLOVE)] = pGloveImage;
	m_pEquipSlots[static_cast<int>(EQUIP_TYPE::LOWER)] = pLowerImage;
	m_pEquipSlots[static_cast<int>(EQUIP_TYPE::BOOTS)] = pIconBoots;

	SetWorldPos(0.f, 400.f, 0.f);

	return true;
}
void CEquip::Start() 
{
	CGameObject::Start();

	CLayer* pLayer = m_pScene->FindLayer(ALBEDO_LAYER);

	m_pPlayer = pLayer->FindObj("Player");
}
void CEquip::Update(float fTime) 
{
	CGameObject::Update(fTime);

	if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
	{
		for (int i = 0; i < static_cast<int>(EQUIP_TYPE::END); i++)
		{
			if (m_pEquipIcons[i])
			{
				static_cast<CSceneComponent*>(m_pEquipIcons[i].get())->SetRelativePos(0.f, 0.f, 0.f);
			}
		}
	}

	Vector3 vPos = GetWorldPos();

	Resolution tRS = RESOLUTION;

	vPos.x -= 220.f;
	vPos.y -= 70.f;

	vPos.x /= tRS.iWidth/2.f;
	vPos.y /= tRS.iHeight / 2.f;

	TransformCBuffer tCBuffer = {};

	tCBuffer.matWVP.Translate(vPos);
	tCBuffer.matWVP.Transpose();

	GET_SINGLE(CRenderManager)->SetEquipBuffer(tCBuffer);
}
void CEquip::PostUpdate(float fTime) 
{
	CGameObject::PostUpdate(fTime);
}
void CEquip::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}
void CEquip::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}
void CEquip::Render(float fTime)
{
	CGameObject::Render(fTime);
}
void CEquip::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}
std::shared_ptr<CGameObject> CEquip::Clone()
{
	return std::shared_ptr<CGameObject>(new CEquip(*this));
}
void CEquip::Save(FILE * pFile)
{
	CGameObject::Save(pFile);
}
void CEquip::Load(FILE * pFile)
{
	CGameObject::Load(pFile);
}

void CEquip::ColInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CEquip::ColStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
	{
		Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();

		Vector3 vPos = GetWorldPos();

		vMousePos.x -= vPos.x;
		vMousePos.y -= vPos.y;

		std::hash<std::string> hs;

		std::string strName = pSrc->GetName();

		size_t iKey = hs(strName);
		int iType = -1;

		if (iKey == hs("BootsSlotBody"))
		{
			iType = static_cast<int>(EQUIP_TYPE::BOOTS);
		}

		else if (iKey == hs("WeaponSlotBody"))
		{
			iType = static_cast<int>(EQUIP_TYPE::WEAPON);
		}

		else if (iKey == hs("ShieldSlotBody"))
		{
			iType = static_cast<int>(EQUIP_TYPE::SHIELD);
		}

		else if (iKey == hs("ShoulderSlotBody"))
		{
			iType = static_cast<int>(EQUIP_TYPE::SHOULDER);
		}

		else if (iKey == hs("UpperSlotBody"))
		{
			iType = static_cast<int>(EQUIP_TYPE::UPPER);
		}

		else if (iKey == hs("BeltSlotBody"))
		{
			iType = static_cast<int>(EQUIP_TYPE::BELT);
		}

		else if (iKey == hs("LowerSlotBody"))
		{
			iType = static_cast<int>(EQUIP_TYPE::LOWER);
		}

		else if (iKey == hs("GloveSlotBody"))
		{
			iType = static_cast<int>(EQUIP_TYPE::GLOVE);
		}

		else if (iKey == hs("HelmetSlotBody"))
		{
			iType = static_cast<int>(EQUIP_TYPE::HELMET);
		}

		SetItem(iType);
	}

	else if(GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
	{
		std::hash<std::string> hs;

		std::string strName = pSrc->GetName();

		size_t iKey = hs(strName);

		if (iKey == hs("BootsSlotBody"))
		{
			m_iSelect = static_cast<int>(EQUIP_TYPE::BOOTS);
		}

		else if (iKey == hs("WeaponSlotBody"))
		{
			m_iSelect = static_cast<int>(EQUIP_TYPE::WEAPON);
		}

		else if (iKey == hs("ShieldSlotBody"))
		{
			m_iSelect = static_cast<int>(EQUIP_TYPE::SHIELD);
		}

		else if (iKey == hs("ShoulderSlotBody"))
		{
			m_iSelect = static_cast<int>(EQUIP_TYPE::SHOULDER);
		}

		else if (iKey == hs("UpperSlotBody"))
		{
			m_iSelect = static_cast<int>(EQUIP_TYPE::UPPER);
		}

		else if (iKey == hs("BeltSlotBody"))
		{
			m_iSelect = static_cast<int>(EQUIP_TYPE::BELT);
		}

		else if (iKey == hs("LowerSlotBody"))
		{
			m_iSelect = static_cast<int>(EQUIP_TYPE::LOWER);
		}

		else if (iKey == hs("GloveSlotBody"))
		{
			m_iSelect = static_cast<int>(EQUIP_TYPE::GLOVE);
		}

		else if (iKey == hs("HelmetSlotBody"))
		{
			m_iSelect = static_cast<int>(EQUIP_TYPE::HELMET);
		}
		else
		{
			m_iSelect = -1;
		}

	}
}

void CEquip::ColEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
