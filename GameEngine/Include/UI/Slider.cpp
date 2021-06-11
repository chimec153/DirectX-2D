#include "Slider.h"
#include "../Resource/Texture.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"
#include "../Component/Collider.h"
#include "../Input.h"
#include "../Object/GameObject.h"
#include "../Component/ColliderRect.h"
#include "UIButton.h"

CSlider::CSlider()	:
	m_eState(BUTTON_STATE::OFF),
	m_eSliderType(SLIDER_TYPE::VERTICAL),
	m_fScroll(0.f),
	m_fLength(300.f),
	m_fPercent(0.f),
	m_pRC(nullptr),
	m_pTopButton(nullptr),
	m_pBackButton(nullptr),
	m_pBotButton(nullptr)
{
	m_vecTexture.resize((int)BUTTON_STATE::END);

	for (int i = 0; i < (int)BUTTON_STATE::END; ++i)
	{
		m_vecColor.push_back(Vector4::White);
	}

	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;

	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_SLIDER;
}

CSlider::CSlider(const CSlider& ui)	:
	CUIControl(ui)
{
	m_eState = ui.m_eState;

	m_vecTexture.clear();
	m_vecColor.clear();

	for (int i = 0; i < (int)BUTTON_STATE::END; ++i)
	{
		m_vecTexture.push_back(ui.m_vecTexture[i]);

		if (m_vecTexture[i])
			m_vecTexture[i]->AddRef();

		m_vecColor.push_back(ui.m_vecColor[i]);
	}

	m_fLength = ui.m_fLength;
	m_fPercent = ui.m_fPercent;
	m_fScroll = ui.m_fScroll;
	m_eSliderType = ui.m_eSliderType;

	m_pRC = std::shared_ptr<CComponent>(FindComponentByType<CColliderRect>());
}

CSlider::~CSlider()
{
}

void CSlider::SetCallBack(BUTTON_STATE eState, void(*pFunc)(float))
{
	m_vecCallBack[(int)eState].push_back(std::bind(pFunc, std::placeholders::_1));
}

void CSlider::SetTexture(BUTTON_STATE eState, const std::string& strKey)
{
	m_vecTexture[(int)eState] = GET_SINGLE(CResourceManager)->FindTexture(strKey);

	if(m_vecTexture[(int)eState])
		((CColliderRect*)m_pRC.get())->SetExtend((float)m_vecTexture[(int)eState]->GetWidth(), 
		(float)m_vecTexture[(int)eState]->GetHeight());
}

void CSlider::SetTexture(BUTTON_STATE eState, std::shared_ptr<CTexture> pTex)
{
	m_vecTexture[(int)eState] = pTex;

	if (m_vecTexture[(int)eState])
	{
		((CColliderRect*)m_pRC.get())->SetExtend((float)m_vecTexture[(int)eState]->GetWidth(),
			(float)m_vecTexture[(int)eState]->GetHeight());
	}
}

void CSlider::SetColor(BUTTON_STATE eState, const Vector4& vColor)
{
	m_vecColor[(int)eState] = vColor;
}

SLIDER_TYPE CSlider::GetSliderType() const
{
	return m_eSliderType;
}

void CSlider::SetSliderType(SLIDER_TYPE eType)
{
	m_eSliderType = eType;
}

void CSlider::Call(float fTime)
{
	size_t iSz = m_vecCallBack[(int)m_eState].size();

	for (size_t i = 0; i < iSz; ++i)
		m_vecCallBack[(int)m_eState][i](fTime);
}

