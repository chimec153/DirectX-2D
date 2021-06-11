#pragma once
#include "Updater.h"
class CIOCP :
    public CUpdater
{
    friend class CServer;

protected:
    CIOCP(class CServer& core);
    virtual ~CIOCP();

public:
    bool Init(UPDATER_TYPE eType, int iPort, const TCHAR* pIp = L"");
    virtual void Update();

public:
    virtual void RecvPacket(POVERPART pPart);
    void SendPacket(class CUser* pUser, const char* pPacket);

private:
    static unsigned __stdcall ThreadFunc(void* pData);

public:
    static void ShowError(int iError);
};

