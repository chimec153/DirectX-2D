#pragma once

#include "../GameEngine.h"

class CShaderManager
{
private:
	std::unordered_map<std::string, ID3D11InputLayout*>	m_mapInputLayout;
	std::unordered_map<std::string, class CShader*>		m_mapShader;

public:
	bool Init();
	bool AddInputDesc(const std::string& strName, const char* pSemanticName, UINT iSemanticIndex,
		DXGI_FORMAT eFmt, UINT iInputSlot, UINT iAlignedByteOffset,
		D3D11_INPUT_CLASSIFICATION eInputSlotClass, UINT iInstanceDataStepRate);
	bool CreateInputLayout(const std::string& strName);

	class CShader* FindShader(const std::string& strName);
	void ReleaseShader(const std::string& strName);	

	ID3D11InputLayout* FindInputLayout(const std::string& strName);
	void ReleaseInputLayout(const std::string& strName);

public:
	template <typename T>
	T* CreateShader(const std::string& strName)
	{
		T* pShader = (T*)FindShader(strName);

		if (pShader)
		{
			SAFE_RELEASE(pShader);
			return nullptr;
		}

		pShader = new T;

		pShader->SetName(strName);

		pShader->AddRef();

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

	DECLARE_SINGLE(CShaderManager)
};
