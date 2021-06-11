#include "SceneComponent.h"
#include "../Resource/Shader.h"
#include "../Scene/Scene.h"
#include "Transform.h"
#include "../RenderManager.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh.h"
#include "../Resource/ResourceManager.h"
#include "../Render/PostProcess.h"
#include "../Resource/ShaderManager.h"
#include "../UI/imgui/imgui.h"
#include "ObjectComponent.h"
#include "Navigation2D.h"
#include "PaperBurn.h"
#include "../Engine.h"
#include "../CameraManager.h"
#include "Camera.h"
#include "Renderer.h"
#include "../Resource/Animation.h"

CSceneComponent::CSceneComponent()	:
	m_eSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D),
	m_pParent(nullptr),
	m_iSceneComponentClassType(0),
	m_iZOrder(0),
	//m_pMesh(nullptr),
	//m_pMaterial(nullptr),
	m_bInstance(false),
	m_bPostProcess(false),
	m_bUIEnable(false),
	m_bTreeOpen(false),
	m_bInFrustum(false),
	m_bFrustumCulling(true),
	m_pRenderer(nullptr),
	m_bCastShadow(false)
{
	m_eType = COMPONENT_TYPE::CT_SCENE;
	m_pTransform = std::shared_ptr<CTransform>(new CTransform);

	m_pTransform->m_pOwner = this;

	m_pRenderer = new CRenderer;

	m_pRenderer->SetOwner(this);
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)	:
	CComponent(com)
	, m_bUIEnable(com.m_bUIEnable)
	, m_bTreeOpen(com.m_bTreeOpen)
	, m_bInFrustum(com.m_bInFrustum)
	, m_bFrustumCulling(com.m_bFrustumCulling)
	, m_pRenderer(com.m_pRenderer)
	, m_bCastShadow(com.m_bCastShadow)
{
	m_eSceneComponentType = com.m_eSceneComponentType;
	m_iSceneComponentClassType = com.m_iSceneComponentClassType;
	m_iZOrder = com.m_iZOrder;

	m_pTransform = com.m_pTransform->Clone();

	m_pTransform->m_pOwner = this;
	m_pTransform->m_pScene = m_pScene;

	m_pParent = nullptr;

	m_ChildList.clear();

	std::list<std::shared_ptr<CComponent>>::const_iterator iter = com.m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::const_iterator iterEnd = com.m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		CSceneComponent* _pCom = static_cast<CSceneComponent*>((*iter).get());
		_pCom->m_pLayer = com.m_pLayer;

		std::shared_ptr<CComponent> pComponent = (*iter).get()->Clone();

		CSceneComponent* pCom = (CSceneComponent*)pComponent.get();

		pCom->m_pScene = m_pScene;
		pCom->m_pObj = m_pObj;
		pCom->m_pParent = this;
		pCom->m_pTransform->m_pParent = m_pTransform.get();

		m_ChildList.push_back(pComponent);
		m_pTransform->m_ChildList.push_back(pCom->m_pTransform.get());
	}

	/*m_pMesh = com.m_pMesh;

	if (com.m_pMaterial)
		m_pMaterial = com.m_pMaterial->Clone();

	else
		m_pMaterial = nullptr;*/

	m_bInstance = com.m_bInstance;
	m_bPostProcess = com.m_bPostProcess;

	if (m_pRenderer)
	{
		m_pRenderer = m_pRenderer->Clone();
	    CAnimation* pAni  =m_pRenderer->GetAnimation(); 

		if (pAni)
		{
			pAni->SetOwner(this);
			pAni->Release();
		}
	}
}

CSceneComponent::~CSceneComponent()
{
	SAFE_RELEASE_VECLIST(m_vecPostProcess);
	SAFE_RELEASE(m_pRenderer);
}

void CSceneComponent::SetMaterial(const std::shared_ptr<CMaterial>& pMaterial, int iContainer, int iSubset)
{
	if (!m_pRenderer)
		return;

	m_pRenderer->SetMaterial(pMaterial, iContainer, iSubset);

	//m_pMaterial = pMaterial;
}

void CSceneComponent::SetMaterial(const std::string& strKey, int iContainer, int iSubset)
{
	if (!m_pRenderer)
		return;

	m_pRenderer->SetMaterial(strKey, iContainer, iSubset);
}

std::shared_ptr<CMaterial> CSceneComponent::GetMaterial(int iContainer, int iSubset) const
{
	if (!m_pRenderer)
		return nullptr;

	return m_pRenderer->GetMaterial(iContainer, iSubset);
}

class CMesh* CSceneComponent::GetMesh() const
{
	if (!m_pRenderer)
		return nullptr;

	return m_pRenderer->GetMesh();
}

void CSceneComponent::SetMesh(const std::string& strName)
{
	if (m_pRenderer)
		m_pRenderer->SetMesh(strName);

	/*m_pMesh = GET_SINGLE(CResourceManager)->FindMesh(strName);

	if (m_pMesh)
	{
		std::shared_ptr<CMaterial> pMaterial = m_pMesh->GetMaterial();

		if (pMaterial)
		{
			std::shared_ptr<CMaterial> pNewMaterial = pMaterial->Clone();

			SetMaterial(pNewMaterial);
		}

		SetMeshSize(m_pMesh->GetMax() - m_pMesh->GetMin());
		m_pTransform->SetMeshPos((m_pMesh->GetMax() + m_pMesh->GetMin()) / 2.f);
	}*/
}

