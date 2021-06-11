#pragma once
#include "Component.h"
class CTestComponent :
	public CComponent
{
	friend class CObj;

public:
	CTestComponent();
	CTestComponent(const CTestComponent& com);
	virtual ~CTestComponent();

public:
	virtual std::shared_ptr<CComponent> Clone();
};

