#pragma once
#include "../Ref.h"
class CShader :
	public CRef
{
	friend class CShaderManager;
	friend class CSceneResource;

protected:
	CShader();
	virtual ~CShader() = 0;

protected:
	std::vector<D3D11_INPUT_ELEMENT_DESC>		m_vecInputDesc;
	UINT										m_iDescSize;
	ID3D11InputLayout*							m_pInputLayout;
	SHADER_TYPE									m_eType;

public:
	SHADER_TYPE GetShaderType()	const
	{
		return m_eType;
	}


public:
	virtual void SetShader();
	virtual bool CreateInputLayout();
	virtual bool AddInputDesc(const char* pSemanticName, UINT iSemanticIndex,
		DXGI_FORMAT eFmt, UINT iInputSlot, UINT iAlignedByteOffset,
		D3D11_INPUT_CLASSIFICATION eInputSlotClass, UINT iInstanceDataStepRate);

};

