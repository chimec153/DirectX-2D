#include "GameObject.h"
#include "../Component/SceneComponent.h"
#include "../Component/ObjectComponent.h"
#include "../Resource/Material.h"

CGameObject::CGameObject()	:
	m_pScene(nullptr),
	m_bStart(false),
	m_pRootComponent(nullptr)
{
}

CGameObject::CGameObject(const CGameObject& obj)
{
	*this = obj;
}

CGameObject::~CGameObject()
{
	SAFE_RELEASE(m_pRootComponent);
	SAFE_RELEASE_VECLIST(m_vecObjectComponent);
}

void CGameObject::SetRootComponent(CSceneComponent* pComponent)
{
	SAFE_RELEASE(m_pRootComponent);

	m_pRootComponent = pComponent;

	if (m_pRootComponent)
		m_pRootComponent->AddRef();
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
	m_pRootComponent->Update(fTime);

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->Update(fTime);
}

void CGameObject::PostUpdate(float fTime)
{
	m_pRootComponent->PostUpdate(fTime);

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->PostUpdate(fTime);
}

void CGameObject::Collision(float fTime)
{
	m_pRootComponent->Collision(fTime);

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->Collision(fTime);
}

void CGameObject::PreRender(float fTime)
{
	m_pRootComponent->PreRender(fTime);

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->PreRender(fTime);
}

void CGameObject::Render(float fTime)
{
	m_pRootComponent->Render(fTime);

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->Render(fTime);
}

void CGameObject::PostRender(float fTime)
{
	m_pRootComponent->PostRender(fTime);

	size_t iSize = m_vecObjectComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjectComponent[i]->PostRender(fTime);
}

void CGameObject::SetInheritScale(bool bInherit)
{
	m_pRootComponent->SetInheritScale(bInherit);
}

void CGameObject::SetInheritRotX(bool bInherit)
{
	m_pRootComponent->SetInheritRotX(bInherit);
}

void CGameObject::SetInheritRotY(bool bInherit)
{
	m_pRootComponent->SetInheritRotY(bInherit);
}

void CGameObject::SetInheritRotZ(bool bInherit)
{
	m_pRootComponent->SetInheritRotZ(bInherit);
}

void CGameObject::InheritScale()
{
	m_pRootComponent->InheritScale();
}

void CGameObject::InheritRot()
{
	m_pRootComponent->InheritRot();
}

void CGameObject::InheritPos()
{
	m_pRootComponent->InheritPos();
}

void CGameObject::SetRelativeScale(const Vector3& v)
{
	m_pRootComponent->SetRelativeScale(v);
}

void CGameObject::SetRelativeScale(float x, float y, float z)
{
	m_pRootComponent->SetRelativeScale(x, y, z);
}

void CGameObject::SetRelativeRot(const Vector3& v)
{
	m_pRootComponent->SetRelativeRot(v);
}

void CGameObject::SetRelativeRot(float x, float y, float z)
{
	m_pRootComponent->SetRelativeRot(x, y, z);
}

void CGameObject::SetRelativeRotX(float x)
{
	m_pRootComponent->SetRelativeRotX(x);
}

void CGameObject::SetRelativeRotY(float y)
{
	m_pRootComponent->SetRelativeRotY(y);
}

void CGameObject::SetRelativeRotZ(float z)
{
	m_pRootComponent->SetRelativeRotZ(z);
}

void CGameObject::SetRelativePos(const Vector3 & v)
{
	m_pRootComponent->SetRelativePos(v);
}

void CGameObject::SetRelativePos(float x, float y, float z)
{
	m_pRootComponent->SetRelativePos(x, y, z);
}

void CGameObject::AddRelativeScale(const Vector3 & v)
{
	m_pRootComponent->AddRelativeScale(v);
}

void CGameObject::AddRelativeScale(float x, float y, float z)
{
	m_pRootComponent->AddRelativeScale(x, y, z);
}

void CGameObject::AddRelativeRot(const Vector3 & v)
{
	m_pRootComponent->AddRelativeRot(v);
}

void CGameObject::AddRelativeRot(float x, float y, float z)
{
	m_pRootComponent->AddRelativeRot(x, y, z);
}

void CGameObject::AddRelativeRotX(float x)
{
	m_pRootComponent->AddRelativeRotX(x);
}

