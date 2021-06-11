#pragma once

#include "../GameEngine.h"
#include "Scene.h"

	class CSceneManager
	{
	private:
		class CScene* m_pScene;
		class CScene* m_pSceneNext;

	private:
		std::function<void(float)> m_EditorUpdate;
		std::function<void(const std::string&)>		m_EditorCreate;
		std::function<void(const std::string&)>		m_EditorDelete;
		std::function<void(const std::string&)>		m_EditorCreateProto;
		std::function<std::shared_ptr<class CGameObject> (int)>		m_EditorCreateObj;
		std::function<class CGameMode* (int)>		m_EditorCreateGameMode;

	public:
		class CScene* GetScene()	const;
		class CScene* GetNextScene()	const;

	public:
		bool Init();
		void Start();
		int Input(float fTime);
		int Update(float fTime);
		int PostUpdate(float fTime);
		int Collision(float fTime);
		void CheckFrustum();
		int PreRender(float fTime);
		int Render(float fTime);
		int PostRender(float fTime);

	public:
		template <typename T>
		bool SetGameMode(bool bCurrent = true)
		{
			if (bCurrent)
				return m_pScene->SetGameMode<T>();

			return m_pSceneNext->SetGameMode<T>();
		}

	public:
		template <typename T>
		void SetEditorUpdateFunc(T* pObj, void(T::* pFunc)(float))
		{
			m_EditorUpdate = std::bind(pFunc, pObj, std::placeholders::_1);
		}

		template <typename T>
		void SetEditorCreateFunc(T* pObj, void(T::* pFunc)(const std::string&))
		{
			m_EditorCreate = std::bind(pFunc, pObj, std::placeholders::_1);
		}

		template <typename T>
		void SetEditorProtoCreateFunc(T* pObj, void(T::* pFunc)(const std::string&))
		{
			m_EditorCreateProto = std::bind(pFunc, pObj, std::placeholders::_1);
		}

		template <typename T>
		void SetEditorDeleteFunc(T* pObj, void(T::* pFunc)(const std::string&))
		{
			m_EditorDelete = std::bind(pFunc, pObj, std::placeholders::_1);
		}

		template <typename T>
		void SetEditorCreateObjFunc(T* pObj, std::shared_ptr<class CGameObject> (T::* pFunc)(int))
		{
			m_EditorCreateObj = std::bind(pFunc, pObj, std::placeholders::_1);
		}

		template <typename T>
		void SetEditorCreateGameModeFunc(T* pObj, class CGameMode* (T::* pFunc)(int))
		{
			m_EditorCreateGameMode = std::bind(pFunc, pObj, std::placeholders::_1);
		}

	public:
		void Save(const char* pFileName, const std::string& strRootPath = DATA_PATH);
		void SaveFromFullPath(const char* pFullPath);
		void Load(const char* pFileName, const std::string& strRootPath = DATA_PATH);
		void LoadFromFullPath(const char* pFullPath);

	public:
		void CreateNextScene();
		bool ChangeScene();
		class CScene* CreateScene();
		void SetNextScene(class CScene* pScene);

	public:
		void ShowWindow();

		DECLARE_SINGLE(CSceneManager)
	};