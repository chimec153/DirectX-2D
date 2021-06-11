#pragma once

#include "Component.h"

class CObjectComponent :
	public CComponent
{
	friend class CGameObject;

public:
	CObjectComponent();
	CObjectComponent(const CObjectComponent& com);
	virtual ~CObjectComponent();

private:
	OBJ_COMPONENT_CLASS_TYPE m_eClassType;

public:
	OBJ_COMPONENT_CLASS_TYPE GetClassType()	const;
	void SetClassType(OBJ_COMPONENT_CLASS_TYPE eType);

public:
	void SetLayer(class CLayer* pLayer);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<class CComponent> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual void ShowWindow() override;
};