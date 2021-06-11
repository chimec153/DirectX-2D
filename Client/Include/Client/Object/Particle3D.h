#pragma once
#include "Object\GameObject.h"
class CParticle3D :
    public CGameObject
{
public:
    CParticle3D();
    CParticle3D(const CParticle3D& obj);
    virtual ~CParticle3D();

private:
    float       m_fLimitTime;
    float       m_fTime;
    float       m_fScaleSpeed;

public:
    void SetLimitTime(float fTime);
    void SetScaleSpeed(float fSpeed)
    {
        m_fScaleSpeed = fSpeed;
    }
public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual std::shared_ptr<CGameObject> Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

