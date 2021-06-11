#pragma once
#include "Component\SceneComponent.h"
class CCoolTimeComponent :
    public CSceneComponent
{
    friend class CObj;

private:
    CCoolTimeComponent();
    CCoolTimeComponent(const CCoolTimeComponent& com);
    virtual ~CCoolTimeComponent() override;

private:
    AniCBuffer  m_tCBuffer;

public:
    void SetRate(float fRate)
    {
        m_tCBuffer.fFrame = fRate;
    }

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float fTime) override;
    virtual void PostUpdate(float fTime) override;
    virtual void Collision(float fTime) override;
    virtual void PreRender(float fTime) override;
    virtual void Render(float fTime) override;
    virtual void PostRender(float fTime) override;
    virtual CCoolTimeComponent* Clone(float fTime);

public:
    virtual void Save(FILE* pFile) override;
    virtual void Load(FILE* pFile) override;

};

