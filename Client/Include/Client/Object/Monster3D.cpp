#include "Monster3D.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"
#include "Scene/Scene.h"
#include "Resource/ResourceManager.h"
#include "Resource/Material.h"
#include "Component/ColliderSphere.h"
#include "Component/Terrain.h"
#include "Component/ColliderOBB.h"
#include "Component/Paperburn.h"
#include "../State.h"
#include "Player3D.h"
#include "../EditorFlags.h"
#include "LootingWindow.h"
#include "../Item.h"
#include "Coin.h"
#include "Particle3D.h"
#include "Component/Particle.h"
#include "UI/Bar.h"
#include "Component/Camera.h"
#include "CameraManager.h"
#include "Device.h"
#include "UI/UFont.h"
#include "Sound/SoundManager.h"

CMonster3D::CMonster3D()	:
	m_fTime(0.f)
	, m_pTerrain(nullptr)
	, m_iHP(500)
	, m_pAni(nullptr)
	, m_bFight(false)
	, m_fDist(0.f)
	, m_fLimitDist(500.f)
	, m_bDie(false)
	, m_iHPMax(m_iHP)
	, m_fSpeed(150.f)
	,m_pState(nullptr)
	, m_iAttackMax(0)
	, m_iAttackMin(0)
	, m_iArmorMax(0)
	, m_iArmorMin(0)
	, m_bRooted(false)
	, m_iLootCount(0)
	, m_iGold(0)
	, m_bDamaged(false)
	, m_bAir(false)
{
	m_iObjType = static_cast<int>(OBJECT_CLASS_TYPE::OCT_MINION);
	m_pLootItems = new char* [MAX_LOOTING];
	memset(m_pLootItems, 0, sizeof(char*) * MAX_LOOTING);

	for (int i = 0; i < MAX_LOOTING; i++)
	{
		m_pLootItems[i] = new char[MAX_PATH];
		memset(m_pLootItems[i], 0, MAX_PATH);
	}
}

CMonster3D::CMonster3D(const CMonster3D& mon)	:
	CGameObject(mon)	
	, m_fTime(mon.m_fTime)
	, m_pTerrain(mon.m_pTerrain)
	, m_iHP(mon.m_iHP)
	, m_pAni(nullptr)
	, m_bFight(mon.m_bFight)
	, m_fDist(mon.m_fDist)
	, m_fLimitDist(mon.m_fLimitDist)
	, m_bDie(mon.m_bDie)
	, m_iHPMax(mon.m_iHPMax)
	, m_fSpeed(mon.m_fSpeed)
	, m_pState(mon.m_pState)
	, m_iAttackMax(mon.m_iAttackMax)
	, m_iAttackMin(mon.m_iAttackMin)
	, m_iArmorMax(mon.m_iArmorMax)
	, m_iArmorMin(mon.m_iArmorMin)
	, m_bRooted(mon.m_bRooted)
	, m_iLootCount(mon.m_iLootCount)
	, m_iGold(mon.m_iGold)
	, m_pHPBar(FindComponentByType<CBar>())
	, m_pName(FindComponentByType<CUFont>())
	, m_bDamaged(mon.m_bDamaged)
	, m_bAir(mon.m_bAir)
{
	if (m_pState)
	{
		m_pState = m_pState->Clone();
	}

	m_pLootItems = new char* [MAX_LOOTING];
	memset(m_pLootItems, 0, sizeof(char*)* MAX_LOOTING);

	for (int i = 0; i < MAX_LOOTING; i++)
	{
		m_pLootItems[i] = new char[MAX_PATH];
		memcpy_s(m_pLootItems[i], MAX_PATH, mon.m_pLootItems[i], MAX_PATH);
	}
}

CMonster3D::~CMonster3D()
{
	SAFE_RELEASE(m_pAni);
	SAFE_DELETE(m_pState);

	for (int i = 0; i < MAX_LOOTING; i++)
	{
		SAFE_DELETE_ARRAY(m_pLootItems[i]);
	}

	SAFE_DELETE_ARRAY(m_pLootItems);
}

