#include "BladePlayer.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"
#include "Scene/Scene.h"
#include "Resource/ResourceManager.h"
#include "Resource/Material.h"
#include "Component/ColliderSphere.h"
#include "Resource/Mesh.h"
#include "Component/Transform.h"
#include "Device.h"
#include "Component/ColliderOBB.h"
#include "Particle3D.h"
#include "Component/Light.h"
#include "Component/Particle.h"
#include "Render/Distortion.h"
#include "Monster3D.h"
#include "Component/PaperBurn.h"
#include "Sound/SoundManager.h"
#include "../GameMode/Town.h"

CBladePlayer::CBladePlayer()	:
	m_fTrailTime(0.f)
	, m_fTrailTimeLimit(0.f)
	, m_iTrailCount(12)
	, m_fTrailStartAlpha(0.45f)
	, m_fTrailEndAlpha(0.f)
	, m_iCombo(0)
	, m_fComboTime(0.f)
	, m_fComboTimeMax(4.f)
	, m_fCharge(0.f)
{
}

CBladePlayer::CBladePlayer(const CBladePlayer& obj)	:
	CPlayer3D(obj)
{
	*this = obj;
}

CBladePlayer::~CBladePlayer()
{
	SAFE_RELEASE(m_pUpperAni);
}

bool CBladePlayer::Init()
{
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH,
		"PPlayerHead",
		L"PPlayer\\Head.msh");
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH,
		"PPlayerTail",
		L"PPlayer\\PPlayerTail.msh");
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH,
		"poporiblade",
		L"PPlayer\\poporiblade.msh");

	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH,
		"Lance",
		L"PPlayer\\PPlayerLance.msh");
	GET_SINGLE(CResourceManager)->LoadSkeleton("poporiBone", "PPlayer\\poporiblade.bne");

	m_pMesh = CreateComponent<CSceneComponent>("Renderer", m_pScene->FindLayer(ALPHA_LAYER));

	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH,
		"PPlayerHead",
		L"PPlayer\\Face.msh");
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH,
		"PPlayerHair",
		L"PPlayer\\Hair.msh");

	GET_SINGLE(CResourceManager)->LoadTexture("Ring_01_TEX", TEXT("FX_Resource_KKJ_05\\Texture2D\\Ring_01_TEX.dds"), TEXTURE_PATH);
	GET_SINGLE(CResourceManager)->LoadTexture("FX_SparkYellow_001_TEX_HKB", TEXT("FX_Resource_KKJ_05\\Texture2D\\FX_SparkYellow_001_TEX_HKB.dds"), TEXTURE_PATH);

	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Damage01_00", false,
		"Skill\\Damage01_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Damage01_01", false,
		"Skill\\Damage01_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Damage01_02", false,
		"Skill\\Damage01_02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Lancer_ComboAttack_00", false,
		"Skill\\Lancer_ComboAttack_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Lancer_ComboAttack_01", false,
		"Skill\\Lancer_ComboAttack_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Lancer_ComboAttack_02", false,
		"Skill\\Lancer_ComboAttack_02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Lancer_MenaceWave_Shot", false,
		"Skill\\Lancer_MenaceWave_Shot.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Lancer_MenaceWave_Explosion", false,
		"Skill\\Lancer_MenaceWave_Explosion.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Lancer_MenaceWave_Casting", false,
		"Skill\\Lancer_MenaceWave_Casting.ogg");

	SetRootComponent(m_pMesh);

	static_cast<CSceneComponent*>(m_pMesh.get())->SetMesh("poporiblade");
	static_cast<CSceneComponent*>(m_pMesh.get())->SetShader("Alpha");
	static_cast<CSceneComponent*>(m_pMesh.get())->SetShader("Alpha", 1);
	static_cast<CSceneComponent*>(m_pMesh.get())->SetShader("Alpha", 2);
	static_cast<CSceneComponent*>(m_pMesh.get())->SetShader("Alpha", 3);
	static_cast<CSceneComponent*>(m_pMesh.get())->AddRenderState("OutLineObject");
	static_cast<CSceneComponent*>(m_pMesh.get())->AddRenderState("OutLineObject", 1);
	static_cast<CSceneComponent*>(m_pMesh.get())->AddRenderState("OutLineObject", 2);
	static_cast<CSceneComponent*>(m_pMesh.get())->AddRenderState("OutLineObject", 3);
	static_cast<CSceneComponent*>(m_pMesh.get())->AddRenderState("NoCullRS",3);

	if (!CPlayer3D::Init())
		return false;

	m_pUpperAni = CAnimation::CreateAnimation<CAnimation>("poporiAni", "poporiBone", "PPlayer\\poporiblade.bne");

	m_pUpperAni->SetSkeleton("poporiBone");
	m_pUpperAni->AddAnimationSequence("Idle", "PPlayer_Wait");
	m_pUpperAni->AddAnimationSequence("poporiAni", "poporiblade");
	m_pUpperAni->AddAnimationSequence("Jump_Land_WP", "PPlayer_JumpEndLand");
	m_pUpperAni->AddAnimationSequence("jump_loop_WP", "PPlayer_JumpLoop");
	m_pUpperAni->AddAnimationSequence("Jump_Up_WP", "PPlayer_JumpStart");
	m_pUpperAni->AddAnimationSequence("SK_14", "poporiblade");
	m_pUpperAni->AddAnimationSequence("Walk", "PPlayer_run");
	m_pUpperAni->AddAnimationSequence("Die", "PPlayer_Death");
	m_pUpperAni->AddAnimationSequence("WalkBack", "PPlayer_run");
	m_pUpperAni->AddAnimationSequence("DMG_F_WP", "PPlayer_bReactionStart2");
	m_pUpperAni->AddAnimationSequence("run_jump_up_F_WP", "PPlayer_JumpStart");
	m_pUpperAni->AddAnimationSequence("run_jump_loop_f_WP", "PPlayer_JumpLoop");
	m_pUpperAni->AddAnimationSequence("run_jump_land_f_WP", "PPlayer_JumpEndLand");
	m_pUpperAni->AddAnimationSequence("Run_F_WP", "PPlayer_run");
	m_pUpperAni->AddAnimationSequence("Run_FR_WP", "PPlayer_run");
	m_pUpperAni->AddAnimationSequence("Run_FL_WP", "PPlayer_run");
	m_pUpperAni->AddAnimationSequence("Run_B_WP", "PPlayer_run");
	m_pUpperAni->AddAnimationSequence("Run_BR_WP", "PPlayer_run");
	m_pUpperAni->AddAnimationSequence("Run_BL_WP", "PPlayer_run");
	m_pUpperAni->AddAnimationSequence("Run_L_WP", "PPlayer_run");
	m_pUpperAni->AddAnimationSequence("Run_R_WP", "PPlayer_run");
	m_pUpperAni->AddAnimationSequence("Down", "PPlayer_Death");
	m_pUpperAni->AddAnimationSequence("Down_Loop", "PPlayer_deathwait");
	m_pUpperAni->AddAnimationSequence("down_deadbody", "PPlayer_deathwait");
	m_pUpperAni->AddAnimationSequence("combo1", "PPlayer_combo1");
	m_pUpperAni->AddAnimationSequence("combo1R", "PPlayer_combo1R");
	m_pUpperAni->AddAnimationSequence("combo2", "PPlayer_combo2");
	m_pUpperAni->AddAnimationSequence("combo2R", "PPlayer_combo2R");
	m_pUpperAni->AddAnimationSequence("combo3", "PPlayer_combo3");
	m_pUpperAni->AddAnimationSequence("combo3R", "PPlayer_combo3R");
	m_pUpperAni->AddAnimationSequence("combo4", "PPlayer_combo4");
	m_pUpperAni->AddAnimationSequence("swordDanceStart", "PPlayer_swordDanceStart");
	m_pUpperAni->AddAnimationSequence("swordDanceLoop", "PPlayer_swordDanceLoop");
	m_pUpperAni->AddAnimationSequence("swordDanceShot", "PPlayer_swordDanceShot");

	//m_pUpperAni->CreateNotify("poporiAni", "Trail1", 0.f, 1.f);
	//m_pUpperAni->CreateNotify("poporiAni", "Trail8", 1.f);
	//m_pUpperAni->AddTimeLineNotifyFunction("Trail1", this, &CBladePlayer::Trail);
	//m_pUpperAni->AddNotifyFunction("Trail8", this, &CBladePlayer::TrailEnd);

	m_pUpperAni->CreateNotify("SK_14", "TrailStart", 0.f);
	m_pUpperAni->CreateNotify("SK_14", "Trail2", 0.f, 0.6f);
	m_pUpperAni->CreateNotify("SK_14", "TrailEnd", 0.7f);
	m_pUpperAni->AddNotifyFunction("TrailStart", this, &CBladePlayer::TrailStart);
	m_pUpperAni->AddNotifyFunction("TrailEnd", this, &CBladePlayer::TrailEnd);
	m_pUpperAni->AddTimeLineNotifyFunction("Trail2", this, &CBladePlayer::Trail);

	m_pUpperAni->CreateNotify("combo1", "combo1_TrailStart", 12.8f);
	m_pUpperAni->CreateNotify("combo1", "combo1_Trail2", 12.8f, 13.2f);
	m_pUpperAni->CreateNotify("combo1", "combo1_TrailEnd", 13.2f);
	m_pUpperAni->AddNotifyFunction("combo1_TrailStart", this, &CBladePlayer::TrailStart);
	m_pUpperAni->AddNotifyFunction("combo1_TrailEnd", this, &CBladePlayer::TrailEnd);
	m_pUpperAni->AddNotifyFunction("combo1_TrailEnd", this, &CBladePlayer::ComboAdd);
	m_pUpperAni->AddTimeLineNotifyFunction("combo1_Trail2", this, &CBladePlayer::Trail);

	m_pUpperAni->CreateNotify("combo2", "combo2_TrailStart", 14.6f);
	m_pUpperAni->CreateNotify("combo2", "combo2_Trail2", 14.6f, 15.1f);
	m_pUpperAni->CreateNotify("combo2", "combo2_TrailEnd", 15.1f);
	m_pUpperAni->AddNotifyFunction("combo2_TrailStart", this, &CBladePlayer::TrailStart);
	m_pUpperAni->AddNotifyFunction("combo2_TrailEnd", this, &CBladePlayer::TrailEnd);
	m_pUpperAni->AddNotifyFunction("combo2_TrailEnd", this, &CBladePlayer::ComboAdd);
	m_pUpperAni->AddTimeLineNotifyFunction("combo2_Trail2", this, &CBladePlayer::Trail);

	m_pUpperAni->CreateNotify("combo3", "combo3_TrailStart", 17.f);
	m_pUpperAni->CreateNotify("combo3", "combo3_Trail2", 17.f, 17.35f);
	m_pUpperAni->CreateNotify("combo3", "combo3_TrailEnd", 17.35f);
	m_pUpperAni->AddNotifyFunction("combo3_TrailStart", this, &CBladePlayer::TrailStart);
	m_pUpperAni->AddNotifyFunction("combo3_TrailEnd", this, &CBladePlayer::TrailEnd);
	m_pUpperAni->AddNotifyFunction("combo3_TrailEnd", this, &CBladePlayer::ComboAdd);
	m_pUpperAni->AddTimeLineNotifyFunction("combo3_Trail2", this, &CBladePlayer::Trail);

	m_pUpperAni->CreateNotify("combo4", "combo4_TrailStart", 18.9f);
	m_pUpperAni->CreateNotify("combo4", "combo4_Trail2", 18.9f, 20.f);
	m_pUpperAni->CreateNotify("combo4", "combo4_TrailEnd", 20.f);
	m_pUpperAni->AddNotifyFunction("combo4_TrailStart", this, &CBladePlayer::TrailStart);
	m_pUpperAni->AddNotifyFunction("combo4_TrailEnd", this, &CBladePlayer::TrailEnd);
	m_pUpperAni->AddNotifyFunction("combo4_TrailEnd", this, &CBladePlayer::ComboEnd);
	m_pUpperAni->AddTimeLineNotifyFunction("combo4_Trail2", this, &CBladePlayer::Trail);

	m_pUpperAni->CreateNotify("swordDanceShot", "swordDanceShot_TrailStart", 50.f);
	m_pUpperAni->CreateNotify("swordDanceShot", "swordDanceShot_Trail2", 50.f, 52.f);
	m_pUpperAni->CreateNotify("swordDanceShot", "swordDanceShot_TrailEnd", 52.f);
	m_pUpperAni->AddNotifyFunction("swordDanceShot_TrailStart", this, &CBladePlayer::TrailStart);
	m_pUpperAni->AddNotifyFunction("swordDanceShot_Trail2", this, &CBladePlayer::TrailEnd);
	m_pUpperAni->AddTimeLineNotifyFunction("swordDanceShot_Trail2", this, &CBladePlayer::Trail);

	SetTransition();

	static_cast<CSceneComponent*>(m_pMesh.get())->SetAnimation(m_pUpperAni);

	m_pUpperOutLine = CreateComponent<CSceneComponent>("OutLine", m_pScene->FindLayer(ALPHA_LAYER));

	static_cast<CSceneComponent*>(m_pMesh.get())->AddChild(m_pUpperOutLine);

	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetMesh("poporiblade");
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetAnimation(m_pUpperAni);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetWorldScale(1.05f, 1.05f, 1.05f);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine");
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine",1);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 2);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 3);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("NoCullRS", 3);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineAlphaShader");
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineAlphaShader",1);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineAlphaShader",2);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineAlphaShader",3);

	m_pWeapon = m_pScene->CreateObject<CGameObject>("Staff", m_pScene->FindLayer(ALBEDO_LAYER));

	m_pLance = m_pWeapon->CreateComponent<CSceneComponent>("Blade", m_pScene->FindLayer(ALPHA_LAYER));

	m_pWeapon->SetRootComponent(m_pLance);

	m_pWeapon->SetShadowAll(true);
	static_cast<CSceneComponent*>(m_pLance.get())->SetMesh("Lance");
	static_cast<CSceneComponent*>(m_pLance.get())->SetShader("Alpha");
	static_cast<CSceneComponent*>(m_pLance.get())->AddRenderState("OutLineObject");

	m_pStaffOutLine = m_pWeapon->CreateComponent<CSceneComponent>("WeaponOutLine", m_pScene->FindLayer(ALPHA_LAYER));

	static_cast<CSceneComponent*>(m_pLance.get())->AddChild(m_pStaffOutLine);

	static_cast<CSceneComponent*>(m_pStaffOutLine.get())->SetMesh("Lance");
	static_cast<CSceneComponent*>(m_pStaffOutLine.get())->SetWorldScale(1.05f, 1.05f, 1.05f);
	static_cast<CSceneComponent*>(m_pStaffOutLine.get())->AddRenderState("OutLine");
	static_cast<CSceneComponent*>(m_pStaffOutLine.get())->SetShader("OutLineAlphaShader");

	m_pParticle = m_pWeapon->CreateComponent<CParticle>("Sword Particle", m_pScene->FindLayer(PARTICLE_LAYER));

	static_cast<CSceneComponent*>(m_pLance.get())->AddChild(m_pParticle);

	static_cast<CParticle*>(m_pParticle.get())->AddRelativePosY(50.f);
	static_cast<CParticle*>(m_pParticle.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CParticle*>(m_pParticle.get())->SetMinTime(5.f);
	static_cast<CParticle*>(m_pParticle.get())->SetMaxTime(5.f);
	static_cast<CParticle*>(m_pParticle.get())->SetMaxSpeed(5.f);
	static_cast<CParticle*>(m_pParticle.get())->SetMinSpeed(5.f);
	static_cast<CParticle*>(m_pParticle.get())->SetMaxSpawnTime(0.f);
	static_cast<CParticle*>(m_pParticle.get())->SetMinScale(Vector3(1.f, 1.f, 1.f));
	static_cast<CParticle*>(m_pParticle.get())->SetMaxScale(Vector3(1.f, 1.f, 1.f));
	static_cast<CParticle*>(m_pParticle.get())->SetTexture(LINK_TYPE::LT_DIF, "Ring_01_TEX");
	static_cast<CParticle*>(m_pParticle.get())->SetMove(true);
	static_cast<CParticle*>(m_pParticle.get())->SetMaxColor(Vector4(0.2f, 0.2f, 0.7f, 1.f));
	static_cast<CParticle*>(m_pParticle.get())->SetMinColor(Vector4(0.2f, 0.2f, 0.7f, 1.f));
	static_cast<CParticle*>(m_pParticle.get())->SetMaxFrame(1);
	static_cast<CParticle*>(m_pParticle.get())->SetCountX(1);
	static_cast<CParticle*>(m_pParticle.get())->SetCountY(1);
	static_cast<CParticle*>(m_pParticle.get())->SetRange(Vector3(0.f, 0.f, 0.f));
	static_cast<CParticle*>(m_pParticle.get())->SetImageSize(Vector2(256.f, 256.f));
	static_cast<CParticle*>(m_pParticle.get())->SetMaxCount(10000);
	static_cast<CParticle*>(m_pParticle.get())->SetSpawnCount(100);


	m_pUpperAni->AddSocket("Weapon", "Bip01-R-Hand");
	m_pUpperAni->SetSocketComponent("Weapon", m_pLance);
	m_pUpperAni->SetSocketOffset("Weapon", Vector3(4.f, 14.f, 1.f));
	m_pUpperAni->SetSocketRotation("Weapon", Vector3(0.f, 0.f, -9.f));

	m_pAlphaWeapon = CreateComponent<CSceneComponent>("AlphaLence", m_pScene->FindLayer(ALPHA_LAYER));
	m_pAlphaWeapon->SetEnable(false);

	static_cast<CSceneComponent*>(m_pLance.get())->AddChild(m_pAlphaWeapon);

	static_cast<CSceneComponent*>(m_pAlphaWeapon.get())->SetMesh("Lance");
	static_cast<CSceneComponent*>(m_pAlphaWeapon.get())->SetWorldScale(1.5f, 1.f, 1.5f);
	static_cast<CSceneComponent*>(m_pAlphaWeapon.get())->SetShader("LimLightAlpha");
	static_cast<CSceneComponent*>(m_pAlphaWeapon.get())->AddRenderState(ALPHA_BLEND);
	std::shared_ptr<CMaterial> pAlphaLanceMtrl = static_cast<CSceneComponent*>(m_pAlphaWeapon.get())->GetMaterial();
	if (pAlphaLanceMtrl)
	{
		pAlphaLanceMtrl->SetDiffuseColor(Vector4(208.f / 255.f, 0.f, 1.f, 120.f / 255.f));
		pAlphaLanceMtrl->SetEmissiveColor(Vector4(73.f / 255.f, 40.f/255.f, 243.f/ 255.f, 0.f));
	}

	std::shared_ptr<CComponent> pAlphaLancePaper = CreateComponent<CPaperBurn>("AlphaLancePaper");

	static_cast<CSceneComponent*>(m_pAlphaWeapon.get())->AddObjComponent(pAlphaLancePaper);

	static_cast<CPaperBurn*>(pAlphaLancePaper.get())->SetLoop(true);
	static_cast<CPaperBurn*>(pAlphaLancePaper.get())->SetColor(Vector3(180.f/255.f, 27.f/255.f, 226.f/255.f));
	static_cast<CPaperBurn*>(pAlphaLancePaper.get())->SetInColor(Vector3(1.f,78.f / 255.f, 0.f));
	static_cast<CPaperBurn*>(pAlphaLancePaper.get())->SetOutColor(Vector3(226.f / 255.f, 46.f / 255.f, 46.f / 255.f));
	static_cast<CPaperBurn*>(pAlphaLancePaper.get())->SetScale(2.2f);
	static_cast<CPaperBurn*>(pAlphaLancePaper.get())->SetInFilter(0.1f);
	static_cast<CPaperBurn*>(pAlphaLancePaper.get())->SetMidFilter(0.5f);
	static_cast<CPaperBurn*>(pAlphaLancePaper.get())->SetOutFilter(0.9f);

	m_pChargeParticle = CreateComponent<CParticle>("ChargeParticle", m_pScene->FindLayer(PARTICLE_LAYER));

	static_cast<CSceneComponent*>(m_pAlphaWeapon.get())->AddChild(m_pChargeParticle);

	GET_SINGLE(CResourceManager)->LoadTexture("FX_FireTile_011_TEX_KKJ", L"FX_Resource_KKJ_01\\Texture2D\\FX_FireTile_011_TEX_KKJ.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_Point_003_TEX_KKJ_clamp", L"FX_Resource_KKJ_01\\Texture2D\\FX_Point_003_TEX_KKJ_clamp.png");

	static_cast<CParticle*>(m_pChargeParticle.get())->SetShader("AlphaParticle");
	static_cast<CParticle*>(m_pChargeParticle.get())->SetRelativePosY(40.f);
	static_cast<CParticle*>(m_pChargeParticle.get())->SetMove(true);
	static_cast<CParticle*>(m_pChargeParticle.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_FireTile_011_TEX_KKJ");
	static_cast<CParticle*>(m_pChargeParticle.get())->SetTexture(LINK_TYPE::LT_EMI, "FX_Point_003_TEX_KKJ_clamp");
	static_cast<CParticle*>(m_pChargeParticle.get())->SetMaxFrame(1);
	static_cast<CParticle*>(m_pChargeParticle.get())->SetCountX(1);
	static_cast<CParticle*>(m_pChargeParticle.get())->SetCountY(1);
	static_cast<CParticle*>(m_pChargeParticle.get())->SetRange(Vector3(20.f, 20.f, 20.f));
	static_cast<CParticle*>(m_pChargeParticle.get())->SetMaxScale(Vector3(1.f, 1.f, 1.f));
	static_cast<CParticle*>(m_pChargeParticle.get())->SetMinScale(Vector3(1.f, 1.f, 1.f));
	static_cast<CParticle*>(m_pChargeParticle.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(m_pChargeParticle.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(m_pChargeParticle.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CParticle*>(m_pChargeParticle.get())->SetMaxSpeed(40.f);
	static_cast<CParticle*>(m_pChargeParticle.get())->SetMinSpeed(40.f);
	static_cast<CParticle*>(m_pChargeParticle.get())->SetImageSize(Vector2(256.f, 256.f));
	static_cast<CParticle*>(m_pChargeParticle.get())->SetMaxTime(2.f);
	static_cast<CParticle*>(m_pChargeParticle.get())->SetMinTime(2.f);
	static_cast<CParticle*>(m_pChargeParticle.get())->SetMaxCount(5000);
	static_cast<CParticle*>(m_pChargeParticle.get())->SetSpawnCount(500);	


	m_pHeadObj = m_pScene->CreateObject<CGameObject>("Head", m_pScene->FindLayer(ALPHA_LAYER));
	
	std::shared_ptr<CComponent> pHead = m_pHeadObj->CreateComponent<CSceneComponent>("Head");

	m_pHeadObj->SetRootComponent(pHead);

	static_cast<CSceneComponent*>(pHead.get())->SetMesh("PPlayerHead");
	static_cast<CSceneComponent*>(pHead.get())->SetShader("Alpha");
	static_cast<CSceneComponent*>(pHead.get())->SetShader("Alpha",0,1);
	static_cast<CSceneComponent*>(pHead.get())->AddRenderState("OutLineObject");
	static_cast<CSceneComponent*>(pHead.get())->AddRenderState("OutLineObject",0,1);
	static_cast<CSceneComponent*>(pHead.get())->SetRelativePos(0.f, 0.f, -2.5f);
	static_cast<CSceneComponent*>(pHead.get())->AddObjComponent(m_pPaper);
	static_cast<CSceneComponent*>(pHead.get())->SetPivot(0.f, 0.1f, 0.f);

	m_pFaceOutLine = CreateComponent<CSceneComponent>("OutLineFace", m_pScene->FindLayer(ALPHA_LAYER));

	static_cast<CSceneComponent*>(pHead.get())->AddChild(m_pFaceOutLine);
	static_cast<CSceneComponent*>(m_pFaceOutLine.get())->SetMesh("PPlayerHead");
	static_cast<CSceneComponent*>(m_pFaceOutLine.get())->SetWorldScale(1.05f, 1.05f, 1.05f);
	static_cast<CSceneComponent*>(m_pFaceOutLine.get())->AddRenderState("OutLine");
	static_cast<CSceneComponent*>(m_pFaceOutLine.get())->AddRenderState("OutLine", 0,1);
	static_cast<CSceneComponent*>(m_pFaceOutLine.get())->SetShader("OutLineAlphaShader");
	static_cast<CSceneComponent*>(m_pFaceOutLine.get())->SetShader("OutLineAlphaShader", 0, 1);



	std::shared_ptr<CComponent> pHair = m_pHeadObj->CreateComponent<CSceneComponent>("Hair", m_pScene->FindLayer(ALPHA_LAYER));

	static_cast<CSceneComponent*>(pHead.get())->AddChild(pHair);

	static_cast<CSceneComponent*>(pHair.get())->SetMesh("PPlayerHair");
	static_cast<CSceneComponent*>(pHair.get())->SetShader("Alpha");
	static_cast<CSceneComponent*>(pHair.get())->AddRenderState("OutLineObject");
	static_cast<CSceneComponent*>(pHair.get())->AddRenderState("NoCullRS");
	static_cast<CSceneComponent*>(pHair.get())->SetRelativePos(0.f, 0.f, 2.5f);
	static_cast<CSceneComponent*>(pHair.get())->AddObjComponent(m_pPaper);

	m_pHeadObj->SetShadowAll(true);
	m_pUpperAni->AddSocket("Head", "Bip01-Head");
	m_pUpperAni->SetSocketComponent("Head", pHead);
	m_pUpperAni->SetSocketRotation("Head", Vector3(0.f, 90.f, 0.f));

	m_pHairOutLine = CreateComponent<CSceneComponent>("HairOutLine", m_pScene->FindLayer(ALPHA_LAYER));

	static_cast<CSceneComponent*>(pHair.get())->AddChild(m_pHairOutLine);

	static_cast<CSceneComponent*>(m_pHairOutLine.get())->SetMesh("PPlayerHair");
	static_cast<CSceneComponent*>(m_pHairOutLine.get())->SetWorldScale(1.05f, 1.05f, 1.05f);
	static_cast<CSceneComponent*>(m_pHairOutLine.get())->AddRenderState("OutLine");
	static_cast<CSceneComponent*>(m_pHairOutLine.get())->SetShader("OutLineAlphaShader");

	m_pTailObj = m_pScene->CreateObject<CGameObject>("Tail", m_pScene->FindLayer(ALPHA_LAYER));
	

	std::shared_ptr<CComponent> pTail = m_pTailObj->CreateComponent<CSceneComponent>("Tail", m_pScene->FindLayer(ALPHA_LAYER));

	m_pTailObj->SetRootComponent(pTail);
	m_pTailObj->SetShadowAll(true);
	static_cast<CSceneComponent*>(pTail.get())->SetMesh("PPlayerTail");
	static_cast<CSceneComponent*>(pTail.get())->SetShader("Alpha");
	static_cast<CSceneComponent*>(pTail.get())->AddRenderState("OutLineObject");
	static_cast<CSceneComponent*>(pTail.get())->AddObjComponent(m_pPaper);

	m_pTailOutLine = m_pTailObj->CreateComponent<CSceneComponent>("TailOutLine", m_pScene->FindLayer(ALPHA_LAYER));

	static_cast<CSceneComponent*>(pTail.get())->AddChild(m_pTailOutLine);
	static_cast<CSceneComponent*>(m_pTailOutLine.get())->SetMesh("PPlayerTail");
	static_cast<CSceneComponent*>(m_pTailOutLine.get())->SetWorldScale(1.05f, 1.05f, 1.05f);
	static_cast<CSceneComponent*>(m_pTailOutLine.get())->AddRenderState("OutLine");
	static_cast<CSceneComponent*>(m_pTailOutLine.get())->SetShader("OutLineAlphaShader");

	m_pUpperAni->AddSocket("Tail", "Bip01-Pelvis");
	m_pUpperAni->SetSocketComponent("Tail", pTail);
	m_pUpperAni->SetSocketRotation("Tail", Vector3(0.f, 90.f, 0.f));

	m_pTop = m_pWeapon->CreateComponent<CSceneComponent>("Top");

	static_cast<CSceneComponent*>(m_pLance.get())->AddChild(m_pTop);

	static_cast<CSceneComponent*>(m_pTop.get())->SetRelativePosY(80.f);

	m_pBottom = m_pWeapon->CreateComponent<CSceneComponent>("Bottom");

	static_cast<CSceneComponent*>(m_pLance.get())->AddChild(m_pBottom);

	static_cast<CSceneComponent*>(m_pBottom.get())->SetRelativePosY(10.f);

	m_pBladeBody = m_pWeapon->CreateComponent<CColliderOBB>("BladeBody", m_pScene->FindLayer(ALPHA_LAYER));

	static_cast<CSceneComponent*>(m_pLance.get())->AddChild(m_pBladeBody);
	static_cast<CColliderOBB*>(m_pBladeBody.get())->AddRelativePosY(40.f);
	static_cast<CColliderOBB*>(m_pBladeBody.get())->SetExtent(Vector3(5.f, 40.f, 5.f));
	static_cast<CCollider*>(m_pBladeBody.get())->SetCallBack<CBladePlayer>(COLLISION_STATE::CS_INIT, this, &CBladePlayer::ColInit);
	static_cast<CCollider*>(m_pBladeBody.get())->SetCallBack<CBladePlayer>(COLLISION_STATE::CS_STAY, this, &CBladePlayer::ColStay);
	static_cast<CCollider*>(m_pBladeBody.get())->SetCallBack<CBladePlayer>(COLLISION_STATE::CS_LAST, this, &CBladePlayer::ColLast);

	CreateTrailMesh(m_iTrailCount);

	ID3D11SamplerState* pState = nullptr;

	D3D11_SAMPLER_DESC tDesc = {};
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	if (FAILED(DEVICE->CreateSamplerState(&tDesc, &pState)))
		return false;

	CONTEXT->PSSetSamplers(6, 1, &pState);

	SAFE_RELEASE(pState);



	return true;
}

void CBladePlayer::Start()
{
	CPlayer3D::Start();
}

void CBladePlayer::Update(float fTime)
{
	CPlayer3D::Update(fTime);

	static_cast<CSceneComponent*>(m_pLance.get())->SetWorldPos(0.f, 0.f, 0.f);

	if (m_iHP <= 0)
		return;

	if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
	{
		if (typeid(*m_pScene->GetGameMode()) != typeid(CTown))
		{
			if (m_pCoolTime[static_cast<int>(SKILL_TYPE::R)] <= 0.f && !m_bAttack)
			{
				switch (m_iCombo)
				{
				case 0:
					m_pUpperAni->ChangeSequence("combo1");
					break;
				case 1:
					m_pUpperAni->ChangeSequence("combo2");
					break;
				case 2:
					m_pUpperAni->ChangeSequence("combo3");
					break;
				case 3:
					m_pUpperAni->ChangeSequence("combo4");
					break;
				default:
					m_pUpperAni->ChangeSequence("SK_14");
					break;
				}
				m_fComboTime = 0.f;
			}
		}
	}

	if (m_iCombo > 0)
	{
		m_fComboTime += fTime;

		if (m_fComboTime >= m_fComboTimeMax)
		{
			m_fComboTime = 0.f;
			m_iCombo = 0;
		}
	}
}

void CBladePlayer::PostUpdate(float fTime)
{
	CPlayer3D::PostUpdate(fTime);


	if (m_pTrail)
	{
		std::shared_ptr<CComponent> pCom = m_pTrail->GetRootComponent();

		if (pCom)
		{
			std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pCom.get())->GetTransform();

			if (pTransform)
			{
				Vector3 vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
				Vector3 vMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
				for (int i = m_iTrailCount - 1; i >= 0; i--)
				{
					int iUpIndex = i * 2;
					int iDownIndex = iUpIndex + 1;
					int iNextUpIndex = iDownIndex + 1;
					int iNextDownIndex = iNextUpIndex + 1;

					m_vecVtx[iNextUpIndex].vPos = m_vecVtx[iUpIndex].vPos;
					m_vecVtx[iNextDownIndex].vPos = m_vecVtx[iDownIndex].vPos;

					if (vMin.x > m_vecVtx[iUpIndex].vPos.x)
						vMin.x = m_vecVtx[iUpIndex].vPos.x;
					if (vMin.y > m_vecVtx[iUpIndex].vPos.y)
						vMin.y = m_vecVtx[iUpIndex].vPos.y;
					if (vMin.z > m_vecVtx[iUpIndex].vPos.z)
						vMin.z = m_vecVtx[iUpIndex].vPos.z;
					if (vMax.x < m_vecVtx[iUpIndex].vPos.x)
						vMax.x = m_vecVtx[iUpIndex].vPos.x;
					if (vMax.y < m_vecVtx[iUpIndex].vPos.y)
						vMax.y = m_vecVtx[iUpIndex].vPos.y;
					if (vMax.z < m_vecVtx[iUpIndex].vPos.z)
						vMax.z = m_vecVtx[iUpIndex].vPos.z;

					if (vMin.x > m_vecVtx[iDownIndex].vPos.x)
						vMin.x = m_vecVtx[iDownIndex].vPos.x;
					if (vMin.y > m_vecVtx[iDownIndex].vPos.y)
						vMin.y = m_vecVtx[iDownIndex].vPos.y;
					if (vMin.z > m_vecVtx[iDownIndex].vPos.z)
						vMin.z = m_vecVtx[iDownIndex].vPos.z;
					if (vMax.x < m_vecVtx[iDownIndex].vPos.x)
						vMax.x = m_vecVtx[iDownIndex].vPos.x;
					if (vMax.y < m_vecVtx[iDownIndex].vPos.y)
						vMax.y = m_vecVtx[iDownIndex].vPos.y;
					if (vMax.z < m_vecVtx[iDownIndex].vPos.z)
						vMax.z = m_vecVtx[iDownIndex].vPos.z;
				}

				class CMesh* pMesh = GET_SINGLE(CResourceManager)->FindMesh("Trail");
				if (pMesh)
				{
					pMesh->ChangeMeshBuffer(&m_vecVtx[0], (m_iTrailCount + 1) * 2 * sizeof(_tagVertexTrail));
				}

				pTransform->SetMeshPos((vMax + vMin) / 2.f);
				pTransform->SetMeshSize(vMax - vMin);
			}
		}
	}
}

void CBladePlayer::Collision(float fTime)
{
	CPlayer3D::Collision(fTime);
}

void CBladePlayer::PreRender(float fTime)
{
	CPlayer3D::PreRender(fTime);
}

void CBladePlayer::Render(float fTime)
{
	CPlayer3D::Render(fTime);
}

void CBladePlayer::PostRender(float fTime)
{
	CPlayer3D::PostRender(fTime);
}

std::shared_ptr<CGameObject> CBladePlayer::Clone()
{
	return std::shared_ptr<CGameObject>(new CBladePlayer(*this));
}

void CBladePlayer::Save(FILE* pFile)
{
	CPlayer3D::Save(pFile);
}

void CBladePlayer::Load(FILE * pFile)
{
	CPlayer3D::Load(pFile);
}

void CBladePlayer::Trail(float fAnimationTime, float fDeltaTime)
{
	m_fTrailTime += fDeltaTime;

	if (m_fTrailTime >= m_fTrailTimeLimit)
	{
		m_fTrailTime -= m_fTrailTimeLimit;

		if (m_pTrail)
		{
			std::shared_ptr<CComponent> pCom = m_pTrail->GetRootComponent();

			if (pCom)
			{
				std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pCom.get())->GetTransform();

				if (pTransform)
				{
					Matrix matWorld = static_cast<CSceneComponent*>(m_pLance.get())->GetMatWorld();

					Vector3 vTop = static_cast<CSceneComponent*>(m_pTop.get())->GetRelativePos();
					Vector3 vButtom = static_cast<CSceneComponent*>(m_pBottom.get())->GetRelativePos();

					Vector3 vTopPos = {};
					Vector3 vBottomPos = {};

					vTopPos.x = vTop.y * matWorld[1][0] + matWorld[3][0];
					vTopPos.y = vTop.y * matWorld[1][1] + matWorld[3][1];
					vTopPos.z = vTop.y * matWorld[1][2] + matWorld[3][2];
					vBottomPos.x = vButtom.y * matWorld[1][0] + matWorld[3][0];
					vBottomPos.y = vButtom.y * matWorld[1][1] + matWorld[3][1];
					vBottomPos.z = vButtom.y * matWorld[1][2] + matWorld[3][2];

					/*Vector3 vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
					Vector3 vMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
					for (int i = m_iTrailCount - 1; i >= 0; i--)
					{
						m_vecVtx[(i + 1) * 2].vPos = m_vecVtx[i * 2].vPos;
						m_vecVtx[(i + 1) * 2 + 1].vPos = m_vecVtx[i * 2 + 1].vPos;

						if (vMin.x > m_vecVtx[i * 2].vPos.x)
							vMin.x = m_vecVtx[i * 2].vPos.x;
						if (vMin.y > m_vecVtx[i * 2].vPos.y)
							vMin.y = m_vecVtx[i * 2].vPos.y;
						if (vMin.z > m_vecVtx[i * 2].vPos.z)
							vMin.z = m_vecVtx[i * 2].vPos.z;
						if (vMax.x < m_vecVtx[i * 2].vPos.x)
							vMax.x = m_vecVtx[i * 2].vPos.x;
						if (vMax.y < m_vecVtx[i * 2].vPos.y)
							vMax.y = m_vecVtx[i * 2].vPos.y;
						if (vMax.z < m_vecVtx[i * 2].vPos.z)
							vMax.z = m_vecVtx[i * 2].vPos.z;

						if (vMin.x > m_vecVtx[i * 2 + 1].vPos.x)
							vMin.x = m_vecVtx[i * 2 + 1].vPos.x;
						if (vMin.y > m_vecVtx[i * 2 + 1].vPos.y)
							vMin.y = m_vecVtx[i * 2 + 1].vPos.y;
						if (vMin.z > m_vecVtx[i * 2 + 1].vPos.z)
							vMin.z = m_vecVtx[i * 2 + 1].vPos.z;
						if (vMax.x < m_vecVtx[i * 2 + 1].vPos.x)
							vMax.x = m_vecVtx[i * 2 + 1].vPos.x;
						if (vMax.y < m_vecVtx[i * 2 + 1].vPos.y)
							vMax.y = m_vecVtx[i * 2 + 1].vPos.y;
						if (vMax.z < m_vecVtx[i * 2 + 1].vPos.z)
							vMax.z = m_vecVtx[i * 2 + 1].vPos.z;
					}*/

					m_vecVtx[0].vPos = vTopPos;
					m_vecVtx[1].vPos = vBottomPos;

					/*for (int i = 0; i < m_iTrailCount + 1; i++)
					{
						m_vecVtx[((iFrame + i) * 2) % ((m_iTrailCount + 1) * 2)].vUV = Vector2(1.f - (i / 2) / static_cast<float>(m_iTrailCount), 0.f);


						m_vecVtx[((iFrame + i) * 2 + 1) % ((m_iTrailCount + 1) * 2)].vUV = Vector2(1.f - (i / 2) / static_cast<float>(m_iTrailCount), 1.f);
					}*/
					/*class CMesh* pMesh = GET_SINGLE(CResourceManager)->FindMesh("Trail");
					if (pMesh)
					{
						pMesh->ChangeMeshBuffer(&m_vecVtx[0], (m_iTrailCount + 1) * 2 * sizeof(_tagVertexTrail));
					}*/

					/*pTransform->SetMeshPos((vMax + vMin) / 2.f);
					pTransform->SetMeshSize(vMax - vMin);*/
				}
			}
		}

	}
}

void CBladePlayer::TrailEnd(float fTime)
{
	m_pTrail->SetEnable(false);
	m_pBladeBody->SetEnable(false); 
	static_cast<CParticle*>(m_pParticle.get())->SetSpawnCount(0);

	class CMesh* pMesh = GET_SINGLE(CResourceManager)->FindMesh("Trail");

	if (pMesh)
	{
		if (!m_vecVtx.empty())
		{
			CLayer* pLayer = m_pScene->FindLayer(SWORD_LIGHT_LAYER);

			if (pLayer)
			{
				std::shared_ptr<CGameObject> pObj = pLayer->FindObj("Trail");

				if (pObj)
				{
					std::shared_ptr<CComponent> pCom = pObj->GetRootComponent();

					if (pCom)
					{
						std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pCom.get())->GetTransform();

						if (pTransform)
						{
							Vector3 vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
							Vector3 vMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

							for (int i = 0; i < m_vecVtx.size(); i++)
							{
								if (vMin.x > m_vecVtx[i].vPos.x)
									vMin.x = m_vecVtx[i].vPos.x;
								if (vMin.y > m_vecVtx[i].vPos.y)
									vMin.y = m_vecVtx[i].vPos.y;
								if (vMin.z > m_vecVtx[i].vPos.z)
									vMin.z = m_vecVtx[i].vPos.z;
								if (vMax.x < m_vecVtx[i].vPos.x)
									vMax.x = m_vecVtx[i].vPos.x;
								if (vMax.y < m_vecVtx[i].vPos.y)
									vMax.y = m_vecVtx[i].vPos.y;
								if (vMax.z < m_vecVtx[i].vPos.z)
									vMax.z = m_vecVtx[i].vPos.z;

								//memcpy_s(&m_vecVtx[i].vPos.x,12, &m_vecVtx[0].vPos.x, 12);
							}

							pTransform->SetMeshPos((vMax + vMin)/2.f);
							pTransform->SetMeshSize(vMax - vMin);
						}
					}
				}
			}
		}	
	}
}

void CBladePlayer::TrailStart(float fTime)
{
	m_pTrail->SetEnable(true);
	m_pBladeBody->SetEnable(true);
	static_cast<CSceneComponent*>(m_pBladeBody.get())->InheritPos();
	static_cast<CParticle*>(m_pParticle.get())->SetSpawnCount(100);
	switch (m_iCombo)
	{
	case 0:
		GET_SINGLE(CSoundManager)->Play("Lancer_ComboAttack_00");
		break;
	case 1:
		GET_SINGLE(CSoundManager)->Play("Lancer_ComboAttack_01");
		break;
	case 2:
		GET_SINGLE(CSoundManager)->Play("Lancer_ComboAttack_02");
		break;
	}
}

void CBladePlayer::ComboAdd(float fTime)
{
	m_iCombo++;
	m_fComboTime = 0.f;

}

void CBladePlayer::ComboEnd(float fTime)
{
	m_iCombo = 0;
	m_fComboTime = 0.f;
}

void CBladePlayer::ShowWindow()
{
	CPlayer3D::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		bool bTrail = m_pTrail->IsEnable();
		if (ImGui::Checkbox("Trail", &bTrail))
		{
			m_pTrail->SetEnable(bTrail);
		}

		if (ImGui::InputInt("Trail Count",&m_iTrailCount))
		{
			CreateTrailMesh(m_iTrailCount);
		}

		if (ImGui::InputFloat("Trail Time Limit", &m_fTrailTimeLimit))
		{
			SetTrailTimeLimit(m_fTrailTimeLimit);
		}

		ImGui::InputFloat("Trail Start Alpha", &m_fTrailStartAlpha);
		ImGui::InputFloat("Trail End Alpha", &m_fTrailEndAlpha);


	}
	ImGui::End();
}

