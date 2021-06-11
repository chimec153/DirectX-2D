#include "Player.h"
#include "Resource/ResourceManager.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Bullet.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Component/Camera.h"
#include "CameraManager.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderRect.h"
#include "Component/ColliderLine.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "UI/Bar.h"
#include "UI/UFont.h"
#include "UI/UIImage.h"
#include "Component/Navigation2D.h"
#include "NavigationManager.h"
#include "Tile.h"
#include "Device.h"
#include "Component/PaperBurn.h"
#include "../Socket/SocketManager.h"
#include "Component/Particle.h"

CPlayer::CPlayer() :
	m_pMesh(nullptr),
	m_pChildMesh(nullptr),
	m_pChildChildMesh(nullptr),
	m_pCam(nullptr),
	m_fSpeed(320.f),
	m_bPress(false),
	m_bIdleEnable(true),
	m_pBar(nullptr)
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::OCT_PLAYER;
}

CPlayer::CPlayer(const CPlayer& player) :
	CGameObject(player)
{
	*this = player;
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	m_pMesh = CreateComponent<CSpriteComponent>("Mesh");
	m_pChildMesh = CreateComponent<CSpriteComponent>("Mesh2");

	std::shared_ptr<CComponent> pLC = CreateComponent<CColliderLine>("Col");

	((CColliderLine*)pLC.get())->SetInheritScale(false);
	((CColliderLine*)pLC.get())->SetWorldScale(500.f, 50.f, 0.f);
	((CColliderLine*)pLC.get())->SetPivot(0.f, 0.5f, 0.f);
	//pLC->SetRelativePos(0.f, 50.f, 0.f);
	//pLC->SetExtend(40.f, 20.f);
	((CColliderLine*)pLC.get())->SetProFile("Player");
	((CColliderLine*)pLC.get())->SetCallBack<CPlayer>(COLLISION_STATE::CS_INIT, this, &CPlayer::ColInit);
	((CColliderLine*)pLC.get())->SetCallBack<CPlayer>(COLLISION_STATE::CS_STAY, this, &CPlayer::ColCon);
	((CColliderLine*)pLC.get())->SetCallBack<CPlayer>(COLLISION_STATE::CS_LAST, this, &CPlayer::ColEnd);

	((CSceneComponent*)m_pMesh.get())->AddChild(pLC);

	m_pChildChildMesh = CreateComponent<CSpriteComponent>("Mesh3");
	m_pCam = CreateComponent<CCamera>("camera");

	((CCamera*)m_pCam.get())->SetCameraType(CAMERA_TYPE::CT_2D);

	((CCamera*)m_pCam.get())->SetInheritScale(false);
	((CCamera*)m_pCam.get())->SetInheritRotX(false);
	((CCamera*)m_pCam.get())->SetInheritRotY(false);
	((CCamera*)m_pCam.get())->SetInheritRotZ(false);
	((CCamera*)m_pCam.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CCamera*)m_pCam.get())->SetRelativePos(0.f, 0.f, -200.f);

	((CSceneComponent*)m_pMesh.get())->AddChild(m_pCam);
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("Idle", "ARRTIdle", 1.f, true);
	//m_pMesh->SetTexture(LINK_TYPE::LT_DIF, "Exit", TEXT("Height\\5641.png"),TEXTURE_PATH);
