#include "Ocean.h"
#include "Component/Renderer.h"
#include "Resource/ResourceManager.h"
#include "Resource/Material.h"
#include "Component/Terrain.h"
#include "Component/Transform.h"

COcean::COcean()
{
}

COcean::COcean(const COcean& obj)	:
	CGameObject(obj)
{
}

COcean::~COcean()
{
}

void COcean::BuildWater(const std::string& strKey, int iWidth, int iHeight)
{
	std::vector<_tagVertexOcean>	vecVtx(iWidth * iHeight);

	m_vecPos.resize(iWidth * iHeight);

	for (int i = 0; i < iWidth; i++)
	{
		for (int j = 0; j < iHeight; j++)
		{
			_tagVertexOcean tVtx = {};

			tVtx.vPos = Vector3(static_cast<float>(i), 0.f, static_cast<float>(j));
			tVtx.vUV = Vector2(i/static_cast<float>(iWidth-1), j/static_cast<float>(iHeight-1));
			tVtx.vNormal = Vector3(0.f, 1.f, 0.f);
			tVtx.vTangent = Vector3(1.f, 0.f, 0.f);
			tVtx.vBinormal = Vector3(0.f, 0.f, -1.f);

			int iIdx = j * iWidth + i;
			vecVtx[iIdx] = tVtx;

			m_vecPos[iIdx] = Vector3(static_cast<float>(i), 0.f, static_cast<float>(j));
		}
	}

	int iCells = (iWidth - 1) * (iHeight - 1);
	std::vector<unsigned int> vecIdx(iCells * 6);
	for(int i=0;i<iCells;i++)
	{
		int x = i % (iWidth - 1);
		int y = i / (iWidth - 1);

		vecIdx[i * 6] = x + y * iWidth;
		vecIdx[i * 6 + 1] = x + (y + 1) * iWidth;
		vecIdx[i * 6 + 2] = x + y * iWidth + 1;
		vecIdx[i * 6 + 3] = x + y * iWidth + 1;
		vecIdx[i * 6 + 4] = x + (y + 1) * iWidth;
		vecIdx[i * 6 + 5] = x + (y+1) * iWidth + 1;
	}

	GET_SINGLE(CResourceManager)->CreateMesh(strKey, MESH_TYPE::MT_STATIC_MESH, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		iWidth * iHeight, static_cast<int>(sizeof(_tagVertexOcean)), D3D11_USAGE_IMMUTABLE, &vecVtx[0],
		iCells * 6, 4, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT, &vecIdx[0]);

}

bool COcean::Init()
{
	if(!CGameObject::Init())
		return false;

	m_pRenderer = CreateComponent<CSceneComponent>("Renderer");

	static_cast<CSceneComponent*>(m_pRenderer.get())->SetPivot(0.5f, 0.f, 0.5f);

	std::shared_ptr<CComponent> pOcean = CreateComponent<CTerrain>("Ocean");

	SetRootComponent(pOcean);
	static_cast<CTerrain*>(pOcean.get())->AddChild(m_pRenderer);

	static_cast<CTerrain*>(pOcean.get())->SetPivot(0.5f, 0.f, 0.5f);
	static_cast<CTerrain*>(pOcean.get())->SetWorldScale(1000.f, 15.f, 1000.f);
	static_cast<CTerrain*>(pOcean.get())->SetWorldPos(0.f, 1000.f, 0.f);
	static_cast<CTerrain*>(pOcean.get())->BuildHeightMap("Ocean", 257, 257, 0.f);
	static_cast<CTerrain*>(pOcean.get())->SetTiling(100.f, 100.f);
	static_cast<CTerrain*>(pOcean.get())->SetMesh("Ocean");
	static_cast<CTerrain*>(pOcean.get())->SetMaterial("Color");
	static_cast<CTerrain*>(pOcean.get())->SetShader("Ocean");
	static_cast<CTerrain*>(pOcean.get())->AddRenderState(ALPHA_BLEND);

	std::shared_ptr<CMaterial> pMtrl = static_cast<CSceneComponent*>(pOcean.get())->GetMaterial();

	GET_SINGLE(CResourceManager)->LoadTexture("FX_waterTile_001_TEx_PHS",
		L"FX_Resource_PHS_02\\Texture2D\\FX_waterTile_001_TEx_PHS.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_Flowmap_001_tex_PHS",
		L"FX_Resource_PHS_02\\Texture2D\\FX_Flowmap_001_tex_PHS.dds");

	pMtrl->SetTexture(LINK_TYPE::LT_DIF, "FX_waterTile_001_TEx_PHS");
	pMtrl->SetTexture(LINK_TYPE::LT_NOR, "FX_Flowmap_001_tex_PHS");
	pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 200.f / 255.f);

	return true;
}

void COcean::Start()
{
	CGameObject::Start();
}

void COcean::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void COcean::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void COcean::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void COcean::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void COcean::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void COcean::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> COcean::Clone()
{
	return std::shared_ptr<CGameObject>(new COcean(*this));
}

void COcean::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void COcean::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
