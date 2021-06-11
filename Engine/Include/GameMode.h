#pragma once

#include "GameEngine.h"

	class CGameMode
	{
		friend class CScene;

	protected:
		CGameMode();
		virtual ~CGameMode();

	protected:
		class CScene* m_pScene;
		std::shared_ptr<class CGameObject> m_pPlayer;
		int					m_iGameModeType;
		bool			m_bStart;
		std::shared_ptr<class CGameObject>	m_pDome;
		std::shared_ptr<class CComponent>	m_pDomeComponent;

	public:
		std::shared_ptr<class CGameObject> GetPlayer()	const;
		void SetPlayer(std::shared_ptr<class CGameObject> pPlayer);
		int GetGameModeType()	const;
		bool IsStart();
		void SetDomeTexture(const std::string& strTexture);
		void SetDomeShader(const std::string& strKey);
		void SetDomeTexture2D(const std::string& strTexture);
		void SetDomeDiffuseColor(const Vector4& vColor);

	public:
		virtual bool Init();
		virtual void Start();
		virtual void Update(float fTime);
		virtual void PostUpdate(float fTime);
		virtual void Collision(float fTime);
		virtual void PreRender(float fTime);
		virtual void Render(float fTime);
		virtual void PostRender(float fTime);

	public:
		virtual void Save(FILE* pFile);
		virtual void Load(FILE* pFile);
	};