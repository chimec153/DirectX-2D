#include "Distortion.h"
#include "../Resource/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"

CDistortion::CDistortion()
{
}

CDistortion::CDistortion(const CDistortion& pro)	:
	CPostProcess(pro)
	, m_pMaterial(pro.m_pMaterial->Clone())
{
}

CDistortion::~CDistortion()
{
}

void CDistortion::SetMaterial(const std::string& strMtrl)
{
	m_pMaterial = GET_SINGLE(CResourceManager)->FindMaterial(strMtrl);
}

void CDistortion::SetWeight(float fWeight)
{
	m_tCBuffer.fWeight = fWeight;
}

void CDistortion::SetSpeed(float fSpeed)
{
	m_tCBuffer.fSpeed = fSpeed;
}

void CDistortion::SetFrequency(float fFre)
{
	m_tCBuffer.fFrequency = fFre;
}

bool CDistortion::Init()
{
	if (!CPostProcess::Init())
		return false;

	return true;
}

void CDistortion::Start()
{
	CPostProcess::Start();
}

void CDistortion::Update(float fTime)
{
	CPostProcess::Update(fTime);
}

void CDistortion::PostUpdate(float fTime)
{
	CPostProcess::PostUpdate(fTime);
}

void CDistortion::Collision(float fTime)
{
	CPostProcess::Collision(fTime);
}

void CDistortion::PreRender(float fTime)
{
	CPostProcess::PreRender(fTime);
}

void CDistortion::Render(float fTime)
{
	CPostProcess::Render(fTime);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Distortion", &m_tCBuffer);
}

void CDistortion::PostRender(float fTime)
{
	CPostProcess::PostRender(fTime);
}
