#pragma once

#include "../Ref.h"

class CUpdater :
    public CRef
{
    friend class CServer;

protected:
    CUpdater(class CServer& core);
    virtual ~CUpdater() = 0;

protected:
    UPDATER_TYPE    m_eType;
    int             m_iPort;
    const TCHAR*     m_pIp;
    SOCKET          m_tServSock;
    SOCKET          m_tAcceptSocket;
    static HANDLE*  m_pThreadPool;
    static HANDLE   m_tAccept;
    class CServer&  m_tCore;

public:
    void WaitToConnect(class CUser* pUser);

public:
    virtual bool Init(UPDATER_TYPE eType, int iPort, const TCHAR* pIp = L"");
    virtual void Update() = 0;
    
};