void CSceneComponent::SetMesh(class CMesh* pMesh)
{
	if (m_pRenderer)
		m_pRenderer->SetMesh(pMesh);

	/*m_pMesh = pMesh;

	if (m_pMesh)
	{
		std::shared_ptr<CMaterial> pMaterial = m_pMesh->GetMaterial();
		std::shared_ptr<CMaterial> pNewMaterial = pMaterial->Clone();

		SetMaterial(pNewMaterial);

		SetMeshSize(m_pMesh.get()->GetMax() - m_pMesh.get()->GetMin());
		m_pTransform->SetMeshPos((m_pMesh->GetMax() + m_pMesh->GetMin()) / 2.f);
	}*/
}

bool CSceneComponent::IsInstance() const
{
	return m_bInstance;
}

void CSceneComponent::SetInstance(bool bInst)
{
	m_bInstance = bInst;
}

bool CSceneComponent::IsPostProcess() const
{
	return m_bPostProcess;
}

void CSceneComponent::SetShader(const std::string& strKey, int iContainer, int iSubset)
{
	if (m_pRenderer)
		m_pRenderer->SetShader(strKey,iContainer, iSubset);
}

void CSceneComponent::SetShader(const std::shared_ptr<CShader>& pShader, int iContainer, int iSubset)
{
	if (m_pRenderer)
		m_pRenderer->SetShader(pShader, iContainer, iSubset);
}

std::shared_ptr<class CShader> CSceneComponent::GetShader(int iContainer, int iSubset)
{
	if (m_pRenderer)
		return m_pRenderer->GetShader(iContainer, iSubset);

	return nullptr;
}

void CSceneComponent::AddRenderState(const std::string& strKey, int iContainer, int iSubset)
{
	if (m_pRenderer)
		m_pRenderer->AddRenderState(strKey, iContainer, iSubset);
}

void CSceneComponent::AddRenderState(CRenderState* pState, int iContainer, int iSubset)
{
	if (m_pRenderer)
		m_pRenderer->AddRenderState(pState, iContainer, iSubset);
}

CRenderInstancing* CSceneComponent::GetInstancing() const
{
	if (m_pRenderer)
		return m_pRenderer->GetInstancing();

	return nullptr;
}

int CSceneComponent::GetInstIndex() const
{
	if (m_pRenderer)
		return m_pRenderer->GetInstIndex();

	return -1;
}

void CSceneComponent::SetAnimation(CAnimation* pAni)
{
	if (m_pRenderer)
		m_pRenderer->SetAnimation(pAni);
}

size_t CSceneComponent::GetMaterialSize() const
{
	if (m_pRenderer)
		return m_pRenderer->GetMaterialSize();

	return 0;
}

size_t CSceneComponent::GetMaterialSubsetSize(int iContainer) const
{
	if (m_pRenderer)
		return m_pRenderer->GetMaterialSubsetSize(iContainer);

	return 0;
}

void CSceneComponent::ResizeMaterial(int iSize, int iContainer)
{
	if (m_pRenderer)
		m_pRenderer->ResizeMaterial(iSize, iContainer);
}

void CSceneComponent::ResizeShader(int iSize, int iContainer)
{
	if (m_pRenderer)
		m_pRenderer->ResizeShader(iSize, iContainer);
}

void CSceneComponent::ResizeRenderState(RENDER_STATE_TYPE eType, int iSize, int iContainer)
{
	if (m_pRenderer)
		m_pRenderer->ResizeRenderState(eType, iSize, iContainer);
}

void CSceneComponent::SetTexture(LINK_TYPE eType, std::shared_ptr<CTexture> pTexture, int iShaderType, int iRegister, int iContainer, int iSubSet)
{
	if (m_pRenderer)
		m_pRenderer->SetTexture(eType, pTexture, iShaderType, iRegister, iContainer, iSubSet);

	//m_pMaterial->SetTexture(eType, pTexture, iShaderType, iRegister);
}

void CSceneComponent::SetTexture(LINK_TYPE eType, const std::string& strTag, int iShaderType, int iRegister, int iContainer, int iSubSet)
{
	if (m_pRenderer)
		m_pRenderer->SetTexture(eType, strTag, iShaderType, iRegister, iContainer, iSubSet);
	//m_pMaterial->SetTexture(eType, strTag, iShaderType, iRegister);
}

void CSceneComponent::SetTexture(LINK_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath, int iShaderType, int iRegister, int iContainer, int iSubSet)
{
	if (m_pRenderer)
		m_pRenderer->SetTexture(eType, strTag, pFileName, strRootPath, iShaderType, iRegister, iContainer, iSubSet);
	//m_pMaterial->SetTexture(eType, strTag, pFileName, strRootPath, iShaderType, iRegister);
}

void CSceneComponent::SetTexture(LINK_TYPE eType, const std::string& strTag, const TCHAR* pFileName, int iShaderType, int iRegister, int iContainer, int iSubSet)
{
	if (m_pRenderer)
		m_pRenderer->SetTexture(eType, strTag, pFileName, iShaderType, iRegister, iContainer, iSubSet);
	//m_pMaterial->SetTexture(eType, strTag, pFileName, iShaderType, iRegister);
}

int CSceneComponent::GetZOrder() const
{
	return m_iZOrder;
}

void CSceneComponent::SetZOrder(int iZOrder)
{
	m_iZOrder = iZOrder;
}

