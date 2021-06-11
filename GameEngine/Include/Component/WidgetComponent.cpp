#include "WidgetComponent.h"
#include "../Object/GameObject.h"

CWidgetComponent::CWidgetComponent()	:
	m_pUIObj(nullptr)
{
}

CWidgetComponent::CWidgetComponent(const CWidgetComponent& com)	:
	CSceneComponent(com)
{
	if (com.m_pUIObj)
		m_pUIObj = com.m_pUIObj->Clone();

	else
		m_pUIObj = nullptr;
}

CWidgetComponent::~CWidgetComponent()
{
}

bool CWidgetComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CWidgetComponent::Start()
{
	CSceneComponent::Start();

}

void CWidgetComponent::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	if (m_pUIObj)
		m_pUIObj->Update(fTime);
}

void CWidgetComponent::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);

	if (m_pUIObj)
		m_pUIObj->PostUpdate(fTime);
}

void CWidgetComponent::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CWidgetComponent::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CWidgetComponent::Render(float fTime)
{
	if (m_pUIObj)
		m_pUIObj->Render(fTime);
}

void CWidgetComponent::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

std::shared_ptr<CComponent> CWidgetComponent::Clone()
{
	return std::shared_ptr<CComponent>(new CWidgetComponent(*this));
}

void CWidgetComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CWidgetComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
