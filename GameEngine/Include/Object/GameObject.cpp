#include "GameObject.h"
#include "../Component/SceneComponent.h"
#include "../Component/ObjectComponent.h"
#include "../Resource/Material.h"
#include "../Scene/Scene.h"
#include "../Component/Mesh2DComponent.h"
#include "../Component/StaticMeshComponent.h"
#include "../InputObj.h"
#include "../Component/Camera.h"
#include "../Component/SpriteComponent.h"
#include "../Component/ColliderRect.h"
#include "../Component/ColliderLine.h"
#include "../Component/ColliderCircle.h"
#include "../Component/ColliderOBB2D.h"
#include "../Component/ColliderPixel.h"
#include "../Component/ColliderPoint.h"
#include "../UI/Bar.h"
#include "../UI/ChkBx.h"
#include "../UI/UIButton.h"
#include "../UI/Panel.h"
#include "../UI/RadioButton.h"
#include "../UI/Slider.h"
#include "../UI/UFont.h"
#include "../UI/UIImage.h"
#include "../UI/UISprite.h"
#include "../UI/Icon.h"
#include "../UI/Tab.h"
#include "../UI/TitleBar.h"
#include "../UI/UIList.h"
#include "../Component/Sound.h"
#include "../Component/TileMap.h"
#include "../Tile.h"
#include "../UI/imgui/imgui.h"
#include "../Component/Light.h"
#include "../Component/Particle.h"
#include "../Component/Renderer.h"
#include "../Component/ColliderSphere.h"
#include "../Component/ColliderRay.h"
#include "../Component/ColliderTerrain.h"
#include "../Component/ColliderOBB.h"
#include "../Component/Terrain.h"
#include "../Component/Arm.h"
#include "../Component/Decal.h"
#include "../Component/Navigation.h"
#include "../Component/Navigation2D.h"
#include "../Component/PaperBurn.h"

CGameObject::CGameObject() :
	m_pScene(nullptr),
	m_pLayer(nullptr),
	m_bStart(false),
	m_iObjType(0)
{
	m_pInput = new CInputObj;
	m_pRootComponent = std::shared_ptr<CComponent>(new CMesh2DComponent);
	m_pRootComponent->SetName("Default Component");
}

CGameObject::CGameObject(const CGameObject& obj)	:
	CRef(obj)
	,m_pScene(nullptr)
	,m_pLayer(nullptr)
{
	m_bStart = false;

	m_iObjType = obj.m_iObjType;

	m_vecObjectComponent.clear();

	size_t iSize = obj.m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		std::shared_ptr<CComponent> pCom = obj.m_vecObjectComponent[i]->Clone();

		pCom->m_pObj = this;
		pCom->m_pScene = m_pScene;

		m_vecObjectComponent.push_back(pCom);
	}

	if (obj.m_pRootComponent)
	{
		obj.m_pRootComponent->m_pLayer = obj.m_pLayer;

		m_pRootComponent = obj.m_pRootComponent->Clone();

		CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

		pCom->SetObj(this);
		m_pRootComponent->m_pScene = m_pScene;

		std::list<std::shared_ptr<CComponent>>::iterator iterO = 
			static_cast<CSceneComponent*>(obj.m_pRootComponent.get())->m_ObjComList.begin();
		std::list<std::shared_ptr<CComponent>>::iterator iterOEnd = 
			static_cast<CSceneComponent*>(obj.m_pRootComponent.get())->m_ObjComList.end();

		for (; iterO != iterOEnd;++iterO)
		{
			static_cast<CSceneComponent*>(m_pRootComponent.get())->
				m_ObjComList.push_back(FindComponent((*iterO)->GetName()));
		}
	}


	m_pInput = new CInputObj;

	m_EditorDelete = obj.m_EditorDelete;
}

CGameObject::~CGameObject()
{
	SAFE_DELETE(m_pInput);

	if (m_EditorDelete)
		m_EditorDelete(GetName());
}

std::shared_ptr<CComponent> CGameObject::FindComponent(const std::string& strTag)
{
	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; i++)
	{
		if (m_vecObjectComponent[i]->GetName() == strTag)
		{
			return m_vecObjectComponent[i];
		}
	}

	if (m_pRootComponent.get()->GetName() == strTag)
		return m_pRootComponent;

	CSceneComponent* pCom =(CSceneComponent*)m_pRootComponent.get();

	return pCom->FindComponent(strTag);
}

