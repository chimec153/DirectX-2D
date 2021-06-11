#include "ChkBx.h"
#include "../Resource/Texture.h"
#include "../Resource/ResourceManager.h"
#include "../Component/Collider.h"
#include "../Input.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"
#include "../Sound/SoundManager.h"
#include "../UI/UFont.h"
#include "../Object/GameObject.h"
#include "../Component/ColliderRect.h"

CChkBx::CChkBx()	:
	m_bOn(false),
	m_pRC(nullptr)
{
	m_vecTex.resize((int)CHECK_STATE::END);

	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;

	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_CHECK;
}

CChkBx::CChkBx(const CChkBx& ui)	:
	CUIControl(ui)
{
	m_bOn = ui.m_bOn;

	m_vecTex.clear();

	size_t iSz = ui.m_vecTex.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecTex.push_back(ui.m_vecTex[i]);
	}

	m_pRC = std::shared_ptr<CComponent>(FindComponentByType<CColliderRect>());
}

CChkBx::~CChkBx()
{
}

bool CChkBx::IsOn() const
{
	return m_bOn;
}

void CChkBx::Check(bool bOn)
{
	m_bOn = bOn;

	Call(0.f);
	BoxCall(this);
	Play();
}

void CChkBx::SetTexture(CHECK_STATE eState, const std::string& strKey)
{
	m_vecTex[(int)eState] = GET_SINGLE(CResourceManager)->FindTexture(strKey);

	if (m_vecTex[(int)eState])
	{
		((CColliderRect*)m_pRC.get())->SetExtend((float)m_vecTex[(int)eState]->GetWidth(), 
			(float)m_vecTex[(int)eState]->GetHeight());
	}
}

void CChkBx::SetTexture(CHECK_STATE eState, std::shared_ptr<CTexture> pTex)
{
	m_vecTex[(int)eState] = pTex;

	if (m_vecTex[(int)eState])
	{
		((CColliderRect*)m_pRC.get())->SetExtend((float)m_vecTex[(int)eState]->GetWidth(),
			(float)m_vecTex[(int)eState]->GetHeight());
	}		
}

void CChkBx::SetSound(CHECK_STATE eState, const std::string& strKey)
{
	m_vecSound[(int)eState] = strKey;
}

void CChkBx::Play()
{
	GET_SINGLE(CSoundManager)->Play(m_vecSound[m_bOn]);
}

std::string CChkBx::GetSound(CHECK_STATE eStat) const
{
	return m_vecSound[(int)eStat];
}

void CChkBx::SetCallBack(CHECK_STATE eState, void(*pFunc)(float))
{
	m_CallBack[(int)eState].push_back(std::bind(pFunc, std::placeholders::_1));
}

void CChkBx::SetBoxCallBack(CHECK_STATE eState, void(*pFunc)(CChkBx*))
{
	m_BoxCallBack[(int)eState].push_back(std::bind(pFunc, std::placeholders::_1));
}

void CChkBx::Call(float fTime)
{
	size_t iSz = m_CallBack[m_bOn].size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_CallBack[m_bOn][i](fTime);
	}	
}

void CChkBx::BoxCall(CChkBx* pBx)
{
	size_t iSz = m_BoxCallBack[m_bOn].size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_BoxCallBack[m_bOn][i](pBx);
	}
}

bool CChkBx::Init()
{
	if (!CUIControl::Init())
		return false;

	m_pRC = m_pObj->CreateComponent<CColliderRect>("CheckBody");

	((CColliderRect*)m_pRC.get())->SetInheritScale(false);
	((CColliderRect*)m_pRC.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	((CColliderRect*)m_pRC.get())->SetUI();

	AddChild(m_pRC);

	return true;
}

void CChkBx::Start()
{
	CUIControl::Start();

	m_pRC = FindComponentByType<CColliderRect>();

	((CColliderRect*)m_pRC.get())->SetCallBack<CChkBx>(COLLISION_STATE::CS_INIT, this, &CChkBx::ColEnter);
	((CColliderRect*)m_pRC.get())->SetCallBack<CChkBx>(COLLISION_STATE::CS_STAY, this, &CChkBx::ColStay);
	((CColliderRect*)m_pRC.get())->SetCallBack<CChkBx>(COLLISION_STATE::CS_LAST, this, &CChkBx::ColEnd);
}

void CChkBx::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CChkBx::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CChkBx::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CChkBx::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CChkBx::Render(float fTime)
{
	m_vecTex[m_bOn]->SetShader(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	CUIControl::Render(fTime);

	//m_pMesh->Render(fTime);
}

void CChkBx::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CChkBx::Clone()
{
	return std::shared_ptr<CComponent>(new CChkBx(*this));
}

void CChkBx::Save(FILE* pFile)
{
	CUIControl::Save(pFile);

	size_t iSz = m_vecTex.size();

	fwrite(&iSz, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iSz; ++i)
	{
		bool bTex = false;

		if (m_vecTex[i])
			bTex = true;

		fwrite(&bTex, 1, 1, pFile);

		if (m_vecTex[i])
			m_vecTex[i]->Save(pFile);
	}

	fwrite(&m_bOn, 1, 1, pFile);

	for (int i = 0; i < (int)CHECK_STATE::END; ++i)
	{
		int iLength = (int)m_vecSound[i].length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(m_vecSound[i].c_str(), 1, iLength, pFile);
	}
}

void CChkBx::Load(FILE* pFile)
{
	CUIControl::Load(pFile);

	size_t iSz = 0;

	fread(&iSz, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iSz; ++i)
	{
		bool bTex = false;

		fread(&bTex, 1, 1, pFile);

		if (bTex)
			m_vecTex[i] = GET_SINGLE(CResourceManager)->LoadTexture(pFile);
	}

	fread(&m_bOn, 1, 1, pFile);

	for (int i = 0; i < (int)CHECK_STATE::END; ++i)
	{
		int iLength = 0;

		fread(&iLength, 4, 1, pFile);

		char strTag[256] = {};

		fread(strTag, 1, iLength, pFile);

		m_vecSound[i] = strTag;
	}
}

void CChkBx::ColEnter(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
		{
			m_bOn = !m_bOn;

			Call(fTime);
			BoxCall(this);
			Play();
		}
	}
}

void CChkBx::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
		{
			m_bOn = !m_bOn;

			Call(fTime);
			BoxCall(this);
			Play();
		}
	}
}

void CChkBx::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
}