void CBladePlayer::CreateTrailMesh(int iCount)
{
	m_vecVtx.clear();

	m_iTrailCount = iCount;

	if (m_pTrail)
	{
		m_pTrail->Destroy();
		m_pTrail = nullptr;
	}

	m_pTrail = m_pScene->CreateObject<CGameObject>("Trail", m_pScene->FindLayer(SWORD_LIGHT_LAYER));

	std::shared_ptr<CComponent> pCom = m_pTrail->CreateComponent<CSceneComponent>("Renderer");

	m_pTrail->SetRootComponent(pCom);

	for (int i = 0; i < m_iTrailCount + 1; i++)
	{
		_tagVertexTrail tVtx = {};

		tVtx.vPos = Vector3(i * 30.f, 0.f, 0.f);
		tVtx.vUV = Vector2( (i / 2) / static_cast<float>(m_iTrailCount) * 2.f, 0.f);
		tVtx.fAlpha = m_fTrailStartAlpha + i / static_cast<float>(m_iTrailCount) * (m_fTrailEndAlpha - m_fTrailStartAlpha);

		m_vecVtx.push_back(tVtx);

		tVtx.vPos = Vector3(i * 30.f, 30.f, 0.f);
		tVtx.vUV = Vector2((i / 2) / static_cast<float>(m_iTrailCount) * 2.f, 1.f);
		tVtx.fAlpha = m_fTrailStartAlpha + i / static_cast<float>(m_iTrailCount) * (m_fTrailEndAlpha - m_fTrailStartAlpha);
		m_vecVtx.push_back(tVtx);
	}

	int iVtx = 2 * (m_iTrailCount + 1);

	m_vecVtx.resize(iVtx);

	int iIdx = m_iTrailCount * 6;

	unsigned short* pIdx = new unsigned short[iIdx];

	for (int i = 0; i < m_iTrailCount; i++)
	{
		pIdx[i * 6] = 2 * i;
		pIdx[i * 6 + 1] = 2 * i + 1;
		pIdx[i * 6 + 2] = 2 * i + 3;
		pIdx[i * 6 + 3] = 2 * i;
		pIdx[i * 6 + 4] = 2 * i + 3;
		pIdx[i * 6 + 5] = 2 * i + 2;
	}

	GET_SINGLE(CResourceManager)->DeleteMesh("Trail");
	if (!GET_SINGLE(CResourceManager)->CreateMesh("Trail", MESH_TYPE::MT_STATIC_MESH,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (m_iTrailCount + 1) * 2,
		static_cast<int>(sizeof(_tagVertexTrail)), D3D11_USAGE_DYNAMIC, &m_vecVtx[0], m_iTrailCount * 6, 2, D3D11_USAGE_IMMUTABLE,
		DXGI_FORMAT_R16_UINT, pIdx, D3D11_CPU_ACCESS_WRITE))
	{
		SAFE_DELETE_ARRAY(pIdx);
		return;
	}

	delete[] pIdx;

	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Trail");
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState(CULL_NONE);
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState(ALPHA_BLEND);
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState(NO_DEPTH);
	static_cast<CSceneComponent*>(pCom.get())->SetMaterial("Color");
	std::shared_ptr<CMaterial> pMtrl = static_cast<CSceneComponent*>(pCom.get())->GetMaterial();

	static_cast<CSceneComponent*>(pCom.get())->SetShader("Trail");

	if (pMtrl)
	{
		pMtrl->SetEmissiveColor(1.f, 0.f, 1.f, 1.f);
		//pMtrl->SetTexture(LINK_TYPE::LT_DIF, "FX_Trail_012_TEX_PYS");
	}
}

