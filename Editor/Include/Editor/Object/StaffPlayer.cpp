#include "StaffPlayer.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"
#include "Scene/Scene.h"
#include "Component/PaperBurn.h"
#include "Component/Transform.h"
#include "Resource/ResourceManager.h"
#include "Equip.h"
#include "Inventory3D.h"
#include "../Item.h"
#include "Resource/Mesh.h"
#include "Render/Distortion.h"
#include "Component/Particle.h"
#include "Resource/Material.h"
#include "Particle3D.h"
#include "Monster3D.h"
#include "Component/Light.h"
#include "Sound/SoundManager.h"
#include "../GameMode/Town.h"
#include "Component/Decal.h"
#include "Component/ColliderSphere.h"

CStaffPlayer::CStaffPlayer()	:
	m_iTrailCount(20)
{
}

CStaffPlayer::CStaffPlayer(const CStaffPlayer& obj)	:
	CPlayer3D(obj)
	, m_iTrailCount(obj.m_iTrailCount)
{
}

CStaffPlayer::~CStaffPlayer()
{
}

bool CStaffPlayer::Init()
{

	//GET_SINGLE(CResourceManager)->LoadSkeleton("Elf_RB3004", "BPlayer\\Ani2\\Elf_RB3004.bne");
	/*GET_SINGLE(CResourceManager)->LoadSkeleton("Elf_EC4Bone", "BPlayer\\Elf_EC4.bne");
	GET_SINGLE(CResourceManager)->AddSkeleton("Elf_EC4Bone", "Elf_RB3004", "BPlayer\\Elf_EC4.bne");*/

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "PlayerMesh", TEXT("BPlayer\\Elf_EC1.msh"));
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "PlayerMeshCopy", TEXT("BPlayer\\Elf_EC1.msh"));
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "Elf_RB3004", TEXT("BPlayer\\Elf_RB3004.msh"));
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "PlayerStaff", TEXT("BPlayer\\Staff.msh"));

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	//GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "Elf_EC4", TEXT("BPlayer\\Elf_MS.msh"));
	//GET_SINGLE(CResourceManager)->LoadSkeleton("Elf_MS_Bone", "BPlayer\\Elf_MS.bne");
	
	/*GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_jump_loop_WP",			false,			L"BPlayer\\AniEC1\\jump_loop_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Jump_Up_WP",			false,			L"BPlayer\\AniEC1\\Jump_Up_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Idle_WP",				false,			L"BPlayer\\AniEC1\\Idle_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_SK_14",					false,			L"BPlayer\\AniEC1\\SK_14.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_sk_08_upper",			false,			L"BPlayer\\AniEC1\\sk_08_upper.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Walk_F_WP",				false,			L"BPlayer\\AniEC1\\Walk_F_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Die",					false,			L"BPlayer\\AniEC1\\Die.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Walk_B_WP",				false,			L"BPlayer\\AniEC1\\Walk_B_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_DMG_F_WP",				false,			L"BPlayer\\AniEC1\\DMG_F_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_run_jump_up_F_WP",		false,			L"BPlayer\\AniEC1\\run_jump_up_F_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_run_jump_loop_f_WP",	false,			L"BPlayer\\AniEC1\\run_jump_loop_f_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_run_jump_land_f_WP",	false,			L"BPlayer\\AniEC1\\run_jump_land_f_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Run_F_WP",				false,			L"BPlayer\\AniEC1\\Run_F_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Run_FR_WP",				false,			L"BPlayer\\AniEC1\\Run_FR_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Run_FL_WP",				false,			L"BPlayer\\AniEC1\\Run_FL_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Run_B_WP",				false,			L"BPlayer\\AniEC1\\Run_B_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Run_BR_WP",				false,			L"BPlayer\\AniEC1\\Run_BR_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Run_BL_WP",				false,			L"BPlayer\\AniEC1\\Run_BL_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Run_L_WP",				false,			L"BPlayer\\AniEC1\\Run_L_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Run_R_WP",				false,			L"BPlayer\\AniEC1\\Run_R_WP.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Down",					false,			L"BPlayer\\AniEC1\\Down.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_Down_Loop",				false,			L"BPlayer\\AniEC1\\Down_Loop.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("New_Player_down_deadbody",			false,			L"BPlayer\\AniEC1\\down_deadbody.sqc");
	
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Jump_Land_WP",								 "Player_Jump_Land_WP",						 "BPlayer\\Jump_Land_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_jump_loop_WP",								 "Player_jump_loop_WP",						 "BPlayer\\jump_loop_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Jump_Up_WP",									 "Player_Jump_Up_WP",						 "BPlayer\\Jump_Up_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Idle_WP",									"Player_Idle_WP",								 "BPlayer\\Idle_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_SK_14",										 "Player_SK_14",							 "BPlayer\\SK_14.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_sk_08_upper",								 "Player_sk_08_upper",						 "BPlayer\\sk_08_upper.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Walk_F_WP",									"Player_Walk_F_WP",								 "BPlayer\\Walk_F_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Die",										 "Player_Die",								 "BPlayer\\Die.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Walk_B_WP",									 "Player_Walk_B_WP",							 "BPlayer\\Walk_B_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_DMG_F_WP",									 "Player_DMG_F_WP",							 "BPlayer\\DMG_F_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_run_jump_up_F_WP",							 "Player_run_jump_up_F_WP",					 "BPlayer\\run_jump_up_F_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_run_jump_loop_f_WP",							 "Player_run_jump_loop_f_WP",				 "BPlayer\\run_jump_loop_f_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_run_jump_land_f_WP",							 "Player_run_jump_land_f_WP",				 "BPlayer\\run_jump_land_f_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Run_F_WP",									 "Player_Run_F_WP",							 "BPlayer\\Run_F_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Run_FR_WP",									 "Player_Run_FR_WP",						 "BPlayer\\Run_FR_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Run_FL_WP",									 "Player_Run_FL_WP",						 "BPlayer\\Run_FL_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Run_B_WP",									 "Player_Run_B_WP",							 "BPlayer\\Run_B_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Run_BR_WP",									 "Player_Run_BR_WP",						 "BPlayer\\Run_BR_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Run_BL_WP",									 "Player_Run_BL_WP",						 "BPlayer\\Run_BL_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Run_L_WP",									 "Player_Run_L_WP",							 "BPlayer\\Run_L_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Run_R_WP",									 "Player_Run_R_WP",							 "BPlayer\\Run_R_WP.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Down",										 "Player_Down",								 "BPlayer\\Down.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_Down_Loop",									 "Player_Down_Loop",						 "BPlayer\\Down_Loop.sqc");
	GET_SINGLE(CResourceManager)->AddAnimationSequence("New_Player_down_deadbody",								 "Player_down_deadbody",					 "BPlayer\\down_deadbody.sqc");*/

	GET_SINGLE(CResourceManager)->LoadTexture("FX_Flame_001_Tex_CJH",
		L"FX_Resource_CJH_01\\Texture2D\\FX_Flame_001_Tex_CJH.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_trail_007_TEX_CJH",
		L"FX_Resource_CJH_01\\Texture2D\\FX_trail_007_TEX_CJH.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_NormalTile_017_TEX_KKJ",
		L"FX_Resource_KKJ_01\\Texture2D\\FX_NormalTile_017_TEX_KKJ.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_fumefire_003_TEX_KJS",
		L"FX_Resource_KJS_01\\Texture2D\\FX_fumefire_003_TEX_KJS.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_explosion_002_TEX_KJS",
		L"FX_Resource_KJS_01\\Texture2D\\FX_explosion_002_TEX_KJS.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_Explosion_Sub_08_White6",
		L"FX_Resource_KKJ_05\\Texture2D\\FX_Explosion_Sub_08_White6.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_FlameFlip_002_TEX_HKB",
		L"FX_Resource_KKJ_05\\Texture2D\\FX_FlameFlip_002_TEX_HKB.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_FlameFlip_004_TEX_HKB",
		L"FX_Resource_KKJ_05\\Texture2D\\FX_FlameFlip_004_TEX_HKB.dds");

	GET_SINGLE(CResourceManager)->LoadTexture("FX_ice_001_TEX_ksj",
		L"FX_Resource_KJS_01\\Texture2D\\FX_ice_001_TEX_ksj.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_icetrail_003_TEX_KJS",
		L"FX_Resource_KJS_01\\Texture2D\\FX_icetrail_003_TEX_KJS.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_fumefie_005_TEX_KJS",
		L"FX_Resource_KJS_01\\Texture2D\\FX_fumefie_005_TEX_KJS.dds");

	GET_SINGLE(CResourceManager)->LoadTexture("FX_CloudTile_015_TEX_KKJ",
		L"FX_Resource_KKJ_01\\Texture2D\\FX_CloudTile_015_TEX_KKJ.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_Crack_001_TEX_HKB",
		L"FX_Resource_HKB_02\\Texture2D\\FX_Crack_001_TEX_HKB.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_Flare_006_TEX_KKJ",
		L"FX_Resource_KKJ_02\\Texture2D\\FX_Flare_006_TEX_KKJ.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_Wave_001_TEX_PHS",
		L"FX_Resource_PHS_01\\Texture2D\\FX_Wave_001_TEX_PHS.dds");

	GET_SINGLE(CResourceManager)->LoadTexture("FX_WaveTile_007_TEX_KKJ",
		L"FX_Resource_KKJ_01\\Texture2D\\FX_WaveTile_007_TEX_KKJ.dds");

	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Sorcerer_BlastMine_Casting_00", false,
		"Skill\\Sorcerer_BlastMine_Casting_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Sorcerer_BlastMine_Loop", false,
		"Skill\\Sorcerer_BlastMine_Loop.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Sorcerer_FireBall_Casting_00", false,
		"Skill\\Sorcerer_FireBall_Casting_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Sorcerer_FireBall_Loop", false,
		"Skill\\Sorcerer_FireBall_Loop.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Sorcerer_LightOfRuin_Casting_00", false,
		"Skill\\Sorcerer_LightOfRuin_Casting_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Sorcerer_LightOfRuin_Explosion", false,
		"Skill\\Sorcerer_LightOfRuin_Explosion.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Awaken_Sorcerer_IceSpear_Cast_01", false,
		"Skill\\Awaken_Sorcerer_IceSpear_Cast_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Awaken_Sorcerer_IceSpear_Debris_01", false,
		"Skill\\Awaken_Sorcerer_IceSpear_Debris_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Awaken_Sorcerer_IceSpear_Debris_02", false,
		"Skill\\Awaken_Sorcerer_IceSpear_Debris_02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Awaken_Sorcerer_IceSpear_Debris_03", false,
		"Skill\\Awaken_Sorcerer_IceSpear_Debris_03.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Awaken_Sorcerer_IceSpear_Debris_04", false,
		"Skill\\Awaken_Sorcerer_IceSpear_Debris_04.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Awaken_Sorcerer_IceSpear_Debris_05", false,
		"Skill\\Awaken_Sorcerer_IceSpear_Debris_05.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Awaken_Sorcerer_IceSpear_Shot_01", false,
		"Skill\\Awaken_Sorcerer_IceSpear_Shot_01.ogg");

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif

	m_pMesh = CreateComponent<CSceneComponent>("Renderer");
	CSceneComponent* _pUpper = static_cast<CSceneComponent*>(m_pMesh.get());

	_pUpper->SetWorldScale(1.f, 1.f, 1.f);
	_pUpper->SetWorldPos(300.f, 1.f, 3000.f);
	_pUpper->SetMesh("PlayerMeshCopy");

	SetRootComponent(m_pMesh);

	if (!CPlayer3D::Init())
		return false;

	_pUpper->AddRenderState("OutLineObject", 0, 0);
	_pUpper->AddRenderState("OutLineObject", 0, 1);
	_pUpper->AddRenderState("OutLineObject", 1, 0);
	_pUpper->AddRenderState("OutLineObject", 2, 0);
	_pUpper->AddRenderState("OutLineObject", 3, 0);
	_pUpper->AddRenderState("OutLineObject", 4, 0);
	_pUpper->AddRenderState("OutLineObject", 4, 1);
	_pUpper->AddRenderState("OutLineObject", 5, 0);
	_pUpper->AddRenderState("OutLineObject", 6, 0);
	_pUpper->AddRenderState("OutLineObject", 7, 0);
	_pUpper->AddRenderState("OutLineObject", 7, 1);
	_pUpper->AddRenderState("OutLineObject", 7, 2);
	_pUpper->AddRenderState("OutLineObject", 8, 0);
	_pUpper->AddRenderState("OutLineObject", 8, 1);
	_pUpper->AddRenderState("OutLineObject", 8, 2);
	_pUpper->AddRenderState("OutLineObject", 8, 3);
	_pUpper->AddRenderState("OutLineObject", 8, 4);
	_pUpper->AddRenderState("OutLineObject", 8, 5);
	_pUpper->AddRenderState("OutLineObject", 8, 6);
	_pUpper->AddRenderState("OutLineObject", 8, 7);
	_pUpper->AddRenderState("OutLineObject", 8, 8);
	_pUpper->AddRenderState("OutLineObject", 8, 9);
	_pUpper->AddRenderState("OutLineObject", 8, 10);
	_pUpper->AddRenderState("OutLineObject", 8, 11);
	_pUpper->AddRenderState("OutLineObject", 8, 12);
	_pUpper->AddRenderState("OutLineObject", 8, 13);
	_pUpper->AddRenderState("OutLineObject", 8, 14);
	_pUpper->AddRenderState("OutLineObject", 8, 15);
	_pUpper->AddRenderState("OutLineObject", 8, 16);
	_pUpper->AddRenderState("OutLineObject", 9, 0);

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif

	m_pUpperAni = CAnimation::CreateAnimation<CAnimation>("UpperAni", "Elf_EC4Bone", "BPlayer\\Elf_EC4.bne");
	m_pUpperAni->SetSkeleton("Elf_EC4Bone");

	CAnimationSequence* pSequence = GET_SINGLE(CResourceManager)->FindSequence("Player_SK_14");

	if (pSequence)
	{
		for (int i = 1; i < 6; ++i)
		{
			pSequence->SetBlendMask(i, (i - 1) * 0.2f + 0.1f);
		}
		for (int i = 6; i < 170; ++i)
		{
			pSequence->SetBlendMask(i, 1.f);
		}
	}

	SAFE_RELEASE(pSequence); 

	pSequence = GET_SINGLE(CResourceManager)->FindSequence("Player_sk_08_upper");

	if (pSequence)
	{
		for (int i = 1; i < 6; ++i)
		{
			pSequence->SetBlendMask(i, (i - 1) * 0.2f + 0.1f);
		}
		for (int i = 6; i < 170; ++i)
		{
			pSequence->SetBlendMask(i, 1.f);
		}
	}

	SAFE_RELEASE(pSequence);

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	std::vector<const char*> vecSeq;

	vecSeq.push_back("Player_Run_F_WP");
	vecSeq.push_back("Player_Run_FR_WP");
	vecSeq.push_back("Player_Run_FL_WP");
	vecSeq.push_back("Player_Run_B_WP");
	vecSeq.push_back("Player_Run_BR_WP");
	vecSeq.push_back("Player_Run_BL_WP");
	vecSeq.push_back("Player_Run_L_WP");
	vecSeq.push_back("Player_Run_R_WP");

	for (int j = 0; j < 8; ++j)
	{
		pSequence = GET_SINGLE(CResourceManager)->FindSequence(vecSeq[j]);

		if (pSequence)
		{
			for (int i = 1; i < 6; ++i)
			{
				pSequence->SetBlendMask(i, 1.f - ((i - 1) * 0.2f + 0.1f));
			}
			for (int i = 6; i < 170; ++i)
			{
				pSequence->SetBlendMask(i, 0.f);
			}
			for (int i = 170; i < 246; ++i)
			{
				pSequence->SetBlendMask(i, 1.f);
			}
		}

		SAFE_RELEASE(pSequence);
	}

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	m_pUpperAni->AddAnimationSequence("Idle", "Player_Idle_WP");
	m_pUpperAni->AddAnimationSequence("Jump_Land_WP", "Player_Jump_Land_WP");
	m_pUpperAni->AddAnimationSequence("jump_loop_WP", "Player_jump_loop_WP");
	m_pUpperAni->AddAnimationSequence("Jump_Up_WP", "Player_Jump_Up_WP");
	m_pUpperAni->AddAnimationSequence("SK_14", "Player_SK_14");
	m_pUpperAni->AddAnimationSequence("sk_08_upper", "Player_sk_08_upper");
	m_pUpperAni->AddAnimationSequence("Walk", "Player_Walk_F_WP");
	m_pUpperAni->AddAnimationSequence("Die", "Player_Die_WP");
	m_pUpperAni->AddAnimationSequence("WalkBack", "Player_Walk_B_WP");
	m_pUpperAni->AddAnimationSequence("DMG_F_WP", "Player_DMG_F_WP");
	m_pUpperAni->AddAnimationSequence("run_jump_up_F_WP", "Player_run_jump_up_F_WP");
	m_pUpperAni->AddAnimationSequence("run_jump_loop_f_WP", "Player_run_jump_loop_f_WP");
	m_pUpperAni->AddAnimationSequence("run_jump_land_f_WP", "Player_run_jump_land_f_WP");
	m_pUpperAni->AddAnimationSequence("Run_F_WP", "Player_Run_F_WP");
	m_pUpperAni->AddAnimationSequence("Run_FR_WP", "Player_Run_FR_WP");
	m_pUpperAni->AddAnimationSequence("Run_FL_WP", "Player_Run_FL_WP");
	m_pUpperAni->AddAnimationSequence("Run_B_WP", "Player_Run_B_WP");
	m_pUpperAni->AddAnimationSequence("Run_BR_WP", "Player_Run_BR_WP");
	m_pUpperAni->AddAnimationSequence("Run_BL_WP", "Player_Run_BL_WP");
	m_pUpperAni->AddAnimationSequence("Run_L_WP", "Player_Run_L_WP");
	m_pUpperAni->AddAnimationSequence("Run_R_WP", "Player_Run_R_WP");
	m_pUpperAni->AddAnimationSequence("Down", "Player_Down");
	m_pUpperAni->AddAnimationSequence("Down_Loop", "Player_Down_Loop");
	m_pUpperAni->AddAnimationSequence("down_deadbody", "Player_down_deadbody");
	m_pUpperAni->AddAnimationSequence("sk_28_cst_start", "Player_sk_28_cst_start");
	m_pUpperAni->AddAnimationSequence("sk_28_cst_loop", "Player_sk_28_cst_loop");
	m_pUpperAni->AddAnimationSequence("sk_28_cst_end", "Player_sk_28_cst_end");
	m_pUpperAni->AddAnimationSequence("sk_11_cst_start_Upper", "Player_sk_11_cst_start_Upper");
	m_pUpperAni->AddAnimationSequence("sk_11_cst_loop_Upper", "Player_sk_11_cst_loop_Upper");
	m_pUpperAni->AddAnimationSequence("sk_11_cst_end_Upper", "Player_sk_11_cst_end_Upper");
	m_pUpperAni->AddEndCallBack<CStaffPlayer>("SK_14", this, &CStaffPlayer::AttackEnd);
	m_pUpperAni->AddEndCallBack<CStaffPlayer>("sk_08_upper", this, &CStaffPlayer::AttackEnd);
	m_pUpperAni->AddEndCallBack<CStaffPlayer>("sk_11_cst_end_Upper", this, &CStaffPlayer::AttackEnd);

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	//m_pUpperAni->CreateNotify("SK_14", "AttackEnd", 37.2f);
	//m_pUpperAni->CreateNotify("sk_08_upper", "sk08End", 37.2f);
	_pUpper->SetAnimation(m_pUpperAni);

	m_pWeapon = m_pScene->CreateObject<CGameObject>("Staff", m_pScene->FindLayer(ALBEDO_LAYER));
	m_pWeapon->SetShadowAll(true);

	std::shared_ptr<CComponent> pStaff = CreateComponent<CSceneComponent>("Staff");

	m_pWeapon->SetRootComponent(pStaff);

	static_cast<CSceneComponent*>(pStaff.get())->SetWorldScale(1.f, 1.f, 1.f);
	static_cast<CSceneComponent*>(pStaff.get())->SetMesh("PlayerStaff");
	static_cast<CSceneComponent*>(pStaff.get())->AddRenderState("OutLineObject");

	m_pUp = m_pWeapon->CreateComponent<CSceneComponent>("Up");

	static_cast<CSceneComponent*>(pStaff.get())->AddChild(m_pUp);

	static_cast<CSceneComponent*>(m_pUp.get())->AddRelativePosY(5.f);

	m_pDown = m_pWeapon->CreateComponent<CSceneComponent>("Down");
	static_cast<CSceneComponent*>(pStaff.get())->AddChild(m_pDown);
	static_cast<CSceneComponent*>(m_pDown.get())->AddRelativePosY(-5.f);

	m_pFire = m_pWeapon->CreateComponent<CParticle>("Fire", m_pScene->FindLayer(SWORD_LIGHT_LAYER));

	m_pFire->SetEnable(false);
	static_cast<CSceneComponent*>(pStaff.get())->AddChild(m_pFire);

	static_cast<CParticle*>(m_pFire.get())->AddRelativePosY(18.f);
	static_cast<CParticle*>(m_pFire.get())->SetShader("ParticleBlackClip");	
	static_cast<CParticle*>(m_pFire.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_fumefire_003_TEX_KJS");
	static_cast<CParticle*>(m_pFire.get())->SetMaxSpawnTime(0.f);
	static_cast<CParticle*>(m_pFire.get())->SetMaxScale(Vector3(20.f, 20.f, 1.f));
	static_cast<CParticle*>(m_pFire.get())->SetMinScale(Vector3(20.f, 20.f, 1.f));
	static_cast<CParticle*>(m_pFire.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(m_pFire.get())->SetMinTime(0.4f);
	static_cast<CParticle*>(m_pFire.get())->SetMaxTime(0.4f);	
	static_cast<CParticle*>(m_pFire.get())->SetImageSize(Vector2(1024.f, 1024.f));
	static_cast<CParticle*>(m_pFire.get())->SetCountX(6);
	static_cast<CParticle*>(m_pFire.get())->SetCountY(6);
	static_cast<CParticle*>(m_pFire.get())->SetMaxFrame(36);

	m_pUpperAni->AddSocket("Weapon", "Bip01-R-Hand");
	m_pUpperAni->SetSocketComponent("Weapon", pStaff);
	m_pUpperAni->SetSocketOffset("Weapon", Vector3(1.5f, 0.f, 0.5f));
	m_pUpperAni->SetSocketRotation("Weapon", Vector3(0.f, 0.f, 0.f));

	m_pUpperOutLine = CreateComponent<CSceneComponent>("UpperOutLine");

	_pUpper->AddChild(m_pUpperOutLine);
	m_pUpperOutLine->SetEnable(false);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetMesh("PlayerMeshCopy");
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 0, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 0, 1);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 1, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 2, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 3, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 4, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 4, 1);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 5, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 6, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 7, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 7, 1);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 7, 2);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 1);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 2);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 3);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 4);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 5);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 6);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 7);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 8);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 9);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 10);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 11);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 12);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 13);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 14);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 15);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 8, 16);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetShader("OutLineShader", 9, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetWorldScale(1.05f, 1.05f, 1.05f);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetRelativePosY(-0.5f);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 0, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 0, 1);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 1, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 2, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 3, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 4, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 4, 1);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 5, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 6, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 7, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 7, 1);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 7, 2);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 1);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 2);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 3);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 4);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 5);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 6);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 7);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 8);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 9);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 10);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 11);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 12);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 13);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 14);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 15);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 8, 16);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->AddRenderState("OutLine", 9, 0);
	static_cast<CSceneComponent*>(m_pUpperOutLine.get())->SetAnimation(m_pUpperAni);
	std::shared_ptr<CMaterial> pUpperLineMtrl = static_cast<CSceneComponent*>(m_pUpperOutLine.get())->GetMaterial();

	m_pStaffOutLine = CreateComponent<CSceneComponent>("StaffOutLine");
	m_pStaffOutLine->SetEnable(false);
	static_cast<CSceneComponent*>(pStaff.get())->AddChild(m_pStaffOutLine);

	static_cast<CSceneComponent*>(m_pStaffOutLine.get())->SetMesh("PlayerStaff");
	static_cast<CSceneComponent*>(m_pStaffOutLine.get())->SetShader("OutLineShader");
	static_cast<CSceneComponent*>(m_pStaffOutLine.get())->SetWorldScale(1.05f, 1.05f, 1.05f);
	static_cast<CSceneComponent*>(m_pStaffOutLine.get())->AddRenderState("OutLine");
	SetTransition();

	std::shared_ptr<CGameObject> pInven = m_pScene->FindLayer(UI_LAYER)->FindObj("Inventory");

	std::shared_ptr<CGameObject> pEquip = m_pScene->FindLayer(UI_LAYER)->FindObj("Equip");

	if (pInven)
	{
		static_cast<CInventory3D*>(pInven.get())->AddItem(CItem::FindItem("세트1헬멧"));
		static_cast<CInventory3D*>(pInven.get())->SetSelect(0);
		static_cast<CEquip*>(pEquip.get())->SetItem(3);
		static_cast<CInventory3D*>(pInven.get())->AddItem(CItem::FindItem("세트1어깨"));
		static_cast<CInventory3D*>(pInven.get())->SetSelect(0);
		static_cast<CEquip*>(pEquip.get())->SetItem(4);
		static_cast<CInventory3D*>(pInven.get())->AddItem(CItem::FindItem("세트1상의"));
		static_cast<CInventory3D*>(pInven.get())->SetSelect(0);
		static_cast<CEquip*>(pEquip.get())->SetItem(5);
		static_cast<CInventory3D*>(pInven.get())->AddItem(CItem::FindItem("세트1벨트"));
		static_cast<CInventory3D*>(pInven.get())->SetSelect(0);
		static_cast<CEquip*>(pEquip.get())->SetItem(6);
		static_cast<CInventory3D*>(pInven.get())->AddItem(CItem::FindItem("세트1하의"));
		static_cast<CInventory3D*>(pInven.get())->SetSelect(0);
		static_cast<CEquip*>(pEquip.get())->SetItem(8);
		static_cast<CInventory3D*>(pInven.get())->AddItem(CItem::FindItem("세트1장갑"));
		static_cast<CInventory3D*>(pInven.get())->SetSelect(0);
		static_cast<CEquip*>(pEquip.get())->SetItem(7);
		static_cast<CInventory3D*>(pInven.get())->AddItem(CItem::FindItem("세트1신발"));
		static_cast<CInventory3D*>(pInven.get())->SetSelect(0);
		static_cast<CEquip*>(pEquip.get())->SetItem(9);
	}

	m_pUpperAni->CreateNotify("sk_11_cst_start_Upper", "sk11Trail", 152.266f, 153.25f);
	m_pUpperAni->AddTimeLineNotifyFunction("sk11Trail", this, 
		&CStaffPlayer::UpdateTrail);
	m_pUpperAni->AddTransitionNotify("IdleTosk_11_cst_start", "Idle", this, 
		&CStaffPlayer::StartTrail);
	m_pUpperAni->AddTransitionNotify("sk_11_cst_startToLoop", "sk_11_cst_start_Upper", this, 
		&CStaffPlayer::EndTrail);

	m_pUpperAni->CreateNotify("sk_11_cst_end_Upper", "sk11Boom", 150.725f);
	m_pUpperAni->AddNotifyFunction("sk11Boom", this, &CStaffPlayer::Skill_11);

	int iVtx = (m_iTrailCount + 1) * 2;

	m_vecTrailVtx.resize(iVtx);

	for (int j=0, i = 0; i < m_iTrailCount + 1; i++)
	{
		VERTEXTRAIL tVtx = {};
		tVtx.vPos.x = i / static_cast<float>(m_iTrailCount) * 500.f;
		tVtx.vPos.y = 100.f;
		tVtx.vUV.x = i / static_cast<float>(m_iTrailCount);
		tVtx.vUV.y = 0.f;
		tVtx.fAlpha = 1.f;
		m_vecTrailVtx[j++] = tVtx;
		VERTEXTRAIL tVtx2 = {};
		tVtx2.vPos.x = i / static_cast<float>(m_iTrailCount) * 500.f;
		tVtx2.vPos.y = 0.f;
		tVtx2.vUV.x = i / static_cast<float>(m_iTrailCount);
		tVtx2.vUV.y = 1.f;
		tVtx2.fAlpha = 1.f;
		m_vecTrailVtx[j++] = tVtx2;
	}

	int iIdx = m_iTrailCount * 6;

	std::vector<unsigned short> vecIdx(iIdx);

	for (int j=0, i = 0; i < m_iTrailCount; i++)
	{
		vecIdx[j++] = i * 2;
		vecIdx[j++] = i * 2 + 3;
		vecIdx[j++] = i * 2 + 1;
		vecIdx[j++] = i * 2;
		vecIdx[j++] = i * 2 + 2;
		vecIdx[j++] = i * 2 + 3;
	}

	GET_SINGLE(CResourceManager)->CreateMesh("FireTrail", MESH_TYPE::MT_STATIC_MESH,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, iVtx, static_cast<int>(sizeof(VERTEXTRAIL)),
		D3D11_USAGE_DYNAMIC, &m_vecTrailVtx[0], m_iTrailCount * 6, 2, D3D11_USAGE_IMMUTABLE,
		DXGI_FORMAT_R16_UINT, &vecIdx[0], D3D11_CPU_ACCESS_WRITE);

	m_pTrailObj = m_pScene->CreateObject<CGameObject>("TrailObj", m_pScene->FindLayer(SWORD_LIGHT_LAYER));

	std::shared_ptr<CComponent> pTrail = m_pTrailObj->CreateComponent<CSceneComponent>("TrailB");

	m_pTrailObj->SetRootComponent(pTrail);

	m_pTrailObj->SetEnable(false);

	static_cast<CSceneComponent*>(pTrail.get())->SetMesh("FireTrail");
	static_cast<CSceneComponent*>(pTrail.get())->SetMaterial("Color");
	static_cast<CSceneComponent*>(pTrail.get())->SetShader("TrailTexture");
	static_cast<CSceneComponent*>(pTrail.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_Flame_001_Tex_CJH");
	static_cast<CSceneComponent*>(pTrail.get())->SetTexture(LINK_TYPE::LT_EMI, "FX_trail_007_TEX_CJH");
	static_cast<CSceneComponent*>(pTrail.get())->AddRenderState("NoCullRS");

	std::shared_ptr<CComponent> _pTrail = m_pTrailObj->CreateComponent<CSceneComponent>("Trail", m_pScene->FindLayer(PARTICLE_LAYER));

	static_cast<CSceneComponent*>(pTrail.get())->AddChild(_pTrail);

	static_cast<CSceneComponent*>(_pTrail.get())->SetMesh("FireTrail");
	static_cast<CSceneComponent*>(_pTrail.get())->SetMaterial("Color");
	std::shared_ptr<CMaterial> pFireMtrl = static_cast<CSceneComponent*>(_pTrail.get())->GetMaterial();

	pFireMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f);
	static_cast<CSceneComponent*>(_pTrail.get())->SetShader("TrailTexture");
	static_cast<CSceneComponent*>(_pTrail.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_Flame_001_Tex_CJH");
	static_cast<CSceneComponent*>(_pTrail.get())->SetTexture(LINK_TYPE::LT_EMI, "FX_trail_007_TEX_CJH");
	static_cast<CSceneComponent*>(_pTrail.get())->AddRenderState("NoCullRS");

	m_pIceCollider = CreateComponent<CColliderSphere>("IceCollider", 
		m_pScene->FindLayer(ALBEDO_LAYER));

	static_cast<CColliderSphere*>(m_pIceCollider.get())->SetRadius(500.f);
	static_cast<CColliderSphere*>(m_pIceCollider.get())->SetCallBack<CStaffPlayer>(
		COLLISION_STATE::CS_INIT, this, &CStaffPlayer::CollisionInit);
	static_cast<CColliderSphere*>(m_pIceCollider.get())->SetCallBack<CStaffPlayer>(
		COLLISION_STATE::CS_STAY, this, &CStaffPlayer::CollisionStay);
	static_cast<CColliderSphere*>(m_pIceCollider.get())->SetCallBack<CStaffPlayer>(
		COLLISION_STATE::CS_LAST, this, &CStaffPlayer::CollisionEnd);
	static_cast<CSceneComponent*>(m_pMesh.get())->AddChild(m_pIceCollider);

	m_pIceCollider->SetEnable(false);

	return true;
}

