#pragma once
#include "SceneComponent.h"
class CMeshComponent :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CMeshComponent();
	CMeshComponent(const CMeshComponent& com);
	virtual ~CMeshComponent();

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

