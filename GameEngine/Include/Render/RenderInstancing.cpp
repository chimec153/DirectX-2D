#include "RenderInstancing.h"
#include "../Device.h"
#include "../CameraManager.h"
#include "../Component/Camera.h"
#include "../Component/Renderer.h"
#include "../Resource/Animation.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "../Resource/ComputeShader.h"
#include "../LightManager.h"
#include "../Component/Light.h"
#include "RasterizerState.h"

CRenderInstancing::CRenderInstancing()	:
	m_strName()
	, m_iCount(0)
	, m_pMesh(nullptr)
	, m_pInstanceBuffer(nullptr)
	, m_pBoneBuffer(nullptr)
	, m_iBoneInstancingCount(1000)
	, m_iBoneCount(0)
#ifdef _DEBUG
	, m_pDepth(nullptr)
	, m_pRasterizer(nullptr)
#endif
{

}
CRenderInstancing::~CRenderInstancing()
{
	if (m_pInstanceBuffer)
	{
		SAFE_DELETE_ARRAY(m_pInstanceBuffer->pData);
		SAFE_RELEASE(m_pInstanceBuffer->pBuffer);
		SAFE_DELETE(m_pInstanceBuffer);
	}
	SAFE_DELETE(m_pBoneBuffer);
#ifdef _DEBUG
	SAFE_RELEASE(m_pDepth);
	SAFE_RELEASE(m_pRasterizer);
#endif
}


int CRenderInstancing::GetCount()	const
{
	return static_cast<int>(m_RenderList.size());
}

void CRenderInstancing::AddComponent(class CSceneComponent* pCom)
{
	m_RenderList.push_back(pCom);
}

void CRenderInstancing::DeleteComponent(class CSceneComponent* pCom)
{
	std::list<class CSceneComponent*>::iterator iter = m_RenderList.begin();
	std::list<class CSceneComponent*>::iterator iterEnd = m_RenderList.end();

	for (; iter != iterEnd; iter++)
	{
		if (*iter == pCom)
		{
			m_RenderList.erase(iter);
			return;
		}
	}
}

class CMesh* CRenderInstancing::GetMesh() const
{
	return m_pMesh;
}

bool CRenderInstancing::Init(class CMesh* pMesh, int iBoneCount)
{
	m_iBoneCount = iBoneCount;
	m_pMesh = pMesh;

	CreateInstBuffer(1000, static_cast<int>(sizeof(_tagMatrices)));

	//m_pInstanceShader = GET_SINGLE(CShaderManager)->FindShader("Inst3D");
	m_pInstanceShader = GET_SINGLE(CShaderManager)->FindShader("InstSpecR");
	m_pInstanceShadow = GET_SINGLE(CShaderManager)->FindShader("InstShadow");
	//m_pInstanceShadow = GET_SINGLE(CShaderManager)->FindShader("PointShadowInst");

	return true;
}