std::shared_ptr<CTransform> CSceneComponent::GetTransform() const
{
	return m_pTransform;
}

SCENE_COMPONENT_TYPE CSceneComponent::GetSceneComponentType() const
{
	return m_eSceneComponentType;
}

void CSceneComponent::AddChild(const std::shared_ptr<CComponent>& pComponent)
{
	CSceneComponent* pCom = (CSceneComponent*)pComponent.get();

	pCom->m_pParent = this;
	pCom->m_pTransform->m_pParent = m_pTransform.get();
	
	m_ChildList.push_back(pComponent);
	m_pTransform->m_ChildList.push_back(pCom->m_pTransform.get());

	pCom->InheritPos();
	pCom->InheritScale();
	pCom->InheritRot();
}

void CSceneComponent::DeleteChild(const std::shared_ptr<CComponent>& pComponent)
{
	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (; iter != iterEnd; ++iter,++iterTr)
	{
		if (*iter == pComponent)
		{
			Detach(pComponent);

			m_ChildList.erase(iter);

			m_pTransform->m_ChildList.erase(iterTr);
			break;
		}
	}
}

const std::shared_ptr<CComponent> CSceneComponent::DeleteChild(CSceneComponent* pComponent)
{
	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (; iter != iterEnd; ++iter, ++iterTr)
	{
		if ((*iter).get() == pComponent)
		{
			pComponent->m_pParent = nullptr;
			pComponent->m_pTransform->m_pParent = nullptr;
			std::shared_ptr<CComponent> pCom = *iter;

			m_ChildList.erase(iter);
			m_pTransform->m_ChildList.erase(iterTr);

			return pCom;
		}
	}

	return nullptr;
}

void CSceneComponent::Detach(const std::shared_ptr<CComponent>& pComponent)
{
	CSceneComponent* pCom = (CSceneComponent*)pComponent.get();

	pCom->m_pParent = nullptr;
	pCom->m_pTransform->m_pParent = nullptr;
}

bool CSceneComponent::IsUIEnabled() const
{
	return m_bUIEnable;
}

void CSceneComponent::UIEnable(bool bEnable)
{
	m_bUIEnable = bEnable;
}

void CSceneComponent::UIEnableAll(bool bEnable)
{
	m_bUIEnable = bEnable;

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		static_cast<CSceneComponent*>((*iter).get())->UIEnableAll(bEnable);
	}
}

std::shared_ptr<CComponent> CSceneComponent::FindComponent(const std::string& strTag)
{
	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd= m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == strTag)
		{
			return *iter;
		}

		std::shared_ptr<CComponent> pCom = ((CSceneComponent*)(*iter).get())->FindComponent(strTag);

		if (pCom)
			return pCom;
	}

	std::list<std::shared_ptr<CComponent>>::iterator iterO = m_ObjComList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterOEnd = m_ObjComList.end();

	for (; iterO != iterOEnd; ++iterO)
	{
		if ((*iterO)->GetName() == strTag)
		{
			return *iterO;
		}
	}

	return nullptr;
}

int CSceneComponent::GetSceneComponentClassType() const
{
	return m_iSceneComponentClassType;
}

void CSceneComponent::SetObj(CGameObject* pObj)
{
	m_pObj = pObj;

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		CSceneComponent* pCom = (CSceneComponent*)(*iter).get();

		pCom->SetObj(pObj);
	}
}

void CSceneComponent::SetScene(CScene* pScene)
{
	m_pScene = pScene;

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->SetScene(pScene);
}

void CSceneComponent::SetLayer(CLayer* pLayer)
{
	if (m_pLayer)
	{
		m_pLayer = pLayer->GetScene()->FindLayer(m_pLayer->GetZOrder());
	}
	else
	{
		m_pLayer = pLayer;
	}

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		((CSceneComponent*)(*iter).get())->SetLayer(pLayer);
	}
}

void CSceneComponent::SetSceneComponentType(SCENE_COMPONENT_TYPE eType)
{
	m_eSceneComponentType = eType;
}

CSceneComponent* CSceneComponent::GetParent() const
{
	return m_pParent;
}

const TransformCBuffer& CSceneComponent::GetCBuffer() const
{
	return m_pTransform->GetCBuffer();
}

std::shared_ptr<CComponent> CSceneComponent::FindObjCom(const std::string& strKey) const
{
	std::list<std::shared_ptr<CComponent>>::const_iterator iter = m_ObjComList.begin();
	std::list<std::shared_ptr<CComponent>>::const_iterator iterEnd = m_ObjComList.end();

	for (;iter!=iterEnd;++iter)
	{
		if ((*iter)->GetName() == strKey)
		{
			return (*iter);
		}
	}

	return nullptr;
}

bool CSceneComponent::IsFrustumIn() const
{
	return m_bInFrustum;
}

CRenderer* CSceneComponent::GetRenderer() const
{
	return m_pRenderer;
}

CAnimation* CSceneComponent::GetAnimation() const
{
	if (!m_pRenderer)
		return nullptr;

	return m_pRenderer->GetAnimation();
}

void CSceneComponent::SetInstIndex(int iIndex)
{
	if (!m_pRenderer)
		return;

	m_pRenderer->SetInstIndex(iIndex);
}

void CSceneComponent::SetInstancing(CRenderInstancing* pInst)
{
	if (!m_pRenderer)
		return;

	m_pRenderer->SetInstancing(pInst);
}

void CSceneComponent::SetShadow(bool bCast)
{
	m_bCastShadow = bCast;
}

