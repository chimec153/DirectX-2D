#include "UIViewPort.h"
#include "../UI/UIObject.h"

CUIViewPort::CUIViewPort()	:
	m_pScene(nullptr)
{
}

CUIViewPort::~CUIViewPort()
{
	SAFE_RELEASE_VECLIST(m_UIList);
}

void CUIViewPort::AddObj(CGameObject* pObj)
{
	pObj->AddRef();

	m_UIList.push_back(pObj);
}

bool CUIViewPort::Init()
{
	return true;
}

void CUIViewPort::Start()
{
	std::list<CGameObject*>::iterator iter = m_UIList.begin();
	std::list<CGameObject*>::iterator iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

void CUIViewPort::Update(float fTime)
{
	std::list<CGameObject*>::iterator iter = m_UIList.begin();
	std::list<CGameObject*>::iterator iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Update(fTime);
	}
}

void CUIViewPort::PostUpdate(float fTime)
{
	std::list<CGameObject*>::iterator iter = m_UIList.begin();
	std::list<CGameObject*>::iterator iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PostUpdate(fTime);
	}

	m_UIList.sort(CUIViewPort::SortZ);
}

void CUIViewPort::Collision(float fTime)
{
	std::list<CGameObject*>::iterator iter = m_UIList.begin();
	std::list<CGameObject*>::iterator iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Collision(fTime);
	}
}

void CUIViewPort::PreRender(float fTime)
{
	std::list<CGameObject*>::iterator iter = m_UIList.begin();
	std::list<CGameObject*>::iterator iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PreRender(fTime);
	}
}

void CUIViewPort::Render(float fTime)
{
	std::list<CGameObject*>::iterator iter = m_UIList.begin();
	std::list<CGameObject*>::iterator iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}
}

void CUIViewPort::PostRender(float fTime)
{
	std::list<CGameObject*>::iterator iter = m_UIList.begin();
	std::list<CGameObject*>::iterator iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PostRender(fTime);
	}
}

CUIViewPort* CUIViewPort::Clone()
{
	return new CUIViewPort(*this);
}

void CUIViewPort::Save(FILE* pFile)
{
	CRef::Save(pFile);
}

void CUIViewPort::Load(FILE* pFile)
{
	CRef::Load(pFile);
}

bool CUIViewPort::SortZ(CGameObject* pObj1, CGameObject* pObj2)
{
	return ((CUIObject*)pObj1)->GetZOrder() > ((CUIObject*)pObj2)->GetZOrder();
}
