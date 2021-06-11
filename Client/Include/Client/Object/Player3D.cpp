#include "Player3D.h"
#include "Component/Mesh2DComponent.h"
#include "Component/Camera.h"
#include "Object/GameObject.h"
#include "Component/Renderer.h"
#include "CameraManager.h"
#include "Input.h"
#include "InputObj.h"
#include "Resource/ResourceManager.h"
#include "Resource/ShaderManager.h"
#include "Resource/GraphicShader.h"
#include "RenderManager.h"
#include "GeometryGenerator.h"
#include "Resource/Material.h"
#include "Component/Paperburn.h"
#include "Component/Transform.h"
#include "../Object/UI.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"
#include "Component/Terrain.h"
#include "Component/ColliderSphere.h"
#include "Component/Arm.h"
#include "Component/PaperBurn.h"
#include "UI/UFont.h"
#include "Object/MouseObj.h"
#include "Component/Collider.h"
#include "UI/ImGui/ImGui.h"
#include "Ride.h"
#include "Tree.h"
#include "Monster3D.h"
#include "GoblinRock.h"
#include "Component/Particle.h"
#include "Component/PaperBurn.h"
#include "Particle3D.h"
#include "Component/Decal.h"
#include "Component/ColliderRay.h"
#include "UI/Bar.h"
#include "UI/UIImage.h"
#include "Component/Light.h"
#include "LightManager.h"
#include "Sound/SoundManager.h"
#include "../GameMode/Town.h"

CPlayer3D::CPlayer3D()	:
	m_pTerrain(nullptr)
	, m_fRot(0.f)
	, m_pUpperAni(nullptr)
	, m_fX(0.f)
	, m_fZ(0.f)
	, m_fDist(100.f)
	, m_bAttack(false)
	, m_fSpeed(0.f)
	, m_bAir(false)
	, m_iHP(500)
	, m_iHPMax(500)
	, m_fRunTime(0.f)
	, m_fAngle(0.f)
	, m_bAttacked(false)
	, m_fDeadTime(0.f)
	, m_pCoolTime()
	, m_pCoolTimeMax()
	, m_iExp(0)
	, m_iExpMax(1000)
	, m_fHeight(0.f)
	, m_fY(0.f)
	, m_iAttackMax(300)
	, m_iAttackMin(300)
	, m_iArmorMax(20)
	, m_iArmorMin(10)
	, m_iGold(0)
	, m_bEquipWindowOpened(false)
	, m_pTarget(nullptr)
	, m_fDestDist(100.f)
	, m_fCastMax(1.f)
	, m_fCastTime(0.f)
	, m_bCast(false)
	, m_iLevel(1)
	, m_eSkillType(SKILL_TYPE::NONE)
	, m_fMoveSpeed(100.f)
	, m_bSequenceMode(false)
{
	m_pCoolTimeMax[static_cast<int>(SKILL_TYPE::R)] = 3.f;
	m_pCoolTimeMax[static_cast<int>(SKILL_TYPE::_1)] = 3.2f;
	m_pCoolTimeMax[static_cast<int>(SKILL_TYPE::_2)] = 2.5f;
	m_pCoolTimeMax[static_cast<int>(SKILL_TYPE::_3)] = 2.5f;
	m_iObjType = static_cast<int>(OBJECT_CLASS_TYPE::OCT_PLAYER);
}

CPlayer3D::CPlayer3D(const CPlayer3D& obj) :
	CGameObject(obj)
{
	*this = obj;
}

CPlayer3D::~CPlayer3D()
{
	SAFE_RELEASE(m_pUpperAni);
}

void CPlayer3D::SetTarget(CGameObject* pObj)
{
	m_pTarget = pObj;

	if (m_pTarget)
	{
		m_pTargetParticle->SetEnable(true);
		m_pTargetParticle->SetWorldPos(m_pTarget->GetWorldPos());
		m_pTargetParticle->AddWorldPosY(
			static_cast<CSceneComponent*>(
				m_pTarget->GetRootComponent().get()
				)->GetMeshSize().y / 2.f);
	}
}

