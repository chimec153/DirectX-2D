#include "GameMode.h"
#include "Object/GameObject.h"

CGameMode::CGameMode()	:
	m_pScene(nullptr),
	m_pPlayer(nullptr)
{
}

CGameMode::~CGameMode()
{
	SAFE_RELEASE(m_pPlayer);
}

CGameObject* CGameMode::GetPlayer() const
{
	if (m_pPlayer)
		m_pPlayer->AddRef();

	return m_pPlayer;
}

void CGameMode::SetPlayer(CGameObject* pPlayer)
{
	SAFE_RELEASE(m_pPlayer);

	m_pPlayer = pPlayer;

	if (m_pPlayer)
		m_pPlayer->AddRef();
}

bool CGameMode::Init()
{
	return true;
}

void CGameMode::Start()
{
}

void CGameMode::Update(float fTime)
{
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
