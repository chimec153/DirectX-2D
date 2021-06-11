#include "ColliderRay.h"
#include "Collision.h"
#include "ColliderSphere.h"
#include "ColliderTerrain.h"
#include "ColliderOBB.h"
#include "ColliderRect.h"
#include "ColliderCapsule.h"

CColliderRay::CColliderRay()	:
	m_tInfo()
{
	m_eColType = COLLIDER_TYPE::RAY;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_3D;
	m_iSceneComponentClassType = static_cast<int>(SCENECOMPONENT_CLASS_TYPE::COLLIDER_RAY);
}

CColliderRay::CColliderRay(const CColliderRay& ray)	:
	CCollider(ray)
	, m_tInfo(ray.m_tInfo)
{
}

CColliderRay::~CColliderRay()
{
}

const RAYINFO& CColliderRay::GetInfo() const
{
	return m_tInfo;
}

void CColliderRay::SetOrigin(const Vector3& vOrigin)
{
	m_tInfo.vOrigin = vOrigin;
}

void CColliderRay::SetDir(const Vector3& vDir)
{
	m_tInfo.vDir = vDir;
}

bool CColliderRay::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderRay::Start()
{
	CCollider::Start();
}

void CColliderRay::Update(float fTime)
{
	CCollider::Update(fTime);

}

void CColliderRay::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);
}

void CColliderRay::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderRay::PreRender(float fTime)
{
		CCollider::PreRender(fTime);
}

void CColliderRay::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderRay::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

std::shared_ptr<CComponent> CColliderRay::Clone()
{
	return std::shared_ptr<CComponent>(new CColliderRay(*this));
}

bool CColliderRay::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionRectToRay(static_cast<CColliderRect*>(pDest), this);
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
		return CCollision::CollisionSphereToRay(static_cast<CColliderSphere*>(pDest), this);
	case COLLIDER_TYPE::RAY:
		return CCollision::CollisionRayToRay(this, static_cast<CColliderRay*>(pDest));
	case COLLIDER_TYPE::TERRAIN:
		return CCollision::CollisionRayToTerrain(this, static_cast<CColliderTerrain*>(pDest));
	case COLLIDER_TYPE::OBB:
		return CCollision::CollisionRayToOBB(this, static_cast<CColliderOBB*>(pDest));
	case COLLIDER_TYPE::CAPSULE:
		return CCollision::CollisionRayToCapsule(this, static_cast<CColliderCapsule*>(pDest));
	}

	return false;
}

bool CColliderRay::CollisionMouse(const Vector2& vPos)
{
	return false;
}

void CColliderRay::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tInfo, sizeof(RAYINFO), 1, pFile);
}

void CColliderRay::Load(FILE* pFile)
{
	CCollider::Load(pFile);
	fread(&m_tInfo, sizeof(RAYINFO), 1, pFile);
}
