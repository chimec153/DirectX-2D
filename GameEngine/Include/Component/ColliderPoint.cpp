#include "ColliderPoint.h"
#include "../Resource/ResourceManager.h"
#include "Collision.h"
#include "ColliderRect.h"
#include "ColliderPixel.h"
#include "ColliderOBB2D.h"
#include "ColliderCircle.h"
#include "../CameraManager.h"
#include "Camera.h"
#include "../Device.h"

CColliderPoint::CColliderPoint()	:
	m_vPt()
{
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::COLLIDER_POINT;
	m_eColType = COLLIDER_TYPE::CT_POINT;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;

	m_b2D = true;
}

CColliderPoint::CColliderPoint(const CColliderPoint& col)	:
	CCollider(col)
{
	m_vPt = col.m_vPt;
}

CColliderPoint::~CColliderPoint()
{
}

const Vector3& CColliderPoint::GetInfo() const
{
	return m_vPt;
}

const Vector3& CColliderPoint::GetUIInfo() const
{
	return m_vUIPt;
}

bool CColliderPoint::Init()
{
	if (!CCollider::Init())
		return false;

#ifdef _DEBUG	
	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("Collider2DRect");
#endif

	return true;
}

void CColliderPoint::Start()
{
	CCollider::Start();

	SetWorldScale(1.f, 1.f, 1.f);
}

void CColliderPoint::Update(float fTime)
{
	CCollider::Update(fTime);
}

void CColliderPoint::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	m_vPt = GetWorldPos();

	Resolution tRS = RESOLUTION;

	if (pCam)
	{
		m_vUIPt = m_vPt - (pCam->GetWorldPos() - Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f) * pCam->GetPivot());
	}
	else
	{
		m_vUIPt = m_vPt;
	}

	m_vUIPt.z = 0.f;

	SetWorldScale(1.f, 1.f, 1.f);

	m_vMin = m_vPt;
	m_vMax = m_vPt;
}

void CColliderPoint::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderPoint::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderPoint::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderPoint::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

std::shared_ptr<CComponent> CColliderPoint::Clone()
{
	return std::shared_ptr<CComponent> (new CColliderPoint(*this));
}

void CColliderPoint::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_vPt, sizeof(Vector3), 1, pFile);
	fwrite(&m_vUIPt, sizeof(Vector3), 1, pFile);
}

void CColliderPoint::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	fread(&m_vPt, sizeof(Vector3), 1, pFile);
	fread(&m_vUIPt, sizeof(Vector3), 1, pFile);
}

bool CColliderPoint::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionRectToPoint((CColliderRect*)pDest, this);
	case COLLIDER_TYPE::CT_SPHERE2D:
		return CCollision::CollisionCircleToPoint((CColliderCircle*)pDest, this);
	case COLLIDER_TYPE::CT_POINT:
		return CCollision::CollisionPointToPoint(this, (CColliderPoint*)pDest);
	case COLLIDER_TYPE::CT_OBB2D:
		return CCollision::CollisionOBB2DToPoint((CColliderOBB2D*)pDest, this);
	case COLLIDER_TYPE::CT_PIXEL:
		return CCollision::CollisionPixelToPoint((CColliderPixel*)pDest, this);
	case COLLIDER_TYPE::CT_LINE:
		break;
	}

	return false;
}

bool CColliderPoint::CollisionMouse(const Vector2& vMousePos)
{
	Vector3 vCrs;

	if (CCollision::CollisionPointToPoint(vCrs, GetInfo(), Vector3(vMousePos.x, vMousePos.y, 0.f)))
	{
		SetCross(vCrs);

		return true;
	}

	return false;
}
