#include "BlueFlame.h"
#include "Component/Renderer.h"
#include "Component/Particle.h"
#include "Component/ColliderSphere.h"
#include "Component/Transform.h"
#include "Resource/Material.h"
#include "Component/Light.h"
#include "Resource/ResourceManager.h"
#include "Scene/Scene.h"
#include "Particle3D.h"
#include "Sound/SoundManager.h"

CBlueFlame::CBlueFlame()	:
	m_fSpeed(300.f)
	, m_fDist(0.f)
	, m_fLimitDist(900.f)
{
}

CBlueFlame::CBlueFlame(const CBlueFlame& obj)	:
	CGameObject(obj)
	, m_fSpeed(obj.m_fSpeed)
	, m_fDist(obj.m_fDist)
	, m_fLimitDist(obj.m_fLimitDist)
{
	std::shared_ptr<CComponent> pCol = FindComponentByType<CColliderSphere>();

	CColliderSphere* pCom = static_cast<CColliderSphere*>(pCol.get());

	pCom->SetCallBack<CBlueFlame>(COLLISION_STATE::CS_INIT, this, &CBlueFlame::CollisionInit);
	pCom->SetCallBack<CBlueFlame>(COLLISION_STATE::CS_STAY, this, &CBlueFlame::CollisionStay);
	pCom->SetCallBack<CBlueFlame>(COLLISION_STATE::CS_LAST, this, &CBlueFlame::CollisionLast);
}

CBlueFlame::~CBlueFlame()
{
}

bool CBlueFlame::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Sorcerer_BlastMine_Explosion", false,
		"Skill\\Sorcerer_BlastMine_Explosion.ogg");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_Ice_001_TEX_HKB", L"FX_Resource_HKB_05\\Texture2D\\FX_Ice_001_TEX_HKB.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_fumefie_005_TEX_KJS", L"FX_Resource_KJS_01\\Texture2D\\FX_fumefie_005_TEX_KJS.dds");

	if(!CGameObject::Init())
		return false;
	std::shared_ptr<CComponent> pRenderer = CreateComponent<CSceneComponent>("BulletRenderer");

	SetRootComponent(pRenderer);

	std::shared_ptr<CComponent> pCol = CreateComponent<CColliderSphere>("BulletBody");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pCol);

	CColliderSphere* pCom = static_cast<CColliderSphere*>(pCol.get());
	pCom->SetRadius(40.f);

	int iCount = 5;
	for (int i = 0; i < iCount; i++)
	{
		std::shared_ptr<CComponent> pParticle = CreateComponent<CParticle>("BlueParticle");

		static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pParticle);

		static_cast<CParticle*>(pParticle.get())->AddRelativePos(27.f * cosf(PI / iCount * 2.f * i), 27.f * sinf(PI / iCount * 2.f * i), 0.f);
		static_cast<CParticle*>(pParticle.get())->SetShader("ParticleBlackClip");
		static_cast<CParticle*>(pParticle.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_fumefie_005_TEX_KJS");
		static_cast<CParticle*>(pParticle.get())->SetCountX(6);
		static_cast<CParticle*>(pParticle.get())->SetCountY(6);
		static_cast<CParticle*>(pParticle.get())->SetImageSize(Vector2(1024.f, 1024.f));
		static_cast<CParticle*>(pParticle.get())->SetMaxFrame(36);
		static_cast<CParticle*>(pParticle.get())->SetMaxScale(Vector3(32.f, 32.f, 1.f));
		static_cast<CParticle*>(pParticle.get())->SetMinScale(Vector3(32.f, 32.f, 1.f));
		static_cast<CParticle*>(pParticle.get())->SetMaxTime(1.0f);
		static_cast<CParticle*>(pParticle.get())->SetMinTime(1.0f);
		static_cast<CParticle*>(pParticle.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
		static_cast<CParticle*>(pParticle.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
		static_cast<CParticle*>(pParticle.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
		static_cast<CParticle*>(pParticle.get())->SetMaxSpawnTime(0.05f);
	}

	std::shared_ptr<CComponent> pParticle = CreateComponent<CParticle>("BlueParticle");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pParticle);

	static_cast<CParticle*>(pParticle.get())->SetShader("ParticleBlackClip");
	static_cast<CParticle*>(pParticle.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_fumefie_005_TEX_KJS");
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

	std::shared_ptr<CComponent> pLight = CreateComponent<CLight>("BlueLight");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pLight);

	static_cast<CLight*>(pLight.get())->SetLightType(LIGHT_TYPE::POINT);
	static_cast<CLight*>(pLight.get())->SetRange(128.f);
	static_cast<CLight*>(pLight.get())->SetDifColor( 0.f, 0.f, 1.f, 1.f);
	static_cast<CLight*>(pLight.get())->SetAmbColor(0.f, 0.f, 1.f, 1.f);
	static_cast<CLight*>(pLight.get())->SetSpcColor(0.f, 0.f, 1.f, 1.f);
	static_cast<CLight*>(pLight.get())->SetEmiColor(0.f, 0.f, 1.f, 1.f);
	static_cast<CLight*>(pLight.get())->SetAttn(0.f, 0.09f, 0.f);

	return true;
}

void CBlueFlame::Start()
{
	CGameObject::Start();

	m_vDir = GetWorldAxis(AXIS_Z);
}

void CBlueFlame::Update(float fTime)
{
	CGameObject::Update(fTime);

	std::shared_ptr<CComponent> pRoot = GetRootComponent();

	std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pRoot.get())->GetTransform();

	static_cast<CTransform*>(pTransform.get())->AddQuaternionRotAxis(m_vDir, fTime * 240.f);

	float fDist = m_fSpeed * fTime;

	AddWorldPos(m_vDir * fDist);

	m_fDist += fDist;

	if (m_fDist >= m_fLimitDist)
	{
		Destroy();
	}
}

