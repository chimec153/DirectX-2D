#include "Arm.h"

CArm::CArm()	:
	m_vOffset()
	, m_fDist(0.f)
	, m_fMax(500.f)
	, m_fMin(0.f)
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_3D;
	m_iSceneComponentClassType = static_cast<int>(SCENECOMPONENT_CLASS_TYPE::ARM);
}

CArm::CArm(const CArm& arm)	:
	m_vOffset(arm.m_vOffset)
	, m_fDist(arm.m_fDist)
	, m_fMax(arm.m_fMax)
	, m_fMin(arm.m_fMin)
{
}

CArm::~CArm()
{
}

void CArm::SetDist(float fDist)
{
	m_fDist = fDist;
}

void CArm::AddDist(float fDist)
{
	m_fDist += fDist;
}

void CArm::SetMax(float fMax)
{
	m_fMax = fMax;
}

void CArm::SetMin(float fMin)
{
	m_fMin = fMin;
}

void CArm::SetOffset(const Vector3& vOffset)
{
	m_vOffset = vOffset;	
}

bool CArm::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CArm::Start()
{
	CSceneComponent::Start();
}

void CArm::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	Vector3 vAxis = GetWorldAxis(AXIS_Z);

	SetRelativePos(vAxis * -m_fDist +m_vOffset);
}

void CArm::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CArm::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CArm::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CArm::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CArm::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

std::shared_ptr<CComponent> CArm::Clone()
{
	return std::shared_ptr<CComponent>(new CArm(*this));
}

void CArm::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_vOffset, 12, 1, pFile);
	fwrite(&m_fDist, 4, 1, pFile);
	fwrite(&m_fMax, 4, 1, pFile);
	fwrite(&m_fMin, 4, 1, pFile);
}

void CArm::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	fread(&m_vOffset, 12, 1, pFile);
	fread(&m_fDist, 4, 1, pFile);
	fread(&m_fMax, 4, 1, pFile);
	fread(&m_fMin, 4, 1, pFile);
}
