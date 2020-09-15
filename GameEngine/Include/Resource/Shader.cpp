#include "Shader.h"

CShader::CShader()	:
	m_iDescSize(0),
	m_pInputLayout(nullptr),
	m_eType(SHADER_TYPE::ST_COMPUTE)
{
}

CShader::~CShader()
{
	SAFE_RELEASE(m_pInputLayout);
}

void CShader::SetShader()
{
}

bool CShader::CreateInputLayout()
{
	return false;
}

bool CShader::AddInputDesc(const char* pSemanticName, UINT iSemanticIndex, 
	DXGI_FORMAT eFmt, UINT iInputSlot, UINT iAlignedByteOffset, 
	D3D11_INPUT_CLASSIFICATION eInputSlotClass, UINT iInstanceDataStepRate)
{
	D3D11_INPUT_ELEMENT_DESC tDesc = {};

	tDesc.Format = eFmt;
	tDesc.InputSlot = iInputSlot;
	tDesc.InputSlotClass = eInputSlotClass;
	tDesc.InstanceDataStepRate = iInstanceDataStepRate;
	tDesc.SemanticIndex = iSemanticIndex;
	tDesc.SemanticName = pSemanticName;
	tDesc.AlignedByteOffset = m_iDescSize;

	m_vecInputDesc.push_back(tDesc);

	m_iDescSize += iAlignedByteOffset;

	return true;
}
