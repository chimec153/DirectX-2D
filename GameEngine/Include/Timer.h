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
	GLOBALCBUFFER		m_tCBuffer;

public:
	float GetDeltaTime()	const;
	float GetFPS()	const;
	float GetTotalTime()	const;
	void SetTimeScale(float fScale);

public:
	bool Init();
	void Update();

public:
	void ShowWindow();

	DECLARE_SINGLE(CTimer)
};


