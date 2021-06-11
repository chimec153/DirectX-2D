#include "Particle.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material.h"
#include "../Device.h"
#include "../Resource/ComputeShader.h"
#include "../Resource/Mesh2D.h"
#include "../RenderManager.h"
#include "../Render/RenderState.h"
#include "../UI/imgui/imgui.h"
#include "Transform.h"
#include "Renderer.h"

CParticle::CParticle()	:
	m_pUAV(nullptr)
	, m_pUAVShare(nullptr)
	, m_pSRV(nullptr)
	, m_pSRVShare(nullptr)
	, m_fMaxSpawnTime(0.1f)
	, m_fTime(m_fMaxSpawnTime)
	, m_pSrcBuffer(nullptr)
	, m_pSrcShareBuffer(nullptr)
#ifdef _DEBUG
	, m_pBuffer(nullptr)
	, m_pShareBuffer(nullptr)
#endif
	, m_iSpawnCount(1)
	, m_iThreadGroupSize(64)
{
	m_iSceneComponentClassType = static_cast<int>(SCENECOMPONENT_CLASS_TYPE::PARTICLE);
	memset(&m_tCBuffer, 0, sizeof(PARTICLECBUFFER));
}

CParticle::CParticle(const CParticle& com)	:
	CSceneComponent(com)
	, m_tCBuffer(com.m_tCBuffer)
	, m_pUAV(nullptr)
	, m_pUAVShare(nullptr)
	, m_pSRV(nullptr)
	, m_pSRVShare(nullptr)
	, m_fMaxSpawnTime(com.m_fMaxSpawnTime)
	, m_fTime(com.m_fTime)
	, m_pSrcBuffer(nullptr)
	, m_pSrcShareBuffer(nullptr)
#ifdef _DEBUG
	, m_pBuffer(nullptr)
	, m_pShareBuffer(nullptr)
#endif
	, m_iSpawnCount(com.m_iSpawnCount)
	, m_iThreadGroupSize(com.m_iThreadGroupSize)
{
	m_pUpdateShader = com.m_pUpdateShader;
	SetMaxCount(m_tCBuffer.iMaxCount);
}

CParticle::~CParticle()
{
	SAFE_RELEASE(m_pUAV);
	SAFE_RELEASE(m_pUAVShare);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pSRVShare);
	SAFE_RELEASE(m_pSrcBuffer);
	SAFE_RELEASE(m_pSrcShareBuffer);
#ifdef _DEBUG
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pShareBuffer);
#endif
}

bool CParticle::CreateBuffer(int iCount, int iSize, 
	ID3D11UnorderedAccessView** pUAV, ID3D11ShaderResourceView** pSRV, 
	bool bUAV, ID3D11Buffer** pBuffer)
{
	SAFE_RELEASE((*pSRV));
	SAFE_RELEASE((*pUAV));
	SAFE_RELEASE((*pBuffer));

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = iCount * iSize;
	tDesc.StructureByteStride = iSize;
	tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	if (bUAV)
	{
		tDesc.Usage = D3D11_USAGE_DEFAULT;
		tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	}

	else
	{
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		tDesc.Usage = D3D11_USAGE_DYNAMIC;
		tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}

	if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, pBuffer)))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tSRVDesc.BufferEx.FirstElement = 0;
	tSRVDesc.BufferEx.Flags = 0;
	tSRVDesc.BufferEx.NumElements = iCount;

	if (FAILED(DEVICE->CreateShaderResourceView(*pBuffer, &tSRVDesc, pSRV)))
	{
		return false;
	}

	if (bUAV)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC tUAV = {};

		tUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		tUAV.Format = DXGI_FORMAT_UNKNOWN;
		tUAV.Buffer.FirstElement = 0;
		tUAV.Buffer.Flags = 0;
		tUAV.Buffer.NumElements = iCount;

		if (FAILED(DEVICE->CreateUnorderedAccessView(*pBuffer, &tUAV, pUAV)))
		{
			return false;
		}
	}

	return true;
}

