#pragma once
#include "PostProcess.h"
class CHDR :
    public CPostProcess
{
    friend class CRenderManager;

private:
    CHDR();
    CHDR(const CHDR& hdr);
    virtual ~CHDR();

private:
    ID3D11ShaderResourceView* m_pSRV;
    ID3D11UnorderedAccessView* m_pUAV;

    ID3D11ShaderResourceView* m_pAverageSRV;
    ID3D11UnorderedAccessView* m_pAverageUAV;

    ID3D11Buffer* m_pConstBuffer1;
    ID3D11Buffer* m_pConstBuffer2;


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
};

