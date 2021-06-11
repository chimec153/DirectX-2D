#pragma once
#include "Object\GameObject.h"
class CBlueFlame :
    public CGameObject
{
public:
    CBlueFlame();
    CBlueFlame(const CBlueFlame& obj);
    virtual ~CBlueFlame();

private:
    float	m_fSpeed;
    float	m_fDist;
    float	m_fLimitDist;
    Vector3 m_vDir;

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

public:
    void CollisionInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
    void CollisionStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
    void CollisionLast(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

