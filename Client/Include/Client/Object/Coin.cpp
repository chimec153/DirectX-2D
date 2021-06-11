#include "Coin.h"
#include "Component/Renderer.h"
#include "Component/ColliderSphere.h"
#include "Resource/Animation.h"
#include "Player3D.h"
#include "Scene/Scene.h"
#include "Component/Terrain.h"
#include "Sound/SoundManager.h"

CCoin::CCoin()	:
	m_vSpeed()
	, m_iGold(0)
	, m_pPlayer(nullptr)
	, m_fRatio(0.f)
{
}

CCoin::CCoin(const CCoin& obj)	:
	CGameObject(obj)
	, m_vSpeed(obj.m_vSpeed)
	, m_iGold(obj.m_iGold)
	, m_pPlayer(obj.m_pPlayer)
	, m_fRatio(obj.m_fRatio)

{
	std::shared_ptr<CComponent> pCollider = FindComponentByType<CColliderSphere>();
	if (pCollider)
	{
		static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CCoin>(COLLISION_STATE::CS_INIT, this, &CCoin::CollisionInit);
		static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CCoin>(COLLISION_STATE::CS_STAY, this, &CCoin::CollisionStay);
		static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CCoin>(COLLISION_STATE::CS_END, this, &CCoin::CollisionEnd);
	}
}

CCoin::~CCoin()
{
}

bool CCoin::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "TBA_10Gold", false, 
		"TBA_10Gold.ogg");

	if(!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CSceneComponent>("Renderer");

	SetRootComponent(pRenderer);

	static_cast<CSceneComponent*>(pRenderer.get())->SetMesh("Coin");
	static_cast<CSceneComponent*>(pRenderer.get())->SetPivot(0.f, -0.1f, 0.5f);
	static_cast<CSceneComponent*>(pRenderer.get())->SetWorldScale(0.2f, 0.2f, 0.2f);
	static_cast<CSceneComponent*>(pRenderer.get())->SetWorldRotX(90.f);

	std::shared_ptr<CComponent> pCollider = CreateComponent<CColliderSphere>("CoinBody");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pCollider);

	static_cast<CColliderSphere*>(pCollider.get())->SetRadius(3.f);
	static_cast<CColliderSphere*>(pCollider.get())->AddRelativePosZ(-3.f);

	//CAnimation* pAni = CAnimation::CreateAnimation<CAnimation>("CoinAni", "CoinBone", "Coin\\Coin_Dice_Anim.bne");

	//pAni->AddAnimationSequence("unarmedwait", "Coin_unarmedwait");
	//pAni->AddAnimationSequence("Spawn", "Coin_Spawn");
	//pAni->AddAnimationSequence("Death", "Coin_Death");
	//pAni->AddAnimationSequence("deathwait", "Coin_deathwait");

	//static_cast<CSceneComponent*>(pRenderer.get())->SetAnimation(pAni);

	//SAFE_RELEASE(pAni);

	return true;
}

void CCoin::Start()
{
	CGameObject::Start();

	m_vSpeed = Vector3((rand() % 100 / 100.f - 0.5f) * 100.f, rand() % 50 +100.f, (rand() % 100 / 100.f - 0.5f) * 100.f);

	CLayer* pLayer = m_pScene->FindLayer(ALBEDO_LAYER);

	m_pPlayer = pLayer->FindObj("Player");

	m_pTerrain = CTerrain::GetMainTerrain();
}

void CCoin::Update(float fTime)
{
	CGameObject::Update(fTime);

	AddWorldRotY(90.f * fTime);

	if (m_pPlayer)
	{
		Vector3 vTargetPos = m_pPlayer->GetWorldPos() + Vector3(0.f, 25.f, 0.f);

		Vector3 vPos = GetWorldPos();

		Vector3 vDir = vTargetPos - vPos;

		float fDist = vDir.Length();

		if (fDist <= 75.f)
		{
			m_fRatio += fTime* 0.8f;

			vDir /= fDist;

			Vector3 vControlDir = -GetWorldAxis(AXIS_Z) + vDir;
			vControlDir.Normalize();

			Vector3 vControlDir2 = m_pPlayer->GetWorldAxis(AXIS_Y) - vDir;
			vControlDir2.Normalize();

			Vector3 vControlPoint1 = vPos + vControlDir * fDist / 2.f;

			Vector3 vControlPoint2 = vTargetPos + vControlDir2 * fDist / 2.f;

			Vector3 vP0 = vControlPoint1 - vPos;

			vP0 = vP0 * m_fRatio + vPos;

			Vector3 vP1 = vControlPoint2 - vControlPoint1;

			vP1 = vP1 * m_fRatio + vControlPoint1;

			Vector3 vP2 = vTargetPos - vControlPoint2;

			vP2 = vP2 * m_fRatio + vControlPoint2;

			Vector3 _vP0 = vP1 - vP0;

			_vP0 = _vP0 * m_fRatio + vP0;

			Vector3 _vP1 = vP2 - vP1;

			_vP1 = _vP1 * m_fRatio + vP1;

			Vector3 __vP0 = _vP1 - _vP0;

			__vP0 = __vP0 * m_fRatio + _vP0;

			SetWorldPos(__vP0);

			return;
		}
	}

	m_vSpeed.y -= 100.f * fTime;

	AddWorldPos(m_vSpeed* fTime);

	Vector3 vPos = GetWorldPos();

	if (m_pTerrain)
	{
		float fHeight = m_pTerrain->GetHeight(vPos);

		if (vPos.y <= fHeight)
		{
			SetWorldPosY(fHeight);
			m_vSpeed.y *= -0.5f;
			m_vSpeed.x = 0.f;
			m_vSpeed.z = 0.f;
		}

	}
	else
	{

		if (vPos.y <= 0.f)
		{
			SetWorldPosY(0.f);
			m_vSpeed.y *= -0.5f;
			m_vSpeed.x = 0.f;
			m_vSpeed.z = 0.f;
		}
	}
}

void CCoin::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CCoin::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CCoin::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CCoin::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CCoin::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CCoin::Clone()
{
	return std::shared_ptr<CGameObject>(new CCoin(*this));
}

void CCoin::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CCoin::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CCoin::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (pDest->GetName() == "PlayerBody")
	{
		GET_SINGLE(CSoundManager)->Play("TBA_10Gold");
		static_cast<CPlayer3D*>(pDest->GetObj())->AddGold(m_iGold);
		Destroy();
	}
}

void CCoin::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (pDest->GetName() == "PlayerBody")
	{
		static_cast<CPlayer3D*>(pDest->GetObj())->AddGold(m_iGold);
		Destroy();
	}
}

void CCoin::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
