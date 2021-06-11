#pragma once
#include "Object\GameObject.h"
class CGoblinRock :
    public CGameObject
{
public:
    CGoblinRock();
    CGoblinRock(const CGoblinRock& rock);
    virtual ~CGoblinRock();

private:
    int m_iAttackMax;
    int m_iAttackMin;
    float m_fSpeed;
    float m_fDist;
    float m_fLimitDist;
    class CGameObject* m_pOwner;

public:
    int GetAttackMax()  const
    {
        return m_iAttackMax;
    }
    int GetAttackMin()  const
    {
        return m_iAttackMin;
    }
    void SetAttackMax(int iMax)
    {
        m_iAttackMax = iMax;
    }
    void SetAttackMin(int iMin)
    {
        m_iAttackMin = iMin;
    }
    void SetOwner(class CGameObject* pObj)
    {
        m_pOwner = pObj;
    }
    class CGameObject* GetOwner()   const
    {
        return m_pOwner;
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

public:
    virtual void CollisionInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
    virtual void CollisionStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
    virtual void CollisionEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
    void DestroyRock();
};

