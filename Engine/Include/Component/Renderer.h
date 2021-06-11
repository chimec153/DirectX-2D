#pragma once
#include "../Ref.h"
class CRenderer	:
	public CRef
{
public:
	CRenderer();
	CRenderer(const CRenderer& com);
	virtual ~CRenderer();

private:
	class CMesh*	m_pMesh;
	std::vector<std::vector<std::shared_ptr<class CShader>>> m_vecShader;
	std::vector<std::vector<std::shared_ptr<class CMaterial>>> m_vecMaterial;
	std::vector<std::vector<class CRenderState*>> m_vecRenderStates[static_cast<int>(RENDER_STATE_TYPE::END)];
	class CAnimation*				m_pAnimation;
	bool							m_bIndexMode;
	int								m_iIndex;
	int								m_iStartIndex;
	class CRenderInstancing*		m_pInstancing;
	int								m_iInstIndex;
	class CSceneComponent*			m_pOwner;

public:
	void SetMesh(const std::string& strKey);
	void SetMaterial(const std::string& strKey,int iContainer = 0, int iSubset = 0);
	void SetShader(const std::string& strKey, int iContainer = 0, int iSubset = 0);
	void SetMesh(class CMesh* pMesh);
	void SetMaterial(std::shared_ptr<class CMaterial> pMtrl, int iContainer = 0, int iSubset = 0);
	void SetShader(std::shared_ptr<class CShader> pShader, int iContainer = 0, int iSubset = 0);
	class CMesh* GetMesh()	const;
	std::shared_ptr<class CShader> GetShader(int iContainer = 0, int iSubset = 0)	const;
	std::shared_ptr<class CMaterial> GetMaterial(int iContainer = 0, int iSubset = 0)	const;
	size_t GetMaterialSize()	const;
	size_t GetMaterialSubsetSize(int idx = 0)	const;
	void AddRenderState(const std::string& strKey, int iContainer = 0, int iSubset = 0);
	void AddRenderState(class CRenderState* pState, int iContainer = 0, int iSubset = 0);
	class CRenderState* FindRenderState(RENDER_STATE_TYPE eType, int iContainer = 0, int iSubset = 0);
	void SetTexture(LINK_TYPE eType, std::shared_ptr<class CTexture> pTexture,
		int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0, int iContainer = 0, int iSubset = 0);
	void SetTexture(LINK_TYPE eType, const std::string& strTag,
		int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0, int iContainer = 0, int iSubset = 0);
	void SetTexture(LINK_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath,
		int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0, int iContainer = 0, int iSubset = 0);
	void SetTexture(LINK_TYPE eType, const std::string& strTag, const TCHAR* pFileName,
		int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0, int iContainer = 0, int iSubset = 0);
	void SetAnimation(class CAnimation* pAni);
	void ChangeSequence(const std::string& strKey);
	class CAnimation* GetAnimation()	const;
	virtual void SetInstance(bool bInst);
	void SetInstancing(class CRenderInstancing* pInst);
	class CRenderInstancing* GetInstancing()	const;
	void SetInstIndex(int iIdx);
	int GetInstIndex()	const;
	class CSceneComponent* GetOwner()	const;
	void SetOwner(class CSceneComponent* pCom);
	void ResizeMaterial(int iSize,int iContainer = 0);
	void ResizeShader(int iSize, int iContainer = 0);
	void ResizeRenderState(RENDER_STATE_TYPE eType, int iSize, int iContainer = 0);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void RenderShadow(float fTime);
	virtual void Render(float fTime);
	void RenderParticle(int iCount, float fTime);
	virtual void PostRender(float fTime);
	virtual CRenderer* Clone();

public:
	virtual void Save(FILE * pFile);
	virtual void Load(FILE * pFile);

public:
	virtual bool ShowNode();
	virtual void ShowWindow();
};

