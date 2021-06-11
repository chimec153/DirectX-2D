#include "StartGameMode.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"
#include "Resource/ResourceManager.h"
#include "Resource/Material.h"
#include "Device.h"
#include "UI/UIButton.h"
#include "Engine.h"
#include "Component/Camera.h"
#include "CameraManager.h"
#include "Thread/ThreadManager.h"
#include "Scene/SceneManager.h"
#include "GameMode/LoadingMode3D.h"
#include "EditorFlags.h"
#include "../TownLoadingThread.h"
#include "Resource/ShaderManager.h"
#include "Resource/GraphicShader.h"
#include "Sound/SoundManager.h"
#include "Server/ClientSocket.h"
#include "UI/UFont.h"
#include "../Editor/Editor3D.h"
#include "UI/UIImage.h"

CStartGameMode::CStartGameMode()	:
	m_fTime(0.f)
{
	m_iGameModeType = static_cast<int>(EDITOR_GAME_MODE_TYPE::START);
}

CStartGameMode::~CStartGameMode()
{
}

bool CStartGameMode::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Button_Normal", false,
		"Button\\Button_Normal.ogg");

	if (!LoadShader())
		return false;
	if (!LoadTexture())
		return false;

	Resolution tRS = RESOLUTION;

	m_pTitle = m_pScene->CreateObject<CGameObject>("Title", m_pScene->FindLayer(ALBEDO_LAYER));

	m_pCam = m_pTitle->CreateComponent<CCamera>("Cam");

	CCamera* _pCam = static_cast<CCamera*>(m_pCam.get());

	GET_SINGLE(CCameraManager)->AddCam(_pCam);
	GET_SINGLE(CCameraManager)->SetMainCam("Cam");

	std::shared_ptr<CComponent> pTitleImg = m_pTitle->CreateComponent<CSpriteComponent>("TitleImg");

	m_pTitle->SetRootComponent(pTitleImg);

	CSpriteComponent* pImage = static_cast<CSpriteComponent*>(pTitleImg.get());
	
	pImage->SetWorldScale(1280, 400, 0.f);
	pImage->SetWorldPos(0,100.f, 0.f);
	pImage->SetPivot(0.5f, 0.5f, 0.f);
	pImage->GetMaterial()->SetTexture(LINK_TYPE::LT_DIF, "TitleImg");
	pImage->SetSize(Vector2(2048, 512));
	pImage->SetEnd(Vector2(1280, 400));
	pImage->SetImageType(IMAGE_TYPE::IT_ATLAS);	
	pImage->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	std::shared_ptr<CComponent> pLoginWindow = m_pTitle->CreateComponent<CUIImage>("LoginWindow");

	pImage->AddChild(pLoginWindow);

	static_cast<CUIImage*>(pLoginWindow.get())->SetTexture("Login");
	static_cast<CUIImage*>(pLoginWindow.get())->SetWorldScale(302.f,162.f, 1.f);
	static_cast<CUIImage*>(pLoginWindow.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pLoginWindow.get())->AddWorldPosY(-300.f);
	static_cast<CUIImage*>(pLoginWindow.get())->SetUV(Vector2(310.f, 1.f), Vector2(612.f, 163.f));
	static_cast<CUIImage*>(pLoginWindow.get())->SetSize(Vector2(1024.f, 256.f));

	std::shared_ptr<CComponent> pButton = m_pTitle->CreateComponent<CUIButton>("Button");

	pImage->AddChild(pButton);

	CUIButton* _pButton = static_cast<CUIButton*>(pButton.get());

	_pButton->SetWorldScale(130.f, 36.f, 1.f);
	_pButton->SetRelativePos(-65.f, -378.f, 0.f);
	_pButton->SetPivot(0.5f, 0.5f, 0.f);
	_pButton->SetCallBack<CStartGameMode>(BUTTON_STATE::PRESS, this,&CStartGameMode::Button );
	_pButton->SetTexture(BUTTON_STATE::OFF, "Buttons");
	_pButton->SetTexture(BUTTON_STATE::ON, "Buttons");
	_pButton->SetTexture(BUTTON_STATE::PRESS, "Buttons");	
	_pButton->SetUV(Vector2(703, 246), Vector2(831, 281));
	_pButton->SetSize(Vector2(1024, 512));

	std::shared_ptr<CComponent> pOKFont = m_pTitle->CreateComponent<CUFont>("LoginFont");

	_pButton->AddChild(pOKFont);

	static_cast<CUFont*>(pOKFont.get())->SetText("로그인");
	static_cast<CUFont*>(pOKFont.get())->SetRect(-55.f, 0.f, 55.f, 0.f);
	static_cast<CUFont*>(pOKFont.get())->SetSize(18.f);
	static_cast<CUFont*>(pOKFont.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::RIGHT);

	std::shared_ptr<CComponent> pDeleteButton = m_pTitle->CreateComponent<CUIButton>("DeleteButton");

	pImage->AddChild(pDeleteButton);

	static_cast<CUIButton*>(pDeleteButton.get())->SetTexture(BUTTON_STATE::OFF, "Buttons");
	static_cast<CUIButton*>(pDeleteButton.get())->SetTexture(BUTTON_STATE::ON, "Buttons");
	static_cast<CUIButton*>(pDeleteButton.get())->SetTexture(BUTTON_STATE::PRESS, "Buttons");
	static_cast<CUIButton*>(pDeleteButton.get())->SetWorldScale(130.f, 36.f, 1.f);
	static_cast<CUIButton*>(pDeleteButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(pDeleteButton.get())->SetSize(Vector2(1024, 512));
	static_cast<CUIButton*>(pDeleteButton.get())->SetUV(Vector2(315.f, 246.f), Vector2(443.f, 281.f));
	static_cast<CUIButton*>(pDeleteButton.get())->SetCallBack(BUTTON_STATE::PRESS, this, &CStartGameMode::Delete);
	static_cast<CUIButton*>(pDeleteButton.get())->SetRelativePos(65.f, -378.f, 0.f);

	std::shared_ptr<CComponent> pDeleteFont = m_pTitle->CreateComponent<CUFont>("DeleteFont");

	static_cast<CUIButton*>(pDeleteButton.get())->AddChild(pDeleteFont);

	static_cast<CUFont*>(pDeleteFont.get())->SetText("취소");
	static_cast<CUFont*>(pDeleteFont.get())->SetSize(18.f);
	static_cast<CUFont*>(pDeleteFont.get())->SetRect(-45.f, 0.f, 55.f, 0.f);
	static_cast<CUFont*>(pDeleteFont.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::LEFT);

	std::shared_ptr<CComponent> pInc = m_pTitle->CreateComponent<CSpriteComponent>("LoginInc");

	pImage->AddChild(pInc);

	CSpriteComponent* pIncCom = static_cast<CSpriteComponent*>(pInc.get());

	pIncCom->SetWorldScale(1280, 64, 0.f);
	pIncCom->SetWorldPos(0.f, -400.f, 0.f);
	pIncCom->SetPivot(0.5f, 0.5f, 0.f);
	pIncCom->GetMaterial()->SetTexture(LINK_TYPE::LT_DIF, "Inc");
	pIncCom->SetSize(Vector2(2048, 128));
	pIncCom->SetStart(Vector2(0, 0));
	pIncCom->SetEnd(Vector2(1280, 64));
	pIncCom->SetImageType(IMAGE_TYPE::IT_ATLAS);
	pIncCom->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	m_pTitle->SetEnable(false);

	m_pOpening = m_pScene->CreateObject<CGameObject>("Opening", m_pScene->FindLayer(ALBEDO_LAYER));

	std::shared_ptr<CComponent> pOpeningCom = m_pOpening->CreateComponent<CSpriteComponent>("Opening");

	m_pOpening->SetRootComponent(pOpeningCom);

	CSpriteComponent* pOpenCom = static_cast<CSpriteComponent*>(pOpeningCom.get());

	pOpenCom->SetWorldScale(1023, 256, 0.f);
	pOpenCom->SetWorldPos(0.f, 0.f, 0.f);
	pOpenCom->SetPivot(0.5f, 0.5f, 0.f);
	pOpenCom->GetMaterial()->SetTexture(LINK_TYPE::LT_DIF, "Opening");
	pOpenCom->SetSize(Vector2(1024, 1024));
	pOpenCom->SetStart(Vector2(0, 0));
	pOpenCom->SetEnd(Vector2(1023, 256));
	pOpenCom->SetImageType(IMAGE_TYPE::IT_ATLAS);
	pOpenCom->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	std::shared_ptr<CComponent> pOpeningI = m_pOpening->CreateComponent<CSpriteComponent>("OpeningI");

	pOpenCom->AddChild(pOpeningI);

	CSpriteComponent* pOpeningICom = static_cast<CSpriteComponent*>(pOpeningI.get());

	pOpeningICom->SetWorldScale(814.f, 233.f, 0.f);
	pOpeningICom->SetWorldPos(tRS.iWidth/2.f, -tRS.iHeight/ 2.f, 0.f);
	pOpeningICom->SetPivot(1.f, 0.f, 0.f);
	pOpeningICom->GetMaterial()->SetTexture(LINK_TYPE::LT_DIF, "Opening");
	pOpeningICom->SetSize(Vector2(1024.f, 1024.f));
	pOpeningICom->SetStart(Vector2(0.f, 257.f));
	pOpeningICom->SetEnd(Vector2(814.f, 489.f));
	pOpeningICom->SetImageType(IMAGE_TYPE::IT_ATLAS);
	pOpeningICom->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	std::shared_ptr<CGameObject> pFontObj = m_pScene->CreateObject<CGameObject>("LoginFont", m_pScene->FindLayer(UI_LAYER));

	m_pIDFont = pFontObj->CreateComponent<CUFont>("IDFont");

	static_cast<CUFont*>(m_pIDFont.get())->SetText(L"");
	static_cast<CUFont*>(m_pIDFont.get())->SetRect(-80.f, 0.f, 80.f, 0.f);
	static_cast<CUFont*>(m_pIDFont.get())->SetWorldPos(45.f, -178.f, 0.f);
	static_cast<CUFont*>(m_pIDFont.get())->SetSize(16.f);

	pFontObj->SetRootComponent(m_pIDFont);

	m_pPWFont = pFontObj->CreateComponent<CUFont>("PWFont");

	static_cast<CUFont*>(m_pPWFont.get())->SetText(L"");
	static_cast<CUFont*>(m_pPWFont.get())->SetRect(-80.f, 0.f, 80.f, 0.f);
	static_cast<CUFont*>(m_pPWFont.get())->SetSize(16.f);

	static_cast<CSceneComponent*>(m_pPWFont.get())->AddRelativePosY(-30.f);

	static_cast<CSceneComponent*>(m_pIDFont.get())->AddChild(m_pPWFont);

	return true;
}

