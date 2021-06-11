#include "Layer.h"
#include "../Object/GameObject.h"
#include "../LayerInstance.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"
#include "../RenderManager.h"
#include "../UI/imgui/imgui.h"
#include "../Scene/Scene.h"
#include "../Component/ColliderOBB.h"
#include "../Input.h"
#include "../CameraManager.h"
#include "../Component/Camera.h"
#include "../Engine.h"

CLayer::CLayer()	:
	m_iZOrder(0),
	m_bStart(false),
	m_pScene(nullptr)
{
	m_pInstance = new CLayerInstance;
}

CLayer::~CLayer()
{
	SAFE_DELETE(m_pInstance);
}

void CLayer::SetZOrder(int iZ)
{
	m_iZOrder = iZ;
}

int CLayer::GetZOrder() const
{
	return m_iZOrder;
}

void CLayer::AddObj(std::shared_ptr<CGameObject> pObj)
{
	pObj->SetLayer(this);
	pObj->SetScene(m_pScene);
	m_ObjList.push_back(pObj);
}

void CLayer::DeleteEditorDelete()
{
	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->m_EditorDelete = nullptr;
}

std::shared_ptr<CGameObject> CLayer::FindObj(const std::string& strKey)
{
	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == strKey)
		{
			return (*iter);
		}
	}

	return nullptr;
}

bool CLayer::IsStart() const
{
	return m_bStart;
}

CLayerInstance* CLayer::GetInstance() const
{
	return m_pInstance;
}

void CLayer::DeleteObject(const std::string& strKey)
{
	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == strKey)
		{
			m_ObjList.erase(iter);
			return;
		}
	}
}

void CLayer::DeleteObject(const std::shared_ptr<class CGameObject>& pObj)
{
	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter== pObj)
		{
			m_ObjList.erase(iter);
			return;
		}
	}
}

CScene* CLayer::GetScene() const
{
	return m_pScene;
}

bool CLayer::Init()
{

	return true;
}

void CLayer::Start()
{
	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	m_bStart = true;
}

