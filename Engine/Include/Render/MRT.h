#pragma once
#include "../Ref.h"
class CMRT :
	public CRef
{
	friend class CRenderManager;

private:
	CMRT();
	CMRT(const CMRT& mrt);
	virtual ~CMRT();

private:
	ID3D11Texture2D* m_pTex;
	ID3D11ShaderResourceView* m_pSRV;
	ID3D11UnorderedAccessView*	m_pUAV;
	class CTexture*	m_pTexture;
	ID3D11ShaderResourceView* m_pPrevSRV;
	ID3D11RenderTargetView*	m_pPrevRenderTarget;
	ID3D11RenderTargetView*	m_pRenderTarget;
	ID3D11DepthStencilView* m_pDepthView;
	ID3D11Texture2D* m_pDepthTex;
	ID3D11DepthStencilView* m_pPrevDepthView;
	float	m_pClearColor[4];
	ID3D11ShaderResourceView* m_pDepthSRV;
#ifdef _DEBUG
	Matrix	m_matDebugWVP;
#endif
	D3D11_VIEWPORT		m_tViewPort;
	D3D11_VIEWPORT		m_tPrevViewPort;
	ID3D11RenderTargetView* m_pPrevRTV[6];

public:
	ID3D11RenderTargetView* GetRenderTargetView()	const;
	ID3D11Texture2D* GetTex()	const;
	ID3D11DepthStencilView* GetDepthStencilView()	const;
#ifdef _DEBUG
	const Matrix& GetDebugWVP()	const;
#endif

public:
	bool CreateTexture(int iWidth, int iHeight, DXGI_FORMAT eTexFmt, DXGI_FORMAT eFmt, DXGI_FORMAT eDepthFmt);
	bool CreateCubeTexture(int iCubeMapSize, DXGI_FORMAT eDepthFormat);
#ifdef _DEBUG
	bool CreateDebugMatrix(const Vector3& vScale, const Vector3& vTranslation);
#endif

public:
	void SetTarget();
	void SetDepthView();
	void SetCubeDepthView();
	void ResetTarget();
	void ClearTarget();
	void SetShader(int iRegister, int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	void ResetShader(int iRegister, int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	void CSSetUAV(int iRegister);
	void SetDepthSRV(int iRegister, int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	void ResetDepthSRV(int iRegister, int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	void SetViewPort();
	void ResetViewPort();
};

