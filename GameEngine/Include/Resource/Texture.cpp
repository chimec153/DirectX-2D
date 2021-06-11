#include "Texture.h"
#include "../Device.h"
#include "../PathManager.h"
#include "ResourceManager.h"

CTexture::CTexture()	:
	m_eType(IMAGE_TYPE::IT_ATLAS)
	, m_pArraySRV(nullptr)
{
}

CTexture::~CTexture()
{
	size_t iSize = m_vecResourceInfo.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		SAFE_DELETE(m_vecResourceInfo[i]->pImage);
		SAFE_DELETE(m_vecResourceInfo[i]);
	}

	SAFE_RELEASE(m_pArraySRV);
}

PResourceInfo CTexture::GetInfo(int idx) const
{
	return m_vecResourceInfo[idx];
}

size_t CTexture::GetSize()	const
{
	return m_vecResourceInfo.size();
}

void CTexture::SetImageType(IMAGE_TYPE eType)
{
	m_eType = eType;
}

void CTexture::SetTexture(ID3D11ShaderResourceView* pSRV, int idx)
{
	if (m_vecResourceInfo.empty())
	{
		PResourceInfo pInfo = new ResourceInfo;

		pInfo->pSRV = pSRV;

		if (pInfo->pSRV)
		{
			pInfo->pSRV->AddRef();
		}

		m_vecResourceInfo.push_back(pInfo);

		return;
	}

	m_vecResourceInfo[idx]->pSRV = pSRV;

	if (m_vecResourceInfo[idx]->pSRV)
		m_vecResourceInfo[idx]->pSRV->AddRef();
}

bool CTexture::LoadTexture(const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath)
{
	SetName(strTag);

	PResourceInfo pInfo = new ResourceInfo;

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strRootPath);

	if (pPath)
		lstrcpy(pInfo->pFullPath, pPath);

	lstrcat(pInfo->pFullPath, pFileName);

	lstrcpy(pInfo->pFileName, pFileName);

	strcpy_s(pInfo->pPathName,strRootPath.length() + 1,strRootPath.c_str());

	m_vecResourceInfo.push_back(pInfo);

	TCHAR strExt[_MAX_EXT] = {};

	_wsplitpath_s(pInfo->pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

	char pExt[_MAX_EXT] = {};

	WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

	_strupr_s(pExt);

	pInfo->pImage = new ScratchImage;

	if (strcmp(pExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pInfo->pFullPath, DDS_FLAGS_NONE, nullptr, *pInfo->pImage)))
		{
			SAFE_DELETE(pInfo->pImage);
			return false;
		}

	}

	else if (strcmp(pExt, ".TGA") == 0)
	{
		if (FAILED(DirectX::LoadFromTGAFile(pInfo->pFullPath, nullptr, *pInfo->pImage)))
		{
			SAFE_DELETE(pInfo->pImage);
			return false;
		}
	}

	else
	{
		if (FAILED(DirectX::LoadFromWICFile(pInfo->pFullPath, WIC_FLAGS_NONE, nullptr, *pInfo->pImage)))
		{
			SAFE_DELETE(pInfo->pImage);
			return false;
		}
	}

	return CreateImage();
}

bool CTexture::LoadTextureFromFullPath(const std::string& strTag, const TCHAR* pFullPath)
{
	SetName(strTag);

	PResourceInfo pInfo = new ResourceInfo;

	m_vecResourceInfo.push_back(pInfo);

	lstrcpy(pInfo->pFullPath, pFullPath);

	TCHAR strExt[_MAX_EXT] = {};

	_wsplitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

	char pExt[_MAX_EXT] = {};

	WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

	_strupr_s(pExt);

	pInfo->pImage = new ScratchImage;
	
	if (strcmp(pExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr, *pInfo->pImage)))
		{
			SAFE_DELETE(pInfo->pImage);
			return false;
		}
			
	}
	
	else if (strcmp(pExt, ".TGA") == 0)
	{
		if (FAILED(DirectX::LoadFromTGAFile(pFullPath, nullptr, *pInfo->pImage)))
		{
			SAFE_DELETE(pInfo->pImage);
			return false;
		}
	}

	else
	{
		if (FAILED(DirectX::LoadFromWICFile(pFullPath, WIC_FLAGS_NONE, nullptr, *pInfo->pImage)))
		{
			SAFE_DELETE(pInfo->pImage);
			return false;
		}
	}

	return CreateImage();
}

