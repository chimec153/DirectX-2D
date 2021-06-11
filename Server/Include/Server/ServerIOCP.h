#pragma once
#include "Server\IOCP.h"

enum class EDITOR_HEADER
{
    NONE,
    CHAT,
    LOGIN,
    END
};

class CServerIOCP :
    public CIOCP
{
    friend class CServer;

private:
    CServerIOCP(class CServer& core);
    virtual ~CServerIOCP();

public:
    virtual bool Init(UPDATER_TYPE eType, int iPort, const TCHAR* pIp = L"");
    virtual void Update();
    virtual void RecvPacket( POVERPART pPart);

public:
    void RecvLoginPacket(class CUser* pUser,  const char* pPacket);

};

