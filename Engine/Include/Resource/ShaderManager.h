#pragma once

#include "../GameEngine.h"
#include "Shader.h"

	class CShaderManager
	{
	private:
		std::unordered_map<std::string, ID3D11InputLayout*>	m_mapInputLayout;
		std::unordered_map<std::string, std::shared_ptr<class CShader>>		m_mapShader;
		std::unordered_map<std::string, PCBuffer>				m_mapCBuffer;		

	public:
		bool Init();
		void Update(float fTime);
		bool AddInputDesc(const std::string& strName, const char* pSemanticName, UINT iSemanticIndex,
			DXGI_FORMAT eFmt, UINT iInputSlot, UINT iAlignedByteOffset,
			D3D11_INPUT_CLASSIFICATION eInputSlotClass, UINT iInstanceDataStepRate);
		bool CreateInputLayout(const std::string& strName);

		std::shared_ptr<class CShader> FindShader(const std::string& strName);

		ID3D11InputLayout* FindInputLayout(const std::string& strName);

	public:
		bool CreateCBuffer(const std::string& strTag, int iSize, int iRegister,
			int iType = ((int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX) | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
		bool UpdateCBuffer(const std::string& strTag, void* pData);

	private:
		PCBuffer FindCBuffer(const std::string& strTag);

	public:
		template <typename T>
		std::shared_ptr<class CShader> CreateShader(const std::string& strName)
		{
			std::shared_ptr<class CShader> pShader = FindShader(strName);

			if (pShader)
			{
				return nullptr;
			}

			pShader = std::shared_ptr<class CShader>(new T);

			pShader->SetName(strName);

			m_mapShader.insert(std::make_pair(strName, pShader));

			return pShader;
		}

	public:
		bool LoadVertexShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName,
			const std::string& strPathName = SHADER_PATH);
		bool LoadPixelShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName,
			const std::string& strPathName = SHADER_PATH);
		bool LoadHullShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName,
			const std::string& strPathName = SHADER_PATH);
		bool LoadGeometryShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName,
			const std::string& strPathName = SHADER_PATH);
		bool LoadDomainShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName,
			const std::string& strPathName = SHADER_PATH);
		bool LoadComputeShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName,
			const std::string& strPathName = SHADER_PATH);

		DECLARE_SINGLE(CShaderManager)
	};
