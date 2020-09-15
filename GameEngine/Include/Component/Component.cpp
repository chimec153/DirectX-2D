#include "Component.h"
#include "../Object/GameObject.h"

CComponent::CComponent()	:
	m_bStart(false),
	m_pObj(nullptr),
	m_pScene(nullptr)
{
}

CComponent::CComponent(const CComponent& com)
{
	*this = com;
}

CComponent::~CComponent()
{
}

CGameObject* CComponent::GetObj()
{
	if (m_pObj)
		m_pObj->AddRef();

	return m_pObj;
}

bool CComponent::Init()
{
	return true;
}

void CComponent::Start()
{
}

void CComponent::Update(float fTime)
{
}

void CComponent::PostUpdate(float fTime)
{
}

void CComponent::Collision(float fTime)
{
}

void CComponent::PreRender(float fTime)
{
}

void CComponent::Render(float fTime)
{
}

void CComponent::PostRender(float fTime)
{
}
