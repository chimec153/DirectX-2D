#include "ColliderCapsule.h"
#include "Collision.h"
#include "ColliderSphere.h"
#include "ColliderRay.h"

CColliderCapsule::CColliderCapsule()	:
	m_tInfo()
	, m_fOffset(0.f)
{
	m_iSceneComponentClassType = static_cast<int>(SCENECOMPONENT_CLASS_TYPE::COLLIDER_CAPSULE);
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_3D;
	m_eColType = COLLIDER_TYPE::CAPSULE;
}

CColliderCapsule::CColliderCapsule(const CColliderCapsule& com)	:
	CCollider(com)
	, m_tInfo(com.m_tInfo)
	, m_fOffset(com.m_fOffset)
{
}

CColliderCapsule::~CColliderCapsule()
{
}

void CColliderCapsule::SetRadius(float fRadius)
{
	m_tInfo.fRadius = fRadius;
}

void CColliderCapsule::SetHeight(float fHeight)
{
	m_tInfo.fHeight = fHeight;
}

void CColliderCapsule::SetOffset(float fOffset)
{
	m_fOffset = fOffset;
}

const CAPSULEINFO& CColliderCapsule::GetInfo() const
{
	return m_tInfo;
}

bool CColliderCapsule::Init()
{
	if(!CCollider::Init())
		return false;

	return true;
}

void CColliderCapsule::Start()
{
	CCollider::Start();
}

void CColliderCapsule::Update(float fTime)
{
	CCollider::Update(fTime);

	m_tInfo.vDir = GetWorldAxis(AXIS_Y);

	m_tInfo.vCenter = GetWorldPos() + m_fOffset * m_tInfo.vDir;
}

void CColliderCapsule::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);
}

void CColliderCapsule::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderCapsule::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderCapsule::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderCapsule::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

std::shared_ptr<class CComponent> CColliderCapsule::Clone()
{
	return std::shared_ptr<class CComponent>(new CColliderCapsule(*this));
}

void CColliderCapsule::Save(FILE* pFile)
{
	CCollider::Save(pFile);
}

void CColliderCapsule::Load(FILE* pFile)
{
	CCollider::Load(pFile);
}

bool CColliderCapsule::CollisionMouse(const Vector2& vPos)
{
	return false;
}

bool CColliderCapsule::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
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
		return CCollision::CollisionSphereToCapsule(static_cast<CColliderSphere*>(pDest), this);
	case COLLIDER_TYPE::RAY:
		return CCollision::CollisionRayToCapsule(static_cast<CColliderRay*>(pDest), this);
	case COLLIDER_TYPE::TERRAIN:
		break;
	case COLLIDER_TYPE::OBB:
		break;
	case COLLIDER_TYPE::CAPSULE:
		return CCollision::CollisionCapsuleToCapsule(this, static_cast<CColliderCapsule*>(pDest));
	}

	return false;
}
