#include "Bullet3D.h"
#include "Component/ColliderSphere.h"
#include "Component/Renderer.h"
#include "Component/Particle.h"
#include "Component/Transform.h"
#include "Resource/Material.h"
#include "Component/Light.h"
#include "Particle3D.h"
#include "Scene/Scene.h"
#include "Monster3D.h"
#include "Sound/SoundManager.h"

CBullet3D::CBullet3D()	:
	m_fSpeed(200.f)
	, m_fDist(0.f)
	, m_fLimitDist(600.f)
{
	m_iObjType = static_cast<int>(OBJECT_CLASS_TYPE::OCT_BULLET);
}

CBullet3D::CBullet3D(const CBullet3D& obj)	:
	CGameObject(obj)
	, m_fSpeed(obj.m_fSpeed)
	, m_fDist(obj.m_fDist)
	, m_fLimitDist(obj.m_fLimitDist)
{
	std::shared_ptr<CComponent> pCol = FindComponentByType<CColliderSphere>();

	CColliderSphere* pCom = static_cast<CColliderSphere*>(pCol.get());

	pCom->SetCallBack<CBullet3D>(COLLISION_STATE::CS_INIT, this, &CBullet3D::CollisionInit);
	pCom->SetCallBack<CBullet3D>(COLLISION_STATE::CS_STAY, this, &CBullet3D::CollisionStay);
	pCom->SetCallBack<CBullet3D>(COLLISION_STATE::CS_LAST, this, &CBullet3D::CollisionLast);
}

CBullet3D::~CBullet3D()
{
}

