#include "Sample.h"
#include "../Device.h"

CSample::CSample()	:
	m_pState(nullptr)
{
}

CSample::~CSample()
{
	SAFE_RELEASE(m_pState);
}

ID3D11SamplerState* CSample::GetState() const
{
	return m_pState;
}

void CSample::SetSampler(int iRegister, int iType)
{
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX))
		CONTEXT->VSSetSamplers(iRegister, 1, &m_pState);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL))
		CONTEXT->PSSetSamplers(iRegister, 1, &m_pState);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_HULL))
		CONTEXT->HSSetSamplers(iRegister, 1, &m_pState);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN))
		CONTEXT->DSSetSamplers(iRegister, 1, &m_pState);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY))
		CONTEXT->GSSetSamplers(iRegister, 1, &m_pState);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE))
		CONTEXT->CSSetSamplers(iRegister, 1, &m_pState);
}

void CSample::ResetSampler(int iRegister, int iType)
{
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX))
		CONTEXT->VSSetSamplers(iRegister, 1, &m_pState);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL))
		CONTEXT->PSSetSamplers(iRegister, 1, &m_pState);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_HULL))
		CONTEXT->HSSetSamplers(iRegister, 1, &m_pState);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN))
		CONTEXT->DSSetSamplers(iRegister, 1, &m_pState);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY))
		CONTEXT->GSSetSamplers(iRegister, 1, &m_pState);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE))
		CONTEXT->CSSetSamplers(iRegister, 1, &m_pState);
}

bool CSample::CreateSample(D3D11_FILTER eFilter, D3D11_TEXTURE_ADDRESS_MODE eUMode, 
	D3D11_TEXTURE_ADDRESS_MODE eVMode, D3D11_TEXTURE_ADDRESS_MODE eWMode, 
	D3D11_COMPARISON_FUNC eFunc, int iMaxAnisotropy, float pColor[4])
{	
	D3D11_SAMPLER_DESC tDesc = {};

	tDesc.Filter = eFilter;
	tDesc.AddressU = eUMode;
	tDesc.AddressV = eVMode;
	tDesc.AddressW = eWMode;
	tDesc.ComparisonFunc = eFunc;
	tDesc.MaxLOD = FLT_MAX;
	tDesc.MinLOD = -FLT_MAX;
	tDesc.MaxAnisotropy = iMaxAnisotropy;

	if(pColor)
	memcpy(tDesc.BorderColor, pColor, 16);

	if (FAILED(DEVICE->CreateSamplerState(&tDesc, &m_pState)))
		return false;

	return true;
}
