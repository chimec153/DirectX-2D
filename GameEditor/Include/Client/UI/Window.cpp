#include "Window.h"
#include "UI/Panel.h"
#include "UI/UIButton.h"
#include "UI/TitleBar.h"

CWindow::CWindow()	:
	m_pMainPanel(nullptr),
	m_pCloseButton(nullptr),
	m_pTitleBar(nullptr)
{
}

CWindow::CWindow(const CWindow& ui)	:
	CGameObject(ui)
{
	
}

CWindow::~CWindow()
{
}

bool CWindow::Init()
{
	if (!CGameObject::Init())
		return false;

	m_pMainPanel = CreateComponent<CPanel>("MainPanel");
	((CSceneComponent*)m_pMainPanel.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CPanel*)m_pMainPanel.get())->SetZOrder(5);

	SetRootComponent(m_pMainPanel);

	m_pCloseButton = CreateComponent<CUIButton>("CloseButton");
	((CUIButton*)m_pCloseButton.get())->SetInheritScale(false);
	((CUIButton*)m_pCloseButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)m_pCloseButton.get())->SetWorldScale(20.f, 20.f, 0.f);

	((CPanel*)m_pMainPanel.get())->AddChild(m_pCloseButton);

	m_pTitleBar = CreateComponent<CTitleBar>("TitleBar");
	((CTitleBar*)m_pTitleBar.get())->SetInheritScale(false);
	((CTitleBar*)m_pTitleBar.get())->SetPivot(0.5f, 0.5f, 0.f);

	((CPanel*)m_pMainPanel.get())->AddChild(m_pTitleBar);

	((CPanel*)m_pMainPanel.get())->SetTexture("Panel");

	((CUIButton*)m_pCloseButton.get())->SetTexture(BUTTON_STATE::OFF, "CloseButton");
	((CUIButton*)m_pCloseButton.get())->SetTexture(BUTTON_STATE::ON, "CloseButton");
	((CUIButton*)m_pCloseButton.get())->SetTexture(BUTTON_STATE::PRESS, "CloseButton");

	((CTitleBar*)m_pTitleBar.get())->SetTexture("Panel");
	((CTitleBar*)m_pTitleBar.get())->SetColor(1.f, 1.f, 1.f, 1.f);
	((CTitleBar*)m_pTitleBar.get())->SetTextPivot(0.5f, 0.5f, 0.f);
	((CTitleBar*)m_pTitleBar.get())->SetTextSize(12.f);

	return true;
}

void CWindow::Start()
{
	CGameObject::Start();

	m_pCloseButton = FindComponentByType<CUIButton>("CloseButton");

	((CUIButton*)m_pCloseButton.get())->SetCallBack<CWindow>(BUTTON_STATE::PRESS, this, &CWindow::CloseWindow);
}

void CWindow::Update(float fTime)
{
	CGameObject::Update(fTime);

	Vector3 vMainScale = ((CPanel*)m_pMainPanel.get())->GetWorldScale();
	Vector3 vMainPivot = ((CPanel*)m_pMainPanel.get())->GetPivot();

	((CTitleBar*)m_pTitleBar.get())->SetWorldScale(vMainScale.x - 20.f, 20.f, 1.f);
	((CTitleBar*)m_pTitleBar.get())->SetRelativePos(-10.f, vMainScale.y * vMainPivot.y + 10.f, 0.f);

	((CUIButton*)m_pCloseButton.get())->SetRelativePos(vMainScale.x * vMainPivot.x - 10.f, vMainScale.y * vMainPivot.y + 10.f, 0.f);
}

void CWindow::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CWindow::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CWindow::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CWindow::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CWindow::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CWindow::Clone()
{
	return std::shared_ptr<CGameObject>(new CWindow(*this));
}

void CWindow::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CWindow::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	m_pMainPanel = FindComponentByType<CPanel>();

	m_pCloseButton = FindComponentByType<CUIButton>();

	m_pTitleBar = FindComponentByType<CTitleBar>();
}

void CWindow::CloseWindow(float fTime)
{
	SetEnable(false);
}

void CWindow::OpenWindow(float fTime)
{
	SetEnable(true);
}
