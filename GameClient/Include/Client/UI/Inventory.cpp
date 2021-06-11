#include "Inventory.h"
#include "UI/TitleBar.h"
#include "UI/Panel.h"
#include "UI/UIButton.h"
#include "Component/ColliderRect.h"
#include "UI/Icon.h"
#include "Resource/ResourceManager.h"
#include "Input.h"

CInventory::CInventory()	:
	m_pTitle(nullptr),
	m_pCloseBtn(nullptr),
	m_pMainPnl(nullptr)
{
	m_pBlock = new char[40];

	memset(m_pBlock, 0, 40);
}

CInventory::CInventory(const CInventory& obj)	:
	CGameObject(obj)
{ 
	/*if(obj.m_pTitle)
	m_pTitle = obj.m_pTitle->Clone();

	if (obj.m_pCloseBtn)
	m_pCloseBtn = obj.m_pCloseBtn->Clone();

	if (obj.m_pMainPnl)
	m_pMainPnl = obj.m_pMainPnl->Clone();*/
}

CInventory::~CInventory()
{
	SAFE_RELEASE(m_pTitle);
	SAFE_RELEASE(m_pCloseBtn);
	SAFE_RELEASE(m_pMainPnl);
	delete[] m_pBlock;
}

bool CInventory::Init()
{
	if (!CGameObject::Init())
		return false;

/*
	m_pMainPnl = CreateComponent<CPanel>("Panel");
	m_pMainPnl->SetWorldScale(319.f, 352.f, 1.f);
	m_pMainPnl->SetPivot(0.5f, 0.5f, 0.f);
	m_pMainPnl->SetTexture("Inventory");
	m_pMainPnl->SetUV(Vector2(0.f, 0.f), Vector2(319.f, 352.f));
	m_pMainPnl->SetZOrder(2);

	SetRootComponent(m_pMainPnl);

	m_pTitle = CreateComponent<CTitleBar>("Title");
	m_pTitle->SetTexture("Panel");
	m_pTitle->SetWorldScale(299.f, 20.f, 1.f);
	m_pTitle->SetRelativePos(-10.f, 186.f, 0.f);
	m_pTitle->SetPivot(0.5f, 0.5f, 0.f);
	m_pTitle->SetInheritScale(false);

	m_pMainPnl->AddChild(m_pTitle);

	m_pCloseBtn = CreateComponent<CUIButton>("CloseBtn");
	m_pCloseBtn->SetTexture(BUTTON_STATE::OFF, "CloseButton");
	m_pCloseBtn->SetTexture(BUTTON_STATE::ON, "CloseButton");
	m_pCloseBtn->SetTexture(BUTTON_STATE::PRESS, "CloseButton");
	m_pCloseBtn->SetTexture(BUTTON_STATE::DISABLE, "CloseButton");
	m_pCloseBtn->SetWorldScale(20.f, 20.f, 1.f);
	m_pCloseBtn->SetRelativePos(149.5, 186.f, 0.f);
	m_pCloseBtn->SetPivot(0.5f, 0.5f, 0.f);
	m_pCloseBtn->SetCallBack<CInventory>(BUTTON_STATE::PRESS, this, &CInventory::Close);
	m_pCloseBtn->SetInheritScale(false);

	m_pMainPnl->AddChild(m_pCloseBtn);

	CColliderRect* pRC = CreateComponent<CColliderRect>("ArmSlot");

	pRC->SetExtend(54.f, 81.f);
	pRC->SetRelativePos(-112.5f, 59.f, 0.f);
	pRC->SetUI();
	pRC->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	m_pMainPnl->AddChild(pRC);

	SAFE_RELEASE(pRC);

	pRC = CreateComponent<CColliderRect>("ArmorSlot");

	pRC->SetExtend(54.f, 81.f);
	pRC->SetRelativePos(2.5f, 59.f, 0.f);
	pRC->SetUI();
	pRC->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	m_pMainPnl->AddChild(pRC);

	SAFE_RELEASE(pRC);

	pRC = CreateComponent<CColliderRect>("ShieldSlot");

	pRC->SetExtend(54.f, 81.f);
	pRC->SetRelativePos(117.5f, 59.f, 0.f);
	pRC->SetUI();
	pRC->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	m_pMainPnl->AddChild(pRC);

	SAFE_RELEASE(pRC);

	CPanel* pPanel = CreateComponent<CPanel>("InvenPanel");

	pPanel->SetTexture("Panel");
	pPanel->SetZOrder(2);

	pRC = CreateComponent<CColliderRect>("Inven");

	pRC->SetExtend(290.f, 116.f);
	pRC->SetRelativePos(0.f, -103.f, 0.f);
	pRC->SetUI();
	pRC->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	pRC->SetCallBack<CInventory>(COLLISION_STATE::CS_INIT, this, &CInventory::ColEnter);
	pRC->SetCallBack<CInventory>(COLLISION_STATE::CS_STAY, this, &CInventory::ColStay);
	pRC->SetCallBack<CInventory>(COLLISION_STATE::CS_LAST, this, &CInventory::ColEnd);

	pPanel->AddChild(pRC);

	m_pMainPnl->AddChild(pPanel);

	SAFE_RELEASE(pRC);

	SAFE_RELEASE(pPanel);

	CIcon* pIcon = CreateComponent<CIcon>("Armor");

	pIcon->SetWorldScale(56.f, 81.f, 1.f);
	pIcon->SetPivot(0.5f, 0.5f, 0.f);
	pIcon->SetTexture("Items");
	pIcon->SetUV(Vector2(1.f, 20.f), Vector2(57.f, 104.f));
	pIcon->SetInheritScale(false);

	m_pMainPnl->AddChild(pIcon);

	SAFE_RELEASE(pIcon);

	pIcon = CreateComponent<CIcon>("Weapon");

	pIcon->SetWorldScale(56.f, 81.f, 1.f);
	pIcon->SetPivot(0.5f, 0.5f, 0.f);
	pIcon->SetTexture("Items");
	pIcon->SetUV(Vector2(5.f, 126.f), Vector2(60.f, 209.f));
	pIcon->SetInheritScale(false);

	m_pMainPnl->AddChild(pIcon);

	SAFE_RELEASE(pIcon);

	pIcon = CreateComponent<CIcon>("Bow");

	pIcon->SetWorldScale(56.f, 81.f, 1.f);
	pIcon->SetPivot(0.5f, 0.5f, 0.f);
	pIcon->SetTexture("Items");
	pIcon->SetUV(Vector2(544.f, 497.f), Vector2(599.f, 580.f));
	pIcon->SetInheritScale(false);

	m_pMainPnl->AddChild(pIcon);

	SAFE_RELEASE(pIcon);*/

	return true;
}

