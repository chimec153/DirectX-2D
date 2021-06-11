#pragma once

#include "CreateInstance.h"

class CClientInstance :
	public CCreateInstance
{
	friend class CEngine;

public:
	CClientInstance();
	~CClientInstance();

public:
	virtual bool Init();
};