bool CPlayer3D::Init()
{
	SetGravity(true);
	GET_SINGLE(CResourceManager)->LoadTexture("32_UI2_Waypoint_Manager_Start_icon", L"UI_Share_Etc\\Texture2D\\32_UI2_Waypoint_Manager_Start_icon.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_Target_001_TEX_KKJ", L"FX_Resource_KKJ_04\\Texture2D\\FX_Target_001_TEX_KKJ.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_SelectRing_001_TEX_KKJ", L"FX_Resource_KKJ_04\\Texture2D\\FX_SelectRing_001_TEX_KKJ.dds");

	GET_SINGLE(CSoundManager)->LoadSound("Effect", "PCStep_Rock_RL_00", false,
		"PCStep_Rock_RL_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "PCStep_Rock_RL_01", false,
		"PCStep_Rock_RL_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "PCStep_Rock_RL_02", false,
		"PCStep_Rock_RL_02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "PCStep_Rock_RL_03", false,
		"PCStep_Rock_RL_03.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Moving_Grass_Jump_00", false,
		"Moving_Grass_Jump_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Moving_Grass_Jump_01", false,
		"Moving_Grass_Jump_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Moving_Grass_Jump_02", false,
		"Moving_Grass_Jump_02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Moving_Grass_Land_00", false,
		"Moving_Grass_Land_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Moving_Grass_Land_01", false,
		"Moving_Grass_Land_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Moving_Grass_Land_02", false,
		"Moving_Grass_Land_02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Damage_Blood_01", false,
		"Damage_Blood_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Damage_Blood_02", false,
		"Damage_Blood_02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Damage_Blood_03", false,
		"Damage_Blood_03.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "PCStep_Grass_RL_00", false,
		"PCStep_Grass_RL_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "PCStep_Grass_RL_01", false,
		"PCStep_Grass_RL_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "PCStep_Grass_RL_02", false,
		"PCStep_Grass_RL_02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "PCStep_Grass_RL_03", false,
		"PCStep_Grass_RL_03.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Window_InventoryClose", false,
		"Button\\Window_InventoryClose.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Window_InventoryOpen", false,
		"Button\\Window_InventoryOpen.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Window_QuestClose", false,
		"Button\\Window_QuestClose.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Window_QuestOpen", false,
		"Button\\Window_QuestOpen.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Window_UIOpen", false,
		"Button\\Window_UIOpen.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Window_UIClose", false,
		"Button\\Window_UIClose.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "ArmorHit_Stone_M_00", false,
		"ArmorHit_Stone_M_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "ArmorHit_Stone_M_01", false,
		"ArmorHit_Stone_M_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "ArmorHit_Stone_M_02", false,
		"ArmorHit_Stone_M_02.ogg");

	if (!CGameObject::Init())
		return false;

	SetShadowAll(true);
	GET_SINGLE(CInput)->AddAxisKey("R", DIK_R, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("F", DIK_F, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("T", DIK_T, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("G", DIK_G, -1.f);

	GET_SINGLE(CInput)->AddAxisKey("W", DIK_W, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("S", DIK_S, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("A", DIK_A, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("D", DIK_D, -1.f);
	GET_SINGLE(CInput)->AddActionKey("1", DIK_1);
	GET_SINGLE(CInput)->AddActionKey("2", DIK_2);
	GET_SINGLE(CInput)->AddActionKey("3", DIK_3);
	//GET_SINGLE(CInput)->AddAxisKey("RotY", DIK_2, 1.f);
	//GET_SINGLE(CInput)->AddAxisKey("RotY", DIK_3, -1.f);
	GET_SINGLE(CInput)->AddActionKey("Q", DIK_Q);
	GET_SINGLE(CInput)->AddActionKey("E", DIK_E);
	GET_SINGLE(CInput)->AddActionKey("I", DIK_I);
	GET_SINGLE(CInput)->AddActionKey("T", DIK_T);
	GET_SINGLE(CInput)->AddActionKey("Paper", DIK_C);
	GET_SINGLE(CInput)->AddAxisKey("Space", DIK_SPACE, 1.f);

	m_pInput->AddBindAxis("R", this, &CPlayer3D::BoneRotX);
	m_pInput->AddBindAxis("F", this, &CPlayer3D::BoneRotX);
	m_pInput->AddBindAxis("T", this, &CPlayer3D::BoneRotY);
	m_pInput->AddBindAxis("G", this, &CPlayer3D::BoneRotY);

	m_pInput->AddBindAxis("A", this, &CPlayer3D::CameraMoveZ);
	m_pInput->AddBindAxis("D", this, &CPlayer3D::CameraMoveZ);
	m_pInput->AddBindAxis("W", this, &CPlayer3D::CameraMoveX);
	m_pInput->AddBindAxis("S", this, &CPlayer3D::CameraMoveX);
	m_pInput->AddBindAction("1", this, &CPlayer3D::Skill1Down, KEY_TYPE::KT_DOWN);
	m_pInput->AddBindAction("1", this, &CPlayer3D::Skill1, KEY_TYPE::KT_UP);
	m_pInput->AddBindAction("2", this, &CPlayer3D::Skill2, KEY_TYPE::KT_UP);
	m_pInput->AddBindAction("3", this, &CPlayer3D::Skill3, KEY_TYPE::KT_UP);
	m_pInput->AddBindAction("E", this, &CPlayer3D::Equip, KEY_TYPE::KT_UP);
	m_pInput->AddBindAction("Q", this, &CPlayer3D::Quest, KEY_TYPE::KT_UP);
	m_pInput->AddBindAction("I", this, &CPlayer3D::Inventory, KEY_TYPE::KT_UP);
	m_pInput->AddBindAction("T", this, &CPlayer3D::SpawnRide, KEY_TYPE::KT_UP);
	m_pInput->AddBindAxis("Space", this, &CPlayer3D::Space);

	m_pCam = CreateComponent<CCamera>("Player Camera");
	static_cast<CCamera*>(m_pCam.get())->SetCameraType(CAMERA_TYPE::CT_3D);
	static_cast<CCamera*>(m_pCam.get())->AddRelativePos(25.f,40.f, 0.f);
	static_cast<CCamera*>(m_pCam.get())->AddWorldRotX(45.f);
	static_cast<CCamera*>(m_pCam.get())->AddWorldRotY(-90.f);
	static_cast<CCamera*>(m_pCam.get())->SetTarget(static_cast<CSceneComponent*>(m_pMesh.get()));

	GET_SINGLE(CCameraManager)->AddCam(static_cast<CCamera*>(m_pCam.get()));
	GET_SINGLE(CCameraManager)->SetMainCam("Player Camera");

	static_cast<CSceneComponent*>(m_pMesh.get())->AddChild(m_pCam);

	std::shared_ptr<CComponent> pCollider = CreateComponent<CColliderSphere>("PlayerBody", m_pScene->FindLayer(COLLIDER_LAYER));

	static_cast<CSceneComponent*>(m_pMesh.get())->AddChild(pCollider);

	CColliderSphere* _pCollider = static_cast<CColliderSphere*>(pCollider.get());
	_pCollider->SetWorldScale(25.f, 25.f, 25.f);
	_pCollider->SetRadius(12.f);
	_pCollider->SetRelativePosY(25.f);
	_pCollider->SetCallBack<CPlayer3D>(COLLISION_STATE::CS_INIT, this, &CPlayer3D::ColInit);
	_pCollider->SetCallBack<CPlayer3D>(COLLISION_STATE::CS_STAY, this, &CPlayer3D::ColStay);
	_pCollider->SetCallBack<CPlayer3D>(COLLISION_STATE::CS_LAST, this, &CPlayer3D::ColLast);

	std::shared_ptr<CComponent> pMinimapCam = CreateComponent<CCamera>("Minimap");
	static_cast<CCamera*>(pMinimapCam.get())->SetCameraType(CAMERA_TYPE::CT_3D);
	static_cast<CCamera*>(pMinimapCam.get())->AddRelativePos(0.f, 2000.f, 0.f);
	static_cast<CCamera*>(pMinimapCam.get())->SetWorldRotY(90.f);
	static_cast<CCamera*>(pMinimapCam.get())->SetWorldRotX(90.f);
	//static_cast<CCamera*>(pMinimapCam.get())->SetTarget(static_cast<CSceneComponent*>(m_pMesh.get()));

	GET_SINGLE(CCameraManager)->AddCam(static_cast<CCamera*>(pMinimapCam.get()));

	m_pEquipCam = CreateComponent<CCamera>("EquipCam");
	static_cast<CCamera*>(m_pEquipCam.get())->SetCameraType(CAMERA_TYPE::CT_3D);
	static_cast<CCamera*>(m_pEquipCam.get())->AddRelativePos(50.f, 0.f, 0.f);
	static_cast<CCamera*>(m_pEquipCam.get())->SetWorldRotY(-90.f);
	static_cast<CCamera*>(m_pEquipCam.get())->SetTarget(static_cast<CSceneComponent*>(m_pMesh.get()));

	GET_SINGLE(CCameraManager)->AddCam(static_cast<CCamera*>(m_pEquipCam.get()));

	m_pPaper = CreateComponent<CPaperBurn>("Paper");

	static_cast<CSceneComponent*>(m_pMesh.get())->AddObjComponent(m_pPaper);

	//_pUpper->AddChild(pMinimapCam);

	static_cast<CSceneComponent*>(m_pMesh.get())->AddChild(m_pEquipCam);

	//SetUIEnableAll(true);

	GET_SINGLE(CResourceManager)->LoadTexture("T_UI_BaseA", L"UI_Base\\Texture2D\\T_UI_BaseA.dds");

	

	m_pTargetParticle = m_pScene->CreateObject<CGameObject>("TargetParticle",m_pScene->FindLayer(PARTICLE_LAYER));
	
	std::shared_ptr<CComponent> pCom = m_pTargetParticle->CreateComponent<CUIImage>("TargetArrow");

	m_pTargetParticle->SetRootComponent(pCom);

	static_cast<CUIImage*>(pCom.get())->SetRelativePosY(15.f);
	static_cast<CUIImage*>(pCom.get())->AddRenderState(NO_DEPTH);
	static_cast<CUIImage*>(pCom.get())->SetShader("AlphaTextureUI");
	static_cast<CUIImage*>(pCom.get())->SetTexture("FX_Target_001_TEX_KKJ");
	static_cast<CUIImage*>(pCom.get())->SetSize(Vector2(512.f,512.f));
	static_cast<CUIImage*>(pCom.get())->SetUV(Vector2(0.f, 0.f), Vector2(512.f, 512.f));
	static_cast<CUIImage*>(pCom.get())->SetWorldScale(30.f, 30.f, 1.f);
	static_cast<CUIImage*>(pCom.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	std::shared_ptr<CMaterial> pMtrl = static_cast<CUIImage*>(pCom.get())->GetMaterial();
	if (pMtrl)
	{
		pMtrl->SetDiffuseColor(144.f / 255.f, 165.f / 255.f, 1.f, 1.f);
	}

	std::shared_ptr<CComponent> _pCom = m_pTargetParticle->CreateComponent<CUIImage>("TargetArrow", m_pScene->FindLayer(SWORD_LIGHT_LAYER));

	static_cast<CSceneComponent*>(pCom.get())->AddChild(_pCom);

	static_cast<CUIImage*>(_pCom.get())->AddRenderState(NO_DEPTH);
	static_cast<CUIImage*>(_pCom.get())->SetShader("AlphaTextureUI");
	static_cast<CUIImage*>(_pCom.get())->SetTexture("FX_Target_001_TEX_KKJ");
	static_cast<CUIImage*>(_pCom.get())->SetSize(Vector2(512.f, 512.f));
	static_cast<CUIImage*>(_pCom.get())->SetUV(Vector2(0.f, 0.f), Vector2(512.f, 512.f));
	static_cast<CUIImage*>(_pCom.get())->SetWorldScale(30.f, 30.f, 1.f);
	static_cast<CUIImage*>(_pCom.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(_pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	pMtrl = static_cast<CUIImage*>(_pCom.get())->GetMaterial();
	if (pMtrl)
	{
		pMtrl->SetDiffuseColor(144.f / 255.f, 165.f / 255.f, 1.f, 1.f);
	}

	_pCom = m_pTargetParticle->CreateComponent<CUIImage>("Ring", m_pScene->FindLayer(PARTICLE_LAYER));

	static_cast<CSceneComponent*>(pCom.get())->AddChild(_pCom);

	static_cast<CUIImage*>(_pCom.get())->AddRenderState(NO_DEPTH);
	static_cast<CUIImage*>(_pCom.get())->SetShader("AlphaTextureUI");
	static_cast<CUIImage*>(_pCom.get())->SetTexture("FX_SelectRing_001_TEX_KKJ");
	static_cast<CUIImage*>(_pCom.get())->SetSize(Vector2(512.f, 512.f));
	static_cast<CUIImage*>(_pCom.get())->SetUV(Vector2(0.f, 0.f), Vector2(512.f, 512.f));
	static_cast<CUIImage*>(_pCom.get())->SetWorldScale(30.f, 30.f, 1.f);
	static_cast<CUIImage*>(_pCom.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(_pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	pMtrl = static_cast<CUIImage*>(_pCom.get())->GetMaterial();
	if (pMtrl)
	{
		pMtrl->SetDiffuseColor(144.f / 255.f, 165.f / 255.f, 1.f, 1.f);
	}

	_pCom = m_pTargetParticle->CreateComponent<CUIImage>("Ring", m_pScene->FindLayer(SWORD_LIGHT_LAYER));

	static_cast<CSceneComponent*>(pCom.get())->AddChild(_pCom);

	static_cast<CUIImage*>(_pCom.get())->AddRenderState(NO_DEPTH);
	static_cast<CUIImage*>(_pCom.get())->SetShader("AlphaTextureUI");
	static_cast<CUIImage*>(_pCom.get())->SetTexture("FX_SelectRing_001_TEX_KKJ");
	static_cast<CUIImage*>(_pCom.get())->SetSize(Vector2(512.f, 512.f));
	pMtrl = static_cast<CUIImage*>(_pCom.get())->GetMaterial();
	if (pMtrl)
	{
		pMtrl->SetDiffuseColor(144.f / 255.f, 165.f / 255.f, 1.f, 1.f);
	}
	static_cast<CUIImage*>(_pCom.get())->SetUV(Vector2(0.f, 0.f), Vector2(512.f, 512.f));
	static_cast<CUIImage*>(_pCom.get())->SetWorldScale(Vector3(30.f, 30.f, 1.f));
	static_cast<CUIImage*>(_pCom.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(_pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);

	GET_SINGLE(CResourceManager)->LoadTexture("FX_Target_010_Tex_HKB", L"FX_Resource_HKB_03\\Texture2D\\FX_Target_010_Tex_HKB.dds");

	std::shared_ptr<CComponent> pDecal = m_pTargetParticle->CreateComponent<CDecal>("Decal", m_pScene->FindLayer(DECAL_LAYER));

	static_cast<CSceneComponent*>(pCom.get())->AddChild(pDecal);

	static_cast<CDecal*>(pDecal.get())->SetShader("DecalAlphaMask");
	std::shared_ptr<CMaterial> pDecalMtrl = static_cast<CDecal*>(pDecal.get())->GetMaterial();

	if (pDecalMtrl)
		pDecalMtrl->SetDiffuseColor(Vector4(0.8f, 0.8f, 1.f, 1.f));

	static_cast<CDecal*>(pDecal.get())->SetTexture(LINK_TYPE::LT_EMI, "FX_Target_010_Tex_HKB");
	static_cast<CDecal*>(pDecal.get())->SetWorldScale(50.f, 50.f, 50.f);
	static_cast<CDecal*>(pDecal.get())->SetPivot(0.5f, 0.5f, 0.5f);

	m_pCameraRay = CreateComponent<CColliderRay>("CameraRay", m_pScene->FindLayer(COLLIDER_LAYER));

	static_cast<CSceneComponent*>(m_pCam.get())->AddChild(m_pCameraRay);

	static_cast<CColliderRay*>(m_pCameraRay.get())->SetCallBack<CPlayer3D>(COLLISION_STATE::CS_INIT, this, &CPlayer3D::ColInit);
	static_cast<CColliderRay*>(m_pCameraRay.get())->SetCallBack<CPlayer3D>(COLLISION_STATE::CS_STAY, this, &CPlayer3D::ColStay);
	static_cast<CColliderRay*>(m_pCameraRay.get())->SetCallBack<CPlayer3D>(COLLISION_STATE::CS_LAST, this, &CPlayer3D::ColLast);

	m_pHPBar = CreateComponent<CBar>("HPBar");

	static_cast<CSceneComponent*>(m_pMesh.get())->AddChild(m_pHPBar);

	static_cast<CBar*>(m_pHPBar.get())->SetTexture("32_UI2_Waypoint_Manager_Start_icon");
	static_cast<CBar*>(m_pHPBar.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CBar*>(m_pHPBar.get())->SetUV(Vector2(505.f, 533.f), Vector2(729.f, 539.f));
	static_cast<CBar*>(m_pHPBar.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CBar*>(m_pHPBar.get())->SetWorldScale(224.f, 6.f, 0.f);
	static_cast<CBar*>(m_pHPBar.get())->SetInheritRotX(false);
	static_cast<CBar*>(m_pHPBar.get())->SetInheritRotY(false);
	static_cast<CBar*>(m_pHPBar.get())->SetInheritRotZ(false);

	m_pName = CreateComponent<CUFont>("NameFont");

	static_cast<CSceneComponent*>(m_pMesh.get())->AddChild(m_pName);
	 
	static_cast<CUFont*>(m_pName.get())->SetRelativePosY(GetMeshSize().y + 10.f);
	static_cast<CUFont*>(m_pName.get())->SetRect(-100.f, 0.f, 100.f, 0.f);
	static_cast<CUFont*>(m_pName.get())->SetText(L"Player");
	static_cast<CUFont*>(m_pName.get())->Set3D();
	static_cast<CUFont*>(m_pName.get())->SetShadowOffset(Vector3(2.f, -2.f, 0.f));
	static_cast<CUFont*>(m_pName.get())->SetShadowColor(0.f, 0.f, 0.f, 1.f);

	/*m_pLight = CreateComponent<CLight>("PlayerLight");

	static_cast<CSceneComponent*>(m_pMesh.get())->AddChild(m_pLight);

	static_cast<CLight*>(m_pLight.get())->SetLightType(LIGHT_TYPE::SPOT);
	static_cast<CLight*>(m_pLight.get())->SetInheritRotX(false);
	static_cast<CLight*>(m_pLight.get())->SetInheritRotY(false);
	static_cast<CLight*>(m_pLight.get())->SetInheritRotZ(false);
	static_cast<CLight*>(m_pLight.get())->SetWorldRotX(90.f);
	static_cast<CLight*>(m_pLight.get())->SetRelativePosY(150.f);*/

	return true;
}

void CPlayer3D::Start()
{
	CGameObject::Start();

	GET_SINGLE(CCameraManager)->AddCam(static_cast<CCamera*>(m_pCam.get()));
	GET_SINGLE(CCameraManager)->SetMainCam("Player Camera");

	static_cast<CCamera*>(m_pCam.get())->SetTarget(static_cast<CSceneComponent*>(m_pMesh.get()));

	CTerrain* pTerrain = CTerrain::GetMainTerrain();
	
	SetTerrain(pTerrain);	

	if (!m_pRide)
	{
		CLayer *pLayer = m_pScene->FindLayer(ALPHA_LAYER);
		
		m_pRide = pLayer->FindObj("BoneDragon");
	}

	
}

void CPlayer3D::Update(float fTime)
{
	for (int i = 0; i < static_cast<int>(SKILL_TYPE::END); i++)
	{
		m_pCoolTime[i] -= fTime;

		if (m_pCoolTime[i] < 0.f)
			m_pCoolTime[i] = 0.f;
	}


	Vector3 vPos = {};
	std::shared_ptr<CComponent> pRoot = GetRootComponent(); 
	if (pRoot)
	{
		std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pRoot.get())->GetTransform();

		if (pTransform)
		{
			vPos = pTransform->GetBoneWorldPos();
		}
	}


	if (!m_bSequenceMode)
	{
		if (GET_SINGLE(CInput)->IsPress(MOUSE_TYPE::RIGHT))
		{
			Vector2 vMouseMove = GET_SINGLE(CInput)->GetMouseMove();

			m_fRot += vMouseMove.x * fTime;
			m_fAngle -= vMouseMove.y * fTime * 10.f;
		}
	}

	int iWheel = GET_SINGLE(CInput)->GetWheel();
	if (iWheel != 0)
	{
		m_fDestDist -= iWheel / 60.f;

	}
	CGameObject::Update(fTime);

	CAnimationSequence* pSequence = m_pUpperAni->GetSequence();

	if (pSequence)
	{
		if (pSequence->GetName() == "Upper_Down_Loop")
		{
			m_fDeadTime += fTime;
		}
	}

	if (m_bEquipWindowOpened)
	{
		Matrix mat = Matrix(XMMatrixRotationAxis(Vector3::Axis[AXIS::AXIS_Y].Convert(), m_fRot));
		Vector3 vPos = GetWorldPos() + static_cast<CCamera*>(m_pEquipCam.get())->GetTargetOffset();
		memcpy_s(&mat._41, 12, &vPos, 12);

		Vector3 vOffset = Vector3(42.f, 0.f, 0.f);

		Vector3 vCamPos = vOffset.TranslationCoor(mat);

		static_cast<CCamera*>(m_pEquipCam.get())->SetWorldPos(vCamPos);
	}
	else
	{
		if (m_pCam)
		{
			Matrix mat = Matrix(XMMatrixRotationAxis(Vector3::Axis[AXIS::AXIS_Y].Convert(), m_fRot));
			Vector3 vPos = GetWorldPos() + static_cast<CCamera*>(m_pCam.get())->GetTargetOffset();
			memcpy_s(&mat._41, 12, &vPos, 12);

			float fAngle = DegToRad(m_fAngle);
			Vector3 vOffset = Vector3(cosf(fAngle), sinf(fAngle), 0.f);

			vOffset.Normalize();

			vOffset *= m_fDist;

			Vector3 vCamPos = vOffset.TranslationCoor(mat);

			static_cast<CCamera*>(m_pCam.get())->SetWorldPos(vCamPos);
		}
	}

	if (m_iHP <= 0.f )
	{
		m_fDeadTime += fTime;

		if (m_pRide)
		{
			if (m_pRide->IsEnable())
			{
				SetWorldPos(GetBoneWorldPos());

				static_cast<CRide*>(m_pRide.get())->ExitPlayer();

				m_pRide->SetEnable(false);
			}
		}
	}

	vPos = GetBoneWorldPos() + static_cast<CCamera*>(m_pCam.get())->GetTargetOffset();

	Vector3 vCamPos = static_cast<CCamera*>(m_pCam.get())->GetWorldPos();

	Vector3 vDir = vPos - vCamPos;

	vDir.Normalize();

	static_cast<CColliderRay*>(m_pCameraRay.get())->SetDir(vDir);
	static_cast<CColliderRay*>(m_pCameraRay.get())->SetOrigin(vCamPos);

	if (m_fDestDist > m_fDist + 10.f)
	{
		m_fDist += fTime * 100.f;
	}
	else if (m_fDestDist < m_fDist -10.f)
	{
		m_fDist -= fTime * 100.f;
	}

	if (m_bCast)
	{
		m_fCastTime += fTime;

		if (m_fCastTime >= m_fCastMax)
		{
			m_bCast = false;
			m_fCastTime = m_fCastMax;
		}
	}

	if (m_pCam)
	{
		Matrix matVP = static_cast<CCamera*>(m_pCam.get())->GetVP();

		Vector3 vBarPos = GetBoneWorldPos() + Vector3(0.f, 25.f, 0.f);

		Vector3 vProjPos = vBarPos.TranslationCoor(matVP);

		float w = vPos.x * matVP[0][3] + vPos.y * matVP[1][3] + vPos.z * matVP[2][3] + matVP[3][3];

		vProjPos /= w;

		Resolution tRS = RESOLUTION;

		vProjPos *= Vector3(static_cast<float>(tRS.iWidth), static_cast<float>(tRS.iHeight), 0.f);

		static_cast<CBar*>(m_pHPBar.get())->SetWorldPos(vProjPos + Vector3(0.f, 25.f, 0.f));
		static_cast<CBar*>(m_pHPBar.get())->SetPrg(m_iHP / static_cast<float>(m_iHPMax));
	}

	if (m_pTarget)
	{
		m_pTargetParticle->SetEnable(true);
		m_pTargetParticle->SetWorldPos(m_pTarget->GetWorldPos());
		m_pTargetParticle->AddWorldPosY(
			static_cast<CSceneComponent*>(
				m_pTarget->GetRootComponent().get()
				)->GetMeshSize().y / 2.f);
		std::shared_ptr<CComponent> pRoot = m_pTargetParticle->GetRootComponent();

		std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pRoot.get())->GetTransform();

		pTransform->LookAt(static_cast<CSceneComponent*>(m_pCam.get())->GetBoneWorldPos());

		if (static_cast<CMonster3D*>(m_pTarget)->IsDie())
		{
			m_pTarget = nullptr;
		}
	}
	else
	{
		m_pTargetParticle->SetEnable(false);
	}
}

void CPlayer3D::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
	if (!m_bAir)
	{
		if (m_fX == 0.f && m_fZ == 0.f)
		{
			m_fRunTime = 0.f;

			CAnimationSequence* pSequence = m_pUpperAni->GetBlendSequence();

			if (pSequence)
			{
				std::string strName = pSequence->GetName();

				if (strName == "Player_Run_R_WP" || 
					strName == "Player_Run_FR_WP" || 
					strName == "Player_Run_F_WP" || 
					strName == "Player_Run_FL_WP" || 
					strName == "Player_Run_L_WP" || 
					strName == "Player_Run_BR_WP" || 
					strName == "Player_Run_B_WP" || 
					strName == "Player_Run_BL_WP")
				{
					m_pUpperAni->DeleteBlendSequence();//체비세퍼 부등식 ,vsm
				}
			}
		}

		else
		{
			if (m_bAttack)
			{
				switch (static_cast<int>(m_fZ))
				{
				case -1:
				{
					switch (static_cast<int>(m_fX))
					{
					case -1:
						m_pUpperAni->SetBlendSequence("Run_BR_WP");
						break;
					case 0:
						m_pUpperAni->SetBlendSequence("Run_R_WP");
						break;
					case 1:
						m_pUpperAni->SetBlendSequence("Run_FR_WP");
						break;
					}
				}
					break;
				case 0:
				{
					switch (static_cast<int>(m_fX))
					{
					case -1:
						m_pUpperAni->SetBlendSequence("Run_B_WP");
						break;
					case 1:
						m_pUpperAni->SetBlendSequence("Run_F_WP");
						break;
					}
				}
					break;
				case 1:
				{
					switch (static_cast<int>(m_fX))
					{
					case -1:
						m_pUpperAni->SetBlendSequence("Run_FL_WP");
						break;
					case 0:
						m_pUpperAni->SetBlendSequence("Run_L_WP");
						break;
					case 1:
						m_pUpperAni->SetBlendSequence("Run_BL_WP");
						break;
					}
				}
					break;
				}
			}

			m_fRunTime += fTime;

			m_fX = 0.f;
			m_fZ = 0.f;
		}
	}
	m_fY = 0.f;
}

void CPlayer3D::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CPlayer3D::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);

	if (m_pTerrain)
	{
		Vector3 vPos = GetBoneWorldPos();
		m_fHeight = vPos.y - m_pTerrain->GetHeight(vPos);

		if (0.f > m_fHeight)
		{
			Vector3 vSpeed = GetSpeed();

			if (vSpeed.y < 0.f)
			{
				SetSpeed(Vector3(0.f, 0.f, 0.f));
			}
			SetWorldPosY(vPos.y - m_fHeight);
			m_bAir = false;
		}
	}
	else
	{
		m_pTerrain = CTerrain::GetMainTerrain();
	}
}

void CPlayer3D::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CPlayer3D::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CPlayer3D::Clone()
{
	return std::shared_ptr<CGameObject>(new CPlayer3D(*this));
}

void CPlayer3D::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CPlayer3D::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	m_pMesh = GetRootComponent();

	m_pInput->AddBindAxis("A", this, &CPlayer3D::CameraMoveZ);
	m_pInput->AddBindAxis("D", this, &CPlayer3D::CameraMoveZ);
	m_pInput->AddBindAxis("W", this, &CPlayer3D::CameraMoveX);
	m_pInput->AddBindAxis("S", this, &CPlayer3D::CameraMoveX);
	m_pInput->AddBindAction("E", this, &CPlayer3D::Equip, KEY_TYPE::KT_UP);
	m_pInput->AddBindAction("I", this, &CPlayer3D::Inventory, KEY_TYPE::KT_UP);
	m_pInput->AddBindAxis("RotY", this, &CPlayer3D::CameraRotY);
	m_pInput->AddBindAxis("RotZ", this, &CPlayer3D::CameraRotZ);
	m_pInput->AddBindAxis("Space", this, &CPlayer3D::Space);

	m_pCam = FindComponentByType<CCamera>();

	m_pUpperAni = static_cast<CSceneComponent*>(m_pMesh.get())->GetAnimation();

	SetTransition();
}

void CPlayer3D::ColInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strDest = pDest->GetName();
	std::string strSrc = pSrc->GetName();

	if (strSrc == "PlayerBody")
	{
		if (strDest == "MonsterHandBody")
		{
			m_fSpeed = 0.f;

			std::shared_ptr<CGameObject> pObj = m_pScene->FindLayer(ALBEDO_LAYER)->FindObj("Cyclops");

			CMonster3D* pMonster = static_cast<CMonster3D*>(pObj.get());

			int iAttackMax = pMonster->GetAttackMax();
			int iAttackMin = pMonster->GetAttackMin();

			CalcDamage(iAttackMax, iAttackMin);

			Vector3 vCross = pDest->GetCrs();

			if (m_iHP > 0)
			{
				std::shared_ptr<CGameObject> pParticle = m_pScene->CreateCloneObj("BloodParticle", "BloodParticle",
					m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

				pParticle->SetWorldPos(vCross);

				pParticle = m_pScene->CreateCloneObj("BloodParticle2", "BloodParticle2",
					m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

				pParticle->SetWorldPos(vCross);

				int iSound = rand() % 3;
				switch (iSound)
				{
				case 0:
					GET_SINGLE(CSoundManager)->Play("Damage_Blood_01");
					break;
				case 1:
					GET_SINGLE(CSoundManager)->Play("Damage_Blood_02");
					break;
				case 2:
					GET_SINGLE(CSoundManager)->Play("Damage_Blood_03");
					break;
				}
			}
		}

		else if (strDest == "BallBody")
		{
			m_fSpeed = 250.f;
			if (!m_bAttack)
			{
				m_bAttacked = true;
			}
			if (m_iHP > 0)
			{
				std::shared_ptr<CGameObject> pFont = m_pScene->CreateCloneObj("MonsterDamageFont", "MonsterDamageFont", 
					m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

				if (pFont)
				{
					std::shared_ptr<CComponent> pCom = pFont->GetRootComponent();

					static_cast<CParticle*>(pCom.get())->SetCountX(200);
					pFont->SetWorldPos(GetWorldPos() + Vector3(0.f, 50.f, 0.f));
				}
				m_iHP -= 200;
				if (m_iHP < 0)
				{
					m_iHP = 0;
				}

				switch (rand() % 3)
				{
				case 0:
					GET_SINGLE(CSoundManager)->Play("ArmorHit_Stone_M_00");
					break;
				case 1:
					GET_SINGLE(CSoundManager)->Play("ArmorHit_Stone_M_01");
					break;
				case 2:
					GET_SINGLE(CSoundManager)->Play("ArmorHit_Stone_M_02");
					break;
				}

			}
		}

		else if (strDest == "MouseBody")
		{
			SetOutLine(true);
		}

		else if (strDest == "GoblinRockBody")
		{
			CGoblinRock* pRock = static_cast<CGoblinRock*>(pDest->GetObj());

			int iAttackMax = pRock->GetAttackMax();
			int iAttackMin = pRock->GetAttackMin();
			CalcDamage(iAttackMax, iAttackMin);

			if (m_iHP > 0)
			{
				switch (rand() % 3)
				{
				case 0:
					GET_SINGLE(CSoundManager)->Play("ArmorHit_Stone_M_00");
					break;
				case 1:
					GET_SINGLE(CSoundManager)->Play("ArmorHit_Stone_M_01");
					break;
				case 2:
					GET_SINGLE(CSoundManager)->Play("ArmorHit_Stone_M_02");
					break;
				}

			}
			CGameObject* pObj = pRock->GetOwner();

			if (pObj)
			{
				SetTarget(pObj);
			}
		}
	}
}

void CPlayer3D::ColStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strSrc = pSrc->GetName();
	
	if (strSrc == "CameraRay" && pDest->GetColType() == COLLIDER_TYPE::TERRAIN)
	{
		Vector3 vCross = pDest->GetCrs();

		Vector3 vPos = pSrc->GetWorldPos();

		Vector3 vDist = vCross - vPos;

		float fDist = vDist.Length();

		if (m_fDist > fDist)
		{
			m_fDist -= fDist;
		}
	}
}

void CPlayer3D::ColLast(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strDest = pDest->GetName();
	std::string strSrc = pSrc->GetName();

	if (strDest == "MouseBody" && strSrc == "PlayerBody")
	{
		SetOutLine(false);
	}
}

void CPlayer3D::ShowWindow()
{
	CGameObject::ShowWindow();

	if (ImGui::Begin(GetName().c_str()))
	{
		ImGui::InputFloat("AngleY", &m_fRot);
		ImGui::InputFloat("AngleX", &m_fAngle);
		ImGui::InputFloat("Dist", &m_fDist);

		ImGui::InputInt("Attack Max", &m_iAttackMax);
		ImGui::InputInt("Attack Min", &m_iAttackMin);
		ImGui::Checkbox("Attack ", &m_bAttack);
	}
	ImGui::End();
}

void CPlayer3D::CameraMoveZ(float fScale, float fTime)
{
	if (m_bSequenceMode || m_iHP <= 0.f || m_bAttacked)
		return;

	MoveZ(fScale, fTime);
}

void CPlayer3D::CameraMoveX(float fScale, float fTime)
{
	if (m_bSequenceMode || m_iHP <= 0.f || m_bAttacked)
		return;

	MoveX(fScale, fTime);
}

void CPlayer3D::CameraRotX(float fScale, float fTime)
{
	if (m_bSequenceMode)
		return;
	CSceneComponent* pMesh = static_cast<CSceneComponent*>(m_pMesh.get());

	if (pMesh)
	{
		pMesh->AddWorldRotX(fScale * fTime * -90.f);
	}
}

void CPlayer3D::CameraRotY(float fScale, float fTime)
{
	if (m_bSequenceMode)
		return;
	CSceneComponent* pMesh = static_cast<CSceneComponent*>(m_pMesh.get());

	if (pMesh)
	{
		pMesh->AddWorldRotY(fScale * fTime * -90.f);
	}
}

void CPlayer3D::CameraRotZ(float fScale, float fTime)
{
	if (m_bSequenceMode)
		return;
	CSceneComponent* pMesh = static_cast<CSceneComponent*>(m_pMesh.get());

	if (pMesh)
	{
		pMesh->AddWorldRotZ(fScale * fTime * -90.f);
	}
}

void CPlayer3D::CamRotY(float fScale, float fTime)
{
	if (m_bSequenceMode)
		return;
	if (m_pCam)
	{
		m_fRot += fScale * fTime * PI_DIV2;
		
		Matrix mat = Matrix(XMMatrixRotationAxis(Vector3::Axis[AXIS::AXIS_Y].Convert(), m_fRot));
		Vector3 vPos = GetWorldPos() + Vector3(0.f , 35.f, 0.f);
		memcpy_s(&mat._41, 12, &vPos, 12);

		float fAngle = DegToRad(m_fAngle);
		Vector3 vOffset = Vector3(cosf(fAngle), sinf(fAngle), 0.f);

		vOffset.Normalize();

		vOffset *= m_fDist;

		Vector3 vCamPos = vOffset.TranslationCoor(mat);

		static_cast<CCamera*>(m_pCam.get())->SetWorldPos(vCamPos);

	}
}


void CPlayer3D::Space(float fScale, float fTime)
{
	if (fScale != 0.f && !m_bAir &&m_iHP> 0 && !m_bAttack && !m_bSequenceMode)
	{
		if (m_pRide)
		{
			if (!m_pRide->IsEnable())
			{
				switch (rand() % 3)
				{
				case 0:
					GET_SINGLE(CSoundManager)->Play("Moving_Grass_Jump_00");
					break;
				case 1:
					GET_SINGLE(CSoundManager)->Play("Moving_Grass_Jump_01");
					break;
				case 2:
					GET_SINGLE(CSoundManager)->Play("Moving_Grass_Jump_02");
					break;
				}
				SetSpeedY(200.f);
				//m_fSpeed = 500.f;	//	500 m/s
				m_bAir = true;
			}
			else
			{
				m_pRide->AddWorldPosY(500.f * fTime);
				m_fY = fScale;
			}
		}
		else
		{
			switch (rand() % 3)
			{
			case 0:
				GET_SINGLE(CSoundManager)->Play("Moving_Grass_Jump_00");
				break;
			case 1:
				GET_SINGLE(CSoundManager)->Play("Moving_Grass_Jump_01");
				break;
			case 2:
				GET_SINGLE(CSoundManager)->Play("Moving_Grass_Jump_02");
				break;
			}

			SetSpeedY(200.f);
			//m_fSpeed = 500.f;	//	500 m/s
			m_bAir = true;
		}
	}
}
void CPlayer3D::MoveZ(float fScale, float fTime)
{
	CSceneComponent* pMesh = static_cast<CSceneComponent*>(m_pMesh.get());
	if (fScale != 0.f)
	{
		if (pMesh)
		{
			pMesh->SetRelativeRotY(RadToDeg(m_fRot) + 180.f);
			Vector3 vAxis = pMesh->GetWorldAxis(AXIS_Z) * fScale;
			Vector3 vPos = GetWorldPos();
			if (!m_bAir)
			{
				if (m_pTerrain)
				{
					Vector3 vTangent = m_pTerrain->GetTangent(vAxis, vPos);

					float fDot = vTangent.Dot(vAxis);
					if (fDot > 1.f)
						fDot = 1.f;
					float fAngle = acosf(fDot);

					if (fAngle < PI / 3.f || vTangent.y < vAxis.y)
					{
						if (m_pRide)
						{
							if (!m_pRide->IsEnable())
							{
								pMesh->AddWorldPos(vTangent * fTime * m_fMoveSpeed * abs(fScale));
							}
						}
						else
						{
							pMesh->AddWorldPos(vTangent * fTime * m_fMoveSpeed * abs(fScale));
						}
					}
				}
			}
			else
			{
				pMesh->AddWorldPos(vAxis * fTime * m_fMoveSpeed);
			}
		}
		m_fZ = fScale;
	}
}
void CPlayer3D::MoveX(float fScale, float fTime)
{
	CSceneComponent* pMesh = static_cast<CSceneComponent*>(m_pMesh.get());
	if (fScale != 0.f)
	{
		if (pMesh)
		{
			pMesh->SetWorldRotY(RadToDeg(m_fRot) + 180.f);

			Vector3 vAxis = pMesh->GetWorldAxis(AXIS_X) * fScale;
			Vector3 vPos = GetWorldPos();

			if (!m_bAir)
			{
				if (m_pTerrain)
				{
					Vector3 vTangent = m_pTerrain->GetTangent(vAxis, vPos);

					float fDot = vTangent.Dot(vAxis);
					if (fDot > 1.f)
						fDot = 1.f;
					float fAngle = acosf(fDot);

					if (fAngle < PI / 3.f || vTangent.y < vAxis.y)
					{
						if (m_pRide)
						{
							if (!m_pRide->IsEnable())
							{
								pMesh->AddWorldPos(vTangent * fTime * m_fMoveSpeed * abs(fScale));
							}
							else
							{
								Vector3 vAxis = m_pRide->GetWorldAxis(AXIS_X);
								m_pRide->AddWorldPos(vAxis * fTime * m_fMoveSpeed * 2.f);
							}
						}
						else
						{
							pMesh->AddWorldPos(vTangent * fTime * m_fMoveSpeed * abs(fScale));
						}
					}
				}
			}
			else
			{
				pMesh->AddWorldPos(vAxis * fTime * m_fMoveSpeed);
			}
		}
		m_fX = fScale;
	}
}
void CPlayer3D::BoneRotX(float fScale, float fTime)
{
	if (m_pRide)
	{
		std::shared_ptr<CComponent> pRoot = m_pRide->GetRootComponent();

		std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pRoot.get())->GetTransform();

		pTransform->AddQuaternionRotZ(fScale * fTime * 90.f);
	}
}
void CPlayer3D::BoneRotY(float fScale, float fTime)
{
	if (m_pRide)
	{
		std::shared_ptr<CComponent> pRoot = m_pRide->GetRootComponent();

		std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pRoot.get())->GetTransform();

		pTransform->AddQuaternionRotY(fScale * fTime * 90.f);
	}
}

void CPlayer3D::Inventory(float fTime)
{
	CLayer* pLayer = m_pScene->FindLayer(UI_LAYER);

	if (pLayer)
	{
		std::shared_ptr<CGameObject> pInven = pLayer->FindObj("Inventory");

		if (pInven)
		{
			pInven->SetEnable(!pInven->IsEnable());

			if (pInven->IsEnable())
			{
				GET_SINGLE(CSoundManager)->Play("Window_InventoryOpen");
			}
			else
			{
				GET_SINGLE(CSoundManager)->Play("Window_InventoryClose");
			}
		}
	}
}
void CPlayer3D::Equip(float fTime)
{
	CLayer* pLayer = m_pScene->FindLayer(UI_LAYER);

	if (pLayer)
	{
		std::shared_ptr<CGameObject> pEquip = pLayer->FindObj("Equip");
		bool bOpen = false;

		if (pEquip)
		{
			bOpen = !pEquip->IsEnable();

			pEquip->SetEnable(bOpen);
		}

		SetUIEnableAll(bOpen);
		m_bEquipWindowOpened = bOpen;
		if (m_pWeapon)
		{
			m_pWeapon->SetUIEnableAll(bOpen);
		}

		if (bOpen)
		{
			GET_SINGLE(CSoundManager)->Play("Window_UIOpen");
		}
		else
		{
			GET_SINGLE(CSoundManager)->Play("Window_UIClose");
		}
	}
}
void CPlayer3D::SpawnRide(float fTime)
{
	if (m_pRide)
	{
		if (m_pRide->IsEnable())
		{
			SpawnRide();
			return;
		}
	}
	ChangeSequenceAll("sk_11_cst_start_Upper");

	m_bCast = true;
	m_fCastMax = 2.5f;
	m_eSkillType = SKILL_TYPE::NONE;
}

void CPlayer3D::Skill1(float fTime)
{
}
void CPlayer3D::Skill1Down(float fTime)
{
}
void CPlayer3D::Skill2(float fTime)
{
}
void CPlayer3D::Skill3(float fTime)
{
}
void CPlayer3D::Quest(float fTime)
{
	std::shared_ptr<CGameObject> pQuest = m_pScene->FindLayer(UI_LAYER)->FindObj("Mission");

	if (pQuest)
	{
		pQuest->SetEnable(!pQuest->IsEnable());

		if (pQuest->IsEnable())
		{
			GET_SINGLE(CSoundManager)->Play("Window_QuestOpen");
		}
		else
		{
			GET_SINGLE(CSoundManager)->Play("Window_QuestClose");
		}
	}
}

void CPlayer3D::RunSound(float fTime)
{
	CGameMode* pMode = m_pScene->GetGameMode();

	if (typeid(*pMode) == typeid(CTown))
	{
		switch (rand() % 4)
		{
		case 0:
			GET_SINGLE(CSoundManager)->Play("PCStep_Rock_RL_00");
			break;
		case 1:
			GET_SINGLE(CSoundManager)->Play("PCStep_Rock_RL_01");
			break;
		case 2:
			GET_SINGLE(CSoundManager)->Play("PCStep_Rock_RL_02");
			break;
		case 3:
			GET_SINGLE(CSoundManager)->Play("PCStep_Rock_RL_03");
			break;
		}
	}
	else
	{
		switch (rand() % 4)
		{
		case 0:
			GET_SINGLE(CSoundManager)->Play("PCStep_Grass_RL_00");
			break;
		case 1:
			GET_SINGLE(CSoundManager)->Play("PCStep_Grass_RL_01");
			break;
		case 2:
			GET_SINGLE(CSoundManager)->Play("PCStep_Grass_RL_02");
			break;
		case 3:
			GET_SINGLE(CSoundManager)->Play("PCStep_Grass_RL_03");
			break;
		}
	}
}

void CPlayer3D::SetTransition()
{
	m_pUpperAni->AddTransition("WalkToIdle", "Walk", this, &CPlayer3D::WalkToIdle, "Idle");
	m_pUpperAni->AddTransition("IdleToWalk", "Idle", this, &CPlayer3D::IdleToWalk, "Walk");
	m_pUpperAni->AddTransition("IdleToWalkBack", "Idle", this, &CPlayer3D::IdleToWalkBack, "WalkBack");
	m_pUpperAni->AddTransition("WalkBackToIdle", "WalkBack", this, &CPlayer3D::WalkToIdle, "Idle");
	m_pUpperAni->AddTransition("IdleToJump", "Idle", this, &CPlayer3D::IdleToJump, "Jump_Up_WP");
	m_pUpperAni->AutoTransition("Jump_Up_WP", "JumpToLoop", "jump_loop_WP");
	m_pUpperAni->AddTransition("JumpLoopToLand", "jump_loop_WP", this, &CPlayer3D::JumpLoopToLand, "Jump_Land_WP");
	m_pUpperAni->AutoTransition("Jump_Land_WP", "JumpLandToIdle", "Idle");
	m_pUpperAni->AddTransition("JumpUpToLand", "Jump_Up_WP", this, &CPlayer3D::JumpLoopToLand, "Jump_Land_WP");
	m_pUpperAni->AddTransition("IdleToDMG", "Idle", this, &CPlayer3D::IdleToDMG, "DMG_F_WP");
	m_pUpperAni->AddTransition("WalkToRunF", "Walk", this, &CPlayer3D::WalkToRunF, "Run_F_WP");
	m_pUpperAni->AddTransition("WalkToRunFL", "Walk", this, &CPlayer3D::WalkToRunFL, "Run_FL_WP");
	m_pUpperAni->AddTransition("WalkToRunFR", "Walk", this, &CPlayer3D::WalkToRunFR, "Run_FR_WP");
	m_pUpperAni->AddTransition("WalkToRunB", "Walk", this, &CPlayer3D::WalkToRunB, "Run_B_WP");
	m_pUpperAni->AddTransition("WalkToRunBL", "Walk", this, &CPlayer3D::WalkToRunBL, "Run_BL_WP");
	m_pUpperAni->AddTransition("WalkToRunBR", "Walk", this, &CPlayer3D::WalkToRunBR, "Run_BR_WP");
	m_pUpperAni->AddTransition("WalkToRunL", "Walk", this, &CPlayer3D::WalkToRunL, "Run_L_WP");
	m_pUpperAni->AddTransition("WalkToRunR", "Walk", this, &CPlayer3D::WalkToRunR, "Run_R_WP");
	m_pUpperAni->AddTransition("RunJumpLoopToLand", "run_jump_loop_f_WP", this, &CPlayer3D::JumpLoopToLand, "run_jump_land_f_WP");
	m_pUpperAni->AddTransition("RunJumpLandToRunF", "run_jump_land_f_WP", this, &CPlayer3D::RunJumpLandToRunF, "Run_F_WP");
	m_pUpperAni->AddTransition("RunJumpLandToRunFR", "run_jump_land_f_WP", this, &CPlayer3D::RunJumpLandToRunFR, "Run_FR_WP");
	m_pUpperAni->AddTransition("RunJumpLandToRunFL", "run_jump_land_f_WP", this, &CPlayer3D::RunJumpLandToRunFL, "Run_FL_WP");
	m_pUpperAni->AddTransition("RunJumpLandToRunB", "run_jump_land_f_WP", this, &CPlayer3D::RunJumpLandToRunB, "Run_B_WP");
	m_pUpperAni->AddTransition("RunJumpLandToRunBR", "run_jump_land_f_WP", this, &CPlayer3D::RunJumpLandToRunBR, "Run_BR_WP");
	m_pUpperAni->AddTransition("RunJumpLandToRunBL", "run_jump_land_f_WP", this, &CPlayer3D::RunJumpLandToRunBL, "Run_BL_WP");
	m_pUpperAni->AddTransition("RunJumpLandToRunL", "run_jump_land_f_WP", this, &CPlayer3D::RunJumpLandToRunL, "Run_L_WP");
	m_pUpperAni->AddTransition("RunJumpLandToRunR", "run_jump_land_f_WP", this, &CPlayer3D::RunJumpLandToRunR, "Run_R_WP");
	m_pUpperAni->AddTransition("RunJumpUpToLand", "run_jump_up_F_WP", this, &CPlayer3D::JumpLoopToLand, "run_jump_land_f_WP");
	m_pUpperAni->AutoTransition("run_jump_up_F_WP", "RunJumpToLoop", "run_jump_loop_f_WP");
	m_pUpperAni->AddTransition("RunFToWalk", "Run_F_WP", TransitionFunc, "Walk");
	m_pUpperAni->AddTransition("RunFLToWalk", "Run_FL_WP", TransitionFunc, "Walk");
	m_pUpperAni->AddTransition("RunFRToWalk", "Run_FR_WP", TransitionFunc, "Walk");
	m_pUpperAni->AddTransition("RunBToWalk", "Run_B_WP", TransitionFunc, "Walk");
	m_pUpperAni->AddTransition("RunBLToWalk", "Run_BL_WP", TransitionFunc, "Walk");
	m_pUpperAni->AddTransition("RunBRToWalk", "Run_BR_WP", TransitionFunc, "Walk");
	m_pUpperAni->AddTransition("RunLToWalk", "Run_L_WP", TransitionFunc, "Walk");
	m_pUpperAni->AddTransition("RunRToWalk", "Run_R_WP", TransitionFunc, "Walk");
	m_pUpperAni->AddTransition("RunFToIdle", "Run_F_WP", this, &CPlayer3D::WalkToIdle, "Idle");
	m_pUpperAni->AddTransition("RunFLToIdle", "Run_FL_WP", this, &CPlayer3D::WalkToIdle, "Idle");
	m_pUpperAni->AddTransition("RunFRToIdle", "Run_FR_WP", this, &CPlayer3D::WalkToIdle, "Idle");
	m_pUpperAni->AddTransition("RunBToIdle", "Run_B_WP", this, &CPlayer3D::WalkToIdle, "Idle");
	m_pUpperAni->AddTransition("RunBLToIdle", "Run_BL_WP", this, &CPlayer3D::WalkToIdle, "Idle");
	m_pUpperAni->AddTransition("RunBRToIdle", "Run_BR_WP", this, &CPlayer3D::WalkToIdle, "Idle");
	m_pUpperAni->AddTransition("RunLToIdle", "Run_L_WP", this, &CPlayer3D::WalkToIdle, "Idle");
	m_pUpperAni->AddTransition("RunRToIdle", "Run_R_WP", this, &CPlayer3D::WalkToIdle, "Idle");
	m_pUpperAni->AddTransition("WalkBackToRunB", "WalkBack", this, &CPlayer3D::WalkToRunB, "Run_B_WP");
	m_pUpperAni->AddTransition("WalkBackToRunBL", "WalkBack", this, &CPlayer3D::WalkToRunBL, "Run_BL_WP");
	m_pUpperAni->AddTransition("WalkBackToRunBR", "WalkBack", this, &CPlayer3D::WalkToRunBR, "Run_BR_WP");
	m_pUpperAni->AddTransition("WalkBackToRunR", "WalkBack", this, &CPlayer3D::WalkToRunR, "Run_R_WP");
	m_pUpperAni->AddTransition("RunFToJump", "Run_F_WP", this, &CPlayer3D::IdleToJump, "run_jump_up_F_WP");
	m_pUpperAni->AddTransition("RunFLToJump", "Run_FL_WP", this, &CPlayer3D::IdleToJump, "run_jump_up_F_WP");
	m_pUpperAni->AddTransition("RunFRToJump", "Run_FR_WP", this, &CPlayer3D::IdleToJump, "run_jump_up_F_WP");
	m_pUpperAni->AddTransition("RunBLToJump", "Run_BL_WP", this, &CPlayer3D::IdleToJump, "run_jump_up_F_WP");
	m_pUpperAni->AddTransition("RunBRToJump", "Run_BR_WP", this, &CPlayer3D::IdleToJump, "run_jump_up_F_WP");
	m_pUpperAni->AddTransition("RunBToJump", "Run_B_WP", this, &CPlayer3D::IdleToJump, "run_jump_up_F_WP");
	m_pUpperAni->AddTransition("RunLToJump", "Run_L_WP", this, &CPlayer3D::IdleToJump, "run_jump_up_F_WP");
	m_pUpperAni->AddTransition("RunRToJump", "Run_R_WP", this, &CPlayer3D::IdleToJump, "run_jump_up_F_WP");
	m_pUpperAni->AddTransition("RunFToRunFR", "Run_F_WP", this, &CPlayer3D::RunJumpLandToRunFR, "Run_FR_WP");
	m_pUpperAni->AddTransition("RunLFToRunFR", "Run_LF_WP", this, &CPlayer3D::RunJumpLandToRunFR, "Run_FR_WP");
	m_pUpperAni->AddTransition("RunLToRunFR", "Run_L_WP", this, &CPlayer3D::RunJumpLandToRunFR, "Run_FR_WP");
	m_pUpperAni->AddTransition("RunBLToRunFR", "Run_BL_WP", this, &CPlayer3D::RunJumpLandToRunFR, "Run_FR_WP");
	m_pUpperAni->AddTransition("RunBToRunFR", "Run_B_WP", this, &CPlayer3D::RunJumpLandToRunFR, "Run_FR_WP");
	m_pUpperAni->AddTransition("RunRBToRunFR", "Run_RB_WP", this, &CPlayer3D::RunJumpLandToRunFR, "Run_FR_WP");
	m_pUpperAni->AddTransition("RunFLToRunF", "Run_FL_WP", this, &CPlayer3D::RunJumpLandToRunF, "Run_F_WP");
	m_pUpperAni->AddTransition("RunRToRunF", "Run_R_WP", this, &CPlayer3D::RunJumpLandToRunF, "Run_F_WP");
	m_pUpperAni->AddTransition("RunLToRunF", "Run_L_WP", this, &CPlayer3D::RunJumpLandToRunF, "Run_F_WP");
	m_pUpperAni->AddTransition("RunBLToRunF", "Run_BL_WP", this, &CPlayer3D::RunJumpLandToRunF, "Run_F_WP");
	m_pUpperAni->AddTransition("RunBToRunF", "Run_B_WP", this, &CPlayer3D::RunJumpLandToRunF, "Run_F_WP");
	m_pUpperAni->AddTransition("RunBRToRunF", "Run_BR_WP", this, &CPlayer3D::RunJumpLandToRunF, "Run_F_WP");
	m_pUpperAni->AddTransition("RunFRToRunR", "Run_FR_WP", this, &CPlayer3D::RunJumpLandToRunR, "Run_R_WP");
	m_pUpperAni->AddTransition("RunFToRunR", "Run_F_WP", this, &CPlayer3D::RunJumpLandToRunR, "Run_R_WP");
	m_pUpperAni->AddTransition("RunFLToRunR", "Run_FL_WP", this, &CPlayer3D::RunJumpLandToRunR, "Run_R_WP");
	m_pUpperAni->AddTransition("RunLToRunR", "Run_L_WP", this, &CPlayer3D::RunJumpLandToRunR, "Run_R_WP");
	m_pUpperAni->AddTransition("RunBLToRunR", "Run_BL_WP", this, &CPlayer3D::RunJumpLandToRunR, "Run_R_WP");
	m_pUpperAni->AddTransition("RunBToRunR", "Run_B_WP", this, &CPlayer3D::RunJumpLandToRunR, "Run_R_WP");
	m_pUpperAni->AddTransition("RunRToRunBR", "Run_R_WP", this, &CPlayer3D::RunJumpLandToRunBR, "Run_BR_WP");
	m_pUpperAni->AddTransition("RunFRToRunBR", "Run_FR_WP", this, &CPlayer3D::RunJumpLandToRunBR, "Run_BR_WP");
	m_pUpperAni->AddTransition("RunFToRunBR", "Run_F_WP", this, &CPlayer3D::RunJumpLandToRunBR, "Run_BR_WP");
	m_pUpperAni->AddTransition("RunFLToRunBR", "Run_FL_WP", this, &CPlayer3D::RunJumpLandToRunBR, "Run_BR_WP");
	m_pUpperAni->AddTransition("RunLToRunBR", "Run_L_WP", this, &CPlayer3D::RunJumpLandToRunBR, "Run_BR_WP");
	m_pUpperAni->AddTransition("RunBLToRunBR", "Run_BL_WP", this, &CPlayer3D::RunJumpLandToRunBR, "Run_BR_WP");
	m_pUpperAni->AddTransition("RunBRToRunB", "Run_BR_WP", this, &CPlayer3D::RunJumpLandToRunB, "Run_B_WP");
	m_pUpperAni->AddTransition("RunRToRunB", "Run_R_WP", this, &CPlayer3D::RunJumpLandToRunB, "Run_B_WP");
	m_pUpperAni->AddTransition("RunFRToRunB", "Run_FR_WP", this, &CPlayer3D::RunJumpLandToRunB, "Run_B_WP");
	m_pUpperAni->AddTransition("RunFToRunB", "Run_F_WP", this, &CPlayer3D::RunJumpLandToRunB, "Run_B_WP");
	m_pUpperAni->AddTransition("RunFLToRunB", "Run_FL_WP", this, &CPlayer3D::RunJumpLandToRunB, "Run_B_WP");
	m_pUpperAni->AddTransition("RunLToRunB", "Run_L_WP", this, &CPlayer3D::RunJumpLandToRunB, "Run_B_WP");
	m_pUpperAni->AddTransition("RunBToRunBL", "Run_B_WP", this, &CPlayer3D::RunJumpLandToRunBL, "Run_BL_WP");
	m_pUpperAni->AddTransition("RunBLToRunL", "Run_BL_WP", this, &CPlayer3D::RunJumpLandToRunL, "Run_L_WP");
	m_pUpperAni->AddTransition("RunBToRunL", "Run_B_WP", this, &CPlayer3D::RunJumpLandToRunL, "Run_L_WP");
	m_pUpperAni->AddTransition("RunBRToRunL", "Run_BR_WP", this, &CPlayer3D::RunJumpLandToRunL, "Run_L_WP");
	m_pUpperAni->AddTransition("RunRToRunL", "Run_R_WP", this, &CPlayer3D::RunJumpLandToRunL, "Run_L_WP");
	m_pUpperAni->AddTransition("RunFRToRunL", "Run_FR_WP", this, &CPlayer3D::RunJumpLandToRunL, "Run_L_WP");
	m_pUpperAni->AddTransition("RunFToRunL", "Run_F_WP", this, &CPlayer3D::RunJumpLandToRunL, "Run_L_WP");
	m_pUpperAni->AddTransition("RunLToRunFL", "Run_L_WP", this, &CPlayer3D::RunJumpLandToRunFL, "Run_FL_WP");
	m_pUpperAni->AddTransition("RunBLToRunFL", "Run_BL_WP", this, &CPlayer3D::RunJumpLandToRunFL, "Run_FL_WP");
	m_pUpperAni->AddTransition("RunBToRunFL", "Run_B_WP", this, &CPlayer3D::RunJumpLandToRunFL, "Run_FL_WP");
	m_pUpperAni->AddTransition("RunBRToRunFL", "Run_BR_WP", this, &CPlayer3D::RunJumpLandToRunFL, "Run_FL_WP");
	m_pUpperAni->AddTransition("RunRToRunFL", "Run_R_WP", this, &CPlayer3D::RunJumpLandToRunFL, "Run_FL_WP");
	m_pUpperAni->AddTransition("RunFRToRunFL", "Run_FR_WP", this, &CPlayer3D::RunJumpLandToRunFL, "Run_FL_WP");
	m_pUpperAni->AddTransition("RunFToRunFL", "Run_F_WP", this, &CPlayer3D::RunJumpLandToRunFL, "Run_FL_WP");
	m_pUpperAni->AddTransition("RunLToRunBL", "Run_L_WP", this, &CPlayer3D::RunJumpLandToRunBL, "Run_BL_WP");
	m_pUpperAni->AddTransition("RunFLToRunBL", "Run_FL_WP", this, &CPlayer3D::RunJumpLandToRunBL, "Run_BL_WP");
	m_pUpperAni->AddTransition("RunFToRunBL", "Run_F_WP", this, &CPlayer3D::RunJumpLandToRunBL, "Run_BL_WP");
	m_pUpperAni->AddTransition("RunFRToRunBL", "Run_FR_WP", this, &CPlayer3D::RunJumpLandToRunBL, "Run_BL_WP");
	m_pUpperAni->AddTransition("RunRToRunBL", "Run_R_WP", this, &CPlayer3D::RunJumpLandToRunBL, "Run_BL_WP");
	m_pUpperAni->AddTransition("RunBRToRunBL", "Run_BR_WP", this, &CPlayer3D::RunJumpLandToRunBL, "Run_BL_WP");
	m_pUpperAni->AddTransition("RunFLToRunL", "Run_FL_WP", this, &CPlayer3D::RunJumpLandToRunL, "Run_L_WP");
	m_pUpperAni->AddTransition("RunBLToRunB", "Run_BL_WP", this, &CPlayer3D::RunJumpLandToRunB, "Run_B_WP");
	m_pUpperAni->AddTransition("RunBToRunBR", "Run_B_WP", this, &CPlayer3D::RunJumpLandToRunBR, "Run_BR_WP");
	m_pUpperAni->AddTransition("RunBRToRunR", "Run_BR_WP", this, &CPlayer3D::RunJumpLandToRunR, "Run_R_WP");
	m_pUpperAni->AddTransition("RunRToRunFR", "Run_R_WP", this, &CPlayer3D::RunJumpLandToRunFR, "Run_FR_WP");
	m_pUpperAni->AddTransition("RunFRToRunF", "Run_FR_WP", this, &CPlayer3D::RunJumpLandToRunF, "Run_F_WP");
	m_pUpperAni->AddTransition("RunJumpLandToIdle", "run_jump_land_f_WP", this, &CPlayer3D::RunJumpLandToIdle, "Idle");
	m_pUpperAni->AddTransition("RunFToDMG", "Run_F_WP", this, &CPlayer3D::IdleToDMG, "DMG_F_WP");
	m_pUpperAni->AddTransition("RunFLToDMG", "Run_FL_WP", this, &CPlayer3D::IdleToDMG, "DMG_F_WP");
	m_pUpperAni->AddTransition("RunLToDMG", "Run_L_WP", this, &CPlayer3D::IdleToDMG, "DMG_F_WP");
	m_pUpperAni->AddTransition("RunBLToDMG", "Run_BL_WP", this, &CPlayer3D::IdleToDMG, "DMG_F_WP");
	m_pUpperAni->AddTransition("RunBToDMG", "Run_B_WP", this, &CPlayer3D::IdleToDMG, "DMG_F_WP");
	m_pUpperAni->AddTransition("RunBRToDMG", "Run_BR_WP", this, &CPlayer3D::IdleToDMG, "DMG_F_WP");
	m_pUpperAni->AddTransition("RunRToDMG", "Run_R_WP", this, &CPlayer3D::IdleToDMG, "DMG_F_WP");
	m_pUpperAni->AddTransition("RunFRToDMG", "Run_FR_WP", this, &CPlayer3D::IdleToDMG, "DMG_F_WP");
	m_pUpperAni->AddTransition("WalkBackToDMG", "WalkBack", this, &CPlayer3D::IdleToDMG, "DMG_F_WP");
	m_pUpperAni->AddTransition("WalkToDMG", "Walk", this, &CPlayer3D::IdleToDMG, "DMG_F_WP");
	m_pUpperAni->AddTransition("IdleToDown", "Idle", this, &CPlayer3D::IdleToDown, "Down");
	m_pUpperAni->AddTransition("DMGToDown", "DMG_F_WP", this, &CPlayer3D::IdleToDown, "Down");
	m_pUpperAni->AddTransition("LoopToDeadBody", "Down_Loop", this, &CPlayer3D::LoopToDeadBody, "down_deadbody");

	m_pUpperAni->AddTransition("IdleToSK_14", "Idle", &CPlayer3D::TransitionFunc,"SK_14");
	m_pUpperAni->AddTransition("SK_14ToDMG", "SK_14", static_cast<CPlayer3D*>(this), &CPlayer3D::IdleToDMG, "DMG_F_WP");
	m_pUpperAni->AddTransition("Idle_To_sk_08_upper", "Idle", &CPlayer3D::TransitionFunc, "sk_08_upper");
	m_pUpperAni->AutoTransition("SK_14", "SK_14ToIdle", "Idle");
	m_pUpperAni->AutoTransition("sk_08_upper", "sk_08_upper_To_Idle", "Idle");

	m_pUpperAni->AutoTransition("Down", "DownToLoop", "Down_Loop");
	m_pUpperAni->AutoTransition("DMG_F_WP", "DMGToIdle", "Idle");

	m_pUpperAni->AddTransition("IdleTosk_28_cst_start", "Idle", &CPlayer3D::TransitionFunc, "sk_28_cst_start");
	m_pUpperAni->AutoTransition("sk_28_cst_start", "sk_28_cst_startToLoop", "sk_28_cst_loop");
	m_pUpperAni->AddTransition("sk_28_cstToEnd", "sk_28_cst_loop", static_cast<CPlayer3D*>(this), &CPlayer3D::Sk_28_LoopToEnd, "sk_28_cst_end");
	m_pUpperAni->AutoTransition("sk_28_cst_end", "sk_28_cstToIdle", "Idle");
	m_pUpperAni->AddTransition("sk_28_cstToWalk", "sk_28_cst_loop", static_cast<CPlayer3D*>(this), &CPlayer3D::Sk_28ToWalk, "Walk");
	m_pUpperAni->AddTransition("sk_28_cstStartToWalk", "sk_28_cst_start", static_cast<CPlayer3D*>(this), &CPlayer3D::Sk_28ToWalk, "Walk");

	m_pUpperAni->AddTransition("IdleTosk_11_cst_start", "Idle", 
		&CPlayer3D::TransitionFunc, "sk_11_cst_start_Upper");
	m_pUpperAni->AutoTransition("sk_11_cst_start_Upper", "sk_11_cst_startToLoop", 
		"sk_11_cst_loop_Upper");
	m_pUpperAni->AddTransition("sk_11_cstToEnd", "sk_11_cst_loop_Upper", 
		static_cast<CPlayer3D*>(this), &CPlayer3D::Sk_28_LoopToEnd, "sk_11_cst_end_Upper");
	m_pUpperAni->AutoTransition("sk_11_cst_end_Upper", "sk_11_cstToIdle", "Idle");
	m_pUpperAni->AddTransition("sk_11_cstToWalk", "sk_11_cst_loop_Upper", 
		static_cast<CPlayer3D*>(this), &CPlayer3D::Sk_28ToWalk, "Walk");
	m_pUpperAni->AddTransition("sk_11_cstStartToWalk", "sk_11_cst_start_Upper", 
		static_cast<CPlayer3D*>(this), &CPlayer3D::Sk_28ToWalk, "Walk");

	m_pUpperAni->AddTransitionNotify("JumpLandToIdle", "Jump_Land_WP", this, &CPlayer3D::Land);
	m_pUpperAni->AddTransitionNotify("RunJumpLandToRunFR", "run_jump_land_f_WP", this, &CPlayer3D::Land);
	m_pUpperAni->AddTransitionNotify("RunJumpLandToRunFL", "run_jump_land_f_WP", this, &CPlayer3D::Land);
	m_pUpperAni->AddTransitionNotify("RunJumpLandToRunF", "run_jump_land_f_WP", this, &CPlayer3D::Land);
	m_pUpperAni->AddTransitionNotify("RunJumpLandToRunR", "run_jump_land_f_WP", this, &CPlayer3D::Land);
	m_pUpperAni->AddTransitionNotify("RunJumpLandToRunBR", "run_jump_land_f_WP", this, &CPlayer3D::Land);
	m_pUpperAni->AddTransitionNotify("RunJumpLandToRunBL", "run_jump_land_f_WP", this, &CPlayer3D::Land);
	m_pUpperAni->AddTransitionNotify("RunJumpLandToRunB", "run_jump_land_f_WP", this, &CPlayer3D::Land);
	m_pUpperAni->AddTransitionNotify("RunJumpLandToRunL", "run_jump_land_f_WP", this, &CPlayer3D::Land);
	m_pUpperAni->AddTransitionNotify("RunJumpLandToIdle", "run_jump_land_f_WP", this, &CPlayer3D::Land);
	m_pUpperAni->AddTransitionNotify("DMGToIdle", "DMG_F_WP", this, &CPlayer3D::DamageEnd);
	m_pUpperAni->AddTransitionNotify("DownToLoop", "Down", this, &CPlayer3D::DeadTimeStart);
	m_pUpperAni->AddTransitionNotify("LoopToDeadBody", "Down_Loop", this, &CPlayer3D::DeadBody);

	m_pUpperAni->AddTransitionNotify("SK_14ToIdle", "SK_14", this, 
		&CPlayer3D::AttackEnd);
	m_pUpperAni->AddTransitionNotify("sk_08_upper_To_Idle", "sk_08_upper", this, 
		&CPlayer3D::AttackEnd);
	m_pUpperAni->AddTransitionNotify("IdleToSK_14", "Idle", this, 
		&CPlayer3D::AttackStart);
	m_pUpperAni->AddTransitionNotify("Idle_To_sk_08_upper", "Idle", this, 
		&CPlayer3D::sk08Start);
	m_pUpperAni->AddTransitionNotify("sk_28_cstToIdle", "sk_28_cst_end", this, 
		&CPlayer3D::CastFinish);
	m_pUpperAni->AddTransitionNotify("IdleTosk_28_cst_start", "Idle", this,
		&CPlayer3D::CastStart);
	m_pUpperAni->AddTransitionNotify("sk_28_cstToWalk", "sk_28_cst_loop", this, 
		&CPlayer3D::CastCancel);
	m_pUpperAni->AddTransitionNotify("sk_28_cstStartToWalk", "sk_28_cst_start", this, 
		&CPlayer3D::CastCancel);
	m_pUpperAni->AddTransitionNotify("sk_11_cstToIdle", "sk_11_cst_end_Upper", this, 
		&CPlayer3D::CastFinish);
	m_pUpperAni->AddTransitionNotify("IdleTosk_11_cst_start", "Idle", this, 
		&CPlayer3D::CastStart);
	m_pUpperAni->AddTransitionNotify("sk_11_cstToWalk", "sk_11_cst_loop_Upper", this, 
		&CPlayer3D::CastCancel);
	m_pUpperAni->AddTransitionNotify("sk_11_cstStartToWalk", "sk_11_cst_start_Upper", this,
		&CPlayer3D::CastCancel);

	m_pUpperAni->AddNotifyFunction("AttackEnd", this, &CPlayer3D::AttackEnd);
	m_pUpperAni->AddNotifyFunction("sk08End", this, &CPlayer3D::AttackEnd);

	m_pUpperAni->CreateNotify("Run_F_WP", "RunF1", 24.62f);
	m_pUpperAni->AddNotifyFunction("RunF1", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_F_WP", "RunF2", 25.f);
	m_pUpperAni->AddNotifyFunction("RunF2", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_R_WP", "RunR1", 15.62f);
	m_pUpperAni->AddNotifyFunction("RunF1", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_R_WP", "RunR2", 16.05f);
	m_pUpperAni->AddNotifyFunction("RunF2", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_L_WP", "RunL1", 16.52f);
	m_pUpperAni->AddNotifyFunction("RunF1", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_L_WP", "RunL2", 16.8f);
	m_pUpperAni->AddNotifyFunction("RunF2", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_FR_WP", "RunFR1", 24.62f);
	m_pUpperAni->AddNotifyFunction("RunBR1", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_FR_WP", "RunFR2", 25.f);
	m_pUpperAni->AddNotifyFunction("RunBR2", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_FL_WP", "RunFL1", 27.35f);
	m_pUpperAni->AddNotifyFunction("RunBL1", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_FL_WP", "RunFL2", 27.7f);
	m_pUpperAni->AddNotifyFunction("RunBL2", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_B_WP", "RunB1", 28.08f);
	m_pUpperAni->AddNotifyFunction("RunB1", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_B_WP", "RunB2", 28.5f);
	m_pUpperAni->AddNotifyFunction("RunB2", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_BR_WP", "RunBR1", 24.62f);
	m_pUpperAni->AddNotifyFunction("RunBR1", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_BR_WP", "RunBR2", 25.f);
	m_pUpperAni->AddNotifyFunction("RunBR2", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_BL_WP", "RunBL1", 24.62f);
	m_pUpperAni->AddNotifyFunction("RunBL1", this, &CPlayer3D::RunSound);
	m_pUpperAni->CreateNotify("Run_BL_WP", "RunBL2", 25.f);
	m_pUpperAni->AddNotifyFunction("RunBL2", this, &CPlayer3D::RunSound);
}
void CPlayer3D::ChangeSequenceAll(const char* pSeq)
{
	m_pUpperAni->ChangeSequence(pSeq); 
}
bool CPlayer3D::TransitionFunc()
{
	return false;
}
void CPlayer3D::Land()
{
	switch (rand()%3)
	{
	case 0:
		GET_SINGLE(CSoundManager)->Play("Moving_Grass_Land_00");
		break;
	case 1:
		GET_SINGLE(CSoundManager)->Play("Moving_Grass_Land_01");
		break;
	case 2:
		GET_SINGLE(CSoundManager)->Play("Moving_Grass_Land_02");
		break;
	}

	m_bAir = false;
}
bool CPlayer3D::AttackToIdleFunc()
{
	return false;
}
bool CPlayer3D::RunJumpLandToIdle()
{
	return m_fX == 0.f && m_fZ == 0.f;
}
bool CPlayer3D::RunJumpLandToRunFR()
{
	return m_fX == 1.f && m_fZ == -1.f;
}
bool CPlayer3D::RunJumpLandToRunFL()
{
	return m_fX == 1.f && m_fZ == 1.f;
}
bool CPlayer3D::RunJumpLandToRunF()
{
	return m_fX == 1.f && m_fZ == 0.f;
}
bool CPlayer3D::RunJumpLandToRunL()
{
	return m_fX == 0.f && m_fZ == 1.f;
}
bool CPlayer3D::RunJumpLandToRunBR()
{
	return m_fX == -1.f && m_fZ == -1.f;
}
bool CPlayer3D::RunJumpLandToRunBL()
{
	return m_fX == -1.f && m_fZ == 1.f;
}
bool CPlayer3D::RunJumpLandToRunB()
{
	return m_fX == -1.f && m_fZ == 0.f;
}
bool CPlayer3D::RunJumpLandToRunR()
{
	return m_fX == 0.f && m_fZ == -1.f;
}
bool CPlayer3D::IdleToWalk()
{
	if (m_pRide)
	{
		return !m_pRide->IsEnable() && (m_fX > 0.f || (m_fX == 0.f && m_fZ > 0.f));
	}

	return m_fX > 0.f || (m_fX == 0.f && m_fZ > 0.f);
}
bool CPlayer3D::IdleToWalkBack()
{
	if (m_pRide)
	{
		return !m_pRide->IsEnable() && (m_fX < 0.f || (m_fX == 0.f && m_fZ < 0.f));
	}
	return m_fX < 0.f || (m_fX == 0.f && m_fZ < 0.f);
}
bool CPlayer3D::WalkToIdle()
{
	return m_fX == 0.f && m_fZ == 0.f;
}
bool CPlayer3D::IdleToJump()
{
	if(m_pRide)
		return (m_bAir || m_fHeight >= 10.f) && !m_pRide->IsEnable();

	return m_bAir || m_fHeight >= 10.f;
}
bool CPlayer3D::IdleToSk14()
{
	return m_bAttack && m_fX == 0.f && m_fZ == 0.f;
}
bool CPlayer3D::JumpLandToIdle()
{
	return true;
}
bool CPlayer3D::JumpLoopToLand()
{
	return !m_bAir;
}
bool CPlayer3D::WalkToRunF()
{
	return RunJumpLandToRunF() && m_fRunTime > 0.3f;
}
bool CPlayer3D::WalkToRunFR()
{
	return RunJumpLandToRunFR() && m_fRunTime > 0.3f;
}
bool CPlayer3D::WalkToRunFL()
{
	return RunJumpLandToRunFL() && m_fRunTime > 0.3f;
}
bool CPlayer3D::WalkToRunL()
{
	return RunJumpLandToRunL() && m_fRunTime > 0.3f;
}
bool CPlayer3D::WalkToRunB()
{
	return RunJumpLandToRunB() && m_fRunTime > 0.3f;
}
bool CPlayer3D::WalkToRunBR()
{
	return RunJumpLandToRunBR() && m_fRunTime > 0.3f;
}
bool CPlayer3D::WalkToRunBL()
{
	return RunJumpLandToRunBL() && m_fRunTime > 0.3f;
}
bool CPlayer3D::WalkToRunR()
{
	return RunJumpLandToRunR() && m_fRunTime > 0.3f;
}
bool CPlayer3D::IdleToDMG()
{
	return m_bAttacked;
}
void CPlayer3D::DamageEnd()
{
	m_bAttacked = false;
}
bool CPlayer3D::IdleToDown()
{
	return m_iHP <= 0;
}
bool CPlayer3D::LoopToDeadBody()
{
	return m_fDeadTime > 3.f;
}
bool CPlayer3D::IdleToSk_28_Start()
{
	return m_bCast;
}
bool CPlayer3D::Sk_28_LoopToEnd()
{
	return m_fCastTime >= m_fCastMax;
}
bool CPlayer3D::Sk_28ToWalk()
{
	return m_fX != 0.f || m_fZ != 0.f;
}
void CPlayer3D::DeadTimeStart()
{
	m_fDeadTime = 0.f;
}
void CPlayer3D::DeadBody()
{
	static_cast<CPaperBurn*>(m_pPaper.get())->ToggleBurn();
}
void CPlayer3D::CastFinish()
{
	m_fCastTime = 0.f;
	m_bCast = false;
	std::shared_ptr<CGameObject> pUI = m_pScene->FindLayer(UI_LAYER)->FindObj("UI");

	if (pUI)
	{
		static_cast<CUI*>(pUI.get())->SetCastingBar(false);
	}
}
void CPlayer3D::CastStart()
{
	std::shared_ptr<CGameObject> pUI = m_pScene->FindLayer(UI_LAYER)->FindObj("UI");

	if (pUI)
	{
		static_cast<CUI*>(pUI.get())->SetCastingBar(true);
	}

}
void CPlayer3D::CastCancel()
{
	CastFinish();
	
}
void CPlayer3D::AttackEnd(float fTime)
{
	m_bAttack = false;
}
void CPlayer3D::AttackStart()
{
}
void CPlayer3D::sk08Start()
{
}
void CPlayer3D::SetOutLine(bool bLine)
{
	if (m_pUpperOutLine)
	{
		m_pUpperOutLine->SetEnable(bLine);
	}
	if (m_pStaffOutLine)
	{
		m_pStaffOutLine->SetEnable(bLine);
	}
}
void CPlayer3D::SpawnRide()
{
	if (m_pRide)
	{
		if (!m_pRide->IsEnable())
		{
			m_pRide->SetEnable(true);
			m_pRide->SetWorldPos(GetWorldPos());

			CSceneComponent* pMesh = static_cast<CSceneComponent*>(m_pMesh.get());
			pMesh->SetWorldRotY(RadToDeg(m_fRot) + 180.f);

			Vector3 vRot = GetWorldRot();

			std::shared_ptr<CComponent> pRoot = m_pRide->GetRootComponent();

			std::shared_ptr<CTransform> pTrans = static_cast<CSceneComponent*>(pRoot.get())->GetTransform();

			pTrans->SetWorldRot(0.f, 0.f, 0.f);
			pTrans->SetQuaternionRotY(RadToDeg(m_fRot) + 180.f);

			static_cast<CRide*>(m_pRide.get())->SetPlayer();
		}

		else
		{
			SetWorldPos(GetBoneWorldPos());

			static_cast<CRide*>(m_pRide.get())->ExitPlayer();

			m_pRide->SetEnable(false);
		}
	}
}
void CPlayer3D::SetLayer(CLayer* pLayer)
{
	CGameObject::SetLayer(pLayer);

	if (m_pTargetParticle)
	{
		pLayer->AddObj(m_pTargetParticle);
	}
}
void CPlayer3D::SetScene(CScene* pScene)
{
	CGameObject::SetScene(pScene);
}
void CPlayer3D::CalcDamage(int iAttackMax, int iAttackMin)
{
	if (iAttackMax == iAttackMin)
		return;

	if (!m_bAttack)
	{
		m_bAttacked = true;
	}
	int iDamage = rand() % (iAttackMax - iAttackMin) + iAttackMin -
		(rand() % (m_iArmorMax - m_iArmorMin) + m_iArmorMin);

	iDamage = iDamage <= 0 ? 1 : iDamage;

	if (m_iHP > 0)
	{


		std::shared_ptr<CGameObject> pFont = m_pScene->CreateCloneObj("MonsterDamageFont", "MonsterDamageFont", m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

		if (pFont)
		{
			std::shared_ptr<CComponent> pCom = pFont->GetRootComponent();

			static_cast<CParticle*>(pCom.get())->SetCountX(iDamage);
			pFont->SetWorldPos(GetWorldPos() + Vector3(0.f, 50.f, 0.f));
		}
		m_iHP -= iDamage;
		if (m_iHP < 0)
		{
			m_iHP = 0;
		}

	}
}
void CPlayer3D::AttackEnd()
{
	m_bAttack = false;
}
// 스크린 스페이스 데칼