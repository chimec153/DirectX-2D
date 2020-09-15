#include "Timer.h"

DEFINITION_SINGLE(CTimer)

CTimer::CTimer()	:
	m_fDeltaTime(0.f),
	m_tPrevTime(),
	m_tSecond(),
	m_fTimeScale(1.f),
	m_iFrame(0),
	m_fFPS(0.f),
	m_fFrameTime(0.f)
{

}

CTimer::~CTimer()
{

}

bool CTimer::Init()
{
	QueryPerformanceCounter(&m_tPrevTime);

	QueryPerformanceFrequency(&m_tSecond);

	return true;
}

void CTimer::Update()
{
	LARGE_INTEGER tTime;

	QueryPerformanceCounter(&tTime);

	m_fDeltaTime = tTime.QuadPart - m_tPrevTime.QuadPart / (float)m_tSecond.QuadPart;

	m_tPrevTime = tTime;

	++m_iFrame;

	m_fFrameTime += m_fDeltaTime;

	if (m_fFrameTime >= 1.f)
	{
		m_fFrameTime -= 1.f;

		m_fFPS = m_iFrame / m_fFrameTime;

		m_iFrame = 0;
	}
}
