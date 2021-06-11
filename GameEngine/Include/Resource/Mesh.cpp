#include "Mesh.h"
#include "../Resource/Material.h"
#include "ResourceManager.h"
#include "../Device.h"
#include "../PathManager.h"
#include "FBXLoader.h"
#include "ShaderManager.h"
#include "Skeleton.h"
#include "AnimationSequence.h"

CMesh::CMesh()	:
	m_eMeshType(MESH_TYPE::MT_MESH2D),
	m_pInstBuffer(nullptr),
	m_iCount(0)
{
	m_vMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	m_vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
}

CMesh::~CMesh()
{
	if (m_pInstBuffer)
	{
		SAFE_DELETE_ARRAY(m_pInstBuffer->pData);
		delete m_pInstBuffer;
	}

	size_t iSize = m_vecMeshContainer.size();

	for (size_t i = 0; i < iSize; i++)
	{
		SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->tVtx.pData);
		SAFE_RELEASE(m_vecMeshContainer[i]->tVtx.pBuffer);
		size_t iIBSize = m_vecMeshContainer[i]->vecIdx.size();

		for (size_t j = 0; j < iIBSize; j++)
		{
			SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->vecIdx[j].pData);
			SAFE_RELEASE(m_vecMeshContainer[i]->vecIdx[j].pBuffer);
		}

		SAFE_DELETE(m_vecMeshContainer[i]);
	}

	m_vecMeshContainer.clear();
}

MESH_TYPE CMesh::GetMeshType() const
{
	return m_eMeshType;
}

void CMesh::SetMaterial(std::shared_ptr<class CMaterial> pMaterial, int iContainer , int iSubset )
{
	m_vecMeshContainer[iContainer]->vecMaterial[iSubset] = pMaterial;
}

std::shared_ptr<class CMaterial> CMesh::GetMaterial(int iContainer, int iSubset) const
{
	return m_vecMeshContainer[iContainer]->vecMaterial[iSubset];
}

int CMesh::GetInstCount() const
{
	return m_iCount;
}

const std::vector<PMESHCONTAINER>* CMesh::GetMeshContainer() const
{
	return &m_vecMeshContainer;
}

const std::vector<VERTEX3D>* CMesh::GetVecVtxCopy() const
{
	return &m_vecVtx;
}

const std::vector<UINT>* CMesh::GetVecIdxCopy() const
{
	return &m_vecIdx;
}

void CMesh::SetShader(const std::string& strKey, int iContainer, int iSubset)
{
	if (m_vecMeshContainer.size() <= iContainer)
		return;

	else if (m_vecMeshContainer[iContainer]->vecShader.size() <= iSubset)
		return;

	m_vecMeshContainer[iContainer]->vecShader[iSubset] = GET_SINGLE(CShaderManager)->FindShader(strKey);
}

bool CMesh::Init()
{
	return true;
}

void CMesh::Render(float fTime, int iContainer, int iSubset)
{
	UINT iStride = m_vecMeshContainer[iContainer]->tVtx.iSize;
	UINT iOffset = 0;

	CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[iContainer]->eTopology);
	CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[iContainer]->tVtx.pBuffer, &iStride, &iOffset);

	size_t iSubsetSize = m_vecMeshContainer[iContainer]->vecIdx.size();

	if (m_vecMeshContainer[iContainer]->vecIdx.size())
	{
		CONTEXT->IASetIndexBuffer(m_vecMeshContainer[iContainer]->vecIdx[iSubset].pBuffer, m_vecMeshContainer[iContainer]->vecIdx[iSubset].eFmt, iOffset);
		CONTEXT->DrawIndexed(m_vecMeshContainer[iContainer]->vecIdx[iSubset].iCount, 0, 0);
	}

	else
	{
		CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->Draw(m_vecMeshContainer[iContainer]->tVtx.iCount, 0);
	}
}

void CMesh::RenderIndex(float fTime, int iCount, int iStart, int iContainer, int iSubset)
{
	UINT iSize = m_vecMeshContainer[iContainer]->tVtx.iSize;
	UINT iOffset = 0;

	CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[iContainer]->eTopology);
	CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[iContainer]->tVtx.pBuffer, &iSize, &iOffset);

	if (m_vecMeshContainer[iContainer]->vecIdx[iSubset].pBuffer)
	{
		CONTEXT->IASetIndexBuffer(m_vecMeshContainer[iContainer]->vecIdx[iSubset].pBuffer, m_vecMeshContainer[iContainer]->vecIdx[iSubset].eFmt, iOffset);
		CONTEXT->DrawIndexed(iCount, iStart, 0);
	}
	else
	{
		CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->Draw(iCount, iStart);
	}
}

void CMesh::RenderInstancing(InstancingBuffer* pIBuffer, int iCount, int iSize, float fTime)
{
	size_t _iSize = m_vecMeshContainer.size();
	for (size_t i = 0; i < _iSize; i++)
	{
		size_t iIdxSize = m_vecMeshContainer[i]->vecIdx.size();

		for (size_t j = 0; j < iIdxSize; j++)
		{
			UINT iStride[2] = { (UINT)m_vecMeshContainer[i]->tVtx.iSize,(UINT)iSize };
			UINT iOffset[2] = { 0,0 };

			ID3D11Buffer* vbs[2] = { m_vecMeshContainer[i]->tVtx.pBuffer, pIBuffer->pBuffer };

			CONTEXT->IASetVertexBuffers(0, 2, vbs, iStride, iOffset);
			CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[i]->eTopology);

			if (m_vecMeshContainer[i]->vecIdx[j].pBuffer)
			{
				CONTEXT->IASetIndexBuffer(m_vecMeshContainer[i]->vecIdx[j].pBuffer, m_vecMeshContainer[i]->vecIdx[j].eFmt, 0);
				CONTEXT->DrawIndexedInstanced(m_vecMeshContainer[i]->vecIdx[j].iCount, iCount, 0, 0, 0);
			}

			else
			{
				CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
				CONTEXT->DrawInstanced(m_vecMeshContainer[i]->tVtx.iCount, iCount, 0, 0);
			}
		}
	}
}

void CMesh::RenderInstancing(InstancingBuffer* pInstBuffer, int iCount, float fTime, int iContainer, int iSubset)
{
	UINT iStride[2] = { (UINT)m_vecMeshContainer[iContainer]->tVtx.iSize,(UINT)pInstBuffer->iSize };
	UINT iOffset[2] = { 0,0 };


	ID3D11Buffer* vbs[2] = { m_vecMeshContainer[iContainer]->tVtx.pBuffer, pInstBuffer->pBuffer };

	CONTEXT->IASetVertexBuffers(0, 2, vbs, iStride, iOffset);
	CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[iContainer]->eTopology);

	if (m_vecMeshContainer[iContainer]->vecIdx[iSubset].pBuffer)
	{
		CONTEXT->IASetIndexBuffer(m_vecMeshContainer[iContainer]->vecIdx[iSubset].pBuffer, m_vecMeshContainer[iContainer]->vecIdx[iSubset].eFmt, 0);
		CONTEXT->DrawIndexedInstanced(m_vecMeshContainer[iContainer]->vecIdx[iSubset].iCount, iCount, 0, 0, 0);
	}

	else
	{
		CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->DrawInstanced(m_vecMeshContainer[iContainer]->tVtx.iCount, iCount, 0, 0);
	}
}

