#pragma once
#include "Object/GameObject.h"
class CCoin :
    public CGameObject
{
public:
    CCoin();
    CCoin(const CCoin& obj);
    virtual ~CCoin();

private:
    Vector3 m_vSpeed;
    int     m_iGold;
    std::shared_ptr<CGameObject>    m_pPlayer;
    float   m_fRatio;
    class CTerrain* m_pTerrain;

public:
    void SetGold(int iGold)
    {
        m_iGold = iGold;
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
    void CollisionInit(class CCollider* pSrc, class CCollider* pDset, float fTime);
    void CollisionStay(class CCollider* pSrc, class CCollider* pDset, float fTime);
    void CollisionEnd(class CCollider* pSrc, class CCollider* pDset, float fTime);
};

