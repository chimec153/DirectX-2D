#pragma once
#include "Object\GameObject.h"
class CCylopsBall :
    public CGameObject
{
public:
    CCylopsBall();
    CCylopsBall(const CCylopsBall& ball);
    virtual ~CCylopsBall();

private:
    class CAnimation* m_pBallAni;
    bool                m_bThrow;
    float               m_fSpeed;

public:
    void Throw();
    void Punched();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float fTime) override;
    virtual void PostUpdate(float fTime) override;
    virtual void Collision(float fTime) override;
    virtual void PreRender(float fTime) override;
    virtual void Render(float fTime) override;
    virtual void PostRender(float fTime) override;
    virtual std::shared_ptr<CGameObject> Clone();

public:
    virtual void Save(FILE* pFile) override;
    virtual void Load(FILE* pFile) override;

public:
    bool Transition();

public:
    void CollisionInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
    void CollisionStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
    void CollisionLast(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
    void LongAtkBEnd();
};

