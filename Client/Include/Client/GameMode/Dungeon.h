#pragma once
#include "GameMode.h"
class CDungeon :
    public CGameMode
{
public:
    CDungeon();
    virtual ~CDungeon();

public:
    virtual bool Init();

public:
    bool LoadMesh();
    bool LoadTexture();
};