/*
	m_pMesh->AddAni2DInfo("Idle", "MoneyIcon");
	m_pMesh->AddAni2DInfo("Run", "Burger");
	m_pMesh->AddAni2DInfo("Attack", "Info");*/

	((CSpriteComponent*)m_pMesh.get())->SetFunc<CPlayer>("Attack", this, &CPlayer::FireEnd);

	((CSpriteComponent*)m_pChildChildMesh.get())->SetInheritScale(false);

	((CSpriteComponent*)m_pChildChildMesh.get())->SetWorldScale(99.f, 108.f, 1.f);
	((CSpriteComponent*)m_pChildChildMesh.get())->SetRelativePos(-200.f, 0.f, 0.f);
	((CSpriteComponent*)m_pChildChildMesh.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CSpriteComponent*)m_pChildChildMesh.get())->SetTexture(LINK_TYPE::LT_DIF, "Exit", TEXT("Height\\5641.png"), TEXTURE_PATH);
	((CSpriteComponent*)m_pChildChildMesh.get())->AddAni2DInfo("Crooked House", "Crooked House");

	((CSpriteComponent*)m_pChildMesh.get())->AddChild(m_pChildChildMesh);
	((CSpriteComponent*)m_pChildMesh.get())->SetTexture(LINK_TYPE::LT_DIF, "Exit", TEXT("Height\\5641.png"), TEXTURE_PATH);
	((CSpriteComponent*)m_pChildMesh.get())->AddAni2DInfo("Idle", "ARRTIdleClient");

	SetRootComponent(m_pMesh);

	((CSceneComponent*)m_pMesh.get())->SetRelativePos(0.f, 0.f, 0.f);
	((CSceneComponent*)m_pMesh.get())->SetRelativeScale(500.f, 500.f, 1.f);
	((CSceneComponent*)m_pMesh.get())->SetPivot(0.5f, 0.5f, 0.f);

	//CColliderCircle* pRC = CreateComponent<CColliderCircle>("PlayerBody");

	//pRC->SetInheritScale(false);
	////pRC->SetExtend(100.f, 100.f, 0.f);
	//pRC->SetRadius(40.f);
	//pRC->SetProFile("Player");
	//m_pMesh->AddChild(pRC);

	//SAFE_RELEASE(pRC);

	//CColliderOBB2D* pOC = CreateComponent<CColliderOBB2D>("OBBBody");

	//pOC->SetExtend(50.f, 20.f, 0.f);
	//pOC->SetProFile("Player");

	//m_pMesh->AddChild(pOC);

	//SAFE_RELEASE(pOC);

	//CColliderPixel* pPC = CreateComponent<CColliderPixel>("PlayerCol");

	//pPC->LoadInfo(TEXT("test.bmp"));
	//pPC->SetPixelType(PIXEL_COLLISION_TYPE::COLOR_IGNORE);
	//pPC->SetIgnoreColor(0, 12, 0);
	////pPC->SetAlpha(10);
	//pPC->SetProFile("Player");

	//m_pMesh->AddChild(pPC);

	//SAFE_RELEASE(pPC);

	((CSpriteComponent*)m_pChildMesh.get())->SetRelativePos(0.f, 0.f, 0.f);
	((CSpriteComponent*)m_pChildMesh.get())->SetInheritScale(false);
	((CSpriteComponent*)m_pChildMesh.get())->SetRelativeScale(133.f, 119.f, 1.f);
	((CSpriteComponent*)m_pChildMesh.get())->SetPivot(0.5f, 0.5f, 0.f);

	((CSpriteComponent*)m_pMesh.get())->AddChild(m_pChildMesh);

	std::shared_ptr<CComponent> pImg = CreateComponent<CUIImage>("Back");

	((CUIImage*)pImg.get())->SetWorldScale(60.f, 10.f, 0.f);
	((CUIImage*)pImg.get())->SetInheritScale(false);
	((CUIImage*)pImg.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIImage*)pImg.get())->SetRelativePos(0.f, 25.f, 0.f);
	((CUIImage*)pImg.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_2D);
	((CUIImage*)pImg.get())->SetTexture("Back");

	m_pBar = CreateComponent<CBar>("Bar");

	((CBar*)m_pBar.get())->SetWorldScale(57.f, 8.f, 0.f);
	((CBar*)m_pBar.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CBar*)m_pBar.get())->SetInheritScale(false);
	((CBar*)m_pBar.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_2D);
	((CBar*)m_pBar.get())->SetTexture("Bar");

	((CUIImage*)pImg.get())->AddChild(m_pBar);

	((CSpriteComponent*)m_pMesh.get())->AddChild(pImg);

	std::shared_ptr<CComponent> pFont = CreateComponent<CUFont>("Name");

	((CUFont*)pFont.get())->SetText(TEXT("Player"));
	((CUFont*)pFont.get())->SetColor(0.7f, 0.15f, 0.7f, 1.f);
	((CUFont*)pFont.get())->SetShadowColor(0.4f, 0.4f, 0.4f, 0.4f);
	((CUFont*)pFont.get())->SetShadowOffset(Vector3(1.f, 1.f, 0.f));
	((CUFont*)pFont.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_2D);
	((CUFont*)pFont.get())->SetRelativePos(0.f, 70.f, 0.f);
	((CUFont*)pFont.get())->SetRect(-40.f, 50.f, 100.f, 0.f);
	((CUFont*)pFont.get())->SetShadowAlpha(true);
	((CUFont*)pFont.get())->SetShadowOpacity(0.5f);

	((CSpriteComponent*)m_pMesh.get())->AddChild(pFont);

	m_pNav = CreateComponent<CNavigation2D>("Nav");

	m_pPaper = CreateComponent<CPaperBurn>("PaperBurn");

	//static_cast<CSceneComponent*>(m_pMesh.get())->AddChild(pPaper);

	static_cast<CSpriteComponent*>(m_pMesh.get())->GetMaterial()->SetShader("PaperBurn2D");

	//std::shared_ptr<CComponent> pParticle = CreateComponent<CParticle>("Particle");

	////static_cast<CParticle*>(pParticle.get())->Create();
	//static_cast<CSceneComponent*>(pParticle.get())->SetWorldScale(50.f, 50.f, 0.f);
	//static_cast<CSceneComponent*>(pParticle.get())->SetInheritScale(false);
	//static_cast<CSceneComponent*>(pParticle.get())->GetMaterial()->SetTexture(LINK_TYPE::LT_DIF, "Exit", (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	//CParticle* _pParticle = static_cast<CParticle*>(pParticle.get());

	//_pParticle->SetMaxScale(Vector3(30.f, 30.f, 0.f));
	//_pParticle->SetMinScale(Vector3(0.f, 0.f, 0.f));
	//_pParticle->SetMaxColor(Vector4(1.f, 0.12f, 1.f, 1.f));
	//_pParticle->SetMinColor(Vector4(0.f, 0.12f, 1.f, 0.f));
	//_pParticle->SetMaxTime(1.f);
	//_pParticle->SetMinTime(0.1f);
	//_pParticle->SetMove(true);
	//_pParticle->SetMaxSpeed(100.f);
	//_pParticle->SetMinSpeed(100.f);
	//_pParticle->SetAccel(Vector3(0.f, -100.f, 0.f));
	//_pParticle->SetRange(Vector3(100.f, 100.f, 0.f));
	//_pParticle->SetMaxCount(512);

	//static_cast<CSceneComponent*>(m_pMesh.get())->AddChild(pParticle);

	return true;
}

