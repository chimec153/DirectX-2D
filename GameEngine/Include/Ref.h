#pragma once

#include "GameEngine.h"

class CRef
{
protected:
	CRef();
	virtual ~CRef() = 0;

protected:
	int				m_iRefCount;
	std::string		m_strName;
	bool			m_bActive;
	bool			m_bEnable;

public:
	bool IsActive()	const
	{
		return m_bActive;
	}

	bool IsEnable()	const
	{
		return m_bEnable;
	}

	void SetEnable(bool bEnable)
	{
		m_bEnable = bEnable;
	}

	void Destroy()
	{
		m_bActive = false;
	}

	void SetName(const std::string strName)
	{
		m_strName = strName;
	}

	const std::string& GetName()	const
	{
		return m_strName;
	}

public:
	void AddRef();
	int Release();

};