bool CBullet3D::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Sorcerer_FireBall_Explosion", false,
		"Skill\\Sorcerer_FireBall_Explosion.ogg");
	if (!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CSceneComponent>("BulletRenderer");

	SetRootComponent(pRenderer);

	std::shared_ptr<CComponent> pCol = CreateComponent<CColliderSphere>("BulletBody");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pCol);

	CColliderSphere* pCom = static_cast<CColliderSphere*>(pCol.get());
	pCom->SetRadius(50.f);

	int iCount = 6;
	for (int i = 0; i < iCount; i++)
	{
		std::shared_ptr<CComponent> pParticle = CreateComponent<CParticle>("FireParticle");

		static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pParticle);

		static_cast<CParticle*>(pParticle.get())->AddRelativePos(30.f * cosf(PI / iCount * 2.f * i), 30.f * sinf(PI / iCount * 2.f * i), 0.f);
		static_cast<CParticle*>(pParticle.get())->SetTexture(LINK_TYPE::LT_DIF, "B_Fire004_emis");
		static_cast<CParticle*>(pParticle.get())->SetCountX(4);
		static_cast<CParticle*>(pParticle.get())->SetCountY(4);
		static_cast<CParticle*>(pParticle.get())->SetImageSize(Vector2(256.f, 256.f));
		static_cast<CParticle*>(pParticle.get())->SetMaxFrame(16);
		static_cast<CParticle*>(pParticle.get())->SetMaxScale(Vector3(32.f, 32.f, 1.f));
		static_cast<CParticle*>(pParticle.get())->SetMinScale(Vector3(32.f, 32.f, 1.f));
		static_cast<CParticle*>(pParticle.get())->SetMaxTime(1.0f);
		static_cast<CParticle*>(pParticle.get())->SetMinTime(1.0f);
		static_cast<CParticle*>(pParticle.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
		static_cast<CParticle*>(pParticle.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
		static_cast<CParticle*>(pParticle.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
		static_cast<CParticle*>(pParticle.get())->SetMaxSpawnTime(0.05f);
		static_cast<CParticle*>(pParticle.get())->SetShader("ParticleBullet");

	}

	std::shared_ptr<CComponent> pParticle = CreateComponent<CParticle>("FireParticle");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pParticle);

	static_cast<CParticle*>(pParticle.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_fireaura_001_TEX_KJS");
	static_cast<CParticle*>(pParticle.get())->SetShader("ParticleBlackClip");
	static_cast<CParticle*>(pParticle.get())->SetCountX(6);
	static_cast<CParticle*>(pParticle.get())->SetCountY(5);
	static_cast<CParticle*>(pParticle.get())->SetImageSize(Vector2(1024.f, 1024.f));
	static_cast<CParticle*>(pParticle.get())->SetMaxFrame(30);
	static_cast<CParticle*>(pParticle.get())->SetMaxScale(Vector3(32.f, 32.f, 1.f));
	static_cast<CParticle*>(pParticle.get())->SetMinScale(Vector3(32.f, 32.f, 1.f));
	static_cast<CParticle*>(pParticle.get())->SetMaxTime(1.0f);
	static_cast<CParticle*>(pParticle.get())->SetMinTime(1.0f);
	static_cast<CParticle*>(pParticle.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pParticle.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pParticle.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CParticle*>(pParticle.get())->SetMaxSpawnTime(1.f);

	std::shared_ptr<CComponent> pLight = CreateComponent<CLight>("FireLight");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pLight);

	static_cast<CLight*>(pLight.get())->SetLightType(LIGHT_TYPE::POINT);
	static_cast<CLight*>(pLight.get())->SetRange(128.f);
	static_cast<CLight*>(pLight.get())->SetDifColor(1.f, 0.f, 0.f, 1.f);
	static_cast<CLight*>(pLight.get())->SetAmbColor(1.f, 0.f, 0.f, 1.f);
	static_cast<CLight*>(pLight.get())->SetSpcColor(1.f, 0.f, 0.f, 1.f);
	static_cast<CLight*>(pLight.get())->SetEmiColor(1.f, 0.f, 0.f, 1.f);
	static_cast<CLight*>(pLight.get())->SetAttn(0.f, 0.09f, 0.f);

	return true;
}

void CBullet3D::Start()
{
	CGameObject::Start();

	m_vDir = GetWorldAxis(AXIS_Z);
}

void CBullet3D::Update(float fTime)
{
	CGameObject::Update(fTime);

	std::shared_ptr<CComponent> pRoot = GetRootComponent();

	std::shared_ptr<CTransform> pTransform =static_cast<CSceneComponent*>(pRoot.get())->GetTransform();

	static_cast<CTransform*>(pTransform.get())->AddQuaternionRotAxis(m_vDir,fTime * 120.f);

	float fDist = m_fSpeed * fTime;

	AddWorldPos(m_vDir * fDist);

	m_fDist += fDist;

	if (m_fDist >= m_fLimitDist)
	{
		Destroy();
	}
}

void CBullet3D::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CBullet3D::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CBullet3D::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CBullet3D::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CBullet3D::Clone()
{
	return std::shared_ptr<CGameObject>(new CBullet3D(*this));
}

void CBullet3D::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CBullet3D::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CBullet3D::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strDest = pDest->GetName();

	if (strDest == "MonsterBody")
	{
		GET_SINGLE(CSoundManager)->Play("Sorcerer_FireBall_Explosion");
		std::shared_ptr<CGameObject> pObj = m_pScene->CreateObject<CParticle3D>("explosion", m_pScene->FindLayer(PARTICLE_LAYER));

		static_cast<CParticle3D*>(pObj.get())->SetLimitTime(1.f);

		std::shared_ptr<CComponent> _pCom = pObj->CreateComponent<CParticle>("explosion");

		pObj->SetRootComponent(_pCom);

		static_cast<CParticle*>(_pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
		static_cast<CParticle*>(_pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
		static_cast<CParticle*>(_pCom.get())->SetMaxScale(Vector3(80.f, 80.f, 1.f));
		static_cast<CParticle*>(_pCom.get())->SetMinScale(Vector3(80.f, 80.f, 1.f));
		static_cast<CParticle*>(_pCom.get())->SetMaxTime(1.f);
		static_cast<CParticle*>(_pCom.get())->SetMinTime(1.f);
		static_cast<CParticle*>(_pCom.get())->SetMaxSpawnTime(1.f);
		static_cast<CParticle*>(_pCom.get())->SetCountX(4);
		static_cast<CParticle*>(_pCom.get())->SetCountY(4);
		static_cast<CParticle*>(_pCom.get())->SetMaxFrame(16);
		static_cast<CParticle*>(_pCom.get())->SetImageSize(Vector2(1024.f, 1024.f));
		static_cast<CParticle*>(_pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_Explosion_Sub_08_White6");

		pObj->SetWorldPos(pSrc->GetCrs());

		if (!static_cast<CMonster3D*>(pDest->GetObj())->IsDie())
		{
			Destroy();
		}
	}
	else if (strDest == "GoblinBody")
	{
		GET_SINGLE(CSoundManager)->Play("Sorcerer_FireBall_Explosion");
		std::shared_ptr<CGameObject> pObj = m_pScene->CreateObject<CParticle3D>("explosion", m_pScene->FindLayer(PARTICLE_LAYER));

		static_cast<CParticle3D*>(pObj.get())->SetLimitTime(1.f);

		std::shared_ptr<CComponent> _pCom = pObj->CreateComponent<CParticle>("explosion");

		pObj->SetRootComponent(_pCom);

		static_cast<CParticle*>(_pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
		static_cast<CParticle*>(_pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
		static_cast<CParticle*>(_pCom.get())->SetMaxScale(Vector3(80.f, 80.f, 1.f));
		static_cast<CParticle*>(_pCom.get())->SetMinScale(Vector3(80.f, 80.f, 1.f));
		static_cast<CParticle*>(_pCom.get())->SetMaxTime(1.f);
		static_cast<CParticle*>(_pCom.get())->SetMinTime(1.f);
		static_cast<CParticle*>(_pCom.get())->SetMaxSpawnTime(1.f);
		static_cast<CParticle*>(_pCom.get())->SetCountX(4);
		static_cast<CParticle*>(_pCom.get())->SetCountY(4);
		static_cast<CParticle*>(_pCom.get())->SetMaxFrame(16);
		static_cast<CParticle*>(_pCom.get())->SetImageSize(Vector2(1024.f, 1024.f));
		static_cast<CParticle*>(_pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_Explosion_Sub_08_White6");

		pObj->SetWorldPos(pSrc->GetCrs());

	}
}

void CBullet3D::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CBullet3D::CollisionLast(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
