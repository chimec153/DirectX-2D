#pragma once
#include "../Ref.h"
class CPostProcess :
	public CRef
{
	friend class CRenderManager;

protected:
	CPostProcess();
	CPostProcess(const CPostProcess& pro);
	virtual ~CPostProcess();

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