void CPlayer::Start()
{
	CGameObject::Start();

	m_pCam = FindComponentByType<CCamera>();

	GET_SINGLE(CCameraManager)->SetMainCam(m_pCam);
	GET_SINGLE(CCameraManager)->AddCam(m_pCam);

	AddAxisBind("MoveSide", this, &CPlayer::MoveSide);
	AddAxisBind("MoveVertical", this, &CPlayer::MoveVertical);
	AddAxisBind("Rotate", this, &CPlayer::Rotate);
	AddAxisBind("FireCon", this, &CPlayer::FireCon);

	AddActionBind("fire", this, &CPlayer::Fire, KT_DOWN);
	AddActionBind("fire2", this, &CPlayer::Fire2, KT_DOWN);
	AddActionBind("fire3", this, &CPlayer::Fire3, KT_DOWN);
	AddActionBind("Color", this, &CPlayer::Color, KT_DOWN);
	AddActionBind("X", this, &CPlayer::Paper, KT_DOWN);
}

void CPlayer::Input(float fTime)
{
	CGameObject::Input(fTime);

	if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::RIGHT))
	{
		Vector2 vRatio = RATIO;

		GET_SINGLE(CSocketManager)->Move(GET_SINGLE(CInput)->GetMouseWorldPos() / vRatio);

	}

}

