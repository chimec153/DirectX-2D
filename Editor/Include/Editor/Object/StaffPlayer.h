#pragma once
#include "Player3D.h"
class CStaffPlayer :
    public CPlayer3D
{
public:
    CStaffPlayer();
    CStaffPlayer(const CStaffPlayer& obj);
    virtual ~CStaffPlayer();

private:
    int m_iTrailCount;
    std::shared_ptr<CGameObject>    m_pTrailObj;
    std::vector<VERTEXTRAIL>        m_vecTrailVtx;
    std::shared_ptr<CComponent>     m_pUp;
    std::shared_ptr<CComponent>     m_pDown;
    std::shared_ptr<CComponent>     m_pFire;
    std::shared_ptr<CComponent>     m_pIceCollider;

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
    void CollisionEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
    void AttackStart();
    void sk08Start();
    virtual void CastFinish()   override;
    virtual void CastCancel()   override;
    void CaskSkill();

public:
    virtual void SetTransition();

public:
    virtual void Skill1(float fTime);
    virtual void Skill2(float fTime);
    virtual void Skill3(float fTime);

public:
    void UpdateTrail(float fAniTime, float fDeltaTime);
    void StartTrail();
    void EndTrail();
    void Skill_11(float fTime);
    void SkillBoom(float fTime);
    void SkillIce(float fTime);

public:
    virtual void SetLayer(class CLayer* pLayer) override;
};