void CGameObject::SetRootComponent(std::shared_ptr<CComponent> pComponent)
{
	m_pRootComponent = pComponent;
}

const std::shared_ptr<class CComponent>& CGameObject::GetRootComponent() const
{
	return m_pRootComponent;
}

void CGameObject::SetScene(CScene* pScene)
{
	m_pScene = pScene;

	if (m_pRootComponent)
		static_cast<CSceneComponent*>(m_pRootComponent.get())->CSceneComponent::SetScene(pScene);

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->SetScene(pScene);
}

void CGameObject::SetLayer(CLayer* pLayer)
{
	if (m_pLayer)
	{
		m_pLayer = pLayer->GetScene()->FindLayer(m_pLayer->GetZOrder());
	}
	else
	{
		m_pLayer = pLayer;
	}

	((CSceneComponent*)m_pRootComponent.get())->SetLayer(pLayer);

	size_t iSz = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		((CObjectComponent*)m_vecObjectComponent[i].get())->SetLayer(pLayer);
	}
}

CLayer* CGameObject::GetLayer() const
{
	return m_pLayer;
}

const TransformCBuffer& CGameObject::GetCBuffer() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetCBuffer();
}

const std::vector<std::shared_ptr<CComponent>>* CGameObject::GetObjComponent() const
{
	return &m_vecObjectComponent;
}

void CGameObject::SetShadowAll(bool bShadow)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->SetShadowAll(bShadow);
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Start()
{
	m_bStart = true;

	m_pRootComponent->Start();

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->Start();
}

void CGameObject::Input(float fTime)
{
}

void CGameObject::Update(float fTime)
{
	if (m_pRootComponent->IsEnable())
	{
		m_pRootComponent->Update(fTime);
	}

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->Update(fTime);
}

void CGameObject::PostUpdate(float fTime)
{
	if (m_pRootComponent->IsEnable())
	{
		m_pRootComponent->PostUpdate(fTime);
	}

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->PostUpdate(fTime);
}

void CGameObject::Collision(float fTime)
{
	if (m_pRootComponent->IsEnable())
	{
		m_pRootComponent->Collision(fTime);
	}

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->Collision(fTime);
}

void CGameObject::CheckFrustum()
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->CheckFrustum();
}

void CGameObject::PreRender(float fTime)
{
	if (m_pRootComponent->IsEnable())
	{
		m_pRootComponent->PreRender(fTime);
	}

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->PreRender(fTime);
}

void CGameObject::Render(float fTime)
{
	//m_pRootComponent->Render(fTime);

	std::vector<CSceneComponent*> vecCom;
	static_cast<CSceneComponent*>(m_pRootComponent.get())->GetAllComponent(vecCom);

	size_t iComSize = vecCom.size();
	for (size_t i = 0; i < iComSize; ++i)
	{
		vecCom[i]->Render(fTime);
	}

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->Render(fTime);
}

void CGameObject::PostRender(float fTime)
{
	if (m_pRootComponent->IsEnable())
	{
		m_pRootComponent->PostRender(fTime);
	}

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->PostRender(fTime);
}

std::shared_ptr<CGameObject> CGameObject::Clone()
{
	return std::shared_ptr<CGameObject>(new CGameObject(*this));
}

void CGameObject::SetInheritScale(bool bInherit)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetInheritScale(bInherit);

}

void CGameObject::SetInheritRotX(bool bInherit)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetInheritRotX(bInherit);

}

void CGameObject::SetInheritRotY(bool bInherit)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetInheritRotY(bInherit);

}

void CGameObject::SetInheritRotZ(bool bInherit)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetInheritRotZ(bInherit);

}

void CGameObject::SetInheritPos(bool bInherit)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->SetInheritPos(bInherit);
}

void CGameObject::InheritScale()
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->InheritScale();

}

void CGameObject::InheritRot()
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->InheritRot();

}

void CGameObject::InheritPos()
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->InheritPos();

}

void CGameObject::SetRelativeScale(const Vector3& v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetRelativeScale(v);

}

void CGameObject::SetRelativeScale(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetRelativeScale(x, y, z);

}

void CGameObject::SetRelativeRot(const Vector3& v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetRelativeRot(v);

}

void CGameObject::SetRelativeRot(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetRelativeRot(x, y, z);

}

void CGameObject::SetRelativeRotX(float x)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetRelativeRotX(x);

}

