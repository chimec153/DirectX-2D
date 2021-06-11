#include "ColliderOBB2D.h"
#include "../Resource/ResourceManager.h"
#include "Collision.h"
#include "ColliderLine.h"
#include "ColliderCircle.h"
#include "ColliderPixel.h"
#include "ColliderRect.h"
#include "ColliderPoint.h"

CColliderOBB2D::CColliderOBB2D()
{
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_OBB2D;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;
	m_eColType = COLLIDER_TYPE::CT_OBB2D;
	m_b2D = true;
}

CColliderOBB2D::CColliderOBB2D(const CColliderOBB2D& col)	:
	CCollider(col)
{
}

CColliderOBB2D::~CColliderOBB2D()
{
}

void CColliderOBB2D::SetExtend(float x, float y,float z)
{
	m_tInfo.fLength[AXIS_X] = x;
	m_tInfo.fLength[AXIS_Y] = y;
	m_tInfo.fLength[AXIS_Z] = z;

	SetWorldScale(x, y, z);
}

OBBInfo CColliderOBB2D::GetInfo() const
{
	return m_tInfo;
}

bool CColliderOBB2D::Init()
{
	if (!CCollider::Init())
		return false;

#ifdef _DEBUG
	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("Collider2DRect");
#endif

	return true;
}

void CColliderOBB2D::Start()
{
	CCollider::Start();
}

void CColliderOBB2D::Update(float fTime)
{
	CCollider::Update(fTime);
}

void CColliderOBB2D::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);

	for (int i = 0; i < AXIS_END; ++i)
		m_tInfo.vAxis[i] = GetWorldAxis(AXIS(i));		

	Vector3 vScale;

	SetWorldScale(m_tInfo.fLength[AXIS_X] * 2.f, m_tInfo.fLength[AXIS_Y] * 2.f, m_tInfo.fLength[AXIS_Z] * 2.f);

	vScale = GetWorldScale();

	m_tInfo.vPos = GetWorldPos();

	Vector3 vLB = m_tInfo.vPos - m_tInfo.vAxis[AXIS_X] * m_tInfo.fLength[AXIS_X] / 2.f - m_tInfo.vAxis[AXIS_Y] * m_tInfo.fLength[AXIS_Y] / 2.f;
	Vector3 vRT = m_tInfo.vPos + m_tInfo.vAxis[AXIS_X] * m_tInfo.fLength[AXIS_X] / 2.f + m_tInfo.vAxis[AXIS_Y] * m_tInfo.fLength[AXIS_Y] / 2.f;
	Vector3 vLT = m_tInfo.vPos - m_tInfo.vAxis[AXIS_X] * m_tInfo.fLength[AXIS_X] / 2.f + m_tInfo.vAxis[AXIS_Y] * m_tInfo.fLength[AXIS_Y] / 2.f;
	Vector3 vRB = m_tInfo.vPos + m_tInfo.vAxis[AXIS_X] * m_tInfo.fLength[AXIS_X] / 2.f - m_tInfo.vAxis[AXIS_Y] * m_tInfo.fLength[AXIS_Y] / 2.f;

	m_vMin = vLB;
	m_vMax = vLB;

	m_vMin.x > vRT.x ? m_vMin.x = vRT.x : 0;

	m_vMin.x > vLT.x ? m_vMin.x = vLT.x : 0;

	m_vMin.x > vRB.x ? m_vMin.x = vRB.x : 0;

	m_vMin.y > vRT.y ? m_vMin.y = vRT.y : 0;

	m_vMin.y > vLT.y ? m_vMin.y = vLT.y : 0;

	m_vMin.y > vRB.y ? m_vMin.y = vRB.y : 0;

	m_vMax.x < vRT.x ? m_vMax.x = vRT.x : 0;

	m_vMax.x < vLT.x ? m_vMax.x = vLT.x : 0;

	m_vMax.x < vRB.x ? m_vMax.x = vRB.x : 0;

	m_vMax.y < vRT.y ? m_vMax.y = vRT.y : 0;

	m_vMax.y < vLT.y ? m_vMax.y = vLT.y : 0;

	m_vMax.y < vRB.y ? m_vMax.y = vRB.y : 0;
}

void CColliderOBB2D::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderOBB2D::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderOBB2D::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderOBB2D::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

std::shared_ptr<CComponent> CColliderOBB2D::Clone()
{
	return std::shared_ptr<CComponent> (new CColliderOBB2D(*this));
}

void CColliderOBB2D::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tInfo, sizeof(OBBInfo), 1, pFile);
}

void CColliderOBB2D::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	fread(&m_tInfo, sizeof(OBBInfo), 1, pFile);
}

bool CColliderOBB2D::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionOBB2DToRect((CColliderOBB2D*)this, (CColliderRect*)pDest);
	case COLLIDER_TYPE::CT_SPHERE2D:
		return CCollision::CollisionCircleToOBB2D((CColliderCircle*)pDest, (CColliderOBB2D*)this);
	case COLLIDER_TYPE::CT_POINT:
		return CCollision::CollisionOBB2DToPoint((CColliderOBB2D*)this, (CColliderPoint*)pDest);
	case COLLIDER_TYPE::CT_OBB2D:
		return CCollision::CollisionOBB2DToOBB2D((CColliderOBB2D*)this, (CColliderOBB2D*)pDest);
	case COLLIDER_TYPE::CT_PIXEL:
		return CCollision::CollisionPixelToOBB2D((CColliderPixel*)pDest, (CColliderOBB2D*)this);
	case COLLIDER_TYPE::CT_LINE:
		return CCollision::CollisionLineToOBB2D((CColliderLine*)pDest, (CColliderOBB2D*)this);
	}

	return false;
}

bool CColliderOBB2D::CollisionMouse(const Vector2& vMousePos)
{
	Vector3 vCrs;

	if (CCollision::CollisionOBB2DToPoint(vCrs, GetInfo(), Vector3(vMousePos.x, vMousePos.y, 0.f)))
	{
		SetCross(vCrs);

		return true;
	}

	return false;
}
