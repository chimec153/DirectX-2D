#pragma once

class CRef
{
protected:
	CRef();
	virtual ~CRef() = 0;

protected:
	int		m_iRefCount;

public:
	void AddRef();
	bool Release();
};