void CMesh::RenderParticle(int iCount, float fTime)
{
}

bool CMesh::CreateMesh(D3D11_PRIMITIVE_TOPOLOGY eTopo, int iVtxCount, int iVtxSize,
	D3D11_USAGE eVtxUsage, void* pVtxData, int iIdxCount, 
	int iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, void* pIdxData, D3D11_CPU_ACCESS_FLAG eCpuFlag, D3D11_CPU_ACCESS_FLAG eICpuFlag)
{
	size_t iSize = m_vecMeshContainer.size();

	for (size_t i = 0; i < iSize; i++)
	{
		SAFE_DELETE_ARRAY( m_vecMeshContainer[i]->tVtx.pData);
		SAFE_RELEASE(m_vecMeshContainer[i]->tVtx.pBuffer);

		size_t iIdxSize = m_vecMeshContainer[i]->vecIdx.size();

		for (size_t j = 0; j < iIdxSize; j++)
		{
			SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->vecIdx[j].pData);
			SAFE_RELEASE(m_vecMeshContainer[i]->vecIdx[j].pBuffer);
		}
	}

	m_vecMeshContainer.clear();

	PMESHCONTAINER pContainer = new MESHCONTAINER;
	m_vecMeshContainer.push_back(pContainer);
	pContainer->eTopology = eTopo;

	pContainer->tVtx.iCount = iVtxCount;
	pContainer->tVtx.iSize = iVtxSize;
	pContainer->tVtx.iUsage = eVtxUsage;
	int iData = iVtxSize * iVtxCount;
	pContainer->tVtx.pData = new char[iData];
	memcpy_s(pContainer->tVtx.pData, iData, pVtxData, iData);

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.Usage = eVtxUsage;
	tDesc.ByteWidth = pContainer->tVtx.iCount * pContainer->tVtx.iSize;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	
	tDesc.CPUAccessFlags = eCpuFlag;

	D3D11_SUBRESOURCE_DATA tData = {};

	tData.pSysMem = pVtxData;

	m_vMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	m_vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

	char* pVtx = (char*)pVtxData;

	for (int i = 0; i < iVtxCount; ++i)
	{
		Vector3* pPos = (Vector3*)pVtx;

		if (pPos)
		{
			if (pPos->x > m_vMax.x)
				m_vMax.x = pPos->x;

			if (pPos->y > m_vMax.y)
				m_vMax.y = pPos->y;

			if (pPos->z > m_vMax.z)
				m_vMax.z = pPos->z;

			if (pPos->x < m_vMin.x)
				m_vMin.x = pPos->x;

			if (pPos->y < m_vMin.y)
				m_vMin.y = pPos->y;

			if (pPos->z < m_vMin.z)
				m_vMin.z = pPos->z;
		}

		pVtx += iVtxSize;
	}

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &pContainer->tVtx.pBuffer)))
		return false;

	if (pIdxData)
	{
		Index tIdx = {};
		tIdx.iCount = iIdxCount;
		tIdx.iSize = iIdxSize;
		tIdx.eUsage = eIdxUsage;
		tIdx.eFmt = eFmt;

		int iSize = iIdxSize * iIdxCount;

		tIdx.pData = new char[iSize];
		memcpy_s(tIdx.pData, iSize, pIdxData, iSize);

		D3D11_BUFFER_DESC tIndexDesc = {};

		tIndexDesc.Usage = eIdxUsage;
		tIndexDesc.ByteWidth = tIdx.iCount * tIdx.iSize;
		tIndexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		tIndexDesc.CPUAccessFlags = eICpuFlag;

		D3D11_SUBRESOURCE_DATA tIndexData = {};

		tIndexData.pSysMem = pIdxData;

		if (FAILED(DEVICE->CreateBuffer(&tIndexDesc, &tIndexData, &tIdx.pBuffer)))
			return false;

		pContainer->vecIdx.push_back(tIdx);
	}

	pContainer->vecMaterial.resize(1);
	pContainer->vecShader.resize(1);

	return true;
}

bool CMesh::CreateInstBuffer(int iSize, int iCount)
{
	if (m_pInstBuffer)
	{
		SAFE_DELETE_ARRAY(m_pInstBuffer->pData);
		delete m_pInstBuffer;
	}

	m_pInstBuffer = new Vertex;

	m_pInstBuffer->pData = new char[(size_t)iSize * (size_t)iCount];
	m_pInstBuffer->iUsage = D3D11_USAGE_DYNAMIC;
	m_pInstBuffer->iSize = iSize;
	m_pInstBuffer->iCount = iCount;
	
	D3D11_BUFFER_DESC tDesc = {};

	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tDesc.ByteWidth = iSize * iCount;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &m_pInstBuffer->pBuffer)))
		return false;

	return true;
}

void CMesh::AddInstanceData(const PINSTANCINGDATA pData)
{
	memcpy(((PINSTANCINGDATA)m_pInstBuffer->pData) + m_iCount++, pData, m_pInstBuffer->iSize);
}

void CMesh::ChangeMeshBuffer(const std::vector<VERTEX3D>& vecVtx)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};

	int iSize = m_vecMeshContainer[0]->tVtx.iSize * m_vecMeshContainer[0]->tVtx.iCount;
	CONTEXT->Map(m_vecMeshContainer[0]->tVtx.pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

	memcpy_s(tSub.pData, iSize, &vecVtx[0], sizeof(vecVtx[0]) * vecVtx.size());

	CONTEXT->Unmap(m_vecMeshContainer[0]->tVtx.pBuffer, 0);

}

void CMesh::ChangeMeshBuffer(void* pData, int iSize)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_vecMeshContainer[0]->tVtx.pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

	memcpy_s(tSub.pData, m_vecMeshContainer[0]->tVtx.iSize * m_vecMeshContainer[0]->tVtx.iCount,
		pData, iSize);

	CONTEXT->Unmap(m_vecMeshContainer[0]->tVtx.pBuffer, 0);
}

