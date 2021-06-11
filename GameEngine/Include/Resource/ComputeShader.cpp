#include "ComputeShader.h"
#include "../Device.h"
#include "../PathManager.h"

CComputeShader::CComputeShader()
{
	m_eType = SHADER_TYPE::ST_COMPUTE;
}

CComputeShader::~CComputeShader()
{
}

bool CComputeShader::LoadComputeShader(const char* pEntry, const TCHAR* pFileName, const std::string& strPathKey)
{
	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3D11ComputeShader* pShader = nullptr;

	ID3DBlob* pBlob = nullptr;

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry, "cs_5_0",
		iFlag, 0, &pBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		return false;
	}

	if (FAILED(DEVICE->CreateComputeShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), nullptr, &pShader)))
	{
		return false;
	}

	m_pCSBlob = std::shared_ptr<ID3DBlob>(pBlob, deleter());

	m_pCS = std::shared_ptr<ID3D11ComputeShader>(pShader, deleter());

	m_iCBufferType |= (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE;

	return true;
}

void CComputeShader::SetShader()
{
	ID3D11ComputeShader* pShader = m_pCS.get();

	CONTEXT->CSSetShader(pShader, NULL, 0);
}

void CComputeShader::Dispatch(int iCountX, int iCountY, int iCountZ)
{
	CONTEXT->Dispatch(iCountX, iCountY, iCountZ);
}

void CComputeShader::Clear()
{
	CONTEXT->CSSetShader(NULL, NULL, 0);
}