bool CSceneComponent::IsShadow() const
{
	return m_bCastShadow;
}

void CSceneComponent::SetShadowAll(bool bShadow)
{
	m_bCastShadow = bShadow;

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		static_cast<CSceneComponent*>((*iter).get())->SetShadowAll(bShadow);
	}
}

void CSceneComponent::SetFrustumCulling(bool bCull)
{
	m_bFrustumCulling = bCull;
}

void CSceneComponent::AddObjComponent(const std::shared_ptr<CComponent>& pCom)
{
	if (pCom)
	{
		if (pCom->GetType() == COMPONENT_TYPE::CT_OBJECT)
		{
			m_ObjComList.push_back(pCom);
			return;
		}
	}

	return;
}

bool CSceneComponent::Init()
{
	m_pTransform->m_pScene = m_pScene;

	return true;
}

void CSceneComponent::Start()
{
	m_pTransform->Start();

	if (m_pRenderer)
		m_pRenderer->Start();

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->Start();
}

void CSceneComponent::Update(float fTime)
{
	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();
	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_ChildList.erase(iter);
			iterEnd = m_ChildList.end();
			iterTr = m_pTransform->m_ChildList.erase(iterTr);
			continue;
		}

		else if ((*iter)->IsEnable())
		{
			(*iter)->Update(fTime);
		}

		++iter;
		++iterTr;
	}

	m_pTransform->Update(fTime);

	if (m_pRenderer)
		m_pRenderer->Update(fTime);
}

void CSceneComponent::PostUpdate(float fTime)
{
	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();
	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_ChildList.erase(iter);
			iterEnd = m_ChildList.end();
			iterTr = m_pTransform->m_ChildList.erase(iterTr);
			continue;
		}

		else if ((*iter)->IsEnable())
		{
			(*iter)->PostUpdate(fTime);
		}

		++iter;
		++iterTr;
	}

	m_pTransform->PostUpdate(fTime);

	if (m_pRenderer)
		m_pRenderer->PostUpdate(fTime);
}

void CSceneComponent::Collision(float fTime)
{
	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();
	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_ChildList.erase(iter);
			iterEnd = m_ChildList.end();
			iterTr = m_pTransform->m_ChildList.erase(iterTr);
			continue;
		}

		else if ((*iter)->IsEnable())
		{
			(*iter)->Collision(fTime);
		}

		++iter;
		++iterTr;
	}
}

void CSceneComponent::CheckFrustum()
{
	Vector3 vScale = GetWorldScale();
	Vector3 vPos = GetBoneWorldPos() + m_pTransform->GetMeshPos() * vScale;
	Vector3 vMeshSize = GetMeshSize() * vScale;

	float fRadius = vMeshSize.Length() / 2.f;

	CCamera* pCam = nullptr;

	switch (m_eSceneComponentType)
	{
	case SCENE_COMPONENT_TYPE::SCT_2D:
		pCam = GET_SINGLE(CCameraManager)->GetMainCam();
		break;
	case SCENE_COMPONENT_TYPE::SCT_3D:
		pCam = GET_SINGLE(CCameraManager)->GetMainCam();
		break;
	case SCENE_COMPONENT_TYPE::SCT_UI:
		pCam = GET_SINGLE(CCameraManager)->GetUICam();
		break;
	}
	if (pCam)
	{
		if (m_bFrustumCulling)
		{
			m_bInFrustum = pCam->FrustumInSphere(vPos, fRadius);
		}
		else
		{
			m_bInFrustum = true;
		}
	}
	else
	{
		m_bInFrustum = false;
	}

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_ChildList.erase(iter);
			iterEnd = m_ChildList.end();
			continue;
		}

		else if ((*iter)->IsEnable())
		{
			static_cast<CSceneComponent*>((*iter).get())->CheckFrustum();
		}

		++iter;
	}
}

void CSceneComponent::PreRender(float fTime)
{
	if(m_bInFrustum)
	GET_SINGLE(CRenderManager)->AddComponent(this);

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();
	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_ChildList.erase(iter);
			iterEnd = m_ChildList.end();
			iterTr = m_pTransform->m_ChildList.erase(iterTr);
			continue;
		}

		else if ((*iter)->IsEnable())
		{
			(*iter)->PreRender(fTime);
		}

		++iter;
		++iterTr;
	}

}

void CSceneComponent::RenderShadow(float fTime)
{
	if (!m_bCastShadow)
		return;

	PaperBurnCBuffer tCBuffer = {};

	GET_SINGLE(CShaderManager)->UpdateCBuffer("PaperBurn", &tCBuffer);

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ObjComList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ObjComList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjComList.erase(iter);
			iterEnd = m_ObjComList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		else if (!(*iter)->IsStart())
		{
			(*iter)->Start();
		}

		(*iter)->Render(fTime);
		++iter;
	}

	m_pTransform->SetTransform();

	if (m_pRenderer)
		m_pRenderer->RenderShadow(fTime);

	size_t iSize = m_vecPostProcess.size();

	for (size_t i = 0; i < iSize; i++)
	{
		m_vecPostProcess[i]->Render(fTime);
	}
}

void CSceneComponent::Render(float fTime)
{
	RenderObj(fTime);

	size_t iSize = m_vecPostProcess.size();

	for (size_t i = 0; i < iSize; i++)
	{
		m_vecPostProcess[i]->Render(fTime);
	}

	m_pTransform->SetTransform();

	if(m_pRenderer)
		m_pRenderer->Render(fTime);
}