void CGameObject::AddRelativeRotY(float y)
{
	m_pRootComponent->AddRelativeRotY(y);
}

void CGameObject::AddRelativeRotZ(float z)
{
	m_pRootComponent->AddRelativeRotZ(z);
}

void CGameObject::AddRelativePos(const Vector3 & v)
{
	m_pRootComponent->AddRelativePos(v);
}

void CGameObject::AddRelativePos(float x, float y, float z)
{
	m_pRootComponent->AddRelativePos(x, y, z);
}

Vector3 CGameObject::GetRelativeScale() const
{
	return m_pRootComponent->GetRelativeScale();
}

Vector3 CGameObject::GetRelativeRot() const
{
	return m_pRootComponent->GetRelativeRot();
}

Vector3 CGameObject::GetRelativePos() const
{
	return m_pRootComponent->GetRelativePos();
}

Vector3 CGameObject::GetRelativeAxis(AXIS axis) const
{
	return m_pRootComponent->GetRelativeAxis(axis);
}

Vector3 CGameObject::GetWorldScale() const
{
	return m_pRootComponent->GetWorldScale();
}

Vector3 CGameObject::GetWorldRot() const
{
	return m_pRootComponent->GetWorldRot();
}

Vector3 CGameObject::GetWorldPos() const
{
	return m_pRootComponent->GetWorldPos();
}

Vector3 CGameObject::GetWorldPivot() const
{
	return m_pRootComponent->GetWorldPivot();
}

Vector3 CGameObject::GetWorldAxis(AXIS axis) const
{
	return m_pRootComponent->GetWorldAxis(axis);
}

void CGameObject::SetWorldScale(const Vector3 & v)
{
	m_pRootComponent->SetWorldScale(v);
}

void CGameObject::SetWorldScale(float x, float y, float z)
{
	m_pRootComponent->SetWorldScale(x, y, z);
}

void CGameObject::SetWorldRot(const Vector3 & v)
{
	m_pRootComponent->SetWorldRot(v);
}

void CGameObject::SetWorldRot(float x, float y, float z)
{
	m_pRootComponent->SetWorldRot(x, y, z);
}

void CGameObject::SetWorldRotX(float x)
{
	m_pRootComponent->SetWorldRotX(x);
}

void CGameObject::SetWorldRotY(float y)
{
	m_pRootComponent->SetWorldRotY(y);
}

void CGameObject::SetWorldRotZ(float z)
{
	m_pRootComponent->SetWorldRotZ(z);
}

void CGameObject::SetWorldPos(const Vector3 & v)
{
	m_pRootComponent->SetWorldPos(v);
}

void CGameObject::SetWorldPos(float x, float y, float z)
{
	m_pRootComponent->SetWorldPos(x, y, z);
}

void CGameObject::AddWorldScale(const Vector3 & v)
{
	m_pRootComponent->AddWorldScale(v);
}

void CGameObject::AddWorldScale(float x, float y, float z)
{
	m_pRootComponent->AddWorldScale(x, y, z);
}

void CGameObject::AddWorldRot(const Vector3 & v)
{
	m_pRootComponent->AddWorldRot(v);
}

void CGameObject::AddWorldRot(float x, float y, float z)
{
	m_pRootComponent->AddWorldRot(x, y, z);
}

void CGameObject::AddWorldRotX(float x)
{
	m_pRootComponent->AddWorldRotX(x);
}

void CGameObject::AddWorldRotY(float y)
{
	m_pRootComponent->AddWorldRotY(y);
}

void CGameObject::AddWorldRotZ(float z)
{
	m_pRootComponent->AddWorldRotZ(z);
}

void CGameObject::AddWorldPos(const Vector3 & v)
{
	m_pRootComponent->AddWorldPos(v);
}

void CGameObject::AddWorldPos(float x, float y, float z)
{
	m_pRootComponent->AddWorldPos(x, y, z);
}

void CGameObject::SetPivot(const Vector3& v)
{
	m_pRootComponent->SetPivot(v);
}

void CGameObject::SetPivot(float x, float y, float z)
{
	m_pRootComponent->SetPivot(x,y,z);
}

void CGameObject::SetMeshSize(const Vector3& v)
{
	m_pRootComponent->SetMeshSize(v);
}
