#pragma once

#include "Thread/Thread.h"

class CLoadingThread :
	public CThread
{
public:
	CLoadingThread();
	~CLoadingThread();

private:
	bool	m_bIsDone;
	class CScene* m_pScene;
	int		m_iProgress;

public:
	bool IsDone()	const
	{
		return m_bIsDone;
	}
	void AddProgress()
	{
		m_iProgress++;
	}
	int GetProgress()	const
	{
		return m_iProgress;
	}

	class CScene* GetScene()	const
	{
		return m_pScene;
	}

public:
	virtual void Run();
};

