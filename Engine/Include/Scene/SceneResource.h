#pragma once

#include "../GameEngine.h"

	class CSceneResource
	{
		friend class CScene;

	private:
		CSceneResource();
		~CSceneResource();

	private:
		std::unordered_map<std::string, class CMesh*>					m_mapMesh;
		std::unordered_map<std::string, class CShader*>					m_mapShader;
		std::unordered_map<std::string, class CMaterial*>				m_mapMaterial;
		std::unordered_map<std::string, class CTexture*>				m_mapTexture;
		std::unordered_map<std::string, class CAnimation2DSequence*>	m_mapAni2DSequence;

	public:
		bool Init();

		class CMesh* FindMesh(const std::string& strName);

	public:
		class CMaterial* FindMaterial(const std::string& strName);

	public:
		class CTexture* FindTexture(const std::string& strTag);
		bool LoadTexture(const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath = TEXTURE_PATH);
		bool LoadTextureFromFullPath(const std::string& strTag, const TCHAR* pFileName);
		bool LoadTexture(const std::string& strTag,
			const std::vector<const TCHAR*>& vecFileName,
			const std::string& strRootPath = TEXTURE_PATH);
		bool LoadTextureFromFullPath(const std::string& strTag,
			const std::vector<const TCHAR*>& vecFullPath);

	public:
		bool CreateAni2DSequence(const std::string& strTag, class CTexture* pTex, float fPlayRate = 1.f, float fMaxTime = 1.f);
		bool LoadAni2DSequence(const std::string& strTag, const TCHAR* pFileName, const std::string& strPathName = RESOURCE_PATH);
		void AddAniFrame(const std::string& strTag, const Vector2& vStart, const Vector2& vEnd);
		class CAnimation2DSequence* FindAni2DSequence(const std::string& strTag);
		void AddAni2DNotify(const std::string& strSeq, const std::string& strNot, int iFrame);

	public:
		void Save(FILE* pFile);
		void Load(FILE* pFile);
	};