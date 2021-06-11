#pragma once

#include "GameEngine.h"

enum class LAYER
{
	TERRAIN,
	OUTLINE,
	ALBEDO,
	COLLIDER,
	DECAL,
	GBUFFER,
	CUBEMAP,
	ALPHA,
	SWORD_LIGHT,
	PARTICLE,
	UI,
	EQUIP,
	END
};

class CRenderManager
{
private:
	std::vector<class CSceneComponent*>								m_vecComponent[static_cast<int>(LAYER::END)];
	std::vector<class CSceneComponent*>								m_vec2DComponent;
	std::vector<class CSceneComponent*>								m_vecUI;
	std::unordered_map<std::string, class CRenderState*>			m_mapState;
	std::unordered_map<std::string, class CMultiRenderTarget*>		m_mapMultiRenderTarget;
	std::list<class CGameObject*>									m_ObjList;
	std::unordered_map<unsigned __int64, class CLayerInstance*>		m_mapInstancing;
	std::list<class CRenderInstancing*>								m_RenderInstList;
	std::list<class CLayer*>										m_LayerList;
	class CLayerInstance* m_pTileInstancing;
	std::unordered_map<std::string, class CMRT*>				m_mapMRT;
	std::list<class CSceneComponent*>							m_PostList;
	std::shared_ptr<class CShader>								m_pFullScreenShader;
	std::shared_ptr<class CShader>								m_pLightAccShader;
	std::shared_ptr<class CShader>								m_pLightCombineShader;
	std::shared_ptr<class CShader>								m_pLensShader;
	std::vector<class CObjectComponent*>						m_vecObjCom;
#ifdef _DEBUG
	std::shared_ptr<class CShader>								m_pDebugShader;
	class CMesh*								m_pDebugMesh;
	std::shared_ptr<class CMaterial>							m_pDebugMtrl;
#endif
	class CBlur* m_pBlur;
	class CBlur* m_pFullBlur;
	class CRenderState* m_pAlphaBlend;
	class CRenderState* m_pNoDepthWrite;
	std::shared_ptr<class CShader>								m_pAlphaShader;
	std::shared_ptr<class CShader>								m_pEquipShader;
	std::shared_ptr<class CShader>								m_pRadialBlur;
	TransformCBuffer											m_tEquipBuffer;
	bool														m_bFullBlur;
	GLOBALCBUFFER												m_tGlobalBuffer;
	std::shared_ptr<CShader>									m_pShader;
	std::shared_ptr<CShader>									m_pPointShader;

public:
	bool Init();
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);
	void Clear();

public:
	void ShowWindow();

public:
	void SetEquipBuffer(const TransformCBuffer& tCBuffer);

public:
	bool AddComponent(class CSceneComponent* pComponent);
	bool AddAnim2DInstancing(class CSpriteComponent* pComponent);
	bool AddColliderInstancing(class CCollider* pComponent);
	void AddObj(class CUIObject* pObj);
	void ChangeScene();
	void AddObjCom(class CObjectComponent* pObjCom);