void CMonster3D::ChangeState(CState* pState)
{
	SAFE_DELETE(m_pState);
	m_pState = pState;
}

void CMonster3D::ChangeSequenceAll(const char* pSequence)
{
}

void CMonster3D::AddLootItem(const char* pItem)
{	
	strcpy_s(m_pLootItems[m_iLootCount++], MAX_PATH, pItem);
}

bool CMonster3D::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect","bReaction3-00", false, "Cyclops\\bReaction3-00.ogg");

	if (!CGameObject::Init())
		return false;

	SetShadowAll(true);
	GET_SINGLE(CResourceManager)->LoadTexture("FX_Target_001_TEX_KKJ", L"FX_Resource_KKJ_04\\Texture2D\\FX_Target_001_TEX_KKJ.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_SelectRing_001_TEX_KKJ", L"FX_Resource_KKJ_04\\Texture2D\\FX_SelectRing_001_TEX_KKJ.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("32_UI2_Waypoint_Manager_Start_icon", L"UI_Share_Etc\\Texture2D\\32_UI2_Waypoint_Manager_Start_icon.dds");

	m_pHPBar = CreateComponent<CBar>("HPBar");

	std::shared_ptr<CComponent> pRoot = GetRootComponent();

	static_cast<CSceneComponent*>(pRoot.get())->AddChild(m_pHPBar);

	static_cast<CBar*>(m_pHPBar.get())->SetTexture("32_UI2_Waypoint_Manager_Start_icon");
	static_cast<CBar*>(m_pHPBar.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CBar*>(m_pHPBar.get())->SetUV(Vector2(505.f, 533.f), Vector2(729.f, 539.f));
	static_cast<CBar*>(m_pHPBar.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CBar*>(m_pHPBar.get())->SetWorldScale(224.f, 6.f, 0.f);
	static_cast<CBar*>(m_pHPBar.get())->SetInheritRotX(false);
	static_cast<CBar*>(m_pHPBar.get())->SetInheritRotY(false);
	static_cast<CBar*>(m_pHPBar.get())->SetInheritRotZ(false);

	m_pName = CreateComponent<CUFont>("NameFont");

	static_cast<CSceneComponent*>(pRoot.get())->AddChild(m_pName);

	static_cast<CUFont*>(m_pName.get())->SetRelativePosY(GetMeshSize().y *1.1f + 10.f);
	static_cast<CUFont*>(m_pName.get())->SetRect(-100.f, 0.f, 100.f, 0.f);
	static_cast<CUFont*>(m_pName.get())->SetText(GetName().c_str());
	static_cast<CUFont*>(m_pName.get())->Set3D();
	static_cast<CUFont*>(m_pName.get())->SetShadowOffset(Vector3(2.f, -2.f, 0.f));
	static_cast<CUFont*>(m_pName.get())->SetShadowColor(0.f, 0.f, 0.f, 1.f);

	SetGravity(true);

	return true;
}

void CMonster3D::Start()
{
	CGameObject::Start();

	m_pTerrain = CTerrain::GetMainTerrain();
}

void CMonster3D::Update(float fTime)
{
	CGameObject::Update(fTime);
	//AddWorldPosY(-100.f * fTime);
	Vector3 vPos = GetWorldPos();
	float fHeight  = m_pTerrain->GetHeight(vPos);


	if (vPos.y < fHeight)
	{
		float fSpeedY = GetSpeed().y;
		if ( fSpeedY< 0.f)
		{
			SetSpeedY(0.f);
		}
		if (IsAir())
		{
			std::shared_ptr<CGameObject> pParticle = m_pScene->CreateCloneObj("DarkDustParticle", "DarkDustParticle",
				m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

			if (pParticle)
			{
				pParticle->SetWorldPos(GetWorldPos());
			}

			CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

			if (pCam)
			{
				pCam->SetShake(true);
				pCam->SetShakePower(-fSpeedY / 20.f);
				pCam->SetShakeMass(50.f);
				pCam->SetFrequency(32 * PI);
			}

			GET_SINGLE(CSoundManager)->Play("bReaction3-00");

			SetAir(false);
		}
		SetWorldPosY(fHeight);
	}

	if (m_bDie)
		return;

	if (m_pPlayer)
	{
		int iHP = static_cast<CPlayer3D*>(m_pPlayer.get())->GetHP();

		if (iHP <= 0)
		{
			m_pPlayer = nullptr;
		}
	}

	if (m_pHPBar)
	{
		m_pHPBar->SetEnable(static_cast<bool>(m_pPlayer));
	}

	if (m_pName)
	{
		m_pName->SetEnable(static_cast<bool>(m_pPlayer));
	}
}

void CMonster3D::PostUpdate(float fTime)
{
	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	if (pCam)
	{
		if (m_pHPBar)
		{
			if (m_pHPBar->IsEnable())
			{
				Vector3 vMeshSize = GetMeshSize();

				Vector3 vBarPos = GetBoneWorldPos() + Vector3(0.f, vMeshSize.y * 1.1f, 0.f);

				Matrix matVP = pCam->GetVP();

				Vector3 vProjPos = vBarPos.TranslationCoor(matVP);

				if (vProjPos.z < 0.f || vProjPos.z>1.f)
				{
					m_pHPBar->SetEnable(false);
					if (m_pName)
					{
						m_pName->SetEnable(false);
					}
				}

				Resolution tRS = RESOLUTION;

				vProjPos *= Vector3(static_cast<float>(tRS.iWidth) / 2.f, static_cast<float>(tRS.iHeight) / 2.f, 0.f);

				static_cast<CBar*>(m_pHPBar.get())->SetWorldPos(vProjPos);
				static_cast<CBar*>(m_pHPBar.get())->SetPrg(m_iHP / static_cast<float>(m_iHPMax));
			}
		}
	}

	CGameObject::PostUpdate(fTime);
}

void CMonster3D::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CMonster3D::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CMonster3D::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CMonster3D::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CMonster3D::Clone()
{
	return std::shared_ptr<CGameObject>();
}

void CMonster3D::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CMonster3D::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

}

void CMonster3D::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strSrc = pSrc->GetName();
	std::string strDest = pDest->GetName();
	if ((strDest == "BulletBody" || strDest == "BladeBody") && ( strSrc== "MonsterBody" ||  strSrc == "GoblinBody"))
	{
		CalcDamage();
	}
}

void CMonster3D::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strSrc = pSrc->GetName();
	std::string strDest = pDest->GetName();

	if (strSrc == strDest)
	{
		Vector3 vDest = pDest->GetWorldPos();
		Vector3 vSrc = pDest->GetWorldPos();

		Vector3 vDir = vSrc - vDest;
		

		Vector3 vDist = vDir - pSrc->GetCrs();

		float fDist = vDist.Length();

		if (fDist != 0.f)
		{
			vDir.Normalize();

			pSrc->GetObj()->AddWorldPos(vDir * fDist);
		}
	}

	if (strDest == "MouseBody")
	{
		if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
		{
			if (m_bDie)
			{
				if (!m_bRooted)
				{
					m_bRooted = true;
					std::shared_ptr<CGameObject> pObj = m_pScene->CreateCloneObj(
						"Looting", "Looting", m_pScene->FindLayer(UI_LAYER), m_pScene->GetSceneType());

					for (int i = 0; i < m_iLootCount; i++)
					{
						CItem* pItem = CItem::FindItem(m_pLootItems[i]);

						float fRand = (rand() % 10000) / 10000.f;

						if (pItem)
						{
							float fRate = pItem->GetDropRate();

							if (fRand < fRate)
							{
								static_cast<CLootingWindow*>(pObj.get())->CreateItem(pItem);
							}
						}
					}

					static_cast<CLootingWindow*>(pObj.get())->SetLootingCallBack(this, &CMonster3D::LootingCallBack);
				}
			}
			else
			{
				if (!m_pPlayer)
				{
					std::shared_ptr<CGameObject> pPlayer = m_pScene->FindLayer(ALBEDO_LAYER)->FindObj("Player");

					static_cast<CPlayer3D*>(pPlayer.get())->SetTarget(this);
				}
				else
				{
					static_cast<CPlayer3D*>(m_pPlayer.get())->SetTarget(this);
				}
			}
		}
	}
}

