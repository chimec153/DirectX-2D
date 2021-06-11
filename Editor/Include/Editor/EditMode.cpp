#include "EditMode.h"
#include "Resource/ResourceManager.h"
#include "Scene/Scene.h"
#include "Component/Renderer.h"
#include "Input.h"
#include "CameraManager.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Object/Player3D.h"
#include "StartGameMode.h"
#include "Resource/GraphicShader.h"
#include "Resource/ShaderManager.h"
#include "Resource/Material.h"

std::shared_ptr<CComponent> CEditMode::m_pCam = nullptr;

CEditMode::CEditMode()
{
	m_iGameModeType = static_cast<int>(EDITOR_GAME_MODE_TYPE::EDIT);
}

CEditMode::~CEditMode()
{
}

bool CEditMode::Init()
{
	std::shared_ptr<CShader> pShader = GET_SINGLE(CShaderManager)->CreateShader<CGraphicShader>("EmvRClip");

	CGraphicShader* _pShader = static_cast<CGraphicShader*>(pShader.get());
	if (!_pShader->LoadVertexShader("StandardVS3D", TEXT("3D.fx")))
		return false;
	if (!_pShader->LoadPixelShader("EmvRClipPS", TEXT("3D.fx")))
		return false;

	_pShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pShader->CreateInputLayout())
		return false;

	pShader = GET_SINGLE(CShaderManager)->CreateShader<CGraphicShader>("EmvGClip");

	_pShader = static_cast<CGraphicShader*>(pShader.get());
	if (!_pShader->LoadVertexShader("StandardVS3D", TEXT("3D.fx")))
		return false;
	if (!_pShader->LoadPixelShader("EmvGClipPS", TEXT("3D.fx")))
		return false;

	_pShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pShader->CreateInputLayout())
		return false;

	if (!GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "ManzanitaTree3", TEXT("Obj\\matree.FBX"), MESH_PATH))
		return false;

	std::shared_ptr<CGameObject> pObj = m_pScene->CreateObject<CGameObject>("Obj", m_pScene->FindLayer(ALBEDO_LAYER));

	std::shared_ptr<CComponent> pCom = pObj->CreateComponent<CSceneComponent>("Renderer");

	pObj->SetRootComponent(pCom);

	static_cast<CSceneComponent*>(pCom.get())->SetWorldScale(1.f, 1.f, 1.f);
	//_pCom->SetWorldRotY(90.f);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("ManzanitaTree3");
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState(CULL_NONE);
	static_cast<CSceneComponent*>(pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CSceneComponent*>(pCom.get())->GetMaterial()->SetAnimation(false);
	//_pCom->SetShader("EmvGClip");
	//4938 18
	GET_SINGLE(CInput)->AddAxisKey("W", DIK_W, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("S", DIK_S, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("A", DIK_A, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("D", DIK_D, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("R", DIK_R, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("F", DIK_F, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("Q", DIK_Q, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("E", DIK_E, -1.f);

	GET_SINGLE(CInput)->AddBindAxis("W", &CEditMode::MoveZ);
	GET_SINGLE(CInput)->AddBindAxis("S", &CEditMode::MoveZ);
	GET_SINGLE(CInput)->AddBindAxis("R", &CEditMode::MoveY);
	GET_SINGLE(CInput)->AddBindAxis("F", &CEditMode::MoveY);
	GET_SINGLE(CInput)->AddBindAxis("A", &CEditMode::MoveX);
	GET_SINGLE(CInput)->AddBindAxis("D", &CEditMode::MoveX);
	GET_SINGLE(CInput)->AddBindAxis("Q", &CEditMode::RotY);
	GET_SINGLE(CInput)->AddBindAxis("E", &CEditMode::RotY);

	m_pCam = pObj->CreateComponent<CCamera>("Cam");

	CCamera* pCam = static_cast<CCamera*>(m_pCam.get());
	pCam->AddWorldPosZ(-100.f);
	GET_SINGLE(CCameraManager)->AddCam(pCam);
	GET_SINGLE(CCameraManager)->SetMainCam("Cam");

	std::shared_ptr<CGameObject> pLightObj = m_pScene->CreateObject<CGameObject>("Light", m_pScene->FindLayer(ALBEDO_LAYER));

	std::shared_ptr<CComponent> pLight = pObj->CreateComponent<CLight>("Light");

	pLightObj->SetRootComponent(pLight);

	static_cast<CLight*>(pLight.get())->AddWorldRotX(45.f);

	return true;
}

void CEditMode::MoveX(const std::string& strKey, float fScale, float fTime)
{
	static_cast<CCamera*>(m_pCam.get())->AddWorldPos(static_cast<CCamera*>(m_pCam.get())->GetWorldAxis(AXIS::AXIS_X) * fScale * fTime * 100.f);
}

void CEditMode::MoveY(const std::string& strKey, float fScale, float fTime)
{
	static_cast<CCamera*>(m_pCam.get())->AddWorldPos(static_cast<CCamera*>(m_pCam.get())->GetWorldAxis(AXIS::AXIS_Y) * fScale * fTime * 100.f);
}

void CEditMode::MoveZ(const std::string& strKey, float fScale, float fTime)
{
	static_cast<CCamera*>(m_pCam.get())->AddWorldPos(static_cast<CCamera*>(m_pCam.get())->GetWorldAxis(AXIS::AXIS_Z) * fScale * fTime * 100.f);
}

void CEditMode::RotY(const std::string& strKey, float fScale, float fTime)
{
	static_cast<CCamera*>(m_pCam.get())->AddWorldRotY(fScale * fTime * 90.f);
}
