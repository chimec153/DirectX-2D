#include "ColliderTerrain.h"
#include "Collision.h"
#include "ColliderSphere.h"
#include "ColliderRay.h"
#include "../Resource/ResourceManager.h"
#include "../RenderManager.h"
#include "../Resource/ShaderManager.h"
#include "../Render/RenderState.h"
#include "../Resource/Mesh.h"
#ifdef _DEBUG
#include "../Engine.h"
#endif

CColliderTerrain::CColliderTerrain()	:
	m_tInfo()
{
	m_eColType = COLLIDER_TYPE::TERRAIN;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_3D;
	m_iSceneComponentClassType = static_cast<int>(SCENECOMPONENT_CLASS_TYPE::COLLIDER_TERRAIN);
}

CColliderTerrain::CColliderTerrain(const CColliderTerrain& col)	:
	CCollider(col)
	, m_tInfo(col.m_tInfo)
{
}

CColliderTerrain::~CColliderTerrain()
{
}

const TERRAININFO& CColliderTerrain::GetInfo() const
{
	return m_tInfo;
}

void CColliderTerrain::SetInfo(const std::vector<Vector3>& vecPos, int iWidth, int iHeight)
{
	m_tInfo.vecPos = vecPos;
	m_tInfo.iWidth = iWidth;
	m_tInfo.iHeight = iHeight;
}

bool CColliderTerrain::Init()
{
	if (!CCollider::Init())
		return false;
#ifdef _DEBUG
	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("Terrain");
	
	m_pRasterizer = GET_SINGLE(CRenderManager)->FindState(WIRE_FRAME);
	m_pDepthStencilState = GET_SINGLE(CRenderManager)->FindState(WIRE_FRAME);
	m_pDebugShader = GET_SINGLE(CShaderManager)->FindShader(STANDARD3D_SHADER);
#endif
	return true;
}

void CColliderTerrain::Start()
{
	CCollider::Start();
}

void CColliderTerrain::Update(float fTime)
{
	CCollider::Update(fTime);

	m_tInfo.vPos = GetWorldPos();
	m_tInfo.vScale = GetWorldScale();
}

void CColliderTerrain::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);
}

void CColliderTerrain::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderTerrain::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderTerrain::Render(float fTime)
{
	CSceneComponent::Render(fTime);

#ifdef _DEBUG
	if (GET_SINGLE(CEngine)->IsColliderEnabled())
	{
		m_pDepthStencilState->SetState();
		m_pRasterizer->SetState();
		m_pDebugShader->SetShader();
		m_pDebugMesh->Render(fTime);
		m_pDepthStencilState->ResetState();
		m_pRasterizer->ResetState();
	}
#endif
}

void CColliderTerrain::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

std::shared_ptr<CComponent> CColliderTerrain::Clone()
{
	return std::shared_ptr<CComponent>(new CColliderTerrain(*this));
}

bool CColliderTerrain::Collision(CCollider* pDest)
{
	COLLIDER_TYPE eType  = pDest->GetColType();

	switch (eType)
	{
	case COLLIDER_TYPE::CT_RECT:
		break;
	case COLLIDER_TYPE::CT_SPHERE2D:
		break;
	case COLLIDER_TYPE::CT_POINT:
		break;
	case COLLIDER_TYPE::CT_OBB2D:
		break;
	case COLLIDER_TYPE::CT_PIXEL:
		break;
	case COLLIDER_TYPE::CT_LINE:
		break;
	case COLLIDER_TYPE::SPHERE:		
		return CCollision::CollisionSphereToTerrain(static_cast<CColliderSphere*>(pDest), this);
	case COLLIDER_TYPE::RAY:
		return CCollision::CollisionRayToTerrain(static_cast<CColliderRay*>(pDest), this);
	case COLLIDER_TYPE::TERRAIN:
		return false;
	case COLLIDER_TYPE::OBB:
		return false;
	}

	return false;
}

bool CColliderTerrain::CollisionMouse(const Vector2& vPos)
{
	return false;
}

void CColliderTerrain::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tInfo, sizeof(TERRAININFO), 1, pFile);
}

void CColliderTerrain::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	fread(&m_tInfo, sizeof(TERRAININFO), 1, pFile);
#ifdef _DEBUG
	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("Terrain");
	m_pRasterizer = GET_SINGLE(CRenderManager)->FindState(WIRE_FRAME);
	m_pDepthStencilState = GET_SINGLE(CRenderManager)->FindState(WIRE_FRAME);
	m_pDebugShader = GET_SINGLE(CShaderManager)->FindShader(STANDARD3D_SHADER);
#endif
}
