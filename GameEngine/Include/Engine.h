#pragma once

#include "GameEngine.h"

class CEngine
{
private:
	static bool		m_bLoop;

private:
	HINSTANCE		m_hInst;
	HWND			m_hWnd;
	Resolution		m_tRS;

public:
	HINSTANCE GetWindowInstance()	const
	{
		return m_hInst;
	}

	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	const Resolution& GetResolution()	const
	{
		return m_tRS;
	}

	bool IsExit()	const
	{
		return !m_bLoop;
	}

public:
	bool Init(const TCHAR* strClass, const TCHAR* strTitle,
		HINSTANCE hInst, int iIcon, int iSmIcon,
		int iWidth, int iHeight, bool bWindowMode = true);
	bool Init(HINSTANCE hInst, HWND hWnd, int iWidth, int iHeight, bool bWindowMode = true);
	int Run();
	void Logic();

public:
	int Input(float fTime);
	int Update(float fTime);
	int PostUpdate(float fTime);
	int Collision(float fTime);
	int PreRender(float fTime);
	int Render(float fTime);
	int PostRender(float fTime);

private:
	ATOM Register(const TCHAR* strClass, int iIcon, int iSmIcon);
	int Create(const TCHAR* strClass, const TCHAR* strTitle);

	static LRESULT WINAPI WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	DECLARE_SINGLE(CEngine)
};

