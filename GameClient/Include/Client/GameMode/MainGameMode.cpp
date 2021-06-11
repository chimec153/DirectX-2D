#include "MainGameMode.h"
#include "../Object/Player.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Texture.h"
#include "../Object/Minion.h"
#include "../Object/Bullet.h"
#include "../Object/MinionBullet.h"
#include "../UI/ClientButton.h"
#include "../Object/Effect.h"
#include "../UI/Br.h"
#include "Resource/ResourceManager.h"
#include "../UI/CheckBox.h"
#include "../UI/RadioBox.h"
#include "../UI/UIPanel.h"
#include "../UI/SliderControl.h"
#include "../Object/Marry.h"
#include "UI/Panel.h"
#include "UI/RadioButton.h"
#include "UI/ChkBx.h"
#include "UI/Slider.h"
#include "UI/UIButton.h"
#include "UI/UIList.h"
#include "../Object/TileObj.h"
#include "Component/TileMap.h"
#include "UI/UFont.h"
//#include "Timer.h"
#include "RenderManager.h"
#include "Device.h"
#include "UI/UIImage.h"
#include "../UI/RideWindow.h"
#include "../UI/ConstructionWindow.h"
#include "Scene/SceneManager.h"
#include "StartGameMode.h"
//#include "../../ViewManager.h"
#include "PathManager.h"
#include "../Object/Coaster.h"
#include "LoadingMode.h"
#include "../UI/Br.h"

std::shared_ptr<CGameObject> CMainGameMode::m_pContructWindow = nullptr;
std::shared_ptr<CGameObject> CMainGameMode::m_pRideWindow = nullptr;

CMainGameMode::CMainGameMode()
{
	m_iGameModeType = (int)GAMEMODE_CLASS_TYPE::GCT_MAIN;
}

CMainGameMode::~CMainGameMode()
{
}

bool CMainGameMode::Init()
{
	LoadTexture();
	LoadAnimation2D();

	CreateProto();

	CLayer* pLayer = m_pScene->FindLayer("Default");

	std::shared_ptr<CGameObject> pPlayer = m_pScene->CreateObject<CPlayer>("Player", pLayer);

	SetPlayer(pPlayer);

	std::shared_ptr<CGameObject> pMinion = m_pScene->CreateObject<CMinion>("Minion", pLayer);

	std::shared_ptr<CGameObject> pMar = m_pScene->CreateObject<CMarry>("Marry", pLayer);

	std::shared_ptr<CGameObject> pCoaster = m_pScene->CreateObject<CCoaster>("Coaster", pLayer);

	std::shared_ptr<CGameObject> pCoaster2 = m_pScene->CreateObject<CCoaster>("Coaster2", pLayer);

	((CCoaster*)pCoaster2.get())->SetDist(60.f);

	std::shared_ptr<CGameObject> pCoaster3 = m_pScene->CreateObject<CCoaster>("Coaster3", pLayer);

	((CCoaster*)pCoaster3.get())->SetDist(120.f);

	std::shared_ptr<CGameObject> pCoaster4 = m_pScene->CreateObject<CCoaster>("Coaster4", pLayer);

	((CCoaster*)pCoaster4.get())->SetDist(180.f);

	std::shared_ptr<CGameObject> pCoaster5 = m_pScene->CreateObject<CCoaster>("Coaster5", pLayer);

	((CCoaster*)pCoaster5.get())->SetDist(240.f);

	std::shared_ptr<CGameObject> pCoaster6 = m_pScene->CreateObject<CCoaster>("Coaster5", pLayer);

	((CCoaster*)pCoaster6.get())->SetDist(300.f);

	if (!SetUI())
		return false;

	std::shared_ptr<CGameObject> pTile = m_pScene->CreateObject<CTileObj>("TileMap",pLayer);

	std::shared_ptr<CComponent> pMap = pTile->CreateComponent<CTileMap>("TileMap");

	((CTileMap*)pMap.get())->CreateTile(TILE_TYPE::ISOMETRIC, 100, 100, Vector3::Zero, Vector2(64.f, 31.f), TILE_OPTION::NONE, 0);

	((CTileMap*)pMap.get())->SetTileTexture(IMAGE_TYPE::IT_ARRAY, "Tile");

	((CTileObj*)pTile.get())->SetRootComponent(pMap);
	
	return true;
}

bool CMainGameMode::SetUI()
{

	CLayer* pUI = m_pScene->FindLayer("UI");
/*
	CInventory* pInv = m_pScene->CreateObject<CInventory>("Inventory", pUI);

	pInv->SetWorldPos(200.f, 400.f, 0.f);

	SAFE_RELEASE(pInv);*/

	std::shared_ptr<CGameObject> pBtn = m_pScene->CreateObject<CClientButton>("BackButton", pUI);

	((CClientButton*)pBtn.get())->SetTexture(BUTTON_STATE::OFF, "StartBtn");
	((CClientButton*)pBtn.get())->SetTexture(BUTTON_STATE::ON, "StartBtn");
	((CClientButton*)pBtn.get())->SetTexture(BUTTON_STATE::PRESS, "StartBtn");

	((CClientButton*)pBtn.get())->SetColor(BUTTON_STATE::OFF, 0.5f, 0.5f, 0.5f, 1.f);
	((CClientButton*)pBtn.get())->SetColor(BUTTON_STATE::ON, 1, 1, 1, 1.f);
	((CClientButton*)pBtn.get())->SetColor(BUTTON_STATE::PRESS, 0.8f, 0.8f, 0.8f, 1.f);

	pBtn->SetPivot(0.5f, 0.5f, 0.f);

	//pBtn->SetSound(BUTTON_STATE::OFF, SOUND_TYPE::EFFECT, "Click1");
	//pBtn->SetSound(BUTTON_STATE::ON, SOUND_TYPE::EFFECT, "Click1");
	((CClientButton*)pBtn.get())->SetSound(BUTTON_STATE::PRESS, SOUND_TYPE::EFFECT, "Click1");
	((CClientButton*)pBtn.get())->SetClickCallBack<CMainGameMode>(this, BUTTON_STATE::PRESS,&CMainGameMode::StartScene);

	pBtn->SetWorldScale(58.f, 78.f, 1.f);
	pBtn->SetWorldPos(500.f, 300.f, 0.f);

	std::shared_ptr<CGameObject> pBox = m_pScene->CreateObject<CCheckBox>("Box", pUI);

	std::shared_ptr<CGameObject> pBr = m_pScene->CreateObject<CBr>("Bar", pUI);

	//CreatePanel();
	CreateButton();

	m_pRideWindow = m_pScene->CreateObject<CRideWindow>("RideWindow", pUI);

	m_pRideWindow->SetEnable(false);

	m_pContructWindow = m_pScene->CreateObject<CConstructionWindow>("ConstructionWindow", pUI);

	m_pContructWindow->SetEnable(false);

	return true;
}

