#include "UIList.h"
#include "UFont.h"
#include "../Object/GameObject.h"
#include "Panel.h"
#include "Slider.h"
#include "UIButton.h"
#include "../Component/ColliderRect.h"
#include "../Device.h"

CUIList::CUIList()	:
	m_pRC(nullptr)
{
	m_tExtent = Vector2(100.f, 20.f);
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_LIST;
}

CUIList::CUIList(const CUIList& ui)	:
	CUIControl(ui)
{
}

CUIList::~CUIList()
{
	SAFE_DELETE_ARRAY_VECLIST(m_strList);
}

void CUIList::AddString(const std::string& str)
{
	TCHAR* pStr = new TCHAR[str.length() + 1];

	memset(pStr, 0, sizeof(TCHAR) * (str.length() + 1));

	const char* pText = str.c_str();

#ifdef _UNICODE
	MultiByteToWideChar(CP_ACP, 0, pText, -1, pStr, (int)str.length());
#else
	strcpy_s(pStr, pText);
#endif

	m_strList.push_back(pStr);

	std::shared_ptr<CComponent> pFont = m_pObj->CreateComponent<CUFont>("Font");

	Vector3 vPos = GetWorldPos() - GetPivot() * Vector3(m_tExtent.x, m_tExtent.y, 0.f);

	((CUFont*)pFont.get())->SetRect(vPos.x, vPos.y + m_tExtent.y + m_FontList.size() * m_tExtent.y, 
		vPos.x + m_tExtent.x, vPos.y + m_FontList.size() * m_tExtent.y);
	((CUFont*)pFont.get())->SetText(pStr);

	m_FontList.push_back(pFont);

	((CUFont*)pFont.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUFont*)pFont.get())->SetColor(0.f, 0.f, 0.f, 0.f);

	AddChild(pFont);
}

void CUIList::SetExtent(float x, float y)
{
	m_tExtent.x = x;
	m_tExtent.y = y;

	((CSceneComponent*)m_pSel.get())->SetWorldScale(m_tExtent.x, m_tExtent.y, 0.f);
}

bool CUIList::Init()
{
	if (!CUIControl::Init())
		return false;

	m_pPanel = m_pObj->CreateComponent<CPanel>("ListPanel");
	((CPanel*)m_pPanel.get())->SetTexture("PanelBack");
	((CPanel*)m_pPanel.get())->SetZOrder(3);
	((CPanel*)m_pPanel.get())->SetPivot(0.5f, 0.5f, 0.f);

	AddChild(m_pPanel);

	m_pSlider = m_pObj->CreateComponent<CSlider>("ListSlider");
	((CSlider*)m_pSlider.get())->SetWorldScale(20.f, 40.f, 1.f);
	((CSlider*)m_pSlider.get())->SetTexture(BUTTON_STATE::OFF, "Slider");
	((CSlider*)m_pSlider.get())->SetTexture(BUTTON_STATE::ON, "Slider");
	((CSlider*)m_pSlider.get())->SetTexture(BUTTON_STATE::PRESS, "Slider");
	((CSlider*)m_pSlider.get())->SetInheritScale(false);
	((CSlider*)m_pSlider.get())->SetPivot(0.5f, 0.5f, 0.f);

	std::shared_ptr<CComponent> pCom (((CSlider*)m_pSlider.get())->GetParent());

	AddChild(pCom);

	m_pRC = m_pObj->CreateComponent<CColliderRect>("ListBody");

	((CColliderRect*)m_pRC.get())->SetUI();
	((CColliderRect*)m_pRC.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	((CColliderRect*)m_pRC.get())->SetCallBack<CUIList>(COLLISION_STATE::CS_INIT, this, &CUIList::ColEnter);
	((CColliderRect*)m_pRC.get())->SetCallBack<CUIList>(COLLISION_STATE::CS_STAY, this, &CUIList::ColStay);
	((CColliderRect*)m_pRC.get())->SetCallBack<CUIList>(COLLISION_STATE::CS_LAST, this, &CUIList::ColEnd);

	AddChild(m_pRC);

	m_pSel = m_pObj->CreateComponent<CPanel>("Select");
	((CPanel*)m_pSel.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CPanel*)m_pSel.get())->SetTexture("ListSelect");
	((CPanel*)m_pSel.get())->SetInheritScale(false);
	((CPanel*)m_pSel.get())->SetWorldScale(m_tExtent.x, m_tExtent.y, 0.f);
	((CPanel*)m_pSel.get())->SetZOrder(2);

	((CPanel*)m_pPanel.get())->AddChild(m_pSel);

	return true;
}

