#include "Shader.h"

CShader::CShader()	:
	m_iDescSize(0),
	m_iInstSize(0),
	m_pInputLayout(nullptr),
	m_eType(SHADER_TYPE::ST_COMPUTE),
	m_iCBufferType(0)
{
}

CShader::~CShader()
{
	SAFE_RELEASE(m_pInputLayout);
}

SHADER_TYPE CShader::GetShaderType() const
{
	return m_eType;
}

UINT CShader::GetDescSize() const
{
	return m_iDescSize;
}

UINT CShader::GetInstSize() const
{
	return m_iInstSize;
}

void CShader::SetShader()
{
}

bool CShader::CreateInputLayout()
{
	return false;
}

bool CShader::AddInputDesc(const char* pSemanticName,
	UINT iSemanticIndex, DXGI_FORMAT eFmt, UINT iInputSlot,
	UINT iAlignedByteOffset, D3D11_INPUT_CLASSIFICATION eInputSlotClass,
	UINT iInstanceDataStepRate)
{
	D3D11_INPUT_ELEMENT_DESC tDesc = {};

	tDesc.SemanticName = pSemanticName;
	tDesc.SemanticIndex = iSemanticIndex;
	tDesc.Format = eFmt;
	tDesc.InputSlot = iInputSlot;

	if (eInputSlotClass == D3D11_INPUT_PER_INSTANCE_DATA)
	{
		tDesc.AlignedByteOffset = m_iInstSize;
		m_iInstSize += iAlignedByteOffset;
	}

	else
	{
		tDesc.AlignedByteOffset = m_iDescSize;
		m_iDescSize += iAlignedByteOffset;
	}

	tDesc.InputSlotClass = eInputSlotClass;
	tDesc.InstanceDataStepRate = iInstanceDataStepRate;

	m_vecInputDesc.push_back(tDesc);

	//m_iDescSize += iAlignedByteOffset;
	
	return true;
}
