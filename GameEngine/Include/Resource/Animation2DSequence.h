#pragma once

#include "../Ref.h"

	class CAnimation2DSequence :
		public CRef
	{
		friend class CResourceManager;
		friend class CSpriteComponent;

	public:
		CAnimation2DSequence();
		CAnimation2DSequence(const CAnimation2DSequence& pSeq);
		virtual ~CAnimation2DSequence();

	private:
		std::shared_ptr<class CTexture>				m_pTexture;
		float									m_fPlayRate;
		float									m_fMaxTime;
		int										m_iFrameMax;
		std::vector<FrameInfo>					m_vecFrame;
		std::list<class CSpriteComponent*>		m_SpriteList;
		std::list<class CAnimation2DNotify*>	m_NotifyList;

	public:
		void AddSprite(class CSpriteComponent* pSprite);
		void DeleteSprite(class CSpriteComponent* pSprite);
		float GetPlayRate()	const;
		float GetMaxTime()	const;
		std::shared_ptr<class CTexture> GetTexture()	const;
		const FrameInfo& GetFrame(int idx = 0)	const;
		int GetFrameCount()	const;

	public:
		bool Init(std::shared_ptr<class CTexture> pTex, float fPlayRate = 1.f, float fMaxTime = 1.f);
		bool Init(const std::string& strTex, float fPlayRate = 1.f, float fMaxTime = 1.f);
		void Init(const TCHAR* pFileName, const std::string& strPathName = TEXTURE_PATH);
		void LoadFromFullPath(const TCHAR* pFileName);
		void SaveFromFullPath(const TCHAR* pFileName);
		void AddFrame(Vector2 vStart, Vector2 vEnd);
		void SetFrames();
		void AddAni2DNotify(const std::string& strTag, int iFrame);
		void Update(int iFrame);
		virtual std::shared_ptr<CAnimation2DSequence> Clone();

	public:
		virtual void Save(FILE* pFile);
		virtual void Load(FILE* pFile);
	};
