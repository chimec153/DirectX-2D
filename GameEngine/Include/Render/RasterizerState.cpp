#include "RasterizerState.h"
#include "../Device.h"
#include "../UI/imgui/imgui.h"

CRasterizerState::CRasterizerState()
{
	m_eType = RENDER_STATE_TYPE::RASTERIZER;
}

CRasterizerState::~CRasterizerState()
{
}

void CRasterizerState::SetState()
{
	CONTEXT->RSGetState((ID3D11RasterizerState**)&m_pPrevState);

	CONTEXT->RSSetState((ID3D11RasterizerState*)m_pState);
}

void CRasterizerState::ResetState()
{
	CONTEXT->RSSetState((ID3D11RasterizerState*)m_pPrevState);

	SAFE_RELEASE(m_pPrevState);
}

bool CRasterizerState::AddRasterizerInfo(D3D11_FILL_MODE eFillMode, D3D11_CULL_MODE eCullMode, bool bClock, bool bClip,
	int iDepthBias, float fDepthBiasClamp, float fSlopeScaledDepthBias)
{
	D3D11_RASTERIZER_DESC tDesc = {};

	tDesc.FillMode = eFillMode;
	tDesc.CullMode = eCullMode;
	tDesc.FrontCounterClockwise = bClock;
	tDesc.DepthClipEnable = bClip;
	//tDesc.ScissorEnable = true;
	tDesc.DepthBias = iDepthBias;
	tDesc.DepthBiasClamp = fDepthBiasClamp;
	tDesc.SlopeScaledDepthBias = fSlopeScaledDepthBias;

	m_vecDesc.push_back(tDesc);

	return true;
}

bool CRasterizerState::CreateRasterizerState()
{
	if (FAILED(DEVICE->CreateRasterizerState(&m_vecDesc[0], (ID3D11RasterizerState * *)& m_pState)))
		return false;

	return true;
}

void CRasterizerState::ShowWindow()
{
	CRenderState::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		for (size_t i = 0; i < m_vecDesc.size(); i++)
		{
			char strAnti[MAX_PATH] = {};

			sprintf_s(strAnti, "Antialiased Line Enable %d", static_cast<int>(i));
			bool bAnti = m_vecDesc[i].AntialiasedLineEnable;
			ImGui::Checkbox(strAnti, &bAnti);
		}
	}
	ImGui::End();
}
