#include "ColliderLine.h"
#include "Collision.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/ResourceManager.h"
#include "ColliderRect.h"
#include "ColliderCircle.h"
#include "ColliderOBB2D.h"
#include "ColliderPoint.h"

CColliderLine::CColliderLine()	:
	m_tInfo()
{
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_LINE;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;
	m_eColType = COLLIDER_TYPE::CT_LINE;
	m_b2D = true;
}

CColliderLine::CColliderLine(const CColliderLine& col)	:
	CCollider(col)
{
	m_tInfo = col.m_tInfo;
}

CColliderLine::~CColliderLine()
{
}

LineInfo CColliderLine::GetInfo() const
{
	return m_tInfo;
}

bool CColliderLine::Init()
{
	if (!CCollider::Init())
		return false;

#ifdef _DEBUG
	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("Collider2DLine");
#endif

	return true;
}

void CColliderLine::Start()
{
	CCollider::Start();
}

void CColliderLine::Update(float fTime)
{
	CCollider::Update(fTime);
}

void CColliderLine::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);

	Matrix matWorld = GetMatWorld();

	m_tInfo.vStart = Vector3(0.f, 0.f, 0.f);
	m_tInfo.vEnd = Vector3(0.f, 1.f, 0.f);

	m_tInfo.vStart  = m_tInfo.vStart.TranslationCoor(matWorld);
	m_tInfo.vEnd = m_tInfo.vEnd.TranslationCoor(matWorld);

	m_vMin.x = m_tInfo.vStart.x;
	m_vMin.y = m_tInfo.vStart.y;
	m_vMax.x = m_tInfo.vEnd.x;
	m_vMax.y = m_tInfo.vEnd.y;

	if (m_vMin.x > m_vMax.x)
	{
		float x = m_vMax.x;
		m_vMax.x = m_vMin.x;
		m_vMin.x = x;
	}

	if (m_vMin.y > m_vMax.y)
	{
		float y = m_vMax.y;
		m_vMax.y = m_vMin.y;
		m_vMin.y = y;
	}
}

void CColliderLine::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderLine::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderLine::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderLine::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

std::shared_ptr<CComponent> CColliderLine::Clone()
{
	return std::shared_ptr<CComponent> (new CColliderLine(*this));
}

void CColliderLine::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tInfo, sizeof(LineInfo), 1, pFile);
}

void CColliderLine::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	fread(&m_tInfo, sizeof(LineInfo), 1, pFile);
}

bool CColliderLine::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionLineToRect((CColliderLine*)this, (CColliderRect*)pDest);
	case COLLIDER_TYPE::CT_SPHERE2D:
		return CCollision::CollisionLineToCircle((CColliderLine*)this, (CColliderCircle*)pDest);
	case COLLIDER_TYPE::CT_POINT:
		return CCollision::CollisionLineToPoint((CColliderLine*)this, (CColliderPoint*)pDest);
	case COLLIDER_TYPE::CT_OBB2D:
		return CCollision::CollisionLineToOBB2D((CColliderLine*)this, (CColliderOBB2D*)pDest);
	case COLLIDER_TYPE::CT_PIXEL:
		break;
	case COLLIDER_TYPE::CT_LINE:
		return CCollision::CollisionLineToLine((CColliderLine*)this, (CColliderLine*)pDest);
	}

	return false;
}

bool CColliderLine::CollisionMouse(const Vector2& vMousePos)
{
	Vector3 vCrs;

	if (CCollision::CollisionLineToPoint(vCrs, GetInfo(), Vector3(vMousePos.x, vMousePos.y, 0.f)))
	{
		SetCross(vCrs);

		return true;
	}

	return false;
}
