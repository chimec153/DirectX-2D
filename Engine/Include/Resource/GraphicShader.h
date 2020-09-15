#pragma once
#include "Shader.h"
class CGraphicShader :
	public CShader
{
	friend class CShaderManager;
	friend class CSceneResource;

protected:
	CGraphicShader();
	virtual ~CGraphicShader();

protected:
	ID3D11VertexShader* m_pVS;
	ID3DBlob* m_pVSBlob;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pPSBlob;
	ID3D11HullShader* m_pHS;
	ID3DBlob* m_pHSBlob;
	ID3D11GeometryShader* m_pGS;
	ID3DBlob* m_pGSBlob;
	ID3D11DomainShader* m_pDS;
	ID3DBlob* m_pDSBlob;

public:
	bool LoadVertexShader(const char* pSemantic, const TCHAR* pFileName,
		const std::string& strPathName = SHADER_PATH);
	bool LoadPixelShader(const char* pSemantic, const TCHAR* pFileName,
		const std::string& strPathName = SHADER_PATH);
	bool LoadHullShader(const char* pSemantic, const TCHAR* pFileName,
		const std::string& strPathName = SHADER_PATH);
	bool LoadGeometryShader(const char* pSemantic, const TCHAR* pFileName,
		const std::string& strPathName = SHADER_PATH);
	bool LoadDomainShader(const char* pSemantic, const TCHAR* pFileName,
		const std::string& strPathName = SHADER_PATH);

public:
	bool CreateInputLayout();
	virtual void SetShader();
};