void CRenderInstancing::Render(float fTime)
{
	if (m_RenderList.empty())
		return;

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
	CLight* pLight = GET_SINGLE(CLightManager)->GetMainLight();

	Matrix matView = pCam->GetView();
	Matrix matProj = pCam->GetProj();
	Matrix matShadowVP = pLight->GetVP();

	std::list<CSceneComponent*>::iterator iter = m_RenderList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_RenderList.end();

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	int iSize = m_pInstanceBuffer->iSize * m_pInstanceBuffer->iCount;

	int iCount = 0;

	CONTEXT->Map(m_pInstanceBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
#ifdef _DEBUG
	switch (static_cast<SCENECOMPONENT_CLASS_TYPE>((*iter)->GetSceneComponentClassType()))
	{
	case SCENECOMPONENT_CLASS_TYPE::COLLIDER_SPHERE:
	{
		for (; iter != iterEnd; ++iter)
		{
			if (!(*iter)->IsEnable())
			{
				continue;
			}

			DEBUGMATRIX tMatrix = {};

			Matrix matWorld = (*iter)->GetMatWorld();

			tMatrix.matWV = matWorld * matView;
			tMatrix.matWVP = tMatrix.matWV * matProj;
			tMatrix.matShadowWVP = matWorld * matShadowVP;
			tMatrix.matWorld = matWorld;

			std::shared_ptr<CMaterial> pMtrl = (*iter)->GetMaterial();
			if (pMtrl)
			{
				tMatrix.vColor = pMtrl->GetDiffuseColor();
			}

			tMatrix.matWV.Transpose();
			tMatrix.matWVP.Transpose();
			tMatrix.matShadowWVP.Transpose();
			tMatrix.matWorld.Transpose();

			int iSrcSize = (m_pInstanceBuffer->iCount - iCount) * m_pInstanceBuffer->iSize;
			int iPos = m_pInstanceBuffer->iSize * iCount;

			memcpy_s(static_cast<char*>(tSub.pData) + iPos, iSrcSize, &tMatrix, sizeof(tMatrix));
			++iCount;
		}
	}
		break;
	case SCENECOMPONENT_CLASS_TYPE::NONE:
	{
		for (; iter != iterEnd; ++iter)
		{
			if (!(*iter)->IsEnable())
			{
				continue;
			}

			_tagMatrices tMatrices = {};

			Matrix matWorld = (*iter)->GetMatWorld();

			tMatrices.matShadowWVP = matWorld * matShadowVP;
			tMatrices.matWV = matWorld * matView;
			tMatrices.matWVP = tMatrices.matWV * matProj;
			tMatrices.matWorld = matWorld;

			tMatrices.matWV.Transpose();
			tMatrices.matWVP.Transpose();
			tMatrices.matShadowWVP.Transpose();
			tMatrices.matWorld.Transpose();

			int iSrcSize = (m_pInstanceBuffer->iCount - iCount) * m_pInstanceBuffer->iSize;
			int iPos = m_pInstanceBuffer->iSize * iCount;

			memcpy_s(static_cast<char*>(tSub.pData) + iPos, iSrcSize, &tMatrices, sizeof(_tagMatrices));
			++iCount;
		}
	}
		break;
	}
#else
	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
		{
			continue;
		}

		_tagMatrices tMatrices = {};

		Matrix matWorld = (*iter)->GetMatWorld();

		tMatrices.matWV = matWorld * matView;
		tMatrices.matWVP = tMatrices.matWV * matProj;
		tMatrices.matShadowWVP = matWorld * matShadowVP;
		tMatrices.matWorld = matWorld;

		tMatrices.matWV.Transpose();
		tMatrices.matWVP.Transpose();
		tMatrices.matShadowWVP.Transpose();
		tMatrices.matWorld.Transpose();

		int iSrcSize = (m_pInstanceBuffer->iCount - iCount) * m_pInstanceBuffer->iSize;
		int iPos = m_pInstanceBuffer->iSize * iCount;

		memcpy_s(static_cast<char*>(tSub.pData) + iPos, iSrcSize, &tMatrices, sizeof(_tagMatrices));
		++iCount;
	}
#endif

	CONTEXT->Unmap(m_pInstanceBuffer->pBuffer, 0);

	iter = m_RenderList.begin();

	if (m_pMesh)
	{
		if (m_pMesh->GetMeshType() == MESH_TYPE::MT_ANIMATION_MESH)
		{
			ANIMATIONCBUFFER tCBuffer = {};

			tCBuffer.iBoneCount = m_iBoneCount;

			GET_SINGLE(CShaderManager)->UpdateCBuffer("Bone", &tCBuffer);

			CONTEXT->VSSetShaderResources(110, 1, &m_pBoneBuffer->pSRV);
		}


		m_pInstanceShader->SetShader();

		int iType = (*iter)->GetSceneComponentClassType();

		switch (static_cast<SCENECOMPONENT_CLASS_TYPE>(iType))
		{
		case SCENECOMPONENT_CLASS_TYPE::SCT_MESH2D:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_STATICMESH:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_CAMERA:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_SPRITE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_RECT:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_LINE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_CIRCLE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_OBB2D:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_PIXEL:
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_POINT:
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_RAY:
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_SPHERE:
		{
#ifdef _DEBUG
			if (m_pDepth)
			{
				m_pDepth->SetState();
			}

			if (m_pRasterizer)
			{
				m_pRasterizer->SetState();
			}
#endif
			std::shared_ptr<CMaterial> pMtrl = (*iter)->GetMaterial();

			if(pMtrl)
			pMtrl->SetMaterialNoShader();

			m_pMesh->RenderInstancing(m_pInstanceBuffer, iCount, fTime);

#ifdef _DEBUG
			if (m_pDepth)
			{
				m_pDepth->ResetState();
			}

			if (m_pRasterizer)
			{
				m_pRasterizer->ResetState();
			}
#endif
		}
		break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_TERRAIN:
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_OBB:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_BAR:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_CHECK:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_BUTTON:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_RADIO:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_PANEL:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_SLIDER:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_FONT:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_IMAGE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_SPRITE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_TITLE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_ICON:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_TAB:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_LIST:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SOUND:
			break;
		case SCENECOMPONENT_CLASS_TYPE::TILEMAP:
			break;
		case SCENECOMPONENT_CLASS_TYPE::TILE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::NONE:
		{
			int iContainer = static_cast<int>(static_cast<CSceneComponent*>(*iter)->GetMaterialSize());

			for (int i = 0; i < iContainer; i++)
			{
				int iSub = static_cast<int>(static_cast<CSceneComponent*>(*iter)->GetMaterialSubsetSize(i));

				for (int j = 0; j < iSub; j++)
				{
					CRenderer* pRenderer = static_cast<CSceneComponent*>(*iter)->GetRenderer();

					for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); ++k)
					{
						CRenderState* pState = pRenderer->FindRenderState(static_cast<RENDER_STATE_TYPE>(k), i, j);

						if(pState)
						pState->SetState();
					}

					std::shared_ptr<CMaterial> pMtrl = static_cast<CSceneComponent*>(*iter)->GetMaterial(i, j);

					pMtrl->SetMaterialNoShader();

					m_pMesh->RenderInstancing(m_pInstanceBuffer, iCount, fTime, i, j);

					for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); ++k)
					{
						CRenderState* pState = pRenderer->FindRenderState(static_cast<RENDER_STATE_TYPE>(k), i, j);

						if (pState)
						pState->ResetState();
					}
				}
			}
		}
		break;
		case SCENECOMPONENT_CLASS_TYPE::PARTICLE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::LIGHT:
			break;
		case SCENECOMPONENT_CLASS_TYPE::TERRAIN:
			break;
		case SCENECOMPONENT_CLASS_TYPE::ARM:
			break;
		case SCENECOMPONENT_CLASS_TYPE::DECAL:
			break;
		}

		static_cast<CComputeShader*>(m_pInstanceShader.get())->Clear();

		if (m_pMesh->GetMeshType() == MESH_TYPE::MT_ANIMATION_MESH)
		{
			ID3D11ShaderResourceView* pSRV = nullptr;

			CONTEXT->VSSetShaderResources(110, 1, &pSRV);
		}
	}
	m_RenderList.clear();
}

