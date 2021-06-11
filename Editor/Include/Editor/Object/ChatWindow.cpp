#include "ChatWindow.h"
#include "UI/UIButton.h"

CChatWindow::CChatWindow()
{
}

CChatWindow::CChatWindow(const CChatWindow& ui)	:
	CGameObject(ui)
{
}

CChatWindow::~CChatWindow()
{
}

bool CChatWindow::Init()
{
	if(!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pCom = CreateComponent<CUIButton>("ChatWindow");

	SetRootComponent(pCom);

	return true;
}

void CChatWindow::Start()
{
	CGameObject::Start();
}

void CChatWindow::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CChatWindow::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CChatWindow::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CChatWindow::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CChatWindow::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CChatWindow::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CChatWindow::Clone()
{
	return nullptr;
}

void CChatWindow::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CChatWindow::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
