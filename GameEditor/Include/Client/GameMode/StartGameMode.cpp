#include "StartGameMode.h"
#include "../UI/ClientButton.h"
#include "Scene/Scene.h"
#include "Resource/ResourceManager.h"
#include "../UI/UIPanel.h"
#include "Scene/SceneManager.h"
#include "MainGameMode.h"
#include "Engine.h"
#include "UI/UIImage.h"
#include "UI/UIButton.h"
#include "CameraManager.h"
#include "Component/Camera.h"
#include "Scene/SceneCollision.h"
#include "Device.h"
#include "LoadingMode.h"

CStartGameMode::CStartGameMode()
{
}

CStartGameMode::~CStartGameMode()
{
}

bool CStartGameMode::Init()
{
	LoadResource();

	CLayer* pUI = m_pScene->FindLayer("UI");

	std::shared_ptr<CGameObject> pStartBtn = m_pScene->CreateObject<CClientButton>("StartButton", pUI);

	((CClientButton*)pStartBtn.get())->SetTexture(BUTTON_STATE::OFF, "StartBtn");
	((CClientButton*)pStartBtn.get())->SetTexture(BUTTON_STATE::ON, "StartBtn");
	((CClientButton*)pStartBtn.get())->SetTexture(BUTTON_STATE::PRESS, "StartBtn");
	((CClientButton*)pStartBtn.get())->SetColor(BUTTON_STATE::OFF, 0.8f, 0.8f, 0.8f, 1.f);
	((CClientButton*)pStartBtn.get())->SetColor(BUTTON_STATE::ON, 1.f, 1.f, 1.f, 1.f);
	((CClientButton*)pStartBtn.get())->SetColor(BUTTON_STATE::PRESS, 1.f, 1.f, 1.f, 1.f);
	((CClientButton*)pStartBtn.get())->SetClickCallBack<CStartGameMode>(this, BUTTON_STATE::PRESS, &CStartGameMode::NextScene);
	pStartBtn->SetWorldScale(58.f, 78.f, 1.f);
	pStartBtn->SetPivot(0.5f, 0.5f, 0.f);
	pStartBtn->SetWorldPos(440.f, 100.f, 0.f);

	std::shared_ptr<CGameObject> pEndBtn = m_pScene->CreateObject<CClientButton>("EndButton", pUI);

	((CClientButton*)pEndBtn.get())->SetTexture(BUTTON_STATE::OFF, "EndBtn");
	((CClientButton*)pEndBtn.get())->SetTexture(BUTTON_STATE::ON, "EndBtn");
	((CClientButton*)pEndBtn.get())->SetTexture(BUTTON_STATE::PRESS, "EndBtn");
	((CClientButton*)pEndBtn.get())->SetColor(BUTTON_STATE::OFF, 0.8f, 0.8f, 0.8f, 1.f);
	((CClientButton*)pEndBtn.get())->SetColor(BUTTON_STATE::ON, 1.f, 1.f, 1.f, 1.f);
	((CClientButton*)pEndBtn.get())->SetColor(BUTTON_STATE::PRESS, 1.f, 1.f, 1.f, 1.f);
	((CClientButton*)pEndBtn.get())->SetClickCallBack<CStartGameMode>(this, BUTTON_STATE::PRESS, &CStartGameMode::EndGame);
	pEndBtn->SetWorldScale(27.f, 58.f, 1.f);
	pEndBtn->SetPivot(0.5f, 0.5f, 0.f);
	pEndBtn->SetWorldPos(840.f, 100.f, 0.f);

	std::shared_ptr<CGameObject> pPanel = m_pScene->CreateObject<CUIPanel>("MainPanel", pUI);

	std::shared_ptr<CComponent> pImg = pPanel->CreateComponent<CUIImage>("Logo");

	((CUIImage*)pImg.get())->SetTexture("Logo");
	((CUIImage*)pImg.get())->SetWorldScale(640.f, 480.f, 1.f);
	((CUIImage*)pImg.get())->SetPivot(0.5f, 0.5f, 0.f);

	pPanel->SetRootComponent(pImg);

	pPanel->SetWorldScale(640.f, 480.f, 1.f);
	pPanel->SetWorldPos(640.f, 500.f, 0.f);
	pPanel->SetPivot(0.5f, 0.5f, 0.f);	

	Resolution tRS = RESOLUTION;

	m_pScene->GetCollision()->SetCenter(Vector3((float)tRS.iWidth / 2.f, (float)tRS.iHeight / 2.f, 0.f));

	return true;
}

void CStartGameMode::NextScene(float fTime)
{
	GET_SINGLE(CSceneManager)->CreateNextScene();

	GET_SINGLE(CSceneManager)->SetGameMode<CLoadingMode>(false);
}

void CStartGameMode::EndGame(float fTime)
{
	GET_SINGLE(CEngine)->EndGame();
}

void CStartGameMode::LoadResource()
{
	GET_SINGLE(CResourceManager)->LoadTexture("StartBtn", TEXT("Logo\\23207.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("EditBtn", TEXT("Logo\\23209.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("EndBtn", TEXT("Logo\\23210.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("Logo", TEXT("Logo\\logo.png"));

}