bool CSlider::Init()
{
	if (!CUIControl::Init())
		return false;

	m_pRC = m_pObj->CreateComponent<CColliderRect>("SliderBody");

	((CColliderRect*)m_pRC.get())->SetInheritScale(false);
	((CColliderRect*)m_pRC.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	((CColliderRect*)m_pRC.get())->SetUI();

	((CColliderRect*)m_pRC.get())->SetCallBack<CSlider>(COLLISION_STATE::CS_INIT, this, &CSlider::ColEnter);
	((CColliderRect*)m_pRC.get())->SetCallBack<CSlider>(COLLISION_STATE::CS_STAY, this, &CSlider::ColStay);
	((CColliderRect*)m_pRC.get())->SetCallBack<CSlider>(COLLISION_STATE::CS_LAST, this, &CSlider::ColEnd);

	AddChild(m_pRC);

	m_pBackButton = m_pObj->CreateComponent<CUIButton>("Back");
	((CUIButton*)m_pBackButton.get())->SetTexture(BUTTON_STATE::OFF, "SliderBack");
	((CUIButton*)m_pBackButton.get())->SetTexture(BUTTON_STATE::ON, "SliderBack");
	((CUIButton*)m_pBackButton.get())->SetTexture(BUTTON_STATE::PRESS, "SliderBack");
	((CUIButton*)m_pBackButton.get())->SetWorldScale(20.f, 300.f, 1.f);
	((CUIButton*)m_pBackButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)m_pBackButton.get())->SetZOrder(2);
	((CUIButton*)m_pBackButton.get())->SetRelativePos(205.f, 0.f, 0.f);
	((CUIButton*)m_pBackButton.get())->SetCallBack<CSlider>(BUTTON_STATE::PRESS, this, &CSlider::JumpSlider);
	((CUIButton*)m_pBackButton.get())->SetInheritScale(false);

	((CUIButton*)m_pBackButton.get())->AddChild(std::shared_ptr<CComponent>(this));

	m_pTopButton = m_pObj->CreateComponent<CUIButton>("LeftBtn");
	((CUIButton*)m_pTopButton.get())->SetWorldScale(20.f, 20.f, 0.f);
	((CUIButton*)m_pTopButton.get())->SetInheritScale(false);
	((CUIButton*)m_pTopButton.get())->SetRelativePos(0.f, ((CUIButton*)m_pBackButton.get())->GetWorldScale().y / 2.f + 
		((CUIButton*)m_pBackButton.get())->GetWorldScale().y / 2.f, 0.f);
	((CUIButton*)m_pTopButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)m_pTopButton.get())->SetTexture(BUTTON_STATE::OFF, "Up");
	((CUIButton*)m_pTopButton.get())->SetTexture(BUTTON_STATE::ON, "Up");
	((CUIButton*)m_pTopButton.get())->SetTexture(BUTTON_STATE::PRESS, "Up");
	((CUIButton*)m_pTopButton.get())->SetCallBack<CSlider>(BUTTON_STATE::PRESS, this, &CSlider::AddSlider);

	((CUIButton*)m_pBackButton.get())->AddChild(m_pTopButton);

	m_pBotButton = m_pObj->CreateComponent<CUIButton>("RightBtn");
	((CUIButton*)m_pBotButton.get())->SetWorldScale(20.f, 20.f, 0.f);
	((CUIButton*)m_pBotButton.get())->SetInheritScale(false);
	((CUIButton*)m_pBotButton.get())->SetRelativePos(0.f, -((CUIButton*)m_pBackButton.get())->GetWorldScale().y / 2.f 
		- ((CUIButton*)m_pBackButton.get())->GetWorldScale().y / 2.f, 0.f);
	((CUIButton*)m_pBotButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)m_pBotButton.get())->SetTexture(BUTTON_STATE::OFF, "Down");
	((CUIButton*)m_pBotButton.get())->SetTexture(BUTTON_STATE::ON, "Down");
	((CUIButton*)m_pBotButton.get())->SetTexture(BUTTON_STATE::PRESS, "Down");
	((CUIButton*)m_pBotButton.get())->SetCallBack<CSlider>(BUTTON_STATE::PRESS, this, &CSlider::SubSlider);

	((CUIButton*)m_pBackButton.get())->AddChild(m_pBotButton);

	return true;
}

void CSlider::Start()
{
	CUIControl::Start();
}

void CSlider::Update(float fTime)
{
	CUIControl::Update(fTime);

	Vector3 vPos = GetRelativePos();

	Vector3 vScale = GetWorldScale();

	if (vPos.y > m_fLength / 2.f - vScale.y / 2.f)
		SetRelativePos(0.f, m_fLength / 2.f - vScale.y / 2.f, 0.f);

	else if (vPos.y < m_fLength / -2.f + vScale.y / 2.f)
		SetRelativePos(0.f, -m_fLength / 2.f + vScale.y / 2.f, 0.f);

	m_fPercent = (-vPos.y + (m_fLength - vScale.y) / 2.f) / (m_fLength - vScale.y);

	if (m_fPercent > 1.f)
		m_fPercent = 1.f;

	else if (m_fPercent < 0.f)
		m_fPercent = 0.f;

	((CColliderRect*)m_pRC.get())->SetExtend(vScale.x, vScale.y);
}