void CBladePlayer::SetTransition()
{
	CPlayer3D::SetTransition();

	m_pUpperAni->AutoTransition("combo1", "combo1ToIdle", "Idle");
	m_pUpperAni->AutoTransition("combo2", "combo2ToIdle", "Idle");
	m_pUpperAni->AutoTransition("combo3", "combo3ToIdle", "Idle");
	m_pUpperAni->AutoTransition("combo4", "combo4ToIdle", "Idle");
	m_pUpperAni->AutoTransition("swordDanceStart", "swordDanceStartToLoop", "swordDanceLoop");
	m_pUpperAni->AutoTransition("swordDanceShot", "swordDanceShotToIdle", "Idle");

	m_pUpperAni->AddTransition("IdleTocombo1", "Idle", TransitionFunc, "combo1");
	m_pUpperAni->AddTransition("combo1Tocombo2", "combo1", TransitionFunc, "combo2");
	m_pUpperAni->AddTransition("IdleTocombo2", "Idle", TransitionFunc, "combo2");
	m_pUpperAni->AddTransition("combo2Tocombo3", "combo2", TransitionFunc, "combo3");
	m_pUpperAni->AddTransition("IdleTocombo3", "Idle", TransitionFunc, "combo3");
	m_pUpperAni->AddTransition("combo3Tocombo4", "combo3", TransitionFunc, "combo4");
	m_pUpperAni->AddTransition("IdleTocombo4", "Idle", TransitionFunc, "combo4");
	m_pUpperAni->AddTransition("IdleToswordDanceStart", "Idle", TransitionFunc, "swordDanceStart");
	m_pUpperAni->AddTransition("swordDanceLoopToShot", "swordDanceLoop", TransitionFunc, "swordDanceShot");

	m_pUpperAni->AddTransitionNotify("combo1ToIdle", "combo1",
		static_cast<CPlayer3D*>(this), &CPlayer3D::AttackEnd);
	m_pUpperAni->AddTransitionNotify("IdleTocombo1", "Idle",
		static_cast<CPlayer3D*>(this), &CPlayer3D::AttackStart);
	m_pUpperAni->AddTransitionNotify("combo2ToIdle", "combo2",
		static_cast<CPlayer3D*>(this), &CPlayer3D::AttackEnd);
	m_pUpperAni->AddTransitionNotify("IdleTocombo2", "Idle",
		static_cast<CPlayer3D*>(this), &CPlayer3D::AttackStart);
	m_pUpperAni->AddTransitionNotify("combo3ToIdle", "combo3",
		static_cast<CPlayer3D*>(this), &CPlayer3D::AttackEnd);
	m_pUpperAni->AddTransitionNotify("IdleTocombo3", "Idle",
		static_cast<CPlayer3D*>(this), &CPlayer3D::AttackStart);
	m_pUpperAni->AddTransitionNotify("combo4ToIdle", "combo4",
		static_cast<CPlayer3D*>(this), &CPlayer3D::AttackEnd);
	m_pUpperAni->AddTransitionNotify("IdleTocombo4", "Idle",
		static_cast<CPlayer3D*>(this), &CPlayer3D::AttackStart);
	m_pUpperAni->AddTransitionNotify("swordDanceShotToIdle", "swordDanceShot",
		static_cast<CPlayer3D*>(this), &CPlayer3D::AttackEnd);
	m_pUpperAni->AddTransitionNotify("IdleToswordDanceStart", "Idle",
		static_cast<CPlayer3D*>(this), &CPlayer3D::AttackStart);
	m_pUpperAni->AddTransitionNotify("IdleToswordDanceStart", "Idle",
		this, &CBladePlayer::Skill1StartSound);
	m_pUpperAni->AddTransitionNotify("swordDanceLoopToShot", "swordDanceLoop",
		this, &CBladePlayer::Skill1EndSound);

}

