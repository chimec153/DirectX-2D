#include "CheckBox.h"
#include "Component/ColliderRect.h"
#include "UI/Chkbx.h"
#include "UI/UFont.h"

CCheckBox::CCheckBox()	:
	m_pBox(nullptr),
	m_pRC(nullptr),
	m_pText(nullptr)
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::CHECKBOX;
}

CCheckBox::CCheckBox(const CCheckBox& ui)	:
	CGameObject(ui)
{
	m_pRC = std::shared_ptr<CComponent>(FindComponent("BoxBody"));
	m_pBox = std::shared_ptr<CComponent>(FindComponent("Box"));

}

CCheckBox::~CCheckBox()
{
}

void CCheckBox::SetText(const TCHAR* pText)
{
	((CUFont*)m_pText.get())->SetText(pText);
}

bool CCheckBox::Init()
{
	if (!CGameObject::Init())
		return false;

	m_pBox = CreateComponent<CChkBx>("Box");

	((CChkBx*)m_pBox.get())->SetTexture(CHECK_STATE::OFF, "CheckBox");
	((CChkBx*)m_pBox.get())->SetTexture(CHECK_STATE::ON, "Check");
	((CChkBx*)m_pBox.get())->SetWorldScale(40.f, 40.f, 1.f);
	((CChkBx*)m_pBox.get())->SetWorldPos(220.f, 400.f, 0.f);
	((CChkBx*)m_pBox.get())->SetPivot(0.5f, 0.5f, 0.f);

	SetRootComponent(m_pBox);

	m_pRC = CreateComponent<CColliderRect>("BoxBody");
	((CColliderRect*)m_pRC.get())->SetExtend(20.f, 20.f);
	((CColliderRect*)m_pRC.get())->SetInheritScale(false);
	((CColliderRect*)m_pRC.get())->SetCallBack<CChkBx>(COLLISION_STATE::CS_INIT, (CChkBx*)m_pBox.get(), &CChkBx::ColEnter);
	((CColliderRect*)m_pRC.get())->SetCallBack<CChkBx>(COLLISION_STATE::CS_STAY, (CChkBx*)m_pBox.get(), &CChkBx::ColStay);
	((CColliderRect*)m_pRC.get())->SetCallBack<CChkBx>(COLLISION_STATE::CS_LAST, (CChkBx*)m_pBox.get(), &CChkBx::ColEnd);
	((CColliderRect*)m_pRC.get())->SetUI();
	((CColliderRect*)m_pRC.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	((CSceneComponent*)m_pBox.get())->AddChild(m_pRC);

	m_pText = CreateComponent<CUFont>("Text");
	((CUFont*)m_pText.get())->SetRelativePos(-120.f, 10.f, 0.f);
	((CUFont*)m_pText.get())->SetText(TEXT("롤러코스터"));
	((CUFont*)m_pText.get())->SetColor((BYTE)120, (BYTE)20, (BYTE)60, (BYTE)255);
	((CUFont*)m_pText.get())->SetShadowOffset(Vector3(1.f, -1.f, 0.f));
	((CUFont*)m_pText.get())->SetShadowOpacity(0.5f);
	((CUFont*)m_pText.get())->SetShadowAlpha(true);
	((CUFont*)m_pText.get())->SetFormat("Normal1", TEXT("궁서"), 500, 5, 20.f, TEXT("ko"));
	((CUFont*)m_pText.get())->SetRect(-150.f, 0.f, 0.f, 0.f);

	((CSceneComponent*)m_pBox.get())->AddChild(m_pText);

	return true;
}

void CCheckBox::Start()
{
	CGameObject::Start();
}

void CCheckBox::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CCheckBox::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CCheckBox::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CCheckBox::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CCheckBox::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CCheckBox::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CCheckBox::Clone()
{
	return std::shared_ptr<CGameObject>(new CCheckBox(*this));
}

void CCheckBox::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CCheckBox::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	m_pBox = FindComponentByType<CChkBx>();
	m_pRC = FindComponentByType<CColliderRect>();
	m_pText = FindComponentByType<CUFont>();
}
