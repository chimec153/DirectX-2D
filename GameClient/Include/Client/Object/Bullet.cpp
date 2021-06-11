#include "Bullet.h"
#include "Resource/Mesh2D.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Component/Camera.h"
#include "CameraManager.h"
#include "Component/SpriteComponent.h"
#include "Scene/Scene.h"
#include "Component/ColliderLine.h"
#include "Component/ColliderCircle.h"
#include "UI/Bar.h"
#include "UI/UIImage.h"
#include "UI/UFont.h"

CBullet::CBullet()	:
	m_fSpeed(600.f),
	m_fDist(0.f),
	m_fLimitDist(1000.f),
	m_pMesh(nullptr),
	m_pCam(nullptr)
{
	m_iObjType = static_cast<int>(OBJECT_CLASS_TYPE::OCT_BULLET);
}

CBullet::CBullet(const CBullet& bullet)	:
	CGameObject(bullet)
{
	m_fSpeed = bullet.m_fSpeed;
	m_fDist = bullet.m_fDist;
	m_fLimitDist = bullet.m_fLimitDist;

	m_pMesh = m_pRootComponent;

	m_pCam = FindComponent("bulCam");

	((CCamera*)m_pCam.get())->SetObj(this);

	((CSpriteComponent*)m_pRootComponent.get())->SetFunc<CBullet>("explode", this, &CBullet::Destroy);

	CColliderCircle* pCC = (CColliderCircle*)FindComponent("BulletBody").get();

	pCC->SetCallBack<CBullet>(COLLISION_STATE::CS_INIT, this, &CBullet::ColEnter);
}

CBullet::~CBullet()
{
	
}
void CBullet::SetPt(const Vector3* v)
{
	memcpy(m_pPoint, v, sizeof(Vector3) * CONTROL_POINT_COUNT);
}

bool CBullet::Init()
{
	m_pMesh = CreateComponent<CSpriteComponent>("Bullet_Mesh");

	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("AnaIdle", "AnaIdle", 1.f, true);
	//m_pMesh->AddAni2DInfo("explode", "Explode", 1.f, false);
	//m_pMesh->SetFunc<CBullet>("explode", this, &CRef::Destroy);
	((CSpriteComponent*)m_pMesh.get())->SetInstance();
	
	std::shared_ptr<CComponent> pLC = CreateComponent<CColliderCircle>("BulletBody");
	//pLC->SetCallBack<CBullet>(COLLISION_STATE::CS_INIT, this, &CBullet::ColEnter);
	//pLC->SetWorldScale(100.f, 100.f, 0.f);

	//pLC->SetInheritScale(false);
	((CColliderCircle*)pLC.get())->SetRadius(25.f);
	((CColliderCircle*)pLC.get())->SetProFile("Custom1");
	((CColliderCircle*)pLC.get())->SetInstance();

	((CSpriteComponent*)m_pMesh.get())->AddChild(pLC);

	m_pCam = CreateComponent<CCamera>("bulCam");
	((CCamera*)m_pCam.get())->SetCameraType(CAMERA_TYPE::CT_2D);

	((CCamera*)m_pCam.get())->SetInheritScale(false);
	((CCamera*)m_pCam.get())->SetInheritRotX(false);
	((CCamera*)m_pCam.get())->SetInheritRotY(false);
	((CCamera*)m_pCam.get())->SetInheritRotZ(false);

	((CCamera*)m_pCam.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CSpriteComponent*)m_pMesh.get())->AddChild(m_pCam);

	SetRootComponent(m_pMesh);

	((CSpriteComponent*)m_pMesh.get())->SetRelativeScale(50.f, 50.f, 1.f);
	((CSpriteComponent*)m_pMesh.get())->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CBullet::Start()
{
	//GET_SINGLE(CCameraManager)->SetMainCam(m_pCam);
}

void CBullet::Input(float fTime)
{
}

