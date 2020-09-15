#pragma once

#include "Component.h"

class CObjectComponent	:
	public CComponent
{
	friend class CGameObject;
	
private:
	CObjectComponent();
	CObjectComponent(const CObjectComponent& com);
	virtual ~CObjectComponent();
};