bool CMesh::ConvertFBX(CFBXLoader* pLoader, const char* pFullPath, bool bLoop, int iIndex)
{
	const std::vector<PFBXMESHCONTAINER>* pvecConatainer = pLoader->GetMeshContainers();
	const std::vector<std::vector<PFBXMATERIAL>>* pvecMaterial = pLoader->GetMaterials();

	std::vector<PFBXMESHCONTAINER>::const_iterator iter;
	std::vector<PFBXMESHCONTAINER>::const_iterator iterEnd = pvecConatainer->end();

	int iContainer = 0;
	std::vector<std::vector<bool>>	vecvecEmpty;

	bool bAnimation = false;
	bool bBump = false;

	for (iter = pvecConatainer->begin(); iter != iterEnd; iter++, iContainer++)
	{
		std::vector<VERTEX3D>	vecVertex;
		std::vector<bool>		vecEmpty;

		PMESHCONTAINER pContainer = new MESHCONTAINER;

		m_vecMeshContainer.push_back(pContainer);

		vecvecEmpty.push_back(vecEmpty);

		bAnimation = (*iter)->bAnimation;
		bBump = (*iter)->bBump;

		size_t iSize = (*iter)->vecPos.size();

		for (size_t i = 0; i < iSize; i++)
		{
			VERTEX3D tVtx = {};

			tVtx.vPos = (*iter)->vecPos[i];
			tVtx.vUV = (*iter)->vecUV[i];

			if (bBump)
			{
				tVtx.vNormal = (*iter)->vecNormal[i];
				tVtx.vTangent = (*iter)->vecTangent[i];
				tVtx.vBinormal = (*iter)->vecBinormal[i];
			}

			if (!(*iter)->vecBlendWeight.empty() && bAnimation)
			{
				tVtx.vWeight = (*iter)->vecBlendWeight[i];

				tVtx.vWeightFactor = (*iter)->vecBlendIndex[i];
			}

			vecVertex.push_back(tVtx);
		}

		if (!AddVertexBuffer(&vecVertex[0], sizeof(VERTEX3D), (int)vecVertex.size(), D3D11_USAGE_DEFAULT,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pContainer))
		{
			return false;
		}

		iSize = (*iter)->vecIndices.size();

		for (size_t i = 0; i < iSize; i++)
		{
			if ((*iter)->vecIndices[i].empty())
			{
				vecvecEmpty[iContainer].push_back(false);
				continue;
			}

			vecvecEmpty[iContainer].push_back(true);

			int iIdxSize = (int)(*iter)->vecIndices[i].size();
			std::vector<unsigned int> vecDebugIdx(iIdxSize);
			std::vector<Vector2>	vecDebugVtx(iIdxSize);
			//4938 ,18
			if (iIndex != -1)
			{
				if (!AddIndexBuffer(&(*iter)->vecIndices[i][0], 4, iIndex, D3D11_USAGE_DEFAULT,
					DXGI_FORMAT_R32_UINT, pContainer))
				{
					return false;
				}

				/*GET_SINGLE(CResourceManager)->CreateMesh("test", MESH_TYPE::MT_ANIMATION_MESH, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
					static_cast<int>(vecVertex.size()), static_cast<int>(sizeof(VERTEX3D)), D3D11_USAGE_IMMUTABLE, &vecVertex[0],
					1584, 4, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT, &(*iter)->vecIndices[i][0]);

				class CMesh* pMesh = GET_SINGLE(CResourceManager)->FindMesh("test");
				pMesh->SetMaterial(GET_SINGLE(CResourceManager)->FindMaterial("Color"));
				pMesh->SetShader(STANDARD3D_SHADER);*/

				if (!AddIndexBuffer(&(*iter)->vecIndices[i][iIndex], 4, iIdxSize - iIndex, D3D11_USAGE_DEFAULT,
					DXGI_FORMAT_R32_UINT, pContainer))
				{
					return false;
				}

				/*if (!AddIndexBuffer(&(*iter)->vecIndices[i][2115], 4, 972, D3D11_USAGE_DEFAULT,
					DXGI_FORMAT_R32_UINT, pContainer))
				{
					return false;
				}*/

				pContainer->vecMaterial.push_back(std::shared_ptr<CMaterial>(new CMaterial));
				pContainer->vecShader.push_back(GET_SINGLE(CShaderManager)->FindShader(STANDARD3D_SHADER));

				//pContainer->vecMaterial.push_back(std::shared_ptr<CMaterial>(new CMaterial));
				//pContainer->vecShader.push_back(GET_SINGLE(CShaderManager)->FindShader(STANDARD3D_SHADER));
			}
			else
			{
				if (!AddIndexBuffer(&(*iter)->vecIndices[i][0], 4, iIdxSize, D3D11_USAGE_DEFAULT,
					DXGI_FORMAT_R32_UINT, pContainer))
				{
					return false;
				}

				memcpy_s(&vecDebugIdx[0], iIdxSize * 4, &(*iter)->vecIndices[i][0], iIdxSize * 4);

				for (int i = 0; i < iIdxSize; i++)
				{
					
					vecDebugVtx[i] = (*iter)->vecUV[vecDebugIdx[i]];

					if (vecDebugVtx[i].x > 1.f || vecDebugVtx[i].y > 1.f)
					{
						int i = 0;
					}
				}
			}
		}
	}

	std::vector<std::vector<PFBXMATERIAL>>::const_iterator iterM = pvecMaterial->begin();
	std::vector<std::vector<PFBXMATERIAL>>::const_iterator iterMEnd = pvecMaterial->end();

	iContainer = 0;
	for (; iterM != iterMEnd; ++iterM, ++iContainer)
	{
		size_t iSize = (*iterM).size();

		for (size_t i = 0; i < iSize; i++)
		{
			if (!vecvecEmpty[iContainer][i])
				continue;

			PFBXMATERIAL pMaterial = (*iterM)[i];

			std::shared_ptr<class CMaterial> pMtrl = std::shared_ptr<class CMaterial>(new CMaterial);

			pMtrl->SetDiffuseColor(pMaterial->vDif);
			pMtrl->SetAmbientColor(pMaterial->vAmb);
			pMtrl->SetSpecularColor(pMaterial->vSpc);
			pMtrl->SetEmissiveColor(pMaterial->vEmv);
			pMtrl->SetSpecularPower(pMaterial->fShininess);

			std::shared_ptr<class CShader> pShader = GET_SINGLE(CShaderManager)->FindShader("Standard3D");

			char strName[_MAX_FNAME] = {};

			_splitpath_s(pMaterial->strDifTex.c_str(), nullptr, 0, nullptr, 0, strName, _MAX_FNAME, nullptr, 0);

			TCHAR strPath[MAX_PATH] = {};

#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, pMaterial->strDifTex.c_str(), -1, strPath, (int)pMaterial->strDifTex.length());
#else
			strcpy_s(strPath, pMaterial->strDifTex.c_str());
#endif			
			pMtrl->SetTexture(LINK_TYPE::LT_DIF, strName, strPath, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, 0);

			if (!pMaterial->strBumpTex.empty())
			{
				memset(strName, 0, _MAX_FNAME);
				memset(strPath, 0, MAX_PATH * sizeof(TCHAR));

				_splitpath_s(pMaterial->strBumpTex.c_str(), nullptr, 0, nullptr, 0, strName, _MAX_FNAME, nullptr, 0);

#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, pMaterial->strBumpTex.c_str(), -1, strPath, (int)pMaterial->strBumpTex.length());
#else
				strcpy_s(strPath, pMaterial->strNorTex.c_str());
#endif
				pMtrl->SetTexture(LINK_TYPE::LT_NOR, strName, strPath, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
			}

			if (!pMaterial->strSpcTex.empty())
			{
				memset(strName, 0, _MAX_FNAME);
				memset(strPath, 0, MAX_PATH * sizeof(TCHAR));

				_splitpath_s(pMaterial->strSpcTex.c_str(), nullptr, 0, nullptr, 0, strName, _MAX_FNAME, nullptr, 0);

#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, pMaterial->strSpcTex.c_str(), -1, strPath, (int)pMaterial->strSpcTex.length());
#else
				strcpy_s(strPath, pMaterial->strSpeTex.c_str());
#endif
				pMtrl->SetTexture(LINK_TYPE::LT_SPE, strName, strPath, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
			}

			if (!pMaterial->strBumpTex.empty())
			{
				memset(strName, 0, _MAX_FNAME);
				memset(strPath, 0, MAX_PATH * sizeof(TCHAR));

				_splitpath_s(pMaterial->strSpcTex.c_str(), nullptr, 0, nullptr, 0, strName, _MAX_FNAME, nullptr, 0);

#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, pMaterial->strSpcTex.c_str(), -1, strPath, (int)pMaterial->strSpcTex.length());
#else
				strcpy_s(strPath, pMaterial->strSpeTex.c_str());
#endif
				pMtrl->SetTexture(LINK_TYPE::LT_SPE, strName, strPath, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
			}

			if (!pMaterial->strEmvTex.empty())
			{
				memset(strName, 0, _MAX_FNAME);
				memset(strPath, 0, MAX_PATH * sizeof(TCHAR));

				_splitpath_s(pMaterial->strEmvTex.c_str(), nullptr, 0, nullptr, 0, strName, _MAX_FNAME, nullptr, 0);

#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, pMaterial->strEmvTex.c_str(), -1, strPath, (int)pMaterial->strEmvTex.length());
#else
				strcpy_s(strPath, pMaterial->strEmvTex.c_str());
#endif
				pMtrl->SetTexture(LINK_TYPE::LT_EMI, strName, strPath, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
			}

			if (bAnimation)
				pMtrl->EnableAnimation();

			if (bBump)
				pMtrl->EnableBump();

			m_vecMeshContainer[iContainer]->vecMaterial.push_back(pMtrl);
			m_vecMeshContainer[iContainer]->vecShader.push_back(pShader);

			/*m_vecMeshContainer[iContainer]->vecMaterial.push_back(std::shared_ptr<CMaterial>(new CMaterial));
			m_vecMeshContainer[iContainer]->vecShader.push_back(GET_SINGLE(CShaderManager)->FindShader(STANDARD3D_SHADER));
			m_vecMeshContainer[iContainer]->vecMaterial.push_back(std::shared_ptr<CMaterial>(new CMaterial));
			m_vecMeshContainer[iContainer]->vecShader.push_back(GET_SINGLE(CShaderManager)->FindShader(STANDARD3D_SHADER));*/
		}
	}

	char strPath[MAX_PATH] = {};
	int iLength = (int)strlen(pFullPath);
	strcpy_s(strPath, pFullPath);
	memcpy(&strPath[iLength - 3], "msh", 3);

	SaveMesh(strPath);
	
	// 애니메이션 처리
	const std::vector<PFBXBONE>* pvecBone = pLoader->GetBones();

	if (pvecBone->empty())
		return true;

	std::string	strSkeletonName = m_strName + "_Skeleton";

	CSkeleton* pSkeleton = new CSkeleton;

	pSkeleton->SetName(strSkeletonName);

	// 본 수만큼 반복한다.
	std::vector<PFBXBONE>::const_iterator	iterB;
	std::vector<PFBXBONE>::const_iterator	iterBEnd = pvecBone->end();

	for (iterB = pvecBone->begin(); iterB != iterBEnd; ++iterB)
	{
		if (!*iterB)
		{
			pSkeleton->AddBone(nullptr);
			continue;
		}

		PBONE	pBone = new BONE;

		pBone->strName = (*iterB)->strName;
		pBone->iDepth = (*iterB)->iDepth;
		pBone->iParentIndex = (*iterB)->iParentIndex;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				float f = static_cast<float>((*iterB)->matOffset.mData[i].mData[j]);
				pBone->matOffset[i][j] = f;
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				pBone->matBone[i][j] = static_cast<float>((*iterB)->matBone.mData[i].mData[j]);
			}
		}

		pSkeleton->AddBone(pBone);
	}

	//pSkeleton->CreateBoneTexture();

	GET_SINGLE(CResourceManager)->SetSkeleton(strSkeletonName, pSkeleton);

	// 애니메이션 클립을 추가한다.
	const std::vector<PFBXANIMATIONCLIP>* pvecClip = pLoader->GetClips();

	// 파일 이름을 제외한 경로를 만들어둔다.
	char	strAnimPath[MAX_PATH] = {};
	strcpy_s(strAnimPath, pFullPath);

	iLength = (int)strlen(strAnimPath);
	for (int i = iLength - 1; i >= 0; --i)
	{
		// aa/bb.exe 9개, 2번인덱스 3 ~ 8번까지 제거
		if (strAnimPath[i] == '/' || strAnimPath[i] == '\\')
		{
			memset(&strAnimPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
			break;
		}
	}

	// 클립을 읽어온다.
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterC;
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterCEnd = pvecClip->end();

	for (iterC = pvecClip->begin(); iterC != iterCEnd; ++iterC)
	{
		GET_SINGLE(CResourceManager)->LoadAnimationSequence(
			(*iterC)->strName, bLoop, *iterC);

		CAnimationSequence* pSequence = GET_SINGLE(CResourceManager)->FindSequence((*iterC)->strName);

		if (!pSequence)
			continue;

		char	strAnimFullPath[MAX_PATH] = {};
		strcpy_s(strAnimFullPath, strAnimPath);
		strcat_s(strAnimFullPath, (*iterC)->strName.c_str());
		strcat_s(strAnimFullPath, ".sqc");

		pSequence->SaveFullPath(strAnimFullPath);
	}

	if (pSkeleton)
	{
		memcpy(const_cast<char*>(&pFullPath[iLength - 3]), "bne", 3);
		pSkeleton->SaveSkeletonMultiByte(pFullPath);
	}

	return true;
}

bool CMesh::LoadMesh(const char* pFilePath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFilePath, "rb");

	if (pFile)
	{
		size_t iLength = 0;

		fread(&iLength, sizeof(size_t), 1, pFile);
		if (iLength > 0)
		{
			char strName[MAX_PATH] = {};
			fread(strName, 1, iLength, pFile);
			m_strName = strName;
		}

		fread(&m_vMax, 12, 1, pFile);
		fread(&m_vMin, 12, 1, pFile);
		fread(&m_eMeshType, 4, 1, pFile);

		size_t iSize = m_vecMeshContainer.size();

		for (size_t i = 0; i < iSize; i++)
		{
			SAFE_DELETE_ARRAY( m_vecMeshContainer[i]->tVtx.pData);
			size_t iIBSize = m_vecMeshContainer[i]->vecIdx.size();

			for (size_t j = 0; j < iIBSize; j++)
			{
				SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->vecIdx[j].pData);
			}

			//SAFE_RELEASE_VECLIST(m_vecMeshContainer[i]->vecMaterial);
			SAFE_DELETE(m_vecMeshContainer[i]);
		}

		m_vecMeshContainer.clear();

		iSize = 0;

		fread(&iSize, sizeof(size_t), 1, pFile);

		for (size_t i = 0; i < iSize; i++)
		{
			PMESHCONTAINER pContainer = new MESHCONTAINER;

			Vertex& tVtx = pContainer->tVtx;

			fread(&pContainer->eTopology, 4, 1, pFile);
			fread(&tVtx.iCount, 4, 1, pFile);
			fread(&tVtx.iSize, 4, 1, pFile);
			fread(&tVtx.iUsage, 4, 1, pFile);

			bool bVtxData = false;

			fread(&bVtxData, 1, 1, pFile);

			if (bVtxData)
			{
				if (tVtx.iSize * tVtx.iCount > 0)
				{
					tVtx.pData = new char[(size_t)tVtx.iCount * (size_t)tVtx.iSize];
					fread(tVtx.pData, tVtx.iSize, tVtx.iCount, pFile);
				}
			}

			m_vecVtx.resize(tVtx.iCount);

			memcpy_s(&m_vecVtx[0], tVtx.iSize * tVtx.iCount, tVtx.pData, tVtx.iSize * tVtx.iCount);

			D3D11_BUFFER_DESC tDesc = {};

			tDesc.Usage = tVtx.iUsage;
			tDesc.ByteWidth = tVtx.iCount * tVtx.iSize;
			tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA tData = {};

			tData.pSysMem = tVtx.pData;

			if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &tVtx.pBuffer)))
				return false;

			size_t iIdxSize = 0;

			fread(&iIdxSize, sizeof(size_t), 1, pFile);

			for (size_t j = 0; j < iIdxSize; j++)
			{
				Index tIdx = {};

				fread(&tIdx.eFmt, 4, 1, pFile);
				fread(&tIdx.eUsage, 4, 1, pFile);
				fread(&tIdx.iCount, 4, 1, pFile);
				fread(&tIdx.iSize, 4, 1, pFile);
				bool bIdxData = false;

				fread(&bIdxData, 1, 1, pFile);
				if (bIdxData)
				{
					if (tIdx.iCount * tIdx.iSize > 0)
					{
						tIdx.pData = new char[(size_t)tIdx.iCount * (size_t)tIdx.iSize];
						fread(tIdx.pData,
							tIdx.iSize,
							tIdx.iCount, pFile);
					}
				}

				if (j == 0)
				{
					m_vecIdx.resize(tIdx.iCount);
					memcpy_s(&m_vecIdx[0], (size_t)tIdx.iCount* (size_t)tIdx.iSize, tIdx.pData, (size_t)tIdx.iCount* (size_t)tIdx.iSize);
				}

				D3D11_BUFFER_DESC tIndexDesc = {};

				tIndexDesc.Usage = tIdx.eUsage;
				tIndexDesc.ByteWidth = tIdx.iCount * tIdx.iSize;
				tIndexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

				D3D11_SUBRESOURCE_DATA tIndexData = {};

				tIndexData.pSysMem = tIdx.pData;

				if (FAILED(DEVICE->CreateBuffer(&tIndexDesc, &tIndexData, &tIdx.pBuffer)))
					return false;

				pContainer->vecIdx.push_back(tIdx);

				std::shared_ptr<class CMaterial> pMtrl = std::shared_ptr<class CMaterial>(new CMaterial);

				pMtrl->Load(pFile);

				pContainer->vecMaterial.push_back(pMtrl);

				int iLength = 0;
				fread(&iLength, 4, 1, pFile);

				if (iLength > 0)
				{
					char strShader[MAX_PATH] = {};
					fread(strShader, 1, iLength, pFile);

					pContainer->vecShader.push_back(GET_SINGLE(CShaderManager)->FindShader(strShader));
				}
			}

			m_vecMeshContainer.push_back(pContainer);
		}

		fclose(pFile);

		return true;
	}

	return false;
}

