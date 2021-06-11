#include "UIImage.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"

CUIImage::CUIImage()
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;

	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_IMAGE;
}

CUIImage::CUIImage(const CUIImage& ui)	:
	CUIControl(ui)
{
}

CUIImage::~CUIImage()
{
}


bool CUIImage::Init()
{
	if (!CUIControl::Init())
		return false;

	return true;
}

void CUIImage::Start()
{
	CUIControl::Start();
}

void CUIImage::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CUIImage::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CUIImage::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CUIImage::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CUIImage::Render(float fTime)
{
	m_pTexture->SetShader(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	CUIControl::Render(fTime);

	//m_pMesh->Render(fTime);
}

void CUIImage::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CUIImage::Clone()
{
	return std::shared_ptr<CComponent>(new CUIImage(*this));
}

void CUIImage::Save(FILE* pFile)
{
	CUIControl::Save(pFile);
}

void CUIImage::Load(FILE* pFile)
{
	CUIControl::Load(pFile);
}
