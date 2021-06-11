#pragma once
#include "GameEngine.h"

	class CCameraManager
	{
	private:
		std::unordered_map<std::string, class CCamera*>		m_mapCam;
		class CCamera* m_pMainCam;
		class CCamera* m_pUICam;
		Resolution		m_tRS;

	public:
		void SetMainCam(class CCamera* pCam);
		void SetMainCam(const std::string& strTag);
		void SetUICam(class CCamera* pCam);
		void SetUICam(const std::string& strTag);
		void AddCam(class CCamera* pCam);
		class CCamera* GetMainCam()	const;
		class CCamera* GetUICam()	const;
		class CCamera* FindCam(const std::string& strTag);
		void EraseCam(const std::string& strTag);
		void EraseCam(class CCamera* pCam);
		const Resolution& GetResolution()	const;

	public:
		bool Init(const Resolution& tRS);
		void Update(float fTime);
		void PostUpdate(float fTime);

	public:
		void ShowWindow();

		DECLARE_SINGLE(CCameraManager)
	};