void CParticle::SetMaxSpawnTime(float fTime)
{
	m_fMaxSpawnTime = fTime;
	m_fTime = fTime;
}

void CParticle::SetAccel(const Vector3& vAccel)
{
	m_tCBuffer.vAccel = vAccel;
}

void CParticle::SetMaxTime(float fTime)
{
	m_tCBuffer.fMaxTime = fTime;
}

void CParticle::SetMinTime(float fTime)
{
	m_tCBuffer.fMinTime = fTime;
}

void CParticle::SetMaxColor(const Vector4 & vColor)
{
	m_tCBuffer.vMaxColor = vColor;
}

void CParticle::SetMinColor(const Vector4 & vColor)
{
	m_tCBuffer.vMinColor = vColor;
}

void CParticle::SetAngleAccel(float fAccel)
{
	m_tCBuffer.fAngleAccel = fAccel;
}

void CParticle::SetMaxScale(const Vector3 & vScale)
{
	m_tCBuffer.vMaxScale = vScale;
}

void CParticle::SetMinScale(const Vector3 & vScale)
{
	m_tCBuffer.vMinScale = vScale;
}

void CParticle::SetMove(bool bMove)
{
	m_tCBuffer.bMove = bMove;
}

void CParticle::SetRange(const Vector3 & vRange)
{
	m_tCBuffer.vRange = vRange;
}

void CParticle::SetMaxSpeed(float fSpeed)
{
	m_tCBuffer.fMaxSpeed = fSpeed;
}

void CParticle::SetMinSpeed(float fSpeed)
{
	m_tCBuffer.fMinSpeed = fSpeed;
}

void CParticle::SetMaxCount(int iCount)
{
	m_tCBuffer.iMaxCount = iCount;

	CreateBuffer(m_tCBuffer.iMaxCount, sizeof(PARTICLE), &m_pUAV, &m_pSRV, true, &m_pSrcBuffer);
	CreateBuffer(1, sizeof(PARTICLESHARE), &m_pUAVShare, &m_pSRVShare, true, &m_pSrcShareBuffer);
#ifdef _DEBUG
	SAFE_RELEASE(m_pBuffer);
	D3D11_BUFFER_DESC tCopy = {};
	tCopy.ByteWidth = sizeof(PARTICLE) * iCount;
	tCopy.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	tCopy.StructureByteStride = sizeof(PARTICLE);
	tCopy.Usage = D3D11_USAGE_STAGING;

	DEVICE->CreateBuffer(&tCopy, nullptr, &m_pBuffer);

	SAFE_RELEASE(m_pShareBuffer);
	D3D11_BUFFER_DESC tShareCopy = {};
	tShareCopy.ByteWidth = sizeof(PARTICLESHARE);
	tShareCopy.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	tShareCopy.StructureByteStride = sizeof(PARTICLESHARE);
	tShareCopy.Usage = D3D11_USAGE_STAGING;

	DEVICE->CreateBuffer(&tShareCopy, nullptr, &m_pShareBuffer);
#endif
}

void CParticle::SetMaxFrame(int iFrame)
{
	m_tCBuffer.iMaxFrame = iFrame;
}

void CParticle::SetImageSize(const Vector2& vSize)
{
	m_tCBuffer.vImageSize = vSize;
}

void CParticle::SetCountX(int iCount)
{
	m_tCBuffer.iCountX = iCount;
}

void CParticle::SetCountY(int iCount)
{
	m_tCBuffer.iCountY = iCount;
}

void CParticle::SetSpawnCount(int iCount)
{
	m_iSpawnCount = iCount;
}

void CParticle::SetUpdateShader(const std::string& strKey)
{
	m_pUpdateShader = GET_SINGLE(CShaderManager)->FindShader(strKey);
}

void CParticle::SetThreadGroupSize(int iSize)
{
	m_iThreadGroupSize = iSize;
}