void CBlueFlame::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CBlueFlame::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CBlueFlame::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CBlueFlame::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CBlueFlame::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CBlueFlame::Clone()
{
	return std::shared_ptr<CGameObject>(new CBlueFlame(*this));
}

void CBlueFlame::Save(FILE * pFile)
{
	CGameObject::Save(pFile);
}

void CBlueFlame::Load(FILE * pFile)
{
	CGameObject::Load(pFile);
}

void CBlueFlame::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strDest = pDest->GetName();

	if (strDest == "MonsterBody" || strDest == "GoblinBody")
	{
		GET_SINGLE(CSoundManager)->Play("Sorcerer_BlastMine_Explosion");
		std::shared_ptr<CGameObject> pObj = m_pScene->CreateObject<CParticle3D>("explosion", m_pScene->FindLayer(PARTICLE_LAYER));

		static_cast<CParticle3D*>(pObj.get())->SetLimitTime(1.5f);

		std::shared_ptr<CComponent> _pCom = pObj->CreateComponent<CParticle>("explosion");

		pObj->SetRootComponent(_pCom);

		static_cast<CParticle*>(_pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
		static_cast<CParticle*>(_pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
		static_cast<CParticle*>(_pCom.get())->SetMaxScale(Vector3(20.f, 20.f, 1.f));
		static_cast<CParticle*>(_pCom.get())->SetMinScale(Vector3(20.f, 20.f, 1.f));
		static_cast<CParticle*>(_pCom.get())->SetMaxTime(1.f);
		static_cast<CParticle*>(_pCom.get())->SetMinTime(1.f);
		static_cast<CParticle*>(_pCom.get())->SetMaxSpawnTime(1.f);
		static_cast<CParticle*>(_pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_Ice_001_TEX_HKB");
		static_cast<CParticle*>(_pCom.get())->SetMove(true);
		static_cast<CParticle*>(_pCom.get())->SetAngleAccel(20.f);
		static_cast<CParticle*>(_pCom.get())->SetMaxSpeed(100.f);
		static_cast<CParticle*>(_pCom.get())->SetMinSpeed(80.f);
		static_cast<CParticle*>(_pCom.get())->SetRange(Vector3(50.f, 50.f, 50.f));
		static_cast<CParticle*>(_pCom.get())->SetSpawnCount(10);
		static_cast<CParticle*>(_pCom.get())->SetMaxCount(3000);

		std::shared_ptr<CComponent> pCom = pObj->CreateComponent<CParticle>("explosion");

		static_cast<CSceneComponent*>(_pCom.get())->AddChild(pCom);

		static_cast<CParticle*>(pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 0.f));
		static_cast<CParticle*>(pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
		static_cast<CParticle*>(pCom.get())->SetMaxScale(Vector3(160.f, 160.f, 1.f));
		static_cast<CParticle*>(pCom.get())->SetMinScale(Vector3(120.f, 120.f, 1.f));
		static_cast<CParticle*>(pCom.get())->SetMaxTime(1.5f);
		static_cast<CParticle*>(pCom.get())->SetMinTime(1.5f);
		static_cast<CParticle*>(pCom.get())->SetMaxSpawnTime(1.5f);
		static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_fumefie_005_TEX_KJS");
		static_cast<CParticle*>(pCom.get())->SetCountX(6);
		static_cast<CParticle*>(pCom.get())->SetCountY(6);
		static_cast<CParticle*>(pCom.get())->SetMaxFrame(36);
		static_cast<CParticle*>(pCom.get())->SetImageSize(Vector2(1024.f, 1024.f));
		static_cast<CParticle*>(pCom.get())->SetShader("ParticleBlackClip");

		pObj->SetWorldPos(pSrc->GetCrs());

		//Destroy();
	}
}

void CBlueFlame::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CBlueFlame::CollisionLast(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
