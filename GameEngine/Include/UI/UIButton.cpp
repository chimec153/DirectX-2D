#include "UIButton.h"
#include "../Resource/Texture.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"
#include "../Input.h"
#include "../Component/Collider.h"
#include "../Component/ColliderRect.h"
#include "../Object/GameObject.h"

CUIButton::CUIButton()	:
	m_eState(BUTTON_STATE::OFF),
	m_pRC(nullptr)
{
	m_vecInfo.resize((int)BUTTON_STATE::END);

	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;

	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_BUTTON;
}

CUIButton::CUIButton(const CUIButton& ui)	:
	CUIControl(ui)
{
	m_eState = ui.m_eState;

	m_vecInfo.resize((int)BUTTON_STATE::END);

	for (int i = 0; i < (int)BUTTON_STATE::END; ++i)
		m_vecInfo[i].pTex = ui.m_vecInfo[i].pTex;

	if (ui.m_pRC)
		m_pRC = ui.m_pRC->Clone();

	else
		m_pRC = nullptr;
}

CUIButton::~CUIButton()
{
}

void CUIButton::SetTexture(BUTTON_STATE eState, std::shared_ptr<CTexture> pTex)
{
	m_vecInfo[(int)eState].pTex = pTex;

	if (m_vecInfo[(int)eState].pTex)
	{
		((CColliderRect*)m_pRC.get())->SetExtend((float)m_vecInfo[(int)eState].pTex->GetWidth(), (float)m_vecInfo[(int)eState].pTex->GetHeight());
	}
}

void CUIButton::SetTexture(BUTTON_STATE eState, const std::string& strKey)
{
	m_vecInfo[(int)eState].pTex = GET_SINGLE(CResourceManager)->FindTexture(strKey);

	if (m_vecInfo[(int)eState].pTex)
		((CColliderRect*)m_pRC.get())->SetExtend((float)m_vecInfo[(int)eState].pTex->GetWidth(), (float)m_vecInfo[(int)eState].pTex->GetHeight());
}

void CUIButton::SetColor(BUTTON_STATE eState, const Vector4& vColor)
{
	m_vecInfo[(int)eState].vDif = vColor;
}

void CUIButton::SetColor(BUTTON_STATE eState, float r, float g, float b, float a)
{
	m_vecInfo[(int)eState].vDif = Vector4(r,g,b,a);
}

BUTTON_STATE CUIButton::GetState() const
{
	return m_eState;
}

void CUIButton::SetCallBack(BUTTON_STATE eState, void(*pFunc)(float))
{
	m_vecCallBack[(int)eState].push_back(std::bind(pFunc, std::placeholders::_1));
}

void CUIButton::SetButtonCallBack(BUTTON_STATE eState, void(*pFunc)(CUIButton*))
{
	m_vecBtnCallBack[(int)eState].push_back(std::bind(pFunc, std::placeholders::_1));
}

void CUIButton::Call(float fTime)
{
	size_t iSz = m_vecCallBack[(int)m_eState].size();

	for (size_t i = 0; i < iSz; ++i)
		m_vecCallBack[(int)m_eState][i](fTime);
}

void CUIButton::CallButton(CUIButton* pBtn)
{
	size_t iSz = m_vecBtnCallBack[(int)m_eState].size();

	for (size_t i = 0; i < iSz; ++i)
		m_vecBtnCallBack[(int)m_eState][i](pBtn);
}

bool CUIButton::Init()
{
	if (!CUIControl::Init())
		return false;

	m_pRC = m_pObj->CreateComponent<CColliderRect>("ButtonBody");

	CColliderRect* pRC = (CColliderRect*)m_pRC.get();

	pRC->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	pRC->SetUI();

	AddChild(m_pRC);

	return true;
}

void CUIButton::Start()
{
	CUIControl::Start();

	m_pRC = FindComponentByType<CColliderRect>();

	CColliderRect* pRC = (CColliderRect*)m_pRC.get();

	pRC->SetCallBack<CUIButton>(COLLISION_STATE::CS_INIT, this, &CUIButton::ColEnter);
	pRC->SetCallBack<CUIButton>(COLLISION_STATE::CS_STAY, this, &CUIButton::ColStay);
	pRC->SetCallBack<CUIButton>(COLLISION_STATE::CS_LAST, this, &CUIButton::ColEnd);
}

void CUIButton::Update(float fTime)
{
	CUIControl::Update(fTime);

	Vector3 vScale = GetWorldScale();

	((CColliderRect*)m_pRC.get())->SetExtend(vScale.x, vScale.y);
}

void CUIButton::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);

	if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
	{
		if (m_eState == BUTTON_STATE::PRESS)
		{
			m_eState = BUTTON_STATE::OFF;

			Call(fTime);
			CallButton(this);
		}			
	}
}

void CUIButton::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CUIButton::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CUIButton::Render(float fTime)
{
	GetMaterial()->SetDiffuseColor(m_vecInfo[(int)m_eState].vDif);

	if (m_vecInfo[(int)m_eState].pTex)
	{
		m_vecInfo[(int)m_eState].pTex->SetShader(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	}

	CUIControl::Render(fTime);

	//m_pMesh.get()->Render(fTime);
}

void CUIButton::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CUIButton::Clone()
{
	return std::shared_ptr<CComponent>(new CUIButton(*this));
}

void CUIButton::Save(FILE* pFile)
{
	CUIControl::Save(pFile);

	fwrite(&m_eState, 4, 1, pFile);

	size_t iSz = m_vecInfo.size();

	fwrite(&iSz, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iSz; ++i)
	{
		bool bTex = false;

		if (m_vecInfo[i].pTex)
			bTex = true;

		fwrite(&bTex, 1, 1, pFile);

		if (m_vecInfo[i].pTex)
			m_vecInfo[i].pTex->Save(pFile);

		fwrite(&m_vecInfo[i].vDif, sizeof(Vector4), 1, pFile);
	}
}

void CUIButton::Load(FILE* pFile)
{
	CUIControl::Load(pFile);

	fread(&m_eState, 4, 1, pFile);

	size_t iSz = 0;

	fread(&iSz, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iSz; ++i)
	{
		bool bTex = false;

		fread(&bTex, 1, 1, pFile);

		if (bTex)
		{
			m_vecInfo[i].pTex = GET_SINGLE(CResourceManager)->LoadTexture(pFile);
		}

		fread(&m_vecInfo[i].vDif, sizeof(Vector4), 1, pFile);
	}

}

void CUIButton::ColEnter(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (m_eState == BUTTON_STATE::OFF)
		{
			m_eState = BUTTON_STATE::ON;

			Call(fTime);
			CallButton(this);
		}
	}
}

void CUIButton::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (m_eState == BUTTON_STATE::OFF)
		{
			m_eState = BUTTON_STATE::ON;

			Call(fTime);
			CallButton(this);
		}

		else if (m_eState == BUTTON_STATE::ON)
		{
			if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
			{
				m_eState = BUTTON_STATE::PRESS;

				Call(fTime);
				CallButton(this);
			}
		}

		else if (m_eState == BUTTON_STATE::PRESS)
		{
			if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
			{
				m_eState = BUTTON_STATE::ON;

				Call(fTime);
				CallButton(this);
			}
		}
	}
}

void CUIButton::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (m_eState == BUTTON_STATE::ON)
		{
			m_eState = BUTTON_STATE::OFF;

			Call(fTime);
			CallButton(this);
		}
	}
}