bool CParticle::Init()
{
	if (!CSceneComponent::Init())
		return false;

	SetUpdateShader("ParicleUpdateShader");


	SetMesh("PointMesh");
	ResizeMaterial(1);
	SetMaterial("Color");
	ResizeShader(1);
	SetShader("ParticleShader");
	ResizeRenderState(RENDER_STATE_TYPE::BLEND, 1);
	ResizeRenderState(RENDER_STATE_TYPE::RASTERIZER, 1);
	ResizeRenderState(RENDER_STATE_TYPE::DEPTHSTENCIL, 1);

	SetMaxCount(100);

	return true;
}

void CParticle::Start()
{
	CSceneComponent::Start();
}

void CParticle::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	m_fTime += fTime;

	if (m_fTime >= m_fMaxSpawnTime)
	{
		m_fTime -= m_fMaxSpawnTime;
		m_tCBuffer.iSpawn = m_iSpawnCount;

		/*if (m_iSpawnCount > 0)
		{
			--m_iSpawnCount;
		}
		else if(m_iSpawnCount < 0)
		{
			m_tCBuffer.iSpawn = 0;
		}*/
	}

	else
	{
		m_tCBuffer.iSpawn = 0;
	}
	
	m_tCBuffer.vPos = GetBoneWorldPos();

	UINT iCount = -1;

	CONTEXT->CSSetUnorderedAccessViews(0, 1, &m_pUAV, &iCount);
	CONTEXT->CSSetUnorderedAccessViews(1, 1, &m_pUAVShare, &iCount);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Particle", &m_tCBuffer);

	CComputeShader* pShader = (CComputeShader*)m_pUpdateShader.get();

	pShader->SetShader();
	pShader->Dispatch(m_tCBuffer.iMaxCount / m_iThreadGroupSize + 1, 1, 1);
	pShader->Clear();

	ID3D11UnorderedAccessView* pUAV = nullptr;

	CONTEXT->CSSetUnorderedAccessViews(0, 1, &pUAV, &iCount);
	CONTEXT->CSSetUnorderedAccessViews(1, 1, &pUAV, &iCount);

#ifdef _DEBUG
	CONTEXT->CopyResource(m_pBuffer, m_pSrcBuffer);
	CONTEXT->CopyResource(m_pShareBuffer, m_pSrcShareBuffer);
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	std::vector<PARTICLE> vecParticles(m_tCBuffer.iMaxCount);
	PARTICLESHARE tShare = {};
	CONTEXT->Map(m_pBuffer, 0, D3D11_MAP_READ, 0, &tSub);
	memcpy_s(&vecParticles[0], m_tCBuffer.iMaxCount * sizeof(PARTICLE), tSub.pData, m_tCBuffer.iMaxCount * sizeof(PARTICLE));
	CONTEXT->Unmap(m_pBuffer, 0);
	CONTEXT->Map(m_pShareBuffer, 0, D3D11_MAP_READ, 0, &tSub);
	memcpy_s(&tShare, sizeof(PARTICLESHARE), tSub.pData, sizeof(PARTICLESHARE));
	CONTEXT->Unmap(m_pShareBuffer, 0);
#endif
}

void CParticle::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CParticle::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CParticle::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CParticle::Render(float fTime)
{
	CSceneComponent::RenderObj(fTime);

	CONTEXT->GSSetShaderResources(104, 1, &m_pSRV);
	CONTEXT->GSSetShaderResources(105, 1, &m_pSRVShare);

	std::shared_ptr<CTransform> pTransform = GetTransform();

	if (pTransform)
	{
		pTransform->SetTransform();
	}

	CRenderer* pRenderer = GetRenderer();

	if (pRenderer)
	{
		pRenderer->RenderParticle(m_tCBuffer.iMaxCount, fTime);
	}

	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->GSSetShaderResources(104, 1, &pSRV);
	CONTEXT->GSSetShaderResources(105, 1, &pSRV);
}

