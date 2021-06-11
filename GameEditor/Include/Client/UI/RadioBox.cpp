#include "RadioBox.h"
#include "UI/RadioButton.h"
#include "Component/ColliderCircle.h"

CRadioBox::CRadioBox()	:
	m_pRadioBtn(nullptr),
	m_pCC(nullptr)
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::RADIOBOX;
}

CRadioBox::CRadioBox(const CRadioBox& ui)	:
	CGameObject(ui)
{
	m_pRadioBtn = std::shared_ptr<CComponent>(FindComponent("Radio"));

	m_pCC = std::shared_ptr<CComponent>(FindComponent("RadioBody"));
}

CRadioBox::~CRadioBox()
{
}

void CRadioBox::SetGroup(CRadioBox* pBtn)
{
	((CRadioButton*)m_pRadioBtn.get())->AddButton((CRadioButton*)pBtn->m_pRadioBtn.get());
}

bool CRadioBox::Init()
{
	if (!CGameObject::Init())
		return false;

	m_pRadioBtn = CreateComponent<CRadioButton>("Radio");
	((CRadioButton*)m_pRadioBtn.get())->SetWorldScale(40.f, 40.f, 1.f);
	((CRadioButton*)m_pRadioBtn.get())->SetWorldPos(500.f, 500.f, 0.f);
	((CRadioButton*)m_pRadioBtn.get())->SetTexture(RADIO_STATE::OFF, "Radio");
	((CRadioButton*)m_pRadioBtn.get())->SetTexture(RADIO_STATE::ON, "Select");
	((CRadioButton*)m_pRadioBtn.get())->SetSound(RADIO_STATE::OFF, "Click1");
	((CRadioButton*)m_pRadioBtn.get())->SetSound(RADIO_STATE::ON, "Click1");
	((CRadioButton*)m_pRadioBtn.get())->SetCallBack<CRadioButton>(RADIO_STATE::OFF, (CRadioButton*)m_pRadioBtn.get(), &CRadioButton::Play);
	((CRadioButton*)m_pRadioBtn.get())->SetCallBack<CRadioButton>(RADIO_STATE::ON, (CRadioButton*)m_pRadioBtn.get(), &CRadioButton::Play);
	((CRadioButton*)m_pRadioBtn.get())->SetPivot(0.5f, 0.5f, 0.f);

	SetRootComponent(m_pRadioBtn);

	m_pCC = CreateComponent<CColliderCircle>("RadioBody");
	((CColliderCircle*)m_pCC.get())->SetRadius(12.f);
	((CColliderCircle*)m_pCC.get())->SetCallBack<CRadioButton>(COLLISION_STATE::CS_INIT, (CRadioButton*)m_pRadioBtn.get(), &CRadioButton::ColEnter);
	((CColliderCircle*)m_pCC.get())->SetCallBack<CRadioButton>(COLLISION_STATE::CS_STAY, (CRadioButton*)m_pRadioBtn.get(), &CRadioButton::ColStay);
	((CColliderCircle*)m_pCC.get())->SetCallBack<CRadioButton>(COLLISION_STATE::CS_LAST, (CRadioButton*)m_pRadioBtn.get(), &CRadioButton::ColEnd);
	((CColliderCircle*)m_pCC.get())->SetUI();
	((CColliderCircle*)m_pCC.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	((CSceneComponent*)m_pRadioBtn.get())->AddChild(m_pCC);

	return true;
}

void CRadioBox::Start()
{
	CGameObject::Start();
}

void CRadioBox::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CRadioBox::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CRadioBox::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CRadioBox::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CRadioBox::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CRadioBox::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CRadioBox::Clone()
{
	return std::shared_ptr<CGameObject>(new CRadioBox(*this));
}

void CRadioBox::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CRadioBox::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	m_pRadioBtn = FindComponentByType<CRadioButton>();

	m_pCC = FindComponentByType<CColliderCircle>();
}
