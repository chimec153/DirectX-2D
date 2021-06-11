#pragma once
#include "GameMode.h"
class CTown :
    public CGameMode
{
public:
    CTown();
    virtual ~CTown();

public:
    virtual bool Init();
    virtual void Start();

public:
    bool CreateProtoType();
    bool CreateUI();
    bool LoadShader();
    void CreatePlayer();

public:
    static void LoadMeshs(const char* pFilePath, const std::string& strKey = DATA_PATH);
    static void LoadTextures(const char* pFilePath, const std::string& strKey = DATA_PATH);
    static void LoadProtoTypes(class CScene* pScene, const char* pFilePath, const std::string& strKey = DATA_PATH);
    static void LoadCloneObjects(class CScene* pScene, const char* pFilePath, const std::string& strKey = DATA_PATH);
    static void LoadObjects(class CScene* pScene, const char* pFilePath, const std::string& strKey = DATA_PATH);

};