void CGameObject::SetRelativeRotY(float y)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetRelativeRotY(y);

}

void CGameObject::SetRelativeRotZ(float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetRelativeRotZ(z);

}

void CGameObject::SetRelativePos(const Vector3& v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetRelativePos(v);

}

void CGameObject::SetRelativePos(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetRelativePos(x, y, z);

}

void CGameObject::SetRelativePosX(float d)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->SetRelativePosX(d);
}

void CGameObject::SetRelativePosY(float d)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->SetRelativePosY(d);
}

void CGameObject::SetRelativePosZ(float d)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->SetRelativePosZ(d);
}

void CGameObject::AddRelativeScale(const Vector3& v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddRelativeScale(v);

}

void CGameObject::AddRelativeScale(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddRelativeScale(x, y, z);

}

void CGameObject::AddRelativeRot(const Vector3& v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddRelativeRot(v);

}

void CGameObject::AddRelativeRot(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddRelativeRot(x, y, z);

}

void CGameObject::AddRelativeRotX(float x)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddRelativeRotX(x);

}

void CGameObject::AddRelativeRotY(float y)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddRelativeRotY(y);

}

void CGameObject::AddRelativeRotZ(float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddRelativeRotZ(z);

}

void CGameObject::AddRelativePos(const Vector3& v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddRelativePos(v);

}

void CGameObject::AddRelativePos(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddRelativePos(x, y, z);

}

void CGameObject::AddRelativePosX(float x)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->AddRelativePosX(x);
}

void CGameObject::AddRelativePosY(float y)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->AddRelativePosY(y);
}

void CGameObject::AddRelativePosZ(float z)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->AddRelativePosZ(z);
}

void CGameObject::SetGravity(bool bGravity)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->SetGravity(bGravity);
}

void CGameObject::AddSpeed(const Vector3& vSpeed)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->AddSpeed(vSpeed);
}

void CGameObject::SetSpeed(const Vector3& vSpeed)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->SetSpeed(vSpeed);
}

const Vector3& CGameObject::GetSpeed() const
{
	return static_cast<CSceneComponent*>(m_pRootComponent.get())->GetSpeed();
}

void CGameObject::SetSpeedY(float fSpeedY)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->SetSpeedY(fSpeedY);
}

void CGameObject::AddSpeedY(float fSpeedY)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->AddSpeedY(fSpeedY);
}

const Vector3& CGameObject::GetVelocityScale() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetVelocityScale();
}

const Vector3& CGameObject::GetVelocityRot() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetVelocityRot();
}

const Vector3& CGameObject::GetVelocity() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetVelocity();
}

float CGameObject::GetVelocityAmount() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	float fAmt = pCom->GetVelocityAmount();


	return fAmt;
}

const Vector3& CGameObject::GetRelativeScale() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetRelativeScale();
}

const Vector3& CGameObject::GetRelativeRot() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetRelativeRot();
}

const Vector3& CGameObject::GetRelativePos() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetRelativePos();
}

const Vector3& CGameObject::GetRelativeAxis(AXIS axis) const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetRelativeAxis(axis);
}

const Vector3& CGameObject::GetWorldScale() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetWorldScale();
}

const Vector3& CGameObject::GetWorldRot() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetWorldRot();
}

const Vector3& CGameObject::GetWorldPos() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetWorldPos();
}

const Vector3& CGameObject::GetPivot() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetPivot();
}

const Vector3& CGameObject::GetWorldAxis(AXIS axis) const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetWorldAxis(axis);
}

const Vector3& CGameObject::GetMeshSize() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetMeshSize();
}

const Vector3 CGameObject::GetBoneWorldPos() const
{
	return static_cast<CSceneComponent*>(m_pRootComponent.get())->GetBoneWorldPos();
}

void CGameObject::SetWorldScale(const Vector3 & v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetWorldScale(v);

}

void CGameObject::SetWorldScale(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetWorldScale(x, y, z);

}

void CGameObject::SetWorldRot(const Vector3 & v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetWorldRot(v);
}

void CGameObject::SetWorldRot(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetWorldRot(x, y, z);
}

void CGameObject::SetWorldRotX(float x)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetWorldRotX(x);

}

void CGameObject::SetWorldRotY(float y)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetWorldRotY(y);

}

void CGameObject::SetWorldRotZ(float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetWorldRotZ(z);

}