bool CStartGameMode::LoadShader()
{
	

	return true;
}

void CStartGameMode::Update(float fTime)
{
	CGameMode::Update(fTime);

	m_fTime += fTime;

	if (m_fTime >= 4.f)
	{
		m_fTime -= FLT_MAX;
		m_pTitle->SetEnable(true);
		m_pOpening->SetEnable(false);
	}
}

bool CStartGameMode::LoadTexture()
{
	if (!GET_SINGLE(CResourceManager)->LoadTexture("TitleImg", TEXT("BLUILogin\\Texture2D\\BLUILogin_I9.dds")))
		return false;
	if (!GET_SINGLE(CResourceManager)->LoadTexture("Inc", TEXT("BLUILogin\\Texture2D\\BLUILogin_IC.dds")))
		return false;
	if (!GET_SINGLE(CResourceManager)->LoadTexture("Login", TEXT("BLUILogin\\Texture2D\\BLUILogin_IF.dds")))
		return false;
	if (!GET_SINGLE(CResourceManager)->LoadTexture("Opening", TEXT("BLUIOpening\\Texture2D\\BLUIOpening_I5.dds")))
		return false;
	if (!GET_SINGLE(CResourceManager)->LoadTexture("Buttons", TEXT("UI_Share_Button\\Texture2D\\UI2_Worldmap_lock_gray_Img.dds")))
		return false;
	if (!GET_SINGLE(CResourceManager)->LoadTexture("UI2_Loading_G08", 
		TEXT("LoadingUISetting\\Texture2D\\UI2_Loading_G08.dds")))
		return false;
	return true;
}