bool CMesh::SaveMesh(const char* pFilePath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFilePath, "wb");

	if (pFile)
	{
		size_t iLength = m_strName.length();

		fwrite(&iLength, sizeof(size_t), 1, pFile);
		if (iLength > 0)
		{
			fwrite(m_strName.c_str(), 1, iLength, pFile);
		}

		fwrite(&m_vMax, 12, 1, pFile);
		fwrite(&m_vMin, 12, 1, pFile);
		fwrite(&m_eMeshType, 4, 1, pFile);

		size_t iSize = m_vecMeshContainer.size();

		fwrite(&iSize, sizeof(size_t), 1, pFile);

		for (size_t i = 0; i < iSize; i++)
		{
			PMESHCONTAINER pContainer = m_vecMeshContainer[i];

			Vertex tVtx = pContainer->tVtx;

			fwrite(&pContainer->eTopology, 4, 1, pFile);
			fwrite(&tVtx.iCount, 4, 1, pFile);
			fwrite(&tVtx.iSize, 4, 1, pFile);
			fwrite(&tVtx.iUsage, 4, 1, pFile);

			//std::vector<VERTEX3D>	vec(tVtx.iCount);

			//memcpy_s(&vec[0], tVtx.iCount * tVtx.iSize, tVtx.pData, tVtx.iCount * tVtx.iSize);
			bool bVtxData = false;

			if (tVtx.pData)
			{
				bVtxData = true;
			}

			fwrite(&bVtxData, 1, 1, pFile);

			if (tVtx.pData)
			{
				fwrite(tVtx.pData, tVtx.iSize, tVtx.iCount, pFile);
			}

			size_t iIdxSize = m_vecMeshContainer[i]->vecIdx.size();

			fwrite(&iIdxSize, sizeof(size_t), 1, pFile);

			for (size_t j = 0; j < iIdxSize; j++)
			{
				Index tIdx = pContainer->vecIdx[j];

				fwrite(&tIdx.eFmt, 4, 1, pFile);
				fwrite(&tIdx.eUsage, 4, 1, pFile);
				fwrite(&tIdx.iCount, 4, 1, pFile);
				fwrite(&tIdx.iSize, 4, 1, pFile);

				bool bIdxData = false;

				if (tIdx.pData)
				{
					bIdxData = true;
				}

				fwrite(&bIdxData, 1, 1, pFile);

				if (tIdx.pData)
				{
					fwrite(tIdx.pData,
						tIdx.iSize,
						tIdx.iCount, pFile);
				}

				pContainer->vecMaterial[j]->Save(pFile);

				std::string strShader = pContainer->vecShader[j]->GetName();

				int iLength = (int)strShader.length();

				fwrite(&iLength, 4, 1, pFile);
				if (iLength > 0)
				{
					fwrite(strShader.c_str(), 1, iLength, pFile);
				}
			}
		}

		fclose(pFile);

		return true;
	}

	return false;
}