void CRenderInstancing::RenderShadow(float fTime)
{
	if (m_RenderList.empty())
		return;

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
	CLight* pLight = GET_SINGLE(CLightManager)->GetMainLight();

	Matrix matView = pCam->GetView();
	Matrix matProj = pCam->GetProj();
	Matrix matShadowVP = pLight->GetVP();

	std::list<CSceneComponent*>::iterator iter = m_RenderList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_RenderList.end();

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	int iSize = m_pInstanceBuffer->iSize * m_pInstanceBuffer->iCount;

	int iCount = 0;

	CONTEXT->Map(m_pInstanceBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
#ifdef _DEBUG
	switch (static_cast<SCENECOMPONENT_CLASS_TYPE>((*iter)->GetSceneComponentClassType()))
	{
	case SCENECOMPONENT_CLASS_TYPE::COLLIDER_SPHERE:
	{
		for (; iter != iterEnd; ++iter)
		{
			if (!(*iter)->IsShadow())
			{
				continue;
			}

			DEBUGMATRIX tMatrix = {};

			Matrix matWorld = (*iter)->GetMatWorld();

			tMatrix.matWV = matWorld * matView;
			tMatrix.matWVP = tMatrix.matWV * matProj;
			tMatrix.matShadowWVP = matWorld * matShadowVP;
			tMatrix.matWorld = matWorld;

			std::shared_ptr<CMaterial> pMtrl = (*iter)->GetMaterial();
			if (pMtrl)
			{
				tMatrix.vColor = pMtrl->GetDiffuseColor();
			}

			tMatrix.matWV.Transpose();
			tMatrix.matWVP.Transpose();
			tMatrix.matShadowWVP.Transpose();
			tMatrix.matWorld.Transpose();

			int iSrcSize = (m_pInstanceBuffer->iCount - iCount) * m_pInstanceBuffer->iSize;
			int iPos = m_pInstanceBuffer->iSize * iCount;

			memcpy_s(static_cast<char*>(tSub.pData) + iPos, iSrcSize, &tMatrix, sizeof(tMatrix));
			++iCount;
		}
	}
	break;
	case SCENECOMPONENT_CLASS_TYPE::NONE:
	{
		for (; iter != iterEnd; ++iter)
		{
			if (!(*iter)->IsShadow())
			{
				continue;
			}

			_tagMatrices tMatrices = {};

			Matrix matWorld = (*iter)->GetMatWorld();

			tMatrices.matShadowWVP = matWorld * matShadowVP;
			tMatrices.matWV = matWorld * matView;
			tMatrices.matWVP = tMatrices.matWV * matProj;
			tMatrices.matWorld = matWorld;

			tMatrices.matWV.Transpose();
			tMatrices.matWVP.Transpose();
			tMatrices.matShadowWVP.Transpose();
			tMatrices.matWorld.Transpose();

			int iSrcSize = (m_pInstanceBuffer->iCount - iCount) * m_pInstanceBuffer->iSize;
			int iPos = m_pInstanceBuffer->iSize * iCount;

			memcpy_s(static_cast<char*>(tSub.pData) + iPos, iSrcSize, &tMatrices, sizeof(_tagMatrices));
			++iCount;
		}
	}
	break;
	}
#else
	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsShadow() )
		{
			continue;
		}

		_tagMatrices tMatrices = {};

		Matrix matWorld = (*iter)->GetMatWorld();

		tMatrices.matWV = matWorld * matView;
		tMatrices.matWVP = tMatrices.matWV * matProj;
		tMatrices.matShadowWVP = matWorld * matShadowVP;
		tMatrices.matWorld = matWorld;

		tMatrices.matWV.Transpose();
		tMatrices.matWVP.Transpose();
		tMatrices.matShadowWVP.Transpose();
		tMatrices.matWorld.Transpose();

		int iSrcSize = (m_pInstanceBuffer->iCount - iCount) * m_pInstanceBuffer->iSize;
		int iPos = m_pInstanceBuffer->iSize * iCount;

		memcpy_s(static_cast<char*>(tSub.pData) + iPos, iSrcSize, &tMatrices, sizeof(_tagMatrices));
		++iCount;
	}
