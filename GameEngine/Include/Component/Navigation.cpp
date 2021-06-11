#include "Navigation.h"

CNavigation::CNavigation()
{
	SetClassType(OBJ_COMPONENT_CLASS_TYPE::NAVIGATION);
}

CNavigation::CNavigation(const CNavigation& nav)	:
	CObjectComponent(nav)
{
}

CNavigation::~CNavigation()
{
}

bool CNavigation::Init()
{
	if (!CObjectComponent::Init())
		return false;

	return true;
}

void CNavigation::Start()
{
	CObjectComponent::Start();
}

void CNavigation::Update(float fTime)
{
	CObjectComponent::Update(fTime);
}

void CNavigation::PostUpdate(float fTime)
{
	CObjectComponent::PostUpdate(fTime);
}

void CNavigation::Collision(float fTime)
{
	CObjectComponent::Collision(fTime);
}

void CNavigation::PreRender(float fTime)
{
	CObjectComponent::PreRender(fTime);
}

void CNavigation::Render(float fTime)
{
	CObjectComponent::Render(fTime);
}

void CNavigation::PostRender(float fTime)
{
	CObjectComponent::PostRender(fTime);
}

std::shared_ptr<CComponent> CNavigation::Clone()
{
	return std::shared_ptr<CComponent>();
}

void CNavigation::Save(FILE* pFile)
{
	CObjectComponent::Save(pFile);
}

void CNavigation::Load(FILE* pFile)
{
	CObjectComponent::Load(pFile);
}
