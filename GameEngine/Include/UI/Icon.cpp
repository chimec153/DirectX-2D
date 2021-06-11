#include "Icon.h"
#include "../Component/ColliderRect.h"
#include "../Object/GameObject.h"
#include "../Input.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"

CIcon::CIcon()	:
	m_pRC(nullptr),
	m_bPush(false)
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;

	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_ICON;
}

CIcon::CIcon(const CIcon& ui)	:
	CUIControl(ui)
{
	m_pRC = std::shared_ptr<CComponent>(FindComponentByType<CColliderRect>());

	m_bPush = false;
}

CIcon::~CIcon()
{
}

bool CIcon::IsPush() const
{
	return m_bPush;
}

void CIcon::SetPos(const Vector3& vPos)
{
	m_vPos = vPos;
}

bool CIcon::Init()
{
	if (!CUIControl::Init())
		return false;

	m_pRC = m_pObj->CreateComponent<CColliderRect>("IconBody");

	((CColliderRect*)m_pRC.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	((CColliderRect*)m_pRC.get())->SetUI();

	((CColliderRect*)m_pRC.get())->SetCallBack<CIcon>(COLLISION_STATE::CS_INIT, this, &CIcon::ColEnter);
	((CColliderRect*)m_pRC.get())->SetCallBack<CIcon>(COLLISION_STATE::CS_STAY, this, &CIcon::ColStay);
	((CColliderRect*)m_pRC.get())->SetCallBack<CIcon>(COLLISION_STATE::CS_LAST, this, &CIcon::ColEnd);

	AddChild(m_pRC);

	return true;
}

void CIcon::Start()
{
	CUIControl::Start();
}

void CIcon::Update(float fTime)
{
	CUIControl::Update(fTime);

	Vector3 vScale = GetWorldScale();

	((CColliderRect*)m_pRC.get())->SetExtend(vScale.x, vScale.y);

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

void CIcon::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CIcon::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CIcon::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CIcon::Render(float fTime)
{
	m_pTexture->SetShader(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	CUIControl::Render(fTime);

	//m_pMesh->Render(fTime);
}

void CIcon::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CIcon::Clone()
{
	return std::shared_ptr<CComponent>(new CIcon(*this));
}

void CIcon::Save(FILE* pFile)
{
	CUIControl::Save(pFile);

	fwrite(&m_bPush, 1, 1, pFile);
	fwrite(&m_vPos, 12, 1, pFile);
}

void CIcon::Load(FILE* pFile)
{
	CUIControl::Load(pFile);

	m_pRC = m_pObj->FindComponentByType<CColliderRect>();

	fread(&m_bPush, 1, 1, pFile);
	fread(&m_vPos, 12, 1, pFile);
}

void CIcon::ColEnter(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (!m_bPush)
		{
			if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
				m_bPush = true;
		}
	}
}

void CIcon::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (!m_bPush)
		{
			if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
			{
				m_bPush = true;

				m_vPos = GetRelativePos();
			}
		}
	}

	else if (!m_bPush)
	{
		if (pDst->GetParent() == GetParent())
		{
			SetRelativePos(pDst->GetRelativePos());
		}
	}
}

void CIcon::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void CIcon::Cancle()
{
	SetRelativePos(m_vPos);
}
