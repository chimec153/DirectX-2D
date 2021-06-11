#pragma once
#include "GameMode.h"
// 충돌체 끄고켜기 , 데미지 폰트, 
enum class ROCK_TYPE
{
	NONE,
	LAKEROCK1 = 255,
	SMALLROCK1 = 254,
	SMALLROCK2 = 253,
	SMALLROCK3 = 252,
	TURTLEROCK1 = 251,
	LAKEROCK2 = 250,
	LAKEROCK3 = 249,
	LAKESTONE1 = 248,
	LAKESTONE2 = 247,
	LAKESTONE3 = 246,
	GOBLINROCK1 = 245,
	GOBLINROCK2 = 244,
	GOBLINROCK3 = 243,
	GOBLINROCK4 = 242,
	GOBLINROCK5 = 241,
	TURTLEROCK2 = 240,
	TURTLEROCK3 = 239,
	TURTLEROCK4 = 238,
	END
};
enum class TREE_TYPE
{
	NONE,
	MANZANITATREE1 = 255,
	MANZANITATREE2 = 254,
	MANZANITATREE3 = 253,
	MADRONETREE1 = 252,
	MADRONETREE2 = 251,
	MADRONETREE3 = 250,
	STUMP1 = 249,
	STUMP2 = 248,
	END
};
enum class GRASS_TYPE
{
	NONE,
	BUSH1 = 255,
	VINE1 = 254,
	BUSH2 = 253,
	BUSH3 = 252,
	BUSH4 = 251,
	BUSHTREE = 250,
	CATTAIL1 = 249,
	CATTAIL2 = 248,
	CROP1 = 247,
	CROP2 = 246,
	DUCKWEED1 = 245,
	DUCKWEED2 = 244,
	IVY1 = 243,
	IVY2 = 242,
	IVY3 = 241,
	MG1 = 240,
	PGRASS = 239,
	VINE2 = 238,
	WALLCHII = 237,
	WATERCOIN = 236,
	WATERCOIN2 = 235,
	WILDFLOWER = 234,
	END
};

class CMainMode :
	public CGameMode
{
	friend class CScene;
	friend class CEditor3D;

private:
	CMainMode();
	~CMainMode();

public:
	bool Init();
public:
	static bool LoadTexture();
	static void LoadMesh();
	static bool LoadShader();
	static bool LoadSequence();
	bool CreateTree();
	bool CreateRock();
	bool CreateUI();
	bool CreateLOD();
	bool CreateCampA();
	bool CreateParticleProto();
	bool CreateProtoType();

public:
	virtual void Start();
	static bool LoadSequence(const char* pFilePath);
	static bool LoadObjMap(class CScene* pScene, const TCHAR* pFilePath,
		const std::string& strPathKey = TEXTURE_PATH);

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

public:
	void BossSpawn();
};

//매쉬 uv확대 원판 네모판자
//판자 띄어놓으세요
//서류작성 문제풀이
//좋지않은 부분, 개선방법