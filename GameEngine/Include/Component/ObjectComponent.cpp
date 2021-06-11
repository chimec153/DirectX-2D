#include "ObjectComponent.h"
#include "../Resource/ShaderManager.h"
#include "../RenderManager.h"
#include "../Scene/Scene.h"

CObjectComponent::CObjectComponent()	:
	m_eClassType(OBJ_COMPONENT_CLASS_TYPE::NAVIGATION)
{
	m_eType = COMPONENT_TYPE::CT_OBJECT;
}

CObjectComponent::CObjectComponent(const CObjectComponent& com)	:
	CComponent(com)
	, m_eClassType(com.m_eClassType)
{
}

CObjectComponent::~CObjectComponent()
{
}

OBJ_COMPONENT_CLASS_TYPE CObjectComponent::GetClassType() const
{
	return m_eClassType;
}

void CObjectComponent::SetClassType(OBJ_COMPONENT_CLASS_TYPE eType)
{
	m_eClassType = eType;
}

void CObjectComponent::SetLayer(CLayer* pLayer)
{
	if (m_pLayer)
	{
		m_pLayer = pLayer->GetScene()->FindLayer(m_pLayer->GetZOrder());
	}
	else
	{
		m_pLayer = pLayer;
	}
}

bool CObjectComponent::Init()
{
	if (!CComponent::Init())
		return false;

	return true;
}

void CObjectComponent::Start()
{
	CComponent::Start();
}

void CObjectComponent::Update(float fTime)
{
	CComponent::Update(fTime);
}

void CObjectComponent::PostUpdate(float fTime)
{
	CComponent::PostUpdate(fTime);
}

void CObjectComponent::Collision(float fTime)
{
	CComponent::Collision(fTime);
}

void CObjectComponent::PreRender(float fTime)
{
	CComponent::PreRender(fTime);
}

void CObjectComponent::Render(float fTime)
{
	CComponent::Render(fTime);
}

void CObjectComponent::PostRender(float fTime)
{
	CComponent::PostRender(fTime);
}

std::shared_ptr<class CComponent> CObjectComponent::Clone()
{
	return std::shared_ptr<class CComponent>(new CObjectComponent(*this));
}

void CObjectComponent::Save(FILE* pFile)
{
	CComponent::Save(pFile);
}

void CObjectComponent::Load(FILE* pFile)
{
	CComponent::Load(pFile);
}

void CObjectComponent::ShowWindow()
{
	CComponent::ShowWindow();
}
