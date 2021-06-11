#include "Cyclops.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"
#include "Scene/Scene.h"
#include "Component/ColliderSphere.h"
#include "Component/PaperBurn.h"
#include "../Idle.h"
#include "../Fight.h"
#include "Resource/Material.h"
#include "CylopsBall.h"
#include "Component/Transform.h"
#include "../Rage.h"
#include "../Walk.h"
#include "../Die.h"
#include "Particle3D.h"
#include "Player3D.h"
#include "Scene/Scene.h"
#include "RenderManager.h"
#include "CameraManager.h"
#include "Component/Camera.h"
#include "../../Quest.h"
#include "UI/UFont.h"
#include "TargetUI.h"
#include "Resource/ResourceManager.h"
#include "Sound/SoundManager.h"
#include "Portal.h"
#include "Scene/SceneManager.h"
#include "Component/Decal.h"
#include "Component/Particle.h"
CCyclops::CCyclops()	:
	m_fLongAtkTime(0.f)
	, m_fLongAtkTimeLimit(2.5f)
	,m_bLongAtk(false)
	, m_fCoolTime(0.f)
	, m_fCoolTimeLimit(2.5f)
	, m_pLeftHand(nullptr)
	, m_pRightHand(nullptr)
	, m_fModeChangeTime(0.f)
	, m_fModeChangeLimit(2.5f)
	, m_bModeChange(false)
	, m_bLongMove(false)
	, m_bSequenceMode(false)
{
	m_pState = new CIdle;
	
}