public:
	bool AddBlendInfo(const std::string& strTag, bool bEnable = true, D3D11_BLEND eSrcBlend = D3D11_BLEND_SRC_ALPHA, D3D11_BLEND eDestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP eBlendOp = D3D11_BLEND_OP_ADD, D3D11_BLEND eSrcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND eDestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP eBlendOpAlpha = D3D11_BLEND_OP_ADD, UINT8 cRenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(const std::string& strTag, bool bAlpha = false, bool bBlend = false);
	bool AddRasterizerInfo(const std::string& strTag, D3D11_FILL_MODE eFillMode = D3D11_FILL_SOLID, D3D11_CULL_MODE eCullMode = D3D11_CULL_BACK, bool bClock = false, bool bClip = true,
		int iDepthBias = 0, float fDepthBiasClamp = 0.f, float fSlopeScaledDepthBias = 0.f);
	bool CreateRasterizerState(const std::string& strTag);

public:
	bool CreateDepthStencilState(const std::string& strTag, bool bEnable = true, D3D11_DEPTH_WRITE_MASK eMask = D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_FUNC eFunc = D3D11_COMPARISON_LESS,
		bool bStencilEnable = false, UINT8 iReadMask = 0xff, UINT8 iWriteMask = 0xff, const D3D11_DEPTH_STENCILOP_DESC & tFront = {}, const D3D11_DEPTH_STENCILOP_DESC & tBack = {});

private:
	void RenderObject(float fTime);
	void Render3D(float fTime);
	void RenderUI(float fTime);

private:
	void RenderTarget2D(float fTime);
	void PostProcess2D(float fTime);

private:
	void RenderShadow(float fTime);
	void RenderTerrain(float fTime);
	void RenderOutLine(float fTime);
	void RenderCollider(float fTime);
	void RenderCubeMap(float fTime);
	void RenderAlbedo(float fTime);
	void RenderDecal(float fTime);
	void RenderGBuffer(float fTime);
	void RenderLightAcc(float fTime);
	void RenderLightCombine(float fTime);
	void RenderScene(float fTime);
	void RenderAlpha(float fTime);
	void RenderSwordLight(float fTime);
	void RenderParticle(float fTime);
	void RenderEtc(float fTime);
	void RenderMinimap(float fTime);

public:
	void SetState(const std::string& strTag);
	void ResetState(const std::string& strTag);

public:
	void AddTileInstancing(class CTile* pTile);
	void RenderTileInstancing();
	void ClearTileInstancing();
	void SetTileInstTex(const std::string& strKey, bool bArray);
	PInstancingBuffer CreateInstancingBuffer(int iSize, int iCount);
	class CLayerInstance* FindInstancingGeometry(unsigned __int64);
	void ResizeInstancingBuffer(PInstancingBuffer pBuffer, int iCount);
	void CopyInstancingData(PInstancingBuffer pBuffer, int iCount);
	void ComputeInstancing();
	void SetTilesTexture(const std::string& strKey);
	void SetTilesTexture(std::shared_ptr<class CTexture> pTex);

public:
	void AddRenderObject(class CGameObject* pObj);
	void AddLayer(class CLayer* pLayer);

public:
	class CRenderState* FindState(const std::string& strTag);

private:
	static bool SortY(class CSceneComponent* pCom1, class CSceneComponent* pCom2);
	static int __cdecl SortZ(const void* pSrc, const void* pDst);

public:
	static unsigned int GetHashingKey(const std::string& strKey);

public:
	class CMRT* FindRenderTarget(const std::string& strKey);
	void SetTarget(const std::string& strKey);
	void ResetTarget(const std::string& strKey);
	void SetShader(const std::string& strKey, int iRegister, int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	void ResetShader(const std::string& strKey, int iRegister, int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	void SetUAV(const std::string& strKey, int iRegister);
#ifdef _DEBUG
	void CreateDebugWVP(const std::string& strKey, const Vector3& vScale, const Vector3& vTranslation);
#endif
	void ClearTarget(const std::string& strKey);
	bool CreateMRT(const std::string& strKey,int iWidth, int iHeight, DXGI_FORMAT eTexFmt, DXGI_FORMAT eFmt, DXGI_FORMAT eDepthFmt);

public:
	void SetRenderTargetView(const std::string& strKey, const std::string& strMRT);
	bool CreateMultiRenderTarget(const std::string& strKey, int iWidth = 0, 
		int iHeight = 0, DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);
	void SetMRT(const std::string& strKey);
	void ResetMRT(const std::string& strKey);
	void ClearMRT(const std::string& strKey);
	class CMultiRenderTarget* FindMRT(const std::string& strKey);

private:
	static bool SortVZ(class CSceneComponent* pSrc, class CSceneComponent* pDest);
	static bool SortVZInv(class CSceneComponent* pSrc, class CSceneComponent* pDest);
	static bool SortZOrder(class CSceneComponent* pSrc, class CSceneComponent* pDest);

public:
	void SetFullBlur(bool bBlur);
	void SetBlurCenter(const Vector2& vCenter);
	void SetSampleDist(float fDist);
	void SetSampleStrength(float fStrength);

	DECLARE_SINGLE(CRenderManager)
};