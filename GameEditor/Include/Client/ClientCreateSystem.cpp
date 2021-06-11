#include "../pch.h"
#include "ClientCreateSystem.h"
#include "Scene/SceneManager.h"
#include "Object/Bullet.h"
#include "Object/Player.h"
#include "GameMode/MainGameMode.h"
#include "GameEngine.h"
#include "Object/Minion.h"
#include "Object/MinionBullet.h"
#include "Object/Effect.h"
#include "Object/Marry.h"
#include "Object/TileObj.h"
#include "UI/RideWindow.h"
#include "UI/PathWindow.h"
#include "UI/ConstructionWindow.h"
#include "UI/Br.h"
#include "UI/CheckBox.h"
#include "UI/ClientButton.h"
#include "UI/RadioBox.h"
#include "UI/SliderControl.h"
#include "UI/UIPanel.h"

DEFINITION_SINGLE(CClientCreateSystem)

CClientCreateSystem::CClientCreateSystem()
{
	/*AllocConsole();

	WSADATA tData = {};
	WSAStartup(MAKEWORD(2, 2), &tData);*/
}

CClientCreateSystem::~CClientCreateSystem()
{
	//WSACleanup();
}

bool CClientCreateSystem::Init()
{
	GET_SINGLE(CSceneManager)->SetEditorCreateObjFunc<CClientCreateSystem>(this, &CClientCreateSystem::CreateObject);
	GET_SINGLE(CSceneManager)->SetEditorCreateGameModeFunc<CClientCreateSystem>(this, &CClientCreateSystem::CreateGameMode);


	return true;
}

std::shared_ptr<CGameObject> CClientCreateSystem::CreateObject(int iObjType)
{
	switch (OBJECT_CLASS_TYPE(iObjType))
	{
	case OBJECT_CLASS_TYPE::OBJECT:
		return std::shared_ptr<CGameObject>(new CGameObject);
	case OBJECT_CLASS_TYPE::OCT_PLAYER:
		return std::shared_ptr<CGameObject>(new CPlayer);
	case OBJECT_CLASS_TYPE::OCT_BULLET:
		return std::shared_ptr<CGameObject>(new CBullet);
	case OBJECT_CLASS_TYPE::OCT_MINION:
		return std::shared_ptr<CGameObject>(new CMinion);
	case OBJECT_CLASS_TYPE::MINION_BULLET:
		return std::shared_ptr<CGameObject>(new CMinionBullet);
	case OBJECT_CLASS_TYPE::TILEOBJ:
		return std::shared_ptr<CGameObject>(new CTileObj);
	case OBJECT_CLASS_TYPE::EFFECT:
		return std::shared_ptr<CGameObject>(new CEffect);
	case OBJECT_CLASS_TYPE::MARRY:
		return std::shared_ptr<CGameObject>(new CMarry);
	case OBJECT_CLASS_TYPE::RIDE_WINDOW:
		return std::shared_ptr<CGameObject>(new CRideWindow);
	case OBJECT_CLASS_TYPE::PATH_WINDOW:
		return std::shared_ptr<CGameObject>(new CPathWindow);
	case OBJECT_CLASS_TYPE::CONSTRUCTION_WINDOW:
		return std::shared_ptr<CGameObject>(new CConstructionWindow);
	case OBJECT_CLASS_TYPE::BAR:
		return std::shared_ptr<CGameObject>(new CBr);
	case OBJECT_CLASS_TYPE::CHECKBOX:
		return std::shared_ptr<CGameObject>(new CCheckBox);
	case OBJECT_CLASS_TYPE::CLIENTBUTTON:
		return std::shared_ptr<CGameObject>(new CClientButton);
	case OBJECT_CLASS_TYPE::RADIOBOX:
		return std::shared_ptr<CGameObject>(new CRadioBox);
	case OBJECT_CLASS_TYPE::SLIDERCONTROL:
		return std::shared_ptr<CGameObject>(new CSliderControl);
	case OBJECT_CLASS_TYPE::UIPANEL:
		return std::shared_ptr<CGameObject>(new CUIPanel);
	}

	return nullptr;
}

CGameMode* CClientCreateSystem::CreateGameMode(int iType)
{
	switch (iType)
	{
	case (int)GAMEMODE_CLASS_TYPE::GCT_MAIN:
		return new CMainGameMode;
	}

	return nullptr;
}
