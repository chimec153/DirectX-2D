#pragma once
#include "../Ref.h"
class CComponent :
	public CRef
{
	friend class CGameObject;

protected:
	CComponent();
	CComponent(const CComponent& com);
	virtual ~CComponent() = 0;

protected:
	class CGameObject*		m_pObj;
	class CScene*			m_pScene;

public:
	class CGameObject* GetObj();

protected:
	bool					m_bStart;

public:
	bool IsStart()	const
	{
		return m_bStart;
	}

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