void CGameObject::SetWorldPos(const Vector3 & v)
{
	((CSceneComponent*)m_pRootComponent.get())->SetWorldPos(v);
}

void CGameObject::SetWorldPos(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetWorldPos(x, y, z);

}

void CGameObject::SetWorldPosX(float x)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->SetWorldPosX(x);
}

void CGameObject::SetWorldPosY(float y)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->SetWorldPosY(y);
}

void CGameObject::SetWorldPosZ(float z)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->SetWorldPosZ(z);
}

void CGameObject::AddWorldScale(const Vector3 & v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddWorldScale(v);

}

void CGameObject::AddWorldScale(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddWorldScale(x, y, z);

}

void CGameObject::AddWorldRot(const Vector3 & v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddWorldRot(v);

}

void CGameObject::AddWorldRot(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddWorldRot(x, y, z);

}

void CGameObject::AddWorldRotX(float x)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddWorldRotX(x);

}

void CGameObject::AddWorldRotY(float y)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddWorldRotY(y);

}

void CGameObject::AddWorldRotZ(float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddWorldRotZ(z);

}

void CGameObject::AddWorldPos(const Vector3 & v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddWorldPos(v);

}

void CGameObject::AddWorldPos(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->AddWorldPos(x, y, z);
}

void CGameObject::AddWorldPosX(float x)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->AddWorldPosX(x);
}

void CGameObject::AddWorldPosY(float y)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->AddWorldPosY(y);
}

void CGameObject::AddWorldPosZ(float z)
{
	static_cast<CSceneComponent*>(m_pRootComponent.get())->AddWorldPosZ(z);
}

void CGameObject::SetPivot(const Vector3 & v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetPivot(v);
}

void CGameObject::SetPivot(float x, float y, float z)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetPivot(x, y, z);

}

void CGameObject::SetMeshSize(const Vector3 & v)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->SetMeshSize(v);

}

const Matrix& CGameObject::GetMatScale() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetMatScale();
}

const Matrix& CGameObject::GetMatRot() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();


	return pCom->GetMatRot();
}

const Matrix& CGameObject::GetMatPos() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetMatPos();
}

const Matrix& CGameObject::GetMatWorld() const
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	return pCom->GetMatWorld();
}

void CGameObject::GetAllComponentName(std::vector<Hierarchy> & vecstrCom)
{
	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->GetAllComponentName(vecstrCom);

}

void CGameObject::AddChild(std::shared_ptr<CComponent> pCom)
{
	CSceneComponent* _pCom = (CSceneComponent*)m_pRootComponent.get();

	_pCom->AddChild(pCom);
	pCom->SetScene(m_pScene);
	pCom->m_pLayer = m_pLayer;
	pCom->m_pObj = this;

}

void CGameObject::SetUIEnableAll(bool bEnable)
{
	if (m_pRootComponent)
	{
		static_cast<CSceneComponent*>(m_pRootComponent.get())->UIEnableAll(bEnable);
	}
}

void CGameObject::AddAxisBind(const std::string& strTag, void(*pFunc)(float, float))
{
}

void CGameObject::AddActionBind(const std::string& strTag, void(*pFunc)(float))
{
}

void CGameObject::CallNotify(const std::string& strTag)
{
}