void CInventory::Start()
{
	CGameObject::Start();
}

void CInventory::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CInventory::PostUpdate(float fTime)
{
	memset(m_pBlock, 0, 40);

	CGameObject::PostUpdate(fTime);
}

void CInventory::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CInventory::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CInventory::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CInventory::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CInventory::Clone()
{
	return std::shared_ptr<CGameObject>(new CInventory(*this));
}

void CInventory::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CInventory::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CInventory::Close(float fTime)
{
	SetEnable(false);
}

void CInventory::ColEnter(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void CInventory::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
	CIcon* pIcon = (CIcon*)pDst->GetParent();

	if (pIcon->GetSceneComponentClassType() == (int)SCENECOMPONENT_CLASS_TYPE::UI_ICON)
	{
		bool bPush = pIcon->IsPush();

		if (!bPush && GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
		{
			RectInfo tInfo = ((CColliderRect*)pDst)->GetInfo();

			float fSzX = (tInfo.fR - tInfo.fL) / 27.f;
			float fSzY = (tInfo.fT - tInfo.fB) / 27.f;

			int iSzX = fSzX > (int)fSzX ? (int)fSzX + 1: (int)fSzX;
			int iSzY = fSzY > (int)fSzY ? (int)fSzY + 1 : (int)fSzY;

			int x;
			int y;

			RectInfo tSrc = ((CColliderRect*)pSrc)->GetInfo();

			RectInfo tIdx = tInfo - tSrc;

			x = (int)(tIdx.fL / 24.f);
			y = (int)(-tIdx.fT / 24.f);


			if (x < 0)
				x = 0;

			if (y < 0)
				y = 0;

			if (x + iSzX > 10)
				x = 10 - iSzX;

			if (y + iSzY > 4)
				y = 4 - iSzY;

			for (int i = x; i < x + iSzX; ++i)
			{
				for (int j = y; j < y + iSzY; ++j)
				{
					if (m_pBlock[i + j * 10] != 0)
					{
						pIcon->Cancle();
						return;
					}
				}
			}

			Vector3 vPos = Vector3(-142.5f, -47.f, 0.f) + Vector3(x * 29.f + (iSzX / 2.f + 0.5f) * 29.f , -y * 29.f - iSzY / 2.f * 29.f , 0.f);

			pIcon->SetRelativePos(vPos);

			pIcon->SetPos(vPos);

			for (int i = x; i < x + iSzX; ++i)
			{
				for (int j = y; j < y + iSzY; ++j)
					m_pBlock[i + j * 10] = 1;
			}
		}
	}
}

void CInventory::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
}
