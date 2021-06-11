#include "Bar.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Material.h"
#include "../Resource/Texture.h"

CBar::CBar()	:
	m_eDir(DIR::R),
	m_fPct(1.f)
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;

	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_BAR;
}

CBar::CBar(const CBar& ui)	:
	CUIControl(ui)
{
	m_eDir = ui.m_eDir;
	m_fPct = ui.m_fPct;
	m_tCBuffer = ui.m_tCBuffer;
}

CBar::~CBar()
{
}

void CBar::SetPrg(float f)
{
	m_fPct = f;
}

void CBar::SetDir(DIR e)
{
	m_eDir = e;
}

float CBar::GetPercent() const
{
	return m_fPct;
}

DIR CBar::GetDir() const
{
	return m_eDir;
}

bool CBar::Init()
{
	if (!CUIControl::Init())
		return false;

	SetShader("Bar");

	return true;
}

void CBar::Start()
{
	CUIControl::Start();
}

void CBar::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CBar::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CBar::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CBar::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CBar::Render(float fTime)
{
	m_tCBuffer.iDir = (int)m_eDir;
	m_tCBuffer.fPrg = m_fPct;

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Bar", &m_tCBuffer);

	if (m_pTexture)
		m_pTexture->SetShader(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	CUIControl::Render(fTime);

	//m_pMesh->Render(fTime);
}

void CBar::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CBar::Clone()
{
	return std::shared_ptr<CComponent>(new CBar(*this));
}

void CBar::Save(FILE* pFile)
{
	CUIControl::Save(pFile);

	fwrite(&m_eDir, 4, 1, pFile);
	fwrite(&m_fPct, 4, 1, pFile);
	fwrite(&m_tCBuffer, sizeof(BARCBUFFER), 1, pFile);
}

void CBar::Load(FILE* pFile)
{
	CUIControl::Load(pFile);

	fread(&m_eDir, 4, 1, pFile);
	fread(&m_fPct, 4, 1, pFile);
	fread(&m_tCBuffer, sizeof(BARCBUFFER), 1, pFile);
}

void CBar::ShowWindow()
{
	CUIControl::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{

	}
	ImGui::End();
}
