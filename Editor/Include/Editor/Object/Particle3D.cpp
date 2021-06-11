#include "Particle3D.h"

CParticle3D::CParticle3D()  :
    m_fLimitTime(-1.f)
    , m_fTime(0.f)
    , m_fScaleSpeed(0.f)
{

}

CParticle3D::CParticle3D(const CParticle3D& obj)    :
    CGameObject(obj)
    , m_fLimitTime(obj.m_fLimitTime)
    , m_fTime(obj.m_fTime)
    , m_fScaleSpeed(obj.m_fScaleSpeed)
{

}

CParticle3D::~CParticle3D()
{

}

void CParticle3D::SetLimitTime(float fTime)
{
    m_fLimitTime = fTime;
}

bool CParticle3D::Init()
{
    if (!CGameObject::Init())
        return false;

    return true;
}

void CParticle3D::Start()
{
    CGameObject::Start();
}

void CParticle3D::Update(float fTime)
{
    CGameObject::Update(fTime);

    if (m_fLimitTime != -1.f)
    {
        m_fTime += fTime;

        if (m_fTime >= m_fLimitTime)
        {
            Destroy();
        }
    }

    Vector3 vScale = {}; 
    
    vScale = m_fScaleSpeed* fTime;

    AddWorldScale(vScale);
}

void CParticle3D::PostUpdate(float fTime)
{
    CGameObject::PostUpdate(fTime);
    }

void CParticle3D::Collision(float fTime)
{
    CGameObject::Collision(fTime);
}

void CParticle3D::PreRender(float fTime)
{
    CGameObject::PreRender(fTime);
}

void CParticle3D::Render(float fTime)
{
    CGameObject::Render(fTime);
}

void CParticle3D::PostRender(float fTime)
{
    CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CParticle3D::Clone()
{
    return std::shared_ptr<CGameObject>(new CParticle3D(*this));
}

void CParticle3D::Save(FILE* pFile)
{
    CGameObject::Save(pFile);
}

void CParticle3D::Load(FILE* pFile)
{
    CGameObject::Load(pFile);
}