CCyclops::CCyclops(const CCyclops& mon)	:
	CMonster3D(mon)
	, m_fLongAtkTime(mon.m_fLongAtkTime)
	, m_fLongAtkTimeLimit(mon.m_fLongAtkTimeLimit)
	, m_bLongAtk(mon.m_bLongAtk)
	, m_fCoolTime(mon.m_fCoolTime)
	, m_fCoolTimeLimit(mon.m_fCoolTimeLimit)
	, m_pLeftHand(mon.m_pLeftHand)
	, m_pRightHand(mon.m_pRightHand)
	, m_fModeChangeTime(mon.m_fModeChangeTime)
	, m_fModeChangeLimit(mon.m_fModeChangeLimit)
	, m_bModeChange(mon.m_bModeChange)
	, m_bLongMove(mon.m_bLongMove)
	, m_bSequenceMode(mon.m_bSequenceMode)
{
	m_pAni = static_cast<CSceneComponent*>(GetRootComponent().get())->GetAnimation();
	m_pAni->AutoTransition("pullAtk_Start", "pullToLoop", "pullAtk_Loop");
	m_pAni->AutoTransition("pullAtk_Loop", "pullToEnd", "pullAtk_End");
	m_pAni->AutoTransition("pullAtk_End", "EndToIdle", "MonsterWait");
	m_pAni->AddTransition("WaitToAttack", "MonsterWait", this, &CCyclops::Transition, "MonsterAttack");
	m_pAni->AddTransition("WaitToDeath", "MonsterWait", this, &CCyclops::ToDeath, "MonsterDeath");
	m_pAni->AddTransition("WaitToLongAttack", "MonsterWait", this, &CCyclops::Transition, "longAtk01_Start");
	m_pAni->AddTransition("WaitToHeavyAttack", "MonsterWait", this, &CCyclops::Transition, "MonsterHeavyAttack");
	m_pAni->AddTransition("WaitToWalk", "MonsterWait", this, &CCyclops::Transition, "MonsterWalk");
	m_pAni->AddTransition("WaitToUltra", "MonsterWait", this, &CCyclops::Transition, "UltraAtk01");
	m_pAni->AddTransition("WaitToLongMove", "MonsterWait", this, &CCyclops::IdleToLongMove, "longMove_Start");
	m_pAni->AutoTransition("MonsterAttack", "AttackToWait", "MonsterWait");
	m_pAni->AddTransition("WaitToFlinch", "MonsterWait", static_cast<CMonster3D*>(this),
		&CMonster3D::DamagedTransitionFunc, "flinch");
	m_pAni->AddTransition("WaitTomodeAlarm_Start", "MonsterWait", this, &CCyclops::Transition, "modeAlarm_Start");
	m_pAni->AddTransition("WalkToDeath", "MonsterWalk", this, &CCyclops::ToDeath, "MonsterDeath");
	m_pAni->AddTransition("WalkToWait", "MonsterWalk", this, &CCyclops::Transition, "MonsterWait");
	m_pAni->AddTransition("WalkToAttack", "MonsterWalk", this, &CCyclops::Transition, "MonsterAttack");
	m_pAni->AddTransition("WalkToLongAtk", "MonsterWalk", this, &CCyclops::Transition, "longAtk01_Start");
	m_pAni->AddTransition("WalkToHeavyAttack", "MonsterWalk", this, &CCyclops::Transition, "MonsterHeavyAttack");
	m_pAni->AddTransition("WalkToUltra", "MonsterWalk", this, &CCyclops::Transition, "UltraAtk01");
	m_pAni->AddTransition("WalkToFlinch", "MonsterWalk", static_cast<CMonster3D*>(this),
		&CMonster3D::DamagedTransitionFunc, "flinch");
	m_pAni->AddTransition("WalkToLongMove", "MonsterWalk", this, &CCyclops::IdleToLongMove, "longMove_Start");
	m_pAni->AddTransition("AttackToWalk", "MonsterAttack", this, &CCyclops::Transition, "MonsterWalk");
	m_pAni->AddTransition("AttackToDeath", "MonsterAttack", this, &CCyclops::ToDeath, "MonsterDeath");
	m_pAni->AddTransition("AttackToMode", "MonsterAttack", this, &CCyclops::Transition, "modeAlarm_Start");
	m_pAni->AutoTransition("UltraAtk01", "UltraAttackToWait", "MonsterWait");
	m_pAni->AutoTransition("MonsterHeavyAttack", "HeavyAttackToWait", "MonsterWait");
	m_pAni->AddTransition("HeavyAttackToDeath", "MonsterHeavyAttack", this, &CCyclops::ToDeath, "MonsterDeath");
	m_pAni->AddTimeLineNotifyFunction("Move", this, &CCyclops::HeavyAttackMove);
	m_pAni->AddTransition("HeavyAttackToWalk", "MonsterHeavyAttack", this, &CCyclops::Transition, "MonsterWalk");
	m_pAni->AddTransition("HeavyAttackToMode", "MonsterHeavyAttack", this, &CCyclops::Transition, "modeAlarm_Start");
	m_pAni->AddTransition("UltraAttackToDeath", "UltraAtk01", this, &CCyclops::ToDeath, "MonsterDeath");
	m_pAni->AddTransition("UltraAttackToWalk", "UltraAtk01", this, &CCyclops::Transition, "MonsterWalk");
	m_pAni->AddTransition("UltraToMode", "UltraAtk01", this, &CCyclops::Transition, "modeAlarm_Start");
	m_pAni->AutoTransition("longAtk01_Start", "LongAtkToLoop", "LongAtk01_Loop");
	m_pAni->AutoTransition("LongAtk01_End_A", "LongAtkToWait", "MonsterWait");
	m_pAni->AddTransition("LongAtkToWalk", "LongAtk01_End_A", this, &CCyclops::Transition, "MonsterWalk");
	m_pAni->AddTransition("LongAtkToEnd", "LongAtk01_Loop", this, &CCyclops::LongAttackA, "LongAtk01_End_A");
	m_pAni->AddTransition("LongAtkToEndB", "LongAtk01_Loop", this, &CCyclops::LongAttackB, "LongAtk01_End_B");
	m_pAni->AutoTransition("LongAtk01_End_B", "LongAtkBToWait", "MonsterWait");
	m_pAni->AddTransition("LongAtkLoopToDeath", "LongAtk01_Loop", this, &CCyclops::ToDeath, "MonsterDeath");
	m_pAni->AddTransition("LongAtkEndToDeath", "LongAtk01_End_A", this, &CCyclops::ToDeath, "MonsterDeath");
	m_pAni->AddTransition("LongAtkStartToDeath", "longAtk01_Start", this, &CCyclops::ToDeath, "MonsterDeath");
	m_pAni->AddTransition("LongAtkLoopToMode", "LongAtk01_Loop", this, &CCyclops::Transition, "modeAlarm_Start");
	m_pAni->AddTransition("LongAtkEndToMode", "LongAtk01_End_A", this, &CCyclops::Transition, "modeAlarm_Start");
	m_pAni->AddTransition("LongAtkStartToMode", "longAtk01_Start", this, &CCyclops::Transition, "modeAlarm_Start");
	m_pAni->AutoTransition("flinch", "FlinchToWait", "MonsterWait");
	m_pAni->AddTransition("FlinchToWalk", "flinch", this, &CCyclops::Transition, "MonsterWalk");
	m_pAni->AutoTransition( "modeAlarm_End", "modeAlarmToWait", "MonsterWait");
	m_pAni->AutoTransition("modeAlarm_Start", "ModeAlarmToLoop", "modeAlarm_Loop");
	m_pAni->AddTransition("modeLoopToEnd", "modeAlarm_Loop", this, &CCyclops::Transition, "modeAlarm_End");
	m_pAni->AutoTransition("longMove_Start", "LongStartToMove", "LongMove_Move");
	m_pAni->AddTransition("LongMoveToEnd", "LongMove_Move", this, &CCyclops::LongMoveToIdle, "longMove_End");
	m_pAni->AutoTransition("longMove_End", "LongMoveToIdle", "MonsterWait");
	m_pAni->AddTimeLineNotifyFunction("LongMoveMove", this, &CCyclops::LongMove);
	m_pAni->AutoTransition("MonsterDeath", "DeathToDeadBody", "deathwait");
	m_pAni->AddTransition("IdleToPullAtk", "MonsterWait", this, &CCyclops::Transition, "pullAtk_Start");
	m_pAni->AddTransitionNotify("FlinchToWait", "flinch", static_cast<CMonster3D*>(this),
		&CMonster3D::DamagedEnd);
	m_pAni->AddTransitionNotify("LongAtkToEndB", "LongAtk01_Loop", this,
		&CCyclops::LongAtkBStart);
	m_pAni->AddTransitionNotify("LongStartToMove", "longMove_Start", this, &CCyclops::LongMoveStart);
	m_pAni->AddNotifyFunction("LongMove", this, &CCyclops::LongMoveEnd);
	m_pAni->AddNotifyFunction("Death", this, &CCyclops::Death);
	m_pAni->AddTransitionNotify("WalkToLongAtk", "MonsterWalk", this, &CCyclops::WalkToLongAtk);
	m_pAni->AddTransitionNotify("WaitToLongAttack", "MonsterWait", this, &CCyclops::WalkToLongAtk);
	m_pAni->AddTransitionNotify("ModeAlarmToLoop", "modeAlarm_Start", this, &CCyclops::ModeChangeStart);
	m_pAni->AddTransitionNotify("modeLoopToEnd", "modeAlarm_Loop", this, &CCyclops::ModeChangeBlurEnd);
	m_pAni->AddTransitionNotify("modeAlarmToWait", "modeAlarm_End", this, &CCyclops::ModeChangeEnd);
	m_pAni->AddNotifyFunction("ModeChange", this, &CCyclops::ModeChangeBlur);
	m_pAni->AddNotifyFunction("ModeChangeEnd", this, &CCyclops::ModeChangeBlurEnd);
	m_pAni->AddNotifyFunction("LongAtkEnd", this, &CCyclops::LongAtk);
	m_pAni->AddNotifyFunction("HeavyStart", this, &CCyclops::HeavyAttackSound);
	//m_pAni->AddNotifyFunction("HeavyNotify", this, &CCyclops::HeavyAttackEnd);
	m_pAni->AddNotifyFunction("LongAtkStart", this, &CCyclops::LongAtkStart);
	m_pAni->AddNotifyFunction("HitGround1", this, &CCyclops::HitGroundLeft);
	m_pAni->AddNotifyFunction("HitGround2", this, &CCyclops::HitGroundRight);
	m_pAni->AddNotifyFunction("HitGround3", this, &CCyclops::HitGroundLeft);
	m_pAni->AddNotifyFunction("HitGround", this, &CCyclops::HitGroundLeft);
	m_pAni->AddNotifyFunction("HitGround", this, &CCyclops::HitGroundRight);
	m_pAni->AddNotifyFunction("AttackStart", this, &CCyclops::AttackSound);
	m_pAni->AddNotifyFunction("Rage", this, &CCyclops::Rage);
	m_pAni->AddNotifyFunction("Damaged", this, &CCyclops::DamagedSound);
	m_pAni->AddNotifyFunction("IdleSound", this, &CCyclops::IdleSound);
	m_pAni->AddNotifyFunction("WalkSound", this, &CCyclops::WalkSound);
	m_pAni->AddNotifyFunction("LongMoveSound", this, &CCyclops::LongMoveSound);
	m_pAni->AddNotifyFunction("LoneAttackSound", this, &CCyclops::LoneAttackSound);
	m_pAni->AddNotifyFunction("LongAtkBRock", this, &CCyclops::LongAtkBRockHold);
	m_pAni->AddNotifyFunction("LongAtk_B_Punch", this, &CCyclops::LongAtk_B_Punch);

	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	m_pLeftHand = pScene->CreateObject<CGameObject>("LeftHand", pScene->FindLayer(COLLIDER_LAYER));
	m_pRightHand = pScene->CreateObject<CGameObject>("RightHand", pScene->FindLayer(COLLIDER_LAYER));

	std::shared_ptr<CComponent> pHandBody = m_pLeftHand->CreateComponent<CColliderSphere>("MonsterHandBody", pScene->FindLayer(COLLIDER_LAYER));
	std::shared_ptr<CComponent> pLeftHandBody = m_pRightHand->CreateComponent<CColliderSphere>("MonsterHandBody", pScene->FindLayer(COLLIDER_LAYER));

	m_pLeftHand->SetRootComponent(pLeftHandBody);
	m_pRightHand->SetRootComponent(pHandBody);

	static_cast<CColliderSphere*>(pHandBody.get())->SetRadius(17.f);
	static_cast<CColliderSphere*>(pHandBody.get())->SetCallBack<CMonster3D>(COLLISION_STATE::CS_INIT, this, &CMonster3D::CollisionInit);
	static_cast<CColliderSphere*>(pHandBody.get())->SetCallBack<CMonster3D>(COLLISION_STATE::CS_STAY, this, &CMonster3D::CollisionStay);
	static_cast<CColliderSphere*>(pHandBody.get())->SetCallBack<CMonster3D>(COLLISION_STATE::CS_LAST, this, &CMonster3D::CollisionLast);

	static_cast<CColliderSphere*>(pLeftHandBody.get())->SetRadius(17.f);
	static_cast<CColliderSphere*>(pLeftHandBody.get())->SetCallBack<CMonster3D>(COLLISION_STATE::CS_INIT, this, &CMonster3D::CollisionInit);
	static_cast<CColliderSphere*>(pLeftHandBody.get())->SetCallBack<CMonster3D>(COLLISION_STATE::CS_STAY, this, &CMonster3D::CollisionStay);
	static_cast<CColliderSphere*>(pLeftHandBody.get())->SetCallBack<CMonster3D>(COLLISION_STATE::CS_LAST, this, &CMonster3D::CollisionLast);

	m_pAni->SetSocketComponent("RightHand", pHandBody);
	m_pAni->SetSocketOffset("RightHand", Vector3(0.f, 0.f, 0.f));
	m_pAni->SetSocketComponent("LeftHand", pLeftHandBody);
	m_pAni->SetSocketOffset("LeftHand", Vector3(0.f, 0.f, 0.f));
	m_pPaper = FindComponentByType<CPaperBurn>();
	static_cast<CPaperBurn*>(m_pPaper.get())->AddEndCallBack(this, &CCyclops::End);

	std::shared_ptr<CComponent> pCollider = FindComponentByType<CColliderSphere>();

	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CMonster3D>(COLLISION_STATE::CS_INIT, this, &CMonster3D::CollisionInit);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CMonster3D>(COLLISION_STATE::CS_STAY, this, &CMonster3D::CollisionStay);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CMonster3D>(COLLISION_STATE::CS_LAST, this, &CMonster3D::CollisionLast);
}

