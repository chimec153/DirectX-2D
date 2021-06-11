#include "PathWindow.h"

CPathWindow::CPathWindow()
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::PATH_WINDOW;
}

CPathWindow::CPathWindow(const CPathWindow& ui)	:
	CWindow(ui)
{
}

CPathWindow::~CPathWindow()
{
}

bool CPathWindow::Init()
{
	if (!CWindow::Init())
		return false;

	

	return true;
}

void CPathWindow::Start()
{
	CWindow::Start();
}

void CPathWindow::Update(float fTime)
{
	CWindow::Update(fTime);
}

void CPathWindow::PostUpdate(float fTime)
{
	CWindow::PostUpdate(fTime);
}

void CPathWindow::Collision(float fTime)
{
	CWindow::Collision(fTime);
}

void CPathWindow::PreRender(float fTime)
{
	CWindow::PreRender(fTime);
}

void CPathWindow::Render(float fTime)
{
	CWindow::Render(fTime);
}

void CPathWindow::PostRender(float fTime)
{
	CWindow::PostRender(fTime);
}

std::shared_ptr<CGameObject> CPathWindow::Clone()
{
	return std::shared_ptr<CGameObject>(new CPathWindow(*this));
}

void CPathWindow::Save(FILE* pFile)
{
	CWindow::Save(pFile);
}

void CPathWindow::Load(FILE* pFile)
{
	CWindow::Load(pFile);
}
