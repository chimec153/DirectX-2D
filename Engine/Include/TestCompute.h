#pragma once
#include "Component/SceneComponent.h"
class CTestCompute :
	public CSceneComponent
{
public:
	CTestCompute();
	virtual ~CTestCompute();

private:

public:
	void Create(float width, float height, DXGI_FORMAT format);
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