void CParticle::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

std::shared_ptr<CComponent> CParticle::Clone()
{
	return std::shared_ptr<CComponent>(new CParticle(*this));
}

void CParticle::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_tCBuffer, sizeof(m_tCBuffer), 1, pFile);
	fwrite(&m_fMaxSpawnTime, 4, 1, pFile);
	fwrite(&m_fTime, 4, 1, pFile);
}

void CParticle::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	fread(&m_tCBuffer, sizeof(m_tCBuffer), 1, pFile);
	fread(&m_fMaxSpawnTime, 4, 1, pFile);
	fread(&m_fTime, 4, 1, pFile);

	m_pUpdateShader = GET_SINGLE(CShaderManager)->FindShader("ParicleUpdateShader");

	SetMaxCount(m_tCBuffer.iMaxCount);
}

void CParticle::ShowWindow()
{
	CSceneComponent::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		ImGui::Text("Particle");
		ImGui::Checkbox("Spawn", reinterpret_cast<bool*>(&m_tCBuffer.iSpawn));
		ImGui::InputFloat3("Acceleration", &m_tCBuffer.vAccel.x);
		ImGui::InputFloat("Max Time", &m_tCBuffer.fMaxTime, 0.f, 30.f);
		ImGui::InputFloat("Min Time", &m_tCBuffer.fMinTime, 0.f, 30.f);
		ImGui::ColorPicker4("Max Color", &m_tCBuffer.vMaxColor.x);
		ImGui::ColorPicker4("Min Color", &m_tCBuffer.vMinColor.x);
		ImGui::InputFloat3("Max Scale", &m_tCBuffer.vMaxScale.x);
		if (ImGui::InputFloat3("Min Scale", &m_tCBuffer.vMinScale.x))
		{
			SetMinScale(m_tCBuffer.vMinScale);
		}
		if (ImGui::InputFloat("Angle Acceleration", &m_tCBuffer.fAngleAccel))
		{
			SetAngleAccel(m_tCBuffer.fAngleAccel);
		}
		if (ImGui::Checkbox("Move", reinterpret_cast<bool*>(&m_tCBuffer.bMove)))
		{
			SetMove(m_tCBuffer.bMove);
		}
		if (ImGui::InputInt("Max Count", &m_tCBuffer.iMaxCount))
		{
			SetMaxCount(m_tCBuffer.iMaxCount);
		}
		if (ImGui::InputFloat("Max Speed", &m_tCBuffer.fMaxSpeed))
		{
			SetMaxSpeed(m_tCBuffer.fMaxSpeed);
		}
		if (ImGui::InputFloat("Min Speed", &m_tCBuffer.fMinSpeed))
		{
			SetMinSpeed(m_tCBuffer.fMinSpeed);
		}

		if (ImGui::InputFloat("Max Spawn Time", &m_fMaxSpawnTime))
		{
			SetMaxSpawnTime(m_fMaxSpawnTime);
		}

		if (ImGui::InputFloat2("Image Size", &m_tCBuffer.vImageSize.x))
		{
			SetImageSize(m_tCBuffer.vImageSize);
		}

		if (ImGui::InputInt("Max Frame", &m_tCBuffer.iMaxFrame))
		{
			SetMaxFrame(m_tCBuffer.iMaxFrame);
		}

		if (ImGui::InputInt("Animation Count X", &m_tCBuffer.iCountX))
		{
			SetCountX(m_tCBuffer.iCountX);
		}

		if (ImGui::InputInt("Animation Count Y", &m_tCBuffer.iCountY))
		{
			SetCountY(m_tCBuffer.iCountY);
		}
		if (ImGui::InputFloat3("Range", &m_tCBuffer.vRange.x))
		{
			SetRange(m_tCBuffer.vRange);
		}
		ImGui::InputInt("Spawn Count", &m_iSpawnCount);

	}
	ImGui::End();
}
