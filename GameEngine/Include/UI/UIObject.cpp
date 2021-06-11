#include "UIObject.h"
#include "UIControl.h"

CUIObject::CUIObject()	:
	m_iZOrder(0)
{
}

CUIObject::CUIObject(const CUIObject& ui)	:
	CGameObject(ui)
{
	m_iZOrder = ui.m_iZOrder;
}

CUIObject::~CUIObject()
{
}

int CUIObject::GetZOrder() const
{
	return m_iZOrder;
}

bool CUIObject::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CUIObject::Start()
{
	CGameObject::Start();
}

void CUIObject::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CUIObject::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CUIObject::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CUIObject::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CUIObject::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CUIObject::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CUIObject::Clone()
{
	return std::shared_ptr<CGameObject>();
}

void CUIObject::Save(FILE* pFile)
{
	CGameObject::Save(pFile);

	fwrite(&m_iZOrder, 4, 1, pFile);
}

void CUIObject::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	fread(&m_iZOrder, 4, 1, pFile);
}