void CSceneComponent::PostRender(float fTime)
{
	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();
	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_ChildList.erase(iter);
			iterEnd = m_ChildList.end();
			iterTr = m_pTransform->m_ChildList.erase(iterTr);
			continue;
		}

		else if ((*iter)->IsEnable())
		{
			(*iter)->PostRender(fTime);
		}

		++iter;
		++iterTr;
	}

	if (m_pRenderer)
		m_pRenderer->PostRender(fTime);

	size_t iSize = m_vecPostProcess.size();

	for (size_t i = 0; i < iSize; i++)
	{
		m_vecPostProcess[i]->PostRender(fTime);
	}
}

std::shared_ptr<CComponent> CSceneComponent::Clone()
{
	return std::shared_ptr<CComponent>(new CSceneComponent(*this));
}

void CSceneComponent::RenderObj(float fTime)
{
	PaperBurnCBuffer tCBuffer = {};

	GET_SINGLE(CShaderManager)->UpdateCBuffer("PaperBurn", &tCBuffer);

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ObjComList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ObjComList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjComList.erase(iter);
			iterEnd = m_ObjComList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		else if (!(*iter)->IsStart())
		{
			(*iter)->Start();
		}

		(*iter)->Render(fTime);
		++iter;
	}
}

void CSceneComponent::SetInheritScale(bool bInherit)
{
	m_pTransform->SetInheritScale(bInherit);
}

void CSceneComponent::SetInheritRotX(bool bInherit)
{
	m_pTransform->SetInheritRotX(bInherit);
}

void CSceneComponent::SetInheritRotY(bool bInherit)
{
	m_pTransform->SetInheritRotY(bInherit);
}

void CSceneComponent::SetInheritRotZ(bool bInherit)
{
	m_pTransform->SetInheritRotZ(bInherit);
}

void CSceneComponent::SetInheritPos(bool bInherit)
{
	m_pTransform->SetInheritPos(bInherit);
}

void CSceneComponent::InheritScale()
{
	m_pTransform->InheritScale();
}

void CSceneComponent::InheritRot()
{
	m_pTransform->InheritRot();
}

void CSceneComponent::InheritPos()
{
	m_pTransform->InheritPos();
}

void CSceneComponent::SetRelativeScale(const Vector3& v)
{
	m_pTransform->SetRelativeScale(v);
}

void CSceneComponent::SetRelativeScale(float x, float y, float z)
{
	m_pTransform->SetRelativeScale(x, y, z);
}

void CSceneComponent::SetRelativeRot(const Vector3& v)
{
	m_pTransform->SetRelativeRot(v);
}

void CSceneComponent::SetRelativeRot(float x, float y, float z)
{
	m_pTransform->SetRelativeRot(x, y, z);
}

void CSceneComponent::SetRelativeRotX(float x)
{
	m_pTransform->SetRelativeRotX(x);
}

void CSceneComponent::SetRelativeRotY(float y)
{
	m_pTransform->SetRelativeRotY(y);
}

void CSceneComponent::SetRelativeRotZ(float z)
{
	m_pTransform->SetRelativeRotZ(z);
}

void CSceneComponent::SetRelativePos(const Vector3 & v)
{
	m_pTransform->SetRelativePos(v);
}

void CSceneComponent::SetRelativePos(float x, float y, float z)
{
	m_pTransform->SetRelativePos(x, y, z);
}

void CSceneComponent::SetRelativePosX(float x)
{
	m_pTransform->SetRelativePosX(x);
}

void CSceneComponent::SetRelativePosY(float y)
{
	m_pTransform->SetRelativePosY(y);
}

void CSceneComponent::SetRelativePosZ(float z)
{
	m_pTransform->SetRelativePosZ(z);
}

void CSceneComponent::AddRelativeScale(const Vector3 & v)
{
	m_pTransform->AddRelativeScale(v);
}

void CSceneComponent::AddRelativeScale(float x, float y, float z)
{
	m_pTransform->AddRelativeScale(x, y, z);
}

void CSceneComponent::AddRelativeRot(const Vector3 & v)
{
	m_pTransform->AddRelativeRot(v);
}

void CSceneComponent::AddRelativeRot(float x, float y, float z)
{
	m_pTransform->AddRelativeRot(x, y, z);
}

void CSceneComponent::AddRelativeRotX(float x)
{
	m_pTransform->AddRelativeRotX(x);
}

void CSceneComponent::AddRelativeRotY(float y)
{
	m_pTransform->AddRelativeRotY(y);
}

void CSceneComponent::AddRelativeRotZ(float z)
{
	m_pTransform->AddRelativeRotZ(z);
}

void CSceneComponent::AddRelativePos(const Vector3 & v)
{
	m_pTransform->AddRelativePos(v);
}

void CSceneComponent::AddRelativePos(float x, float y, float z)
{
	m_pTransform->AddRelativePos(x, y, z);
}

void CSceneComponent::AddRelativePosX(float x)
{
	m_pTransform->AddRelativePosX(x);
}

void CSceneComponent::AddRelativePosY(float y)
{
	m_pTransform->AddRelativePosY(y);
}

void CSceneComponent::AddRelativePosZ(float z)
{
	m_pTransform->AddRelativePosZ(z);
}

void CSceneComponent::SetWorldMatrix()
{
	m_pTransform->SetWorldMatrix();
}