bool CMesh::AddVertexBuffer(void* _pVtx, int iSize, int iCount, D3D11_USAGE eUsg, D3D11_PRIMITIVE_TOPOLOGY eTopo, PMESHCONTAINER pContainer)
{
	pContainer->eTopology = eTopo;

	pContainer->tVtx.iCount = iCount;
	pContainer->tVtx.iSize = iSize;
	pContainer->tVtx.iUsage = eUsg;
	pContainer->tVtx.pData = new char[(size_t)iSize * (size_t)iCount];

	memcpy_s(pContainer->tVtx.pData, iSize * iCount, _pVtx, iSize * iCount);

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.Usage = eUsg;
	tDesc.ByteWidth = pContainer->tVtx.iCount * pContainer->tVtx.iSize;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tData = {};

	tData.pSysMem = _pVtx;

	char* pVtx = (char*)_pVtx;

	for (int i = 0; i < iCount; ++i)
	{
		Vector3* pPos = (Vector3*)pVtx;

		if (pPos->x > m_vMax.x)
			m_vMax.x = pPos->x;

		if (pPos->y > m_vMax.y)
			m_vMax.y = pPos->y;

		if (pPos->z > m_vMax.z)
			m_vMax.z = pPos->z;

		if (pPos->x < m_vMin.x)
			m_vMin.x = pPos->x;

		if (pPos->y < m_vMin.y)
			m_vMin.y = pPos->y;

		if (pPos->z < m_vMin.z)
			m_vMin.z = pPos->z;

		pVtx += iSize;
	}

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &pContainer->tVtx.pBuffer)))
		return false;

	return true;
}

