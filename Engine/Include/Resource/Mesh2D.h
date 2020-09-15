#pragma once
#include "Mesh.h"
class CMesh2D :
	public CMesh
{
	friend class CResourceManager;
	friend class CSceneResource;

public:
	CMesh2D();
	~CMesh2D();

private:
	Vertex						m_tVB;
	Index						m_tIB;
	D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitive;

public:
	bool Init();
	bool CreateMesh();
	virtual void Render(float fTime);
};