void CMonster3D::CollisionLast(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CMonster3D::Move(float fTime)
{
	m_pState->Move(this, fTime);
}

void CMonster3D::Attack(float fTime)
{
	m_pState->Attack(this, fTime);
}

void CMonster3D::AttackAround(float fTime)
{
	m_pState->AttackAround(this, fTime);
}

void CMonster3D::HeavyAttack(float fTime)
{
	m_pState->HeavyAttack(this, fTime);
}

void CMonster3D::LongAttack(float fTime)
{
	m_pState->LongAtk(this, fTime);
}

void CMonster3D::Idle(float fTime)
{
	m_pState->Idle(this, fTime);
}
void CMonster3D::LongMove(float fTime)
{
	m_pState->LongMove(this, fTime);
}
void CMonster3D::Attacked()
{
}
// 스마트 포인터

void CMonster3D::LootingCallBack()
{
	std::shared_ptr<CComponent> pRoot = GetRootComponent();
	static_cast<CPaperBurn*>(m_pPaper.get())->ToggleBurn();
	static_cast<CSceneComponent*>(pRoot.get())->SetInstance(false);
}

void CMonster3D::CalcDamage(int _iAttackMax, int _iAttackMin)
{
	if (!m_pPlayer)
	{
		CLayer* pLayer = m_pScene->FindLayer(ALBEDO_LAYER);
		if (pLayer)
		{
			std::shared_ptr<CGameObject> pPlayer = pLayer->FindObj("Player");

			SetPlayer(pPlayer);
		}
	}
	if (!m_bFight)
	{
		m_bFight = true;
	}

	int iAttackMax = static_cast<CPlayer3D*>(m_pPlayer.get())->GetAttackMax();
	if (_iAttackMax == -1)
	{
		iAttackMax = static_cast<CPlayer3D*>(m_pPlayer.get())->GetAttackMax();
	}

	int iAttackMin = static_cast<CPlayer3D*>(m_pPlayer.get())->GetAttackMin();
	if (_iAttackMin == -1)
	{
		iAttackMin = static_cast<CPlayer3D*>(m_pPlayer.get())->GetAttackMin();
	}
	int iArmor = rand() % (m_iArmorMax - m_iArmorMin + 1) + m_iArmorMin;
	int iAttack = iAttackMin + rand() % (iAttackMax - iAttackMin + 1);
	int iDamage = iAttack - iArmor;
	iDamage = iDamage <= 0 ? 1 : iDamage;

	m_iHP -= iDamage;

	if (!m_bDie)
	{
		std::shared_ptr<CGameObject> pFont = m_pScene->CreateCloneObj("PlayerDamageFont", "PlayerDamageFont", m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

		std::shared_ptr<CComponent> pCom = pFont->GetRootComponent();

		static_cast<CParticle*>(pCom.get())->SetCountX(iDamage);

		float fY = GetMeshSize().y * GetWorldScale().y * (GetPivot().y + 1.f);

		pFont->SetWorldPos(GetWorldPos() + Vector3(0.f, fY, 0.f));

		if (m_iHP <= 0)
		{
			m_bDie = true;
			m_pState->Die(this, 0.f);

			for (int i = 0; i < 3; i++)
			{
				std::shared_ptr<CGameObject> pCoin =
					m_pScene->CreateCloneObj("Coin", "Coin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());

				if (pCoin)
				{
					pCoin->SetWorldPos(GetWorldPos());

					static_cast<CCoin*>(pCoin.get())->SetGold(m_iGold);
				}
			}
		}
		else
		{
			Attacked();
			m_pState->Attacked(this, 0.f);
		}
	}
}

bool CMonster3D::DamagedTransitionFunc()
{
	return m_bDamaged;
}

void CMonster3D::DamagedEnd()
{
	m_bDamaged = false;
}
