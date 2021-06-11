#include "HDR.h"
#include "../Device.h"

CHDR::CHDR()
{
}

CHDR::CHDR(const CHDR& hdr)	:
	CPostProcess(hdr)
{
}

CHDR::~CHDR()
{
}

void CHDR::Start()
{
	CPostProcess::Start();
}

bool CHDR::Init()
{
	if(!CPostProcess::Init())
		return false;

	Resolution tRS = RESOLUTION;

	ID3D11Buffer* pBuffer = nullptr;

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	tDesc.StructureByteStride = 4;
	tDesc.ByteWidth = 4 * tRS.iWidth * tRS.iHeight / (16 * 1024);
	tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &pBuffer)))
		return false;

	D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
	tUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	tUAVDesc.Buffer.NumElements = tRS.iWidth * tRS.iHeight / (16 * 1024);

	if (FAILED(DEVICE->CreateUnorderedAccessView(pBuffer, &tUAVDesc, &m_pUAV)))
	{
		SAFE_RELEASE(pBuffer);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

	if (FAILED(DEVICE->CreateShaderResourceView(pBuffer, &tSRVDesc, &m_pSRV)))
	{
		SAFE_RELEASE(pBuffer);
		return false;
	}

	SAFE_RELEASE(pBuffer);

	ID3D11Buffer* pAvgBuffer = nullptr;

	D3D11_BUFFER_DESC tAvgDesc = {};

	tAvgDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	tAvgDesc.StructureByteStride = 4;
	tAvgDesc.ByteWidth = 4;
	tAvgDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	if (FAILED(DEVICE->CreateBuffer(&tAvgDesc, nullptr, &pAvgBuffer)))
		return false;

	D3D11_UNORDERED_ACCESS_VIEW_DESC tAvgUavDesc = {};

	tAvgUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	tAvgUavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	tAvgUavDesc.Buffer.NumElements = 1;

	if (FAILED(DEVICE->CreateUnorderedAccessView(pAvgBuffer, &tAvgUavDesc, &m_pAverageUAV)))
	{
		SAFE_RELEASE(pAvgBuffer);
		return false;
	}

	if (FAILED(DEVICE->CreateShaderResourceView(pAvgBuffer, &tSRVDesc, &m_pAverageSRV)))
	{
		SAFE_RELEASE(pAvgBuffer);
		return false;
	}

	D3D11_BUFFER_DESC tConstDesc1 = {};

	tConstDesc1.Usage = D3D11_USAGE_DYNAMIC;
	tConstDesc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tConstDesc1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tConstDesc1.ByteWidth = 4;

	if (FAILED(DEVICE->CreateBuffer(&tConstDesc1, nullptr, &m_pConstBuffer1)))
		return false;

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	struct _tagDownScale
	{
		unsigned int x;
		unsigned int y;
		unsigned int iPixel;
		unsigned int iGroup;
	};

	_tagDownScale tScale = {};

	tScale.x = tRS.iWidth / 4;
	tScale.y = tRS.iHeight / 4;
	tScale.iPixel = tScale.x * tScale.y;
	tScale.iGroup = tScale.iPixel / 1024;

	CONTEXT->Map(m_pConstBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

	memcpy_s(tSub.pData, 4, &tScale, 4);

	CONTEXT->Unmap(m_pConstBuffer1, 0);

	return true;
}

void CHDR::Update(float fTime)
{
	CPostProcess::Update(fTime);
}

void CHDR::PostUpdate(float fTime)
{
	CPostProcess::PostUpdate(fTime);
}

void CHDR::Collision(float fTime)
{
	CPostProcess::Collision(fTime);
}

void CHDR::PreRender(float fTime)
{
	CPostProcess::PreRender(fTime);
}

void CHDR::Render(float fTime)
{
	CPostProcess::Render(fTime);
}

void CHDR::PostRender(float fTime)
{
	CPostProcess::PostRender(fTime);
}
