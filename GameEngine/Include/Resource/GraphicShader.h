#pragma once
#include "Shader.h"

	class CGraphicShader :
		public CShader
	{
		friend class CShaderManager;
		friend class CSceneResource;

	public:
		CGraphicShader();
		virtual ~CGraphicShader();

	protected:
		std::shared_ptr<ID3D11VertexShader> m_pVS;
		std::shared_ptr<ID3DBlob> m_pVSBlob;
		std::shared_ptr<ID3D11PixelShader> m_pPS;
		std::shared_ptr<ID3DBlob> m_pPSBlob;
		std::shared_ptr<ID3D11HullShader> m_pHS;
		std::shared_ptr<ID3DBlob> m_pHSBlob;
		std::shared_ptr<ID3D11GeometryShader> m_pGS;
		std::shared_ptr<ID3DBlob> m_pGSBlob;
		std::shared_ptr<ID3D11DomainShader> m_pDS;
		std::shared_ptr<ID3DBlob> m_pDSBlob;
		std::shared_ptr<ID3D11ComputeShader> m_pCS;
		std::shared_ptr<ID3DBlob> m_pCSBlob;

	protected:
		char				m_pSemantic[256];
		TCHAR				m_pFileName[256];
		std::string			m_strPathName;

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
		bool LoadComputeShader(const char* pSemantic, const TCHAR* pFileName,
			const std::string& strPathName = SHADER_PATH);

	public:
		bool CreateInputLayout();
		virtual void SetShader();
	};
