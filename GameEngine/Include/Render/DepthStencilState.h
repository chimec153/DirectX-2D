#pragma once
#include "RenderState.h"

#include "../Device.h"
class CDepthStencilState :
	public CRenderState
{
	friend class CRenderManager;

protected:
	CDepthStencilState();
	virtual ~CDepthStencilState();

private:
	std::vector<D3D11_DEPTH_STENCIL_DESC>	m_vecDesc;
	UINT									m_iStencilRef;
	UINT									m_iPrevStencilRef;

public:
	virtual void SetState();
	virtual void ResetState();

public:
	bool AddDepthStencilInfo(bool bEnable = true, D3D11_DEPTH_WRITE_MASK eMask = D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_FUNC eFunc = D3D11_COMPARISON_LESS,
		bool bStencilEnable = false, UINT8 iReadMask = 0xff, UINT8 iWriteMask = 0xff, const D3D11_DEPTH_STENCILOP_DESC & tFront = {}, const D3D11_DEPTH_STENCILOP_DESC & tBack = {});
	void SetStencilRef(UINT iRef);

public:
	virtual void ShowWindow() override;
};