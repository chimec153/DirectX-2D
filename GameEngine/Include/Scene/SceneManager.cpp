#include "SceneManager.h"
#include "Scene.h"
#include "../PathManager.h"
#include "../Object/GameObject.h"
#include "../Input.h"
#include "../RenderManager.h"
#include "../UI/imgui/imgui.h"
#include "../UI/imgui/ImGuiFileDialog.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()	:
	m_pScene(nullptr),
	m_pSceneNext(nullptr)
{

}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_pScene);
	SAFE_DELETE(m_pSceneNext);
}

CScene* CSceneManager::GetScene() const
{
	return m_pScene;
}

CScene* CSceneManager::GetNextScene() const
{
	return m_pSceneNext;
}

bool CSceneManager::Init()
{
	SAFE_DELETE(m_pScene);

	m_pScene = new CScene;

	m_pScene->Init();

	m_pScene->m_EditorCreate = m_EditorCreate;
	m_pScene->m_EditorProtoCreate = m_EditorCreateProto;
	m_pScene->m_EditorCreateObj = m_EditorCreateObj;
	m_pScene->m_EditorCreateGameMode = m_EditorCreateGameMode;
	m_pScene->m_EditorDelete = m_EditorDelete;

	GET_SINGLE(CInput)->CreateMouse();

	return true;
}

void CSceneManager::Start()
{
	m_pScene->Start();
}

int CSceneManager::Input(float fTime)
{
	m_pScene->Input(fTime);

	return ChangeScene();
}

int CSceneManager::Update(float fTime)
{
	m_pScene->Update(fTime);

	if (m_EditorUpdate)
		m_EditorUpdate(fTime);

	return ChangeScene();
}

int CSceneManager::PostUpdate(float fTime)
{
	m_pScene->PostUpdate(fTime);

	return ChangeScene();
}

int CSceneManager::Collision(float fTime)
{
	m_pScene->Collision(fTime);

	return ChangeScene();
}

void CSceneManager::CheckFrustum()
{
	m_pScene->CheckFrustum();
}

int CSceneManager::PreRender(float fTime)
{
	m_pScene->PreRender(fTime);

	return ChangeScene();
}

int CSceneManager::Render(float fTime)
{
	m_pScene->Render(fTime);

	return ChangeScene();
}

int CSceneManager::PostRender(float fTime)
{
	m_pScene->PostRender(fTime);

	return ChangeScene();
}

void CSceneManager::Save(const char* pFileName, const std::string& strRootPath)
{
	char strFullPath[MAX_PATH] = {};

	const char* pRootPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strRootPath);

	if (pRootPath)
		strcpy_s(strFullPath, pRootPath);

	strcat_s(strFullPath, pFileName);
		
	SaveFromFullPath(strFullPath);
}

void CSceneManager::SaveFromFullPath(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (!pFile)
		return;

	m_pScene->Save(pFile);

	fclose(pFile);
}

void CSceneManager::Load(const char* pFileName, const std::string& strRootPath)
{
	char strFullPath[MAX_PATH] = {};

	const char* pRootPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strRootPath);

	if (pRootPath)
		strcpy_s(strFullPath, pRootPath);

	strcat_s(strFullPath, pFileName);

	LoadFromFullPath(strFullPath);
}

void CSceneManager::LoadFromFullPath(const char* pFullPath)
{
	CreateNextScene();

	m_pSceneNext->LoadFromFullPath(pFullPath);
}

void CSceneManager::CreateNextScene()
{
	SAFE_DELETE(m_pSceneNext);

	m_pSceneNext = new CScene;

	m_pSceneNext->m_eType = SCENE_TYPE::ST_NEXT;
	m_pSceneNext->m_EditorCreate = m_EditorCreate;
	m_pSceneNext->m_EditorProtoCreate = m_EditorCreateProto;
	m_pSceneNext->m_EditorCreateObj = m_EditorCreateObj;
	m_pSceneNext->m_EditorCreateGameMode = m_EditorCreateGameMode;
	m_pSceneNext->m_EditorDelete = m_EditorDelete;

	m_pSceneNext->Init();
}

bool CSceneManager::ChangeScene()
{
	if (m_pSceneNext)
	{
		SAFE_DELETE(m_pScene);

		m_pScene = m_pSceneNext;
		m_pScene->m_eType = SCENE_TYPE::ST_CURRENT;
		m_pSceneNext = nullptr;

		//bool bShow = GET_SINGLE(CInput)->GetShow();

		GET_SINGLE(CInput)->CreateMouse();

		//GET_SINGLE(CInput)->SetShow(bShow);

		GET_SINGLE(CRenderManager)->Clear();

		GET_SINGLE(CRenderManager)->ChangeScene();

		m_pScene->ChangeProto();

		m_pScene->Start();

		return true;
	}

	return false;
}

CScene* CSceneManager::CreateScene()
{
	CScene* pScene = new CScene;

	pScene->m_eType = SCENE_TYPE::ST_NEXT;
	pScene->m_EditorCreate = m_EditorCreate;
	pScene->m_EditorProtoCreate = m_EditorCreateProto;
	pScene->m_EditorCreateObj = m_EditorCreateObj;
	pScene->m_EditorCreateGameMode = m_EditorCreateGameMode;
	pScene->m_EditorDelete = m_EditorDelete;

	pScene->Init();

	return pScene;
}

void CSceneManager::SetNextScene(CScene* pScene)
{
	SAFE_DELETE(m_pSceneNext);

	m_pSceneNext = pScene;
}

void CSceneManager::ShowWindow()
{
	if (ImGui::Begin("SceneManager"))
	{
		if (m_pScene)
		{
			m_pScene->ShowWindow();
		}
		static int iType = -1;

		ImGui::InputInt("Scene Index", &iType);

		if (ImGui::Button("Create Scene"))
		{
			if (iType != -1)
			{
				CScene* pScene = CreateScene();
				SetNextScene(pScene);
			}
		}

		static bool bSave = false;

		if (ImGui::Button("Save Scene"))
		{
			const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(DATA_PATH);
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".scn", pPath);
			bSave = true;
		}

		if (ImGui::Button("Load Scene"))
		{
			const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(DATA_PATH);
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".scn", pPath);
			bSave = false;
		}

		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string strFileName = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string strPath = ImGuiFileDialog::Instance()->GetCurrentPath();

				if (bSave)
				{
					m_pScene->SaveFromFullPath(strFileName.c_str());
				}

				else
				{
					CScene* pScene = CreateScene();
					pScene->LoadFromFullPath(strFileName.c_str());
					SetNextScene(pScene);
				}
			}

			ImGuiFileDialog::Instance()->Close();
		}
	}
	ImGui::End();
}