void CSceneComponent::SetGravity(bool bGravity)
{
	m_pTransform->SetGravity(bGravity);
}

void CSceneComponent::AddSpeed(const Vector3& vSpeed)
{
	m_pTransform->AddSpeed(vSpeed);
}

void CSceneComponent::SetSpeed(const Vector3& vSpeed)
{
	m_pTransform->SetSpeed(vSpeed);
}

const Vector3& CSceneComponent::GetSpeed() const
{
	return m_pTransform->GetSpeed();
}

void CSceneComponent::SetSpeedY(float fSpeedY)
{
	m_pTransform->SetSpeedY(fSpeedY);
}

void CSceneComponent::AddSpeedY(float fSpeedY)
{
	m_pTransform->AddSpeedY(fSpeedY);
}

const Vector3& CSceneComponent::GetVelocityScale() const
{
	return m_pTransform->GetVelocityScale();
}

const Vector3& CSceneComponent::GetVelocityRot() const
{
	return m_pTransform->GetVelocityRot();
}

const Vector3& CSceneComponent::GetVelocity() const
{
	return m_pTransform->GetVelocity();
}

float CSceneComponent::GetVelocityAmount() const
{
	return m_pTransform->GetVelocityAmount();
}

const Vector3& CSceneComponent::GetRelativeScale() const
{
	return m_pTransform->GetRelativeScale();
}

const Vector3& CSceneComponent::GetRelativeRot() const
{
	return m_pTransform->GetRelativeRot();
}

const Vector3& CSceneComponent::GetRelativePos() const
{
	return m_pTransform->GetRelativePos();
}

const Vector3& CSceneComponent::GetRelativeAxis(AXIS axis) const
{
	return m_pTransform->GetRelativeAxis(axis);
}

const Vector3& CSceneComponent::GetWorldScale() const
{
	return m_pTransform->GetWorldScale();
}

const Vector3& CSceneComponent::GetWorldRot() const
{
	return m_pTransform->GetWorldRot();
}

const Vector3& CSceneComponent::GetWorldPos() const
{
	return m_pTransform->GetWorldPos();
}

const Vector3& CSceneComponent::GetPivot() const
{
	return m_pTransform->GetPivot();
}

const Vector3& CSceneComponent::GetWorldAxis(AXIS axis) const
{
	return m_pTransform->GetWorldAxis(axis);
}

const Vector3& CSceneComponent::GetMeshSize() const
{
	return m_pTransform->GetMeshSize();
}

const Vector3 CSceneComponent::GetBoneWorldPos() const
{
	return m_pTransform->GetBoneWorldPos();
}

void CSceneComponent::SetWorldScale(const Vector3 & v)
{
	m_pTransform->SetWorldScale(v);
}

void CSceneComponent::SetWorldScale(float x, float y, float z)
{
	m_pTransform->SetWorldScale(x, y, z);
}

void CSceneComponent::SetWorldRot(const Vector3 & v)
{
	m_pTransform->SetWorldRot(v);
}

void CSceneComponent::SetWorldRot(float x, float y, float z)
{
	m_pTransform->SetWorldRot(x, y, z);
}

void CSceneComponent::SetWorldRotX(float x)
{
	m_pTransform->SetWorldRotX(x);
}

void CSceneComponent::SetWorldRotY(float y)
{
	m_pTransform->SetWorldRotY(y);
}

void CSceneComponent::SetWorldRotZ(float z)
{
	m_pTransform->SetWorldRotZ(z);
}

void CSceneComponent::SetWorldPos(const Vector3 & v)
{
	m_pTransform->SetWorldPos(v);
}

void CSceneComponent::SetWorldPos(float x, float y, float z)
{
	m_pTransform->SetWorldPos(x, y, z);
}

void CSceneComponent::SetWorldPosX(float x)
{
	m_pTransform->SetWorldPosX(x);
}

void CSceneComponent::SetWorldPosY(float y)
{
	m_pTransform->SetWorldPosY(y);
}

void CSceneComponent::SetWorldPosZ(float z)
{
	m_pTransform->SetWorldPosZ(z);
}

void CSceneComponent::AddWorldScale(const Vector3 & v)
{
	m_pTransform->AddWorldScale(v);
}

void CSceneComponent::AddWorldScale(float x, float y, float z)
{
	m_pTransform->AddWorldScale(x, y, z);
}

void CSceneComponent::AddWorldRot(const Vector3 & v)
{
	m_pTransform->AddWorldRot(v);
}

void CSceneComponent::AddWorldRot(float x, float y, float z)
{
	m_pTransform->AddWorldRot(x, y, z);
}

void CSceneComponent::AddWorldRotX(float x)
{
	m_pTransform->AddWorldRotX(x);
}

void CSceneComponent::AddWorldRotY(float y)
{
	m_pTransform->AddWorldRotY(y);
}

void CSceneComponent::AddWorldRotZ(float z)
{
	m_pTransform->AddWorldRotZ(z);
}

void CSceneComponent::AddWorldPos(const Vector3 & v)
{
	m_pTransform->AddWorldPos(v);
}

void CSceneComponent::AddWorldPos(float x, float y, float z)
{
	m_pTransform->AddWorldPos(x, y, z);
}

void CSceneComponent::AddWorldPosX(float x)
{
	m_pTransform->AddWorldPosX(x);
}

void CSceneComponent::AddWorldPosY(float y)
{
	m_pTransform->AddWorldPosY(y);
}

