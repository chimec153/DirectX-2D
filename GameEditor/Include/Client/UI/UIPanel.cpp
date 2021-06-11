#include "UIPanel.h"
#include "Component/ColliderRect.h"
#include "UI/Panel.h"
#include "Component/ColliderCircle.h"
#include "UI/ChkBx.h"
#include "UI/RadioButton.h"
#include "UI/UIImage.h"
#include "UI/Slider.h"
#include "UI/UIButton.h"

CUIPanel::CUIPanel() :
	m_pCurPanel(nullptr)
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::UIPANEL;
}

CUIPanel::CUIPanel(const CUIPanel& ui)	:
	CGameObject(ui)
{
	m_pCurPanel = nullptr;
}

CUIPanel::~CUIPanel()
{
}

bool CUIPanel::Init()
{
	if (!CGameObject::Init())
		return false;


	return true;
}

void CUIPanel::Start()
{
	CGameObject::Start();
}

void CUIPanel::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_pCurPanel)
	{
		CSceneComponent* pSlider = (CSceneComponent*)FindComponent("Slider").get();

		float fPercent = 1.f - ((CSlider*)pSlider)->GetPercent() - 0.5f;

		//m_pCurPanel->GetWorldScale
		CSceneComponent* pPanel = (CSceneComponent*)FindComponent("Main").get();

		float fSizeY = ((CSceneComponent*)m_pCurPanel.get())->GetWorldScale().y - ((CSceneComponent*)pPanel)->GetWorldScale().y;

		((CSceneComponent*)m_pCurPanel.get())->SetRelativePos(0.f, fPercent * fSizeY, 0.f);
	}
}

void CUIPanel::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CUIPanel::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CUIPanel::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CUIPanel::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CUIPanel::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CUIPanel::Clone()
{
	return std::shared_ptr<CGameObject>(new CUIPanel(*this));
}

void CUIPanel::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CUIPanel::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	
}

void CUIPanel::Btn1(float fTime)
{
	m_pCurPanel = std::shared_ptr<CComponent>(FindComponent("Panel1"));

	m_pCurPanel->SetEnable(true);

	std::shared_ptr<CComponent> pPanel = std::shared_ptr<CComponent>(FindComponent("Panel3"));

	pPanel->SetEnable(false);

	pPanel = std::shared_ptr<CComponent>(FindComponent("Panel2"));

	pPanel->SetEnable(false);

	CSceneComponent* pBtn = (CSceneComponent*)FindComponent("MainTab").get();

	((CUIButton*)pBtn)->SetZOrder(1);

	pBtn = (CSceneComponent*)FindComponent("GentleTab").get();

	((CUIButton*)pBtn)->SetZOrder(4);

	pBtn = (CSceneComponent*)FindComponent("TransportTab").get();

	((CUIButton*)pBtn)->SetZOrder(4);
}

void CUIPanel::Btn2(float fTime)
{
	m_pCurPanel = std::shared_ptr<CComponent>(FindComponent("Panel2"));

	m_pCurPanel->SetEnable(true);

	std::shared_ptr<CComponent> pPanel = std::shared_ptr<CComponent>(FindComponent("Panel1"));

	pPanel->SetEnable(false);

	pPanel = std::shared_ptr<CComponent>(FindComponent("Panel3"));

	pPanel->SetEnable(false);

	CSceneComponent* pBtn = (CSceneComponent*)FindComponent("MainTab").get();

	((CUIButton*)pBtn)->SetZOrder(4);

	pBtn = (CSceneComponent*)FindComponent("GentleTab").get();

	((CUIButton*)pBtn)->SetZOrder(1);

	pBtn = (CSceneComponent*)FindComponent("TransportTab").get();

	((CUIButton*)pBtn)->SetZOrder(4);

}

void CUIPanel::Btn3(float fTime)
{
	m_pCurPanel = std::shared_ptr<CComponent>(FindComponent("Panel3"));

	m_pCurPanel->SetEnable(true);

	std::shared_ptr<CComponent> pPanel = std::shared_ptr<CComponent>(FindComponent("Panel1"));

	pPanel->SetEnable(false);

	pPanel = std::shared_ptr<CComponent>(FindComponent("Panel2"));

	pPanel->SetEnable(false);

	CSceneComponent* pBtn = (CSceneComponent*)FindComponent("MainTab").get();

	((CUIButton*)pBtn)->SetZOrder(4);

	pBtn = (CSceneComponent*)FindComponent("GentleTab").get();

	((CUIButton*)pBtn)->SetZOrder(4);

	pBtn = (CSceneComponent*)FindComponent("TransportTab").get();

	((CUIButton*)pBtn)->SetZOrder(1);
}

void CUIPanel::Close(float fTime)
{
	SetEnable(false);
}
