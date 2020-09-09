#pragma once
#include "MeshComponent.h"
class CStaticMeshComponent :
	public CMeshComponent
{
	friend class CGameObject;

protected:
	CStaticMeshComponent();
	CStaticMeshComponent(const CStaticMeshComponent& com);
	virtual ~CStaticMeshComponent();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
};