void CPlayer::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (((CSpriteComponent*)m_pMesh.get())->GetVelocityAmount() != 0)
	{
		m_bIdleEnable = true;
		((CSpriteComponent*)m_pMesh.get())->ChangeClip("Run");
	}

	else if (m_bIdleEnable)
		((CSpriteComponent*)m_pMesh.get())->ChangeClip("Idle");

	if (!m_stack.empty())
	{
		CTile* pTile = m_stack.top();

		Vector3 vTilePos = pTile->GetWorldPos();
		Vector3 vPos = GetWorldPos() - GetPivot() * GetWorldScale();

		vTilePos -= vPos;

		float fLength = vTilePos.Length();

		vTilePos /= fLength;

		Vector3 vPlayerToDest = Vector3(m_vEndPos.x, m_vEndPos.y, 0.f) - vPos;

		vPlayerToDest.Normalize();

		float fAngle = RadToDeg(acosf(vPlayerToDest.Dot(vTilePos)));

		if (fAngle > 90.f)
		{
			m_stack.pop();
		}
		else if (fLength <= 10.f)
		{
			m_stack.pop();
		}
		else
		{
			AddWorldPos(vTilePos * 200.f * fTime);
		}
	}
	else
	{
		((CNavigation2D*)m_pNav.get())->Clear();
	}
}

void CPlayer::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CPlayer::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CPlayer::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CPlayer::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CPlayer::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CPlayer::Clone()
{
	return std::shared_ptr<CGameObject>(new CPlayer(*this));
}

void CPlayer::MoveSide(float fScale, float fTime)
{
	((CSpriteComponent*)m_pMesh.get())->AddRelativePos(((CSpriteComponent*)m_pMesh.get())->GetRelativeAxis(AXIS_X) * m_fSpeed * fTime * fScale);
}

void CPlayer::MoveVertical(float fScale, float fTime)
{
	((CSpriteComponent*)m_pMesh.get())->AddRelativePos(((CSpriteComponent*)m_pMesh.get())->GetRelativeAxis(AXIS_Y) * m_fSpeed * fTime * fScale);
}

void CPlayer::Rotate(float fScale, float fTime)
{
	((CSpriteComponent*)m_pMesh.get())->AddRelativeRotZ(180.f * fTime * fScale);
}

void CPlayer::FireCon(float fScale, float fTime)
{
	if (fScale != 0.f)
	{
		std::shared_ptr<CGameObject> pBullet = m_pScene->CreateCloneObj("Bullet", "Bullet", m_pLayer, m_pScene->GetSceneType());

		pBullet->SetRelativeRot(GetWorldRot());
		pBullet->SetRelativePos(GetWorldPos());
		pBullet->AddRelativePos(pBullet->GetWorldAxis(AXIS_Y) *
			(50.f + pBullet->GetWorldScale().y * pBullet->GetPivot().y));

		Vector3 vPos = pBullet->GetWorldPos();
		Vector3 vAxisX = pBullet->GetWorldAxis(AXIS_X);
		Vector3 vAxisY = pBullet->GetWorldAxis(AXIS_Y);

		Vector3 pPt[] =
		{
			vPos - vAxisX * 200.f - vAxisY * 100.f,
			vPos,
			vPos + vAxisX * 200.f + vAxisY * 100.f,
			vPos - vAxisX * 200.f + vAxisY * 200.f,
			vPos + vAxisX * 300.f + vAxisY * 300.f,
			vPos - vAxisX * 300.f + vAxisY * 400.f
		};

		((CBullet*)pBullet.get())->SetPt(pPt);
	}
}