void CStaffPlayer::Start()
{
	CPlayer3D::Start();
}

void CStaffPlayer::Update(float fTime)
{
	CPlayer3D::Update(fTime);

	if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::RIGHT))
	{
		if (typeid(*m_pScene->GetGameMode()) != typeid(CTown))
		{
			if (m_pCoolTime[static_cast<int>(SKILL_TYPE::R)] <= 0.f && !m_bAttack && !m_bAir && !m_bSequenceMode)
			{
				GET_SINGLE(CSoundManager)->Play("Sorcerer_FireBall_Casting_00");
							
				CAnimationSequence* pSequence = m_pUpperAni->GetSequence();
				if (pSequence)
				{
					if (pSequence->GetName() == "Player_Idle_WP")
					{
						m_pUpperAni->ChangeSequence("SK_14");
					}
					else
					{
						m_pUpperAni->SetBlendSequence("SK_14");
					}
				}
				else
				{
					m_pUpperAni->ChangeSequence("SK_14");
				}
				
			}
		}
	}
}

void CStaffPlayer::PostUpdate(float fTime)
{
	CPlayer3D::PostUpdate(fTime);
}

void CStaffPlayer::Collision(float fTime)
{
	CPlayer3D::Collision(fTime);
}

void CStaffPlayer::PreRender(float fTime)
{
	CPlayer3D::PreRender(fTime);
}