void CBladePlayer::ColInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string	strDest = pDest->GetName();

	if (strDest == "MonsterBody" || strDest == "GoblinBody")
	{
		CGameObject* pMonster = pDest->GetObj();

		if (!static_cast<CMonster3D*>(pMonster)->IsDie())
		{
			CAnimationSequence* pSequence = m_pUpperAni->GetSequence();

			if (pSequence->GetName() == "PPlayer_swordDanceShot")
			{
				GET_SINGLE(CSoundManager)->Play("Lancer_MenaceWave_Explosion");
			}
			else
			{
				switch (rand() % 3)
				{
				case 0:
					GET_SINGLE(CSoundManager)->Play("Damage01_00");
					break;
				case 1:
					GET_SINGLE(CSoundManager)->Play("Damage01_01");
					break;
				case 2:
					GET_SINGLE(CSoundManager)->Play("Damage01_02");
					break;
				}
			}

			std::shared_ptr<CGameObject> pLight = m_pScene->CreateObject<CParticle3D>("Point Light", m_pScene->FindLayer(ALBEDO_LAYER));

			Vector3 vCross = pSrc->GetCrs();

			static_cast<CParticle3D*>(pLight.get())->SetLimitTime(0.5f);
			static_cast<CParticle3D*>(pLight.get())->SetWorldPos(vCross);
			std::shared_ptr<CComponent> pLightCom = static_cast<CParticle3D*>(pLight.get())->CreateComponent<CLight>("Point Light");

			static_cast<CParticle3D*>(pLight.get())->AddChild(pLightCom);
			static_cast<CLight*>(pLightCom.get())->SetRange(128.f);
			static_cast<CLight*>(pLightCom.get())->SetLightType(LIGHT_TYPE::POINT);
			static_cast<CLight*>(pLightCom.get())->SetDifColor(Vector4(1.f, 0.f, 0.f, 1.f));
			static_cast<CLight*>(pLightCom.get())->SetAmbColor(Vector4(1.f, 0.f, 0.f, 1.f));
			static_cast<CLight*>(pLightCom.get())->SetSpcColor(Vector4(1.f, 0.f, 0.f, 1.f));
			static_cast<CLight*>(pLightCom.get())->SetEmiColor(Vector4(1.f, 0.f, 0.f, 1.f));

			std::shared_ptr<CComponent> pDistortion = pLight->CreateComponent<CSceneComponent>("Distortion");

			static_cast<CParticle3D*>(pLight.get())->AddChild(pDistortion);

			CDistortion* pDist = static_cast<CSceneComponent*>(pDistortion.get())->AddPostProcess<CDistortion>("Distortion");
			static_cast<CSceneComponent*>(pDistortion.get())->SetMesh("Cube2");
			static_cast<CSceneComponent*>(pDistortion.get())->SetMaterial("Color");
			static_cast<CSceneComponent*>(pDistortion.get())->SetShader("Distortion");
			pDist->SetSpeed(1.f);
			pDist->SetFrequency(1.f);
			pDist->SetWeight(1.f);

			SAFE_RELEASE(pDist);

			static_cast<CParticle3D*>(pLight.get())->SetScaleSpeed(150.f);

			std::shared_ptr<CGameObject> pParticle = m_pScene->CreateCloneObj("Hit Particle", "Hit Particle", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
			if (pParticle)
			{
				pParticle->SetWorldPos(vCross);
			}
		}
	}
}

