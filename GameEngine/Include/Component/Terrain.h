#pragma once
#include "SceneComponent.h"
#include "../Object/GameObject.h"

typedef struct _tagVertexTerrain
{
	Vector3 vPos;
	Vector3 vNormal;
	Vector2 vUV;
	Vector3 vTangent;
	Vector3 vBinormal;
	Vector4 vWeight;
	Vector4 vWeightFactor;
	Vector2 vBoundsY;
}VERTEXTERRAIN, * PVERTEXTERRAIN;

class CTerrain :
	public CSceneComponent
{
public:
	CTerrain();
	CTerrain(const CTerrain& terrain);
	virtual ~CTerrain() override;

private:
	int						m_iWidth;
	int						m_iHeight;
	float*					m_pHeightMap;
	std::vector<float>		m_vecHeight;
	std::vector<Vector3>	m_vecPos;
	TERRAINCBUFFER			m_tCBuffer;
	std::vector<VERTEX3D>	m_vecVtx;
	std::vector<Vector3>	m_vecFaceNormal;

	ID3D11ShaderResourceView* m_pHeightMapSRV;

	UINT m_iNumPatchVertRows;
	UINT m_iNumPatchVertCols;

	UINT m_iNumPatchVertices;
	UINT m_iNumPatchQuadFaces;

	float m_fCellSpacing;

	ID3D11Buffer* m_pQuadPatchIB;

	ScratchImage* m_pImage;
	ID3D11Texture2D* m_pTexture;
	std::shared_ptr<class CTexture> m_pBrushTexture;
	
	std::shared_ptr<class CGameObject> m_pDecal;
	bool			m_bEditMode;
	bool			m_bBlurMode;
	Vector3			m_vBrushScale;

private:
	std::vector<Vector2>	m_vecPatchBoundsY;

public:
	bool BuildHeightMap(const std::string& strMesh, int iWidth, int iHeight, float fMaxHeight,
		const TCHAR* pHeightMapPath = nullptr, const std::string& strPathKey = TEXTURE_PATH);
	void SaveHeightMap(const TCHAR* pFilePath, const std::string& strPathKey = TEXTURE_PATH);
	void SetBrushTexture(const std::string& strKey, const TCHAR* pFilePath, 
		const std::string& strPathKey = TEXTURE_PATH);

	int GetIndex(const Vector3& vPos);
	float GetHeight(const Vector3& vPos);
	void SetTiling(float fX, float fY);
	const std::vector<Vector3>& GetVecPos()	const;
	void SetCollider(const std::shared_ptr<class CComponent>& pCol);
	void SetSplatCount(int iCount);
	const Vector3 GetNormal(const Vector3& vPos)	const;
	const Vector3 GetTangent(const Vector3& vPos)	const;
	const Vector3 GetBinormal(const Vector3& vPos)	const;
	float GetSlope(const Vector3& vDir, const Vector3& vPos)	const;
	const Vector3 GetTangent(const Vector3& vDir, const Vector3& vPos)	const;

	float GetHeightMapWidth()	const;
	float GetHeightMapDepth()	const;

	void BuildQuadPatch();
	void BuildQuadPatchIB();

	void SetEditorMode(bool bEdit);
	void SetBlurMode(bool bBlur);

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float fTime) override;
	virtual void PostUpdate(float fTime) override;
	virtual void Collision(float fTime) override;
	virtual void PreRender(float fTime) override;
	virtual void Render(float fTime) override;
	virtual void PostRender(float fTime) override;
public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;

private:
	void ComputeNormal();
	void ComputeTangent();

public:
	void ColInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void ColStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void ColLast(class CCollider* pSrc, class CCollider* pDest, float fTime);

private:
	static std::unordered_map< CTerrain*, std::string>	m_mapTerrain;
	static CTerrain* m_pMainTerrain;
	static std::unordered_map<std::string, std::shared_ptr<class CTexture>>	m_mapBrush;

public:
	static CTerrain* FindTerrain(const std::string& strKey);
	static void DeleteTerrain(CTerrain* pTerrain);
	static CTerrain* GetMainTerrain();
	static void SetMainTerrain(CTerrain* pTerrain);
	static void SetMainTerrain(const std::string& strKey);
	static void AddTerrain(CTerrain* pTerrain);
	static std::shared_ptr<class CTexture> FindBrush(const std::string& strKey);

public:
	virtual void ShowWindow();

public:
	void ExtractFrustumPlanes(Vector4 planes[6], Matrix& m);
	void CalcAllPatchBoundsY();
	void CalcPatchBoundsY(UINT i, UINT j);

};

