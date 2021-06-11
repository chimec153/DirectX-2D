#pragma once

#include "Monster3D.h"

class CGoblin :
    public CMonster3D
{
public:
    CGoblin();
    CGoblin(const CGoblin& obj);
    virtual ~CGoblin();

private:
    float               m_fIdleTime;
    float               m_fIdleTimeLimit;
    float               m_fWalkTime;
    float               m_fWalkTimeLimit;

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
    virtual void Move(float fTime);
    virtual void Attack(float fTime);
    virtual void AttackAround(float fTime);
    virtual void HeavyAttack(float fTime);
    virtual void LongAttack(float fTime);
    virtual void Idle(float fTime);

public:
    bool Transition();
    bool DMGToDeath();

public:
    void ThrowRock(float fTime);
    void Death(float fTime);
    void Resolve(float fTime);
    void WalkSound(float fTime);

public:
    virtual void ChangeSequenceAll(const char* pSequence);

public:
    void LootingCallBack();
};

