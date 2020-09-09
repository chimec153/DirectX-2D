#pragma once

#include "GameEngine.h"

class CTimer
{
private:
	float				m_fDeltaTime;
	LARGE_INTEGER		m_tPrevTime;
	LARGE_INTEGER		m_tSecond;
	float				m_fTimeScale;
	int					m_iFrame;
	float				m_fFPS;
	float				m_fFrameTime;

public:
	float GetDeltaTime()	const
	{
		return m_fDeltaTime * m_fTimeScale;
	}

	float GetFPS()	const
	{
		return m_fFPS;
	}

public:
	bool Init();
	void Update();

	DECLARE_SINGLE(CTimer)
};