void CSceneComponent::AddWorldPosZ(float z)
{
	m_pTransform->AddWorldPosZ(z);
}

void CSceneComponent::SetPivot(const Vector3& v)
{
	m_pTransform->SetPivot(v);
}

void CSceneComponent::SetPivot(float x, float y, float z)
{
	m_pTransform->SetPivot(x,y,z);
}

void CSceneComponent::SetMeshSize(const Vector3& v)
{
	m_pTransform->SetMeshSize(v);
}

void CSceneComponent::SetMeshPos(const Vector3& vPos)
{
	m_pTransform->SetMeshPos(vPos);
}

const Matrix& CSceneComponent::GetMatScale() const
{
	return m_pTransform->GetMatScale();
}

const Matrix& CSceneComponent::GetMatRot() const
{
	return m_pTransform->GetMatRot();
}

const Matrix& CSceneComponent::GetMatPos() const
{
	return m_pTransform->GetMatPos();
}

const Matrix& CSceneComponent::GetMatWorld() const
{
	return m_pTransform->GetMatWorld();
}

void CSceneComponent::Save(FILE* pFile)
{
	CComponent::Save(pFile);

	m_pTransform->Save(pFile);

	fwrite(&m_eSceneComponentType, sizeof(SCENE_COMPONENT_TYPE), 1, pFile);
	fwrite(&m_iSceneComponentClassType, sizeof(int), 1, pFile);

	/*bool bMaterial = false;

	if (m_pMaterial)
		bMaterial = true;

	fwrite(&bMaterial, sizeof(bool), 1, pFile);

	if (m_pMaterial)
	{
		std::string strTag = m_pMaterial->GetName();

		int iLength = (int)strTag.length();

		fwrite(&iLength, 4, 1, pFile);

		fwrite(strTag.c_str(), 1, iLength, pFile);

		m_pMaterial->Save(pFile);
	}*/

	fwrite(&m_iZOrder, sizeof(int), 1, pFile);

	/*bool bMesh = false;

	if (m_pMesh)
	{
		bMesh = true;
	}

	fwrite(&bMesh, 1, 1, pFile);
	
	if (m_pMesh)
	{
		std::string strTag = m_pMesh->GetName();

		int iLength = (int)strTag.length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(strTag.c_str(), 1, iLength, pFile);			
	}*/

	bool bRenderer = false;

	if (m_pRenderer)
		bRenderer = true;

	fwrite(&bRenderer, 1, 1, pFile);

	if (m_pRenderer)
		m_pRenderer->Save(pFile);

	fwrite(&m_bInstance, 1, 1, pFile);

	int iSize = static_cast<int>(m_ObjComList.size());

	fwrite(&iSize, 4, 1, pFile);

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ObjComList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ObjComList.end();

	for (; iter != iterEnd; ++iter)
	{
		std::string strName = (*iter)->GetName();

		int iLength = static_cast<int>(strName.length());

		fwrite(&iLength, 4, 1, pFile);

		if (iLength > 0)
		{
			fwrite(strName.c_str(), 1, iLength, pFile);
		}
	}
}

void CSceneComponent::Load(FILE* pFile)
{
	CComponent::Load(pFile);

	m_pTransform->m_pScene = m_pScene;

	m_pTransform->Load(pFile);

	fread(&m_eSceneComponentType, sizeof(SCENE_COMPONENT_TYPE), 1, pFile);
	fread(&m_iSceneComponentClassType, sizeof(int), 1, pFile);

	//class CMesh* pMesh = GET_SINGLE(CResourceManager)->GetDefaultMesh();

	//SetMesh(pMesh);

	//bool bMaterial = false;

	//fread(&bMaterial, sizeof(bool), 1, pFile);

	//if (bMaterial)
	//{
	//	char strTag[MAX_PATH] = {};

	//	int iLength = 0;

	//	fread(&iLength, 4, 1, pFile);

	//	if (iLength > 0)
	//	{
	//		fread(strTag, 1, iLength, pFile);
	//	}

	//	std::shared_ptr<CMaterial> pMtrl = GET_SINGLE(CResourceManager)->FindMaterial(strTag);

	//	if (pMtrl)
	//	{
	//		m_pMaterial = pMtrl->Clone();
	//	}
	//	else
	//	{
	//		m_pMaterial = std::shared_ptr<class CMaterial>(new CMaterial);
	//	}

	//	m_pMaterial->Load(pFile);
	//}

	fread(&m_iZOrder, sizeof(int), 1, pFile);

	bool bRenderer = false;

	fread(&bRenderer, 1, 1, pFile);

	if (bRenderer)
		m_pRenderer->Load(pFile);

	//bool bMesh = false;

	//fread(&bMesh, 1, 1, pFile);

	//if (bMesh)
	//{
	//	char strTag[256] = {};

	//	int iLength = 0;

	//	fread(&iLength, 4, 1, pFile);
	//	fread(strTag, 1, iLength, pFile);

	//	m_pMesh = GET_SINGLE(CResourceManager)->FindMesh(strTag);
	//}

	fread(&m_bInstance, 1, 1, pFile);

	int iSize = 0;

	fread(&iSize, 4, 1, pFile);

	for (int i=0;i<iSize;i++)
	{
		int iLength = 0;

		fread(&iLength, 4, 1, pFile);

		if (iLength > 0)
		{
			char strName[MAX_PATH] = {};
			fread(strName, 1, iLength, pFile);

			std::shared_ptr<CComponent> pCom = m_pObj->FindComponent(strName);

			if (pCom)
			{
				m_ObjComList.push_back(pCom);
			}
		}

	}
}

