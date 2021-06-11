#include "Panel.h"
#include "../Resource/Texture.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"

CPanel::CPanel()
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;

	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_PANEL;
}

CPanel::CPanel(const CPanel& ui)	:
	CUIControl(ui)
{
}

CPanel::~CPanel()
{
}

bool CPanel::Init()
{
	if (!CUIControl::Init())
		return false;

	return true;
}

void CPanel::Start()
{
	CUIControl::Start();
}

void CPanel::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CPanel::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CPanel::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CPanel::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CPanel::Render(float fTime)
{
	m_pTexture->SetShader(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	CUIControl::Render(fTime);

	//m_pMesh->Render(fTime);
}

void CPanel::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CPanel::Clone()
{
	return std::shared_ptr<CComponent>(new CPanel(*this));
}

void CPanel::Save(FILE* pFile)
{
	CUIControl::Save(pFile);
}

void CPanel::Load(FILE* pFile)
{
	CUIControl::Load(pFile);
}
