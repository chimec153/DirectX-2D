#pragma once
#include "../Ref.h"

enum class USER_STATE
{
    NONE,
    WAIT,
    CONNECT,
    LOGIN,
    END
};

class CUser :
    public CRef
{
    friend class CUpdater;
    friend class CClientSocket;
    friend class CServer;

private:
    CUser();
    virtual ~CUser();

private:
    SOCKET      m_tUserSock;
    SOCKADDR_IN m_tSendAddr;
    SOCKADDR_IN m_tRecvAddr;
    POVERPART    m_pSendPart;
    POVERPART    m_pRecvPart;
    USER_STATE      m_eState;

public:
    POVERPART GetSendPart() const;
    POVERPART GetRecvPart() const;
    SOCKET GetUserSock()    const;
    void SetState(USER_STATE eState);
    USER_STATE GetState()   const;
        
public:
    bool Init(SOCKET tSock, int iPort, const TCHAR* pIP = L"");
    SOCKET AsyncInit();
};