#endif

	CONTEXT->Unmap(m_pInstanceBuffer->pBuffer, 0);

	iter = m_RenderList.begin();

	if (m_pMesh)
	{
		if (m_pMesh->GetMeshType() == MESH_TYPE::MT_ANIMATION_MESH)
		{
			ANIMATIONCBUFFER tCBuffer = {};

			tCBuffer.iBoneCount = m_iBoneCount;

			GET_SINGLE(CShaderManager)->UpdateCBuffer("Bone", &tCBuffer);

			CONTEXT->VSSetShaderResources(110, 1, &m_pBoneBuffer->pSRV);
		}


		m_pInstanceShadow->SetShader();

		int iType = (*iter)->GetSceneComponentClassType();

		switch (static_cast<SCENECOMPONENT_CLASS_TYPE>(iType))
		{
		case SCENECOMPONENT_CLASS_TYPE::SCT_MESH2D:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_STATICMESH:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_CAMERA:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_SPRITE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_RECT:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_LINE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_CIRCLE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_OBB2D:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_PIXEL:
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_POINT:
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_RAY:
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_SPHERE:
		{
#ifdef _DEBUG
			if (m_pDepth)
			{
				m_pDepth->SetState();
			}

			if (m_pRasterizer)
			{
				m_pRasterizer->SetState();
			}
#endif
			std::shared_ptr<CMaterial> pMtrl = (*iter)->GetMaterial();

			if (pMtrl)
				pMtrl->SetMaterialNoShader();

			m_pMesh->RenderInstancing(m_pInstanceBuffer, iCount, fTime);

#ifdef _DEBUG
			if (m_pDepth)
			{
				m_pDepth->ResetState();
			}

			if (m_pRasterizer)
			{
				m_pRasterizer->ResetState();
			}
#endif
		}
		break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_TERRAIN:
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDER_OBB:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_BAR:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_CHECK:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_BUTTON:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_RADIO:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_PANEL:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_SLIDER:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_FONT:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_IMAGE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_SPRITE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_TITLE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_ICON:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_TAB:
			break;
		case SCENECOMPONENT_CLASS_TYPE::UI_LIST:
			break;
		case SCENECOMPONENT_CLASS_TYPE::SOUND:
			break;
		case SCENECOMPONENT_CLASS_TYPE::TILEMAP:
			break;
		case SCENECOMPONENT_CLASS_TYPE::TILE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::NONE:
		{
			int iContainer = static_cast<int>(static_cast<CSceneComponent*>(*iter)->GetMaterialSize());

			for (int i = 0; i < iContainer; i++)
			{
				int iSub = static_cast<int>(static_cast<CSceneComponent*>(*iter)->GetMaterialSubsetSize(i));

				for (int j = 0; j < iSub; j++)
				{
					std::shared_ptr<CMaterial> pMtrl = static_cast<CSceneComponent*>(*iter)->GetMaterial(i, j);

					pMtrl->SetMaterialNoShader();

					m_pMesh->RenderInstancing(m_pInstanceBuffer, iCount, fTime, i, j);
				}
			}
		}
		break;
		case SCENECOMPONENT_CLASS_TYPE::PARTICLE:
			break;
		case SCENECOMPONENT_CLASS_TYPE::LIGHT:
			break;
		case SCENECOMPONENT_CLASS_TYPE::TERRAIN:
			break;
		case SCENECOMPONENT_CLASS_TYPE::ARM:
			break;
		case SCENECOMPONENT_CLASS_TYPE::DECAL:
			break;
		}

		static_cast<CComputeShader*>(m_pInstanceShader.get())->Clear();

		if (m_pMesh->GetMeshType() == MESH_TYPE::MT_ANIMATION_MESH)
		{
			ID3D11ShaderResourceView* pSRV = nullptr;

			CONTEXT->VSSetShaderResources(110, 1, &pSRV);
		}
	}
}

