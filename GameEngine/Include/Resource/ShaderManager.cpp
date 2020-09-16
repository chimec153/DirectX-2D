#include "ShaderManager.h"
#include "Shader.h"
#include "../Device.h"
#include "GraphicShader.h"

DEFINITION_SINGLE(CShaderManager)

CShaderManager::CShaderManager()
{

}

CShaderManager::~CShaderManager()
{
	SAFE_RELEASE_MAP(m_mapShader);
	SAFE_RELEASE_MAP(m_mapInputLayout);
}

bool CShaderManager::Init()
{
	CShader* pShader = CreateShader<CGraphicShader>("Standard2D");

	if (!LoadVertexShader("Standard2D", "GetOutputVS", 
		TEXT("VertexShader.fx")))
		return false;

	if (!LoadPixelShader("Standard2D", "GetOutputPS", 
		TEXT("VertexShader.fx")))
		return false;

	pShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);

	return true;
}

bool CShaderManager::AddInputDesc(const std::string& strName, const char* pSemanticName, UINT iSemanticIndex, 
	DXGI_FORMAT eFmt, UINT iInputSlot, UINT iAlignedByteOffset, 
	D3D11_INPUT_CLASSIFICATION eInputSlotClass, UINT iInstanceDataStepRate)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	pShader->AddInputDesc(pSemanticName, iSemanticIndex, eFmt, 
		iInputSlot, iAlignedByteOffset, eInputSlotClass, iInstanceDataStepRate);

	return true;
}

bool CShaderManager::CreateInputLayout(const std::string& strName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	bool bResult = pShader->CreateInputLayout();

	SAFE_RELEASE(pShader);

	return bResult;
}

CShader* CShaderManager::FindShader(const std::string& strName)
{
	std::unordered_map<std::string, CShader*>::iterator iter = m_mapShader.find(strName);

	if (iter == m_mapShader.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CShaderManager::ReleaseShader(const std::string& strName)
{
	std::unordered_map<std::string, CShader*>::iterator iter = m_mapShader.find(strName);

	if (iter == m_mapShader.end())
		return;

	if (iter->second->Release() == 0)
		m_mapShader.erase(iter);
}

ID3D11InputLayout* CShaderManager::FindInputLayout(const std::string& strName)
{
	std::unordered_map<std::string, ID3D11InputLayout*>::iterator iter = m_mapInputLayout.find(strName);

	if (iter == m_mapInputLayout.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CShaderManager::ReleaseInputLayout(const std::string& strName)
{
	std::unordered_map<std::string, ID3D11InputLayout*>::iterator iter = m_mapInputLayout.find(strName);

	if (iter == m_mapInputLayout.end())
		return;

	iter->second->Release();
	m_mapInputLayout.erase(iter);
}

bool CShaderManager::LoadVertexShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	if (!((CGraphicShader*)pShader)->LoadVertexShader(pEntryName, pFileName, strPathName))
	{
		SAFE_RELEASE(pShader);

		return false;
	}

	SAFE_RELEASE(pShader);

	return true;
}

bool CShaderManager::LoadPixelShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	if (!((CGraphicShader*)pShader)->LoadPixelShader(pEntryName, pFileName, strPathName))
	{
		SAFE_RELEASE(pShader);

		return false;
	}

	SAFE_RELEASE(pShader);

	return true;
}

bool CShaderManager::LoadHullShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	if (!((CGraphicShader*)pShader)->LoadHullShader(pEntryName, pFileName, strPathName))
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);
	return true;
}

bool CShaderManager::LoadGeometryShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	if (!((CGraphicShader*)pShader)->LoadGeometryShader(pEntryName, pFileName, strPathName))
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);
	return true;
}

bool CShaderManager::LoadDomainShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	if (!((CGraphicShader*)pShader)->LoadDomainShader(pEntryName, pFileName, strPathName))
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);
	return true;
}