void CMainGameMode::CreateButton()
{
	Resolution tRS = RESOLUTION;

	CLayer* pUI = m_pScene->FindLayer("UI");

	const char* ppName[] =
	{
		"StopButton",
		"StopButtonPush",
		"SaveButton",
		"SaveButtonPush",
		"SmallButton",
		"SmallButtonPush",
		"SmallButtonDis", 
		"BigButton", 
		"BigButtonPush", 
		"BigButtonDis", 
		"TurnRightButton", 
		"TurnRightButtonPush",
		"DigButton", 
		"DigButtonPush", 
		"MapButton", 
		"MapButtonPush", 
		"CustomerButton", 
		"CustomerButtonPush", 
		"ThemeButton",
		"ThemeButtonPush", 
		"WaterButton", 
		"WaterButtonPush",
		"RideButton", 
		"RideButtonPush", 
		"ViewButton", 
		"ViewButtonPush", 
		"PathButton", 
		"PathButtonPush", 
		"EmployeeButton", 
		"EmployeeButtonPush",
		"EmployeeIcon", 
		"EmployeeIconPush", 
		"EnterenceButton", 
		"EnterenceButtonPush", 
		"RollercoasterButton", 
		"RollercoasterButtonPush",
		"DeleteButton", 
		"DeleteButtonPush"
	};

	int j = 0;

	for (int i = 0; i < 17; ++i)
	{
		std::shared_ptr<CGameObject> pButton = m_pScene->CreateObject<CClientButton>(ppName[2*i + j], pUI);

		((CClientButton*)pButton.get())->SetTexture(BUTTON_STATE::OFF, ppName[2 * i + j]);
		((CClientButton*)pButton.get())->SetTexture(BUTTON_STATE::ON, ppName[2 * i + j]);
		((CClientButton*)pButton.get())->SetTexture(BUTTON_STATE::PRESS, ppName[2 * i + 1 + j]);

		if (i == 2 || i == 3)
		{
			((CClientButton*)pButton.get())->SetTexture(BUTTON_STATE::DISABLE, ppName[2 * i + 2 + j++]);
		}

		else if (i == 13)
		{
			((CClientButton*)pButton.get())->SetUIImage();
			((CClientButton*)pButton.get())->SetImgTexture(BUTTON_STATE::OFF, ppName[2 * i + 2 + j]);
			((CClientButton*)pButton.get())->SetImgTexture(BUTTON_STATE::ON, ppName[2 * i + 2 + j++]);
			((CClientButton*)pButton.get())->SetImgTexture(BUTTON_STATE::PRESS, ppName[2 * i + 2 + j++]);

			std::shared_ptr<CComponent> pImg = pButton->FindComponentByType<CUIImage>();

			((CSceneComponent*)pImg.get())->SetPivot(0.5f, 0.5f, 0.f);
			((CSceneComponent*)pImg.get())->SetWorldScale(26.f, 25.f, 0.f);
			((CSceneComponent*)pImg.get())->SetInheritScale(false);
		}

		else if (i == 15)
		{
			CUIButton* pBtn  = ((CClientButton*)pButton.get())->GetButton();

			pBtn->SetCallBack(BUTTON_STATE::PRESS, CMainGameMode::ShowRideWindow);
		}

		pButton->SetPivot(0.5f, 0.5f, 0.f);
		((CClientButton*)pButton.get())->SetSound(BUTTON_STATE::PRESS, SOUND_TYPE::EFFECT, "Click1");

		pButton->SetWorldScale(30.f, 28.f, 1.f);

		Vector3 vButtonScale = pButton->GetWorldScale();
		Vector3 vButtonPivot = pButton->GetPivot();

		pButton->SetWorldPos(vButtonScale.x * vButtonPivot.x + i * vButtonScale.x, tRS.iHeight - vButtonScale.y * vButtonPivot.y, 0.f);
	}
}

