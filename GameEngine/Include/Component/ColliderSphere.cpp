#include "ColliderSphere.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh2D.h"
#include "../RenderManager.h"
#include "../Render/RenderState.h"
#include "Collision.h"
#include "ColliderRay.h"
#include "ColliderTerrain.h"
#include "ColliderOBB.h"
#include "Transform.h"
#include "ColliderCapsule.h"
#ifdef _DEBUG
#include "../Engine.h"
#endif

CColliderSphere::CColliderSphere()	:
	m_tInfo()
{
	m_eColType = COLLIDER_TYPE::SPHERE;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_3D;
	m_iSceneComponentClassType = static_cast<int>(SCENECOMPONENT_CLASS_TYPE::COLLIDER_SPHERE);
}

CColliderSphere::CColliderSphere(const CColliderSphere& col)	:
	CCollider(col)
	, m_tInfo(col.m_tInfo)
{
}

CColliderSphere::~CColliderSphere()
{
}

const SphereInfo& CColliderSphere::GetInfo() const
{
	return m_tInfo;
}

void CColliderSphere::SetRadius(float fRadius)
{
	m_tInfo.fRadius = fRadius;
}

bool CColliderSphere::Init()
{
	if (!CCollider::Init())
		return false;

#ifdef _DEBUG
	//m_pDebugShader = GET_SINGLE(CShaderManager)->FindShader("Tess");
	m_pDebugShader = GET_SINGLE(CShaderManager)->FindShader("Alpha");
	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("Cube2");
	m_pDepthStencilState = GET_SINGLE(CRenderManager)->FindState("DepthNoRead");
	m_pRasterizer = GET_SINGLE(CRenderManager)->FindState(WIRE_FRAME);
#endif
	return true;
}

void CColliderSphere::Start()
{
	CCollider::Start();
}

void CColliderSphere::Update(float fTime)
{
	CCollider::Update(fTime);

	std::shared_ptr<CTransform> pTransform = GetTransform();
	m_tInfo.vCenter = pTransform->GetBoneWorldPos();
	m_vMin = m_tInfo.vCenter - m_tInfo.fRadius;
	m_vMax = m_vMin + m_tInfo.fRadius * 2.f;
}

void CColliderSphere::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);

#ifdef _DEBUG
	SetWorldScale(m_tInfo.fRadius, m_tInfo.fRadius, m_tInfo.fRadius);
#endif
}

void CColliderSphere::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderSphere::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderSphere::Render(float fTime)
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

void CColliderSphere::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

std::shared_ptr<class CComponent> CColliderSphere::Clone()
{
	return std::shared_ptr<class CComponent>(new CColliderSphere(*this));
}

void CColliderSphere::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tInfo, sizeof(SphereInfo), 1, pFile);
}

void CColliderSphere::Load(FILE * pFile)
{
	CCollider::Load(pFile);

	fread(&m_tInfo, sizeof(SphereInfo), 1, pFile);

#ifdef _DEBUG
	if (!m_pDebugShader)
		m_pDebugShader = GET_SINGLE(CShaderManager)->FindShader(STANDARD3D_SHADER);
	if (!m_pDepthStencilState)
		m_pDepthStencilState = GET_SINGLE(CRenderManager)->FindState("DepthNoRead");
	if (!m_pRasterizer)
		m_pRasterizer = GET_SINGLE(CRenderManager)->FindState(WIRE_FRAME);
	if(!m_pDebugMesh)
		m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("Cube2");
#endif
}

bool CColliderSphere::Collision(CCollider* pDest)
{
	COLLIDER_TYPE eType = pDest->GetColType();

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
		return CCollision::CollisionSphereToSphere(this, static_cast<CColliderSphere*>(pDest));
	case COLLIDER_TYPE::RAY:
		return CCollision::CollisionSphereToRay(this, static_cast<CColliderRay*>(pDest));
	case COLLIDER_TYPE::TERRAIN:
		return CCollision::CollisionSphereToTerrain(this, static_cast<CColliderTerrain*>(pDest));
	case COLLIDER_TYPE::OBB:
		return CCollision::CollisionSphereToOBB(this, static_cast<CColliderOBB*>(pDest));
	case COLLIDER_TYPE::CAPSULE:
		return CCollision::CollisionSphereToCapsule(this, static_cast<CColliderCapsule*>(pDest));
	}

	return true;
}

bool CColliderSphere::CollisionMouse(const Vector2& vPos)
{
	return true;
}

void CColliderSphere::ShowWindow()
{
	CSceneComponent::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		ImGui::Text("Collider Sphere");
		ImGui::InputFloat("Radius", &m_tInfo.fRadius);
		ImGui::Text("Center X: %.3f, Y: %.3f, Z: %.3f", m_tInfo.vCenter.x, m_tInfo.vCenter.y, m_tInfo.vCenter.z);
	}
	ImGui::End();
	
}
