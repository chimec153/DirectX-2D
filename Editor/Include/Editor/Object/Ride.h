#pragma once
#include "Object\GameObject.h"
class CRide :
    public CGameObject
{
public:
    CRide();
    CRide(const CRide& ride);
    virtual ~CRide();

private:
    class CPlayer3D*           m_pPlayer; 
    class CAnimation*           m_pAni;
    float       m_fHeight;

public:
    void SetPlayer();
    void ExitPlayer();

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
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

public:
    bool WalkToIdle();
    bool IdleToFlyWait();
    bool IdleToFlyUp();
    bool FlyWaitToForward();
    bool FlyWaitToDescent();
    bool FlyWaitToBack();
    bool FlyForwardToWait();
    bool FlyDescentToWait();
    bool FlyWaitToIdle();
};