void CGameObject::ShowWindow()
{
	std::vector<Hierarchy> vecCom;

	GetAllComponentName(vecCom);

	int iSize = static_cast<int>(m_vecObjectComponent.size());

	const char** pText = new const char* [iSize];

	const char* pName = GetName().c_str();

	if (!strcmp(pName, ""))
	{
		pName = "NULL";
	}

	if (ImGui::Begin("Object"))
	{
		CRef::ShowWindow();

		if (m_pRootComponent)
			static_cast<CSceneComponent*>(m_pRootComponent.get())->ShowNode();

		std::vector<std::shared_ptr<CComponent>>::iterator iter = m_vecObjectComponent.begin();
		std::vector<std::shared_ptr<CComponent>>::iterator iterEnd = m_vecObjectComponent.end();

		for (int i = 0; iter != iterEnd; ++iter, ++i)
		{
			pText[i] = (*iter)->GetName().c_str();
		}

		static int iSel = -1;

		ImGui::ListBox("ObjComponents", &iSel, pText, iSize);

		if (iSel != -1 && iSel < iSize)
		{
			std::vector<std::shared_ptr<CComponent>>::iterator iter = m_vecObjectComponent.begin() + iSel;

			if (*iter)
			{
				(*iter)->ShowWindow();
			}

		}
		static bool bCreate = false;
		if (ImGui::Button("Create Component"))
		{
			bCreate = true;
		}

		if (bCreate)
		{
			if (ImGui::Begin("Component"))
			{
				std::vector<const char*> vecCom(static_cast<int>(SCENECOMPONENT_CLASS_TYPE::END));

				for (int i = 0; i < static_cast<int>(SCENECOMPONENT_CLASS_TYPE::END); ++i)
				{
					switch (static_cast<SCENECOMPONENT_CLASS_TYPE>(i))
					{
					case SCENECOMPONENT_CLASS_TYPE::SCT_MESH2D:
						vecCom[i] = "MESH2D";
						break;
					case SCENECOMPONENT_CLASS_TYPE::SCT_STATICMESH:
						vecCom[i] = "SCT_STATICMESH";
						break;
					case SCENECOMPONENT_CLASS_TYPE::SCT_CAMERA:
						vecCom[i] = "SCT_CAMERA";
						break;
					case SCENECOMPONENT_CLASS_TYPE::SCT_SPRITE:
						vecCom[i] = "SCT_SPRITE";
						break;
					case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_RECT:
						vecCom[i] = "SCT_COLLIDER_RECT";
						break;
					case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_LINE:
						vecCom[i] = "SCT_COLLIDER_LINE";
						break;
					case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_CIRCLE:
						vecCom[i] = "SCT_COLLIDER_CIRCLE";
						break;
					case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_OBB2D:
						vecCom[i] = "SCT_COLLIDER_OBB2D";
						break;
					case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_PIXEL:
						vecCom[i] = "SCT_COLLIDER_PIXEL";
						break;
					case SCENECOMPONENT_CLASS_TYPE::COLLIDER_POINT:
						vecCom[i] = "COLLIDER_POINT";
						break;
					case SCENECOMPONENT_CLASS_TYPE::COLLIDER_RAY:
						vecCom[i] = "COLLIDER_RAY";
						break;
					case SCENECOMPONENT_CLASS_TYPE::COLLIDER_SPHERE:
						vecCom[i] = "COLLIDER_SPHERE";
						break;
					case SCENECOMPONENT_CLASS_TYPE::COLLIDER_TERRAIN:
						vecCom[i] = "COLLIDER_TERRAIN";
						break;
					case SCENECOMPONENT_CLASS_TYPE::COLLIDER_OBB:
						vecCom[i] = "COLLIDER_OBB";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_BAR:
						vecCom[i] = "UI_BAR";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_CHECK:
						vecCom[i] = "UI_CHECK";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_BUTTON:
						vecCom[i] = "UI_BUTTON";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_RADIO:
						vecCom[i] = "UI_RADIO";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_PANEL:
						vecCom[i] = "UI_PANEL";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_SLIDER:
						vecCom[i] = "UI_SLIDER";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_FONT:
						vecCom[i] = "UI_FONT";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_IMAGE:
						vecCom[i] = "UI_IMAGE";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_SPRITE:
						vecCom[i] = "UI_SPRITE";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_TITLE:
						vecCom[i] = "UI_TITLE";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_ICON:
						vecCom[i] = "UI_ICON";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_TAB:
						vecCom[i] = "UI_TAB";
						break;
					case SCENECOMPONENT_CLASS_TYPE::UI_LIST:
						vecCom[i] = "UI_LIST";
						break;
					case SCENECOMPONENT_CLASS_TYPE::SOUND:
						vecCom[i] = "SOUND";
						break;
					case SCENECOMPONENT_CLASS_TYPE::TILEMAP:
						vecCom[i] = "TILEMAP";
						break;
					case SCENECOMPONENT_CLASS_TYPE::TILE:
						vecCom[i] = "TILE";
						break;
					case SCENECOMPONENT_CLASS_TYPE::RENDERER:
						vecCom[i] = "RENDERER";
						break;
					case SCENECOMPONENT_CLASS_TYPE::PARTICLE:
						vecCom[i] = "PARTICLE";
						break;
					case SCENECOMPONENT_CLASS_TYPE::LIGHT:
						vecCom[i] = "LIGHT";
						break;
					case SCENECOMPONENT_CLASS_TYPE::TERRAIN:
						vecCom[i] = "TERRAIN";
						break;
					case SCENECOMPONENT_CLASS_TYPE::ARM:
						vecCom[i] = "ARM";
						break;
					case SCENECOMPONENT_CLASS_TYPE::DECAL:
						vecCom[i] = "DECAL";
						break;
					default:
						vecCom[i] = "NONE";
						break;
					}
				}
				static int iSel = -1;

				ImGui::ListBox("Components", &iSel, &vecCom[0], static_cast<int>(SCENECOMPONENT_CLASS_TYPE::END));
				static char strTag[MAX_PATH] = {};
				ImGui::InputText("Name",strTag, MAX_PATH);

				if (iSel != -1)
				{
					if (ImGui::Button("Create"))
					{
						std::shared_ptr<CComponent> pCom;
						switch (static_cast<SCENECOMPONENT_CLASS_TYPE>(iSel))
						{
						case SCENECOMPONENT_CLASS_TYPE::SCT_MESH2D:
						{
							pCom = CreateComponent<CMesh2DComponent>(strTag);
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::SCT_STATICMESH:
						{
							pCom = CreateComponent<CStaticMeshComponent>(strTag);
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::SCT_CAMERA:
						{
							pCom = CreateComponent<CCamera>(strTag);
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::SCT_SPRITE:
						{
							pCom = CreateComponent<CSpriteComponent>(strTag); 
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_RECT:
							{
							pCom = CreateComponent<CColliderRect>(strTag);
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_LINE:
							{
							pCom = CreateComponent<CColliderLine>(strTag);
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_CIRCLE:
						{
							pCom = CreateComponent<CColliderCircle>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_OBB2D:
						{
							pCom = CreateComponent<CColliderOBB2D>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_PIXEL:
						{
							pCom = CreateComponent<CColliderPixel>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::COLLIDER_RAY:
						{
							pCom = CreateComponent<CColliderRay>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::COLLIDER_SPHERE:
						{
							pCom = CreateComponent<CColliderSphere>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::COLLIDER_TERRAIN:
						{
							pCom = CreateComponent<CColliderTerrain>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::COLLIDER_POINT:
						{
							pCom = CreateComponent<CColliderPoint>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::COLLIDER_OBB:
						{
							pCom = CreateComponent<CColliderOBB>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::UI_BAR:
						{
							pCom = CreateComponent<CBar>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::UI_CHECK:
						{
							pCom = CreateComponent<CChkBx>(strTag);
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::UI_BUTTON:
							{
							pCom = CreateComponent<CUIButton>(strTag);
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::UI_RADIO:
						{
							pCom = CreateComponent<CRadioButton>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::UI_PANEL:
						{
							pCom = CreateComponent<CPanel>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::UI_SLIDER:
						{
							pCom = CreateComponent<CSlider>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::UI_FONT:
						{
							pCom = CreateComponent<CUFont>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::UI_IMAGE:
						{
							pCom = CreateComponent<CUIImage>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::UI_SPRITE:
						{
							pCom = CreateComponent<CUISprite>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::UI_TITLE:
						{
							pCom = CreateComponent<CTitleBar>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::UI_ICON:
						{
							pCom = CreateComponent<CIcon>(strTag);
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::UI_TAB:
							{
							pCom = CreateComponent<CTab>(strTag);
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::UI_LIST:
						{
							pCom = CreateComponent<CUIList>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::SOUND:
						{
							pCom = CreateComponent<CSound>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::TILEMAP:
						{
							pCom = CreateComponent<CTileMap>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::TILE:
						{
							pCom = CreateComponent<CTile>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::RENDERER:
						{
							//pCom = CreateComponent<CRenderer>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::PARTICLE:
						{
							pCom = CreateComponent<CParticle>(strTag);
						}
						break;
						case SCENECOMPONENT_CLASS_TYPE::LIGHT:
						{
							pCom = CreateComponent<CLight>(strTag);
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::TERRAIN:
						{
							pCom = CreateComponent<CTerrain>(strTag);
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::ARM:
						{
							pCom = CreateComponent<CArm>(strTag);
						}
							break;
						case SCENECOMPONENT_CLASS_TYPE::DECAL:
						{
							pCom = CreateComponent<CDecal>(strTag);
						}
							break;
						}


						if (m_pRootComponent)
						{
							static_cast<CSceneComponent*>(m_pRootComponent.get())->AddChild(pCom);
						}

						else
						{
							SetRootComponent(pCom);
						}
						bCreate = false;
					}
				}
				if (ImGui::Button("Cancel"))
				{
					bCreate = false;
				}
			}
			ImGui::End();
		}
	}
	ImGui::End();
	delete[] pText;
}

void CGameObject::Save(FILE* pFile)
{
	CRef::Save(pFile);

	fwrite(&m_bStart, sizeof(bool), 1, pFile);

	int iSize = (int)m_vecObjectComponent.size();

	fwrite(&m_iObjType, sizeof(int), 1, pFile);

	fwrite(&iSize, sizeof(int), 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		OBJ_COMPONENT_CLASS_TYPE eType = static_cast<CObjectComponent*>(m_vecObjectComponent[i].get())->GetClassType();
		fwrite(&eType, 4, 1, pFile);

		m_vecObjectComponent[i]->Save(pFile);
	}

	std::vector<CSceneComponent*> vecSceneCom;

	CSceneComponent* pCom = (CSceneComponent*)m_pRootComponent.get();

	pCom->GetAllComponent(vecSceneCom);

	std::vector<Hierarchy> vecHierarchy;

	pCom->GetAllComponentName(vecHierarchy);


	iSize = (int)vecSceneCom.size();

	fwrite(&iSize, sizeof(int), 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		int iType = vecSceneCom[i]->GetSceneComponentClassType();

		fwrite(&iType, sizeof(int), 1, pFile);

		vecSceneCom[i]->Save(pFile);

		int iLength = (int)vecHierarchy[i].strTag.length();

		fwrite(&iLength, sizeof(int), 1, pFile);
		if (iLength > 0)
		{
			fwrite(vecHierarchy[i].strTag.c_str(), sizeof(char), iLength, pFile);
		}

		iLength = (int)vecHierarchy[i].strParentTag.length();

		fwrite(&iLength, sizeof(int), 1, pFile);
		if (iLength > 0)
		{
			fwrite(vecHierarchy[i].strParentTag.c_str(), sizeof(char), iLength, pFile);
		}
	}
}

void CGameObject::Load(FILE* pFile)
{
	CRef::Load(pFile);

	fread(&m_bStart, sizeof(bool), 1, pFile);

	fread(&m_iObjType, sizeof(int), 1, pFile);

	int iSize = 0;

	fread(&iSize, sizeof(int), 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		OBJ_COMPONENT_CLASS_TYPE eType = OBJ_COMPONENT_CLASS_TYPE::NAVIGATION;
		fread(&eType, 4, 1, pFile);

		std::shared_ptr<CComponent> pCom = nullptr;
		switch (eType)
		{
		case OBJ_COMPONENT_CLASS_TYPE::NAVIGATION:
		{
			//pCom = std::shared_ptr<CComponent>(new CNavigation);
		}
			break;
		case OBJ_COMPONENT_CLASS_TYPE::NAVIGATION2D:
		{
			pCom = std::shared_ptr<CComponent>(new CNavigation2D);
		}
			break;
		case OBJ_COMPONENT_CLASS_TYPE::PAPERBURN:
		{
			pCom = std::shared_ptr<CComponent>(new CPaperBurn);
		}
			break;
		}

		pCom->m_pObj = this;
		pCom->m_pScene = m_pScene;

		pCom->Load(pFile);

		m_vecObjectComponent.push_back(pCom);
	}

	iSize = 0;

	fread(&iSize, sizeof(int), 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		int iType = 0;

		fread(&iType, sizeof(int), 1, pFile);

		std::shared_ptr<CComponent> pCom = nullptr;

		switch ((SCENECOMPONENT_CLASS_TYPE)iType)
		{
		case SCENECOMPONENT_CLASS_TYPE::SCT_MESH2D:
			pCom = std::shared_ptr<CComponent>(new CMesh2DComponent);
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_STATICMESH:
			pCom = std::shared_ptr<CComponent>(new CStaticMeshComponent);
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_CAMERA:
			pCom = std::shared_ptr<CComponent>(new CCamera);
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_SPRITE:
			pCom = std::shared_ptr<CComponent>(new CSpriteComponent);
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_RECT:
			pCom = std::shared_ptr<CComponent>(new CColliderRect);
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_LINE:
			pCom = std::shared_ptr<CComponent>(new CColliderLine);
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_CIRCLE:
			pCom = std::shared_ptr<CComponent>(new CColliderCircle);
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_OBB2D:
			pCom = std::shared_ptr<CComponent>(new CColliderOBB2D);
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_PIXEL:
			pCom = std::shared_ptr<CComponent>(new CColliderPixel);
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_POINT:
			pCom = std::shared_ptr<CComponent>(new CColliderPoint);
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_RAY:
			pCom = std::shared_ptr<CComponent>(new CColliderRay);
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_SPHERE:
			pCom = std::shared_ptr<CComponent>(new CColliderSphere);
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_TERRAIN:
			pCom = std::shared_ptr<CComponent>(new CColliderTerrain);
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_OBB:
			pCom = std::shared_ptr<CComponent>(new CColliderOBB);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_BAR:
			pCom = std::shared_ptr<CComponent>(new CBar);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_CHECK:
			pCom = std::shared_ptr<CComponent>(new CChkBx);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_BUTTON:
     		pCom = std::shared_ptr<CComponent>(new CUIButton);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_RADIO:
			pCom = std::shared_ptr<CComponent>(new CRadioButton);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_PANEL:
			pCom = std::shared_ptr<CComponent>(new CPanel);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_SLIDER:
			pCom = std::shared_ptr<CComponent>(new CSlider);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_FONT:
			pCom = std::shared_ptr<CComponent>(new CUFont);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_IMAGE:
			pCom = std::shared_ptr<CComponent>(new CUIImage);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_SPRITE:
			pCom = std::shared_ptr<CComponent>(new CUISprite);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_TITLE:
			pCom = std::shared_ptr<CComponent>(new CTitleBar);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_ICON:
			pCom = std::shared_ptr<CComponent>(new CIcon);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_TAB:
			pCom = std::shared_ptr<CComponent>(new CTab);
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_LIST:
			pCom = std::shared_ptr<CComponent>(new CUIList);
			break;
		case SCENECOMPONENT_CLASS_TYPE::SOUND:
			pCom = std::shared_ptr<CComponent>(new CSound);
			break;
		case SCENECOMPONENT_CLASS_TYPE::TILEMAP:
			pCom = std::shared_ptr<CComponent>(new CTileMap);
			break;
		case SCENECOMPONENT_CLASS_TYPE::TILE:
			pCom = std::shared_ptr<CComponent>(new CTile);
			break;
		case SCENECOMPONENT_CLASS_TYPE::RENDERER:
			//pCom = std::shared_ptr<CComponent>(new CRenderer);
			break;
		case SCENECOMPONENT_CLASS_TYPE::PARTICLE:
			pCom = std::shared_ptr<CComponent>(new CParticle);
			break;
		case SCENECOMPONENT_CLASS_TYPE::LIGHT:
			pCom = std::shared_ptr<CComponent>(new CLight);
			break;
		case SCENECOMPONENT_CLASS_TYPE::TERRAIN:
			pCom = std::shared_ptr<CComponent>(new CTerrain);
			break;
		case SCENECOMPONENT_CLASS_TYPE::ARM:
			pCom = std::shared_ptr<CComponent>(new CArm);
			break;
		case SCENECOMPONENT_CLASS_TYPE::DECAL:
			pCom = std::shared_ptr<CComponent>(new CDecal);
			break;
		}

		pCom->m_pObj = this;
		pCom->m_pScene = m_pScene;
		pCom->m_pLayer = m_pLayer;

		pCom->Load(pFile);

		Hierarchy tHierarchy;

		int iLength = 0;

		fread(&iLength, sizeof(int), 1, pFile);

		char strTag[256] = {};
		if (iLength > 0)
		{
			fread(strTag, 1, iLength, pFile);
		}

		iLength = 0;

		tHierarchy.strTag = strTag;

		memset(strTag, 0, 256 * sizeof(char));

		fread(&iLength, sizeof(int), 1, pFile);
		if (iLength > 0)
		{
			fread(strTag, 1, iLength, pFile);
		}

		tHierarchy.strParentTag = strTag;

		if (tHierarchy.strParentTag.empty())
			SetRootComponent(pCom);

		else
		{
			std::shared_ptr<CComponent> pParent = FindComponent(tHierarchy.strParentTag);

			CSceneComponent* _pCom = (CSceneComponent*)pCom.get();

			_pCom->m_pParent = (CSceneComponent*)pParent.get();

			((CSceneComponent*)pParent.get())->AddChild(pCom);
		}
	}
}
