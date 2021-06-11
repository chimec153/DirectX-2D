#include "LensFlares.h"
#include "Device.h"
#include "Resource/ResourceManager.h"
#include "CameraManager.h"
#include "Component/Camera.h"
#include "Resource/ShaderManager.h"
#include "Resource/Texture.h"
#include "RenderManager.h"
#include "Render/RenderState.h"
#include "UI/imgui/imgui.h"

DEFINITION_SINGLE(CLensFlares)

CLensFlares::CLensFlares() :
	m_pBuffer(nullptr)
	, m_bInScreen(false)
	, m_pAlphaBlend(nullptr)
	, m_pNoDepth(nullptr)
	, m_bStart(false)
	, m_vSunPos(5000.f, 5500.f, 5000.f)
{
	m_vecCBuffers.resize(8);
	m_vecFlares.resize(8);
}

CLensFlares::~CLensFlares()
{
	SAFE_RELEASE(m_pBuffer);
}

void CLensFlares::SetSunPos(const Vector3& vPos)
{
	m_vSunPos = vPos;
}

bool CLensFlares::Init()
{
	D3D11_BUFFER_DESC tDesc = {};

	tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tDesc.ByteWidth = static_cast<int>(sizeof(FLARECBUFFER) * m_vecCBuffers.size());
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &m_pBuffer)))
		return false;

	GET_SINGLE(CResourceManager)->LoadTexture("Corona",
		TEXT("LV_Common_Lensflare_A\\Texture2D\\LV_LensFlares_35MMPrime03_C_PYO.dds"));

	GET_SINGLE(CResourceManager)->LoadTexture("Flare",
		TEXT("LV_Common_Lensflare_A\\Texture2D\\LV_LensFlares_35MMPrime02_C_PYO.dds"));

	GET_SINGLE(CResourceManager)->LoadTexture("Sun",
		TEXT("LV_Common_Lensflare_A\\Texture2D\\LV_Common_Lensflare_A_Source_Sun02_D_PYO.dds"));

	m_pCoronaTexture = GET_SINGLE(CResourceManager)->FindTexture("Corona");
	m_pFlareTexture = GET_SINGLE(CResourceManager)->FindTexture("Flare");
	m_pSunTexture = GET_SINGLE(CResourceManager)->FindTexture("Sun");

	float  pValues[48] =
	{
		0.2f,
		0.1f,
		1.f, 1.f, 1.f, 0.3f,
		 0.3f,
		0.01f,
		1.f, 1.f, 1.f, 0.3f,
		 0.4f,
		0.05f,
		1.f, 1.f, 1.f, 0.3f,
		 0.6f,
		0.02f,
		1.f, 1.f, 1.f, 0.3f,
		 0.8f,
		0.05f,
		1.f, 1.f, 1.f, 0.3f,
		 1.f,
		0.07f,
		1.f, 1.f, 1.f, 0.3f,
		 1.5f,
		0.03f,
		1.f, 1.f, 1.f, 0.3f,
		 2.f,
		0.1f,
		1.f, 1.f, 1.f, 0.3f
	};

	memcpy_s(&m_vecFlares[0], 48 * 4, pValues, 48 * 4);

	m_pLensShader = GET_SINGLE(CShaderManager)->FindShader("LensFlareShader");

	m_pAlphaBlend = GET_SINGLE(CRenderManager)->FindState(ALPHA_BLEND);
	m_pNoDepth = GET_SINGLE(CRenderManager)->FindState(NO_DEPTH);

	return true;
}

void CLensFlares::Start()
{
	m_bStart = true;
}

