#include "Skeleton.h"
#include "../PathManager.h"
#include "../Device.h"

CSkeleton::CSkeleton()	:
	m_iBoneCount(0)
	, m_pBuffer(nullptr)
	, m_pSRV(nullptr)
{
}

CSkeleton::CSkeleton(const CSkeleton& skel)	:
	CRef(skel)
	, m_iBoneCount(skel.m_iBoneCount)
	, m_pBuffer(nullptr)
	, m_pSRV(nullptr)
{
	size_t iSize = skel.m_vecBones.size();

	m_vecBones.resize(iSize);

	std::vector<Matrix> vecOffset(iSize);

	for (size_t i = 0; i < iSize; i++)
	{
		PBONE pBone = new BONE;

		pBone->strName = skel.m_vecBones[i]->strName;
		pBone->iBoneIndex = skel.m_vecBones[i]->iBoneIndex;
		pBone->iParentIndex = skel.m_vecBones[i]->iParentIndex;
		pBone->matBone = skel.m_vecBones[i]->matBone;
		pBone->matOffset = skel.m_vecBones[i]->matOffset;
		pBone->iDepth = skel.m_vecBones[i]->iDepth;

		m_vecBones[i] = pBone;

		memcpy_s(&vecOffset[i], 64, &pBone->matOffset, 64);
	}

	CreateBuffer(vecOffset);
}

CSkeleton::~CSkeleton()
{
	SAFE_DELETE_VECLIST(m_vecBones);
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pSRV);
}

void CSkeleton::AddBone(const PBONE pBone)
{
	m_vecBones.push_back(pBone);
	++m_iBoneCount;
}

const PBONE CSkeleton::GetBone(int iIdx) const
{
	return m_vecBones[iIdx];
}

const PBONE CSkeleton::GetBone(const std::string& strKey) const
{
	int iIdx = GetBoneIndex(strKey);

	if (iIdx == -1)
		return nullptr;

	return m_vecBones[iIdx];
}

int CSkeleton::GetBoneCount() const
{
	return m_iBoneCount;
}

int CSkeleton::GetBoneIndex(const std::string& strKey) const
{
	int iSize = static_cast<int>(m_vecBones.size());

	for (int i = 0; i < iSize; i++)
	{
		if (m_vecBones[i]->strName == strKey)
		{
			return i;
		}
	}

	return -1;
}

bool CSkeleton::SaveSkeleton(const char* pFilePath, const std::string& strPathKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strPathKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFilePath);

	return SaveSkeletonMultiByte(strFullPath);
}

bool CSkeleton::SaveSkeletonMultiByte(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (pFile)
	{
		Save(pFile);

		fclose(pFile);

		return true;
	}

	return false;
}

void CSkeleton::Save(FILE* pFile)
{
	size_t	iBoneCount = m_vecBones.size();

	fwrite(&iBoneCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iBoneCount; ++i)
	{
		size_t	iNameCount = m_vecBones[i]->strName.size();

		fwrite(&iNameCount, sizeof(size_t), 1, pFile);
		fwrite(m_vecBones[i]->strName.c_str(), 1, iNameCount, pFile);

		fwrite(&m_vecBones[i]->iDepth, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i]->iParentIndex, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i]->matOffset, sizeof(Matrix), 1, pFile);
		fwrite(&m_vecBones[i]->matBone, sizeof(Matrix), 1, pFile);
	}
}

bool CSkeleton::LoadSkeleton(const std::string& strKey, const char* pFilePath, const std::string& strPathKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strPathKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFilePath);

	return LoadSkeletonMultiByte(strKey, strFullPath);
}

bool CSkeleton::LoadSkeletonMultiByte(const std::string& strKey, const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rb");

	if (pFile)
	{
		Load(pFile);

		fclose(pFile);

		return true;
	}

	return false;
}

void CSkeleton::Load(FILE* pFile)
{
	SAFE_DELETE_VECLIST(m_vecBones);

	fread(&m_iBoneCount, sizeof(size_t), 1, pFile);
	m_vecBones.resize(m_iBoneCount);

	std::vector<Matrix> vecOffset(m_iBoneCount);

	for (size_t i = 0; i < m_iBoneCount; ++i)
	{
		PBONE	pBone = new BONE;

		m_vecBones[i] = pBone;

		size_t	iNameCount = 0;

		char	strName[256] = {};
		fread(&iNameCount, sizeof(size_t), 1, pFile);
		fread(strName, 1, iNameCount, pFile);
		pBone->strName = strName;

		fread(&pBone->iDepth, sizeof(int), 1, pFile);
		fread(&pBone->iParentIndex, sizeof(int), 1, pFile);
		fread(&pBone->matOffset, sizeof(Matrix), 1, pFile);
		fread(&pBone->matBone, sizeof(Matrix), 1, pFile);

		vecOffset[i] = pBone->matOffset;
	}

	CreateBuffer(vecOffset);
}