bool CTexture::LoadTexture(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strPathName)
{
	SetName(strTag);

	size_t iSize = vecFileName.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		PResourceInfo pInfo = new ResourceInfo;

		m_vecResourceInfo.push_back(pInfo);

		lstrcpy(pInfo->pFileName, vecFileName[i]);

		strcpy_s(pInfo->pPathName, strPathName.length()+1, strPathName.c_str());

		const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

		if(pPath)
			lstrcpy(pInfo->pFullPath, pPath);

		lstrcat(pInfo->pFullPath, vecFileName[i]);

		TCHAR strExt[_MAX_EXT] = {};

		_wsplitpath_s(pInfo->pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

		char pExt[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

		_strupr_s(pExt);

		pInfo->pImage = new ScratchImage;

		if (strcmp(pExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pInfo->pFullPath, DDS_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}

		}

		else if (strcmp(pExt, ".TGA") == 0)
		{
			if (FAILED(DirectX::LoadFromTGAFile(pInfo->pFullPath, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(DirectX::LoadFromWICFile(pInfo->pFullPath, WIC_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		bool bResult = CreateImage(int(i));

		if (!bResult)
			return false;
	}

	m_eType = IMAGE_TYPE::IT_ARRAY;

	return true;
}

bool CTexture::LoadTextureFromFullPath(const std::string& strTag, const std::vector<const TCHAR*> vecFullPath)
{
	SetName(strTag);

	size_t iSize = vecFullPath.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		PResourceInfo pInfo = new ResourceInfo;

		m_vecResourceInfo.push_back(pInfo);

		lstrcpy(pInfo->pFullPath, vecFullPath[i]);

		TCHAR strExt[_MAX_EXT] = {};

		_wsplitpath_s(pInfo->pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

		char pExt[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

		_strupr_s(pExt);

		pInfo->pImage = new ScratchImage;

		if (strcmp(pExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pInfo->pFullPath, DDS_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}

		}

		else if (strcmp(pExt, ".TGA") == 0)
		{
			if (FAILED(DirectX::LoadFromTGAFile(pInfo->pFullPath, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(DirectX::LoadFromWICFile(pInfo->pFullPath, WIC_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		bool bResult = CreateImage(int(i));

		if (!bResult)
			return false;
	}

	m_eType = IMAGE_TYPE::IT_ARRAY;

	return true;
}

bool CTexture::LoadTextureArray(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strPathName)
{

	SetName(strTag);

	size_t iSize = vecFileName.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		PResourceInfo pInfo = new ResourceInfo;

		m_vecResourceInfo.push_back(pInfo);

		lstrcpy(pInfo->pFileName, vecFileName[i]);

		strcpy_s(pInfo->pPathName, strPathName.length() + 1, strPathName.c_str());

		const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

		if (pPath)
			lstrcpy(pInfo->pFullPath, pPath);

		lstrcat(pInfo->pFullPath, vecFileName[i]);

		TCHAR strExt[_MAX_EXT] = {};

		_wsplitpath_s(pInfo->pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

		char pExt[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

		_strupr_s(pExt);

		pInfo->pImage = new ScratchImage;

		if (strcmp(pExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pInfo->pFullPath, DDS_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}

		}

		else if (strcmp(pExt, ".TGA") == 0)
		{
			if (FAILED(DirectX::LoadFromTGAFile(pInfo->pFullPath, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(DirectX::LoadFromWICFile(pInfo->pFullPath, WIC_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}
	}

	if (!CreateArrayImage())
		return false;

	m_eType = IMAGE_TYPE::TEXTUREARRAY;

	return true;
}

bool CTexture::LoadTextureArrayFromFullPath(const std::string& strTag, const std::vector<const TCHAR*> vecFullPath)
{
	SetName(strTag);

	size_t iSize = vecFullPath.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		PResourceInfo pInfo = new ResourceInfo;

		m_vecResourceInfo.push_back(pInfo);

		lstrcpy(pInfo->pFullPath, vecFullPath[i]);

		TCHAR strExt[_MAX_EXT] = {};

		_wsplitpath_s(pInfo->pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

		char pExt[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

		_strupr_s(pExt);

		pInfo->pImage = new ScratchImage;

		if (strcmp(pExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pInfo->pFullPath, DDS_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}

		}

		else if (strcmp(pExt, ".TGA") == 0)
		{
			if (FAILED(DirectX::LoadFromTGAFile(pInfo->pFullPath, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(DirectX::LoadFromWICFile(pInfo->pFullPath, WIC_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}
	}

	m_eType = IMAGE_TYPE::TEXTUREARRAY;

	return CreateArrayImage();
}

bool CTexture::CreateImage(int idx)
{
	if (FAILED(CreateShaderResourceView(DEVICE, 
		m_vecResourceInfo[idx]->pImage->GetImages(),
		m_vecResourceInfo[idx]->pImage->GetImageCount(),
		m_vecResourceInfo[idx]->pImage->GetMetadata(),
		&m_vecResourceInfo[idx]->pSRV)))
		return false;

	m_vecResourceInfo[idx]->iWidth = (unsigned int)m_vecResourceInfo[idx]->pImage->GetImages()->width;
	m_vecResourceInfo[idx]->iHeight = (unsigned int)m_vecResourceInfo[idx]->pImage->GetImages()->height;

	return true;
}

bool CTexture::CreateArrayImage()
{
	ScratchImage* pArrayImage = new ScratchImage;

	HRESULT result = pArrayImage->Initialize2D(m_vecResourceInfo[0]->pImage->GetMetadata().format,
		m_vecResourceInfo[0]->pImage->GetMetadata().width,
		m_vecResourceInfo[0]->pImage->GetMetadata().height,
		m_vecResourceInfo.size(),
		m_vecResourceInfo[0]->pImage->GetMetadata().mipLevels);

	int	iMipLevel = (int)m_vecResourceInfo[0]->pImage->GetMetadata().mipLevels;

	// Array Texture의 픽셀정보를 불러온 텍스쳐의 픽셀정보로 채워준다.
	for (size_t i = 0; i < m_vecResourceInfo.size(); ++i)
	{
		const Image* pImages = m_vecResourceInfo[i]->pImage->GetImages();

		// 각각의 텍스쳐들을 밉맵 수준만큼 반복한다.
		for (size_t mip = 0; mip < iMipLevel; ++mip)
		{
			const Image* pSrc = &pArrayImage->GetImages()[i * iMipLevel + mip];
			const Image* pDest = &pImages[mip];

			memcpy(pSrc->pixels, pDest->pixels, pSrc->slicePitch);
		}
	}

	ID3D11Texture2D* pTexture = NULL;

	if (FAILED(DirectX::CreateTextureEx(DEVICE, pArrayImage->GetImages(),
		pArrayImage->GetImageCount(),
		pArrayImage->GetMetadata(),
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0, 0, FALSE,
		(ID3D11Resource * *)& pTexture)))
	{
		assert(false);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC	tViewDesc = {};
	tViewDesc.Format = m_vecResourceInfo[0]->pImage->GetMetadata().format;
	tViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	tViewDesc.Texture2DArray.MostDetailedMip = 0;
	tViewDesc.Texture2DArray.MipLevels = iMipLevel;
	tViewDesc.Texture2DArray.FirstArraySlice = 0;
	tViewDesc.Texture2DArray.ArraySize = (unsigned int)m_vecResourceInfo.size();

	//ID3D11ShaderResourceView* pSRV = nullptr;

	if (FAILED(DEVICE->CreateShaderResourceView(pTexture, &tViewDesc, &m_pArraySRV)))
	{
		assert(false);
		return false;
	}

	//m_vecResourceInfo[0]->pSRV = ID3D11ShaderResourceView*(pSRV, deleter());

	SAFE_DELETE(pArrayImage);

	SAFE_RELEASE(pTexture);

	m_vecResourceInfo[0]->iWidth = (unsigned int)m_vecResourceInfo[0]->pImage->GetImages()->width;
	m_vecResourceInfo[0]->iHeight = (unsigned int)m_vecResourceInfo[0]->pImage->GetImages()->height;

	return true;
}

void CTexture::SetShader(int iRegister, int iShaderType, int idx)
{
	if (m_eType != IMAGE_TYPE::TEXTUREARRAY)
	{
		CDevice::Enter();
		if (iShaderType & (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
			CONTEXT->VSSetShaderResources(iRegister, 1, &m_vecResourceInfo[idx]->pSRV);

		if (iShaderType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
			CONTEXT->PSSetShaderResources(iRegister, 1, &m_vecResourceInfo[idx]->pSRV);

		if (iShaderType & (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL)
			CONTEXT->HSSetShaderResources(iRegister, 1, &m_vecResourceInfo[idx]->pSRV);

		if (iShaderType & (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
			CONTEXT->DSSetShaderResources(iRegister, 1, &m_vecResourceInfo[idx]->pSRV);

		if (iShaderType & (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY)
			CONTEXT->GSSetShaderResources(iRegister, 1, &m_vecResourceInfo[idx]->pSRV);

		if (iShaderType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
			CONTEXT->CSSetShaderResources(iRegister, 1, &m_vecResourceInfo[idx]->pSRV);
		CDevice::Leave();
	}
	else
	{

		CDevice::Enter();
		if (iShaderType & (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
			CONTEXT->VSSetShaderResources(iRegister, 1, &m_pArraySRV);

		if (iShaderType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
			CONTEXT->PSSetShaderResources(iRegister, 1, &m_pArraySRV);

		if (iShaderType & (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL)
			CONTEXT->HSSetShaderResources(iRegister, 1, &m_pArraySRV);

		if (iShaderType & (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
			CONTEXT->DSSetShaderResources(iRegister, 1, &m_pArraySRV);

		if (iShaderType & (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY)
			CONTEXT->GSSetShaderResources(iRegister, 1, &m_pArraySRV);

		if (iShaderType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
			CONTEXT->CSSetShaderResources(iRegister, 1, &m_pArraySRV);
		CDevice::Leave();
	}
}

void CTexture::Save(FILE* pFile)
{
	CRef::Save(pFile);

	int iSize = (int)m_vecResourceInfo.size();

	fwrite(&iSize, 4, 1, pFile);
	fwrite(&m_eType, 4, 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		TCHAR strFullPath[MAX_PATH] = {};

		int iLength = lstrlen(m_vecResourceInfo[i]->pFullPath);

		wcscpy_s(strFullPath, m_vecResourceInfo[i]->pFullPath);

		const TCHAR* pFind = TEXT("Bin\\");

		TCHAR strFind[MAX_PATH] = {};

		for (int j = iLength - lstrlen(pFind); j >=0; j--)
		{
			memset(strFind, 0, MAX_PATH);
			memcpy(strFind, &strFullPath[j], lstrlen(pFind) *sizeof(TCHAR));

			if (wcscmp(strFind, pFind) == 0)
			{
				memset(strFind, 0, MAX_PATH);
				memcpy(strFind, strFullPath +j + lstrlen(pFind), ((size_t)iLength - (size_t)j - lstrlen(pFind)) *sizeof(TCHAR));

				int iLength = (int)lstrlen(strFind);

				fwrite(&iLength, 4, 1, pFile);
				if (iLength > 0)
				{
					fwrite(strFind, sizeof(TCHAR), iLength, pFile);
				}

				break;
			}
		}
	}
}

void CTexture::Load(FILE* pFile)
{
	CRef::Load(pFile);

	int iSize = 0;

	fread(&iSize, 4, 1, pFile);
	fread(&m_eType, 4, 1, pFile);

	switch (m_eType)
	{
	case IMAGE_TYPE::IT_ATLAS:
	case IMAGE_TYPE::IT_ARRAY:
	{
		for (int i = 0; i < iSize; ++i)
		{
			TCHAR strFile[MAX_PATH] = {};

			int iLength = 0;

			fread(&iLength, 4, 1, pFile);

			if (iLength > 0)
			{
				fread(strFile, sizeof(TCHAR), iLength, pFile);
			}

			LoadTexture(m_strName, strFile, ROOT_PATH);
		}
	}
		break;
	case IMAGE_TYPE::TEXTUREARRAY:
	{
		std::vector<const TCHAR*> vecstrTexture;

		for (int i = 0; i < iSize; ++i)
		{

			int iLength = 0;

			fread(&iLength, 4, 1, pFile);
			TCHAR* strFile = new TCHAR[iLength + 1];
			memset(strFile, 0, sizeof(TCHAR) * (iLength + 1));

			if (iLength > 0)
			{
				fread(strFile, sizeof(TCHAR), iLength, pFile);
			}
			vecstrTexture.push_back(strFile);
		}
		LoadTextureArray(m_strName, vecstrTexture, ROOT_PATH);

		SAFE_DELETE_VECLIST(vecstrTexture);
	}
		break;
	default:
		break;
	}

}
