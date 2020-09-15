#include "GameObject.h"

CGameObject::CGameObject()	:
	m_pScene(nullptr),
	m_bStart(false)
{
}

CGameObject::CGameObject(const CGameObject& obj)
{
	*this = obj;
}

CGameObject::~CGameObject()
{
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Start()
{
	m_bStart = true;
}

void CGameObject::Update(float fTime)
{
}

void CGameObject::PostUpdate(float fTime)
{
}

void CGameObject::Collision(float fTime)
{
}

void CGameObject::PreRender(float fTime)
{
}

void CGameObject::Render(float fTime)
{
}

void CGameObject::PostRender(float fTime)
{
}