void CSceneComponent::GetAllComponentName(std::vector<Hierarchy>& vecHierarchy)
{
	Hierarchy tHier;

	if (m_pParent)
		tHier.strParentTag = m_pParent->GetName();

	tHier.strTag = GetName();

	vecHierarchy.push_back(tHier);

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		CSceneComponent* pCom = static_cast<CSceneComponent*>((*iter).get());

		pCom->GetAllComponentName(vecHierarchy);
	}
}

void CSceneComponent::GetAllComponent(std::vector<CSceneComponent*>& vecSceneCom)
{
	vecSceneCom.push_back(this);

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		CSceneComponent* pCom = static_cast<CSceneComponent*>((*iter).get());

		pCom->GetAllComponent(vecSceneCom);
	}
}

bool CSceneComponent::ShowNode()
{
	std::string strNode = GetName() + ".";
	std::string strtest = GetName() + "test";
	/*if (ImGui::TreeNode(strNode.c_str()))
	{
		ShowWindow();

		ImGui::InputInt("Scene Component Type", reinterpret_cast<int*>(&m_eSceneComponentType));

		std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
		std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

		for (; iter != iterEnd; ++iter)
		{
			static_cast<CSceneComponent*>((*iter).get())->ShowNode();
		}
		ImGui::TreePop();

		std::vector<const char*> vecstrObjCom;

		std::list<std::shared_ptr<CComponent>>::iterator iterO = m_ObjComList.begin();
		std::list<std::shared_ptr<CComponent>>::iterator iterOEnd = m_ObjComList.end();

		for (; iterO != iterOEnd; ++iterO)
		{
			vecstrObjCom.push_back((*iterO)->GetName().c_str());
		}
		static int iSelect = -1;

		int iSize = static_cast<int>(m_ObjComList.size());
		if (iSize > 0)
		{
			ImGui::ListBox("Object Component List", &iSelect, &vecstrObjCom[0], iSize);
		}

		if (iSelect != -1 && iSize > iSelect)
		{
			std::shared_ptr<CComponent> pCom = FindObjCom(vecstrObjCom[iSelect]);

			pCom->ShowWindow();
		}
	}*/

	bool bChanged = false;


	static std::string strSrc;
	bool selected = false;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
	bool open = ImGui::TreeNodeEx(strNode.c_str(),
		ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen | (selected ? ImGuiTreeNodeFlags_Selected : 0) | (m_ChildList.empty() ? ImGuiTreeNodeFlags_Leaf : 0),
		"%s", strtest.c_str());

	ImGui::PopStyleVar();
	ImGui::PushID(strNode.c_str());

	if (ImGui::BeginPopupContextItem())
	{
		strSrc = GetName();
		ImGui::EndPopup();
	}
	ImGui::PopID();
	bool bOpen = false;
	if (ImGui::IsItemClicked())
	{
		m_bTreeOpen ^= true;
		if (m_pObj)
		{
			if (strSrc != "")
			{
				std::shared_ptr<CComponent> pSrc = m_pObj->FindComponent(strSrc);
				if (pSrc)
				{
					CSceneComponent* pParent = static_cast<CSceneComponent*>(pSrc.get())->GetParent();
					if (pParent)
					{
						CSceneComponent* pDestParent = GetParent();
						if (pSrc.get() == pDestParent)
						{
							std::shared_ptr<CComponent> pDest = static_cast<CSceneComponent*>(pSrc.get())->DeleteChild(this);

							pParent->AddChild(pDest);
						}

						pParent->DeleteChild(pSrc);

						AddChild(pSrc);
					}
					else
					{
						pParent = GetParent();

						if (pParent)
						{
							std::shared_ptr<CComponent> pDest = pParent->DeleteChild(this);

							m_pObj->SetRootComponent(pDest);

							static_cast<CSceneComponent*>(pDest.get())->AddChild(pSrc);
						}
					}

					strSrc = "";

					bChanged = true;
				}
			}
		}
	}
	if (ImGui::BeginDragDropTarget())
	{
		ImGui::EndDragDropTarget();
	}
	
	if (ImGui::BeginDragDropSource())
	{
		ImGui::EndDragDropSource();
	}
	if (open)
	{
		std::list<std::shared_ptr<CComponent>>::iterator iter = m_ChildList.begin();
		std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ChildList.end();

		for (; iter != iterEnd; ++iter)
		{
			if (static_cast<CSceneComponent*>((*iter).get())->ShowNode())
			{
				bChanged = true;
				break;
			}
		}
		ImGui::TreePop();
	}

	if (m_bTreeOpen)
	{
		ShowWindow();
	}

	return bChanged;
}

void CSceneComponent::ShowWindow()
{
	CComponent::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if(ImGui::Begin(strName.c_str()))
	{
		ImGui::Text("Scene Component");

		ImGui::Checkbox("Frustum Culling", &m_bFrustumCulling);
		ImGui::Checkbox("In Frustum", &m_bInFrustum);

		if (ImGui::InputInt("Z Order", &m_iZOrder))
		{
			SetZOrder(m_iZOrder);
		}

		if (m_pTransform)
		{
			m_pTransform->ShowWindow();
		}

		if (m_pRenderer)
		{
			m_pRenderer->ShowWindow();
		}
	}
	ImGui::End();
}
