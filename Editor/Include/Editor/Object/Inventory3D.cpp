#include "Inventory3D.h"
#include "UI/TitleBar.h"
#include "Resource/ResourceManager.h"
#include "UI/Panel.h"
#include "UI/Icon.h"
#include "Input.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Equip.h"
#include "../Item.h"
#include "UI/UFont.h"
#include "UI/UIImage.h"
#include "Player3D.h"

CInventory3D::CInventory3D()	:
	m_pItems()
	, m_pIcons()
	, m_iSelect(-1)
{
	m_iObjType = static_cast<int>(EDITOR_OBJECT_TYPE::INVENTORY);
}

CInventory3D::CInventory3D(const CInventory3D& inven)	:
	CGameObject(inven)
	, m_pItems()
	, m_pIcons()
	, m_iSelect(-1)
{
}

CInventory3D::~CInventory3D()
{
}

void CInventory3D::DeleteItem(const std::string& strKey, int iCount)
{
	int iIndex = FindItem(strKey);

	if (iIndex == -1)
		return;

	DeleteItem(iIndex);	
}

void CInventory3D::DeleteItem(int iIndex)
{
	if (iIndex == -1)
		return;

	static_cast<CPanel*>(m_pPanel.get())->DeleteChild(m_pIcons[iIndex]);
	m_pIcons[iIndex] = nullptr;
	m_pItems[iIndex] = 0;
}

bool CInventory3D::AddItem(CItem* pItem)
{
	if (!pItem)
		return false;

	for (int i = 0; i < MAX_INVEN; i++)
	{
		if (!m_pItems[i])
		{
			m_pItems[i] = pItem;

			Vector2 vIndex = {};
			const char* pName = nullptr;

			if (pItem)
			{
				vIndex = pItem->GetIndex();
				pName = pItem->GetTextureName().c_str();
			}
			std::shared_ptr<CComponent> pIcon = CreateComponent<CIcon>("Item");

			static_cast<CPanel*>(m_pPanel.get())->AddChild(pIcon);

			if (!static_cast<CIcon*>(pIcon.get())->SetTexture(pName))
				return false;
			static_cast<CIcon*>(pIcon.get())->SetUV(Vector2(48.f * vIndex.x, 48.f * vIndex.y),
				Vector2(48.f * (vIndex.x + 1.f), 48.f * (vIndex.y + 1.f)));
			static_cast<CIcon*>(pIcon.get())->SetPivot(0.5f, 0.5f, 0.f);
			static_cast<CIcon*>(pIcon.get())->SetWorldScale(48.f, 48.f, 0.f);
			Vector2 vPos = GetSlotPos(i);
			static_cast<CIcon*>(pIcon.get())->SetRelativePos(vPos.x, vPos.y, 0.f);

			if (m_pIcons[i])
				m_pIcons[i]->Destroy();

			m_pIcons[i] = pIcon;

			return true;
		}
	}

	return false;
}

bool CInventory3D::AddItem(CItem* pItem, const std::shared_ptr<CComponent>& pIcon)
{
	if (!pItem)
		return false;

	for (int i = 0; i < MAX_INVEN; i++)
	{
		if (!m_pItems[i])
		{
			m_pItems[i] = pItem;

			static_cast<CPanel*>(m_pPanel.get())->AddChild(pIcon);

			Vector2 vPos = GetSlotPos(i);
			static_cast<CIcon*>(pIcon.get())->SetRelativePos(vPos.x, vPos.y, 0.f);

			if (m_pIcons[i])
				m_pIcons[i]->Destroy();

			m_pIcons[i] = pIcon;

			return true;
		}
	}

	return false;
}

bool CInventory3D::HasItem(const std::string& strKey) const
{
	CItem* pItem =  CItem::FindItem(strKey);

	if (!pItem)
		return false;

	for (int i = 0; i < MAX_INVEN; i++)
	{
		if (m_pItems[i] == pItem)
			return true;
	}

	return false;
}

int CInventory3D::FindItem(const std::string& strKey) const
{
	CItem* pItem = CItem::FindItem(strKey);

	if (!pItem)
		return -1;

	for (int i = 0; i < MAX_INVEN; i++)
	{
		if (m_pItems[i] == pItem)
			return i;
	}

	return -1;
}