CCyclops::~CCyclops()
{
}

bool CCyclops::Init()
{
	GET_SINGLE(CResourceManager)->LoadTexture("FX_FireTile_007_TEX_KKJ", L"FX_Resource_KKJ_01\\Texture2D\\FX_FireTile_007_TEX_KKJ.dds");

	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "Monster", TEXT("Cyclops_Anim\\CyclopsClown.msh"));

	GET_SINGLE(CSoundManager)->LoadSound("BGM", "NewBattle_02", true,
		"Dungeon\\NewBattle_02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_atk01", false ,
		"Cyclops\\Cyclops_atk01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_damage", false,
		"Cyclops\\Cyclops_damage.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_death_00", false,
		"Cyclops\\Cyclops_death_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_death_01", false,
		"Cyclops\\Cyclops_death_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_heabyatk01", false,
		"Cyclops\\Cyclops_heabyatk01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_Laugh_01", false,
		"Cyclops\\Cyclops_Laugh_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_longatk01", false,
		"Cyclops\\Cyclops_longatk01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_longatk02_00", false,
		"Cyclops\\Cyclops_longatk02_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_longatk02_01", false,
		"Cyclops\\Cyclops_longatk02_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_longmove_00", false,
		"Cyclops\\Cyclops_longmove_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_longmove_01", false,
		"Cyclops\\Cyclops_longmove_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_modearalm", false,
		"Cyclops\\Cyclops_modearalm.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_ultraatk01_00", false,
		"Cyclops\\Cyclops_ultraatk01_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_ultraatk01_01", false,
		"Cyclops\\Cyclops_ultraatk01_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_ultraatk02", false,
		"Cyclops\\Cyclops_ultraatk02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_wait", false,
		"Cyclops\\Cyclops_wait.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_walk_00", false,
		"Cyclops\\Cyclops_walk_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_walk_01", false,
		"Cyclops\\Cyclops_walk_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "WatchManBlack_SkillVox_01", false,
		"Cyclops\\WatchManBlack_SkillVox_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "roar_05", false,
		"Cyclops\\roar_05.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Sorcerer_Inferno_Explo_1", false,
		"Cyclops\\Sorcerer_Inferno_Explo_1.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Awaken_Sorcerer_Flame_01", false,
		"Cyclops\\Awaken_Sorcerer_Flame_01.ogg");

	std::shared_ptr<CComponent> pMon = CreateComponent<CSceneComponent>("Monster");

	SetRootComponent(pMon);
	
	m_iGold = rand() % 501 + 500;
	m_iHPMax = m_iHP = 2000;
	m_iAttackMax = 200;
	m_iAttackMin = 100;
	m_iArmorMax = 50;
	m_iArmorMin = 25;
	m_fSpeed = 90.f;
	SetAir(true);

	AddLootItem("Æ÷¼Ç");
	AddLootItem("»§");
	AddLootItem("¶óÀÌÆ® ¼Òµå");
	AddLootItem("¿Ü´«¹ÚÀÌÀÇ µÎ°³°ñ");

	CSceneComponent* _pMon = static_cast<CSceneComponent*>(pMon.get());
	_pMon->SetWorldScale(1.f, 1.f, 1.f);
	_pMon->AddWorldPos(1000.f, 0.f, 4000.f);
	_pMon->SetMesh("Monster");
	_pMon->SetShader("PaperBurn2D");
	_pMon->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	std::shared_ptr<CMaterial> pMtrl = _pMon->GetMaterial();
	if (pMtrl)
	{
		pMtrl->SetEmissiveTex(false);
	}

	if (!CMonster3D::Init())
		return false;

	m_pAni = CAnimation::CreateAnimation<CAnimation>("Ani", "MonsterBone", "Cyclops_Anim\\Walk.bne");
	m_pAni->SetSkeleton("MonsterBone");
	m_pAni->AddAnimationSequence("MonsterWait", "Cyclops_Wait");
	m_pAni->AddAnimationSequence("MonsterWalk", "MonsterWalk");
	m_pAni->AddAnimationSequence("MonsterAttack", "MonsterAttack");
	m_pAni->AddAnimationSequence("MonsterDeath", "MonsterDeath");
	m_pAni->AddAnimationSequence("MonsterHeavyAttack", "Cyclops_HeavyAtk01");
	m_pAni->AddAnimationSequence("UltraAtk01", "Cyclops_UltraAtk01");
	m_pAni->AddAnimationSequence("longAtk01_Start", "Cyclops_longAtk01_Start");
	m_pAni->AddAnimationSequence("LongAtk01_Loop", "Cyclops_LongAtk01_Loop");
	m_pAni->AddAnimationSequence("LongAtk01_End_A", "Cyclops_LongAtk01_End_A");
	m_pAni->AddAnimationSequence("LongAtk01_End_B", "Cyclops_LongAtk02_End_B");
	m_pAni->AddAnimationSequence("flinch", "Cyclops_flinch");
	m_pAni->AddAnimationSequence("modeAlarm_Start", "Cyclops_modeAlarm_Start");
	m_pAni->AddAnimationSequence("modeAlarm_Loop", "Cyclops_modeAlarm_Loop");
	m_pAni->AddAnimationSequence("modeAlarm_End", "Cyclops_modeAlarm_End");
	m_pAni->AddAnimationSequence("longMove_Start", "Cyclops_longMove_Start");
	m_pAni->AddAnimationSequence("LongMove_Move", "Cyclops_LongMove_Move");
	m_pAni->AddAnimationSequence("longMove_End", "Cyclops_longMove_End");
	m_pAni->AddAnimationSequence("deathwait", "Cyclops_deathwait");
	m_pAni->AddAnimationSequence("pullAtk_End", "Cyclops_pullAtk_End");
	m_pAni->AddAnimationSequence("pullAtk_Loop", "Cyclops_pullAtk_Loop");
	m_pAni->AddAnimationSequence("pullAtk_Start", "Cyclops_pullAtk_Start");
	
	m_pAni->CreateNotify("longMove_End", "LongMove", 0.95f);
	m_pAni->CreateNotify("MonsterDeath", "Death", 0.95f);
	m_pAni->AutoTransition("pullAtk_Start", "PullAtkStartToLoop", "pullAtk_Loop");
	m_pAni->AutoTransition("pullAtk_Loop", "PullAtkLoopToEnd", "pullAtk_End");
	m_pAni->AutoTransition("pullAtk_End", "PullAtkEndToIdle", "MonsterWait");
	
	m_pAni->CreateNotify("modeAlarm_End", "ModeChange", 38.49f);
	m_pAni->CreateNotify("modeAlarm_End", "ModeChangeEnd", 39.5f);
	m_pAni->CreateNotify("LongAtk01_End_A", "LongAtkEnd", 24.5f);
	m_pAni->CreateNotify("MonsterHeavyAttack", "HeavyStart", 0.f);
	m_pAni->CreateNotify("MonsterHeavyAttack", "HeavyNotify", 23.95f);
	m_pAni->CreateNotify("longAtk01_Start", "LongAtkStart", 28.1f);
	m_pAni->CreateNotify("MonsterHeavyAttack", "HitGround1", 21.76f);
	m_pAni->CreateNotify("MonsterHeavyAttack", "HitGround2", 22.2f);
	m_pAni->CreateNotify("MonsterHeavyAttack", "HitGround3", 22.7f);
	m_pAni->CreateNotify("MonsterAttack", "HitGround", 4.785f);
	m_pAni->CreateNotify("MonsterAttack", "AttackStart", 0.f);
	m_pAni->CreateNotify("modeAlarm_End", "Rage", 0.95f);
	m_pAni->CreateNotify("flinch", "Damaged",0.f);
	m_pAni->CreateNotify("MonsterWait", "IdleSound", 0.f);
	m_pAni->CreateNotify("MonsterWalk", "WalkSound", 0.f);
	m_pAni->CreateNotify("longMove_Start", "LongMoveSound", 0.f);
	m_pAni->CreateNotify("longAtk01_Start", "LoneAttackSound", 0.f);
	m_pAni->CreateNotify("MonsterHeavyAttack", "Move", 21.77f, 23.23f);
	m_pAni->CreateNotify("LongMove_Move", "LongMoveMove", 0.f, 100.f);
	m_pAni->CreateNotify("LongAtk01_End_B", "LongAtkBRock", 30.64f);
	m_pAni->CreateNotify("LongAtk01_End_B", "LongAtk_B_Punch", 31.78f);

	m_pAni->AddSocket("RightHand", "Bip01-R-Hand");
	m_pAni->AddSocket("LeftHand", "Bip01-L-Hand");
	m_pAni->AddSocket("LeftBallHand", "Bip01-L-Hand");

	_pMon->SetAnimation(m_pAni);

	std::shared_ptr<CComponent> pCollider = CreateComponent<CColliderSphere>("MonsterBody", m_pScene->FindLayer(COLLIDER_LAYER));

	static_cast<CSceneComponent*>(pMon.get())->AddChild(pCollider);
	CColliderSphere* _pCollider = static_cast<CColliderSphere*>(pCollider.get());

	_pCollider->SetRadius(50.f);
	_pCollider->AddRelativePosY(50.f);

	m_pPaper = CreateComponent<CPaperBurn>("PaperBurn");

	_pMon->AddObjComponent(m_pPaper);

	static_cast<CPaperBurn*>(m_pPaper.get())->SetScale(0.2f);
	static_cast<CPaperBurn*>(m_pPaper.get())->SetOutFilter(0.81f);
	static_cast<CPaperBurn*>(m_pPaper.get())->SetInFilter(0.79f);
	static_cast<CPaperBurn*>(m_pPaper.get())->SetInColor(0.7f, 0.1f, 0.8f);
	static_cast<CPaperBurn*>(m_pPaper.get())->SetColor(0.3f, 0.1f, 0.4f);

	std::shared_ptr<CGameObject> pBall = m_pScene->CreateProtoObj<CCylopsBall>("CylopsBall", m_pScene, m_pScene->GetSceneType());

	static_cast<CUFont*>(m_pName.get())->SetColor(0.32f, 0.008f, 0.62f, 1.f);

	return true;
}