void CBladePlayer::ColStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CBladePlayer::ColLast(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CBladePlayer::AttackEnd()
{
	CPlayer3D::AttackEnd();

	TrailEnd(0.f);
}

void CBladePlayer::Skill1StartSound()
{
	GET_SINGLE(CSoundManager)->Play("Lancer_MenaceWave_Casting");
}

void CBladePlayer::Skill1EndSound()
{
	GET_SINGLE(CSoundManager)->Play("Lancer_MenaceWave_Shot");
}

void CBladePlayer::Skill1(float fTime)
{
	if (m_iHP <= 0)
		return;

	if (m_pUpperAni->GetSequence()->GetName() == "PPlayer_swordDanceLoop")
	{
		m_pCoolTime[static_cast<int>(SKILL_TYPE::_1)] += m_pCoolTimeMax[static_cast<int>(SKILL_TYPE::_1)];
		m_pUpperAni->ChangeSequence("swordDanceShot");
		m_pAlphaWeapon->SetEnable(false);
	}
}

void CBladePlayer::Skill1Down(float fTime)
{
	if (m_iHP <= 0)
		return;

	if (m_pCoolTime[static_cast<int>(SKILL_TYPE::_1)] <= 0.f && !m_bAttack)
	{
		m_pUpperAni->ChangeSequence("swordDanceStart");
		m_pAlphaWeapon->SetEnable(true);
	}
}

void CBladePlayer::SetOutLine(bool bLine)
{
	CPlayer3D::SetOutLine(bLine);

	if (m_pTailOutLine)
		m_pTailOutLine->SetEnable(bLine);

	if (m_pFaceOutLine)
		m_pFaceOutLine->SetEnable(bLine);

	if (m_pHairOutLine)
		m_pHairOutLine->SetEnable(bLine);
}

void CBladePlayer::SetLayer(CLayer* pLayer)
{
	CPlayer3D::SetLayer(pLayer);

	if (m_pTailObj)
	{
		pLayer->AddObj(m_pTailObj);
	}
	if (m_pHeadObj)
	{
		pLayer->AddObj(m_pHeadObj);
	}
	if (m_pTrail)
	{
		pLayer->AddObj(m_pTrail);
	}
}

void CBladePlayer::SetScene(CScene* pScene)
{
	CPlayer3D::SetScene(pScene);
}
