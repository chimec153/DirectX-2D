#pragma once

#include "../Ref.h"

typedef struct _tagVertexBuffer
{
	ID3D11Buffer*		pBuffer;
	int					iSize;
	int					iCount;
	DXGI_USAGE			eUsage;
}Vertex,*PVertex;

typedef struct _tagIndexBuffer
{
	ID3D11Buffer*				pBuffer;
	int							iSize;
	int							iCount;
	DXGI_USAGE					eUsage;
	DXGI_FORMAT					eFmt;
}Index, *PIndex;

class CMesh :
	public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;

protected:
	CMesh();
	virtual ~CMesh() = 0;

protected:
	Vector3		m_vMin;
	Vector3		m_vMax;

public:
	Vector3 GetMax()	const
	{
		return m_vMax;
	}

	Vector3 GetMin()	const
	{
		return m_vMin;
	}

public:
	bool Init();
	virtual void Render(float fTime) = 0;
};

