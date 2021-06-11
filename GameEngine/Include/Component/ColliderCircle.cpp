#include "ColliderCircle.h"
#include "../Resource/ResourceManager.h"
#include "../Component/Collision.h"
#include "ColliderLine.h"
#include "ColliderOBB2D.h"
#include "ColliderRect.h"
#include "ColliderPixel.h"
#include "ColliderPoint.h"

CColliderCircle::CColliderCircle()	:
	m_tInfo()
{
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_CIRCLE;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;
	m_eColType = COLLIDER_TYPE::CT_SPHERE2D;
	m_b2D = true;
}

CColliderCircle::CColliderCircle(const CColliderCircle& col)	:
	CCollider(col)
{
	m_tInfo = col.m_tInfo;
}

CColliderCircle::~CColliderCircle()
{
}

void CColliderCircle::SetRadius(float fRadius)
{
	m_tInfo.fRadius = fRadius;

	SetWorldScale(m_tInfo.fRadius, m_tInfo.fRadius, 0.f);
}

SphereInfo CColliderCircle::GetInfo() const
{
	return m_tInfo;
}

bool CColliderCircle::Init()
{
	if (!CCollider::Init())
		return false;
#ifdef _DEBUG
	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("ColliderCircle2D");
#endif

	return true;
}

void CColliderCircle::Start()
{
	CCollider::Start();
}

void CColliderCircle::Update(float fTime)
{
	CCollider::Update(fTime);

	Vector3 vPos = GetWorldPos();

	m_tInfo.vCenter = vPos;

	m_vMin = vPos - Vector3(m_tInfo.fRadius, m_tInfo.fRadius, 0.f);
	m_vMax = vPos + Vector3(m_tInfo.fRadius, m_tInfo.fRadius, 0.f);

	SetWorldScale(Vector3(m_tInfo.fRadius, m_tInfo.fRadius, 0.f));
}

void CColliderCircle::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);
}

void CColliderCircle::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderCircle::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderCircle::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderCircle::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

std::shared_ptr<CComponent> CColliderCircle::Clone()
{	
	return std::shared_ptr<CComponent>(new CColliderCircle(*this));
}

void CColliderCircle::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tInfo, sizeof(SphereInfo), 1, pFile);
}

void CColliderCircle::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	fread(&m_tInfo, sizeof(SphereInfo), 1, pFile);
}

bool CColliderCircle::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionRectToCircle((CColliderRect*)pDest, (CColliderCircle*)this);
	case COLLIDER_TYPE::CT_SPHERE2D:
		return CCollision::CollisionCircleToCircle((CColliderCircle*)this, (CColliderCircle*)pDest);
	case COLLIDER_TYPE::CT_POINT:
		return CCollision::CollisionCircleToPoint((CColliderCircle*)this, (CColliderPoint*)pDest);
	case COLLIDER_TYPE::CT_OBB2D:
		return CCollision::CollisionCircleToOBB2D((CColliderCircle*)this, (CColliderOBB2D*)pDest);
	case COLLIDER_TYPE::CT_PIXEL:
		return CCollision::CollisionPixelToSphere2D((CColliderPixel*)pDest, (CColliderCircle*)this);
	case COLLIDER_TYPE::CT_LINE:
		return CCollision::CollisionLineToCircle((CColliderLine*)pDest, (CColliderCircle*)this);
	}

	return false;
}

bool CColliderCircle::CollisionMouse(const Vector2& vMousePos)
{
	Vector3 vCrs;

	if (CCollision::CollisionCircleToPoint(vCrs, GetInfo(), Vector3(vMousePos.x, vMousePos.y, 0.f)))
	{
		SetCross(vCrs);

		return true;
	}

	return false;
}
