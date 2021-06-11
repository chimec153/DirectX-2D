#pragma once
#include "Thread/Thread.h"
class CLoadingThread3D :
	public CThread
{
	friend class CThreadManager;

protected:
	CLoadingThread3D();
	virtual ~CLoadingThread3D();

private:
	bool m_bComplete;
	class CScene* m_pScene;
	std::function<void()>	m_Work;
	const char* m_pFilePath;

public:
	bool IsCompleted()	const
	{
		return m_bComplete;
	}
	class CScene* GetScene()	const
	{
		return m_pScene;
	}
	void Complete()
	{
		m_bComplete = true;
	}
	void SetWork(void(*pFunc)());
	template <typename T>
	void SetWork(T* pObj, void(T::* pFunc)())
	{
		m_Work = std::bind(pObj, pFunc);
	}
	void SetFilePath(const char* pFilePath)
	{
		m_pFilePath = pFilePath;
	}

public:
	virtual void Run();
};