void CMainGameMode::CreatePanel()
{
	CLayer* pUI = m_pScene->FindLayer("UI");

	std::shared_ptr<CGameObject> pPanel = m_pScene->CreateObject<CUIPanel>("Panel", pUI);

	std::shared_ptr<CComponent> pMainPanel = pPanel->CreateComponent<CPanel>("Main");

	((CPanel*)pMainPanel.get())->SetTexture("Panel");
	((CPanel*)pMainPanel.get())->SetWorldScale(420.f, 320.f, 1.f);
	((CPanel*)pMainPanel.get())->SetWorldPos(800.f, 500.f, 0.f);
	((CPanel*)pMainPanel.get())->SetZOrder(5);
	((CPanel*)pMainPanel.get())->SetPivot(0.5f, 0.5f, 0.f);

	pPanel->SetRootComponent(pMainPanel);

	std::shared_ptr<CComponent> pPanel1 = pPanel->CreateComponent<CPanel>("Panel1");
	((CPanel*)pPanel1.get())->SetTexture("Panel");
	((CPanel*)pPanel1.get())->SetWorldScale(400.f, 300.f, 1.f);
	((CPanel*)pPanel1.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CPanel*)pPanel1.get())->SetZOrder(4);
	((CPanel*)pPanel1.get())->SetInheritScale(false);

	((CSceneComponent*)pMainPanel.get())->AddChild(pPanel1);

	std::shared_ptr<CComponent> pCheckBox = pPanel->CreateComponent<CChkBx>("Check");
	((CChkBx*)pCheckBox.get())->SetTexture(CHECK_STATE::OFF, "CheckBox");
	((CChkBx*)pCheckBox.get())->SetTexture(CHECK_STATE::ON, "Check");
	((CChkBx*)pCheckBox.get())->SetWorldScale(40.f, 40.f, 1.f);
	((CChkBx*)pCheckBox.get())->SetInheritScale(false);
	((CChkBx*)pCheckBox.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CChkBx*)pCheckBox.get())->SetZOrder(1);

	((CSceneComponent*)pPanel1.get())->AddChild(pCheckBox);

	std::shared_ptr<CComponent> pList = pPanel->CreateComponent<CUIList>("List");

	((CUIList*)pList.get())->AddString("블리츠");
	((CUIList*)pList.get())->AddString("야스오");
	((CUIList*)pList.get())->AddString("조이");
	((CUIList*)pList.get())->AddString("티모");
	((CUIList*)pList.get())->AddString("베인");
	((CUIList*)pList.get())->AddString("마이");
	((CUIList*)pList.get())->AddString("우르곳");
	((CUIList*)pList.get())->AddString("룰루");
	((CUIList*)pList.get())->AddString("트런들");
	((CUIList*)pList.get())->AddString("이즈리얼");
	((CUIList*)pList.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIList*)pList.get())->SetRelativePos(0.f, -100.f, 0.f);
	((CUIList*)pList.get())->SetWorldScale(120.f, 120.f, 1.f);
	((CUIList*)pList.get())->SetInheritScale(false);

	((CSceneComponent*)pPanel1.get())->AddChild(pList);

	std::shared_ptr<CComponent> pRadio1 = pPanel->CreateComponent<CRadioButton>("Radio1");
	((CRadioButton*)pRadio1.get())->SetTexture(RADIO_STATE::OFF, "Radio");
	((CRadioButton*)pRadio1.get())->SetTexture(RADIO_STATE::ON, "Select");
	((CRadioButton*)pRadio1.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CRadioButton*)pRadio1.get())->SetRelativePos(-100.f, 100.f, 0.f);
	((CRadioButton*)pRadio1.get())->SetWorldScale(40.f, 40.f, 1.f);
	((CRadioButton*)pRadio1.get())->SetInheritScale(false);
	((CRadioButton*)pRadio1.get())->SetZOrder(1);

	((CSceneComponent*)pPanel1.get())->AddChild(pRadio1);

	std::shared_ptr<CComponent>  pRadio2 = pPanel->CreateComponent<CRadioButton>("Radio2");
	((CRadioButton*)pRadio2.get())->SetTexture(RADIO_STATE::OFF, "Radio");
	((CRadioButton*)pRadio2.get())->SetTexture(RADIO_STATE::ON, "Select");
	((CRadioButton*)pRadio2.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CRadioButton*)pRadio2.get())->SetRelativePos(0.f, 100.f, 0.f);
	((CRadioButton*)pRadio2.get())->SetWorldScale(40.f, 40.f, 1.f);
	((CRadioButton*)pRadio2.get())->SetInheritScale(false);

	((CSceneComponent*)pPanel1.get())->AddChild(pRadio2);
	
	std::shared_ptr<CComponent> pRadio3 = pPanel->CreateComponent<CRadioButton>("Radio3");
	((CRadioButton*)pRadio3.get())->SetTexture(RADIO_STATE::OFF, "Radio");
	((CRadioButton*)pRadio3.get())->SetTexture(RADIO_STATE::ON, "Select");
	((CRadioButton*)pRadio3.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CRadioButton*)pRadio3.get())->SetRelativePos(100.f, 100.f, 0.f);
	((CRadioButton*)pRadio3.get())->SetWorldScale(40.f, 40.f, 1.f);
	((CRadioButton*)pRadio3.get())->SetInheritScale(false);

	((CSceneComponent*)pPanel1.get())->AddChild(pRadio3);

	((CRadioButton*)pRadio1.get())->AddButton((CRadioButton*)pRadio2.get());
	((CRadioButton*)pRadio1.get())->AddButton((CRadioButton*)pRadio3.get());

	std::shared_ptr<CComponent> pSliderBtn = pPanel->CreateComponent<CSlider>("Slider");
	((CSlider*)pSliderBtn.get())->SetWorldScale(10.f, 20.f, 1.f);
	((CSlider*)pSliderBtn.get())->SetInheritScale(false);
	((CSlider*)pSliderBtn.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CSlider*)pSliderBtn.get())->SetTexture(BUTTON_STATE::OFF, "Slider");
	((CSlider*)pSliderBtn.get())->SetTexture(BUTTON_STATE::ON, "Slider");
	((CSlider*)pSliderBtn.get())->SetTexture(BUTTON_STATE::PRESS, "Slider");

	CSceneComponent* pBackButton = ((CSceneComponent*)pSliderBtn.get())->GetParent();

	((CSceneComponent*)pPanel.get())->AddChild(std::shared_ptr<CComponent>(pBackButton));

	std::shared_ptr<CComponent> pPanel2 = pPanel->CreateComponent<CPanel>("Panel2");
	((CPanel*)pPanel2.get())->SetTexture("Panel");
	((CPanel*)pPanel2.get())->SetWorldScale(400.f, 300.f, 1.f);
	((CPanel*)pPanel2.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CPanel*)pPanel2.get())->SetInheritScale(false);
	((CPanel*)pPanel2.get())->SetEnable(false);
	((CPanel*)pPanel2.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pButton21 = pPanel->CreateComponent<CUIButton>("Button21");
	((CUIButton*)pButton21.get())->SetWorldScale(112.f, 112.f, 1.f);
	((CUIButton*)pButton21.get())->SetInheritScale(false);
	((CUIButton*)pButton21.get())->SetTexture(BUTTON_STATE::OFF, "AML1Main");
	((CUIButton*)pButton21.get())->SetTexture(BUTTON_STATE::ON, "AML1Main");
	((CUIButton*)pButton21.get())->SetTexture(BUTTON_STATE::PRESS, "AML1Main");
	((CUIButton*)pButton21.get())->SetColor(BUTTON_STATE::OFF, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	((CUIButton*)pButton21.get())->SetColor(BUTTON_STATE::ON, Vector4(1.f, 1.f, 1.f, 1.f));
	((CUIButton*)pButton21.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	((CUIButton*)pButton21.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)pButton21.get())->SetRelativePos(-144.f, 94.f, 0.f);

	((CSceneComponent*)pPanel2.get())->AddChild(pButton21);

	std::shared_ptr<CComponent> pButton22 = pPanel->CreateComponent<CUIButton>("Button22");
	((CUIButton*)pButton22.get())->SetWorldScale(112.f, 112.f, 1.f);
	((CUIButton*)pButton22.get())->SetInheritScale(false);
	((CUIButton*)pButton22.get())->SetTexture(BUTTON_STATE::OFF, "MONO1Main");
	((CUIButton*)pButton22.get())->SetTexture(BUTTON_STATE::ON, "MONO1Main");
	((CUIButton*)pButton22.get())->SetTexture(BUTTON_STATE::PRESS, "MONO1Main");
	((CUIButton*)pButton22.get())->SetColor(BUTTON_STATE::OFF, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	((CUIButton*)pButton22.get())->SetColor(BUTTON_STATE::ON, Vector4(1.f, 1.f, 1.f, 1.f));
	((CUIButton*)pButton22.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	((CUIButton*)pButton22.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)pButton22.get())->SetRelativePos(-28.f, 94.f, 0.f);

	((CSceneComponent*)pPanel2.get())->AddChild(pButton22);

	std::shared_ptr<CComponent> pButton23 = pPanel->CreateComponent<CUIButton>("Button23");
	((CUIButton*)pButton23.get())->SetWorldScale(112.f, 112.f, 1.f);
	((CUIButton*)pButton23.get())->SetInheritScale(false);
	((CUIButton*)pButton23.get())->SetTexture(BUTTON_STATE::OFF, "NRL2Main");
	((CUIButton*)pButton23.get())->SetTexture(BUTTON_STATE::ON, "NRL2Main");
	((CUIButton*)pButton23.get())->SetTexture(BUTTON_STATE::PRESS, "NRL2Main");
	((CUIButton*)pButton23.get())->SetColor(BUTTON_STATE::OFF, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	((CUIButton*)pButton23.get())->SetColor(BUTTON_STATE::ON, Vector4(1.f, 1.f, 1.f, 1.f));
	((CUIButton*)pButton23.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	((CUIButton*)pButton23.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)pButton23.get())->SetRelativePos(88.f, 94.f, 0.f);

	((CSceneComponent*)pPanel2.get())->AddChild(pButton23);

	((CSceneComponent*)pMainPanel.get())->AddChild(pPanel2);

	std::shared_ptr<CComponent> pPanel3 = pPanel->CreateComponent<CPanel>("Panel3");
	((CPanel*)pPanel3.get())->SetTexture("Panel");
	((CPanel*)pPanel3.get())->SetWorldScale(400.f, 300.f, 1.f);
	((CPanel*)pPanel3.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CPanel*)pPanel3.get())->SetInheritScale(false);
	((CPanel*)pPanel3.get())->SetEnable(false);
	((CPanel*)pPanel3.get())->SetZOrder(1);

	std::shared_ptr<CComponent> pButton31 = pPanel->CreateComponent<CUIButton>("Button31");
	((CUIButton*)pButton31.get())->SetWorldScale(112.f, 112.f, 1.f);
	((CUIButton*)pButton31.get())->SetInheritScale(false);
	((CUIButton*)pButton31.get())->SetTexture(BUTTON_STATE::OFF, "4X4Main");
	((CUIButton*)pButton31.get())->SetTexture(BUTTON_STATE::ON, "4X4Main");
	((CUIButton*)pButton31.get())->SetTexture(BUTTON_STATE::PRESS, "4X4Main");
	((CUIButton*)pButton31.get())->SetColor(BUTTON_STATE::OFF, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	((CUIButton*)pButton31.get())->SetColor(BUTTON_STATE::ON, Vector4(1.f, 1.f, 1.f, 1.f));
	((CUIButton*)pButton31.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	((CUIButton*)pButton31.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)pButton31.get())->SetRelativePos(-144.f, 94.f, 0.f);

	((CSceneComponent*)pPanel3.get())->AddChild(pButton31);

	std::shared_ptr<CComponent> pButton32 = pPanel->CreateComponent<CUIButton>("Button32");
	((CUIButton*)pButton32.get())->SetWorldScale(112.f, 112.f, 1.f);
	((CUIButton*)pButton32.get())->SetInheritScale(false);
	((CUIButton*)pButton32.get())->SetTexture(BUTTON_STATE::OFF, "BLACKCABMain");
	((CUIButton*)pButton32.get())->SetTexture(BUTTON_STATE::ON, "BLACKCABMain");
	((CUIButton*)pButton32.get())->SetTexture(BUTTON_STATE::PRESS, "BLACKCABMain");
	((CUIButton*)pButton32.get())->SetColor(BUTTON_STATE::OFF, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	((CUIButton*)pButton32.get())->SetColor(BUTTON_STATE::ON, Vector4(1.f, 1.f, 1.f, 1.f));
	((CUIButton*)pButton32.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	((CUIButton*)pButton32.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)pButton32.get())->SetRelativePos(-28.f, 94.f, 0.f);

	((CSceneComponent*)pPanel3.get())->AddChild(pButton32);

	std::shared_ptr<CComponent> pButton33 = pPanel->CreateComponent<CUIButton>("Button33");
	((CUIButton*)pButton33.get())->SetWorldScale(112.f, 112.f, 1.f);
	((CUIButton*)pButton33.get())->SetInheritScale(false);
	((CUIButton*)pButton33.get())->SetTexture(BUTTON_STATE::OFF, "OBS1Main");
	((CUIButton*)pButton33.get())->SetTexture(BUTTON_STATE::ON, "OBS1Main");
	((CUIButton*)pButton33.get())->SetTexture(BUTTON_STATE::PRESS, "OBS1Main");
	((CUIButton*)pButton33.get())->SetColor(BUTTON_STATE::OFF, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	((CUIButton*)pButton33.get())->SetColor(BUTTON_STATE::ON, Vector4(1.f, 1.f, 1.f, 1.f));
	((CUIButton*)pButton33.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	((CUIButton*)pButton33.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)pButton33.get())->SetRelativePos(88.f, 94.f, 0.f);

	((CSceneComponent*)pPanel3.get())->AddChild(pButton33);

	((CSceneComponent*)pMainPanel.get())->AddChild(pPanel3);

	std::shared_ptr<CComponent> pMainTab = pPanel->CreateComponent<CUIButton>("MainTab");
	((CUIButton*)pMainTab.get())->SetInheritScale(false);
	((CUIButton*)pMainTab.get())->SetTexture(BUTTON_STATE::OFF, "waterride");
	((CUIButton*)pMainTab.get())->SetTexture(BUTTON_STATE::ON, "waterride");
	((CUIButton*)pMainTab.get())->SetTexture(BUTTON_STATE::PRESS, "waterride");
	((CUIButton*)pMainTab.get())->SetWorldScale(24.f, 24.f, 1.f);
	((CUIButton*)pMainTab.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)pMainTab.get())->SetRelativePos(-188.f, 164.f, 0.f);
	((CUIButton*)pMainTab.get())->SetCallBack<CUIPanel>(BUTTON_STATE::PRESS, (CUIPanel*)pPanel.get(), &CUIPanel::Btn1);

	((CSceneComponent*)pMainPanel.get())->AddChild(pMainTab);

	std::shared_ptr<CComponent> pGentleTab = pPanel->CreateComponent<CUIButton>("GentleTab");
	((CUIButton*)pGentleTab.get())->SetInheritScale(false);
	((CUIButton*)pGentleTab.get())->SetTexture(BUTTON_STATE::OFF, "gentleride");
	((CUIButton*)pGentleTab.get())->SetTexture(BUTTON_STATE::ON, "gentleride");
	((CUIButton*)pGentleTab.get())->SetTexture(BUTTON_STATE::PRESS, "gentleride");
	((CUIButton*)pGentleTab.get())->SetWorldScale(25.f, 23.f, 1.f);
	((CUIButton*)pGentleTab.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)pGentleTab.get())->SetRelativePos(-162.f, 164.f, 0.f);
	((CUIButton*)pGentleTab.get())->SetCallBack<CUIPanel>(BUTTON_STATE::PRESS, (CUIPanel*)pPanel.get(), &CUIPanel::Btn2);

	((CSceneComponent*)pMainPanel.get())->AddChild(pGentleTab);

	std::shared_ptr<CComponent> pTransportTab = pPanel->CreateComponent<CUIButton>("TransportTab");
	((CUIButton*)pTransportTab.get())->SetInheritScale(false);
	((CUIButton*)pTransportTab.get())->SetTexture(BUTTON_STATE::OFF, "transport");
	((CUIButton*)pTransportTab.get())->SetTexture(BUTTON_STATE::ON, "transport");
	((CUIButton*)pTransportTab.get())->SetTexture(BUTTON_STATE::PRESS, "transport");
	((CUIButton*)pTransportTab.get())->SetWorldScale(29.f, 25.f, 1.f);
	((CUIButton*)pTransportTab.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)pTransportTab.get())->SetRelativePos(-133.f, 164.f, 0.f);
	((CUIButton*)pTransportTab.get())->SetCallBack<CUIPanel>(BUTTON_STATE::PRESS, (CUIPanel*)pPanel.get(), &CUIPanel::Btn3);

	((CSceneComponent*)pMainPanel.get())->AddChild(pTransportTab);

	std::shared_ptr<CComponent> pCloseBtn = pPanel->CreateComponent<CUIButton>("CloseBtn");
	((CUIButton*)pCloseBtn.get())->SetTexture(BUTTON_STATE::OFF, "CloseButton");
	((CUIButton*)pCloseBtn.get())->SetTexture(BUTTON_STATE::ON, "CloseButton");
	((CUIButton*)pCloseBtn.get())->SetTexture(BUTTON_STATE::PRESS, "CloseButton");
	((CUIButton*)pCloseBtn.get())->SetCallBack<CUIPanel>(BUTTON_STATE::PRESS, (CUIPanel*)pPanel.get(), &CUIPanel::Close);

	((CUIButton*)pCloseBtn.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CUIButton*)pCloseBtn.get())->SetRelativePos(190.f, 180.f, 0.f);
	((CUIButton*)pCloseBtn.get())->SetWorldScale(40.f, 40.f, 1.f);
	((CUIButton*)pCloseBtn.get())->SetInheritScale(false);

	((CSceneComponent*)pMainPanel.get())->AddChild(pCloseBtn);

	((CUIButton*)pMainTab.get())->SetZOrder(1);
	((CUIButton*)pGentleTab.get())->SetZOrder(4);
	((CUIButton*)pTransportTab.get())->SetZOrder(4);
}

void CMainGameMode::CreateProto()
{
	std::shared_ptr<CGameObject> pBullet = CScene::CreateProtoObj<CBullet>("Bullet", m_pScene, m_pScene->GetSceneType());

	std::shared_ptr<CGameObject> pEft = CScene::CreateProtoObj<CEffect>("Effect", m_pScene, m_pScene->GetSceneType());

	pBullet = CScene::CreateProtoObj<CMinionBullet>("MinionBullet", m_pScene, m_pScene->GetSceneType());

	std::shared_ptr<CGameObject> pTile = CScene::CreateProtoObj<CTileObj>("TileMap", m_pScene, m_pScene->GetSceneType());

	std::shared_ptr<CComponent> pMap = pTile->CreateComponent<CTileMap>("TileMap");

	((CTileMap*)pMap.get())->CreateTile(TILE_TYPE::ISOMETRIC, 100, 100, Vector3::Zero, Vector2(64.f, 31.f), TILE_OPTION::NONE, 0);

	((CTileMap*)pMap.get())->SetTileTexture(IMAGE_TYPE::IT_ARRAY, "Tile");

	((CTileObj*)pTile.get())->SetRootComponent(pMap);

}

void CMainGameMode::LoadTexture()
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	CLoadingMode* pMode = (CLoadingMode*)pScene->GetGameMode();

	std::shared_ptr<CGameObject> pObj = {};

	CBr* pBar = nullptr;

	if (!pMode)
	{
		pScene = GET_SINGLE(CSceneManager)->GetNextScene();

		CLoadingMode* pMode = (CLoadingMode*)pScene->GetGameMode();

		pObj = pMode->GetBar();

		pBar = (CBr*)pObj.get();
	}

	else
	{
		pObj = pMode->GetBar();

		pBar = (CBr*)pObj.get();
	}


	if (!GET_SINGLE(CResourceManager)->LoadTexture("Anaconda Ride", TEXT("Rides\\Anaconda Ride.png")))
		return;

	CResourceManager* pRes = GET_SINGLE(CResourceManager);

	std::vector<const TCHAR*> vecFileName;

	for (int i = 6409; i <= 6429; i += 4)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("person\\client\\%d.png"), i);

		vecFileName.push_back(pFileName);
		pBar->AddProgress();
	}

	GET_SINGLE(CResourceManager)->LoadTexture("ClientWalkRT", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	for (int i = 7249; i <= 7281; i += 4)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("person\\client\\%d.png"), i);

		vecFileName.push_back(pFileName);
		pBar->AddProgress();
	}

	GET_SINGLE(CResourceManager)->LoadTexture("ClientClap", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	for (int i = 6614; i <= 6622; i += 4)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("person\\client\\%d.png"), i);

		vecFileName.push_back(pFileName);
		pBar->AddProgress();
	}

	GET_SINGLE(CResourceManager)->LoadTexture("ClientIdle", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	for (int i = 23165; i <= 23185; i += 4)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("goose\\fly\\%d.png"), i);

		vecFileName.push_back(pFileName);
		pBar->AddProgress();
	}

	GET_SINGLE(CResourceManager)->LoadTexture("GooseFly", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	for (int i = 22878; i <= 22895; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Effect\\explode\\%d.png"), i);

		vecFileName.push_back(pFileName);
		pBar->AddProgress();
	}

	GET_SINGLE(CResourceManager)->LoadTexture("Explode", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	for (int i = 22915; i <= 22926; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Effect\\particle\\%d.png"), i);

		vecFileName.push_back(pFileName);
		pBar->AddProgress();
	}

	GET_SINGLE(CResourceManager)->LoadTexture("particle", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	for (int i = 3; i <= 34; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("RollerCoaster\\RollerCoaster\\ARRT1\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}

	GET_SINGLE(CResourceManager)->LoadTexture("ARRTIdle", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	std::shared_ptr<CTexture> pTex = GET_SINGLE(CResourceManager)->FindTexture("ARRTIdle");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("ARRTIdle", pTex, 1.f, 5.f);

	for (int i = 375; i <= 407; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("RollerCoaster\\RollerCoaster\\ARRT1\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("ARRTRun", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("ARRTRun");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("ARRTRun", pTex);

	for (int i = 503; i <= 534; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("RollerCoaster\\RollerCoaster\\ARRT1\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("ARRTAttack", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("ARRTAttack");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("ARRTAttack", pTex);

	GET_SINGLE(CResourceManager)->AddAni2DNotify("ARRTAttack", "Attack", 31);

	for (int i = 643; i <= 674; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("RollerCoaster\\RollerCoaster\\ARRT1\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}

	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("ARRTIdleClient", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("ARRTIdleClient");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("ARRTIdleClient", pTex, 1.f, 5.f);

	for (int i = 375 + 640; i <= 407 + 640; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("RollerCoaster\\RollerCoaster\\ARRT1\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("ARRTRunClient", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("ARRTRunClient");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("ARRTRunClient", pTex);

	for (int i = 503 + 640; i <= 534 + 640; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("RollerCoaster\\RollerCoaster\\ARRT1\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("ARRTAttackClient", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("ARRTAttackClient");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("ARRTAttackClient", pTex);

	GET_SINGLE(CResourceManager)->AddAni2DNotify("ARRTAttackClient", "Attack", 31);



	for (int i = 5253; i <= 5260; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Icon\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("MoneyIcon", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("MoneyIcon");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("MoneyIcon", pTex);

	for (int i = 5351; i <= 5366; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Icon\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();
	GET_SINGLE(CResourceManager)->LoadTexture("Burger", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("Burger");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("Burger", pTex);

	for (int i = 5367; i <= 5374; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Icon\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("Info", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("Info");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("Info", pTex);

	GET_SINGLE(CResourceManager)->AddAni2DNotify("Info", "Attack", 7);

	for (int i = 5530; i <= 5536; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Icon\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("eatIcon", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("eatIcon");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("eatIcon", pTex);

	for (int i = 5537; i <= 5541; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Icon\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("transport", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("transport");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("transport", pTex);

	for (int i = 5542; i <= 5545; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Icon\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("gentleride", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("gentleride");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("gentleride", pTex);

	for (int i = 5546; i <= 5550; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Icon\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("coasterIcon", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("coasterIcon");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("coasterIcon", pTex);

	for (int i = 5551; i <= 5556; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Icon\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}

	pBar->AddProgress();
	GET_SINGLE(CResourceManager)->LoadTexture("waterride", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("waterride");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("waterride", pTex);

	GET_SINGLE(CResourceManager)->AddAni2DNotify("waterride", "Fire", 5);

	for (int i = 5557; i <= 5563; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Icon\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("AmazeIcon", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("AmazeIcon");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("AmazeIcon", pTex);

	for (int i = 3; i <= 34; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("RollerCoaster\\GentleRide\\MGR1\\%d.png"), i);

		vecFileName.push_back(pFileName);
	}
	pBar->AddProgress();

	GET_SINGLE(CResourceManager)->LoadTexture("MarryGoRound", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	for (int i = 0; i < 12; ++i)
	{
		int iNum = 1915 + i * (38+19);

		TCHAR* strFile = new TCHAR[MAX_PATH];
		swprintf_s(strFile, MAX_PATH, TEXT("Tile_Big\\%d.png"), iNum);
		vecFileName.push_back(strFile);
		TCHAR* strFile1 = new TCHAR[MAX_PATH];
		swprintf_s(strFile1, MAX_PATH, TEXT("Tile_Big\\%d.png"), iNum + 38);
		vecFileName.push_back(strFile1);
	}
	pBar->AddProgress();
	TCHAR* strFile1 = new TCHAR[MAX_PATH];
	swprintf_s(strFile1, MAX_PATH, TEXT("Tile_Big\\%d.png"), 2599);
	vecFileName.push_back(strFile1);
	TCHAR* strFile2= new TCHAR[MAX_PATH];
	swprintf_s(strFile2, MAX_PATH, TEXT("Tile_Big\\%d.png"), 2623);
	vecFileName.push_back(strFile2);
	TCHAR* strFile3 = new TCHAR[MAX_PATH];
	swprintf_s(strFile3, MAX_PATH, TEXT("Tile_Big\\%d.png"), 2624);
	vecFileName.push_back(strFile3);

	for (int i = 0; i < 1; ++i)
	{
		int iNum = 2625 + i * (38 + 19);

		TCHAR* strFile = new TCHAR[MAX_PATH];
		swprintf_s(strFile, MAX_PATH, TEXT("Tile_Big\\%d.png"), iNum);
		vecFileName.push_back(strFile);
		TCHAR* strFile1 = new TCHAR[MAX_PATH];
		swprintf_s(strFile1, MAX_PATH, TEXT("Tile_Big\\%d.png"), iNum + 19);
		vecFileName.push_back(strFile1);
	}
	pBar->AddProgress();

	for (int i = 0; i < 12; ++i)
	{
		int iNum = 2663 + i * (38);

		TCHAR* strFile = new TCHAR[MAX_PATH];
		swprintf_s(strFile, MAX_PATH, TEXT("Tile_Big\\%d.png"), iNum);
		vecFileName.push_back(strFile);
		pBar->AddProgress();
	}

	GET_SINGLE(CResourceManager)->LoadTextureArray("Tile", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("MarryGoRound");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("MarryGoRound", pTex, 1.f, 3.f);

	GET_SINGLE(CResourceManager)->LoadTexture("Circus", TEXT("GentleRide\\Circus.png"));

	GET_SINGLE(CResourceManager)->LoadTexture("Crooked House", TEXT("GentleRide\\Crooked House.png"));

	pTex = GET_SINGLE(CResourceManager)->FindTexture("Anaconda Ride");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("AnaIdle", pTex);

	for (int i = 0; i < 32; ++i)
		GET_SINGLE(CResourceManager)->AddFrame("AnaIdle", Vector2(7.f, 144.f + 41.f * i), Vector2(67.f, 144.f + 41.f * (i + 1)));

	pBar->AddProgress();
	//GET_SINGLE(CResourceManager)->AddAni2DNotify("AnaIdle", "Fire", 31);

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("AnaRun", pTex);

	for (int i = 0; i < 32; ++i)
		GET_SINGLE(CResourceManager)->AddFrame("AnaRun", Vector2(2138.f, 144.f + 60.f * i), Vector2(2138.f + 45.f, 144.f + 60.f * (i + 1)));

	pBar->AddProgress();
	GET_SINGLE(CResourceManager)->CreateAni2DSequence("AnaAttack", pTex);

	for (int i = 0; i < 32; ++i)
		GET_SINGLE(CResourceManager)->AddFrame("AnaAttack", Vector2(1090.f, 2092.f + 42.f * i), Vector2(1090.f + 65.f, 2092.f + 42.f * (i + 1)));

	pBar->AddProgress();
	GET_SINGLE(CResourceManager)->LoadTexture("test", TEXT("test.bmp"));

	GET_SINGLE(CResourceManager)->LoadTexture("StartBtn", TEXT("23207.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("Bar", TEXT("bar.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("Back", TEXT("back.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("CheckBox", TEXT("CheckBox.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("Check", TEXT("Check.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("Select", TEXT("Select.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("Radio", TEXT("Radio.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("Panel", TEXT("Panel.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("CloseButton", TEXT("close.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("Up", TEXT("Up.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("Down", TEXT("Down.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("Slider", TEXT("Slider.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("SliderBack", TEXT("SliderBack.png"));

	pBar->AddProgress();
	LoadExcel("Textures.csv");
	LoadExcel("RidesTex.csv", IMAGE_TYPE::IT_ARRAY);
	LoadSeq("Sequences.csv");

	pBar->AddProgress();
}

void CMainGameMode::LoadAnimation2D()
{
	std::shared_ptr<CTexture> pTex = GET_SINGLE(CResourceManager)->FindTexture("ClientWalkRT");

	if (!GET_SINGLE(CResourceManager)->CreateAni2DSequence("PlayerWalkRT", pTex, 1.f, 0.3f))
		return;


	pTex = GET_SINGLE(CResourceManager)->FindTexture("Explode");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("Explode", pTex, 1.f, 1.f);


	pTex = GET_SINGLE(CResourceManager)->FindTexture("ClientClap");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("PlayerClap", pTex, 1.f, 0.4f);


	pTex = GET_SINGLE(CResourceManager)->FindTexture("ClientIdle");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("PlayerIdle", pTex, 1.f, 0.3f);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("GooseFly");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("MonsterFly", pTex, 1.f, 0.5f);

	GET_SINGLE(CResourceManager)->AddAni2DNotify("MonsterFly", "Fire", 2);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("particle");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("MonBullet", pTex, 1.f, 1.1f);

	pTex = GET_SINGLE(CResourceManager)->FindTexture("Circus");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("Circus", pTex);

	GET_SINGLE(CResourceManager)->AddFrame("Circus", Vector2(0.f, 118.f), Vector2(133.f, 237.f));

	pTex = GET_SINGLE(CResourceManager)->FindTexture("Crooked House");

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("Crooked House", pTex);

	GET_SINGLE(CResourceManager)->AddFrame("Crooked House", Vector2(4.f, 185.f), Vector2(103.f, 293.f));
}

void CMainGameMode::ShowRideWindow(float fTime)
{
	m_pRideWindow->SetEnable(!m_pRideWindow->IsEnable());

	m_pRideWindow->SetWorldPos(600.f, 400.f, 0.f);
}

void CMainGameMode::ShowConstructWindow(float fTime)
{
	m_pContructWindow->SetEnable(!m_pContructWindow->IsEnable());

	Vector3 vScale = m_pContructWindow->GetWorldScale();
	Vector3 vPivot = m_pContructWindow->GetPivot();

	m_pContructWindow->SetWorldPos(vScale.x * vPivot.x, 400.f, 0.f);
}

void CMainGameMode::CloseRideWindow(float fTime)
{
	m_pRideWindow->SetEnable(false);
}

void CMainGameMode::StartScene(float)
{
	GET_SINGLE(CSceneManager)->CreateNextScene();

	GET_SINGLE(CSceneManager)->SetGameMode<CStartGameMode>(false);

	//GET_SINGLE(CViewManager)->ClearProtoList();
}

void CMainGameMode::LoadExcel(const char* pFileName, IMAGE_TYPE eType)
{
	char pFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(DATA_PATH);

	strcat_s(pFullPath, pPath);
	strcat_s(pFullPath, pFileName);

	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rt");

	if (pFile)
	{
		while (true)
		{
			char cLine[1024] = {};

			fgets(cLine, 1024, pFile);

			if (strcmp(cLine, "") == 0)
				break;

			char* pPathKey = nullptr;

			char* pKey = strtok_s(cLine, ", ", &pPathKey);

			char* pFileName = strtok_s(nullptr, ", ", &pPathKey);

			std::string strKey = pKey;

			TCHAR _pFileName[256] = {};

#ifdef _UNICODE
			MultiByteToWideChar(CP_ACP, 0, pFileName, -1, _pFileName, (int)strlen(pFileName));
#else
			strcpy_s(_pFileName, pFileName);
#endif
			pPathKey[strlen(pPathKey) - 1] = 0;

			std::string strPathKey = pPathKey;

			GET_SINGLE(CResourceManager)->LoadTexture(strKey, _pFileName, strPathKey);

			std::shared_ptr<CTexture> pTex = GET_SINGLE(CResourceManager)->FindTexture(strKey);

			if (pTex)
				pTex->SetImageType(eType);
		}

		fclose(pFile);
	}
}

void CMainGameMode::LoadSeq(const char* pFileName)
{
	char pFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(DATA_PATH);

	if (pPath)
		strcpy_s(pFullPath, pPath);

	strcat_s(pFullPath, pFileName);

	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rt");

	if (pFile)
	{
		while (true)
		{
			char cLine[1024] = {};

			fgets(cLine, 1024, pFile);

			if (strcmp(cLine, "") == 0)
				break;

			char* pContext = nullptr;

			char* pSeq = strtok_s(cLine, ",", &pContext);

			char* pTex = strtok_s(nullptr, ",", &pContext);

			char* pRate = strtok_s(nullptr, ",", &pContext);

			char* pMax = strtok_s(nullptr, ",", &pContext);

			GET_SINGLE(CResourceManager)->CreateAni2DSequence(pSeq, pTex, (float)atoi(pRate), (float)atoi(pMax));
		}

		fclose(pFile);
	}
}

void CMainGameMode::Save(FILE* pFile)
{
	CGameMode::Save(pFile);
}

void CMainGameMode::Load(FILE* pFile)
{
	CGameMode::Load(pFile);

	CLayer* pUI = m_pScene->FindLayer("UI");

	m_pContructWindow = pUI->FindObj("ConstructionWindow");

	m_pRideWindow = pUI->FindObj("RideWindow");

	std::shared_ptr<CGameObject> pObj = pUI->FindObj("RollercoasterButton");

	CUIButton* pBtn = ((CClientButton*)pObj.get())->GetButton();

	pBtn->SetCallBack(BUTTON_STATE::PRESS, CMainGameMode::ShowRideWindow);

	pObj = pUI->FindObj("BackButton");

	((CClientButton*)pObj.get())->SetClickCallBack<CMainGameMode>(this, BUTTON_STATE::PRESS, &CMainGameMode::StartScene);

	

	CreateProto();
}
