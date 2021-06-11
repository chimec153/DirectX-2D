#include "ShaderManager.h"
#include "Shader.h"
#include "../Device.h"
#include "GraphicShader.h"
#include "ComputeShader.h"
#include "../CameraManager.h"
#include "../Component/SceneComponent.h"

int mNumElements = 32;

DEFINITION_SINGLE(CShaderManager)

CShaderManager::CShaderManager()
{

}

CShaderManager::~CShaderManager()
{
	SAFE_RELEASE_MAP(m_mapInputLayout);

	auto iter = m_mapCBuffer.begin();
	auto iterEnd = m_mapCBuffer.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second->pBuffer);
	}

	SAFE_DELETE_MAP(m_mapCBuffer);
}

bool CShaderManager::Init()
{
	std::shared_ptr<class CShader> pShader = CreateShader<CGraphicShader>(STANDARD2D_SHADER);

	if (!LoadVertexShader(STANDARD2D_SHADER, "GetOutputVS",
		TEXT("VertexShader.fx")))
		return false;

	if (!LoadPixelShader(STANDARD2D_SHADER, "GetOutputPS",
		TEXT("VertexShader.fx")))
		return false;

	pShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		return false;
	}

	pShader = CreateShader<CGraphicShader>("Sprite2D");

	if (!LoadVertexShader("Sprite2D", "Standard2DSpriteVS",
		TEXT("VertexShader.fx")))
		return false;

	if (!LoadPixelShader("Sprite2D", "Standard2DSpritePS",
		TEXT("VertexShader.fx")))
		return false;

	pShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		return false;
	}

	pShader = CreateShader<CGraphicShader>("Collider2D");

	if (!LoadVertexShader("Collider2D", "Collider_VS",
		TEXT("Collision.fx")))
		return false;

	if (!LoadPixelShader("Collider2D", "Collider_PS",
		TEXT("Collision.fx")))
		return false;

	pShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		return false;
	}

	pShader = CreateShader<CGraphicShader>("Tex2D");

	if (!LoadVertexShader("Tex2D", "TexVS",
		TEXT("VertexShader.fx")))
		return false;

	if (!LoadPixelShader("Tex2D", "TexPS",
		TEXT("VertexShader.fx")))
		return false;

	pShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		return false;
	}

	pShader = CreateShader<CGraphicShader>("UI");

	if (!LoadVertexShader("UI", "VS",
		TEXT("UI.fx")))
		return false;

	if (!LoadPixelShader("UI", "PS",
		TEXT("UI.fx")))
		return false;

	pShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		return false;
	}

	pShader = CreateShader<CGraphicShader>("AlphaTextureUI");

	if (!LoadVertexShader("AlphaTextureUI", "VS",
		TEXT("UI.fx")))
		return false;

	if (!LoadPixelShader("AlphaTextureUI", "AlphaTexturePS",
		TEXT("UI.fx")))
		return false;

	pShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		return false;
	}

	std::shared_ptr<class CShader> pBarSdr = CreateShader<CGraphicShader>("Bar");

	if (!LoadVertexShader("Bar", "BarVS", TEXT("UI.fx")))
	{
		return false;
	}

	if (!LoadPixelShader("Bar", "BarPS", TEXT("UI.fx")))
	{
		return false;
	}

	pBarSdr->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pBarSdr->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pBarSdr->AddInputDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pBarSdr->CreateInputLayout())
	{
		return false;
	}

	std::shared_ptr<class CShader> pInst = CreateShader<CGraphicShader>("Inst");

	if (!LoadVertexShader("Inst", "Inst_VS", TEXT("Instance.fx")))
	{
		return false;
	}

	if (!LoadPixelShader("Inst", "Inst_PS", TEXT("Instance.fx")))
	{
		return false;
	}

	pInst->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInst->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInst->AddInputDesc("WORLD", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst->AddInputDesc("WORLD", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst->AddInputDesc("WORLD", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst->AddInputDesc("WORLD", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst->AddInputDesc("PIVOT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 1, 12,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst->AddInputDesc("FRAME", 0,
		DXGI_FORMAT_R32_SINT, 1, 4,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst->AddInputDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst->AddInputDesc("SIZE", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 1, 12,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);

	if (!pInst->CreateInputLayout())
	{
		return false;
	}

	std::shared_ptr<class CShader> pAnim2DInst = CreateShader<CGraphicShader>("Anim2DInst");

	if (!LoadVertexShader("Anim2DInst", "Inst_Anim2D_VS", TEXT("Instance.fx")))
	{
		return false;
	}

	if (!LoadPixelShader("Anim2DInst", "Inst_Anim2D_PS", TEXT("Instance.fx")))
	{
		return false;
	}

	pAnim2DInst->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pAnim2DInst->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pAnim2DInst->AddInputDesc("WORLD", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pAnim2DInst->AddInputDesc("WORLD", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pAnim2DInst->AddInputDesc("WORLD", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pAnim2DInst->AddInputDesc("WORLD", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pAnim2DInst->AddInputDesc("PIVOT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 1, 12,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pAnim2DInst->AddInputDesc("SIZE", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 1, 12,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pAnim2DInst->AddInputDesc("TEXSIZE", 0,
		DXGI_FORMAT_R32G32_FLOAT, 1, 8,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pAnim2DInst->AddInputDesc("START", 0,
		DXGI_FORMAT_R32G32_FLOAT, 1, 8,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pAnim2DInst->AddInputDesc("END", 0,
		DXGI_FORMAT_R32G32_FLOAT, 1, 8,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);

	if (!pAnim2DInst->CreateInputLayout())
	{
		return false;
	}

	std::shared_ptr<class CShader> pColliderInst = CreateShader<CGraphicShader>("ColliderInst");

	if (!LoadVertexShader("ColliderInst", "Inst_Collider_VS", TEXT("Instance.fx")))
	{
		return false;
	}

	if (!LoadPixelShader("ColliderInst", "Inst_Collider_PS", TEXT("Instance.fx")))
	{
		return false;
	}

	pColliderInst->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pColliderInst->AddInputDesc("WORLD", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pColliderInst->AddInputDesc("WORLD", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pColliderInst->AddInputDesc("WORLD", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pColliderInst->AddInputDesc("WORLD", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pColliderInst->AddInputDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pColliderInst->AddInputDesc("PIVOT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 1, 12,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pColliderInst->AddInputDesc("SIZE", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 1, 12,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);

	if (!pColliderInst->CreateInputLayout())
	{
		return false;
	}

	std::shared_ptr<class CShader> p3DShader = CreateShader<CGraphicShader>(STANDARD3D_SHADER);

	if (!LoadVertexShader(STANDARD3D_SHADER, "StandardVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader(STANDARD3D_SHADER, "StandardPS3D", TEXT("3D.fx")))
		return false;

	p3DShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	p3DShader->CreateInputLayout();

	p3DShader = CreateShader<CGraphicShader>("Inst3D");

	if (!LoadVertexShader("Inst3D", "InstVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("Inst3D", "StandardPS3D", TEXT("3D.fx")))
		return false;

	p3DShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("WVP", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WVP", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WVP", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WVP", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WV", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WV", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WV", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WV", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("SHADOW", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("SHADOW", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("SHADOW", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("SHADOW", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WORLD", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WORLD", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WORLD", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WORLD", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);

	p3DShader->CreateInputLayout();

	p3DShader = CreateShader<CGraphicShader>("InstSpecR");

	if (!LoadVertexShader("InstSpecR", "InstVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("InstSpecR", "SpecRPS", TEXT("3D.fx")))
		return false;

	p3DShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("WVP", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WVP", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WVP", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WVP", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WV", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WV", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WV", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WV", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("SHADOW", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("SHADOW", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("SHADOW", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("SHADOW", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WORLD", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WORLD", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WORLD", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WORLD", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);

	p3DShader->CreateInputLayout();

	p3DShader = CreateShader<CGraphicShader>("ColliderInst3D");

	if (!LoadVertexShader("ColliderInst3D", "InstColliderVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("ColliderInst3D", "ColliderInstPS3D", TEXT("3D.fx")))
		return false;

	p3DShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("WVP", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WVP", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WVP", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WVP", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WV", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WV", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WV", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WV", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("SHADOW", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("SHADOW", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("SHADOW", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("SHADOW", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("Color", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WORLD", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WORLD", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WORLD", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	p3DShader->AddInputDesc("WORLD", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);

	p3DShader->CreateInputLayout();

	std::shared_ptr<class CShader> pOutLineShader = CreateShader<CGraphicShader>("OutLineShader");

	if (!LoadVertexShader("OutLineShader", "StandardVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("OutLineShader", "OutLinePS", TEXT("3D.fx")))
		return false;

	pOutLineShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOutLineShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOutLineShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOutLineShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOutLineShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOutLineShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOutLineShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pOutLineShader->CreateInputLayout())
		return false;

	pOutLineShader = CreateShader<CGraphicShader>("OutLineAlphaShader");

	if (!LoadVertexShader("OutLineAlphaShader", "StandardVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("OutLineAlphaShader", "OutLineAlphaPS", TEXT("3D.fx")))
		return false;

	pOutLineShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOutLineShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOutLineShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOutLineShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOutLineShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOutLineShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOutLineShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pOutLineShader->CreateInputLayout())
		return false;

	std::shared_ptr<class CShader> pTree = CreateShader<CGraphicShader>("TreeShader");

	if (!LoadVertexShader("TreeShader", "StandardVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("TreeShader", "TreePS", TEXT("3D.fx")))
		return false;

	pTree->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTree->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTree->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTree->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTree->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTree->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTree->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	pTree->CreateInputLayout();

	std::shared_ptr<class CShader> pReflect = CreateShader<CGraphicShader>("Reflect");

	if (!LoadVertexShader("Reflect", "StandardVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("Reflect", "Re3D", TEXT("3D.fx")))
		return false;

	pReflect->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pReflect->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pReflect->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pReflect->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pReflect->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pReflect->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pReflect->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	pReflect->CreateInputLayout();

	std::shared_ptr<class CShader> pDebugCollider = CreateShader<CGraphicShader>("DebugCollider");

	if (!LoadVertexShader("DebugCollider", "StandardVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("DebugCollider", "NormalPS", TEXT("3D.fx")))
		return false;

	pDebugCollider->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pDebugCollider->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pDebugCollider->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pDebugCollider->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pDebugCollider->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pDebugCollider->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pDebugCollider->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pDebugCollider->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pPaper = CreateShader<CGraphicShader>("PaperBurn2D");

	CGraphicShader* pPaperShader = static_cast<CGraphicShader*>(pPaper.get());

	if (!pPaperShader->LoadVertexShader("StandardVS3D", TEXT("3D.fx")))
		return false;

	if (!pPaperShader->LoadPixelShader("PaperBurnPS", TEXT("VertexShader.fx")))
		return false;

	pPaperShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pPaperShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pPaperShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pPaperShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pPaperShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pPaperShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pPaperShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pPaperShader->CreateInputLayout())
	{
		return false;
	}

	std::shared_ptr<CShader> pFullShader = CreateShader<CGraphicShader>("FullScreenShader");

	CGraphicShader* _pFullShader = static_cast<CGraphicShader*>(pFullShader.get());

	if (!_pFullShader->LoadVertexShader("FullVS", TEXT("VertexShader.fx")))
	{
		return false;
	}
	if (!_pFullShader->LoadPixelShader("FXAAPS", TEXT("fxaa.fx")))
	{
		return false;
	}
	_pFullShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pFullShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	if (!_pFullShader->CreateInputLayout())
	{
		return false;
	}

	pFullShader = CreateShader<CGraphicShader>("FullRadialBlurShader");

	_pFullShader = static_cast<CGraphicShader*>(pFullShader.get());

	if (!_pFullShader->LoadVertexShader("FullVS", TEXT("VertexShader.fx")))
	{
		return false;
	}
	if (!_pFullShader->LoadPixelShader("FullRadialBlurPS", TEXT("VertexShader.fx")))
	{
		return false;
	}
	_pFullShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pFullShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	if (!_pFullShader->CreateInputLayout())
	{
		return false;
	}

	std::shared_ptr<CShader> pDebugShader = CreateShader<CGraphicShader>("DebugShader");

	CGraphicShader* _pDebugShader = (CGraphicShader*)pDebugShader.get();

	if (!_pDebugShader->LoadVertexShader("DebugVS", TEXT("Debug.fx")))
	{
		return false;
	}
	if (!_pDebugShader->LoadPixelShader("DebugPS", TEXT("Debug.fx")))
	{
		return false;
	}
	_pDebugShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pDebugShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	if (!_pDebugShader->CreateInputLayout())
	{
		return false;
	}

	std::shared_ptr<CShader> pEquipShader = CreateShader<CGraphicShader>("EquipShader");

	if (!static_cast<CGraphicShader*>(pEquipShader.get())->
		LoadVertexShader("EquipVS", TEXT("VertexShader.fx")))
	{
		return false;
	}
	if (!static_cast<CGraphicShader*>(pEquipShader.get())->
		LoadPixelShader("EquipPS", TEXT("VertexShader.fx")))
	{
		return false;
	}/*
	_pDebugShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pDebugShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	if (!_pDebugShader->CreateInputLayout())
	{
		return false;
	}*/

	std::shared_ptr<CShader> pLensFlareShader = CreateShader<CGraphicShader>("LensFlareShader");

	CGraphicShader* _pLensFlareShader = (CGraphicShader*)pLensFlareShader.get();

	if (!_pLensFlareShader->LoadVertexShader("VS", TEXT("PostProcess.fx")))
	{
		return false;
	}
	if (!_pLensFlareShader->LoadPixelShader("PS", TEXT("PostProcess.fx")))
	{
		return false;
	}
	/*_pLensFlareShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	if (!_pLensFlareShader->CreateInputLayout())
	{
		return false;
	}*/

	std::shared_ptr<CShader> pDistortShader = CreateShader<CGraphicShader>("Distortion2D");

	CGraphicShader* _pDistortShader = static_cast<CGraphicShader*>(pDistortShader.get());

	if (!_pDistortShader->LoadVertexShader("DistortionVS2D", TEXT("Distortion.fx")))
		return false;
	if (!_pDistortShader->LoadPixelShader("DistortionPS2D", TEXT("Distortion.fx")))
		return false;
	_pDistortShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pDistortShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pDistortShader->AddInputDesc("PROJECTION", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	if (!_pDistortShader->CreateInputLayout())
		return false;

	pDistortShader = CreateShader<CGraphicShader>("Distortion");

	_pDistortShader = static_cast<CGraphicShader*>(pDistortShader.get());

	if (!_pDistortShader->LoadVertexShader("DistortionVS", TEXT("Distortion.fx")))
		return false;
	if (!_pDistortShader->LoadPixelShader("DistortionPS", TEXT("Distortion.fx")))
		return false;
	_pDistortShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pDistortShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pDistortShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pDistortShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pDistortShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pDistortShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pDistortShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	if (!_pDistortShader->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pUpdateShader = CreateShader<CComputeShader>("ParicleUpdateShader");
	CComputeShader* _pUpdateShader = (CComputeShader*)pUpdateShader.get();
	_pUpdateShader->LoadComputeShader("CS", TEXT("Particle.fx"));

	pUpdateShader = CreateShader<CComputeShader>("CharUpdateShader");
	_pUpdateShader = (CComputeShader*)pUpdateShader.get();
	_pUpdateShader->LoadComputeShader("CharCS", TEXT("Particle.fx"));

	pUpdateShader = CreateShader<CComputeShader>("RockUpdateShader");
	_pUpdateShader = (CComputeShader*)pUpdateShader.get();
	if (!_pUpdateShader->LoadComputeShader("RockCS", TEXT("Particle.fx")))
		return false;

	std::shared_ptr<CShader> pParticleShader = CreateShader<CGraphicShader>("ParticleShader");
	CGraphicShader* _pParticleShader = (CGraphicShader*)pParticleShader.get();
	if (!_pParticleShader->LoadVertexShader("VS", TEXT("Particle.fx")))
		return false;
	if (!_pParticleShader->LoadGeometryShader("GS", TEXT("Particle.fx")))
		return false;
	if (!_pParticleShader->LoadPixelShader("PS", TEXT("Particle.fx")))
		return false;

	_pParticleShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pParticleShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pParticleShader->CreateInputLayout())
	{
		return false;
	}

	pParticleShader = CreateShader<CGraphicShader>("ParticleLight");
	_pParticleShader = (CGraphicShader*)pParticleShader.get();
	if (!_pParticleShader->LoadVertexShader("VS", TEXT("Particle.fx")))
		return false;
	if (!_pParticleShader->LoadGeometryShader("GS", TEXT("Particle.fx")))
		return false;
	if (!_pParticleShader->LoadPixelShader("LightPS", TEXT("Particle.fx")))
		return false;

	_pParticleShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pParticleShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pParticleShader->CreateInputLayout())
	{
		return false;
	}

	pParticleShader = CreateShader<CGraphicShader>("ParticleBlackClip");
	_pParticleShader = (CGraphicShader*)pParticleShader.get();
	if (!_pParticleShader->LoadVertexShader("VS", TEXT("Particle.fx")))
		return false;
	if (!_pParticleShader->LoadGeometryShader("GS", TEXT("Particle.fx")))
		return false;
	if (!_pParticleShader->LoadPixelShader("BlackClipPS", TEXT("Particle.fx")))
		return false;

	_pParticleShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pParticleShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pParticleShader->CreateInputLayout())
	{
		return false;
	}

	pParticleShader = CreateShader<CGraphicShader>("CharShader");
	_pParticleShader = (CGraphicShader*)pParticleShader.get();
	if (!_pParticleShader->LoadVertexShader("VS", TEXT("Particle.fx")))
		return false;
	if (!_pParticleShader->LoadGeometryShader("CharGS", TEXT("Particle.fx")))
		return false;
	if (!_pParticleShader->LoadPixelShader("PS", TEXT("Particle.fx")))
		return false;

	_pParticleShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pParticleShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pParticleShader->CreateInputLayout())
	{
		return false;
	}

	std::shared_ptr<CShader> pDecalShader = CreateShader<CGraphicShader>("Decal");

	CGraphicShader* _pDecalShader = static_cast<CGraphicShader*>(pDecalShader.get());

	if (!_pDecalShader->LoadVertexShader("DecalVS", TEXT("Decal.fx")))
		return false;
	if (!_pDecalShader->LoadPixelShader("DecalPS", TEXT("Decal.fx")))
		return false;

	_pDecalShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pDecalShader->CreateInputLayout())
		return false;

	pDecalShader = CreateShader<CGraphicShader>("DecalAlphaMask");

	_pDecalShader = static_cast<CGraphicShader*>(pDecalShader.get());

	if (!_pDecalShader->LoadVertexShader("DecalVS", TEXT("Decal.fx")))
		return false;
	if (!_pDecalShader->LoadPixelShader("DecalAlphaMaskPS", TEXT("Decal.fx")))
		return false;

	_pDecalShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pDecalShader->CreateInputLayout())
		return false;

	pDecalShader = CreateShader<CGraphicShader>("DecalSphere");

	_pDecalShader = static_cast<CGraphicShader*>(pDecalShader.get());

	if (!_pDecalShader->LoadVertexShader("DecalVS", TEXT("Decal.fx")))
		return false;
	if (!_pDecalShader->LoadPixelShader("DecalSpherePS", TEXT("Decal.fx")))
		return false;

	pDecalShader = CreateShader<CGraphicShader>("DecalRadius");

	_pDecalShader = static_cast<CGraphicShader*>(pDecalShader.get());

	if (!_pDecalShader->LoadVertexShader("DecalVS", TEXT("Decal.fx")))
		return false;
	if (!_pDecalShader->LoadPixelShader("DecalRadiusPS", TEXT("Decal.fx")))
		return false;

	_pDecalShader->AddInputDesc("POSITION", 0, 
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, 
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pDecalShader->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pVert = CreateShader<CComputeShader>("Vert");

	static_cast<CComputeShader*>(pVert.get())->LoadComputeShader("VertBlurCS", TEXT("Compute.fx"));

	std::shared_ptr<CShader> pHori = CreateShader<CComputeShader>("Horz");

	static_cast<CComputeShader*>(pHori.get())->LoadComputeShader("HorzBlurCS", TEXT("Compute.fx"));

	std::shared_ptr<CShader> pCray = CreateShader<CComputeShader>("Gray");

	static_cast<CComputeShader*>(pCray.get())->LoadComputeShader("GrayCS", TEXT("Compute.fx"));

	std::shared_ptr<CShader> pSepia = CreateShader<CComputeShader>("Sepia");

	static_cast<CComputeShader*>(pSepia.get())->LoadComputeShader("SepiaCS", TEXT("Compute.fx"));

	std::shared_ptr<CShader> pAmbShader = CreateShader<CGraphicShader>("Amb");

	CGraphicShader* _pAmbShader = static_cast<CGraphicShader*>(pAmbShader.get());

	if (!_pAmbShader->LoadVertexShader("RenderSceneVS", TEXT("Forward.fx")))
		return false;

	if (!_pAmbShader->LoadPixelShader("PS", TEXT("Forward.fx")))
		return false;

	_pAmbShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pAmbShader->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pAmbShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	if (!_pAmbShader->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pTessTest = CreateShader<CGraphicShader>("Tess");

	CGraphicShader* _pTess = static_cast<CGraphicShader*>(pTessTest.get());

	if (!_pTess->LoadVertexShader("VS", TEXT("Tess.fx")))
		return false;
	if (!_pTess->LoadHullShader("HS", TEXT("Tess.fx")))
		return false;
	if (!_pTess->LoadDomainShader("DS", TEXT("Tess.fx")))
		return false;
	if (!_pTess->LoadPixelShader("PS", TEXT("Tess.fx")))
		return false;

	_pTess->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pTess->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pAnim = CreateShader<CComputeShader>("Anim");

	CComputeShader* _pAnim = static_cast<CComputeShader*>(pAnim.get());

	if (!_pAnim->LoadComputeShader("CS", TEXT("Animation.fx")))
		return false;
	std::shared_ptr<CShader> pCube = CreateShader<CGraphicShader>("Cube");

	CGraphicShader* _pCube = static_cast<CGraphicShader*>(pCube.get());
	if (!_pCube->LoadVertexShader("VS", TEXT("CubeMap.fx")))
		return false;

	if (!_pCube->LoadPixelShader("PS", TEXT("CubeMap.fx")))
		return false;

	_pCube->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pCube->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pCube->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pCube->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pCube->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pCube->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pCube->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pCube->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pCube2 = CreateShader<CGraphicShader>("Cube2");

	CGraphicShader* _pCube2 = static_cast<CGraphicShader*>(pCube2.get());
	if (!_pCube2->LoadVertexShader("VS2", TEXT("CubeMap.fx")))
		return false;
	if (!_pCube2->LoadPixelShader("PS2", TEXT("CubeMap.fx")))
		return false;
	_pCube2->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	if (!_pCube2->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pLightAcc = CreateShader<CGraphicShader>("LightAcc");

	CGraphicShader* _pLightAcc = static_cast<CGraphicShader*>(pLightAcc.get());

	if (!_pLightAcc->LoadVertexShader("LightAccVS", TEXT("MRT.fx")))
		return false;
	if (!_pLightAcc->LoadPixelShader("LightAccPS", TEXT("MRT.fx")))
		return false;

	_pLightAcc->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pLightAcc->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pLightCombine = CreateShader<CGraphicShader>("LightCombine");

	CGraphicShader* _pLightCombine = static_cast<CGraphicShader*>(pLightCombine.get());

	if (!_pLightCombine->LoadVertexShader("LightAccVS", TEXT("MRT.fx")))
		return false;
	if (!_pLightCombine->LoadPixelShader("LightCombinePS", TEXT("MRT.fx")))
		return false;

	_pLightCombine->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pLightCombine->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pTerrain = CreateShader<CGraphicShader>("Terrain");

	CGraphicShader* _pTerrain = static_cast<CGraphicShader*>(pTerrain.get());

	if (!_pTerrain->LoadVertexShader("VS", TEXT("terrain.fx")))
		return false;
	if (!_pTerrain->LoadPixelShader("PS", TEXT("terrain.fx")))
		return false;

	_pTerrain->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrain->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrain->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrain->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrain->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrain->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrain->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pTerrain->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pTerrainArray = CreateShader<CGraphicShader>("TerrainArray");

	CGraphicShader* _pTerrainArray = static_cast<CGraphicShader*>(pTerrainArray.get());

	if (!_pTerrainArray->LoadVertexShader("VS", TEXT("terrain.fx")))
		return false;
	if (!_pTerrainArray->LoadPixelShader("ArrayPS", TEXT("terrain.fx")))
		return false;

	_pTerrainArray->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pTerrainArray->CreateInputLayout())
		return false;

	pTerrainArray = CreateShader<CGraphicShader>("TerrainTess");

	_pTerrainArray = static_cast<CGraphicShader*>(pTerrainArray.get());

	if (!_pTerrainArray->LoadVertexShader("TerrainVS", TEXT("terrain.fx")))
		return false;
	if (!_pTerrainArray->LoadHullShader("HS", TEXT("terrain.fx")))
		return false;
	if (!_pTerrainArray->LoadDomainShader("DS", TEXT("terrain.fx")))
		return false;
	if (!_pTerrainArray->LoadPixelShader("TerrainPS", TEXT("terrain.fx")))
		return false;

	_pTerrainArray->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pTerrainArray->AddInputDesc("BOUND", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pTerrainArray->CreateInputLayout())
		return false;

	p3DShader = CreateShader<CGraphicShader>("EmvGClipAlpha");

	if (!LoadVertexShader("EmvGClipAlpha", "StandardVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("EmvGClipAlpha", "EmvGClipAlphaPS", TEXT("3D.fx")))
		return false;

	p3DShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	p3DShader->CreateInputLayout();

	p3DShader = CreateShader<CGraphicShader>("Alpha");

	if (!LoadVertexShader("Alpha", "StandardVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("Alpha", "AlphaPS", TEXT("3D.fx")))
		return false;

	p3DShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	p3DShader->CreateInputLayout();

	p3DShader = CreateShader<CGraphicShader>("LimLightAlpha");

	if (!LoadVertexShader("LimLightAlpha", "StandardVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("LimLightAlpha", "LimLightPS", TEXT("3D.fx")))
		return false;

	p3DShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	p3DShader->CreateInputLayout();

	p3DShader = CreateShader<CGraphicShader>("NoDepthAlpha");

	if (!LoadVertexShader("NoDepthAlpha", "NoDepthAlphaVS", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("NoDepthAlpha", "NoDepthAlphaPS", TEXT("3D.fx")))
		return false;

	p3DShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	p3DShader->CreateInputLayout();

	p3DShader = CreateShader<CGraphicShader>("EmvRClipPaper");

	if (!LoadVertexShader("EmvRClipPaper", "StandardVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("EmvRClipPaper", "EmvRClipPaperPS", TEXT("3D.fx")))
		return false;

	p3DShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	p3DShader->CreateInputLayout();

	p3DShader = CreateShader<CGraphicShader>("EmvRClipAlpha");

	if (!LoadVertexShader("EmvRClipAlpha", "StandardVS3D", TEXT("3D.fx")))
		return false;

	if (!LoadPixelShader("EmvRClipAlpha", "EmvRClipAlphaPS", TEXT("3D.fx")))
		return false;

	p3DShader->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3DShader->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	p3DShader->CreateInputLayout();

	std::shared_ptr<CShader> pShadowShader = CreateShader<CGraphicShader>("Shadow");

	if (!LoadVertexShader("Shadow", "VS", TEXT("Shadow.fx")))
		return false;
	/*if (!LoadPixelShader("Shadow", "PS", TEXT("Shadow.fx")))
		return false;*/

	pShadowShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShadowShader->CreateInputLayout())
		return false;

	pShadowShader = CreateShader<CGraphicShader>("ShadowAlphaClipShader");

	if (!LoadVertexShader("ShadowAlphaClipShader", "VS", TEXT("Shadow.fx")))
		return false;
	if (!LoadPixelShader("ShadowAlphaClipShader", "PS", TEXT("Shadow.fx")))
		return false;

	pShadowShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShadowShader->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pInstShadow = CreateShader<CGraphicShader>("InstShadow");

	static_cast<CGraphicShader*>(pInstShadow.get())->LoadVertexShader("InstVS", L"Shadow.fx");

	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,	D3D11_INPUT_PER_VERTEX_DATA, 0);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,D3D11_INPUT_PER_VERTEX_DATA, 0);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("BLENDINDICES", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,D3D11_INPUT_PER_VERTEX_DATA, 0);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("WVP", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("WVP", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("WVP", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("WVP", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("WV", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("WV", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("WV", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("WV", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("SHADOW", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("SHADOW", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("SHADOW", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("SHADOW", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("WORLD", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("WORLD", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("WORLD", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	static_cast<CGraphicShader*>(pInstShadow.get())->AddInputDesc("WORLD", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);

	if (!pInstShadow->CreateInputLayout())
		return false;

	pShadowShader = CreateShader<CGraphicShader>("PointShadow");

	if (!LoadVertexShader("PointShadow", "VS_CubeMap", TEXT("Shadow.fx")))
		return false;
	if (!LoadGeometryShader("PointShadow", "GS_CubeMap", TEXT("Shadow.fx")))
		return false;

	pShadowShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShadowShader->CreateInputLayout())
		return false;

	pShadowShader = CreateShader<CGraphicShader>("PointShadowInst");

	if (!LoadVertexShader("PointShadowInst", "VS_CubeMap_Inst", TEXT("Shadow.fx")))
		return false;
	if (!LoadGeometryShader("PointShadowInst", "GS_CubeMap", TEXT("Shadow.fx")))
		return false;

	pShadowShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadowShader->AddInputDesc("WV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("WV", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("WV", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("WV", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("SHADOW", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("SHADOW", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("SHADOW", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("SHADOW", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("WORLD", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("WORLD", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("WORLD", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShadowShader->AddInputDesc("WORLD", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);

	if (!pShadowShader->CreateInputLayout())
		return false;

	CreateCBuffer("Transform", sizeof(TransformCBuffer), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX 
		| (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY 
		| static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_HULL) 
		| static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
	| static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));
	CreateCBuffer("Material", sizeof(ShaderCBuffer), 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX 
		| (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	CreateCBuffer("Sprite", sizeof(AniCBuffer), 2);
	CreateCBuffer("PaperBurn", sizeof(PaperBurnCBuffer), 4);
	CreateCBuffer("Distortion", sizeof(DISTORTCBUFFER), 5);
	CreateCBuffer("Global", (int)sizeof(GLOBALCBUFFER), 6,
		(int)SHADER_CBUFFER_TYPE::ALL);
	CreateCBuffer("Light", static_cast<int>(sizeof(LIGHTCBUFFER)), 7,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);
	CreateCBuffer("Particle", (int)sizeof(PARTICLECBUFFER), 8,
		(int)SHADER_CBUFFER_TYPE::ALL);
	CreateCBuffer("Bone", sizeof(ANIMATIONCBUFFER), 9,
		static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX) 
		| static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE));
	CreateCBuffer("Bar", sizeof(BARCBUFFER), 10,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	CreateCBuffer("Terrain", sizeof(TERRAINCBUFFER), 11, 
		static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX) |
		static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL) |
		static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_HULL));
	CreateCBuffer("Decal", sizeof(DECALCBUFFER), 3);
	CreateCBuffer("DebugWVP", sizeof(Matrix), 12);

	HEMICONSTANTS tConst = {};

	tConst.vAmbientDown = Vector3(0.4f, 0.3f, 0.3f);
	tConst.vAmbientRange = Vector3(0.7f, 0.7f, 0.8f);

	CreateCBuffer("Hemi", 32, 13, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	UpdateCBuffer("Hemi", &tConst);

	return true;
}

void CShaderManager::Update(float fTime)
{
	//std::shared_ptr<class CComponent> pCom = GET_SINGLE(CCameraManager)->GetMainCam();
	//Vector3 vCamPos = static_cast<CSceneComponent*>(pCom.get())->GetWorldPos();

	//UpdateCBuffer("Camera", &vCamPos);
}


bool CShaderManager::AddInputDesc(const std::string& strName, const char* pSemanticName, UINT iSemanticIndex, 
	DXGI_FORMAT eFmt, UINT iInputSlot, UINT iAlignedByteOffset, 
	D3D11_INPUT_CLASSIFICATION eInputSlotClass, UINT iInstanceDataStepRate)
{
	std::shared_ptr<class CShader> pShader = FindShader(strName);

	if (!pShader)
		return false;

	pShader->AddInputDesc(pSemanticName, iSemanticIndex, eFmt, 
		iInputSlot, iAlignedByteOffset, eInputSlotClass, iInstanceDataStepRate);

	return true;
}

bool CShaderManager::CreateInputLayout(const std::string& strName)
{
	std::shared_ptr<class CShader> pShader = FindShader(strName);

	if (!pShader)
		return false;

	bool bResult = pShader->CreateInputLayout();

	return bResult;
}

std::shared_ptr<class CShader> CShaderManager::FindShader(const std::string& strName)
{
	std::unordered_map<std::string, std::shared_ptr<class CShader>>::iterator iter = m_mapShader.find(strName);

	if (iter == m_mapShader.end())
		return nullptr;

	return iter->second;
}

ID3D11InputLayout* CShaderManager::FindInputLayout(const std::string& strName)
{
	std::unordered_map<std::string, ID3D11InputLayout*>::iterator iter = m_mapInputLayout.find(strName);

	if (iter == m_mapInputLayout.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CShaderManager::CreateCBuffer(const std::string& strTag, int iSize, int iRegister, int iType)
{
	PCBuffer pBuffer = FindCBuffer(strTag);

	if (pBuffer)
		return false;

	pBuffer = new CBuffer;

	pBuffer->iSize = iSize;
	pBuffer->iRegister = iRegister;
	pBuffer->iType = iType;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tDesc.ByteWidth = iSize;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &pBuffer->pBuffer)))
	{
		delete pBuffer;
		return false;
	}		

	m_mapCBuffer.insert(std::make_pair(strTag, pBuffer));

	return true;
}

bool CShaderManager::UpdateCBuffer(const std::string& strTag, void* pData)
{
	PCBuffer pBuffer = FindCBuffer(strTag);

	if (!pBuffer)
		return false;

	D3D11_MAPPED_SUBRESOURCE tMap = {};
	GET_SINGLE(CDevice)->GetThread()->Enter();
	CONTEXT->Map(pBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);
	GET_SINGLE(CDevice)->GetThread()->Leave();

	memcpy(tMap.pData, pData, pBuffer->iSize);

	GET_SINGLE(CDevice)->GetThread()->Enter();
	CONTEXT->Unmap(pBuffer->pBuffer, 0);
	GET_SINGLE(CDevice)->GetThread()->Leave();

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
	{
		GET_SINGLE(CDevice)->GetThread()->Enter();
		CONTEXT->VSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

		GET_SINGLE(CDevice)->GetThread()->Leave();
	}

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
	{
		GET_SINGLE(CDevice)->GetThread()->Enter();
		CONTEXT->PSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);
		GET_SINGLE(CDevice)->GetThread()->Leave();
	}


	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL)
	{
		GET_SINGLE(CDevice)->GetThread()->Enter();
		CONTEXT->HSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);
		GET_SINGLE(CDevice)->GetThread()->Leave();
	}


	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
	{
		GET_SINGLE(CDevice)->GetThread()->Enter();
		CONTEXT->DSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);
		GET_SINGLE(CDevice)->GetThread()->Leave();
	}


	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY)
	{
		GET_SINGLE(CDevice)->GetThread()->Enter();
		CONTEXT->GSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);
		GET_SINGLE(CDevice)->GetThread()->Leave();
	}


	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
	{
		GET_SINGLE(CDevice)->GetThread()->Enter();
		CONTEXT->CSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);
		GET_SINGLE(CDevice)->GetThread()->Leave();
	}

	return true;
}

PCBuffer CShaderManager::FindCBuffer(const std::string& strTag)
{
	std::unordered_map<std::string, PCBuffer>::iterator iter = m_mapCBuffer.find(strTag);

	if (iter == m_mapCBuffer.end())
		return nullptr;

	return iter->second;
}

bool CShaderManager::LoadVertexShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	std::shared_ptr<class CShader> pShader = FindShader(strName);

	if (!pShader)
		return false;

	if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		return false;
	}

	if (!((CGraphicShader*)pShader.get())->LoadVertexShader(pEntryName, pFileName, strPathName))
	{
		return false;
	}

	return true;
}

bool CShaderManager::LoadPixelShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	std::shared_ptr<class CShader> pShader = FindShader(strName);

	if (!pShader)
		return false;

	if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		return false;
	}

	if (!((CGraphicShader*)pShader.get())->LoadPixelShader(pEntryName, pFileName, strPathName))
	{
		return false;
	}
	
	return true;
}

bool CShaderManager::LoadHullShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	std::shared_ptr<class CShader> pShader = FindShader(strName);

	if (!pShader)
		return false;

	if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		return false;
	}

	if (!((CGraphicShader*)pShader.get())->LoadHullShader(pEntryName, pFileName, strPathName))
	{
		return false;
	}
	return true;
}

bool CShaderManager::LoadGeometryShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	std::shared_ptr<class CShader> pShader = FindShader(strName);

	if (!pShader)
		return false;

	if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		return false;
	}

	if (!((CGraphicShader*)pShader.get())->LoadGeometryShader(pEntryName, pFileName, strPathName))
	{
		return false;
	}

	return true;
}

bool CShaderManager::LoadDomainShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	std::shared_ptr<class CShader> pShader = FindShader(strName);

	if (!pShader)
		return false;

	if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		return false;
	}

	if (!((CGraphicShader*)pShader.get())->LoadDomainShader(pEntryName, pFileName, strPathName))
	{
		return false;
	}

	return true;
}

bool CShaderManager::LoadComputeShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strPathName)
{
	std::shared_ptr<class CShader> pShader = FindShader(strName);

	if (!pShader)
		return false;

	if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		return false;
	}

	if (!((CGraphicShader*)pShader.get())->LoadComputeShader(pEntryName, pFileName, strPathName))
	{
		return false;
	}

	return true;
}