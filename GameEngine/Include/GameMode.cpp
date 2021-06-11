#include "GameMode.h"
#include "Object/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneCollision.h"
#include "Component/Renderer.h"
#include "Resource/Material.h"
#include "Resource/ResourceManager.h"
#include "Resource/Texture.h"

CGameMode::CGameMode()	:
	m_pScene(nullptr),
	m_pPlayer(nullptr),
	m_iGameModeType(0),
	m_bStart(false)
{
}

CGameMode::~CGameMode()
{
}

std::shared_ptr<CGameObject> CGameMode::GetPlayer() const
{
	return m_pPlayer;
}

void CGameMode::SetPlayer(std::shared_ptr<CGameObject> pPlayer)
{
	m_pPlayer = pPlayer;
}

int CGameMode::GetGameModeType() const
{
	return m_iGameModeType;
}

bool CGameMode::IsStart()
{
	return m_bStart;
}

void CGameMode::SetDomeTexture(const std::string& strTexture)
{
	std::shared_ptr<CTexture> pSkyTexture = GET_SINGLE(CResourceManager)->FindTexture(strTexture);

	pSkyTexture->SetShader(50, static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));

}

void CGameMode::SetDomeShader(const std::string& strKey)
{
	static_cast<CSceneComponent*>(m_pDomeComponent.get())->SetShader(strKey);
}

void CGameMode::SetDomeTexture2D(const std::string& strTexture)
{
	static_cast<CSceneComponent*>(m_pDomeComponent.get())->GetMaterial()->SetTexture(LINK_TYPE::LT_DIF, strTexture);
		//TEXT("LV_Common_Sky_A\\Texture2D\\LV_Sunset_SKY_HSI.dds"),TEXTURE_PATH);
		//TEXT("MapTemplates\\Texture2D\\T_MapTemplate_Mobile_Afternoon.dds"), TEXTURE_PATH);
}

void CGameMode::SetDomeDiffuseColor(const Vector4& vColor)
{
	static_cast<CSceneComponent*>(m_pDomeComponent.get())->GetMaterial()->SetDiffuseColor(vColor);
}

bool CGameMode::Init()
{
	m_pDome = m_pScene->CreateObject<CGameObject>("Sphere", m_pScene->FindLayer(CUBEMAP_LAYER));

	m_pDomeComponent = m_pDome->CreateComponent<CSceneComponent>("Sphere");

	m_pDome->SetRootComponent(m_pDomeComponent);
	static_cast<CSceneComponent*>(m_pDomeComponent.get())->SetMesh("Cube2");
	static_cast<CSceneComponent*>(m_pDomeComponent.get())->SetMaterial("Color");
	static_cast<CSceneComponent*>(m_pDomeComponent.get())->SetShader("Cube");
	static_cast<CSceneComponent*>(m_pDomeComponent.get())->AddRenderState("Cull_Front");
	static_cast<CSceneComponent*>(m_pDomeComponent.get())->AddRenderState(LESS_EQUAL_DSS);
	//static_cast<CRenderer*>(m_pDomeComponent.get())->GetMaterial()->SetTexture(LINK_TYPE::LT_DIF, "CubeMapTexture",
	//	//TEXT("LV_Common_Sky_A\\Texture2D\\LV_Sunset_SKY_HSI.dds"),TEXTURE_PATH);
	//	TEXT("MapTemplates\\Texture2D\\T_MapTemplate_Mobile_Afternoon.dds"), TEXTURE_PATH);

	static_cast<CSceneComponent*>(m_pDomeComponent.get())->SetWorldScale(100000.f, 100000.f, 100000.f);


	return true;
}

void CGameMode::Start()
{
	m_bStart = true;
}

void CGameMode::Update(float fTime)
{
	if (m_pPlayer)
	{
		m_pScene->GetCollision()->SetCenter(m_pPlayer->GetWorldPos());
	}
}

void CGameMode::PostUpdate(float fTime)
{
}

void CGameMode::Collision(float fTime)
{
}

void CGameMode::PreRender(float fTime)
{
}

void CGameMode::Render(float fTime)
{
}

void CGameMode::PostRender(float fTime)
{
}

void CGameMode::Save(FILE* pFile)
{
	bool bPlayer = false;

	if (m_pPlayer)
		bPlayer = true;

	fwrite(&bPlayer, sizeof(bool), 1, pFile);

	if (m_pPlayer)
	{
		int iLength = (int)m_pPlayer->GetName().length();
		fwrite(&iLength, sizeof(int), 1, pFile);
		fwrite(m_pPlayer->GetName().c_str(), 1, iLength, pFile);
	}
}

void CGameMode::Load(FILE* pFile)
{
	bool bPlayer = false;

	fread(&bPlayer, sizeof(bool), 1, pFile);

	if (bPlayer)
	{
		int iLength = 0;

		char strName[256] = {};

		fread(&iLength, sizeof(int), 1, pFile);
		fread(strName, 1, iLength, pFile);

		std::string strTag = strName;

		CLayer* pLayer = m_pScene->FindLayer("Default");

		SetPlayer(pLayer->FindObj(strTag));		
	}
}