void CBullet::Update(float fTime)
{
	CGameObject::Update(fTime);
/*
	Vector3 vPos = GetWorldPos();

	CGameObject* pObj = m_pScene->FindObject("Minion");

	Vector3 vTarget = pObj->GetWorldPos();

	SAFE_RELEASE(pObj);

	Vector3 vDir = GetWorldAxis(AXIS_Y);

	vTarget -= vPos;

	float fDot = vTarget.Dot(vDir);

	if (fDot > 0)
	{
		float fCross = vTarget.Cross(vDir).z;

		if (fCross < 0)
			AddWorldRotZ(120.f * fTime);

		else
			AddWorldRotZ(120.f * -fTime);
	}*/

	//AddWorldPos(GetWorldAxis(AXIS_Y) * fTime * m_fSpeed);

	m_fDist += m_fSpeed * fTime;

	if (m_fDist >= m_fLimitDist)
		Destroy();
/*
	Vector3 vFirst = (m_vMid - m_vStart) * m_fDist / m_fLimitDist;
	Vector3 vSecond = (m_vEnd - m_vMid) * m_fDist / m_fLimitDist;

	Vector3 vPos = (vSecond - vFirst) * m_fDist / m_fLimitDist;

	SetWorldPos(m_vStart + vFirst + vPos);*/
	Vector3 vPos = {};

	float fT = (m_fDist / m_fLimitDist) * 3.f + 3.f;

	int iStart = (int)fT;

	BSplineCurve(m_pPoint[iStart - 3], m_pPoint[iStart - 2], m_pPoint[iStart - 1], m_pPoint[iStart], fT - iStart, vPos);

	SetWorldPos(vPos);
}

void CBullet::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CBullet::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CBullet::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CBullet::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CBullet::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CBullet::Clone()
{
	return std::shared_ptr<CGameObject>(new CBullet(*this));
}

void CBullet::Save(FILE* pFile)
{
	CGameObject::Save(pFile);

	fwrite(&m_fSpeed, sizeof(float), 1, pFile);
	fwrite(&m_fDist, sizeof(float), 1, pFile);
	fwrite(&m_fLimitDist, sizeof(float), 1, pFile);

	std::string strName = m_pMesh->GetName();

	int iLength = (int)strName.length();

	fwrite(&iLength, sizeof(int), 1, pFile);
	fwrite(strName.c_str(), sizeof(char), iLength, pFile);

	fwrite(m_pPoint, sizeof(Vector3), CONTROL_POINT_COUNT, pFile);
}

void CBullet::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	fread(&m_fSpeed, sizeof(float), 1, pFile);
	fread(&m_fDist, sizeof(float), 1, pFile);
	fread(&m_fLimitDist, sizeof(float), 1, pFile);

	char strTag[256] = {};

	int iLength = 0;

	fread(&iLength, sizeof(int), 1, pFile);
	fread(strTag, sizeof(char), iLength, pFile);

	std::string strName = strTag;

	m_pMesh = std::shared_ptr<CComponent>(FindComponent(strName));

	m_pCam = FindComponentByType<CCamera>();

	fread(m_pPoint, sizeof(Vector3), CONTROL_POINT_COUNT, pFile);
}

void CBullet::ColEnter(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (pDest->GetName() == "MonsterBody")
	{
   		CGameObject* pMon = pDest->GetObj();

		Vector3 vNrm = pMon->GetWorldAxis(AXIS_Y);

		Vector3 vAxis = GetWorldAxis(AXIS_Y);

		Vector3 vRef = vAxis - 2 * (vAxis.Dot(vNrm)) * vNrm;

		float fAngle = DirectX::XMConvertToDegrees(acosf(vAxis.Dot(vRef)));
		
		Vector3 vRot = GetWorldRot();

		vRot.z += fAngle;

		SetWorldRot(vRot);
	}

	else
	{
		m_fSpeed = 0.f;

		Destroy();

		//GET_SINGLE(CCameraManager)->SetMainCam("camera");

		std::shared_ptr<CGameObject> pObj = m_pScene->CreateCloneObj("Effect", "Effect", m_pLayer, m_pScene->GetSceneType());

		pObj->SetWorldPos(pSrc->GetCrs());
	}
}

void CBullet::BSplineCurve(const Vector3& pt1, const Vector3& pt2, const Vector3& pt3, const Vector3& pt4, const float fTime, Vector3& vResult)
{
	const float t2 = fTime * fTime;
	const float t3 = t2 * fTime;
	const float mt = 1.f - fTime;
	const float mt3 = mt * mt * mt;

	const float bi3 = mt3;
	const float bi2 = 3 * t3 - 6 * t2 + 4;
	const float bi1 = -3 * t3 + 3 * t2 + 3 * fTime + 1;
	const float bi = t3;

	vResult.x = pt1.x * bi3 +
		pt2.x * bi2 +
		pt3.x * bi1 +
		pt4.x * bi;

	vResult.x /= 6.f;

	vResult.y = pt1.y * bi3 +
		pt2.y * bi2 +
		pt3.y * bi1 +
		pt4.y * bi;

	vResult.y /= 6.f;
}
