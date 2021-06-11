#include "RadioButton.h"
#include "../Resource/Texture.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"
#include "../Component/Collider.h"
#include "../Input.h"
#include "../Sound/SoundManager.h"
#include "../Component/ColliderCircle.h"
#include "../Object/GameObject.h"
#include "../Component/ColliderRect.h"

CRadioButton::CRadioButton()	:
	m_eState(RADIO_STATE::OFF),
	m_pCC(nullptr)
{
	m_vecTex.resize((int)RADIO_STATE::END);

	for (int i = 0; i < (int)RADIO_STATE::END; ++i)
		m_vecTex[i] = nullptr;

	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;

	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_RADIO;
}

CRadioButton::CRadioButton(const CRadioButton& ui)	:
	CUIControl(ui)
{
	m_eState = ui.m_eState;

	m_vecTex.clear();

	for (int i = 0; i < (int)RADIO_STATE::END; ++i)
	{
		if (ui.m_vecTex[i])
			ui.m_vecTex[i]->AddRef();

		m_vecTex.push_back(ui.m_vecTex[i]);
	}

	m_pCC = std::shared_ptr<CComponent>(FindComponentByType<CColliderCircle>());
}

CRadioButton::~CRadioButton()
{
	std::list<CRadioButton*>::iterator iter = m_ButtonList.begin();
	std::list<CRadioButton*>::iterator iterEnd = m_ButtonList.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->DeleteButton(this);
}

RADIO_STATE CRadioButton::GetState() const
{
	return m_eState;
}

void CRadioButton::SetState(RADIO_STATE eState)
{
	m_eState = eState;

	Play(0.f);
}

void CRadioButton::SetTexture(RADIO_STATE eState, std::shared_ptr<CTexture> pTex)
{
	m_vecTex[(int)eState] = pTex;

	if (m_vecTex[(int)eState])
	{
		float fHeight = (float)m_vecTex[(int)eState]->GetHeight();
		float fWidth = (float)m_vecTex[(int)eState]->GetWidth();

		((CColliderRect*)m_pCC.get())->SetExtend(fHeight, fWidth);
	}
		
}

void CRadioButton::SetTexture(RADIO_STATE eState, const std::string& strKey)
{
	m_vecTex[(int)eState] = GET_SINGLE(CResourceManager)->FindTexture(strKey);

	if (m_vecTex[(int)eState])
	{
		float fHeight = (float)m_vecTex[(int)eState]->GetHeight();
		float fWidth = (float)m_vecTex[(int)eState]->GetWidth();

		((CColliderRect*)m_pCC.get())->SetExtend(fHeight, fWidth);
	}
}

void CRadioButton::AddButton(CRadioButton* pBtn)
{
	if (HasButton(pBtn))
		return;

	std::list<CRadioButton*>::iterator iter = m_ButtonList.begin();
	std::list<CRadioButton*>::iterator iterEnd = m_ButtonList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->m_ButtonList.push_back(pBtn);
		pBtn->m_ButtonList.push_back(*iter);
	}
	
	m_ButtonList.push_back(pBtn);
	pBtn->m_ButtonList.push_back(this);
}

void CRadioButton::DeleteButton(CRadioButton* pBtn)
{
	std::list<CRadioButton*>::iterator iter = m_ButtonList.begin();
	std::list<CRadioButton*>::iterator iterEnd = m_ButtonList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == pBtn)
		{
			m_ButtonList.erase(iter);
			return;
		}
	}
}

void CRadioButton::SetSound(RADIO_STATE eState, const std::string& strKey)
{
	m_vecSound[(int)eState] = strKey;
}

void CRadioButton::Play(float fTime)
{
	GET_SINGLE(CSoundManager)->Play(m_vecSound[(int)m_eState]);
}

bool CRadioButton::HasButton(CRadioButton* pBtn)
{
	std::list<CRadioButton*>::iterator iter = m_ButtonList.begin();
	std::list<CRadioButton*>::iterator iterEnd = m_ButtonList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == pBtn)
			return true;
	}

	return false;
}

void CRadioButton::SetCallBack(RADIO_STATE eState, void(*pFunc)(float))
{
	m_CallBack[(int)eState].push_back(std::bind(pFunc, std::placeholders::_1));
}

void CRadioButton::Call(RADIO_STATE eState, float fTime)
{
	size_t iSz = m_CallBack[(int)eState].size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_CallBack[(int)eState][i](fTime);
	}
}

bool CRadioButton::Init()
{
	if (!CUIControl::Init())
		return false;

	m_pCC = m_pObj->CreateComponent<CColliderRect>("RadioBody");

	((CColliderRect*)m_pCC.get())->SetInheritScale(false);
	((CColliderRect*)m_pCC.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	((CColliderRect*)m_pCC.get())->SetUI();

	AddChild(m_pCC);

	return true;
}

void CRadioButton::Start()
{
	CUIControl::Start();

	m_pCC = FindComponentByType<CColliderRect>();

	((CColliderRect*)m_pCC.get())->SetCallBack<CRadioButton>(COLLISION_STATE::CS_INIT, this, &CRadioButton::ColEnter);
	((CColliderRect*)m_pCC.get())->SetCallBack<CRadioButton>(COLLISION_STATE::CS_STAY, this, &CRadioButton::ColStay);
	((CColliderRect*)m_pCC.get())->SetCallBack<CRadioButton>(COLLISION_STATE::CS_END, this, &CRadioButton::ColEnd);
}

void CRadioButton::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CRadioButton::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CRadioButton::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CRadioButton::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CRadioButton::Render(float fTime)
{
	m_vecTex[(int)m_eState]->SetShader(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	CUIControl::Render(fTime);

	//m_pMesh->Render(fTime);
}

void CRadioButton::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CRadioButton::Clone()
{
	return std::shared_ptr<CComponent>();
}

void CRadioButton::Save(FILE* pFile)
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

	for (int i = 0; i < (int)RADIO_STATE::END; ++i)
	{
		int iLength = (int)m_vecSound[i].length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(m_vecSound[i].c_str(), 1, iLength, pFile);
	}
}

void CRadioButton::Load(FILE* pFile)
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

	for (int i = 0; i < (int)RADIO_STATE::END; ++i)
	{
		int iLength = 0;

		fread(&iLength, 4, 1, pFile);

		char strTag[256] = {};

		fread(strTag, 1, iLength, pFile);

		m_vecSound[i] = strTag;
	}
}

void CRadioButton::ColEnter(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
		{
			if (m_eState == RADIO_STATE::OFF)
			{
				SetState(RADIO_STATE::ON);

				Call(m_eState, fTime);

				std::list<CRadioButton*>::iterator iter = m_ButtonList.begin();
				std::list<CRadioButton*>::iterator iterEnd = m_ButtonList.end();

				for (; iter != iterEnd; ++iter)
				{
					(*iter)->SetState(RADIO_STATE::OFF);
					(*iter)->Call(RADIO_STATE::OFF, fTime);
				}
			}
		}
	}
}

void CRadioButton::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
		{
			if (m_eState == RADIO_STATE::OFF)
			{
				SetState(RADIO_STATE::ON);

				Call(m_eState, fTime);

				std::list<CRadioButton*>::iterator iter = m_ButtonList.begin();
				std::list<CRadioButton*>::iterator iterEnd = m_ButtonList.end();

				for (; iter != iterEnd; ++iter)
				{
					(*iter)->SetState(RADIO_STATE::OFF);
					(*iter)->Call(RADIO_STATE::OFF, fTime);
				}
			}
		}
	}
}

void CRadioButton::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
}
