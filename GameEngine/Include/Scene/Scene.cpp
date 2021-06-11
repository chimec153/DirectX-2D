#include "Scene.h"
#include "../Object/GameObject.h"
#include "SceneCollision.h"
#include "../Layer/Layer.h"
#include "../UI/imgui/imgui.h"

std::unordered_map<std::string, std::shared_ptr<CGameObject>> CScene::m_mapProtoType[(int)SCENE_TYPE::ST_END];
std::function<void(const std::string&)> CScene::m_EditorProtoCreate = nullptr;

CScene::CScene()	:
	m_eType(SCENE_TYPE::ST_CURRENT)
{
	m_pGameMode = new CGameMode;
	m_pCollision = new CSceneCollision;

	CreateLayer(TERRAIN_LAYER);
	CreateLayer(OUTLINE_LAYER,		1);
	CreateLayer(ALBEDO_LAYER,		2);
	CreateLayer(COLLIDER_LAYER,		3);
	CreateLayer(DECAL_LAYER,		4);
	CreateLayer(GBUFFER_LAYER,		5);
	CreateLayer(CUBEMAP_LAYER,		6);
	CreateLayer(ALPHA_LAYER,		7);
	CreateLayer(SWORD_LIGHT_LAYER,	8);
	CreateLayer(PARTICLE_LAYER,		9);
	CreateLayer(UI_LAYER,			10);
}

CScene::~CScene()
{
	SAFE_DELETE(m_pGameMode);
	SAFE_DELETE_VECLIST(m_LayerList);
	SAFE_DELETE(m_pCollision);
}

SCENE_TYPE CScene::GetSceneType() const
{
	return m_eType;
}

std::shared_ptr<CGameObject> CScene::CreateCloneObj(const std::string& strTag, const std::string& strProto, CLayer* pLayer, SCENE_TYPE eType)
{
	std::shared_ptr<CGameObject> pProto = FindProtoType(strProto, eType);

	if (!pProto)
		return nullptr;

	pProto->m_pLayer = pLayer;

	std::shared_ptr<CGameObject> pClone = pProto->Clone();

	pClone->SetName(strTag);

	pLayer->AddObj(pClone);

	if (m_EditorCreate)
		m_EditorCreate(strTag);

	pClone->m_EditorDelete = m_EditorDelete;

	return pClone;
}

CLayer* CScene::FindLayer(const std::string& strTag)	const
{
	std::list<CLayer*>::const_iterator iter = m_LayerList.begin();
	std::list<CLayer*>::const_iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter))
		{
			if ((*iter)->GetName() == strTag)
			{
				return (*iter);
			}
		}
	}

	return nullptr;
}

CLayer* CScene::FindLayer(int iZOrder) const
{
	std::list<CLayer*>::const_iterator iter = m_LayerList.begin();
	std::list<CLayer*>::const_iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter))
		{
			if ((*iter)->GetZOrder() == iZOrder)
			{
				return (*iter);
			}
		}
	}
	return nullptr;
}

void CScene::CreateLayer(const std::string& strTag, int iZOrder)
{
	CLayer* pLayer = FindLayer(strTag);

	if (pLayer)
		return;

	pLayer = new CLayer;

	pLayer->m_pScene = this;
	pLayer->SetName(strTag);
	pLayer->SetZOrder(iZOrder);

	if (!pLayer->Init())
	{
		SAFE_DELETE(pLayer);
		return;
	}

	pLayer->m_EditorObjCreate = m_EditorCreateObj;
	pLayer->m_EditorCreate = m_EditorCreate;
	pLayer->m_EditorDelete = m_EditorDelete;

	m_LayerList.push_back(pLayer);

	m_LayerList.sort(CScene::SortZ);
}

void CScene::DeleteEditorDelete()
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DeleteEditorDelete();
	}

	m_EditorDelete = nullptr;
}
bool CScene::Init()
{
	if (!m_pCollision->Init())
		return false;

	return true;
}

void CScene::Start()
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->Start();

	m_pGameMode->Start();
}

void CScene::Input(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Input(fTime);
		++iter;
	}
}

void CScene::Update(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		else if (!(*iter)->IsStart())
			(*iter)->Start();

		(*iter)->Update(fTime);
		++iter;
	}

	if (!m_pGameMode->IsStart())
	{
		m_pGameMode->Start();
	}

	m_pGameMode->Update(fTime);
}

void CScene::PostUpdate(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(fTime);
		++iter;
	}

	m_pGameMode->PostUpdate(fTime);
}

void CScene::Collision(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Collision(fTime);
		++iter;
	}

	m_pCollision->Collision(fTime);

	m_pGameMode->Collision(fTime);
}

void CScene::CheckFrustum()
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->CheckFrustum();
		++iter;
	}
}

void CScene::PreRender(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PreRender(fTime);
		++iter;
	}

	m_pGameMode->PreRender(fTime);
}

void CScene::Render(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(fTime);
		++iter;
	}

	m_pGameMode->Render(fTime);
}

void CScene::PostRender(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostRender(fTime);
		++iter;
	}		

	m_pGameMode->PostRender(fTime);
}

