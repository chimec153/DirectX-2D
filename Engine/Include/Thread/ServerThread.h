#pragma once
#include "../Thread/Thread.h"
class CServerThread :
    public CThread
{
    friend class CThreadManager;

private:
    CServerThread();
    virtual ~CServerThread();

private:
    class CUpdater* m_pUpdater;
public:
    void SetUpdater(class CUpdater* pUpdater);

public:
    virtual void Run();
};

