#include "Mesh2D.h"
#include "../Device.h"
#include "ShaderManager.h"


CMesh2D::CMesh2D()
{
	m_eMeshType = MESH_TYPE::MT_MESH2D;
}

CMesh2D::~CMesh2D()
{	
}

bool CMesh2D::Init()
{
	return true;
}

bool CMesh2D::CreateMesh()
{
	Vertex2D tVertex[4] = {};

	tVertex[0].vPos = Vector3(0.f, 1.f, 0.f);
	tVertex[0].vUV = Vector2(0.f, 0.f);
	tVertex[0].vColor = Vector4::Red;

	tVertex[1].vPos = Vector3(1.f, 1.f, 0.f);
	tVertex[1].vUV = Vector2(1.f, 0.f);
	tVertex[1].vColor = Vector4::Green;

	tVertex[2].vPos = Vector3(0.f, 0.f, 0.f);
	tVertex[2].vUV = Vector2(0.f, 1.f);
	tVertex[2].vColor = Vector4(1.f, 1.f, 0.f, 1.f);

	tVertex[3].vPos = Vector3(1.f, 0.f, 0.f);
	tVertex[3].vUV = Vector2(1.f, 1.f);
	tVertex[3].vColor = Vector4(1.f, 0.f, 1.f, 1.f);

	unsigned short iIndex[] = { 0, 1, 3, 0, 3, 2};

	return CreateMesh(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 4, sizeof(Vertex2D), D3D11_USAGE_IMMUTABLE, tVertex,
		6, 2, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R16_UINT, iIndex);
}

void CMesh2D::Render(float fTime, int iContainer, int iSubset)
{
	UINT	iStride = m_vecMeshContainer[iContainer]->tVtx.iSize;
	UINT	iOffset = 0;

	CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[iContainer]->eTopology);
	CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[iContainer]->tVtx.pBuffer, &iStride, &iOffset);

	if(!m_vecMeshContainer[iContainer]->vecIdx.empty())
	//if (m_vecMeshContainer[iContainer]->vecIdx[iSubset].pBuffer)
	{
		CONTEXT->IASetIndexBuffer(m_vecMeshContainer[iContainer]->vecIdx[iSubset].pBuffer, m_vecMeshContainer[iContainer]->vecIdx[iSubset].eFmt, 0);
		CONTEXT->DrawIndexed(m_vecMeshContainer[iContainer]->vecIdx[iSubset].iCount, 0, 0);
	}

	else
	{
		CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->Draw(m_vecMeshContainer[iContainer]->tVtx.iCount, 0);
	}
}

void CMesh2D::RenderInstancing(InstancingBuffer* pIBuffer, int iCount, int iSize, float fTime)
{
	size_t _iSize = m_vecMeshContainer.size();
	for (size_t i = 0; i < _iSize; i++)
	{
		UINT iStride[2] = { (UINT)m_vecMeshContainer[i]->tVtx.iSize,(UINT)iSize };
		UINT iOffset[2] = { 0,0 };

		ID3D11Buffer* vbs[2] = { m_vecMeshContainer[i]->tVtx.pBuffer, pIBuffer->pBuffer };

		CONTEXT->IASetVertexBuffers(0, 2, vbs, iStride, iOffset);
		CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[i]->eTopology);

		if (!m_vecMeshContainer[i]->vecIdx.empty())
		{
			size_t iIdxSize = m_vecMeshContainer[i]->vecIdx.size();

			for (size_t j = 0; j < iIdxSize; j++)
			{
				CONTEXT->IASetIndexBuffer(m_vecMeshContainer[i]->vecIdx[j].pBuffer, m_vecMeshContainer[i]->vecIdx[j].eFmt, 0);
				CONTEXT->DrawIndexedInstanced(m_vecMeshContainer[i]->vecIdx[j].iCount, iCount, 0, 0, 0);
			}
		}

		else
		{
			CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CONTEXT->DrawInstanced(m_vecMeshContainer[i]->tVtx.iCount, iCount, 0, 0);
		}
	}
}

void CMesh2D::RenderParticle(int iCount, float fTime)
{
	UINT iStride = m_vecMeshContainer[0]->tVtx.iSize;
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[0]->tVtx.pBuffer, &iStride, &iOffset);
	CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[0]->eTopology);

	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->DrawInstanced(m_vecMeshContainer[0]->tVtx.iCount, iCount, 0, 0);
}

bool CMesh2D::CreateMesh(D3D11_PRIMITIVE_TOPOLOGY eTopo, int iVtxCount, int iVtxSize,
	D3D11_USAGE eVtxUsage, void* pVtxData, int iIdxCount,
	int iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, void* pIdxData,
	D3D11_CPU_ACCESS_FLAG eCpuFlag,
	D3D11_CPU_ACCESS_FLAG eIdxCpuFlag)
{
	size_t iSize = m_vecMeshContainer.size();

	for (size_t i = 0; i < iSize; i++)
	{
		SAFE_RELEASE(m_vecMeshContainer[i]->tVtx.pBuffer);
		SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->tVtx.pData);
		size_t iIdxSize = m_vecMeshContainer[i]->vecIdx.size();

		for (size_t j = 0; j < iIdxSize; j++)
		{
			SAFE_RELEASE(m_vecMeshContainer[i]->vecIdx[j].pBuffer);
			SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->vecIdx[j].pData);

		}
		SAFE_DELETE(m_vecMeshContainer[i]);
	}	

	m_vecMeshContainer.clear();

	PMESHCONTAINER pContainer = new MESHCONTAINER;
	m_vecMeshContainer.push_back(pContainer);
	pContainer->eTopology = eTopo;

	pContainer->tVtx.iCount = iVtxCount;
	pContainer->tVtx.iSize = iVtxSize;
	pContainer->tVtx.iUsage = eVtxUsage;

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

	for (int i = 0; i < iVtxCount;++i)
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

		D3D11_BUFFER_DESC tIndexDesc = {};

		tIndexDesc.Usage = eIdxUsage;
		tIndexDesc.ByteWidth = tIdx.iCount * tIdx.iSize;
		tIndexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		tIndexDesc.CPUAccessFlags = eIdxCpuFlag;

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

void CMesh2D::RenderTest(float fTime)
{
	UINT iOffset =  0 ;
	UINT iStride = m_vecMeshContainer[0]->tVtx.iSize;

	CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[0]->eTopology);
	CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[0]->tVtx.pBuffer, &iStride, &iOffset);

	CONTEXT->IASetIndexBuffer(m_vecMeshContainer[0]->vecIdx[0].pBuffer, DXGI_FORMAT_R16_UINT, 0);
	CONTEXT->DrawIndexed(18, 0, 0);
}

void CMesh2D::RenderTest2(float fTime)
{
	CONTEXT->IASetIndexBuffer(m_vecMeshContainer[0]->vecIdx[0].pBuffer, DXGI_FORMAT_R16_UINT, 0);
	CONTEXT->DrawIndexed(36, 18, 0);
}

void CMesh2D::Save(FILE* pFile)
{
	CMesh::Save(pFile);
}

void CMesh2D::Load(FILE* pFile)
{
	CMesh::Load(pFile);
}
