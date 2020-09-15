#pragma once
#include "Mesh.h"
class CStaticMesh :
	public CMesh
{
private:
	CStaticMesh();
	~CStaticMesh();

public:
	bool Init();
	virtual void Render(float fTime);
};