void CSkeleton::AddSkeleton(CSkeleton* pSkeleton)
{
	if (!pSkeleton)
		return;

	size_t iDestSize = m_vecBones.size();

	size_t iSrcSize = pSkeleton->m_vecBones.size();

	if (iSrcSize != iDestSize)
		return;

	for (size_t i = 0; i < iSrcSize; i++)
	{
		bool bIndentity = true;
		for (int m = 0; m < 4; m++)
		{
			for (int n = 0;n < 4; n++)
			{
				if (m == n)
				{
					if (m_vecBones[i]->matOffset[m][n] != 1.f)
					{
						bIndentity = false;
						break;
					}
				}
				else
				{
					if (m_vecBones[i]->matOffset[m][n] != 0.f)
					{
						bIndentity = false;
						break;
					}
				}
			}

			if (!bIndentity)
				break;
		}

		if (bIndentity)
		{
			bool bSrcIndentity = true;
			for (int m = 0; m < 4; m++)
			{
				for (int n = 0; n < 4; n++)
				{
					if (m == n)
					{
						if (pSkeleton->m_vecBones[i]->matOffset[m][n] != 1.f)
						{
							bSrcIndentity = false;
							break;
						}
					}
					else
					{
						if (pSkeleton->m_vecBones[i]->matOffset[m][n] != 0.f)
						{
							bSrcIndentity = false;
							break;
						}
					}
				}

				if (!bSrcIndentity)
					break;
			}

			if (!bSrcIndentity)
			{
				m_vecBones[i]->matOffset = pSkeleton->m_vecBones[i]->matOffset;
			}
		}
	}
}

bool CSkeleton::CreateBuffer(const std::vector<Matrix>& vecOffset)
{
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pSRV);

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.ByteWidth = m_iBoneCount * static_cast<int>(sizeof(Matrix));
	tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	tDesc.StructureByteStride = static_cast<int>(sizeof(Matrix));
	tDesc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA tSub = {};

	tSub.pSysMem = &vecOffset[0];

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tSub, &m_pBuffer)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC tSrv = {};

	tSrv.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSrv.Format = DXGI_FORMAT_UNKNOWN;
	tSrv.BufferEx.FirstElement = 0;
	tSrv.BufferEx.Flags = 0;
	tSrv.BufferEx.NumElements = m_iBoneCount;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pBuffer, &tSrv, &m_pSRV)))
		return false;

	return true;
}

void CSkeleton::SetShaderResource(int iRegister)
{
	CONTEXT->CSSetShaderResources(iRegister, 1, &m_pSRV);
}

void CSkeleton::ResetShaderResource(int iRegister)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->CSSetShaderResources(iRegister, 1, &pSRV);
}

CSkeleton* CSkeleton::Clone()
{
	return new CSkeleton(*this);
}

void CSkeleton::ShowWindow()
{
	CRef::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if(ImGui::Begin(strName.c_str()))
	{
		ImGui::Text("Skeleton");

		int iSize = static_cast<int>(m_vecBones.size());

		std::vector<const char*> vecstrBones(iSize);

		for (int i = 0; i < iSize; i++)
		{
			vecstrBones[i] = m_vecBones[i]->strName.c_str();
		}

		static int iSelect = -1;

		if (!m_vecBones.empty())
		{
			ImGui::ListBox("Bone", &iSelect, &vecstrBones[0], iSize);

			if (iSelect != -1 && iSize > iSelect)
			{
				ImGui::Text("Bone");
				ImGui::InputFloat4("MatBone row 0", &m_vecBones[iSelect]->matBone[0].x);
				ImGui::InputFloat4("MatBone row 1", &m_vecBones[iSelect]->matBone[1].x);
				ImGui::InputFloat4("MatBone row 2", &m_vecBones[iSelect]->matBone[2].x);
				ImGui::InputFloat4("MatBone row 3", &m_vecBones[iSelect]->matBone[3].x);
				ImGui::InputFloat4("MatOffset row 0", &m_vecBones[iSelect]->matOffset[0].x);
				ImGui::InputFloat4("MatOffset row 1", &m_vecBones[iSelect]->matOffset[1].x);
				ImGui::InputFloat4("MatOffset row 2", &m_vecBones[iSelect]->matOffset[2].x);
				ImGui::InputFloat4("MatOffset row 3", &m_vecBones[iSelect]->matOffset[3].x);
				ImGui::InputInt("Parent Index", &m_vecBones[iSelect]->iParentIndex);
				ImGui::InputInt("Bone Index", &m_vecBones[iSelect]->iBoneIndex);
				ImGui::InputInt("Depth", &m_vecBones[iSelect]->iDepth);
			}
		}
	}
	ImGui::End();
}