bool CInventory3D::Init()
{
	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_176", L"IconSheet\\Texture2D\\icon_sheet_176.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_226", L"IconSheet\\Texture2D\\icon_sheet_226.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_276", L"IconSheet\\Texture2D\\icon_sheet_276.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_326", L"IconSheet\\Texture2D\\icon_sheet_326.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_376", L"IconSheet\\Texture2D\\icon_sheet_376.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_426", L"IconSheet\\Texture2D\\icon_sheet_426.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_476", L"IconSheet\\Texture2D\\icon_sheet_476.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_626", L"IconSheet\\Texture2D\\icon_sheet_626.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_9202", L"IconSheet\\Texture2D\\icon_sheet_9202.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("32_UI2_Waypoint_Manager_Start_icon", L"UI_Share_Etc\\Texture2D\\32_UI2_Waypoint_Manager_Start_icon.dds");

	if(!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pTitleBar = CreateComponent<CTitleBar>("InventoryTitle");

	SetRootComponent(pTitleBar);

	static_cast<CTitleBar*>(pTitleBar.get())->SetTexture("BLUIInventory_I4F");
	static_cast<CTitleBar*>(pTitleBar.get())->SetUV(Vector2(), Vector2(521.f, 77.f));
	static_cast<CTitleBar*>(pTitleBar.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CTitleBar*>(pTitleBar.get())->SetWorldScale(521.f, 77.f, 0.f);
	std::shared_ptr<CComponent> pTitleFont = static_cast<CTitleBar*>(pTitleBar.get())->GetFont();

	static_cast<CUFont*>(pTitleFont.get())->SetText(L"인벤토리");
	static_cast<CUFont*>(pTitleFont.get())->SetRect(-100.f, 0.f, 100.f, 0.f);
	static_cast<CUFont*>(pTitleFont.get())->SetSize(30.f);

	m_pPanel = CreateComponent<CPanel>("InventoryPanel");

	static_cast<CTitleBar*>(pTitleBar.get())->AddChild(m_pPanel);

	static_cast<CPanel*>(m_pPanel.get())->SetTexture("BLUIInventory_I4F");
	static_cast<CPanel*>(m_pPanel.get())->SetUV(Vector2(0.f, 77.f), Vector2(521.f, 790.f));
	static_cast<CPanel*>(m_pPanel.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CPanel*>(m_pPanel.get())->SetWorldScale(521.f, 713.f, 0.f);
	static_cast<CPanel*>(m_pPanel.get())->SetRelativePos(1.f, -393.f,0.f);
	static_cast<CPanel*>(m_pPanel.get())->SetZOrder(2);

	std::shared_ptr<CComponent> pMoneyImage = CreateComponent<CUIImage>("MoneyImage");

	static_cast<CPanel*>(m_pPanel.get())->AddChild(pMoneyImage);

	static_cast<CUIImage*>(pMoneyImage.get())->SetTexture("32_UI2_Waypoint_Manager_Start_icon");
	static_cast<CUIImage*>(pMoneyImage.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CUIImage*>(pMoneyImage.get())->SetUV(Vector2(131.f, 865.f), Vector2(147.f, 881.f));
	static_cast<CUIImage*>(pMoneyImage.get())->SetWorldScale(16.f, 16.f, 1.f);
	static_cast<CUIImage*>(pMoneyImage.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pMoneyImage.get())->SetRelativePos(-200.f, -300.f, 0.f);

	m_pMoneyFont = CreateComponent<CUFont>("MoneyFont");

	static_cast<CUIImage*>(pMoneyImage.get())->AddChild(m_pMoneyFont);

	static_cast<CUFont*>(m_pMoneyFont.get())->SetText(L"0");
	static_cast<CUFont*>(m_pMoneyFont.get())->SetRelativePosX(60.f);
	static_cast<CUFont*>(m_pMoneyFont.get())->SetRect(-100.f, 0.f, 100.f, 0.f);
	static_cast<CUFont*>(m_pMoneyFont.get())->SetSize(18.f);

	SetWorldPos(0.f, 400.f, 0.f);

	return true;
}

void CInventory3D::Start()
{
	CGameObject::Start();
}

void CInventory3D::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
	{
		Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();
		int iSelect = GetSlotIndex(vMousePos);

		if (iSelect != -1)
		{
			if (!m_pItems[iSelect])
			{
				 CLayer* pLayer= m_pScene->FindLayer(PARTICLE_LAYER);

				 if (pLayer)
				 {
					 std::shared_ptr<CGameObject> pEquip = pLayer->FindObj("Equip");

					 if (pEquip)
					 {
						 int iType = static_cast<CEquip*>(pEquip.get())->GetSelect();

						 if (iType != -1)
						 {
							 std::shared_ptr<CComponent> pIcon = static_cast<CEquip*>(pEquip.get())->GetItemIcon(iType);
							 if (pIcon)
							 {
								 class CItem* pItem = static_cast<CEquip*>(pEquip.get())->GetItemIndex(iType);
								 m_pIcons[iSelect] = pIcon;
								 m_pItems[iSelect] = pItem;
								 static_cast<CEquip*>(pEquip.get())->DeleteIcon(iType);
								 Vector2 vPos = GetSlotPos(iSelect);
								 static_cast<CIcon*>(pIcon.get())->SetRelativePos(vPos.x, vPos.y, 0.f);
								 static_cast<CEquip*>(pEquip.get())->SetSelect(-1);
								 static_cast<CSceneComponent*>(m_pPanel.get())->AddChild(pIcon);
							 }
						 }
					 }
				 }
			}
		}
	}

	std::shared_ptr<CGameObject> pPlayer = m_pScene->FindLayer(ALBEDO_LAYER)->FindObj("Player");

	if (pPlayer)
	{
		int iGold = static_cast<CPlayer3D*>(pPlayer.get())->GetGold();

		TCHAR strMoney[MAX_PATH] = {};

		swprintf_s(strMoney, MAX_PATH, L"%d", iGold);

		static_cast<CUFont*>(m_pMoneyFont.get())->SetText(strMoney);
	}
}

void CInventory3D::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CInventory3D::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CInventory3D::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);

	if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
	{
		Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();

		m_iSelect = GetSlotIndex(vMousePos);
	}

	else if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
	{
		if (m_iSelect != -1)
		{
			Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();

			int iIdx = GetSlotIndex(vMousePos);

			if (iIdx != -1)
			{
				class CItem* pTempItem = m_pItems[m_iSelect];

				m_pItems[m_iSelect] = m_pItems[iIdx];

				m_pItems[iIdx] = pTempItem;

				std::shared_ptr<CComponent> pTemp = m_pIcons[m_iSelect];

				m_pIcons[m_iSelect] = m_pIcons[iIdx];

				m_pIcons[iIdx] = pTemp;

				Vector2 vPos = GetSlotPos(m_iSelect);
				Vector2 vChangePos = GetSlotPos(iIdx);
				if (m_pIcons[m_iSelect])
				{
					static_cast<CSceneComponent*>(m_pIcons[m_iSelect].get())->SetRelativePos(vPos.x, vPos.y, 0.f);
				}
				if (m_pIcons[iIdx])
				{
					static_cast<CSceneComponent*>(m_pIcons[iIdx].get())->SetRelativePos(vChangePos.x, vChangePos.y, 0.f);
				}
			}
			else
			{
				Vector2 vPos = GetSlotPos(m_iSelect);
				if (m_pIcons[m_iSelect])
				{
					static_cast<CSceneComponent*>(m_pIcons[m_iSelect].get())->SetRelativePos(vPos.x, vPos.y, 0.f);
				}
			}

			m_iSelect = -1;
		}
	}
}

void CInventory3D::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CInventory3D::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<class CGameObject> CInventory3D::Clone()
{
	return std::shared_ptr<class CGameObject>(new CInventory3D(*this));
}

void CInventory3D::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CInventory3D::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

int CInventory3D::GetSlotIndex(const Vector2& vPos)
{
	Vector3 _vPos = static_cast<CSceneComponent*>(m_pPanel.get())->GetWorldPos();

	Resolution tRS = RESOLUTION;

	Vector2 vSelectPos = vPos;

	vSelectPos.x -= _vPos.x + tRS.iWidth / 2.f;
	vSelectPos.y -= _vPos.y + tRS.iHeight / 2.f;

	vSelectPos.x /= 55.f;
	vSelectPos.y /= 55.f;
	vSelectPos.x += INVEN_COLS / 2;
	vSelectPos.y += INVEN_ROWS / 2;

	vSelectPos.y = INVEN_ROWS - vSelectPos.y;

	int iX= static_cast<int>(vSelectPos.x);
	int iY= static_cast<int>(vSelectPos.y);

	if (iX > 7.f || iX < 0.f || 
		iY > 9.f || iY < 0.f)
		return -1;

	int iIdx = iY * INVEN_COLS + iX;

	return iIdx;
}

const Vector2 CInventory3D::GetSlotPos(int iIdx)
{
	float fX = static_cast<float>(iIdx % INVEN_COLS);
	float fY = static_cast<float>(iIdx / INVEN_COLS);

	fY = INVEN_ROWS - fY;

	fX -= INVEN_COLS / 2;
	fY -= INVEN_ROWS / 2;
	fX *= 55.f;
	fY *= 55.f;

	return Vector2(fX + 34.f, fY - 18.f);
}