void CCyclops::Start()
{
	CMonster3D::Start();
}

void CCyclops::Update(float fTime)
{
	CMonster3D::Update(fTime); 

	if (m_bDie)
	{
		if (m_pBall)
			m_pBall->Destroy();
		if (m_pLeftHand)
			m_pLeftHand->Destroy();
		if (m_pRightHand)
			m_pRightHand->Destroy();
	}
	else if (m_bAir || m_bSequenceMode)
	{

	}
	else
	{
		if (m_pPlayer)
		{
			Vector3 vDir = m_pPlayer->GetBoneWorldPos() - GetWorldPos();
			vDir.y = 0.f;
			vDir.Normalize();

			Vector3 vAxisX = Vector3::Axis[AXIS_X];

			float fAngle = RadToDeg(acosf(vAxisX.Dot(vDir)));

			if (vDir.z < 0.f)
				fAngle *= -1.f;

			SetWorldRotY(-fAngle);

			
		}

		if (m_iHP <= m_iHPMax * 0.3f)
		{
			if (typeid(*m_pState) != typeid(CRage))
			{
				m_bModeChange = true;
			}

		}

		if (!m_bModeChange)
		{
			if (!m_bLongAtk)
			{
				if (m_pPlayer)
				{
					Vector3 vTargetPos = m_pPlayer->GetBoneWorldPos();

					Vector3 vPos = GetBoneWorldPos();

					Vector3 vDir = vTargetPos - vPos;

					float fDist = vDir.Length();

					if (fDist < 300.f)
					{
						if (m_fCoolTime > 0.f)
						{
							m_fCoolTime -= fTime;
							if (m_pAni->IsEnd())
							{
								Idle(fTime);
							}
						}
						else if (!m_bModeChange && !m_bLongAtk && !m_bLongMove)
						{
							CAnimationSequence* pSequence = m_pAni->GetSequence();

							if (pSequence)
							{
								std::string strName = pSequence->GetName();

								if (strName == "Cyclops_Wait")
								{
									int iRand = rand() % static_cast<int>(CYCLOPS_NEAR_SKILL_TYPE::END);

									switch (static_cast<CYCLOPS_NEAR_SKILL_TYPE>(iRand))
									{
									case CYCLOPS_NEAR_SKILL_TYPE::ATTACK:
									{
										Attack(fTime);
									}
									break;
									case CYCLOPS_NEAR_SKILL_TYPE::HEAVY_ATTACK:
									{
										HeavyAttack(fTime);
									}
									break;
									}
									m_fCoolTime += m_fCoolTimeLimit;
								}
							}
						}
					}
					else
					{
						if (m_fCoolTime > 0.f)
						{
							m_fCoolTime -= fTime;

							if (!m_pAni->IsEnd())
							{
								CAnimationSequence* pSequence = m_pAni->GetSequence();

								if (pSequence)
								{
									std::string strName = pSequence->GetName();

									if (strName == "MonsterWalk")
									{
										Move(fTime);
									}
									else if (strName == "Cyclops_Wait")
									{
										Idle(fTime);
									}

								}
							}
							else
							{
								if (!m_bLongAtk && !m_bModeChange && !m_bLongMove)
								{
									Move(fTime);
								}
							}
						}

						else if (!m_bLongAtk && !m_bModeChange && !m_bLongMove)
						{
							CAnimationSequence* pSequence = m_pAni->GetSequence();

							if (pSequence)
							{
								std::string strName = pSequence->GetName();

								if (strName == "Cyclops_Wait")
								{
									int iRand = rand() % static_cast<int>(CYCLOPS_FAR_SKILL_TYPE::END);

									switch (static_cast<CYCLOPS_FAR_SKILL_TYPE>(iRand))
									{
									case CYCLOPS_FAR_SKILL_TYPE::LONG_ATTACK:
									{
										LongAttack(fTime);
									}
									break;
									case CYCLOPS_FAR_SKILL_TYPE::LONG_MOVE:
									{
										ChangeSequenceAll("longMove_Start");
									}
									break;
									}

									m_fCoolTime += m_fCoolTimeLimit;
								}
							}
						}
					}
				}
				else
				{
					if (m_fCoolTime <= 0.f)
					{
						m_fLongAtkTime -= fTime;

						if (m_fLongAtkTime <= 0.f)
						{
							m_fCoolTime += m_fCoolTimeLimit;
							m_fLongAtkTime += m_fLongAtkTimeLimit;
						}

						Idle(fTime);
					}
					else
					{
						m_fCoolTime -= fTime;
						Move(fTime);
					}
				}
			}
			else
			{
				m_fLongAtkTime += fTime;

				if (m_fLongAtkTime >= m_fLongAtkTimeLimit)
				{
					m_fLongAtkTime -= m_fLongAtkTimeLimit;

					m_bLongAtk = false;
				}
			}
			if (m_bLongMove && !m_bLongAtk && !m_bModeChange)
			{
				LongMove(fTime);
			}
		}

		else 
		{
			m_fModeChangeTime += fTime;

			if (m_fModeChangeTime >= m_fModeChangeLimit)
			{
				ChangeSequenceAll("modeAlarm_End");
			}
		}
	}

}

