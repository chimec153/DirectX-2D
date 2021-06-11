#pragma once

#include "../Ref.h"

class CThread :
	public CRef
{
	friend class CThreadManager;

protected:
	CThread();
	~CThread();

protected:
	bool	m_bLoop;

private:
	HANDLE	m_hStartEvent;
	HANDLE	m_hThread;

public:
	void Start();
	virtual void Run() = 0;
	void Wait();
	void Create();

private:
	static unsigned WINAPI ThreadFunc(void* pData);
};

