#include "LayerInstance.h"
#include "Resource/Mesh.h"
#include "Resource/Material.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "UI/UFont.h"

CLayerInstance::CLayerInstance() :
	m_pMesh(nullptr),
	m_pMaterial(nullptr),
	m_pInstancingBuffer(nullptr),
	m_iCount(0),
	m_pFont(nullptr),
	m_pFontObj(nullptr)
{
}

CLayerInstance::~CLayerInstance()
{
	if (m_pInstancingBuffer)
	{
		SAFE_RELEASE(m_pInstancingBuffer->pBuffer);
		SAFE_DELETE_ARRAY(m_pInstancingBuffer->pData);
		delete m_pInstancingBuffer;
	}		
}

void CLayerInstance::SetLayer(CLayer* pLayer)
{
	m_pLayer = pLayer;
}

void CLayerInstance::AddCom(CSceneComponent* pCom)
{
	m_vecCom.push_back(pCom);
}

const std::vector<class CSceneComponent*>* CLayerInstance::GetComponents() const
{
	return &m_vecCom;
}

size_t CLayerInstance::GetComSize() const
{
	return m_vecCom.size();
}

bool CLayerInstance::Init(class CMesh* pMesh, std::shared_ptr<CMaterial> pMtrl, int iSize, int iCount)
{
	m_pMesh = pMesh;

	m_pMaterial = pMtrl;

	m_pInstancingBuffer = new InstancingBuffer;

	m_pInstancingBuffer->eUsg = D3D11_USAGE_DYNAMIC;
	m_pInstancingBuffer->iSize = iSize;
	m_pInstancingBuffer->iCount = iCount;
	m_pInstancingBuffer->pData = new char[m_pInstancingBuffer->iSize * m_pInstancingBuffer->iCount];

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.ByteWidth = m_pInstancingBuffer->iSize * m_pInstancingBuffer->iCount;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &m_pInstancingBuffer->pBuffer)))
		return false;

	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	CLayer* pLayer = pScene->FindLayer(UI_LAYER);

	m_pFontObj = pScene->CreateObject<CGameObject>("Count", pLayer);

	m_pFont = m_pFontObj->CreateComponent<CUFont>("Font");

	((CUFont*)m_pFont.get())->SetFormat("Normal");
	((CUFont*)m_pFont.get())->SetRect(0.f, 200.f, 200.f, 0.f);

	return true;
}

void CLayerInstance::AddInstancingData(const void* pData)
{
	if (m_iCount == m_pInstancingBuffer->iCount)
	{
		m_pInstancingBuffer->iCount *= 2;

		m_pInstancingBuffer->pBuffer->Release();

		char* _pData = new char[m_pInstancingBuffer->iCount];

		memcpy(_pData, m_pInstancingBuffer->pData, m_pInstancingBuffer->iSize * m_iCount);

		delete[] m_pInstancingBuffer->pData;

		m_pInstancingBuffer->pData = _pData;

		D3D11_BUFFER_DESC tDesc = {};

		tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		tDesc.ByteWidth = m_pInstancingBuffer->iSize * m_pInstancingBuffer->iCount;
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.Usage = D3D11_USAGE_DYNAMIC;

		if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &m_pInstancingBuffer->pBuffer)))
			return;
	}

	memcpy(((char*)m_pInstancingBuffer->pData + m_iCount++ * m_pInstancingBuffer->iSize), pData,
		m_pInstancingBuffer->iSize);
}

void CLayerInstance::Render()
{
	D3D11_MAPPED_SUBRESOURCE tMap = {};

	CONTEXT->Map(m_pInstancingBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	memcpy(tMap.pData, m_pInstancingBuffer->pData, m_pInstancingBuffer->iSize * m_iCount);

	CONTEXT->Unmap(m_pInstancingBuffer->pBuffer, 0);

	m_pMaterial->Render(0.f);

	m_pMesh->RenderInstancing(m_pInstancingBuffer, m_iCount, m_pInstancingBuffer->iSize, 0.f);
}

void CLayerInstance::Clear()
{
	m_iCount = 0;
	m_vecCom.clear();
}

void CLayerInstance::RenderFont(int idx)
{
	((CUFont*)m_pFont.get())->SetRect(0.f, 200.f + idx * 100.f, 250.f, 0.f);

	TCHAR strCount[32] = {};

	 char* strMtrl = (char*)m_pMaterial->GetName().c_str();

	TCHAR wstrMtrl[32] = {};

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, strMtrl, -1, wstrMtrl, (int)strlen(strMtrl));
#else
	strcpy(wstrMtrl, strMtrl);
#endif

	swprintf_s(strCount, TEXT("%s Count: %d"), wstrMtrl, m_iCount);

	((CUFont*)m_pFont.get())->SetText(strCount);

	m_pFont->Render(0.f);
}
