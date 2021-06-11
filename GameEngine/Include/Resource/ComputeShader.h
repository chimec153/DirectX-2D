#pragma once
#include "Shader.h"

class CComputeShader :
	public CShader
{
public:
	CComputeShader();
	virtual ~CComputeShader();

private:
	std::shared_ptr<ID3DBlob> m_pCSBlob;
	std::shared_ptr<ID3D11ComputeShader> m_pCS;

public:
	bool LoadComputeShader(const char* pEntry, const TCHAR* pFileName, const std::string& strPathKey = SHADER_PATH);
	virtual void SetShader();
	void Dispatch(int iCountX, int iCountY, int iCountZ);
	void Clear();
};