void CCyclops::PostUpdate(float fTime)
{
	CMonster3D::PostUpdate(fTime);
}

void CCyclops::Collision(float fTime)
{
	CMonster3D::Collision(fTime);
}

void CCyclops::PreRender(float fTime)
{
	CMonster3D::PreRender(fTime);
}

void CCyclops::Render(float fTime)
{
	CMonster3D::Render(fTime);
}

void CCyclops::PostRender(float fTime)
{
	CMonster3D::PostRender(fTime);
}

std::shared_ptr<CGameObject> CCyclops::Clone()
{
	return std::shared_ptr<CGameObject>(new CCyclops(*this));
}

void CCyclops::Save(FILE * pFile)
{
	CMonster3D::Save(pFile);
}

void CCyclops::Load(FILE * pFile)
{
	CMonster3D::Load(pFile);

	std::shared_ptr<CComponent> pCollider = FindComponent("MonsterBody");
	CColliderSphere* _pCollider = static_cast<CColliderSphere*>(pCollider.get());
	_pCollider->SetCallBack<CMonster3D>(COLLISION_STATE::CS_INIT, this, &CMonster3D::CollisionInit);
	_pCollider->SetCallBack<CMonster3D>(COLLISION_STATE::CS_STAY, this, &CMonster3D::CollisionStay);
	_pCollider->SetCallBack<CMonster3D>(COLLISION_STATE::CS_LAST, this, &CMonster3D::CollisionLast);

	std::shared_ptr<CComponent> pRenderer = FindComponent("Monster");

	m_pAni = static_cast<CSceneComponent*>(pRenderer.get())->GetAnimation();
	m_pAni->AddTransition("WalkToDeath", "MonsterWalk", this, &CCyclops::Transition, "MonsterDeath");
	m_pAni->AddTransition("WalkToAttack", "MonsterWalk", this, &CCyclops::Transition, "MonsterAttack");
	m_pAni->AddTransition("AttackToWalk", "MonsterAttack", this, &CCyclops::Transition, "MonsterWalk");
	m_pAni->CreateNotify("MonsterDeath", "Death", 4.55f);
	m_pAni->AddNotifyFunction("Death", this, &CCyclops::Death);

	m_pPaper = FindComponent("PaperBurn");
	static_cast<CPaperBurn*>(m_pPaper.get())->AddEndCallBack(this, &CCyclops::End);
}


