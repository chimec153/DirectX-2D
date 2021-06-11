#pragma once

#include "GameEngine.h"

class CDevice
{
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pTargetView;
	ID3D11DepthStencilView* m_pDepthView;
	HWND						m_hWnd;
	Resolution					m_tRS;
	float						m_pClearColor[4];
	ID2D1Factory* m_p2DFactory;
	ID2D1RenderTarget* m_p2DRenderTarget;
	static ID3D10Multithread* m_pThread;

public:
	ID3D11Device* GetDevice()	const;
	ID3D11DeviceContext* GetContext()	const;
	IDXGISwapChain* GetSwapChain()	const;
	Resolution GetResolution()	const;
	Vector2 GetWindowRatio()	const;
	ID2D1Factory* Get2DFactory()	const;
	ID2D1RenderTarget* Get2DRenderTarget()	const;
	ID3D10Multithread* GetThread()	const;

public:
	bool Init(HWND hWnd, int iWidth, int iHeight, bool bWindowMode = true);
	void ClearTarget();
	void Render();
	void SetTarget();
	//¿¬½À¿ë
public:
	void OnResize();
	DECLARE_SINGLE(CDevice)

public:
	static void Leave();
	static void Enter();
};