bool CMesh::AddIndexBuffer(void* pIdxData, int iIdxSize, int iIdxCount, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, PMESHCONTAINER pContainer)
{
	if (!pIdxData)
		return false;

	Index tIB;

	tIB.iCount = iIdxCount;
	tIB.iSize = iIdxSize;
	tIB.eUsage = eIdxUsage;
	tIB.eFmt = eFmt;
	size_t iTotalSize = (size_t)iIdxCount * (size_t)iIdxSize;
	tIB.pData = new char[iTotalSize];


	memcpy_s(tIB.pData, iTotalSize, pIdxData, iTotalSize);

	D3D11_BUFFER_DESC tIndexDesc = {};

	tIndexDesc.Usage = eIdxUsage;
	tIndexDesc.ByteWidth = tIB.iCount * tIB.iSize;
	tIndexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tIndexData = {};

	tIndexData.pSysMem = pIdxData;

	if (FAILED(DEVICE->CreateBuffer(&tIndexDesc, &tIndexData, &tIB.pBuffer)))
		return false;

	pContainer->vecIdx.push_back(tIB);

	return true;
}

bool CMesh::LoadMesh(const std::string& strKey, const TCHAR* strFilePath, const std::string& strRootPath, bool bLoop, int iIndex)
{
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strRootPath);

	if (pPath)
		wcscpy_s(strFullPath, pPath);

	wcscat_s(strFullPath, strFilePath);

	return LoadMeshFromFull(strKey, strFullPath, bLoop, iIndex);
}

bool CMesh::LoadMeshFromFull(const std::string& strKey, const TCHAR* strFullPath, bool bLoop, int iIndex)
{
	SetName(strKey);

	char pFullPath[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, strFullPath, -1, pFullPath, lstrlen(strFullPath), NULL, NULL);
#else
	strcpy_s(pFullPath, strFullPath);
#endif
	char strExt[_MAX_EXT] = {};

	_splitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

	_strupr_s(strExt);

	if (strcmp(strExt, ".FBX") == 0)
	{
		CFBXLoader loader;

		if (loader.LoadFbx(pFullPath, m_eMeshType != MESH_TYPE::MT_ANIMATION_MESH))
		{
			return	ConvertFBX(&loader, pFullPath, bLoop, iIndex);
		}

		return false;
	}

	return LoadMesh(pFullPath);
}

void CMesh::Save(FILE* pFile)
{
	size_t iLength = m_strName.length();

	fwrite(&iLength, sizeof(size_t), 1, pFile);
	if (iLength > 0)
	{
		fwrite(m_strName.c_str(), 1, iLength, pFile);
	}

	fwrite(&m_vMax, 12, 1, pFile);
	fwrite(&m_vMin, 12, 1, pFile);
	fwrite(&m_eMeshType, 4, 1, pFile);

	size_t iSize = m_vecMeshContainer.size();

	fwrite(&iSize, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iSize; i++)
	{
		PMESHCONTAINER pContainer = m_vecMeshContainer[i];

		Vertex tVtx = pContainer->tVtx;

		fwrite(&pContainer->eTopology, 4, 1, pFile);
		fwrite(&tVtx.iCount, 4, 1, pFile);
		fwrite(&tVtx.iSize, 4, 1, pFile);
		fwrite(&tVtx.iUsage, 4, 1, pFile);

		//std::vector<VERTEX3D>	vec(tVtx.iCount);

		//memcpy_s(&vec[0], tVtx.iCount * tVtx.iSize, tVtx.pData, tVtx.iCount * tVtx.iSize);
		bool bVtxData = false;

		if (tVtx.pData)
		{
			bVtxData = true;
		}

		fwrite(&bVtxData, 1, 1, pFile);

		if (tVtx.pData)
		{
			fwrite(tVtx.pData, tVtx.iSize, tVtx.iCount, pFile);
		}

		size_t iIdxSize = m_vecMeshContainer[i]->vecIdx.size();

		fwrite(&iIdxSize, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iIdxSize; j++)
		{
			Index tIdx = pContainer->vecIdx[j];

			fwrite(&tIdx.eFmt, 4, 1, pFile);
			fwrite(&tIdx.eUsage, 4, 1, pFile);
			fwrite(&tIdx.iCount, 4, 1, pFile);
			fwrite(&tIdx.iSize, 4, 1, pFile);

			bool bIdxData = false;

			if (tIdx.pData)
			{
				bIdxData = true;
			}

			fwrite(&bIdxData, 1, 1, pFile);

			if (tIdx.pData)
			{
				fwrite(tIdx.pData,
					tIdx.iSize,
					tIdx.iCount, pFile);
			}

			bool bMtrl = false;

			if (pContainer->vecMaterial[j])
				bMtrl = true;

			fwrite(&bMtrl, 1, 1, pFile);

			if(pContainer->vecMaterial[j])
				pContainer->vecMaterial[j]->Save(pFile);

			bool bShader = false;

			if (pContainer->vecShader[j])
			{
				bShader = true;
			}

			fwrite(&bShader, 1, 1, pFile);

			if (pContainer->vecShader[j])
			{
				std::string strShader = pContainer->vecShader[j]->GetName();

				int iLength = (int)strShader.length();

				fwrite(&iLength, 4, 1, pFile);
				if (iLength > 0)
				{
					fwrite(strShader.c_str(), 1, iLength, pFile);
				}
			}
		}
	}
}