void CCyclops::Death(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Cyclops_death_00");

	CQuest* pQuest = CQuest::FindQuest("¿Ü´«¹ÚÀÌ");

	if (pQuest)
	{
		if (pQuest->GetState() == QUEST_STATE::ACCEPTED)
		{
			pQuest->SetState(QUEST_STATE::QUALIFIED);
		}
	}

	if (m_pBall)
	{
		m_pBall->Destroy();
	}

	std::shared_ptr<CGameObject> pPortal = m_pScene->CreateObject<CPortal>("Portal", m_pScene->FindLayer(ALBEDO_LAYER));

	pPortal->SetWorldPos(GetWorldPos() + Vector3(300.f, 0.f, 0.f));
	//pPortal->SetWorldRotY(-45.f);
}

void CCyclops::End(float fTime)
{
	Destroy();
}

bool CCyclops::Transition()
{
	return false;
}

bool CCyclops::LongAtkTransition()
{
	return !m_bLongAtk;
}

bool CCyclops::ToDeath()
{
	return m_iHP <= 0;
}

bool CCyclops::IdleToLongMove()
{
	return m_bAir;
}

bool CCyclops::LongMoveToIdle()
{
	return !m_bAir;
}

bool CCyclops::LongAttackA()
{
	if (!LongAtkTransition())
		return false;

	float fDist = (GetWorldPos() - m_pPlayer->GetWorldPos()).Length();

	return fDist > 300.f;
}

bool CCyclops::LongAttackB()
{
	if (!LongAtkTransition())
		return false;

	float fDist = (GetWorldPos() - m_pPlayer->GetWorldPos()).Length();

	return fDist <= 300.f;
}

void CCyclops::HeavyToAttack()
{
}

void CCyclops::WalkToLongAtk()
{
	if (!m_bLongAtk)
	{
		m_bLongAtk = true;

		if (m_pBall)
		{
			m_pBall->Destroy();
			m_pBall = nullptr;
		}

		m_pBall = m_pScene->CreateCloneObj("Ball","CylopsBall", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());

		m_pBall->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_X) * 30.f);
	}
}

void CCyclops::LongAtkToWalk()
{
}

void CCyclops::ToIdle()
{
	ChangeSequenceAll("MonsterWait");
}

void CCyclops::ModeChangeStart()
{
	GET_SINGLE(CSoundManager)->Play("roar_05");
	//GET_SINGLE(CSoundManager)->Play("Cyclops_modearalm");
	m_bModeChange = true;

	if (m_pBall)
	{
		m_pBall->Destroy();
		m_pBall = nullptr;
	}
}

void CCyclops::LongMoveStart()
{
	if (m_bSequenceMode)
		return;

	m_bLongMove = true;
	SetSpeedY(200.f);
	SetAir(true);
	AddWorldPosY(10.f);
}

