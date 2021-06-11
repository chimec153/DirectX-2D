#include "PaperBurn.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture.h"
#include "../UI/imgui/imgui.h"

CPaperBurn::CPaperBurn()	:
	m_bBurn(false)
	, m_fScale(2.f)
	, m_bLoop(false)
{
	memset(&m_tCBuffer, 0, sizeof(m_tCBuffer));
	m_tCBuffer.fOutFilter = 1.9f;
	m_tCBuffer.fMidFilter = 1.8f;
	m_tCBuffer.fInFilter = 1.7f;
	m_tCBuffer.vOutColor = Vector3(0.f, 0.f, 0.f);
	m_tCBuffer.vColor = Vector3(1.f, 0.f, 0.f);
	m_tCBuffer.vInColor = Vector3(1.f, 1.f, 0.f);
	m_tCBuffer.iEnable = true;
	m_eType = COMPONENT_TYPE::CT_OBJECT;
	SetClassType(OBJ_COMPONENT_CLASS_TYPE::PAPERBURN);
}

CPaperBurn::CPaperBurn(const CPaperBurn& com)	:
	CObjectComponent(com)
	, m_tCBuffer(com.m_tCBuffer)
	, m_pPaperTex(com.m_pPaperTex)
	, m_bBurn(com.m_bBurn)
	, m_fScale(com.m_fScale)
	, m_bLoop(com.m_bLoop)
{
}

CPaperBurn::~CPaperBurn()
{
}

void CPaperBurn::AddEndCallBack(void(*pFunc)(float))
{
	m_vecEndCallBack.push_back(std::bind(pFunc, std::placeholders::_1));
}

void CPaperBurn::SetLoop(bool bLoop)
{
	m_bLoop = bLoop;
}

void CPaperBurn::SetBurnTexture(const std::string& strKey)
{
	m_pPaperTex = GET_SINGLE(CResourceManager)->FindTexture(strKey);
}

void CPaperBurn::SetFilter(float fFilter)
{
	m_tCBuffer.fFilter = fFilter;
}

void CPaperBurn::ToggleBurn()
{
	m_bBurn ^= true;
	SetEnable(true);
}

void CPaperBurn::SetScale(float fScale)
{
	m_fScale = fScale;
}

void CPaperBurn::SetOutFilter(float fFilter)
{
	m_tCBuffer.fOutFilter = fFilter;
}

void CPaperBurn::SetMidFilter(float fFilter)
{
	m_tCBuffer.fMidFilter = fFilter;
}

void CPaperBurn::SetInFilter(float fFilter)
{
	m_tCBuffer.fInFilter = fFilter;
}

void CPaperBurn::SetOutColor(const Vector3& vColor)
{
	m_tCBuffer.vOutColor = vColor;
}

void CPaperBurn::SetColor(const Vector3& vColor)
{
	m_tCBuffer.vColor = vColor;
}

void CPaperBurn::SetInColor(const Vector3& vColor)
{
	m_tCBuffer.vInColor = vColor;
}

void CPaperBurn::SetOutColor(float r, float g, float b)
{
	m_tCBuffer.vOutColor = Vector3(r, g, b);
}

void CPaperBurn::SetColor(float r, float g, float b)
{
	m_tCBuffer.vColor = Vector3(r, g, b);
}

void CPaperBurn::SetInColor(float r, float g, float b)
{
	m_tCBuffer.vInColor = Vector3(r, g, b);
}

bool CPaperBurn::Init()
{
	if (!CObjectComponent::Init())
		return false;

	GET_SINGLE(CResourceManager)->LoadTexture("PaperBurn", TEXT("DefaultBurn.png"));

	m_pPaperTex = GET_SINGLE(CResourceManager)->FindTexture("PaperBurn");

	return true;
}

void CPaperBurn::Start()
{
	CObjectComponent::Start();
}

void CPaperBurn::Update(float fTime)
{
	CObjectComponent::Update(fTime);

	m_tCBuffer.fFilter += m_fScale*  fTime * (m_bBurn * 2 - 1);

	if (m_tCBuffer.fFilter >= 2.f)
	{
		m_tCBuffer.fFilter = 2.f;

		size_t iSize = m_vecEndCallBack.size();

		for (size_t i = 0; i < iSize; i++)
		{
			m_vecEndCallBack[i](fTime);
		}

		if (m_bLoop)
		{
			ToggleBurn();
		}
		//SetEnable(false);
	}

	else if (m_tCBuffer.fFilter <= 0.f)
	{
		m_tCBuffer.fFilter = 0.f;

		if (m_bLoop)
		{
			ToggleBurn();
		}
		//SetEnable(true);
	}
}

void CPaperBurn::PostUpdate(float fTime)
{
	CObjectComponent::PostUpdate(fTime);
}

void CPaperBurn::Collision(float fTime)
{
	CObjectComponent::Collision(fTime);
}

void CPaperBurn::PreRender(float fTime)
{
	CObjectComponent::PreRender(fTime);
}

void CPaperBurn::Render(float fTime)
{
	CObjectComponent::Render(fTime);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("PaperBurn", &m_tCBuffer);

	m_pPaperTex->SetShader(99, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
}

void CPaperBurn::PostRender(float fTime)
{
	CObjectComponent::PostRender(fTime);
}

std::shared_ptr<class CComponent> CPaperBurn::Clone()
{
	return std::shared_ptr<class CComponent>(new CPaperBurn(*this));
}

void CPaperBurn::Save(FILE * pFile)
{
	CObjectComponent::Save(pFile);

	fwrite(&m_tCBuffer, sizeof(PaperBurnCBuffer), 1, pFile);

	bool bTexture = false;
	if (m_pPaperTex)
	{
		bTexture = true;
	}

	fwrite(&bTexture, 1, 1, pFile);

	if (m_pPaperTex)
	{
		m_pPaperTex->Save(pFile);
	}

	fwrite(&m_bBurn,1, 1, pFile);
	fwrite(&m_fScale, 4, 1, pFile);
}

void CPaperBurn::Load(FILE * pFile)
{
	CObjectComponent::Load(pFile);

	fread(&m_tCBuffer, sizeof(PaperBurnCBuffer), 1, pFile);

	bool bTexture = false;

	fread(&bTexture, 1, 1, pFile);

	if (bTexture)
	{
		m_pPaperTex = GET_SINGLE(CResourceManager)->LoadTexture(pFile);
	}

	fread(&m_bBurn, 1, 1, pFile);
	fread(&m_fScale, 4, 1, pFile);
}

void CPaperBurn::ShowWindow()
{
	CObjectComponent::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		ImGui::Text("Paper Burn Component");
		ImGui::SliderFloat("Filter", &m_tCBuffer.fFilter, 0.f, 2.f);
		ImGui::InputFloat("Out Filter", &m_tCBuffer.fOutFilter);
		ImGui::InputFloat("Mid Filter", &m_tCBuffer.fMidFilter);
		ImGui::InputFloat("In Filter", &m_tCBuffer.fInFilter);
		ImGui::Checkbox("Reverse", reinterpret_cast<bool*>(&m_tCBuffer.iReverse));
		ImGui::ColorPicker3("Out Color", &m_tCBuffer.vOutColor.x);
		ImGui::ColorPicker3("Color", &m_tCBuffer.vColor.x);
		ImGui::ColorPicker3("In Color", &m_tCBuffer.vInColor.x);
		ImGui::SliderFloat("Scale", &m_fScale, 0.f, 7.f);
		ImGui::Checkbox("Burn", &m_bBurn);

		if (m_pPaperTex)
		{
			m_pPaperTex->ShowWindow();
		}
	}
	ImGui::End();
}
