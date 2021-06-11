#pragma once

#include "GameEngine.h"
#include "CreateInstance.h"

	class CEngine
	{
	private:
		static bool			m_bLoop;

	private:
		HINSTANCE			m_hInst;
		HWND				m_hWnd;
		Resolution			m_tRS;
		class CCreateInstance*	m_pInstance;
		//std::shared_ptr<class CComponent>		m_pFont;
		//std::shared_ptr<class CGameObject>		m_pTimeObj;
		bool				m_bEditor;
#ifdef _DEBUG
		bool				m_bCollider;
#endif

	public:
		HINSTANCE GetWindowInstance()	const;
		HWND GetWindowHandle()	const;
		const Resolution& GetResolution()	const;
		bool IsExit()	const;
		bool IsEditor()	const;
		void SetEditor();
#ifdef _DEBUG
		void SetCollider(bool bCol);
		bool IsColliderEnabled()	const;
#endif

	public:
		bool Init(const TCHAR* strClass, const TCHAR* strTitle,
			HINSTANCE hInst, int iIcon, int iSmIcon,
			int iWidth, int iHeight, bool bWindowMode = true);
		bool Init(HINSTANCE hInst, HWND hWnd, int iWidth, int iHeight, bool bWindowMode = true);
		int Run();
		void Logic();

	public:
		bool Input(float fTime);
		bool Update(float fTime);
		bool PostUpdate(float fTime);
		bool Collision(float fTime);
		void PreRender(float fTime);
		void Render(float fTime);
		void PostRender(float fTime);

	public:
		template <typename T>
		void SetInstance()
		{
			m_pInstance = new T;

			if (!m_pInstance->Init())
			{
				SAFE_DELETE(m_pInstance);
				return;
			}
		}
		void EndGame();

	private:
		ATOM Register(const TCHAR* strClass, int iIcon, int iSmIcon);
		int Create(const TCHAR* strClass, const TCHAR* strTitle);

		static LRESULT WINAPI WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

		DECLARE_SINGLE(CEngine)
	};
