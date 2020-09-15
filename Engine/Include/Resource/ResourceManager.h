#pragma once

#include "../GameEngine.h"

class CResourceManager
{
private:
	std::unordered_map<std::string, class CMesh*>			m_mapMesh;
	class CMesh2D*		m_pDefaultMesh2D;

public:
	class CMesh2D* GetDefaultMesh()	const;

public:
	bool Init();

	class CMesh* FindMesh(const std::string& strName);
	void ReleaseMesh(const std::string& strName);

	DECLARE_SINGLE(CResourceManager)
};

