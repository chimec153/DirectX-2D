#pragma once
#include "../Ref.h"
class CMultiRenderTarget :
	public CRef
{
	friend class CRenderManager;

private:
	CMultiRenderTarget();
	virtual ~CMultiRenderTarget() override;

private:
	std::vector<ID3D11RenderTargetView*>		m_vecTarget;
	std::vector<ID3D11RenderTargetView*>		m_vecPrevTarget;
	ID3D11Texture2D*							m_pDepthTex;
	ID3D11DepthStencilView*						m_pDepthView;
	ID3D11DepthStencilView*						m_pPrevDepthView;
	float										m_pClearColor[4];

public:
	void AddTarget(ID3D11RenderTargetView* pRTV);
	bool CreateDepth(int iWidth, int iHeight, DXGI_FORMAT eFmt);
	void SetTarget();
	void ResetTarget();
	void Clear();
};

