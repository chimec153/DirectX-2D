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
	class CGameObject* m_pObj;
	class CScene* m_pScene;
	class CLayer* m_pLayer;
	COMPONENT_TYPE			m_eType;

public:
	class CGameObject* GetObj()	const;
	COMPONENT_TYPE GetType()	const;
	virtual void SetScene(class CScene* pScene);
	class CLayer* GetLayer()	const;
	class CScene* GetScene()	const;

protected:
	bool					m_bStart;

public:
	bool IsStart()	const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CComponent> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual void ShowWindow() override;

};
