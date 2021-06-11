#pragma once
#include "Collider.h"
class CColliderCapsule :
    public CCollider
{
public:
    CColliderCapsule();
    CColliderCapsule(const CColliderCapsule& com);
    virtual ~CColliderCapsule();

private:
    CAPSULEINFO m_tInfo;
    float       m_fOffset;

public:
    void SetRadius(float fRadius);
    void SetHeight(float fHeight);
    void SetOffset(float fOffset);
    const CAPSULEINFO& GetInfo()    const;

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual std::shared_ptr<class CComponent> Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

public:
    bool CollisionMouse(const Vector2& vPos);
    bool Collision(class CCollider* pDest);
};