void CLayer::Input(float fTime)
{
	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
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

void CLayer::Update(float fTime)
{
	//m_ObjList.sort(CLayer::SortY);

	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
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
}

void CLayer::PostUpdate(float fTime)
{
	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
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
}

void CLayer::Collision(float fTime)
{
	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
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
}

void CLayer::CheckFrustum()
{
	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
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

void CLayer::PreRender(float fTime)
{
	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
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

	GET_SINGLE(CRenderManager)->AddLayer(this);
}

void CLayer::Render(float fTime)
{/*
	std::list<class CGameObject*>::iterator iter = m_ObjList.begin();
	std::list<class CGameObject*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(fTime);
		++iter;
	}*/

	m_pInstance->Render();
}

void CLayer::PostRender(float fTime)
{
	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
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
}

void CLayer::Save(FILE* pFile)
{
	CRef::Save(pFile);

	size_t iSz = m_ObjList.size();

	fwrite(&iSz, sizeof(size_t), 1, pFile);

	std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
	std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		int iType = (*iter)->GetObjType();

		fwrite(&iType, 4, 1, pFile);

		(*iter)->Save(pFile);
	}

	fwrite(&m_iZOrder, 4, 1, pFile);
}

void CLayer::Load(FILE* pFile)
{
	CRef::Load(pFile);

	size_t iSz = 0;

	fread(&iSz, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iSz; ++i)
	{
		std::shared_ptr<CGameObject> pObj = nullptr;

		OBJECT_CLASS_TYPE eType = OBJECT_CLASS_TYPE::OCT_PLAYER;

		fread(&eType, 4, 1, pFile);
		if (m_EditorObjCreate)
		{
			pObj = std::shared_ptr<CGameObject>(m_EditorObjCreate((int)eType));
		}
		else
		{
			pObj = std::shared_ptr<CGameObject>(new CGameObject);
		}

		pObj->m_pScene = m_pScene;
		pObj->m_pLayer = this;

		pObj->Load(pFile);

		pObj->m_EditorDelete = m_EditorDelete;

		if (m_EditorCreate)
			m_EditorCreate(pObj->GetName());

		m_ObjList.push_back(pObj);
	}
	fread(&m_iZOrder, 4, 1, pFile);
}

void CLayer::ShowWindow()
{
	if (!m_pAxisObj && GET_SINGLE(CEngine)->IsEditor())
	{
		m_pAxisObj = m_pScene->CreateObject<CGameObject>("SelectObj", this);

		std::shared_ptr<CComponent> pPlaneXZ = m_pAxisObj->CreateComponent<CColliderOBB>("XZ");
		m_pAxisObj->SetRootComponent(pPlaneXZ);

		static_cast<CColliderOBB*>(pPlaneXZ.get())->SetExtent(Vector3(1.f, 1.f, 1.f));

		std::shared_ptr<CComponent> pAxisX = m_pAxisObj->CreateComponent<CColliderOBB>("X");

		static_cast<CSceneComponent*>(pPlaneXZ.get())->AddChild(pAxisX);

		static_cast<CColliderOBB*>(pAxisX.get())->SetExtent(Vector3(50.f, 10.f, 10.f));
		static_cast<CColliderOBB*>(pAxisX.get())->SetRelativePosX(25.f);

		static_cast<CColliderOBB*>(pAxisX.get())->SetCallBack<CLayer>(
			COLLISION_STATE::CS_INIT, this, &CLayer::CollisionInit);
		static_cast<CColliderOBB*>(pAxisX.get())->SetCallBack<CLayer>(
			COLLISION_STATE::CS_STAY, this, &CLayer::CollisionStay);
		static_cast<CColliderOBB*>(pAxisX.get())->SetCallBack<CLayer>(
			COLLISION_STATE::CS_LAST, this, &CLayer::CollisionEnd);

		pAxisX = m_pAxisObj->CreateComponent<CColliderOBB>("Y");

		static_cast<CSceneComponent*>(pPlaneXZ.get())->AddChild(pAxisX);

		static_cast<CColliderOBB*>(pAxisX.get())->SetExtent(Vector3(10.f, 50.f, 10.f));
		static_cast<CColliderOBB*>(pAxisX.get())->SetRelativePosY(25.f);

		static_cast<CColliderOBB*>(pAxisX.get())->SetCallBack<CLayer>(
			COLLISION_STATE::CS_INIT, this, &CLayer::CollisionInit);
		static_cast<CColliderOBB*>(pAxisX.get())->SetCallBack<CLayer>(
			COLLISION_STATE::CS_STAY, this, &CLayer::CollisionStay);
		static_cast<CColliderOBB*>(pAxisX.get())->SetCallBack<CLayer>(
			COLLISION_STATE::CS_LAST, this, &CLayer::CollisionEnd);

		pAxisX = m_pAxisObj->CreateComponent<CColliderOBB>("Z");

		static_cast<CSceneComponent*>(pPlaneXZ.get())->AddChild(pAxisX);

		static_cast<CColliderOBB*>(pAxisX.get())->SetExtent(Vector3(10.f, 10.f, 50.f));
		static_cast<CColliderOBB*>(pAxisX.get())->SetRelativePosZ(25.f);

		static_cast<CColliderOBB*>(pAxisX.get())->SetCallBack<CLayer>(
			COLLISION_STATE::CS_INIT, this, &CLayer::CollisionInit);
		static_cast<CColliderOBB*>(pAxisX.get())->SetCallBack<CLayer>(
			COLLISION_STATE::CS_STAY, this, &CLayer::CollisionStay);
		static_cast<CColliderOBB*>(pAxisX.get())->SetCallBack<CLayer>(
			COLLISION_STATE::CS_LAST, this, &CLayer::CollisionEnd);
	}

	int iSize = static_cast<int>(m_ObjList.size());
	const char** pText = new const char* [iSize];
	
	if (ImGui::Begin("Layer"))
	{
		ImGui::Text(GetName().c_str());
		std::list<std::shared_ptr<CGameObject>>::iterator iter = m_ObjList.begin();
		std::list<std::shared_ptr<CGameObject>>::iterator iterEnd = m_ObjList.end();

		for (int i=0; iter != iterEnd; ++iter,++i)
		{
			pText[i] = (*iter)->GetName().c_str();
		}

		static int iSel = -1;

		ImGui::ListBox("Objects", &iSel, pText, iSize);

		if (iSel != -1 && iSize > iSel)
		{
			std::shared_ptr<CGameObject> pObj = FindObj(pText[iSel]);

			if (pObj)
			{
				pObj->ShowWindow();

				m_pAxisObj->SetWorldPos(pObj->GetWorldPos());
				m_pSelectObj = pObj;
			}
		}
		else
		{
			m_pSelectObj = nullptr;
		}
		static char strName[MAX_PATH] = {};
		ImGui::InputText("Name: ", strName, MAX_PATH);

		if (ImGui::Button("Create Object"))
		{
			std::shared_ptr<CGameObject> pObj = m_pScene->CreateObject<CGameObject>(strName, this);
		}
	}
	ImGui::End();
	delete[] pText;
}

bool CLayer::SortY(std::shared_ptr<CGameObject> pSrc, std::shared_ptr<CGameObject> pDst)
{
	return pSrc->GetWorldPos().y < pDst->GetWorldPos().y;
}

void CLayer::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CLayer::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (m_pSelectObj)
	{
		if (GET_SINGLE(CInput)->IsPress(MOUSE_TYPE::LEFT))
		{
			if (pDest->GetName() == "MouseBody")
			{
				std::string strName = pSrc->GetName();

				if (strName == "X")
				{
					Vector2 vMove = GET_SINGLE(CInput)->GetMouseMove();

					Vector3 vWorld = Vector3(vMove.x, vMove.y, 0.f);

					CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

					Matrix matVP = pCam->GetVP();

					matVP.Inverse();

					vWorld.x = vMove.x * matVP[0][0] + vMove.y * matVP[1][0];
					vWorld.y = vMove.x * matVP[0][1] + vMove.y * matVP[1][1];
					vWorld.z = vMove.x * matVP[0][2] + vMove.y * matVP[1][2];

					m_pSelectObj->AddWorldPosX(vWorld.Dot(Vector3::Axis[AXIS_X]));
				}

				else if (strName == "Y")
				{
					Vector2 vMove = GET_SINGLE(CInput)->GetMouseMove();

					Vector3 vWorld = Vector3(vMove.x, vMove.y, 0.f);

					CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

					Matrix matVP = pCam->GetVP();

					matVP.Inverse();

					vWorld.x = vMove.x * matVP[0][0] + vMove.y * matVP[1][0];
					vWorld.y = vMove.x * matVP[0][1] + vMove.y * matVP[1][1];
					vWorld.z = vMove.x * matVP[0][2] + vMove.y * matVP[1][2];

					m_pSelectObj->AddWorldPosY(vWorld.Dot(Vector3::Axis[AXIS_Y]));
				}

				else if (strName == "Z")
				{
					Vector2 vMove = GET_SINGLE(CInput)->GetMouseMove();

					Vector3 vWorld = Vector3(vMove.x, vMove.y, 0.f);

					CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

					Matrix matVP = pCam->GetVP();

					matVP.Inverse();

					vWorld.x = vMove.x * matVP[0][0] + vMove.y * matVP[1][0];
					vWorld.y = vMove.x * matVP[0][1] + vMove.y * matVP[1][1];
					vWorld.z = vMove.x * matVP[0][2] + vMove.y * matVP[1][2];
					m_pSelectObj->AddWorldPosZ(vWorld.Dot(Vector3::Axis[AXIS_Z]));
				}
			}
		}
	}
}

void CLayer::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
