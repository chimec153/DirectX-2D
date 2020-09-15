#pragma once

#include "../GameEngine.h"

class CSceneResource
{
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
	std::unordered_map<std::string, class CMesh*>			m_mapMesh;
	std::unordered_map<std::string, class CShader*>			m_mapShader;

public:
	bool Init();

	class CShader* FindShader(const std::string& strName);
};

