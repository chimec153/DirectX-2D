#include "ColliderRect.h"
#include "../Resource/Material.h"
#include "../Resource/ResourceManager.h"
#include "Collision.h"
#include "../Resource/Mesh2D.h"
#include "ColliderPixel.h"
#include "ColliderOBB2D.h"
#include "ColliderLine.h"
#include "ColliderCircle.h"
#include "ColliderPoint.h"
#include "ColliderRay.h"
#include "../CameraManager.h"
#include "Camera.h"

CColliderRect::CColliderRect()
{
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_RECT;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;
	m_eColType = COLLIDER_TYPE::CT_RECT;
	m_b2D = true;
}

CColliderRect::CColliderRect(const CColliderRect& col)	:
	CCollider(col)
{
	m_tInfo = col.m_tInfo;
	m_vExtend = col.m_vExtend;
}

CColliderRect::~CColliderRect()
{
}

void CColliderRect::SetExtend(float x, float y)
{
	m_vExtend.x = x;
	m_vExtend.y = y;

	SetWorldScale(x, y, 0.f);
}

void CColliderRect::SetExtend(const Vector2& v)
{
	SetExtend(v.x, v.y);
}

const RectInfo& CColliderRect::GetInfo() const
{
	return m_tInfo;
}

const Vector2& CColliderRect::GetExtent() const
{
	return m_vExtend;
}

bool CColliderRect::Init()
{
	if (!CCollider::Init())
		return false;
#ifdef _DEBUG
	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("Collider2DRect");
#endif

	SetInheritRotX(false);
	SetInheritRotY(false);
	SetInheritRotZ(false);

	return true;
}

void CColliderRect::Start()
{
	CCollider::Start();
}

void CColliderRect::Update(float fTime)
{
	CCollider::Update(fTime);
}

void CColliderRect::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);

	m_tInfo.fL = GetWorldPos().x - m_vExtend.x / 2.f;
	m_tInfo.fR = GetWorldPos().x + m_vExtend.x / 2.f;
	m_tInfo.fB = GetWorldPos().y - m_vExtend.y / 2.f;
	m_tInfo.fT = GetWorldPos().y + m_vExtend.y / 2.f;

	m_vMin.x = m_tInfo.fL;
	m_vMin.y = m_tInfo.fB;
	m_vMax.x = m_tInfo.fR;
	m_vMax.y = m_tInfo.fT;

	/*if (IsUI())
	{
		CCamera* pCam =GET_SINGLE(CCameraManager)->GetMainCam();
		Resolution tRS = GET_SINGLE(CCameraManager)->GetResolution();
		Vector3 vPivot = pCam->GetPivot();

		Vector3 vPos = pCam->GetWorldPos();
		m_tInfo.fL += vPos.x + tRS.iWidth * (1.f - vPivot.x);
		m_tInfo.fR += vPos.x + tRS.iWidth * (1.f - vPivot.x);
		m_tInfo.fB += vPos.y + tRS.iHeight * (1.f - vPivot.y);
		m_tInfo.fT += vPos.y + tRS.iHeight * (1.f - vPivot.y);
	}*/


	SetWorldScale(m_vExtend.x, m_vExtend.y, 1.f);
}

void CColliderRect::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderRect::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderRect::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderRect::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

std::shared_ptr<CComponent> CColliderRect::Clone()
{
	return std::shared_ptr<CComponent>(new CColliderRect(*this));
}

void CColliderRect::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tInfo, sizeof(RectInfo), 1, pFile);
	fwrite(&m_vExtend, sizeof(Vector2), 1, pFile);
}

void CColliderRect::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	fread(&m_tInfo, sizeof(RectInfo), 1, pFile);
	fread(&m_vExtend, sizeof(Vector2), 1, pFile);
}

bool CColliderRect::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionRectToRect((CColliderRect*)this, (CColliderRect*)pDest);
	case COLLIDER_TYPE::CT_SPHERE2D:
		return CCollision::CollisionRectToCircle((CColliderRect*)this, (CColliderCircle*)pDest);
	case COLLIDER_TYPE::CT_POINT:
		return CCollision::CollisionRectToPoint(this, (CColliderPoint*)pDest);
	case COLLIDER_TYPE::CT_OBB2D:
		return CCollision::CollisionOBB2DToRect((CColliderOBB2D*)pDest, (CColliderRect*)this);
	case COLLIDER_TYPE::CT_PIXEL:
		return CCollision::CollisionPixelToRect((CColliderPixel*)pDest, (CColliderRect*)this);
	case COLLIDER_TYPE::CT_LINE:
		return CCollision::CollisionLineToRect((CColliderLine*)pDest, (CColliderRect*)this);
	case COLLIDER_TYPE::RAY:
		return CCollision::CollisionRectToRay(this, static_cast<CColliderRay*>(pDest));
	}

	return false;
}

bool CColliderRect::CollisionMouse(const Vector2& vMousePos)
{
	Vector3 vCrs;

	if (CCollision::CollisionRectToPoint(vCrs, GetInfo(), Vector3(vMousePos.x, vMousePos.y,0.f)))
	{
		SetCross(vCrs);

		return true;
	}

	return false;
}