void CScene::Save(FILE* pFile)
{
	int iSize = static_cast<int>(m_LayerList.size());

	fwrite(&iSize, 4, 1, pFile);

	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Save(pFile);
	}

	int iType = m_pGameMode->GetGameModeType();

	fwrite(&iType, sizeof(int), 1, pFile);

	m_pGameMode->Save(pFile);

	iSize = static_cast<int>(m_mapProtoType[static_cast<int>(SCENE_TYPE::ST_CURRENT)].size());

	fwrite(&iSize, 4, 1, pFile);

	std::unordered_map<std::string, std::shared_ptr<CGameObject>>::iterator iterP = m_mapProtoType[static_cast<int>(SCENE_TYPE::ST_CURRENT)].begin();
	std::unordered_map<std::string, std::shared_ptr<CGameObject>>::iterator iterPEnd = m_mapProtoType[static_cast<int>(SCENE_TYPE::ST_CURRENT)].end();

	for (; iterP != iterPEnd; ++iterP)
	{
		int iType = iterP->second->GetObjType();

		fwrite(&iType, 4, 1, pFile);

		iterP->second->Save(pFile);
	}
}

void CScene::SaveFromFullPath(const char* pFileName)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFileName, "wb");

	if (pFile)
	{
		Save(pFile);

		fclose(pFile);
	}
}

void CScene::Load(FILE* pFile)
{
	SAFE_DELETE_VECLIST(m_LayerList);

	int iSize = 0;

	fread(&iSize, 4, 1, pFile);

	for (int i = 0; i < iSize; i++)
	{
		CLayer* pLayer = new CLayer;

		pLayer->m_EditorObjCreate = m_EditorCreateObj;
		pLayer->m_EditorCreate = m_EditorCreate;
		pLayer->m_EditorDelete = m_EditorDelete;
		pLayer->m_pScene = this;

		pLayer->Load(pFile);

		m_LayerList.push_back(pLayer);
	}

	m_LayerList.sort(CScene::SortZ);

	int iType = 0;

	fread(&iType, sizeof(int), 1, pFile);

	SAFE_DELETE(m_pGameMode);

	m_pGameMode = EditorCreateGameMode(iType);

	m_pGameMode->m_pScene = this;

	m_pGameMode->Load(pFile);

	iSize = 0;

	fread(&iSize, 4, 1, pFile);

	for (int i=0;i<iSize;i++)
	{
		int iType = 0;

		fread(&iType, 4, 1, pFile);
		std::shared_ptr<CGameObject> pObj = nullptr;
		if (m_EditorCreateObj)
		{
			pObj = m_EditorCreateObj(iType);
		}
		else
		{
			pObj = std::shared_ptr<CGameObject>(new CGameObject);
		}

		pObj->Load(pFile);

		m_mapProtoType[static_cast<int>(SCENE_TYPE::ST_NEXT)].insert(std::make_pair(pObj->GetName(),pObj));
	}
}

void CScene::LoadFromFullPath(const char* pFileName)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFileName, "rb");

	if (!pFile)
		return;

	Load(pFile);

	fclose(pFile);
}

void CScene::ChangeProto()
{
	m_mapProtoType[(int)SCENE_TYPE::ST_CURRENT] = m_mapProtoType[(int)SCENE_TYPE::ST_NEXT];

	m_mapProtoType[(int)SCENE_TYPE::ST_NEXT].clear();
}

void CScene::ShowWindow()
{
	int iSize = static_cast<int>(m_LayerList.size());
	const char** pText = new const char* [iSize];

	if (ImGui::Begin("Scene"))
	{
		std::list<CLayer*>::iterator iter = m_LayerList.begin();
		std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

		for (int i=0; iter != iterEnd; ++iter, ++i)
		{
			pText[i] = (*iter)->GetName().c_str();
		}
		static int iSel = -1;
		ImGui::ListBox("Layers", &iSel,pText, iSize);

		if (iSel != -1)
		{
			FindLayer(pText[iSel])->ShowWindow();
		}

		std::unordered_map<std::string, std::shared_ptr<CGameObject>>::iterator iterC =
			m_mapProtoType[static_cast<int>(SCENE_TYPE::ST_CURRENT)].begin();
		std::unordered_map<std::string, std::shared_ptr<CGameObject>>::iterator iterCEnd = 
			m_mapProtoType[static_cast<int>(SCENE_TYPE::ST_CURRENT)].end();

		int iProto = static_cast<int>(m_mapProtoType[static_cast<int>(SCENE_TYPE::ST_CURRENT)].size());

		std::vector<const char*> vecProtoType(iProto);

		for (int i=0; iterC != iterCEnd; ++iterC, i++)
		{
			vecProtoType[i] = iterC->second->GetName().c_str();
		}

		static int iSelect = -1;

		if (iProto)
		{
			ImGui::Text("Proto Types");

			ImGui::ListBox("Proto Type List", &iSelect, &vecProtoType[0], iProto);

			if (iSelect != -1 && iSelect < iProto)
			{
				std::shared_ptr<CGameObject> pProto = FindProtoType(vecProtoType[iSelect], SCENE_TYPE::ST_CURRENT);

				if (pProto)
				{
					pProto->ShowWindow();
				}
			}
		}
	}
	ImGui::End();

	delete[] pText;
}

bool CScene::SortZ(CLayer* pSrc, CLayer* pDst)
{
	return pSrc->GetZOrder() < pDst->GetZOrder();
}