void CStartGameMode::Login(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Button_Normal");

	CScene* pScene = GET_SINGLE(CSceneManager)->CreateScene();

	if (pScene->SetGameMode<CLoadingMode3D>())
	{
		CGameMode* pGameMode = pScene->GetGameMode();

		static_cast<CLoadingMode3D*>(pGameMode)->SetGameModeType(EDITOR_GAME_MODE_TYPE::TOWN);
		static_cast<CLoadingMode3D*>(pGameMode)->SetBackTexture("UI2_Loading_G08");
		static_cast<CLoadingMode3D*>(pGameMode)->SetCreatePlayer();

		GET_SINGLE(CSceneManager)->SetNextScene(pScene);
	}
}

void CStartGameMode::Button(float fTime)
{
	GET_SINGLE(CEditor3D)->Login();
}

void CStartGameMode::Delete(float fTime)
{
}

void CStartGameMode::SetIDText(const TCHAR* pID)
{
	static_cast<CUFont*>(m_pIDFont.get())->SetText(pID);
}

void CStartGameMode::AddIDText(const TCHAR* w)
{
	static_cast<CUFont*>(m_pIDFont.get())->AddText(w);
}

void CStartGameMode::SetPWText(const TCHAR* pPW)
{
	static_cast<CUFont*>(m_pPWFont.get())->SetText(pPW);
}

void CStartGameMode::AddPWText(const TCHAR* w)
{
	static_cast<CUFont*>(m_pPWFont.get())->AddText(w);
}
