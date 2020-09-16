#pragma once

#include "../GameEngine.h"

class CResourceManager
{
private:
	std::unordered_map<std::string, class CMesh*>			m_mapMesh;
	std::unordered_map<std::string, class CMaterial*>		m_mapMaterial;
	class CMesh2D*											m_pDefaultMesh2D;

public:
	class CMesh2D* GetDefaultMesh()	const;

public:
	bool Init();

	class CMesh* FindMesh(const std::string& strName);
	void ReleaseMesh(const std::string& strName);

public:
	class CMaterial* FindMaterial(const std::string& strName);
	void ReleaseMaterial(const std::string& strName);

public:
	CMaterial* CreateMaterial(const std::string& strName);

	DECLARE_SINGLE(CResourceManager)
};