void CRenderInstancing::SetInstancingCount(int iCount)
{
	if (m_iBoneInstancingCount < iCount)
	{
		m_iBoneInstancingCount = static_cast<int>(iCount * 1.5f);

		CreateBoneBuffer();
	}
	else
	{
		m_iBoneInstancingCount = iCount;
	}
}

PSTRUCTUREDBUFFER CRenderInstancing::GetBoneBuffer() const
{
	return m_pBoneBuffer;
}

bool CRenderInstancing::CreateBoneBuffer()
{
	SAFE_DELETE(m_pBoneBuffer);

	m_pBoneBuffer = new STRUCTUREDBUFFER;

	m_pBoneBuffer->iCount = m_iBoneInstancingCount * m_iBoneCount;
	m_pBoneBuffer->iSize = static_cast<int>(sizeof(Matrix));
	m_pBoneBuffer->iShaderType = static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE);
	m_pBoneBuffer->tDesc.ByteWidth = m_pBoneBuffer->iCount * m_pBoneBuffer->iSize;
	m_pBoneBuffer->tDesc.Usage = D3D11_USAGE_DEFAULT;
	m_pBoneBuffer->tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	m_pBoneBuffer->tDesc.StructureByteStride = m_pBoneBuffer->iSize;
	m_pBoneBuffer->tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	if (FAILED(DEVICE->CreateBuffer(&m_pBoneBuffer->tDesc, nullptr, &m_pBoneBuffer->pBuffer)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC tSrv = {};

	tSrv.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSrv.Format = DXGI_FORMAT_UNKNOWN;
	tSrv.BufferEx.FirstElement = 0;
	tSrv.BufferEx.Flags = 0;
	tSrv.BufferEx.NumElements = m_pBoneBuffer->iCount;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pBoneBuffer->pBuffer, &tSrv, &m_pBoneBuffer->pSRV)))
		return false;

	D3D11_UNORDERED_ACCESS_VIEW_DESC tUav = {};

	tUav.Format = DXGI_FORMAT_UNKNOWN;
	tUav.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	tUav.Buffer.FirstElement = 0;
	tUav.Buffer.Flags = 0;
	tUav.Buffer.NumElements = m_pBoneBuffer->iCount;

	if (FAILED(DEVICE->CreateUnorderedAccessView(m_pBoneBuffer->pBuffer, &tUav, &m_pBoneBuffer->pUAV)))
		return false;

	return true;
}
void CRenderInstancing::SetShader(const std::string& strKey)
{
	m_pInstanceShader = GET_SINGLE(CShaderManager)->FindShader(strKey);
}
bool CRenderInstancing::CreateInstBuffer(int iCount ,int iSize)
{
	if (m_pInstanceBuffer)
	{
		SAFE_RELEASE(m_pInstanceBuffer->pBuffer);
		SAFE_DELETE_ARRAY(m_pInstanceBuffer->pData);
	}

	SAFE_DELETE(m_pInstanceBuffer);

	m_pInstanceBuffer = new InstancingBuffer;

	m_pInstanceBuffer->iCount = iCount;
	m_pInstanceBuffer->iSize = iSize;
	m_pInstanceBuffer->eUsg = D3D11_USAGE_DYNAMIC;
	int _iSize = m_pInstanceBuffer->iCount * m_pInstanceBuffer->iSize;
	m_pInstanceBuffer->pData = new char[_iSize];
	memset(m_pInstanceBuffer->pData, 0, _iSize);

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.ByteWidth = _iSize;
	tDesc.Usage = m_pInstanceBuffer->eUsg;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &m_pInstanceBuffer->pBuffer)))
		return false;

	return true;
}
#ifdef _DEBUG
void CRenderInstancing::SetDepthState(CRenderState* pState)
{
	m_pDepth = pState;

	if (m_pDepth)
		m_pDepth->AddRef();
}

void CRenderInstancing::SetRasterizer(CRenderState* pState)
{
	m_pRasterizer = pState;

	if (m_pRasterizer)
		m_pRasterizer->AddRef();
}
#endif

int CRenderInstancing::GetInstancingCount() const
{
	return m_iBoneInstancingCount;
};