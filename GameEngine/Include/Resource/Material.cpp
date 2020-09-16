#include "Material.h"
#include "Shader.h"
#include "ShaderManager.h"

CMaterial::CMaterial()	:
	m_pShader(nullptr)
{
	m_tCBuffer.vDif = Vector4::White;
	m_tCBuffer.vEmb = Vector4::White;
	m_tCBuffer.vSpec = Vector4::White;
}

CMaterial::CMaterial(const CMaterial& material)
{
	*this = material;
}

CMaterial::~CMaterial()
{
	SAFE_RELEASE(m_pShader);
}

void CMaterial::SetShader(const std::string& strName)
{
	SAFE_RELEASE(m_pShader);

	m_pShader = GET_SINGLE(CShaderManager)->FindShader(strName);
}

bool CMaterial::Init()
{
	return true;
}

void CMaterial::Start()
{
}

void CMaterial::Update(float fTime)
{
}

void CMaterial::PostUpdate(float fTime)
{
}

void CMaterial::Collision(float fTime)
{
}

void CMaterial::PreRender(float fTime)
{
}

void CMaterial::Render(float fTime)
{
}

void CMaterial::PostRender(float fTime)
{
}