void CStaffPlayer::Render(float fTime)
{
	CPlayer3D::Render(fTime);
}

void CStaffPlayer::PostRender(float fTime)
{
	CPlayer3D::PostRender(fTime);
}

std::shared_ptr<CGameObject> CStaffPlayer::Clone()
{
	return std::shared_ptr<CGameObject>(new CStaffPlayer(*this));
}

void CStaffPlayer::Save(FILE* pFile)
{
	CPlayer3D::Save(pFile);
}

void CStaffPlayer::Load(FILE* pFile)
{
	CPlayer3D::Load(pFile);
}

void CStaffPlayer::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strDest = pDest->GetName();

	if (strDest == "MonsterBody" ||
		strDest == "GoblinBody")
	{
		Vector3 vDestPos = pDest->GetWorldPos();

		Vector3 vPos  = GetWorldPos();

		Vector3 vDir = vDestPos - vPos;

		vDir.Normalize();

		float fAngle = acosf(GetWorldAxis(AXIS_X).Dot(vDir));

		if (fAngle < PI / 3.f)
		{
			static_cast<CMonster3D*>(pDest->GetObj())->CalcDamage(
				static_cast<int>(m_iAttackMax * 1.3f),
				static_cast<int>(m_iAttackMin * 1.3f));
		}
	}
}