void CUIList::Start()
{
	CUIControl::Start();
}

void CUIList::Update(float fTime)
{
	CUIControl::Update(fTime);

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_FontList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_FontList.end();

	size_t iFontSize = m_FontList.size();

	Vector3 vScale = GetWorldScale();

	float fPercent = ((CSlider*)m_pSlider.get())->GetPercent();

	int iShowCount = (int)(vScale.y / 20.f);

	int iStartY = (int)iFontSize - iShowCount - (int)(fPercent * ((int)iFontSize - iShowCount));

	Vector3 vPos = GetWorldPos() - GetPivot() * Vector3(m_tExtent.x, m_tExtent.y * (iShowCount + 2), 0.f);

	int i = 0;

	for (; iter != iterEnd; ++iter)
	{
		if (i >= iStartY && i < iStartY + iShowCount)
		{
			(*iter)->SetEnable(true);
			((CUFont*)(*iter).get())->SetWorldPos(vPos + Vector3(0.f, m_tExtent.y * (i - iStartY + 1), 0.f));
			((CUFont*)(*iter).get())->SetRect(vPos.x, vPos.y + m_tExtent.y + iFontSize * m_tExtent.y,
				vPos.x + m_tExtent.x, vPos.y + iFontSize * m_tExtent.y);
		}

		else
			(*iter)->SetEnable(false);

		++i;
	}

	((CPanel*)m_pPanel.get())->SetWorldScale(vScale.x, vScale.y, 1.f);

	((CColliderRect*)m_pRC.get())->SetExtend(vScale.x, vScale.y);

	if (m_iSel >= iStartY && m_iSel < iStartY + iShowCount)
	{
		((CPanel*)m_pSel.get())->SetEnable(true);
		((CPanel*)m_pSel.get())->SetRelativePos(0.f, (m_iSel - iShowCount / 2 - iStartY) * m_tExtent.y + m_tExtent.y / 2.f * ((iShowCount+1)%2), 0.f);
	}

	else
		m_pSel->SetEnable(false);

	((CPanel*)m_pSel.get())->SetWorldScale(vScale.x, m_tExtent.y, 1.f);
}

void CUIList::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CUIList::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CUIList::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CUIList::Render(float fTime)
{
	CUIControl::Render(fTime);

	
}

void CUIList::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CUIList::Clone()
{
	return std::shared_ptr<CComponent>(new CUIList(*this));
}

void CUIList::Save(FILE* pFile)
{
	CUIControl::Save(pFile);
}

void CUIList::Load(FILE* pFile)
{
	CUIControl::Load(pFile);
}

void CUIList::ColEnter(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{

	}
}

void CUIList::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
		{
			Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();

			Vector2 vRatio = RATIO;

			vMousePos /= vRatio;

			Vector3 vScale = GetWorldScale();

			float fPercent = ((CSlider*)m_pSlider.get())->GetPercent();

			int iShowCount = (int)(vScale.y / 20.f);

			size_t iFontSize = m_FontList.size();

			int iStartY = (int)iFontSize - iShowCount - (int)(fPercent * ((int)iFontSize - iShowCount));

			Vector3 vPos = GetWorldPos() - GetWorldScale() * GetPivot();

			vMousePos.y -= vPos.y;

			//vMousePos.y *= -1.f;

			vMousePos.y /= m_tExtent.y;

			m_iSel = (int)vMousePos.y + iStartY;
		}
	}
}

void CUIList::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
}
