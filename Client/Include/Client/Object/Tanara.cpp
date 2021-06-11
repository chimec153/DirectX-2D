#include "Tanara.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"
#include "Component/ColliderSphere.h"
#include "Scene/Scene.h"
#include "Component/Camera.h"
#include "../../Shop.h"
#include "CameraManager.h"
#include "Sound/SoundManager.h"
#include "UI/UFont.h"

CTanara::CTanara()	:
	m_pShopUI(nullptr)
	, m_pCam(nullptr)
	, m_pPrevCam(nullptr)
{
}

CTanara::CTanara(const CTanara& npc)	:
	CNPC(npc)
	, m_pShopUI(npc.m_pShopUI)
	, m_pCam(FindComponentByType<CCamera>())
	, m_pPrevCam(npc.m_pPrevCam)
{
	if (m_pShopUI)
		m_pShopUI = m_pShopUI->Clone();

}

CTanara::~CTanara()
{
}

bool CTanara::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "HighElfF_Y1_12", false,
		"Village\\HighElfF_Y1_12.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "HighElfF_Y1_05", false,
		"Village\\HighElfF_Y1_05.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "HighElfF_Y1_02", false,
		"Village\\HighElfF_Y1_02.ogg");

	if(!CNPC::Init())
	return false;


	std::shared_ptr<CComponent> pRenderer = CreateComponent<CSceneComponent>("Renderer");

	SetRootComponent(pRenderer);

	SetShadowAll(true);

	static_cast<CSceneComponent*>(pRenderer.get())->SetMesh("AN_NPC_Tanara_01");

	m_pAni = CAnimation::CreateAnimation<CAnimation>("TanaraAni", "TanaraBone", "Town\\AN_NPC_Tanara_01.bne");

	m_pAni->SetSkeleton("TanaraBone");
	m_pAni->AddAnimationSequence("Idle", "AN_NPC_Tanara_01");

	static_cast<CSceneComponent*>(pRenderer.get())->SetAnimation(m_pAni);

	std::shared_ptr<CComponent> pCollider = CreateComponent<CColliderSphere>("NPCBody");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pCollider);

	static_cast<CColliderSphere*>(pCollider.get())->SetRadius(12.f);
	static_cast<CColliderSphere*>(pCollider.get())->SetRelativePosY(20.f);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CNPC>(COLLISION_STATE::CS_INIT, this, &CNPC::CollisionInit);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CNPC>(COLLISION_STATE::CS_STAY, this, &CNPC::CollisionStay);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CNPC>(COLLISION_STATE::CS_END, this, &CNPC::CollisionEnd);

	m_pShopUI = m_pScene->CreateObject<CShop>("ShopUI", m_pScene->FindLayer(UI_LAYER));

	m_pShopUI->SetEnable(false);

	static_cast<CShop*>(m_pShopUI.get())->SetItem(0, "엔젤 스태프");
	static_cast<CShop*>(m_pShopUI.get())->SetItem(1, "쓸데없이 큰 지팡이");
	static_cast<CShop*>(m_pShopUI.get())->SetItem(2, "말 지팡이");
	static_cast<CShop*>(m_pShopUI.get())->SetItem(3, "디아블로 스태프");
	static_cast<CShop*>(m_pShopUI.get())->SetItem(4, "홀리 스태프"); 
	static_cast<CShop*>(m_pShopUI.get())->SetItem(5, "롱소드");
	static_cast<CShop*>(m_pShopUI.get())->SetItem(6, "그레이트 소드");
	static_cast<CShop*>(m_pShopUI.get())->SetItem(7, "라이트 소드");

	m_pCam = CreateComponent<CCamera>("TanaraCamera");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(m_pCam);

	static_cast<CCamera*>(m_pCam.get())->SetInheritRotX(false);
	static_cast<CCamera*>(m_pCam.get())->SetInheritRotY(false);
	static_cast<CCamera*>(m_pCam.get())->SetInheritRotZ(false);
	static_cast<CCamera*>(m_pCam.get())->AddRelativePos(14.f, 32.f, 2.f);
	static_cast<CCamera*>(m_pCam.get())->SetRelativeRotY(-60.f);

	SetWorldPos(2619.939f, 15.250f, 5455.663f);

	m_pName = CreateComponent<CUFont>("NameFont");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(m_pName);

	static_cast<CUFont*>(m_pName.get())->SetRelativePosY(GetMeshSize().y * 1.1f + 10.f);
	static_cast<CUFont*>(m_pName.get())->SetRect(-100.f, 0.f, 100.f, 0.f);
	static_cast<CUFont*>(m_pName.get())->SetText("타나라");
	static_cast<CUFont*>(m_pName.get())->Set3D();
	static_cast<CUFont*>(m_pName.get())->SetShadowOffset(Vector3(2.f, -2.f, 0.f));
	static_cast<CUFont*>(m_pName.get())->SetShadowColor(0.f, 0.f, 0.f, 1.f);
	return true;
}

void CTanara::Start()
{
	CNPC::Start();
}

void CTanara::Update(float fTime)
{
	CNPC::Update(fTime);
}

void CTanara::PostUpdate(float fTime)
{
	CNPC::PostUpdate(fTime);
}

void CTanara::Collision(float fTime)
{
	CNPC::Collision(fTime);
}

void CTanara::PreRender(float fTime)
{
	CNPC::PreRender(fTime);
}

void CTanara::Render(float fTime)
{
	CNPC::Render(fTime);
}

void CTanara::PostRender(float fTime)
{
	CNPC::PostRender(fTime);
}

std::shared_ptr<CGameObject> CTanara::Clone()
{
	return std::shared_ptr<CGameObject>(new CTanara(*this));
}

void CTanara::Save(FILE* pFile)
{
	CNPC::Save(pFile);
}

void CTanara::Load(FILE* pFile)
{
	CNPC::Load(pFile);
}

void CTanara::NPCWork()
{
	if (!m_pPrevCam)
	{
		m_pPrevCam = GET_SINGLE(CCameraManager)->GetMainCam();

		GET_SINGLE(CCameraManager)->SetMainCam("TanaraCamera");

		m_pShopUI->SetEnable(true);

		GET_SINGLE(CSoundManager)->Play("HighElfF_Y1_02");
	}
}

void CTanara::Close()
{
	if (m_pPrevCam)
	{
		GET_SINGLE(CCameraManager)->SetMainCam(m_pPrevCam);

		m_pPrevCam = nullptr;

		m_pShopUI->SetEnable(false);
		GET_SINGLE(CSoundManager)->Play("HighElfF_Y1_12");
	}
}
