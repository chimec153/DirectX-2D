#include "GraphicShader.h"
#include "../Device.h"
#include "ShaderManager.h"
#include "../PathManager.h"

CGraphicShader::CGraphicShader()	:
	m_pVS(nullptr),
	m_pVSBlob(nullptr),
	m_pPS(nullptr),
	m_pPSBlob(nullptr),
	m_pHS(nullptr),
	m_pHSBlob(nullptr),
	m_pGS(nullptr),
	m_pGSBlob(nullptr),
	m_pDS(nullptr),
	m_pDSBlob(nullptr),
	m_pCS(nullptr),
	m_pCSBlob(nullptr)
{
	memset(m_pSemantic, 0, 256);
	memset(m_pFileName, 0, sizeof(TCHAR) * 256);
	m_eType = SHADER_TYPE::ST_GRAPHIC;
}

CGraphicShader::~CGraphicShader()
{
}

bool CGraphicShader::LoadVertexShader(const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3D11VertexShader* pShader = nullptr;

	ID3DBlob* pBlob = nullptr;

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "vs_5_0",
		iFlag, 0, &pBlob, &pError)))
	{
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	m_pVSBlob = std::shared_ptr<ID3DBlob>(pBlob, deleter());

	if (FAILED(DEVICE->CreateVertexShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), nullptr, &pShader)))
		return false;

	m_pVS = std::shared_ptr<ID3D11VertexShader>(pShader, deleter());

	m_iCBufferType |= (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX;

	return true;
}

bool CGraphicShader::LoadPixelShader(const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3D11PixelShader* pShader = nullptr;

	ID3DBlob* pBlob = nullptr;

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "ps_5_0",
		iFlag, 0, &pBlob, &pError)))
	{
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(DEVICE->CreatePixelShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), nullptr, &pShader)))
	{
		return false;
	}

	m_pPSBlob = std::shared_ptr<ID3DBlob>(pBlob, deleter());

	m_pPS = std::shared_ptr<ID3D11PixelShader>(pShader, deleter());

	m_iCBufferType |= (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL;

	return true;
}

bool CGraphicShader::LoadHullShader(const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3D11HullShader* pShader = nullptr;

	ID3DBlob* pBlob = nullptr;

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "hs_5_0",
		iFlag, 0, &pBlob, &pError)))
	{
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(DEVICE->CreateHullShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), nullptr, &pShader)))
		return false;

	m_pHSBlob = std::shared_ptr<ID3DBlob>(pBlob, deleter());

	m_pHS = std::shared_ptr<ID3D11HullShader>(pShader, deleter());

	m_iCBufferType |= (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL;

	return true;
}

bool CGraphicShader::LoadGeometryShader(const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3D11GeometryShader* pShader = m_pGS.get();

	ID3DBlob* pBlob = m_pGSBlob.get();

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "gs_5_0",
		iFlag, 0, &pBlob, &pError)))
	{
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(DEVICE->CreateGeometryShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), nullptr, &pShader)))
		return false;

	m_pGSBlob = std::shared_ptr<ID3DBlob>(pBlob, deleter());

	m_pGS = std::shared_ptr<ID3D11GeometryShader>(pShader, deleter());

	m_iCBufferType |= (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY;

	return true;
}

bool CGraphicShader::LoadDomainShader(const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3D11DomainShader* pShader = m_pDS.get();

	ID3DBlob* pBlob = m_pDSBlob.get();

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "ds_5_0",
		iFlag, 0, &pBlob, &pError)))
	{
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(DEVICE->CreateDomainShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), nullptr, &pShader)))
		return false;

	m_pDS = std::shared_ptr<ID3D11DomainShader>(pShader, deleter());

	m_pDSBlob = std::shared_ptr<ID3DBlob>(pBlob, deleter());

	m_iCBufferType |= (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN;

	return true;
}

bool CGraphicShader::LoadComputeShader(const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3D11ComputeShader* pShader = m_pCS.get();

	ID3DBlob* pBlob = m_pCSBlob.get();

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "cs_5_0",
		iFlag, 0, &pBlob, &pError)))
	{
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(DEVICE->CreateComputeShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), nullptr, &pShader)))
		return false;

	m_pCSBlob = std::shared_ptr<ID3DBlob>(pBlob, deleter());

	m_pCS = std::shared_ptr<ID3D11ComputeShader>(pShader, deleter());

	m_iCBufferType |= (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE;

	return true;
}

bool CGraphicShader::CreateInputLayout()
{
	if (FAILED(DEVICE->CreateInputLayout(&m_vecInputDesc[0], 
		(UINT)m_vecInputDesc.size(), m_pVSBlob->GetBufferPointer(), 
		m_pVSBlob->GetBufferSize(), &m_pInputLayout)))
		return false;

	return true;
}

void CGraphicShader::SetShader()
{
	ID3D11VertexShader* pVS = m_pVS.get();
	ID3D11PixelShader* pPS = m_pPS.get();
	ID3D11HullShader* pHS = m_pHS.get();
	ID3D11DomainShader* pDS = m_pDS.get();
	ID3D11GeometryShader* pGS = m_pGS.get();
	ID3D11ComputeShader* pCS = m_pCS.get();

	CDevice::Enter();
	CONTEXT->VSSetShader(pVS, nullptr, 0);

	CONTEXT->PSSetShader(pPS, nullptr, 0);

	CONTEXT->HSSetShader(pHS, nullptr, 0);

	CONTEXT->DSSetShader(pDS, nullptr, 0);

	CONTEXT->GSSetShader(pGS, nullptr, 0);

	CONTEXT->CSSetShader(pCS, nullptr, 0);

	CONTEXT->IASetInputLayout(m_pInputLayout);
	CDevice::Leave();
}
