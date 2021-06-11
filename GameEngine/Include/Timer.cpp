#include "Timer.h"
#include "Engine.h"
#include "Resource/ShaderManager.h"
#include "UI/imgui/imgui.h"

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
	memset(&m_tCBuffer, 0, sizeof(GLOBALCBUFFER));
	m_tCBuffer.vNoiseSize = Vector2(1024.f, 1024.f);
}

CTimer::~CTimer()
{

}

float CTimer::GetDeltaTime() const
{
	return m_fDeltaTime * m_fTimeScale;
}

float CTimer::GetFPS() const
{
	return m_fFPS;
}

float CTimer::GetTotalTime() const
{
	return m_tCBuffer.fAccTime;
}

void CTimer::SetTimeScale(float fScale)
{
	m_fTimeScale =fScale;
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

	m_fDeltaTime = (tTime.QuadPart - m_tPrevTime.QuadPart) / (float)m_tSecond.QuadPart;

	m_tPrevTime = tTime;

	m_tCBuffer.fDeltaTime = m_fDeltaTime * m_fTimeScale;
	m_tCBuffer.fAccTime += m_tCBuffer.fDeltaTime;

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Global", &m_tCBuffer);

	++m_iFrame;

	m_fFrameTime += m_fDeltaTime;

	if (m_fFrameTime >= 1.f)
	{
		m_fFPS = m_iFrame / m_fFrameTime;

		m_fFrameTime -= 1.f;

		m_iFrame = 0;
/*
		char strFPS[32] = {};

		sprintf_s(strFPS, "FPS: %.6f, DT: %.6f\n", m_fFPS, m_fDeltaTime);

		OutputDebugStringA(strFPS);*/
	}
}

void CTimer::ShowWindow()
{
	if(ImGui::Begin("Timer"))
	{
		if (ImGui::SliderFloat("Scale", &m_fTimeScale, 0.f, 5.f))
		{
			SetTimeScale(m_fTimeScale);
		}

		ImGui::Text("FPS: %.2f, DeltaTime: %.2f", m_fFPS, m_fDeltaTime);
	}
	ImGui::End();
}