void CMesh::Load(FILE* pFile)
{
	m_iRefCount = 1;
	m_bActive = true;
	m_bEnable = true;

	size_t iLength = 0;

	fread(&iLength, sizeof(size_t), 1, pFile);
	if (iLength > 0)
	{
		char strName[MAX_PATH] = {};
		fread(strName, 1, iLength, pFile);
		m_strName = strName;
	}

	fread(&m_vMax, 12, 1, pFile);
	fread(&m_vMin, 12, 1, pFile);
	fread(&m_eMeshType, 4, 1, pFile);

	size_t iSize = m_vecMeshContainer.size();

	for (size_t i = 0; i < iSize; i++)
	{
		SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->tVtx.pData);
		size_t iIBSize = m_vecMeshContainer[i]->vecIdx.size();

		for (size_t j = 0; j < iIBSize; j++)
		{
			SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->vecIdx[j].pData);
		}

		//SAFE_RELEASE_VECLIST(m_vecMeshContainer[i]->vecMaterial);
		SAFE_DELETE(m_vecMeshContainer[i]);
	}

	m_vecMeshContainer.clear();

	iSize = 0;

	fread(&iSize, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iSize; i++)
	{
		PMESHCONTAINER pContainer = new MESHCONTAINER;

		Vertex& tVtx = pContainer->tVtx;

		fread(&pContainer->eTopology, 4, 1, pFile);
		fread(&tVtx.iCount, 4, 1, pFile);
		fread(&tVtx.iSize, 4, 1, pFile);
		fread(&tVtx.iUsage, 4, 1, pFile);

		bool bVtxData = false;

		fread(&bVtxData, 1, 1, pFile);

		if (bVtxData)
		{
			if (tVtx.iSize * tVtx.iCount > 0)
			{
				tVtx.pData = new char[(size_t)tVtx.iCount * (size_t)tVtx.iSize];
				fread(tVtx.pData, tVtx.iSize, tVtx.iCount, pFile);
			}
		}

		m_vecVtx.resize(tVtx.iCount);

		size_t iTotal = tVtx.iSize * tVtx.iCount;
		memcpy_s(&m_vecVtx[0], iTotal, tVtx.pData, iTotal);

		D3D11_BUFFER_DESC tDesc = {};

		tDesc.Usage = tVtx.iUsage;
		tDesc.ByteWidth = tVtx.iCount * tVtx.iSize;
		tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA tData = {};

		tData.pSysMem = tVtx.pData;

		if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &tVtx.pBuffer)))
			return;

		size_t iIdxSize = 0;

		fread(&iIdxSize, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iIdxSize; j++)
		{
			Index tIdx = {};

			fread(&tIdx.eFmt, 4, 1, pFile);
			fread(&tIdx.eUsage, 4, 1, pFile);
			fread(&tIdx.iCount, 4, 1, pFile);
			fread(&tIdx.iSize, 4, 1, pFile);
			bool bIdxData = false;

			fread(&bIdxData, 1, 1, pFile);
			if (bIdxData)
			{
				if (tIdx.iCount * tIdx.iSize > 0)
				{
					tIdx.pData = new char[(size_t)tIdx.iCount * (size_t)tIdx.iSize];
					fread(tIdx.pData,
						tIdx.iSize,
						tIdx.iCount, pFile);
				}
			}

			if (j == 0)
			{
				m_vecIdx.resize(tIdx.iCount);
				memcpy_s(&m_vecIdx[0], (size_t)tIdx.iCount * (size_t)tIdx.iSize, tIdx.pData, (size_t)tIdx.iCount * (size_t)tIdx.iSize);
			}

			D3D11_BUFFER_DESC tIndexDesc = {};

			tIndexDesc.Usage = tIdx.eUsage;
			tIndexDesc.ByteWidth = tIdx.iCount * tIdx.iSize;
			tIndexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA tIndexData = {};

			tIndexData.pSysMem = tIdx.pData;

			if (FAILED(DEVICE->CreateBuffer(&tIndexDesc, &tIndexData, &tIdx.pBuffer)))
				return;

			pContainer->vecIdx.push_back(tIdx);

			bool bMtrl = false;

			fread(&bMtrl, 1, 1, pFile);

			std::shared_ptr<class CMaterial> pMtrl = nullptr;
			if (bMtrl)
			{
				pMtrl = std::shared_ptr<class CMaterial>(new CMaterial);

				pMtrl->Load(pFile);
			}

			pContainer->vecMaterial.push_back(pMtrl);

			bool bShader = false;

			fread(&bShader, 1, 1, pFile);

			if (bShader)
			{
				int iLength = 0;
				fread(&iLength, 4, 1, pFile);

				if (iLength > 0)
				{
					char strShader[MAX_PATH] = {};
					fread(strShader, 1, iLength, pFile);

					pContainer->vecShader.push_back(GET_SINGLE(CShaderManager)->FindShader(strShader));
				}
			}
			else
			{
				pContainer->vecShader.push_back(nullptr);
			}
		}

		m_vecMeshContainer.push_back(pContainer);
	}
}

void CMesh::ShowWindow()
{
	if (ImGui::Begin("Mesh"))
	{
		if (ImGui::Button("Save Mesh"))
		{
			const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(MESH_PATH);
			ImGuiFileDialog::Instance()->OpenDialog("Save Mesh", "Save Mesh", ".msh", pPath);
		}

		if (ImGuiFileDialog::Instance()->Display("Save Mesh"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string strFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string strPath = ImGuiFileDialog::Instance()->GetCurrentPath();

				TCHAR pFilePath[MAX_PATH] = {};
#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, strFilePath.c_str(), -1, pFilePath, static_cast<int>(strFilePath.length()));
#else
				strcpy_s(pFilePath, MAX_PATH, strFilePath.c_str());
#endif
				SaveMesh(strFilePath.c_str());
			}
			ImGuiFileDialog::Instance()->Close();
		}
	}
	ImGui::End();
}

void CMesh::DeleteIndexBuffer(int iContainer, int iSubset, int iIndex)
{
	if (iContainer >= m_vecMeshContainer.size())
		return;

	if (iSubset >= m_vecMeshContainer[iContainer]->vecIdx.size())
		return;

	if (iSubset >= m_vecMeshContainer[iContainer]->vecShader.size())
		return;

	if (iSubset >= m_vecMeshContainer[iContainer]->vecMaterial.size())
		return;

	int iSize = m_vecMeshContainer[iContainer]->vecIdx[iSubset].iSize;
	int iCount = m_vecMeshContainer[iContainer]->vecIdx[iSubset].iCount;
	unsigned int* pData = reinterpret_cast<unsigned int*>(m_vecMeshContainer[iContainer]->vecIdx[iSubset].pData);

	std::vector<Index>::iterator iter = m_vecMeshContainer[iContainer]->vecIdx.begin() + iSubset;
	m_vecMeshContainer[iContainer]->vecIdx.erase(iter);

	AddIndexBuffer(pData, iSize, iIndex, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT, m_vecMeshContainer[iContainer]);
	AddIndexBuffer(pData + iIndex, iSize, iCount - iIndex, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT, m_vecMeshContainer[iContainer]);

	std::vector<std::shared_ptr<CMaterial>>::iterator iterM = m_vecMeshContainer[iContainer]->vecMaterial.begin() + iSubset;
	std::shared_ptr<CMaterial> pMtrl = *iterM;
	m_vecMeshContainer[iContainer]->vecMaterial.erase(iterM);

	m_vecMeshContainer[iContainer]->vecMaterial.push_back(pMtrl->Clone());
	m_vecMeshContainer[iContainer]->vecMaterial.push_back(pMtrl->Clone());

	std::vector<std::shared_ptr<CShader>>::iterator iterS = m_vecMeshContainer[iContainer]->vecShader.begin() + iSubset;
	std::shared_ptr<CShader> pShader = *iterS;
	m_vecMeshContainer[iContainer]->vecShader.erase(iterS);
	m_vecMeshContainer[iContainer]->vecShader.push_back(pShader);
	m_vecMeshContainer[iContainer]->vecShader.push_back(pShader);
}

