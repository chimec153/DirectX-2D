#include "TitleBar.h"
#include "../Component/Collider.h"
#include "../Component/ColliderRect.h"
#include "../Input.h"
#include "../Object/GameObject.h"
#include "../Resource/Material.h"
#include "../Resource/Texture.h"
#include "../Resource/Mesh2D.h"
#include "UFont.h"

CTitleBar::CTitleBar()	:
	m_bPush(false),
	m_pRC(nullptr),
	m_pTitle(nullptr)
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_TITLE;
}

CTitleBar::CTitleBar(const CTitleBar& com)	:
	CUIControl(com)
{
	m_bPush = false;

	m_pRC = std::shared_ptr<CComponent>(FindComponentByType<CColliderRect>());
	m_pTitle = std::shared_ptr<CComponent>(FindComponentByType<CUFont>());
}

CTitleBar::~CTitleBar()
{
}

void CTitleBar::SetTitle(const TCHAR* pText)
{
	((CUFont*)m_pTitle.get())->SetText(pText);
}

void CTitleBar::SetColor(float r, float g, float b, float a)
{
	((CUFont*)m_pTitle.get())->SetColor(r,g,b,a);
}

void CTitleBar::SetTextPivot(float x, float y, float z)
{
	((CUFont*)m_pTitle.get())->SetPivot(x, y, z);
}

void CTitleBar::SetTextSize(float fSize)
{
	((CUFont*)m_pTitle.get())->SetSize(fSize);
}

const std::shared_ptr<CComponent>& CTitleBar::GetFont() const
{
	return m_pTitle;
}

bool CTitleBar::Init()
{
	if (!CUIControl::Init())
		return false;

	m_pRC = m_pObj->CreateComponent<CColliderRect>("TitleBarBody");
	
	((CColliderRect*)m_pRC.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	((CColliderRect*)m_pRC.get())->SetUI();

	AddChild(m_pRC);

	m_pTitle = m_pObj->CreateComponent<CUFont>("Title");
	((CUFont*)m_pTitle.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	AddChild(m_pTitle);

	return true;
}

void CTitleBar::Start()
{
	CUIControl::Start();

	m_pRC = FindComponentByType<CColliderRect>();

	m_pTitle = FindComponentByType<CUFont>();

	((CColliderRect*)m_pRC.get())->SetCallBack<CTitleBar>(COLLISION_STATE::CS_INIT, this, &CTitleBar::ColEnter);
	((CColliderRect*)m_pRC.get())->SetCallBack<CTitleBar>(COLLISION_STATE::CS_STAY, this, &CTitleBar::ColStay);
	((CColliderRect*)m_pRC.get())->SetCallBack<CTitleBar>(COLLISION_STATE::CS_LAST, this, &CTitleBar::ColEnd);
}

void CTitleBar::Update(float fTime)
{
	CUIControl::Update(fTime);

	Vector3 vScale = GetWorldScale();

	((CColliderRect*)m_pRC.get())->SetExtend(vScale.x, vScale.y);

	((CUFont*)m_pTitle.get())->SetZOrder(GetZOrder() - 1);

	if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
	{
		if (m_bPush)
		{
			m_bPush = false;
		}
	}

	else if (GET_SINGLE(CInput)->IsPress(MOUSE_TYPE::LEFT))
	{
		if (m_bPush)
		{
			Vector2 vMove = GET_SINGLE(CInput)->GetMouseMove();

			AddWorldPos(vMove.x, vMove.y, 0.f);
		}
	}
}

void CTitleBar::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CTitleBar::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CTitleBar::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CTitleBar::Render(float fTime)
{
	m_pTexture->SetShader(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	CUIControl::Render(fTime);

	//m_pMesh->Render(fTime);
}

void CTitleBar::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CTitleBar::Clone()
{
	return std::shared_ptr<CComponent>(new CTitleBar(*this));
}

void CTitleBar::Save(FILE* pFile)
{
	CUIControl::Save(pFile);
}

void CTitleBar::Load(FILE* pFile)
{
	CUIControl::Load(pFile);
}

void CTitleBar::ColEnter(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
		{
			m_bPush = true;

			Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();

			m_vStart = Vector3(vMousePos.x, vMousePos.y, 0.f);
		}
	}
}

void CTitleBar::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (!m_bPush)
		{
			if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
			{
				m_bPush = true;

				Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();

				m_vStart = Vector3(vMousePos.x, vMousePos.y, 0.f);
			}
		}
	}
}

void CTitleBar::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
}
