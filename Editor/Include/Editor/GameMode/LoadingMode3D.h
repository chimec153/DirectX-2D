#pragma once
#include "GameMode.h"
#include "../EditorFlags.h"
class CLoadingMode3D :
	public CGameMode
{
	friend class CScene;
	friend class CEditor3D;

private:
	CLoadingMode3D();
	virtual ~CLoadingMode3D();

private:
	static float						m_fPercent;
	CRITICAL_SECTION*					m_pSection;
	std::shared_ptr<class CComponent>	m_pBar;
	EDITOR_GAME_MODE_TYPE				m_eGameModeType;
	class CLoadingThread3D*				m_pThread;
	class CLoadingThread3D*				m_pTextureThread;
	class CLoadingThread3D*				m_pMeshThread;
	float		m_fMeshGauge;
	float		m_fTextureGauge;
	std::shared_ptr<class CComponent>	m_pBack;
	bool m_bCreatePlayer;
public:
	void SetPos(float fPercent);
	void SetGameModeType(EDITOR_GAME_MODE_TYPE eType);
	void SetMeshGauge(float fGauge)
	{
		m_fMeshGauge = fGauge;
	}
	void SetTextureGauge(float fGauge)
	{
		m_fTextureGauge = fGauge;
	}
	void SetBackTexture(const std::string& strKey);
	void SetCreatePlayer()
	{
		m_bCreatePlayer = true;
	}

public:
	bool Init();
	void Update(float fTime);
public:
	bool LoadTexture();
};