void CMesh::AddContainer(class CMesh* pMesh, int iContainer, int iSubset)
{
	if (iContainer == -1)
	{
		size_t iSize = pMesh->m_vecMeshContainer.size();
		for (size_t i = 0; i < iSize; i++)
		{
			PMESHCONTAINER pContainer = new MESHCONTAINER;

			m_vecMeshContainer.push_back(pContainer);

			AddVertexBuffer(
				pMesh->m_vecMeshContainer[i]->tVtx.pData,
				pMesh->m_vecMeshContainer[i]->tVtx.iSize,
				pMesh->m_vecMeshContainer[i]->tVtx.iCount,
				pMesh->m_vecMeshContainer[i]->tVtx.iUsage,
				pMesh->m_vecMeshContainer[i]->eTopology, pContainer);

			if (iSubset == -1)
			{
				size_t iSubset = pMesh->m_vecMeshContainer[i]->vecIdx.size();

				for (size_t j = 0; j < iSubset; j++)
				{
					AddIndexBuffer(
						pMesh->m_vecMeshContainer[i]->vecIdx[j].pData,
						pMesh->m_vecMeshContainer[i]->vecIdx[j].iSize,
						pMesh->m_vecMeshContainer[i]->vecIdx[j].iCount,
						pMesh->m_vecMeshContainer[i]->vecIdx[j].eUsage,
						pMesh->m_vecMeshContainer[i]->vecIdx[j].eFmt, pContainer);

				}

				size_t iMtrl = pMesh->m_vecMeshContainer[i]->vecMaterial.size();

				for (size_t k = 0; k < iMtrl; k++)
				{
					pContainer->vecMaterial.push_back(pMesh->m_vecMeshContainer[i]->vecMaterial[k]->Clone());
				}

				size_t iShader = pMesh->m_vecMeshContainer[i]->vecShader.size();

				for (size_t k = 0; k < iShader; k++)
				{
					pContainer->vecShader.push_back(pMesh->m_vecMeshContainer[i]->vecShader[k]);
				}
			}
		}
	}
	else
	{
		PMESHCONTAINER pContainer = new MESHCONTAINER;

		m_vecMeshContainer.push_back(pContainer);

		AddVertexBuffer(
			pMesh->m_vecMeshContainer[iContainer]->tVtx.pData,
			pMesh->m_vecMeshContainer[iContainer]->tVtx.iSize,
			pMesh->m_vecMeshContainer[iContainer]->tVtx.iCount,
			pMesh->m_vecMeshContainer[iContainer]->tVtx.iUsage,
			pMesh->m_vecMeshContainer[iContainer]->eTopology, pContainer);

		if (iSubset == -1)
		{
			size_t iSubset = pMesh->m_vecMeshContainer[iContainer]->vecIdx.size();

			for (size_t j = 0; j < iSubset; j++)
			{
				AddIndexBuffer(
					pMesh->m_vecMeshContainer[iContainer]->vecIdx[j].pData,
					pMesh->m_vecMeshContainer[iContainer]->vecIdx[j].iSize,
					pMesh->m_vecMeshContainer[iContainer]->vecIdx[j].iCount,
					pMesh->m_vecMeshContainer[iContainer]->vecIdx[j].eUsage,
					pMesh->m_vecMeshContainer[iContainer]->vecIdx[j].eFmt, pContainer);
			}

			size_t iMtrl = pMesh->m_vecMeshContainer[iContainer]->vecMaterial.size();

			for (size_t k = 0; k < iMtrl; k++)
			{
				pContainer->vecMaterial.push_back(pMesh->m_vecMeshContainer[iContainer]->vecMaterial[k]->Clone());
			}

			size_t iShader = pMesh->m_vecMeshContainer[iContainer]->vecShader.size();

			for (size_t k = 0; k < iShader; k++)
			{
				pContainer->vecShader.push_back(pMesh->m_vecMeshContainer[iContainer]->vecShader[k]);
			}
		}
		else
		{
			AddIndexBuffer(
				pMesh->m_vecMeshContainer[iContainer]->vecIdx[iSubset].pData,
				pMesh->m_vecMeshContainer[iContainer]->vecIdx[iSubset].iSize,
				pMesh->m_vecMeshContainer[iContainer]->vecIdx[iSubset].iCount,
				pMesh->m_vecMeshContainer[iContainer]->vecIdx[iSubset].eUsage,
				pMesh->m_vecMeshContainer[iContainer]->vecIdx[iSubset].eFmt, pContainer);

			size_t iMtrl = pMesh->m_vecMeshContainer[iContainer]->vecMaterial.size();

			for (size_t k = 0; k < iMtrl; k++)
			{
				pContainer->vecMaterial.push_back(pMesh->m_vecMeshContainer[iContainer]->vecMaterial[iSubset]->Clone());
			}

			size_t iShader = pMesh->m_vecMeshContainer[iContainer]->vecShader.size();

			for (size_t k = 0; k < iShader; k++)
			{
				pContainer->vecShader.push_back(pMesh->m_vecMeshContainer[iContainer]->vecShader[iSubset]);
			}
		}
	}
}

void CMesh::ChangeContainer(class CMesh* pMesh, int iDestContainer, int iContainer)
{
	if (!pMesh)
		return;

	if (m_vecMeshContainer.size() <= iDestContainer)
		return;

	else if (pMesh->m_vecMeshContainer.size() <= iContainer)
		return;

	SAFE_DELETE_ARRAY(m_vecMeshContainer[iDestContainer]->tVtx.pData);
	SAFE_RELEASE(m_vecMeshContainer[iDestContainer]->tVtx.pBuffer);

	for (size_t i = 0; i < m_vecMeshContainer[iDestContainer]->vecIdx.size(); i++)
	{
		SAFE_DELETE_ARRAY(m_vecMeshContainer[iDestContainer]->vecIdx[i].pData);
		SAFE_RELEASE(m_vecMeshContainer[iDestContainer]->vecIdx[i].pBuffer);
	}
	m_vecMeshContainer[iDestContainer]->vecIdx.clear();

	AddVertexBuffer(pMesh->m_vecMeshContainer[iContainer]->tVtx.pData, pMesh->m_vecMeshContainer[iContainer]->tVtx.iSize, pMesh->m_vecMeshContainer[iContainer]->tVtx.iCount,
		pMesh->m_vecMeshContainer[iContainer]->tVtx.iUsage, pMesh->m_vecMeshContainer[iContainer]->eTopology, m_vecMeshContainer[iDestContainer]);

	m_vecMeshContainer[iDestContainer]->vecMaterial.clear();
	m_vecMeshContainer[iDestContainer]->vecShader.clear();
	for (size_t i = 0; i < pMesh->m_vecMeshContainer[iContainer]->vecIdx.size(); i++)
	{
		AddIndexBuffer(pMesh->m_vecMeshContainer[iContainer]->vecIdx[i].pData, pMesh->m_vecMeshContainer[iContainer]->vecIdx[i].iSize,
			pMesh->m_vecMeshContainer[iContainer]->vecIdx[i].iCount, pMesh->m_vecMeshContainer[iContainer]->vecIdx[i].eUsage,
			pMesh->m_vecMeshContainer[iContainer]->vecIdx[i].eFmt, m_vecMeshContainer[iDestContainer]);

		if (pMesh->m_vecMeshContainer[iContainer]->vecMaterial.size() > i)
		{
			if (pMesh->m_vecMeshContainer[iContainer]->vecMaterial[i])
			{
				m_vecMeshContainer[iDestContainer]->vecMaterial.push_back(pMesh->m_vecMeshContainer[iContainer]->vecMaterial[i]->Clone());
			}
		}
		if (pMesh->m_vecMeshContainer[iContainer]->vecShader.size() > i)
		{
			if (pMesh->m_vecMeshContainer[iContainer]->vecShader[i])
			{
				m_vecMeshContainer[iDestContainer]->vecShader.push_back(pMesh->m_vecMeshContainer[iContainer]->vecShader[i]);
			}
		}
	}

}

class CMesh* CMesh::Clone()
{
	return nullptr;
}
