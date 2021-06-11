#pragma once
#include "Editor/LoadingThread3D.h"
#include "Editor\EditorFlags.h"
class CTownLoadingThread :
    public CLoadingThread3D
{
    friend class CThreadManager;

private:
    CTownLoadingThread();
    virtual ~CTownLoadingThread();

private:
    LOADING_TYPE    m_eType;
    const char* m_pFilePath;

public:
    void SetLoadingType(LOADING_TYPE eType)
    {
        m_eType = eType;
    }
    void SetFilePath(const char* pFilePath)
    {
        m_pFilePath = pFilePath;
    }

public:
    virtual void Run();
};

