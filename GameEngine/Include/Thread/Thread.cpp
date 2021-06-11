#include "Thread.h"

CThread::CThread()	:
	m_bLoop(false)
	, m_hStartEvent()
	, m_hThread()
{
}

CThread::~CThread()
{
	m_bLoop = false;

	if (m_hStartEvent)
	{
		CloseHandle(m_hStartEvent);
	}

	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
}

void CThread::Start()
{
	SetEvent(m_hStartEvent);
}

void CThread::Wait()
{
	WaitForSingleObject(m_hStartEvent, INFINITE);
}

void CThread::Create()
{
	m_hStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//m_hThread = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL);
	
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, this, 0, NULL);
}

unsigned __stdcall CThread::ThreadFunc(void* pData)
{
	CThread* pThread = (CThread*)pData;

	pThread->Wait();
	pThread->Run();

	return 0;
}