void CPlayer::Fire(float fTime)
{
	m_bIdleEnable = false;
	((CSpriteComponent*)m_pMesh.get())->ChangeClip("Attack");
}

void CPlayer::Fire2(float fTime)
{
	float fDist = GetWorldScale().y * GetPivot().y;

	for (int i = 0; i < 3; ++i)
	{
		std::shared_ptr<CGameObject> pBullet = m_pScene->CreateCloneObj("Bullet", "Bullet", m_pLayer, m_pScene->GetSceneType());

		Vector3 vRot = GetWorldRot();

		vRot.z += 30.f * (i - 1);

		pBullet->SetRelativeRot(vRot);
		pBullet->SetRelativePos(GetWorldPos());
		pBullet->AddRelativePos(pBullet->GetWorldAxis(AXIS_Y) * (fDist + pBullet->GetWorldScale().y * pBullet->GetPivot().y));
		/*
				CGameObject* pObj = m_pScene->CreateCloneObj("Effect", "Effect", m_pLayer, m_pScene->GetSceneType());

				vRot.z += 30.f * (i - 5);

				pObj->SetRelativeRot(vRot);
				pObj->SetRelativePos(GetWorldPos());
				pObj->AddRelativePos(pObj->GetWorldAxis(AXIS_Y) * 150.f);

				SAFE_RELEASE(pObj);*/
		Vector3 vPos = pBullet->GetWorldPos();
		Vector3 vAxisX = pBullet->GetWorldAxis(AXIS_X);
		Vector3 vAxisY = pBullet->GetWorldAxis(AXIS_Y);

		Vector3 pPt[] =
		{
			vPos - vAxisX * 200.f - vAxisY * 100.f,
			vPos,
			vPos + vAxisX * 200.f + vAxisY * 100.f,
			vPos - vAxisX * 200.f + vAxisY * 200.f,
			vPos + vAxisX * 300.f + vAxisY * 300.f,
			vPos - vAxisX * 300.f + vAxisY * 400.f
		};

		((CBullet*)pBullet.get())->SetPt(pPt);
	}
}

void CPlayer::Fire3(float fTime)
{
	float fDist = GetWorldScale().y * GetPivot().y;

	for (int i = 0; i < 16; ++i)
	{
		std::shared_ptr<CGameObject> pBullet = m_pScene->CreateCloneObj("Bullet", "Bullet", m_pLayer, m_pScene->GetSceneType());

		Vector3 vRot = GetWorldRot();

		vRot.z += 360.f / 16.f * (i - 1);

		pBullet->SetRelativeRot(vRot);
		pBullet->SetRelativePos(GetWorldPos());
		pBullet->AddRelativePos(pBullet->GetWorldAxis(AXIS_Y) * (fDist + pBullet->GetWorldScale().y * pBullet->GetPivot().y));

		Vector3 vPos = pBullet->GetWorldPos();
		Vector3 vAxisX = pBullet->GetWorldAxis(AXIS_X);
		Vector3 vAxisY = pBullet->GetWorldAxis(AXIS_Y);

		Vector3 pPt[] =
		{
			vPos - vAxisX * 200.f - vAxisY * 100.f,
			vPos,
			vPos + vAxisX * 200.f + vAxisY * 100.f,
			vPos - vAxisX * 200.f + vAxisY * 200.f,
			vPos + vAxisX * 300.f + vAxisY * 300.f,
			vPos - vAxisX * 300.f + vAxisY * 400.f
		};

		((CBullet*)pBullet.get())->SetPt(pPt);

	}
}

void CPlayer::Down(float fTime)
{
	m_bPress = true;
}

void CPlayer::Up(float fTime)
{
	m_bPress = false;
}

void CPlayer::FireEnd()
{
	m_bIdleEnable = true;
}