void CSlider::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);

	if (m_eState == BUTTON_STATE::PRESS)
	{
		if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
		{
			m_eState = BUTTON_STATE::OFF;

			Call(fTime);
		}

		else
		{
			Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();
			Vector3 vPos = {};
			float fMove = 0.f;
			switch (m_eSliderType)
			{
			case SLIDER_TYPE::HORIZONTAL:
				m_fScroll += (float)GET_SINGLE(CInput)->GetMoveX();
				SetWorldPos(m_vStartPos + Vector3(m_fScroll, 0.f, 0.f));					
				break;
			case SLIDER_TYPE::VERTICAL:
				m_fScroll += (float)GET_SINGLE(CInput)->GetMoveY();
				SetWorldPos(m_vStartPos + Vector3(0.f, -m_fScroll,0.f));
				break;
			}
		}
	}

	GetMaterial()->SetDiffuseColor(m_vecColor[(int)m_eState]);
}

void CSlider::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CSlider::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CSlider::Render(float fTime)
{
	m_vecTexture[(int)m_eState]->SetShader(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	CUIControl::Render(fTime);

	//m_pMesh->Render(fTime);
}

void CSlider::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CSlider::Clone()
{
	return std::shared_ptr<CComponent>(new CSlider(*this));
}

void CSlider::Save(FILE* pFile)
{
	CUIControl::Save(pFile);
}

void CSlider::Load(FILE* pFile)
{
	CUIControl::Load(pFile);
}

void CSlider::ColEnter(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (m_eState == BUTTON_STATE::OFF)
		{
			m_eState = BUTTON_STATE::ON;

			Call(fTime);
		}
	}
}

void CSlider::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (m_eState == BUTTON_STATE::ON)
		{
			if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
			{
				m_eState = BUTTON_STATE::PRESS;
				m_vStartPos = GetWorldPos();
				m_fScroll = 0.f;
				Call(fTime);
			}
		}
	}
}

void CSlider::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (m_eState == BUTTON_STATE::ON)
		{
			m_eState = BUTTON_STATE::OFF;

			Call(fTime);
		}
	}
}

void CSlider::SetLength(float fLength)
{
	m_fLength = fLength;

	Vector3 vScale = ((CUIButton*)m_pBackButton.get())->GetWorldScale();

	((CUIButton*)m_pBackButton.get())->SetWorldScale(vScale.x, m_fLength, vScale.z);
}

float CSlider::GetPercent() const
{
	return m_fPercent;
}

void CSlider::AddSlider(float f)
{
	switch (m_eSliderType)
	{
	case SLIDER_TYPE::VERTICAL:
		AddWorldPos(0.f, 10.f, 0.f);
		break;
	case SLIDER_TYPE::HORIZONTAL:
		AddWorldPos(10.f, 0.f, 0.f);
		break;
	}
}

void CSlider::SubSlider(float f)
{
	switch (m_eSliderType)
	{
	case SLIDER_TYPE::VERTICAL:
		AddWorldPos(0.f, -10.f, 0.f);
		break;
	case SLIDER_TYPE::HORIZONTAL:
		AddWorldPos(-10.f, 0.f, 0.f);
		break;
	}
}

void CSlider::JumpSlider(float f)
{
	Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos() - 150.f;
	Vector3 vPos = GetWorldPos();

	switch (m_eSliderType)
	{
	case SLIDER_TYPE::VERTICAL:
		if (vPos.y < vMousePos.y)
			AddWorldPos(0.f, 50.f, 0.f);

		else
			AddWorldPos(0.f, -50.f, 0.f);

		break;
	case SLIDER_TYPE::HORIZONTAL:
		if (vPos.x < vMousePos.x)
			AddWorldPos(50.f, 0.f, 0.f);

		else
			AddWorldPos(-50.f, 0.f, 0.f);
		break;
	}
}