void CStaffPlayer::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CStaffPlayer::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CStaffPlayer::sk08Start()
{
	if (m_bAttack)
		return;

	if (m_iHP <= 0)
		return;

	GET_SINGLE(CSoundManager)->Play("Sorcerer_BlastMine_Loop");
	std::shared_ptr<CGameObject> pBullet = m_pScene->CreateCloneObj("Bullet", "BlueBullet", m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

	m_pCoolTime[static_cast<int>(SKILL_TYPE::_1)] += m_pCoolTimeMax[static_cast<int>(SKILL_TYPE::_1)];

	if (pBullet)
	{
		std::shared_ptr<CComponent> pBulletRoot = pBullet->GetRootComponent();

		std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pBulletRoot.get())->GetTransform();

		pTransform->AddQuaternionRotY(RadToDeg(m_fRot) - 90.f);
		pTransform->AddQuaternionRotX(m_fAngle);

		SetWorldRotY(RadToDeg(m_fRot) - 180.f);
		std::shared_ptr<CComponent> pRoot = GetRootComponent();

		if (pRoot)
		{
			std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pRoot.get())->GetTransform();

			if (pTransform)
			{
				Matrix matBone = pTransform->GetMatBone();

				Vector3 vBonePos = {};
				memcpy_s(&vBonePos, 12, &matBone[3][0], 12);
				pBullet->SetWorldPos(GetWorldPos() + vBonePos);
			}
		}
	}
	m_bAttack = true;
}
void CStaffPlayer::CastFinish()
{
	CPlayer3D::CastFinish();

	switch (m_eSkillType)
	{
	case SKILL_TYPE::NONE:
		SpawnRide();
		break;
	case SKILL_TYPE::R:
		break;
	case SKILL_TYPE::_1:
		break;
	case SKILL_TYPE::_2:
		break;
	case SKILL_TYPE::_3:
	{
		m_pIceCollider->SetEnable(false);
	}
		break;
	case SKILL_TYPE::_4:
		break;
	}

}
void CStaffPlayer::CastCancel()
{
	CPlayer3D::CastCancel();
	switch (m_eSkillType)
	{
	case SKILL_TYPE::NONE:
		break;
	case SKILL_TYPE::R:
		break;
	case SKILL_TYPE::_1:
		break;
	case SKILL_TYPE::_2:
		m_pFire->SetEnable(false);
		m_pTrailObj->SetEnable(false);
		break;
	case SKILL_TYPE::_3:
		m_pFire->SetEnable(false);
		m_pTrailObj->SetEnable(false);
		break;
	case SKILL_TYPE::_4:
		break;
	}
}
void CStaffPlayer::CaskSkill()
{
	switch (m_eSkillType)
	{
	case SKILL_TYPE::NONE:
		//SpawnRide();
		break;
	case SKILL_TYPE::R:
		break;
	case SKILL_TYPE::_1:
		break;
	case SKILL_TYPE::_2:
		break;
	case SKILL_TYPE::_3:
	{
		std::shared_ptr<CGameObject> pParticleObj = m_pScene->CreateObject<CParticle3D>("IceFlare", m_pScene->FindLayer(PARTICLE_LAYER));

		static_cast<CParticle3D*>(pParticleObj.get())->SetLimitTime(1.f);

		std::shared_ptr<CComponent> pCom = pParticleObj->CreateComponent<CParticle>("IceFlare");

		pParticleObj->SetRootComponent(pCom);

		static_cast<CParticle*>(pCom.get())->SetShader("ParticleBlackClip");
		static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_Flare_006_TEX_KKJ");
		static_cast<CParticle*>(pCom.get())->SetMaxScale(Vector3(50.f, 50.f, 1.f));
		static_cast<CParticle*>(pCom.get())->SetMinScale(Vector3(50.f, 50.f, 1.f));
		static_cast<CParticle*>(pCom.get())->SetMaxTime(1.f);
		static_cast<CParticle*>(pCom.get())->SetMinTime(1.f);
		static_cast<CParticle*>(pCom.get())->SetMaxSpawnTime(1.f);
		static_cast<CParticle*>(pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
		static_cast<CParticle*>(pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 0.f));

		pParticleObj->SetWorldPos(static_cast<CSceneComponent*>(m_pFire.get())->GetBoneWorldPos());
	}
	break;
	case SKILL_TYPE::_4:
		break;
	}
}
void CStaffPlayer::SetTransition()
{
	CPlayer3D::SetTransition();
	m_pUpperAni->AddTransitionNotify("sk_11_cstToEnd", "sk_11_cst_loop_Upper", this,
		&CStaffPlayer::CaskSkill);
}
void CStaffPlayer::Skill1(float fTime)
{
	if (m_pCoolTime[static_cast<int>(SKILL_TYPE::_1)] <= 0.f && !m_bAttack && !m_bSequenceMode)
	{
		if (m_pUpperAni->GetSequence()->GetName() == "Player_Idle_WP")
		{
			m_pUpperAni->ChangeSequence("sk_08_upper");
		}
		else
		{
			m_pUpperAni->SetBlendSequence("sk_08_upper");
		}
		m_eSkillType = SKILL_TYPE::_1;
		GET_SINGLE(CSoundManager)->Play("Sorcerer_BlastMine_Casting_00");
	}
}
void CStaffPlayer::Skill2(float fTime)
{
	if (typeid(*m_pScene->GetGameMode()) != typeid(CTown))
	{
		if (m_pCoolTime[static_cast<int>(SKILL_TYPE::_2)] <= 0.f && !m_bAttack && !m_bSequenceMode && m_pTarget && !m_bCast)
		{
			m_eSkillType = SKILL_TYPE::_2;
			m_pUpperAni->ChangeSequence("sk_11_cst_start_Upper");
			m_bCast = true;
			m_fCastMax = 1.5f;
			GET_SINGLE(CSoundManager)->Play("Sorcerer_LightOfRuin_Casting_00");
		}
	}
}
void CStaffPlayer::Skill3(float fTime)
{
	if (typeid(*m_pScene->GetGameMode()) != typeid(CTown))
	{
		if (m_pCoolTime[static_cast<int>(SKILL_TYPE::_3)] <= 0.f && !m_bAttack && !m_bSequenceMode && !m_bCast)
		{
			m_eSkillType = SKILL_TYPE::_3;
			m_pUpperAni->ChangeSequence("sk_11_cst_start_Upper");
			m_bCast = true;
			m_fCastMax = 2.5f;
			GET_SINGLE(CSoundManager)->Play("Awaken_Sorcerer_IceSpear_Cast_01");
		}
	}
}
void CStaffPlayer::UpdateTrail(float fAniTime, float fDeltaTime)
{
	Vector3 vUpPos = static_cast<CSceneComponent*>(m_pUp.get())->GetBoneWorldPos();
	Vector3 vDownPos = static_cast<CSceneComponent*>(m_pDown.get())->GetBoneWorldPos();

	Vector3 vMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Vector3 vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

	for (int i = m_iTrailCount-1; i >= 0; i--)
	{
		int iLT = i * 2;
		int iLD = i * 2 + 1;
		int iRT = i * 2 + 2;
		int iRD = i * 2 + 3;
		m_vecTrailVtx[iRT].vPos = m_vecTrailVtx[iLT].vPos;
		m_vecTrailVtx[iRD].vPos = m_vecTrailVtx[iLD].vPos;

		if (m_vecTrailVtx[iRT].vPos.x > vMax.x)
			vMax.x = m_vecTrailVtx[iRT].vPos.x;
		if (m_vecTrailVtx[iRT].vPos.y > vMax.y)
			vMax.y = m_vecTrailVtx[iRT].vPos.y;
		if (m_vecTrailVtx[iRT].vPos.z > vMax.z)
			vMax.z = m_vecTrailVtx[iRT].vPos.z;
		if (m_vecTrailVtx[iRD].vPos.x > vMax.x)
			vMax.x = m_vecTrailVtx[iRD].vPos.x;
		if (m_vecTrailVtx[iRD].vPos.y > vMax.y)
			vMax.y = m_vecTrailVtx[iRD].vPos.y;
		if (m_vecTrailVtx[iRD].vPos.z > vMax.z)
			vMax.z = m_vecTrailVtx[iRD].vPos.z;
		if (m_vecTrailVtx[iRT].vPos.x < vMin.x)
			vMin.x = m_vecTrailVtx[iRT].vPos.x;
		if (m_vecTrailVtx[iRT].vPos.y < vMin.y)
			vMin.y = m_vecTrailVtx[iRT].vPos.y;
		if (m_vecTrailVtx[iRT].vPos.z < vMin.z)
			vMin.z = m_vecTrailVtx[iRT].vPos.z;
		if (m_vecTrailVtx[iRD].vPos.x < vMin.x)
			vMin.x = m_vecTrailVtx[iRD].vPos.x;
		if (m_vecTrailVtx[iRD].vPos.y < vMin.y)
			vMin.y = m_vecTrailVtx[iRD].vPos.y;
		if (m_vecTrailVtx[iRD].vPos.z < vMin.z)
			vMin.z = m_vecTrailVtx[iRD].vPos.z;
	}

	m_vecTrailVtx[0].vPos = vUpPos;
	m_vecTrailVtx[1].vPos = vDownPos;

	CMesh* pMesh = GET_SINGLE(CResourceManager)->FindMesh("FireTrail");

	pMesh->ChangeMeshBuffer(&m_vecTrailVtx[0], static_cast<int>(sizeof(VERTEXTRAIL) * m_vecTrailVtx.size()));

	m_pTrailObj->SetMeshSize(vMax - vMin);
	std::shared_ptr<CComponent> pRoot = m_pTrailObj->GetRootComponent();

	static_cast<CSceneComponent*>(pRoot.get())->SetMeshPos((vMax + vMin) / 2.f);

	pRoot = m_pTrailObj->FindComponent("Trail");
	static_cast<CSceneComponent*>(pRoot.get())->SetMeshPos((vMax + vMin) / 2.f);
	static_cast<CSceneComponent*>(pRoot.get())->SetMeshSize(vMax - vMin);
	//SAFE_RELEASE(pMesh);
}
void CStaffPlayer::StartTrail()
{
	if (m_pTrailObj)
	{
		switch (m_eSkillType)
		{
		case SKILL_TYPE::_2:
		{
			static_cast<CSceneComponent*>(m_pTrailObj->GetRootComponent().get())->SetTexture(
				LINK_TYPE::LT_DIF, "FX_Flame_001_Tex_CJH");
			static_cast<CSceneComponent*>(m_pTrailObj->GetRootComponent().get())->SetTexture(
				LINK_TYPE::LT_EMI, "FX_trail_007_TEX_CJH");

			std::shared_ptr<CComponent> pCom = m_pTrailObj->FindComponent("Trail");

			static_cast<CSceneComponent*>(pCom.get())->SetTexture(
				LINK_TYPE::LT_DIF, "FX_Flame_001_Tex_CJH");
			static_cast<CSceneComponent*>(pCom.get())->SetTexture(
				LINK_TYPE::LT_EMI, "FX_trail_007_TEX_CJH");

			static_cast<CParticle*>(m_pFire.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_fumefire_003_TEX_KJS");

			m_pFire->SetEnable(true);
			m_pTrailObj->SetEnable(true);

			Vector3 vPosUp = static_cast<CSceneComponent*>(m_pUp.get())->GetBoneWorldPos();
			Vector3 vPosDown = static_cast<CSceneComponent*>(m_pDown.get())->GetBoneWorldPos();

			size_t iSize = m_vecTrailVtx.size();

			for (size_t i = 0; i < iSize / 2; ++i)
			{
				int iIdx1 = static_cast<int>(i * 2);
				int iIdx2 = static_cast<int>(i * 2 + 1);
				m_vecTrailVtx[iIdx1].vPos = vPosUp;
				m_vecTrailVtx[iIdx2].vPos = vPosDown;
			}
		}
			break;
		case SKILL_TYPE::_3:
		{
			static_cast<CSceneComponent*>(m_pTrailObj->GetRootComponent().get())->SetTexture(
				LINK_TYPE::LT_DIF, "FX_ice_001_TEX_ksj");
			static_cast<CSceneComponent*>(m_pTrailObj->GetRootComponent().get())->SetTexture(
				LINK_TYPE::LT_EMI, "FX_icetrail_003_TEX_KJS");

			std::shared_ptr<CComponent> pCom = m_pTrailObj->FindComponent("Trail");

			static_cast<CSceneComponent*>(pCom.get())->SetTexture(
				LINK_TYPE::LT_DIF, "FX_ice_001_TEX_ksj");
			static_cast<CSceneComponent*>(pCom.get())->SetTexture(
				LINK_TYPE::LT_EMI, "FX_icetrail_003_TEX_KJS");

			static_cast<CParticle*>(m_pFire.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_fumefie_005_TEX_KJS");

			m_pFire->SetEnable(true);
			m_pTrailObj->SetEnable(true);

			Vector3 vPosUp = static_cast<CSceneComponent*>(m_pUp.get())->GetBoneWorldPos();
			Vector3 vPosDown = static_cast<CSceneComponent*>(m_pDown.get())->GetBoneWorldPos();

			size_t iSize = m_vecTrailVtx.size();

			for (size_t i = 0; i < iSize / 2; ++i)
			{
				int iIdx1 = static_cast<int>(i * 2);
				int iIdx2 = static_cast<int>(i * 2 + 1);
				m_vecTrailVtx[iIdx1].vPos = vPosUp;
				m_vecTrailVtx[iIdx2].vPos = vPosDown;
			}
		}
			break;
		}


	}
}
void CStaffPlayer::EndTrail()
{
	if (m_pTrailObj)
	{
		switch (m_eSkillType)
		{
		case SKILL_TYPE::_2:
			GET_SINGLE(CSoundManager)->Play("Sorcerer_LightOfRuin_Loop");
			m_pTrailObj->SetEnable(false);
			break;
		case SKILL_TYPE::_3:
			m_pTrailObj->SetEnable(false);
			break;
		}

	}
}
void CStaffPlayer::Skill_11(float fTime)
{
	switch (m_eSkillType)
	{
	case SKILL_TYPE::_2:
		SkillBoom(fTime);
		break;
	case SKILL_TYPE::_3:
		SkillIce(fTime);
		break;
	}
}
void CStaffPlayer::SkillBoom(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Sorcerer_LightOfRuin_Explosion");
	m_pCoolTime[static_cast<int>(SKILL_TYPE::_2)] += m_pCoolTimeMax[static_cast<int>(SKILL_TYPE::_2)];
	m_pFire->SetEnable(false);
	std::shared_ptr<CGameObject> pObj = m_pScene->CreateObject<CParticle3D>("FireExplosion", m_pScene->FindLayer(PARTICLE_LAYER));

	std::shared_ptr<CComponent> pCom = pObj->CreateComponent<CParticle>("FireEx1");

	pObj->SetRootComponent(pCom);

	static_cast<CParticle3D*>(pObj.get())->SetLimitTime(1.f);

	static_cast<CParticle*>(pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxScale(Vector3(100.f, 100.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinScale(Vector3(100.f, 100.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxTime(1.f);
	static_cast<CParticle*>(pCom.get())->SetMinTime(1.f);
	static_cast<CParticle*>(pCom.get())->SetMaxSpawnTime(2.f);
	static_cast<CParticle*>(pCom.get())->SetShader("ParticleBlackClip");
	static_cast<CParticle*>(pCom.get())->SetCountX(6);
	static_cast<CParticle*>(pCom.get())->SetCountY(6);
	static_cast<CParticle*>(pCom.get())->SetMaxFrame(36);
	static_cast<CParticle*>(pCom.get())->SetImageSize(Vector2(1024.f, 1024.f));
	static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_explosion_002_TEX_KJS");

	std::shared_ptr<CComponent> _pCom = pObj->CreateComponent<CParticle>("FireEx2");

	static_cast<CParticle*>(pCom.get())->AddChild(_pCom);

	static_cast<CParticle*>(_pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMaxScale(Vector3(100.f, 100.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMinScale(Vector3(100.f, 100.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMaxTime(1.f);
	static_cast<CParticle*>(_pCom.get())->SetMinTime(1.f);
	static_cast<CParticle*>(_pCom.get())->SetMaxSpawnTime(1.f);
	static_cast<CParticle*>(_pCom.get())->SetCountX(4);
	static_cast<CParticle*>(_pCom.get())->SetCountY(4);
	static_cast<CParticle*>(_pCom.get())->SetMaxFrame(16);
	static_cast<CParticle*>(_pCom.get())->SetImageSize(Vector2(1024.f, 1024.f));
	static_cast<CParticle*>(_pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_Explosion_Sub_08_White6");

	_pCom = pObj->CreateComponent<CParticle>("FireFlip1", m_pScene->FindLayer(SWORD_LIGHT_LAYER));

	static_cast<CParticle*>(pCom.get())->AddChild(_pCom);

	static_cast<CParticle*>(_pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMaxScale(Vector3(10.f, 10.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMinScale(Vector3(10.f, 10.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMaxTime(1.f);
	static_cast<CParticle*>(_pCom.get())->SetMinTime(1.f);
	static_cast<CParticle*>(_pCom.get())->SetMaxSpawnTime(0.f);
	static_cast<CParticle*>(_pCom.get())->SetSpawnCount(10000);
	static_cast<CParticle*>(_pCom.get())->SetCountX(2);
	static_cast<CParticle*>(_pCom.get())->SetCountY(2);
	static_cast<CParticle*>(_pCom.get())->SetMaxFrame(-4);
	static_cast<CParticle*>(_pCom.get())->SetImageSize(Vector2(512.f, 512.f));
	static_cast<CParticle*>(_pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_FlameFlip_002_TEX_HKB");
	static_cast<CParticle*>(_pCom.get())->SetMove(true);
	static_cast<CParticle*>(_pCom.get())->SetMaxSpeed(200.f);
	static_cast<CParticle*>(_pCom.get())->SetMinSpeed(200.f);
	static_cast<CParticle*>(_pCom.get())->SetShader("ParticleBlackClip");

	_pCom = pObj->CreateComponent<CParticle>("FireFlip2", m_pScene->FindLayer(SWORD_LIGHT_LAYER));

	static_cast<CParticle*>(pCom.get())->AddChild(_pCom);

	static_cast<CParticle*>(_pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMaxScale(Vector3(10.f, 10.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMinScale(Vector3(10.f, 10.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMaxTime(0.3f);
	static_cast<CParticle*>(_pCom.get())->SetMinTime(0.3f);
	static_cast<CParticle*>(_pCom.get())->SetMaxSpawnTime(0.f);
	static_cast<CParticle*>(_pCom.get())->SetSpawnCount(10000);
	static_cast<CParticle*>(_pCom.get())->SetCountX(4);
	static_cast<CParticle*>(_pCom.get())->SetCountY(4);
	static_cast<CParticle*>(_pCom.get())->SetMaxFrame(16);
	static_cast<CParticle*>(_pCom.get())->SetImageSize(Vector2(512.f, 512.f));
	static_cast<CParticle*>(_pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_FlameFlip_004_TEX_HKB");
	static_cast<CParticle*>(_pCom.get())->SetMove(true);
	static_cast<CParticle*>(_pCom.get())->SetMaxSpeed(100.f);
	static_cast<CParticle*>(_pCom.get())->SetMinSpeed(100.f);
	static_cast<CParticle*>(_pCom.get())->SetShader("ParticleBlackClip");

	_pCom = pObj->CreateComponent<CLight>("BlueLight");

	static_cast<CParticle*>(pCom.get())->AddChild(_pCom);

	static_cast<CLight*>(_pCom.get())->SetLightType(LIGHT_TYPE::POINT);
	static_cast<CLight*>(_pCom.get())->SetRange(256.f);
	static_cast<CLight*>(_pCom.get())->SetDifColor(1.f, 0.f, 0.f, 1.f);
	static_cast<CLight*>(_pCom.get())->SetAmbColor(1.f, 0.f, 0.f, 1.f);
	static_cast<CLight*>(_pCom.get())->SetSpcColor(1.f, 0.f, 0.f, 1.f);
	static_cast<CLight*>(_pCom.get())->SetEmiColor(1.f, 0.f, 0.f, 1.f);

	if (m_pTarget)
	{
		pObj->SetWorldPos(m_pTarget->GetWorldPos() + Vector3(0.f, 50.f, 0.f));
		static_cast<CMonster3D*>(m_pTarget)->CalcDamage(
			static_cast<int>(m_iAttackMax * 1.5f),
			static_cast<int>(m_iAttackMin * 1.5f));
	}
	else
	{
		pObj->SetWorldPos(GetWorldPos() + Vector3(0.f, 50.f, 0.f));
	}

	m_bAttack = true;
}
void CStaffPlayer::SkillIce(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Awaken_Sorcerer_IceSpear_Debris_01");
	GET_SINGLE(CSoundManager)->Play("Awaken_Sorcerer_IceSpear_Debris_02");
	GET_SINGLE(CSoundManager)->Play("Awaken_Sorcerer_IceSpear_Debris_03");
	GET_SINGLE(CSoundManager)->Play("Awaken_Sorcerer_IceSpear_Debris_04");
	GET_SINGLE(CSoundManager)->Play("Awaken_Sorcerer_IceSpear_Debris_05");
	GET_SINGLE(CSoundManager)->Play("Awaken_Sorcerer_IceSpear_Shot_01");

	m_pFire->SetEnable(false);
	m_bAttack = true;
	m_pCoolTime[static_cast<int>(SKILL_TYPE::_3)] += m_pCoolTimeMax[static_cast<int>(SKILL_TYPE::_3)];

	std::shared_ptr<CGameObject> pIceDecalObj = m_pScene->CreateObject<CGameObject>("IceDecal", m_pScene->FindLayer(DECAL_LAYER));

	std::shared_ptr<CComponent> pIceDecal = pIceDecalObj->CreateComponent<CDecal>("IceDecal");

	pIceDecalObj->SetRootComponent(pIceDecal);

	static_cast<CDecal*>(pIceDecal.get())->SetAlpha(1.f);
	static_cast<CDecal*>(pIceDecal.get())->SetWorldPos(GetWorldPos());
	static_cast<CDecal*>(pIceDecal.get())->SetWorldScale(500.f, 500.f, 500.f);
	static_cast<CDecal*>(pIceDecal.get())->SetShader("DecalAlphaMask");
	static_cast<CDecal*>(pIceDecal.get())->SetTexture(LINK_TYPE::LT_DIF,
		"FX_IceFloor_003_Tex_PHS");
	static_cast<CDecal*>(pIceDecal.get())->SetTexture(LINK_TYPE::LT_NOR,
		"FX_IceWhipDecal_NRM_001_Tex_PHS");
	static_cast<CDecal*>(pIceDecal.get())->SetTexture(LINK_TYPE::LT_EMI,
		"FX_IceWhipDecal_001_Tex_PHS");
	static_cast<CDecal*>(pIceDecal.get())->SetWorldRotY(GetWorldRot().y);
	static_cast<CDecal*>(pIceDecal.get())->AddWorldPos(static_cast<CDecal*>(pIceDecal.get())->GetWorldAxis(AXIS_X) * 250.f);
	static_cast<CDecal*>(pIceDecal.get())->AddWorldRotY(90.f);
	static_cast<CDecal*>(pIceDecal.get())->SetFadeIn(0.01f);
	static_cast<CDecal*>(pIceDecal.get())->SetDuration(3.f);
	static_cast<CDecal*>(pIceDecal.get())->SetFadeOut(1.f);
	std::shared_ptr<CComponent> pIceFloorPaper = pIceDecalObj->CreateComponent<CPaperBurn>("IceFloorResolve");
	static_cast<CDecal*>(pIceDecal.get())->AddObjComponent(pIceFloorPaper);

	static_cast<CPaperBurn*>(pIceFloorPaper.get())->SetFilter(2.f);
	static_cast<CPaperBurn*>(pIceFloorPaper.get())->SetBurnTexture("FX_WaveTile_007_TEX_KKJ");
	static_cast<CPaperBurn*>(pIceFloorPaper.get())->SetColor(0.f, 56.f / 255.f, 1.f);
	static_cast<CPaperBurn*>(pIceFloorPaper.get())->SetInColor(109.f / 255.f, 177.f / 255.f, 219.f / 255.f);


	std::shared_ptr<CComponent> pWave = pIceDecalObj->CreateComponent<CSceneComponent>("IceWave1",
		m_pScene->FindLayer(ALBEDO_LAYER));

	static_cast<CSceneComponent*>(pIceDecal.get())->AddChild(pWave);

	static_cast<CSceneComponent*>(pWave.get())->SetMesh("IceWave1");
	static_cast<CSceneComponent*>(pWave.get())->AddRelativePos(50.f, 0.f, -50.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldPosY(0.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldScale(3.f, 3.f, 3.f);

	static_cast<CSceneComponent*>(pWave.get())->AddObjComponent(pIceFloorPaper);

	pWave = pIceDecalObj->CreateComponent<CSceneComponent>("IceWave2", m_pScene->FindLayer(ALBEDO_LAYER));

	static_cast<CSceneComponent*>(pIceDecal.get())->AddChild(pWave);

	static_cast<CSceneComponent*>(pWave.get())->SetMesh("IceWave2");
	static_cast<CSceneComponent*>(pWave.get())->AddRelativePos(50.f, 0.f, 50.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldPosY(0.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldScale(10.f, 10.f, 10.f);
	static_cast<CSceneComponent*>(pWave.get())->AddObjComponent(pIceFloorPaper);

	pWave = pIceDecalObj->CreateComponent<CSceneComponent>("IceWave3", m_pScene->FindLayer(ALBEDO_LAYER));

	static_cast<CSceneComponent*>(pIceDecal.get())->AddChild(pWave);

	static_cast<CSceneComponent*>(pWave.get())->SetMesh("IceWave3");
	static_cast<CSceneComponent*>(pWave.get())->AddRelativePos(-50.f, 0.f, 50.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldPosY(0.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldScale(10.f, 10.f, 10.f);
	static_cast<CSceneComponent*>(pWave.get())->AddObjComponent(pIceFloorPaper);

	pWave = pIceDecalObj->CreateComponent<CSceneComponent>("IceWave4", m_pScene->FindLayer(ALBEDO_LAYER));

	static_cast<CSceneComponent*>(pIceDecal.get())->AddChild(pWave);

	static_cast<CSceneComponent*>(pWave.get())->SetMesh("IceWave4");
	static_cast<CSceneComponent*>(pWave.get())->AddRelativePos(-50.f, 0.f, -50.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldPosY(0.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldScale(10.f, 10.f, 10.f);
	static_cast<CSceneComponent*>(pWave.get())->AddObjComponent(pIceFloorPaper);

	pWave = pIceDecalObj->CreateComponent<CSceneComponent>("IceWave5", m_pScene->FindLayer(ALBEDO_LAYER));

	static_cast<CSceneComponent*>(pIceDecal.get())->AddChild(pWave);

	static_cast<CSceneComponent*>(pWave.get())->SetMesh("IceWave5");
	static_cast<CSceneComponent*>(pWave.get())->AddRelativePos(-50.f, 0.f, -100.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldPosY(0.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldScale(10.f, 10.f, 10.f);
	static_cast<CSceneComponent*>(pWave.get())->AddObjComponent(pIceFloorPaper);

	pWave = pIceDecalObj->CreateComponent<CSceneComponent>("IceWave6", m_pScene->FindLayer(ALBEDO_LAYER));

	static_cast<CSceneComponent*>(pIceDecal.get())->AddChild(pWave);

	static_cast<CSceneComponent*>(pWave.get())->SetMesh("IceWave6");
	static_cast<CSceneComponent*>(pWave.get())->AddRelativePos(-50.f, 0.f, 100.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldPosY(0.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldScale(10.f, 10.f, 10.f);
	static_cast<CSceneComponent*>(pWave.get())->AddObjComponent(pIceFloorPaper);

	pWave = pIceDecalObj->CreateComponent<CSceneComponent>("IceWave7", m_pScene->FindLayer(ALBEDO_LAYER));

	static_cast<CSceneComponent*>(pIceDecal.get())->AddChild(pWave);

	static_cast<CSceneComponent*>(pWave.get())->SetMesh("IceWave7");
	static_cast<CSceneComponent*>(pWave.get())->AddRelativePos(-100.f, 0.f, 50.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldPosY(0.f);
	static_cast<CSceneComponent*>(pWave.get())->SetWorldScale(10.f, 10.f, 10.f);
	static_cast<CSceneComponent*>(pWave.get())->AddObjComponent(pIceFloorPaper);

	std::shared_ptr<CGameObject> pIce = m_pScene->CreateCloneObj("IceSmoke", "IceSmoke", m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

	pIce->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_X) * 250.f + Vector3(0.f, 20.f, 0.f));

	std::shared_ptr<CGameObject> pCrackDecalObj = m_pScene->CreateObject<CGameObject>("CrackDecal", m_pScene->FindLayer(DECAL_LAYER));

	std::shared_ptr<CComponent> pCrack = pCrackDecalObj->CreateComponent<CDecal>("CrackDecal");

	pCrackDecalObj->SetRootComponent(pCrack);

	static_cast<CDecal*>(pCrack.get())->SetAlpha(1.f);
	static_cast<CDecal*>(pCrack.get())->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_X) * 20.f);
	
	static_cast<CDecal*>(pCrack.get())->SetWorldScale(20.f, 20.f, 20.f);
	static_cast<CDecal*>(pCrack.get())->SetTexture(LINK_TYPE::LT_DIF,
		"FX_Crack_001_TEX_HKB");	
	static_cast<CDecal*>(pCrack.get())->SetFadeIn(0.01f);
	static_cast<CDecal*>(pCrack.get())->SetDuration(1.f);
	static_cast<CDecal*>(pCrack.get())->SetFadeOut(1.f);
	std::shared_ptr<CMaterial> pMtrl = static_cast<CDecal*>(pCrack.get())->GetMaterial();
	if (pMtrl)
	{
		pMtrl->SetDiffuseColor(0.f, 123.f / 255.f, 1.f, 1.f);
		pMtrl->SetAmbientColor(0.f, 180.f / 255.f, 1.f, 1.f);
	}
	
	/*std::shared_ptr<CGameObject> pParticleObj = m_pScene->CreateObject<CParticle3D>("IcePlane", m_pScene->FindLayer(PARTICLE_LAYER));

	static_cast<CParticle3D*>(pParticleObj.get())->SetLimitTime(1.f);

	std::shared_ptr<CComponent> pCom = pParticleObj->CreateComponent<CParticle>("IcePlane");

	pParticleObj->SetRootComponent(pCom);

	static_cast<CParticle*>(pCom.get())->SetShader("AlphaParticle");
	static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_EMI, "FX_Wave_001_TEX_PHS");
	pMtrl = static_cast<CParticle*>(pCom.get())->GetMaterial();

	if (pMtrl)
	{
		pMtrl->SetDiffuseColor(0.f, 160.f / 255.f, 1.f, 1.f);
	}
	static_cast<CParticle*>(pCom.get())->SetMaxScale(Vector3(100.f, 100.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinScale(Vector3(100.f, 100.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxTime(1.f);
	static_cast<CParticle*>(pCom.get())->SetMinTime(1.f);
	static_cast<CParticle*>(pCom.get())->SetMaxSpawnTime(1.f);
	static_cast<CParticle*>(pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 0.f));
	static_cast<CParticle*>(pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMove(true);
	static_cast<CParticle*>(pCom.get())->SetMaxSpeed(50.f);
	static_cast<CParticle*>(pCom.get())->SetMinSpeed(50.f);
	static_cast<CParticle*>(pCom.get())->SetAccel(Vector3(0.f, 100.f, 0.f));

	pParticleObj->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_X) * 250.f);*/

	m_pIceCollider->SetEnable(true);
	static_cast<CSceneComponent*>(m_pIceCollider.get())->SetWorldPos(GetWorldPos());
}
void CStaffPlayer::SetLayer(CLayer* pLayer)
{
	CPlayer3D::SetLayer(pLayer);

	if (m_pTrailObj)
	{
		pLayer->AddObj(m_pTrailObj);
	}
}
void CStaffPlayer::AttackStart()
{
	if (m_bAttack)
		return;

	if (m_iHP <= 0)
		return;

	GET_SINGLE(CSoundManager)->Play("Sorcerer_FireBall_Loop");
	std::shared_ptr<CGameObject> pBullet = m_pScene->CreateCloneObj("Bullet", "Bullet", m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

	m_pCoolTime[static_cast<int>(SKILL_TYPE::R)] += m_pCoolTimeMax[static_cast<int>(SKILL_TYPE::R)];

	if (pBullet)
	{
		std::shared_ptr<CComponent> pBulletRoot = pBullet->GetRootComponent();

		std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pBulletRoot.get())->GetTransform();

		pTransform->AddQuaternionRotY(RadToDeg(m_fRot) - 90.f);
		pTransform->AddQuaternionRotX(m_fAngle);

		SetWorldRotY(RadToDeg(m_fRot) - 180.f);

		std::shared_ptr<CComponent> pRoot = GetRootComponent();

		if (pRoot)
		{
			std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pRoot.get())->GetTransform();

			if (pTransform)
			{
				Matrix matBone = pTransform->GetMatBone();

				Vector3 vBonePos = {};
				memcpy_s(&vBonePos, 12, &matBone[3][0], 12);
				pBullet->SetWorldPos(GetWorldPos() + vBonePos);
			}
		}
	}
	m_bAttack = true;
}