void CCyclops::LongMoveEnd(float fTime)
{
	if (m_bSequenceMode)
		return;
	m_bLongMove = false;
	//AddDist(294.63855f);
	//AddWorldPos(GetWorldAxis(AXIS_X) * 294.63855f);

	std::shared_ptr<CGameObject> pParticle = m_pScene->CreateCloneObj("DarkDustParticle", "DarkDustParticle", 
		 m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

	pParticle->SetWorldPos(GetWorldPos() + Vector3(0.f, 2.5f, 0.f));
}

void CCyclops::ModeChangeBlurEnd()
{
	ModeChangeBlurEnd(0.f);
}

void CCyclops::ModeChangeEnd()
{
	m_bModeChange = false;
}

void CCyclops::LongAtkBStart()
{
	static_cast<CCylopsBall*>(m_pBall.get())->Punched();
}

void CCyclops::LongMoveNotify(float fTime)
{
	AddDist(294.63855f);
	AddWorldPos(GetWorldAxis(AXIS_X) * 294.63855f);
}

void CCyclops::ModeChangeBlur(float fTime)
{
	Vector3 vPos = GetWorldPos();

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Matrix matVP = pCam->GetVP();

	Vector3 vProjPos = vPos.TranslationCoor(matVP);

	vProjPos.y *= -1.f;

	vProjPos += 1.f;
	vProjPos /= 2.f;
	GET_SINGLE(CRenderManager)->SetSampleDist(5.f);
	GET_SINGLE(CRenderManager)->SetSampleStrength(5.f);
	GET_SINGLE(CRenderManager)->SetBlurCenter(Vector2(vProjPos.x, vProjPos.y));
	GET_SINGLE(CRenderManager)->SetFullBlur(true);
}

void CCyclops::ModeChangeBlurEnd(float fTime)
{
	GET_SINGLE(CRenderManager)->SetFullBlur(false);
}

void CCyclops::DamagedSound(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Cyclops_damage");
}

void CCyclops::AttackSound(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Cyclops_atk01");
}

void CCyclops::IdleSound(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Cyclops_wait");
}

void CCyclops::WalkSound(float fTime)
{
	if (rand() % 2 == 0)
	{
		GET_SINGLE(CSoundManager)->Play("Cyclops_walk_00");
	}
	else
	{
		GET_SINGLE(CSoundManager)->Play("Cyclops_walk_01");
	}
}

void CCyclops::LongMoveSound(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Cyclops_longmove_00");
}

void CCyclops::LoneAttackSound(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Cyclops_longatk01");
}

void CCyclops::LongAtkBRockHold(float fTime)
{
	if (!m_pBall)
		return;

	if (m_pPlayer)
	{
		Vector3 vPos = m_pBall->GetBoneWorldPos();

		m_pBall->SetWorldPos(vPos);
	}

	m_pAni->DeleteSocketObject("LeftBallHand", m_pBall);

	std::shared_ptr<CGameObject> pCircle = m_pScene->CreateObject<CGameObject>("Circle", m_pScene->FindLayer(DECAL_LAYER));

	std::shared_ptr<CComponent> pCircleCom = pCircle->CreateComponent<CDecal>("Circle");

	pCircle->SetRootComponent(pCircleCom);

	static_cast<CDecal*>(pCircleCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_T_EFX_Ring_Thin");
	static_cast<CDecal*>(pCircleCom.get())->SetWorldScale(500.f, 500.f, 500.f);
	static_cast<CDecal*>(pCircleCom.get())->SetWorldPos(m_pBall->GetWorldPos());
	static_cast<CDecal*>(pCircleCom.get())->SetAlpha(1.f);
	static_cast<CDecal*>(pCircleCom.get())->SetRadiusMode(true);
	static_cast<CDecal*>(pCircleCom.get())->SetRadius(250.f);
	static_cast<CDecal*>(pCircleCom.get())->SetFadeIn(1.2f);
	static_cast<CDecal*>(pCircleCom.get())->SetDuration(0.0001f);
	static_cast<CDecal*>(pCircleCom.get())->SetFadeOut(0.0001f);
}

void CCyclops::LongAtk_B_Punch(float fTime)
{
	if (!m_pBall)
		return;

	GET_SINGLE(CSoundManager)->Play("Awaken_Sorcerer_Flame_01");

	Vector3 vPos = m_pRightHand->GetBoneWorldPos();

	std::shared_ptr<CGameObject> pParticle = m_pScene->CreateCloneObj("RockParticle", 
		"RockParticle", m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

	pParticle->SetWorldPos(vPos);

	std::shared_ptr<CGameObject> pDecalObj = m_pScene->CreateObject<CGameObject>("Decal", m_pScene->FindLayer(DECAL_LAYER));

	std::shared_ptr<CComponent> pDecal = pDecalObj->CreateComponent<CDecal>("Decal");

	pDecalObj->SetRootComponent(pDecal);

	static_cast<CDecal*>(pDecal.get())->SetMesh("Cube2");
	static_cast<CDecal*>(pDecal.get())->SetShader("DecalSphere");
	static_cast<CDecal*>(pDecal.get())->SetMaterial("Color");

	static_cast<CDecal*>(pDecal.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_FireTile_007_TEX_KKJ");
	static_cast<CDecal*>(pDecal.get())->SetWorldScale(500.f, 500.f, 500.f);

	vPos.y = 0.f;
	static_cast<CDecal*>(pDecal.get())->SetWorldPos(vPos);
	static_cast<CDecal*>(pDecal.get())->SetAlpha(1.f);
	static_cast<CDecal*>(pDecal.get())->SetFadeIn(0.5f);
	static_cast<CDecal*>(pDecal.get())->SetDuration(1.f);
	static_cast<CDecal*>(pDecal.get())->SetFadeOut(0.5f);

	Vector3 vTargetPos = m_pPlayer->GetWorldPos();

	vTargetPos.y = 0.f;

	Vector3 vDir = vTargetPos - vPos;

	float fDist = vDir.Length();

	if (fDist < 250.f)
	{
		vDir /= fDist;

		m_pPlayer->SetSpeed(vDir * 200.f);
		m_pPlayer->AddSpeedY(300.f);

		static_cast<CPlayer3D*>(m_pPlayer.get())->CalcDamage(
			static_cast<int>(m_iAttackMax * 1.5f),
			static_cast<int>(m_iAttackMin * 1.5f));

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

		pObj->SetWorldPos(vTargetPos);
	}


	m_pBall = nullptr;
}

void CCyclops::HeavyAttackEnd(float fTime)
{
	Vector3 vDir = GetWorldAxis(AXIS_X);

	AddWorldPos(vDir * 150.f);
}

void CCyclops::HeavyAttackSound(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Cyclops_heabyatk01");
}

void CCyclops::Move(float fTime)
{
	CMonster3D::Move(fTime);
}

void CCyclops::Attack(float fTime)
{
	CMonster3D::Attack(fTime);
}

void CCyclops::AttackAround(float fTime)
{
	CMonster3D::AttackAround(fTime);
}

void CCyclops::HeavyAttack(float fTime)
{
	CMonster3D::HeavyAttack(fTime);
}

void CCyclops::LongAttack(float fTime)
{
	CMonster3D::LongAttack(fTime);
}

void CCyclops::Idle(float fTime)
{
	CMonster3D::Idle(fTime);
}

void CCyclops::LongMove(float fTime)
{
	CMonster3D::LongMove(fTime);
}

void CCyclops::LongAtk(float fTime)
{
	if (!m_pBall)
		return;

	if (m_pPlayer)
	{
		Vector3 vPos = m_pBall->GetBoneWorldPos();

		m_pBall->SetWorldPos(vPos);

		Vector3 vTargetPos = m_pPlayer->GetBoneWorldPos() + Vector3(0.f, 10.f, 0.f);

		Vector3 vDir = vTargetPos - vPos;

		vDir.Normalize();

		Vector3 vUp = Vector3::Axis[AXIS_Z];

		Vector3 vAxis = vUp.Cross(vDir);

		float fAngle = RadToDeg(acosf(vUp.Dot(vDir)));

		std::shared_ptr<CComponent> pRoot = m_pBall->GetRootComponent();

		std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pRoot.get())->GetTransform();

		pTransform->SetQuaternionRotAxis(vAxis, fAngle);

		static_cast<CCylopsBall*>(m_pBall.get())->Throw();
	}
	m_pAni->DeleteSocketObject("LeftBallHand", m_pBall);
	m_pBall = nullptr;
}

void CCyclops::LongAtkStart(float fTime)
{
	if (m_pBall)
	{
		m_pBall->SetWorldPos(0.f, 0.f, 0.f);
		m_pAni->SetSocketObject("LeftBallHand", m_pBall);
		m_pAni->SetSocketOffset("LeftBallHand", Vector3(10.f, -15.f, 30.f));
	}
}

void CCyclops::HitGroundLeft(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Sorcerer_Inferno_Explo_1");
	Vector3 vPos = m_pLeftHand->GetBoneWorldPos();

	vPos.y = 0.f;

	std::shared_ptr<CGameObject> pParticle = m_pScene->CreateCloneObj("RockParticle", "RockParticle",
		m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

	static_cast<CParticle3D*>(pParticle.get())->SetWorldPos(vPos);

	std::shared_ptr<CGameObject> pDecal = m_pScene->CreateCloneObj("Crack", "Crack",
		m_pScene->FindLayer(DECAL_LAYER), m_pScene->GetSceneType());

	pDecal->SetWorldPos(vPos);

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	if (pCam)
	{
		pCam->SetShakePower(6.f);
		pCam->SetShake(true);
		pCam->SetShakeMass(50.f);
		pCam->SetFrequency(32 * PI);
	}
}

void CCyclops::HitGroundRight(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Sorcerer_Inferno_Explo_1");
	Vector3 vPos = m_pRightHand->GetBoneWorldPos();

	vPos.y = 0.f;

	std::shared_ptr<CGameObject> pParticle = m_pScene->CreateCloneObj("RockParticle", "RockParticle",
		m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

	static_cast<CParticle3D*>(pParticle.get())->SetWorldPos(vPos);

	std::shared_ptr<CGameObject> pDecal = m_pScene->CreateCloneObj("Crack", "Crack",
		m_pScene->FindLayer(DECAL_LAYER), m_pScene->GetSceneType());

	pDecal->SetWorldPos(vPos);

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	if (pCam)
	{
		pCam->SetShakePower(6.f);
		pCam->SetShake(true);
		pCam->SetShakeMass(50.f);
		pCam->SetFrequency(32 * PI);
	}
}

void CCyclops::Rage(float fTime)
{
	std::shared_ptr<CComponent> pCom = GetRootComponent();

	if (pCom)
	{
		std::shared_ptr<CMaterial> pMtrl = static_cast<CSceneComponent*>(pCom.get())->GetMaterial();
		if (pMtrl)
		{
			pMtrl->SetEmissiveTex(true);
		}
	}
}

void CCyclops::ChangeSequenceAll(const char* pSequence)
{
	m_pAni->ChangeSequence(pSequence);
	//m_pBallAni->ChangeSequence(pSequence);
}

void CCyclops::CollideInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CCyclops::CollideStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CCyclops::CollideEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CCyclops::ShowWindow()
{
	CGameObject::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		ImGui::Checkbox("Long Attack", &m_bLongAtk);
		ImGui::SliderFloat("Long Attack Time", &m_fLongAtkTime, 0.f, m_fLongAtkTimeLimit);
		ImGui::InputFloat("Long Attack Limit", &m_fLongAtkTimeLimit);
		ImGui::SliderFloat("Cool Time", &m_fCoolTime, 0.f, m_fCoolTimeLimit);
		ImGui::InputFloat("Cool Time Limit", &m_fCoolTimeLimit);		

		if (typeid(*m_pState) == typeid(CIdle))
		{
			ImGui::Text("Idle State");
		}

		else if (typeid(*m_pState) == typeid(CFight))
		{
			ImGui::Text("Fight State");
		}

		else if (typeid(*m_pState) == typeid(CRage))
		{
			ImGui::Text("Rage State");
		}

		else if (typeid(*m_pState) == typeid(CWalk))
		{
			ImGui::Text("Walk State");
		}

		else if (typeid(*m_pState) == typeid(CDie))
		{
			ImGui::Text("Die State");
		}

		ImGui::Checkbox("Mode Change", &m_bModeChange);
		ImGui::Checkbox("Long Move", &m_bLongMove);
		ImGui::SliderFloat("Mode Change Time", &m_fModeChangeTime, 0.f, m_fModeChangeLimit);
	}
	ImGui::End();
}

void CCyclops::Attacked()
{
	Damaged(true);
	/*if (!m_bLongAtk && !m_bDie && !m_bModeChange)
	{
	}*/
}

void CCyclops::MotionStart()
{
	m_bLongAtk = true;
	m_pAni->ChangeSequence("pullAtk_Start");
	m_fLongAtkTime = 0.f;
	m_fCoolTime = 0.f;

	GET_SINGLE(CSoundManager)->Play("WatchManBlack_SkillVox_01");

}

void CCyclops::MotionEnd()
{
	m_bLongAtk = false;

	CLayer* pLayer = m_pScene->FindLayer(ALBEDO_LAYER);

	std::shared_ptr<CGameObject> pPlayer = pLayer->FindObj("Player");

	SetPlayer(pPlayer);

	std::shared_ptr<CGameObject> pTargetUI = m_pScene->FindLayer(UI_LAYER)->FindObj("Target UI");

	if (pTargetUI)
	{
		static_cast<CTargetUI*>(pTargetUI.get())->SetTarget(this);
	}
	GET_SINGLE(CSoundManager)->Play("NewBattle_02");
}

void CCyclops::HeavyAttackMove(float fAniTime, float fTime)
{
	if (m_pPlayer)
	{
		if ((m_pPlayer->GetWorldPos() - GetWorldPos()).Length() > 50.f)
		{
			AddWorldPos(GetWorldAxis(AXIS_X) * fTime * 100.f);
		}
	
	}
	else
	{
		AddWorldPos(GetWorldAxis(AXIS_X) * fTime * 100.f);
	}
}

void CCyclops::LongMove(float fAniTime, float fTime)
{
	if (m_bSequenceMode)
	{
		return;
	}

	AddWorldPos(GetWorldAxis(AXIS_X) * fTime * 350.f);
}