void CPlayer::CallNotify(const std::string& strTag)
{
	if (strTag == "Attack")
	{
		std::shared_ptr<CGameObject> pBullet = m_pScene->CreateCloneObj("Bullet", "Bullet", m_pLayer, m_pScene->GetSceneType());

		pBullet->SetRelativeRot(GetWorldRot());
		pBullet->SetRelativePos(GetWorldPos());
		pBullet->AddRelativePos(pBullet->GetWorldAxis(AXIS_Y) *
			(GetWorldScale().y * GetPivot().y + pBullet->GetWorldScale().y * pBullet->GetPivot().y));
	}
}

void CPlayer::Color(float fTime)
{
	((CSpriteComponent*)m_pMesh.get())->LED(true);
}

void CPlayer::Paper(float fTime)
{
	m_pPaper->SetEnable(true);
	static_cast<CPaperBurn*>(m_pPaper.get())->ToggleBurn();
}

void CPlayer::Move(const Vector2& vPos)
{
	m_vEndPos = vPos;

	CLayer* pBack = m_pScene->FindLayer("Default");

	std::shared_ptr<CGameObject> pObj = pBack->FindObj("TileMap");

	std::shared_ptr<CComponent> pCom = pObj->FindComponent("TileMap");

	((CNavigation2D*)m_pNav.get())->SetTileMap(pCom);

	if (((CNavigation2D*)m_pNav.get())->FindPath(GetWorldPos(), Vector3(m_vEndPos.x, m_vEndPos.y, 0.f)))
	{
		m_stack = ((CNavigation2D*)m_pNav.get())->GetStack();
	}
}

void CPlayer::Save(FILE* pFile)
{
	CGameObject::Save(pFile);

	std::string strName = m_pMesh->GetName();

	int iLength = (int)strName.length();

	fwrite(&iLength, sizeof(int), 1, pFile);

	fwrite(strName.c_str(), sizeof(char), iLength, pFile);

	strName = m_pChildMesh->GetName();

	iLength = (int)strName.length();

	fwrite(&iLength, sizeof(int), 1, pFile);

	fwrite(strName.c_str(), sizeof(char), iLength, pFile);

	strName = m_pChildChildMesh->GetName();

	iLength = (int)strName.length();

	fwrite(&iLength, sizeof(int), 1, pFile);

	fwrite(strName.c_str(), sizeof(char), iLength, pFile);

	fwrite(&m_fSpeed, sizeof(float), 1, pFile);

	fwrite(&m_bPress, 1, 1, pFile);

	fwrite(&m_bIdleEnable, 1, 1, pFile);
}

void CPlayer::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	int iLength = 0;

	char strName[256] = {};

	fread(&iLength, sizeof(int), 1, pFile);
	if (iLength > 0)
	{
		fread(strName, sizeof(char), iLength, pFile);
	}

	std::string strTag = strName;

	m_pMesh = std::shared_ptr<CComponent>(FindComponent(strTag));

	fread(&iLength, sizeof(int), 1, pFile);

	memset(strName, 0, 256);

	if (iLength > 0)
	{
		fread(strName, sizeof(char), iLength, pFile);
	}

	strTag = strName;

	m_pChildMesh = std::shared_ptr<CComponent>(FindComponent(strTag));

	fread(&iLength, sizeof(int), 1, pFile);

	memset(strName, 0, 256);

	if (iLength > 0)
	{
		fread(strName, sizeof(char), iLength, pFile);
	}

	strTag = strName;

	m_pChildChildMesh = std::shared_ptr<CComponent>(FindComponent(strTag));

	fread(&m_fSpeed, sizeof(float), 1, pFile);

	fread(&m_bPress, 1, 1, pFile);

	fread(&m_bIdleEnable, 1, 1, pFile);

	m_pBar = FindComponentByType<CBar>();
}

void CPlayer::ColInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (pDest->GetName() == "MonsterBody")
		Fire2(0.f);
}

void CPlayer::ColCon(CCollider* pSrc, CCollider* pDest, float fTime)
{

}

void CPlayer::ColEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (pDest->GetName() == "MonsterBody")
		Fire3(0.f);
}
