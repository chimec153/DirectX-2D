#include "Mesh2D.h"
#include "../Device.h"
#include "ShaderManager.h"

CMesh2D::CMesh2D()	:
	m_ePrimitive(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED)
{
	memset(&m_tVB, 0, sizeof(m_tVB));
	memset(&m_tIB, 0, sizeof(m_tIB));
}

CMesh2D::~CMesh2D()
{	
	SAFE_RELEASE(m_tVB.pBuffer);
	SAFE_RELEASE(m_tIB.pBuffer);
}

bool CMesh2D::Init()
{
	return true;
}

bool CMesh2D::CreateMesh()
{
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VertexColor tVertex[4] = {};

	tVertex[0].vPos = Vector3(0.f, 1.f, 0.f);
	tVertex[0].vColor = Vector4::Red;

	tVertex[1].vPos = Vector3(1.f, 1.f, 0.f);
	tVertex[1].vColor = Vector4::Green;

	tVertex[2].vPos = Vector3(0.f, 0.f, 0.f);
	tVertex[2].vColor = Vector4(1.f, 1.f, 0.f, 1.f);

	tVertex[3].vPos = Vector3(1.f, 0.f, 0.f);
	tVertex[3].vColor = Vector4(1.f, 0.f, 1.f, 1.f);

	m_tVB.iCount = 4;
	m_tVB.iSize = sizeof(VertexColor);
	m_tVB.eUsage = D3D11_USAGE_IMMUTABLE;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tDesc.ByteWidth = m_tVB.iCount * m_tVB.iSize;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tData = {};

	tData.pSysMem = tVertex;

	m_vMax = Vector3(1.f, 1.f, 0.f);
	m_vMin = Vector3(0.f, 0.f, 0.f);

	if(FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &m_tVB.pBuffer)))
		return false;

	m_tIB.iCount = 6;
	m_tIB.iSize = sizeof(unsigned short);
	m_tIB.eUsage = D3D11_USAGE_IMMUTABLE;
	m_tIB.eFmt = DXGI_FORMAT_R16_UINT;

	D3D11_BUFFER_DESC tIndexDesc = {};

	tIndexDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tIndexDesc.ByteWidth = m_tIB.iCount * m_tIB.iSize;
	tIndexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	unsigned short iIndex[6] = { 0, 1, 3, 0, 3, 2 };

	D3D11_SUBRESOURCE_DATA tIndexData = {};

	tIndexData.pSysMem = iIndex;

	if (FAILED(DEVICE->CreateBuffer(&tIndexDesc, &tIndexData, &m_tIB.pBuffer)))
		return false;

	return true;
}

void CMesh2D::Render(float fTime)
{
	UINT	iStride = m_tVB.iSize;
	UINT	iOffset = 0;

	CONTEXT->IASetPrimitiveTopology(m_ePrimitive);
	CONTEXT->IASetVertexBuffers(0, 1, &m_tVB.pBuffer, &iStride, &iOffset);

	if (m_tIB.pBuffer)
	{
		CONTEXT->IASetIndexBuffer(m_tIB.pBuffer, m_tIB.eFmt, 0);
		CONTEXT->DrawIndexed(m_tIB.iCount, 0, 0);
	}

	else
	{
		CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->Draw(m_tVB.iCount, 0);
	}
}
