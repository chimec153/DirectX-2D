#include "UIControl.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"
#include "imgui/imgui.h"

CUIControl::CUIControl() :
	m_pTexture(nullptr)
{
}

CUIControl::CUIControl(const CUIControl& com) :
	CSceneComponent(com)
	, m_tCBuffer(com.m_tCBuffer)
{
	m_pTexture = com.m_pTexture;
}

CUIControl::~CUIControl()
{
}

void CUIControl::SetUV(const Vector2& vStart, const Vector2& vEnd)
{
	m_tCBuffer.eImageType = (int)IMAGE_TYPE::IT_ATLAS;

	m_tCBuffer.vStart = vStart;
	m_tCBuffer.vEnd = vEnd;
	if (m_pTexture)
	{
		m_tCBuffer.vSize.x = (float)m_pTexture->GetWidth();
		m_tCBuffer.vSize.y = (float)m_pTexture->GetHeight();
	}
}

void CUIControl::SetSize(const Vector2& vSize)
{
	m_tCBuffer.vSize = vSize;
}

bool CUIControl::SetTexture(std::shared_ptr<CTexture> pTex)
{
	m_pTexture = pTex;

	return true;
}

bool CUIControl::SetTexture(const std::string& strTag)
{
	m_pTexture = GET_SINGLE(CResourceManager)->FindTexture(strTag);

	

	return static_cast<bool>(m_pTexture);
}


bool CUIControl::Init()
{
	if (!CSceneComponent::Init())
		return false;

	class CMesh* pMesh = GET_SINGLE(CResourceManager)->GetDefaultUIMesh();

	SetMesh(pMesh);

	
	std::shared_ptr<CMaterial> pMat = pMesh->GetMaterial();

	SAFE_RELEASE(pMesh);

	std::shared_ptr<CMaterial> pMaterial = pMat->Clone();

	SetMaterial(pMaterial);

	m_tCBuffer.eImageType = (int)IMAGE_TYPE::IT_ARRAY;

	return true;
}

void CUIControl::Start()
{
	CSceneComponent::Start();
}

void CUIControl::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CUIControl::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CUIControl::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CUIControl::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CUIControl::Render(float fTime)
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Sprite", &m_tCBuffer);

	CSceneComponent::Render(fTime);
}

void CUIControl::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

std::shared_ptr<CComponent> CUIControl::Clone()
{
	return nullptr;
}


void CUIControl::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	bool bTex = false;

	if (m_pTexture)
		bTex = true;

	fwrite(&bTex, 1, 1, pFile);

	if (m_pTexture)
		m_pTexture->Save(pFile);

	fwrite(&m_tCBuffer, sizeof(AniCBuffer), 1, pFile);
}

void CUIControl::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	bool bTex = false;

	fread(&bTex, 1, 1, pFile);

	if (bTex)
		m_pTexture = GET_SINGLE(CResourceManager)->LoadTexture(pFile);

	fread(&m_tCBuffer, sizeof(AniCBuffer), 1, pFile);
}

void CUIControl::ShowWindow()
{
	CSceneComponent::ShowWindow();

	std::string strName = GetName();
	
	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		ImGui::Text("UI Control");

		if (ImGui::InputFloat2("Sprite Size", &m_tCBuffer.vSize.x))
		{
			SetSize(m_tCBuffer.vSize);
		}
		if (ImGui::InputFloat2("Sprite Start", &m_tCBuffer.vStart.x))
		{
			SetUV(m_tCBuffer.vStart, m_tCBuffer.vEnd);
		}
		if (ImGui::InputFloat2("Sprite End", &m_tCBuffer.vEnd.x))
		{
			SetUV(m_tCBuffer.vStart, m_tCBuffer.vEnd);
		}
		if (ImGui::SliderInt("Image Type", &m_tCBuffer.eImageType, static_cast<int>(IMAGE_TYPE::IT_ATLAS), static_cast<int>(IMAGE_TYPE::TEXTUREARRAY)))
		{
		}
		ImGui::InputFloat("Frame", &m_tCBuffer.fFrame);
	}
	ImGui::End();
}