void CLensFlares::Update(float fTime)
{
	if (m_bStart)
	{
		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
		Vector2 vRatio = RATIO;

		float fAspectRatio = vRatio.y / vRatio.x;
		Matrix matVP = {};
		if (pCam)
		{
			matVP = pCam->GetView() * pCam->GetProj();
		}
		float w = m_vSunPos.x * matVP[0][3] + m_vSunPos.y * matVP[1][3] + m_vSunPos.z * matVP[2][3] + matVP[3][3];
		Vector3 vSunPos = m_vSunPos.TranslationCoor(matVP);
		vSunPos.x /= vSunPos.z;
		vSunPos.y /= vSunPos.z;

		Vector3 vDir = -vSunPos;

		float fDist = vDir.Length();
		vDir /= fDist;

		if (vSunPos.x > 1.f || vSunPos.x < -1.f ||
			vSunPos.y > 1.f || vSunPos.y < -1.f || w < 0.f)
		{
			m_bInScreen = false;
		}
		else
		{
			//	Sx	0		cos(theta)	sin(theta)		Sx * cos(theta)		Sx * sin(theta)
			//	0	Sy		-sin(theta)	cos(theta)		-Sy * sin(theta)	Sy * cos(theta)

			m_bInScreen = true;

			for (int i = 0; i < m_vecFlares.size(); i++)
			{
				/*m_pCBuffers[i].vScaleRotation = Vector4(
					m_pFlares[i].fScale * vDir.x / fDist,
					-m_pFlares[i].fScale * vDir.y / fDist,
					m_pFlares[i].fScale * vDir.x / fDist * fAspectRatio,
					m_pFlares[i].fScale * vDir.x / fDist * fAspectRatio);*/
				m_vecCBuffers[i].vScaleRotation = Vector4(
					m_vecFlares[i].fScale,
					0.f,
					0.f,
					m_vecFlares[i].fScale * fAspectRatio);
				Vector3 vPos = m_vecFlares[i].fOffset * fDist * vDir;
				vPos.x += vSunPos.x;
				vPos.y += vSunPos.y;
				memcpy_s(&m_vecCBuffers[i].vPos.x, 12, &vPos.x, 12);

				m_vecCBuffers[i].vColor = m_vecFlares[i].vColor;
			}
		}
	}
}

void CLensFlares::Render(float fTime)
{
	if (!m_bInScreen)
		return;

	if (!m_bStart)
		return;

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

	memcpy_s(tSub.pData, m_vecCBuffers.size() * sizeof(m_vecCBuffers[0]), &m_vecCBuffers[0], m_vecCBuffers.size() * sizeof(m_vecCBuffers[0]));

	CONTEXT->Unmap(m_pBuffer, 0);

	CONTEXT->VSSetConstantBuffers(0, 1, &m_pBuffer);

	if (m_pFlareTexture)
	{
		m_pFlareTexture->SetShader(0, static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));
	}

	if (m_pLensShader)
	{
		m_pLensShader->SetShader();
	}

	m_pAlphaBlend->SetState();
	m_pNoDepth->SetState();

	CONTEXT->IASetInputLayout(nullptr);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ID3D11Buffer* pBuffer = nullptr;
	UINT iStride = 0;
	CONTEXT->IASetVertexBuffers(0, 1, &pBuffer, &iStride, &iStride);
	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->Draw(6 * static_cast<UINT>(m_vecCBuffers.size()), 0);

	m_pAlphaBlend->ResetState();
	m_pNoDepth->ResetState();
}

void CLensFlares::ShowWindow()
{
	if(ImGui::Begin("Lens Flares"))
	{
		ImGui::InputFloat3("Sun Position", &m_vSunPos.x);

		size_t iSize = m_vecFlares.size();

		for (size_t i = 0; i < iSize; i++)
		{
			ImGui::Text("Flare %d", i);
			float fOffset = m_vecFlares[i].fOffset;
			char strOffset[MAX_PATH] = {};
			sprintf_s(strOffset, "Offset %d", static_cast<int>(i));
			if (ImGui::InputFloat(strOffset, &fOffset))
			{
				m_vecFlares[i].fOffset = fOffset;
			}

			float fScale = m_vecFlares[i].fScale;
			char strScale[MAX_PATH] = {};
			sprintf_s(strScale, "Scale %d", static_cast<int>(i));
			if (ImGui::InputFloat(strScale, &fScale))
			{
				m_vecFlares[i].fScale = fScale;
			}
			Vector4 vColor = m_vecFlares[i].vColor;
			char strColor[MAX_PATH] = {};
			sprintf_s(strColor, "Color %d", static_cast<int>(i));
			if (ImGui::ColorPicker4(strColor, &vColor.x))
			{
				m_vecFlares[i].vColor = vColor;
			}
		}

		ImGui::Checkbox("In Screen", &m_bInScreen);
	}
	ImGui::End();
}
