#include "Animation.h"
#include "ResourceManager.h"
#include "AnimationSequence.h"
#include "../Device.h"
#include "Skeleton.h"
#include "ShaderManager.h"
#include "BoneSocket.h"
#include "../Object/GameObject.h"
#include "ComputeShader.h"
#include "../PathManager.h"
#include "../Component/SceneComponent.h"
#include "../Component/Transform.h"
#include "../Component/Renderer.h"
#include "../Render/RenderInstancing.h"

CAnimation::CAnimation()	:
	m_pSequence(nullptr)
	, m_pSkeleton(nullptr)
	, m_pBuffer(nullptr)
	, m_pSrv(nullptr)
	, m_pUav(nullptr)
	, m_pCopyBuffer(nullptr)
	, m_pTransition(nullptr)
	, m_fAnimationTime(0.f)
	, m_bEnd(false)
	, m_fTimeScale(1.f)
	, m_tCBuffer()
	, m_fTransitionTime(0.f)
	, m_fTransitionTimeLimit(0.1f)
	, m_pOwner(nullptr)
	, m_bStop(false)
	, m_bStart(false)
	, m_bUpdate(false)
	, m_pBoneBuffer(nullptr)
	, m_pBoneSRV(nullptr)
	, m_pBoneUAV(nullptr)
	, m_pBlendSequence(nullptr)
	, m_fBlendTime(0.f)
{
}

CAnimation::CAnimation(const CAnimation& ani)	:
	CRef(ani)
	, m_pSkeleton(ani.m_pSkeleton)
	, m_pBuffer(nullptr)
	, m_pSrv(nullptr)
	, m_pUav(nullptr)
	, m_pCopyBuffer(nullptr)
	, m_pTransition(ani.m_pTransition)
	, m_fAnimationTime(ani.m_fAnimationTime)
	, m_bEnd(ani.m_bEnd)
	, m_fTimeScale(ani.m_fTimeScale)
	, m_tCBuffer(ani.m_tCBuffer)
	, m_fTransitionTime(ani.m_fTransitionTime)
	, m_fTransitionTimeLimit(ani.m_fTransitionTimeLimit)
	, m_pOwner(ani.m_pOwner)
	, m_bStop(ani.m_bStop)
	, m_bStart(false)
	, m_bUpdate(ani.m_bUpdate)
	, m_pBoneBuffer(nullptr)
	, m_pBoneSRV(nullptr)
	, m_pBoneUAV(nullptr)
	, m_pBlendSequence(ani.m_pBlendSequence)
	, m_fBlendTime(ani.m_fBlendTime)
{
	if (m_pSkeleton)
	{
		m_pSkeleton = m_pSkeleton->Clone();
	}

	if (m_pTransition)
	{
		m_pTransition = new ANIMATIONTRANSITION;

		*m_pTransition = *ani.m_pTransition;

		if (m_pTransition->pNextSequence)
		{
			m_pTransition->pNextSequence->AddRef();
		}
	}

	size_t iSize = ani.m_mapSequence.size();

	std::unordered_map<size_t, CAnimationSequence*>::const_iterator iter = ani.m_mapSequence.begin();
	std::unordered_map<size_t, CAnimationSequence*>::const_iterator iterEnd = ani.m_mapSequence.end();

	for (; iter != iterEnd; ++iter)
	{
		CAnimationSequence* pSequence = iter->second->Clone();

		m_mapSequence.insert(std::make_pair(iter->first, pSequence));

		if (ani.m_pSequence->GetName() == pSequence->GetName())
			m_pSequence = pSequence;
	}

	std::list<CBoneSocket*>::const_iterator iterS = ani.m_SocketList.begin();
	std::list<CBoneSocket*>::const_iterator iterSEnd = ani.m_SocketList.end();

	for (; iterS != iterSEnd; ++iterS)
	{
		CBoneSocket* pSocket = new CBoneSocket;

		pSocket->SetName((*iterS)->GetName());
		pSocket->m_iBoneIndex = (*iterS)->m_iBoneIndex;
		pSocket->m_strBone = (*iterS)->m_strBone;
		pSocket->m_vOffset = (*iterS)->m_vOffset;
		pSocket->m_vRotation = (*iterS)->m_vRotation;

		std::list<std::shared_ptr<CComponent>>::iterator iterC = (*iterS)->m_ComponentList.begin();
		std::list<std::shared_ptr<CComponent>>::iterator iterCEnd = (*iterS)->m_ComponentList.end();

		for (; iterC != iterCEnd; ++iterC)
		{
			pSocket->m_ComponentList.push_back(*iterC);
		}

		m_SocketList.push_back(pSocket);
	}

	std::list<PANIMATIONNOTIFY>::const_iterator iterN = ani.m_NotifyList.begin();
	std::list<PANIMATIONNOTIFY>::const_iterator iterNEnd = ani.m_NotifyList.end();

	for (; iterN != iterNEnd; ++iterN)
	{
		PANIMATIONNOTIFY pNotify = new ANIMATIONNOTIFY;

		*pNotify = *(*iterN);

		std::unordered_map<size_t, CAnimationSequence*>::iterator iterS = m_mapSequence.begin();
		std::unordered_map<size_t, CAnimationSequence*>::iterator iterSEnd = m_mapSequence.end();

		for (; iterS != iterSEnd; ++iterS)
		{
			std::string strName =pNotify->pSequence->GetName();

			if (strName == iterS->second->GetName())
			{
				pNotify->pSequence = iterS->second;
				break;
			}
		}

		m_NotifyList.push_back(pNotify);
	}

	m_pUpdateShader = GET_SINGLE(CShaderManager)->FindShader("Anim");
}

CAnimation::~CAnimation()
{
	SAFE_RELEASE_MAP(m_mapSequence);
	SAFE_RELEASE_VECLIST(m_SocketList);
	SAFE_DELETE_VECLIST(m_NotifyList);
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pSrv);
	SAFE_RELEASE(m_pUav);
	SAFE_RELEASE(m_pCopyBuffer);
	SAFE_RELEASE(m_pSkeleton);
	SAFE_RELEASE(m_pBoneBuffer);
	SAFE_RELEASE(m_pBoneSRV);
	SAFE_RELEASE(m_pBoneUAV);
}

void CAnimation::SetOwner(CSceneComponent* pCom)
{
	m_pOwner = pCom;
}

bool CAnimation::HasOwner() const
{
	return m_pOwner;
}

bool CAnimation::IsStart() const
{
	return m_bStart;
}

bool CAnimation::IsEnd() const
{
	return m_bEnd;
}

CSkeleton* CAnimation::GetSkeleton() const
{
	if (m_pSkeleton)
		m_pSkeleton->AddRef();

	return m_pSkeleton;
}

void CAnimation::SetTimeScale(float fScale)
{
	m_fTimeScale = fScale;
}

bool CAnimation::AddAnimationSequence(const std::string& strKey, const std::string& strSeq)
{
	CAnimationSequence* pSeq = FindSequence(strKey);

	if (pSeq)
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	CAnimationSequence* pProto = GET_SINGLE(CResourceManager)->FindSequence(strSeq);

	if (!pProto)
		return false;

	pSeq = pProto->Clone();

	SAFE_RELEASE(pProto);

	std::hash<std::string> hs;

	size_t iKey = hs(strKey);

	m_mapSequence.insert(std::make_pair(iKey, pSeq));

	if (!m_pSequence)
	{
		m_pSequence = pSeq;
		m_fAnimationTime = m_pSequence->m_fStartTime;
	}

	return true;
}

void CAnimation::ChangeSequence(const std::string& strKey)
{
	if (!m_pTransition)
	{
		CAnimationSequence* pSequence = FindSequence(strKey);

		if (m_pSequence == pSequence)
		{
			SAFE_RELEASE(pSequence);
			return;
		}

		std::list<_tagAnimationTransition*>::iterator iter = m_pSequence->m_TransitionList.begin();
		std::list<_tagAnimationTransition*>::iterator iterEnd = m_pSequence->m_TransitionList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->pNextSequence == pSequence)
			{
				m_pTransition = *iter;

				if (m_pTransition)
				{
					std::list<std::function<void()>>::iterator iterF = m_pTransition->NotifyFuncList.begin();
					std::list<std::function<void()>>::iterator iterFEnd = m_pTransition->NotifyFuncList.end();

					for (; iterF != iterFEnd; ++iterF)
					{
						(*iterF)();
					}
				}

				SAFE_RELEASE(pSequence);
				return;
			}
		}

		std::unordered_map<size_t, CAnimationSequence*>::iterator iterS = m_mapSequence.begin();
		std::unordered_map<size_t, CAnimationSequence*>::iterator iterSEnd = m_mapSequence.end();

		for (; iterS != iterSEnd; ++iterS)
		{
			if (m_pSequence == iterS->second)
				continue;

			std::list<_tagAnimationTransition*>::iterator iter = iterS->second->m_TransitionList.begin();
			std::list<_tagAnimationTransition*>::iterator iterEnd = iterS->second->m_TransitionList.end();

			for (; iter != iterEnd; ++iter)
			{
				if ((*iter)->pNextSequence == pSequence)
				{
					m_pTransition = *iter;

					if (m_pTransition)
					{
						std::list<std::function<void()>>::iterator iterF = m_pTransition->NotifyFuncList.begin();
						std::list<std::function<void()>>::iterator iterFEnd = m_pTransition->NotifyFuncList.end();

						for (; iterF != iterFEnd; ++iterF)
						{
							(*iterF)();
						}
					}

					break;
				}
			}

			if (m_pTransition)
				break;
		}		

		SAFE_RELEASE(pSequence);
	}
}

void CAnimation::SetPlayTime(const std::string& strKey, float fTime)
{
	CAnimationSequence* pSequence = FindSequence(strKey);

	if (pSequence)
	{
		pSequence;
		SAFE_RELEASE(pSequence);
	}
}

class CAnimationSequence* CAnimation::GetSequence() const
{
	return m_pSequence;
}

void CAnimation::SetBlendSequence(const std::string& strKey)
{
	if (m_pBlendSequence)
		return;

	CAnimationSequence* pSequence = FindSequence(strKey);

	if (!pSequence)
		return;

	m_pBlendSequence = pSequence;

	SAFE_RELEASE(pSequence);

	m_tCBuffer.fBlendRate = 0.f;
	if (m_pBlendSequence)
	{
		m_tCBuffer.iBlendFrameCount = m_pBlendSequence->m_iFrameCount;
		m_fBlendTime = m_pBlendSequence->m_fStartTime;

		std::unordered_map<size_t, CAnimationSequence*>::iterator iter = m_mapSequence.begin();
		std::unordered_map<size_t, CAnimationSequence*>::iterator iterEnd = m_mapSequence.end();

		for (; iter != iterEnd; ++iter)
		{
			std::list<_tagAnimationTransition*>::iterator iterT = iter->second->m_TransitionList.begin();
			std::list<_tagAnimationTransition*>::iterator iterTEnd = iter->second->m_TransitionList.end();

			for (; iterT != iterTEnd; ++iterT)
			{
				if ((*iterT)->pNextSequence == m_pBlendSequence)
				{
					std::list<std::function<void()>>::iterator iterF = (*iterT)->NotifyFuncList.begin();
					std::list<std::function<void()>>::iterator iterFEnd = (*iterT)->NotifyFuncList.end();

					for (; iterF != iterFEnd; ++iterF)
					{
						(*iterF)();
					}
				}
			}
		}
	}
}

void CAnimation::DeleteBlendSequence()
{
	m_pBlendSequence->EndCallBack();
	m_pBlendSequence = nullptr;
	m_tCBuffer.fBlendRate = 0.f;
}

CAnimationSequence* CAnimation::GetBlendSequence() const
{
	return m_pBlendSequence;
}

CAnimationSequence* CAnimation::FindSequence(const std::string& strKey)
{
	std::hash<std::string> hs;

	size_t iKey = hs(strKey);

	std::unordered_map<size_t, CAnimationSequence*>::iterator iter = m_mapSequence.find(iKey);

	if (iter == m_mapSequence.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CAnimation::Init()
{
	m_pUpdateShader = GET_SINGLE(CShaderManager)->FindShader("Anim");

	return true;
}

void CAnimation::Start()
{
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pSrv);
	SAFE_RELEASE(m_pUav);
	SAFE_RELEASE(m_pBoneBuffer);
	SAFE_RELEASE(m_pBoneSRV);
	SAFE_RELEASE(m_pBoneUAV);
	SAFE_RELEASE(m_pCopyBuffer);

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	tDesc.ByteWidth = m_pSkeleton->GetBoneCount() * static_cast<int>(sizeof(Matrix));
	tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	tDesc.StructureByteStride = static_cast<int>(sizeof(Matrix));
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &m_pBuffer)))
		return;

	D3D11_SHADER_RESOURCE_VIEW_DESC tSrv = {};
	tSrv.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSrv.BufferEx.FirstElement = 0;
	tSrv.BufferEx.Flags = 0;
	tSrv.BufferEx.NumElements = m_pSkeleton->GetBoneCount();
	tSrv.Format = DXGI_FORMAT_UNKNOWN;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pBuffer, &tSrv, &m_pSrv)))
		return;

	D3D11_UNORDERED_ACCESS_VIEW_DESC tUav = {};

	tUav.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	tUav.Buffer.FirstElement = 0;
	tUav.Buffer.Flags = 0;
	tUav.Buffer.NumElements = m_pSkeleton->GetBoneCount();
	tUav.Format = DXGI_FORMAT_UNKNOWN;

	if (FAILED(DEVICE->CreateUnorderedAccessView(m_pBuffer, &tUav, &m_pUav)))
		return;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &m_pBoneBuffer)))
		return;
	if (FAILED(DEVICE->CreateShaderResourceView(m_pBoneBuffer, &tSrv, &m_pBoneSRV)))
		return;
	if (FAILED(DEVICE->CreateUnorderedAccessView(m_pBoneBuffer, &tUav, &m_pBoneUAV)))
		return;

	D3D11_BUFFER_DESC tCopyDesc = {};

	tCopyDesc.ByteWidth = static_cast<int>(sizeof(Matrix)) * m_pSkeleton->GetBoneCount();
	tCopyDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	tCopyDesc.Usage = D3D11_USAGE_STAGING;

	if (FAILED(DEVICE->CreateBuffer(&tCopyDesc, nullptr, &m_pCopyBuffer)))
		return;

	m_bStart = true;
}

void CAnimation::Update(float fTime)
{
	if (m_bStop)
		return;

	if (m_bUpdate)
		return;

	if (!m_pSequence)
		return;

	if (m_pTransition)
	{
		m_fTransitionTime += fTime * m_fTimeScale;

		if (m_fTransitionTime >= m_fTransitionTimeLimit)
		{
			m_fTransitionTime = 0.f;

			std::list<PANIMATIONNOTIFY>::iterator iter = m_NotifyList.begin();
			std::list<PANIMATIONNOTIFY>::iterator iterEnd = m_NotifyList.end();

			for (; iter != iterEnd; ++iter)
			{
				(*iter)->bCall = false;
			}
			m_pSequence = m_pTransition->pNextSequence;
			m_bEnd = false;
			m_tCBuffer.iTransition = false;
			m_pTransition = nullptr;
			m_fAnimationTime = m_pSequence->m_fStartTime;
		}
		else
		{
			m_tCBuffer.fTransitionRatio = m_fTransitionTime / m_fTransitionTimeLimit;
			m_tCBuffer.iTransition = true;
			m_tCBuffer.iNextFrameCount = m_pTransition->pNextSequence->m_iFrameCount;
			m_pTransition->pNextSequence->SetShaderResource(15);
		}
	}

	else
	{
		m_fAnimationTime += fTime * m_fTimeScale;

		if (m_fAnimationTime >= m_pSequence->m_fEndTime)
		{
			m_pSequence->EndCallBack();
			m_bEnd = true;
		}
		else if(m_fAnimationTime < m_pSequence->m_fStartTime)
		{
			m_fAnimationTime = m_pSequence->m_fStartTime;
		}
	}

	if (m_pBlendSequence)
	{
		m_fBlendTime += fTime * m_fTimeScale;

		if (m_fBlendTime >= m_pBlendSequence->m_fEndTime)
		{
			DeleteBlendSequence();
		}
		else 
		{
			if (m_fBlendTime < m_pBlendSequence->m_fStartTime)
			{
				m_fBlendTime = m_pBlendSequence->m_fStartTime;
			}

			m_pBlendSequence->SetShaderResource(17);
			m_pBlendSequence->SetBlendMaskSRV(16);
			
			float fFrameTime = m_pBlendSequence->m_fTimeLength/(m_pBlendSequence->m_iFrameCount - 1);

			m_tCBuffer.iBlendFrame = static_cast<int>((m_fBlendTime - m_pBlendSequence->m_fStartTime) / fFrameTime);
			m_tCBuffer.fBlendRatio = (m_fBlendTime - m_pBlendSequence->m_fStartTime) / fFrameTime - m_tCBuffer.iBlendFrame;

			if (m_tCBuffer.iBlendFrame < 3)
			{
				m_tCBuffer.fBlendRate = (m_tCBuffer.fBlendRatio + m_tCBuffer.iBlendFrame) /3.f;
			}
			else if (m_tCBuffer.iBlendFrame + 4 >= m_pBlendSequence->m_iFrameCount)
			{
				m_tCBuffer.fBlendRate = (-m_tCBuffer.fBlendRatio + m_pBlendSequence->m_iFrameCount - m_tCBuffer.iBlendFrame - 1)/3.f;
			}
			else
			{
				m_tCBuffer.fBlendRate = 1.f;
			}
		}

		
	}

	float fFrameTime = m_pSequence->m_fTimeLength / (m_pSequence->m_iFrameCount-1);
	int iFrame = static_cast<int>((m_fAnimationTime - m_pSequence->m_fStartTime) / fFrameTime);
	UINT iCount = -1;

	if (m_pOwner->IsInstance() && m_pOwner->IsFrustumIn())
	{
		CRenderInstancing* pInst = m_pOwner->GetInstancing();

		PSTRUCTUREDBUFFER pBuffer = nullptr;
		if (pInst)
		{
			pBuffer = pInst->GetBoneBuffer();
			CONTEXT->CSSetUnorderedAccessViews(0, 1, &pBuffer->pUAV, &iCount);
		}
		m_tCBuffer.iInstanceIndex = m_pOwner->GetInstIndex();
	}
	else
	{
		m_tCBuffer.iInstanceIndex = 0;
		CONTEXT->CSSetUnorderedAccessViews(0, 1, &m_pUav, &iCount);
	}

	m_tCBuffer.fRatio = (m_fAnimationTime - m_pSequence->m_fStartTime - iFrame * fFrameTime) / fFrameTime;
	m_tCBuffer.iBoneCount = m_pSkeleton->GetBoneCount();
	m_tCBuffer.iFrame = iFrame % m_pSequence->m_iFrameCount;
	m_tCBuffer.iNextFrame = (iFrame + 1) % m_pSequence->m_iFrameCount;
	if (m_bEnd)
	{
		m_tCBuffer.iFrame = m_pSequence->m_iFrameCount - 2;
		m_tCBuffer.iNextFrame = m_tCBuffer.iFrame + 1;
		m_tCBuffer.fRatio = 1.f;
	}
	m_tCBuffer.iFrameCount = m_pSequence->m_iFrameCount;

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Bone", &m_tCBuffer);

	m_pSkeleton->SetShaderResource(13);
	CONTEXT->CSSetUnorderedAccessViews(1, 1, &m_pBoneUAV, &iCount);

	m_pSequence->SetShaderResource(14);

	m_pUpdateShader->SetShader();

	int iX = m_pSkeleton->GetBoneCount() / 256 + 1;

	CONTEXT->Dispatch(iX, 1, 1);

	if (m_pTransition)
	{
		m_pTransition->pNextSequence->ResetShaderResource(15);
	}

	static_cast<CComputeShader*>(m_pUpdateShader.get())->Clear();

	m_pSequence->ResetShaderResource(14);

	ID3D11UnorderedAccessView* pUav = nullptr;
	CONTEXT->CSSetUnorderedAccessViews(0, 1, &pUav, &iCount);
	CONTEXT->CSSetUnorderedAccessViews(1, 1, &pUav, &iCount);

	m_pSkeleton->ResetShaderResource(13);

	m_vecFinalBone.resize(m_pSkeleton->GetBoneCount());

	CONTEXT->CopyResource(m_pCopyBuffer, m_pBoneBuffer);

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_pCopyBuffer, 0, D3D11_MAP_READ, 0, &tSub);

	memcpy_s(&m_vecFinalBone[0], sizeof(Matrix) * m_pSkeleton->GetBoneCount(), tSub.pData, sizeof(Matrix) * m_pSkeleton->GetBoneCount());

	CONTEXT->Unmap(m_pCopyBuffer, 0);

	if (!m_pTransition)
	{
		std::list<_tagAnimationTransition*>::iterator iter = m_pSequence->m_TransitionList.begin();
		std::list<_tagAnimationTransition*>::iterator iterEnd = m_pSequence->m_TransitionList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->bAutoTransition && m_bEnd)
			{
				m_pTransition = *iter;
				m_bEnd = false;

				std::list<std::function<void()>>::iterator iterF = m_pTransition->NotifyFuncList.begin();
				std::list<std::function<void()>>::iterator iterFEnd = m_pTransition->NotifyFuncList.end();

				for (; iterF != iterFEnd; ++iterF)
				{
					(*iterF)();
				}

					std::list<PANIMATIONNOTIFY>::iterator iter = m_NotifyList.begin();
					std::list<PANIMATIONNOTIFY>::iterator iterEnd = m_NotifyList.end();

					for (; iter != iterEnd; ++iter)
					{
						if ((*iter)->pSequence == m_pSequence)
						{
							(*iter)->bCall = false;
						}
					}

			}

			else
			{
				std::vector<std::function<bool()>>::iterator iterF = (*iter)->vecTransitionFunc.begin();
				std::vector<std::function<bool()>>::iterator iterFEnd = (*iter)->vecTransitionFunc.end();

				bool bResult = false;
				for (; iterF != iterFEnd; ++iterF)
				{
					if (*iterF)
					{
						if ((*iterF)())
						{
							bResult = true;
						}
					}
				}

				if (bResult)
				{
					m_pTransition = *iter;
					m_bEnd = false;

					std::list<std::function<void()>>::iterator iterF = m_pTransition->NotifyFuncList.begin();
					std::list<std::function<void()>>::iterator iterFEnd = m_pTransition->NotifyFuncList.end();

					for (; iterF != iterFEnd; ++iterF)
					{
						(*iterF)();
					}

					std::list<PANIMATIONNOTIFY>::iterator iter = m_NotifyList.begin();
					std::list<PANIMATIONNOTIFY>::iterator iterEnd = m_NotifyList.end();

					for (; iter != iterEnd; ++iter)
					{
						if ((*iter)->pSequence == m_pSequence)
						{
							(*iter)->bCall = false;
						}
					}
				}
			}
		}
	}

	if (!m_pTransition)
	{
		std::list<PANIMATIONNOTIFY>::iterator iter = m_NotifyList.begin();
		std::list<PANIMATIONNOTIFY>::iterator iterEnd = m_NotifyList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->bOnce)
			{
				if (!(*iter)->bCall )
				{
					if ((*iter)->pSequence == m_pSequence && (*iter)->fTime <= m_fAnimationTime)
					{
						size_t iSize = (*iter)->vecNotify.size();

						for (size_t i = 0; i < iSize; i++)
						{
							(*iter)->vecNotify[i](fTime);
						}

						(*iter)->bCall = true;
					}

					else if(m_pBlendSequence)
					{
						if ((*iter)->pSequence == m_pBlendSequence && (*iter)->fTime <= m_fBlendTime)
						{
							size_t iSize = (*iter)->vecNotify.size();

							for (size_t i = 0; i < iSize; i++)
							{
								(*iter)->vecNotify[i](fTime);
							}

							(*iter)->bCall = true;
						}
					}
				}
			}
			else
			{
				if ((*iter)->pSequence == m_pSequence &&
					(*iter)->fTime <= m_fAnimationTime &&
					(*iter)->fEndTime >= m_fAnimationTime)
				{
					size_t iSize = (*iter)->vecTimeLineNotify.size();

					for (size_t i = 0; i < iSize; i++)
					{
						(*iter)->vecTimeLineNotify[i](m_fAnimationTime, fTime);
					}
				}
				else if (m_pBlendSequence)
				{
					if ((*iter)->pSequence == m_pBlendSequence &&
						(*iter)->fTime <= m_fBlendTime &&
						(*iter)->fEndTime >= m_fBlendTime)
					{
						size_t iSize = (*iter)->vecTimeLineNotify.size();

						for (size_t i = 0; i < iSize; i++)
						{
							(*iter)->vecTimeLineNotify[i](m_fBlendTime, fTime);
						}
					}
				}
			}
		}
	}

	if (m_bEnd)
	{
		std::list<PANIMATIONNOTIFY>::iterator iter = m_NotifyList.begin();
		std::list<PANIMATIONNOTIFY>::iterator iterEnd = m_NotifyList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->pSequence == m_pSequence)
			{
				(*iter)->bCall = false;
			}
		}

		float fDelta = m_fAnimationTime - m_pSequence->m_fEndTime;
		m_fAnimationTime = m_pSequence->m_fStartTime - fDelta;
		m_bEnd = false;
	}

	
	UpdateSocket(fTime);

	m_bUpdate = true;
}

void CAnimation::PostUpdate(float fTime)
{
	m_bUpdate = false;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Save(FILE* pFile)
{
	CRef::Save(pFile);

	bool bSeq = false;

	if (m_pSequence)
	{
		bSeq = true;
	}

	fwrite(&bSeq, 1, 1, pFile);

	if (m_pSequence)
	{
		std::string strSeq = m_pSequence->GetName();
		int iLength = static_cast<int>(strSeq.length());
		fwrite(&iLength, 4, 1, pFile);

		if (iLength > 0)
		{
			fwrite(strSeq.c_str(), 1, iLength, pFile);
		}
	}

	int iSeqSize = static_cast<int>(m_mapSequence.size());

	fwrite(&iSeqSize, 4, 1, pFile);

	std::unordered_map<size_t, CAnimationSequence*>::iterator iter = m_mapSequence.begin();
	std::unordered_map<size_t, CAnimationSequence*>::iterator iterEnd = m_mapSequence.end();
	for (; iter != iterEnd; ++iter)
	{
		fwrite(&iter->first, 8, 1, pFile);
		std::string strSeq = iter->second->GetName();

		int iLength = static_cast<int>(strSeq.length());

		fwrite(&iLength, 4, 1, pFile);

		if (iLength > 0)
		{
			fwrite(strSeq.c_str(), 1, iLength, pFile);
		}
		//iter->second->Save(pFile);
	}

	bool bSkel = false;

	if (m_pSkeleton)
	{
		bSkel = true;
	}

	fwrite(&bSkel, 1, 1, pFile);

	if (m_pSkeleton)
	{
		m_pSkeleton->Save(pFile);

		/*std::string strSkel = m_pSkeleton->GetName();
		
		int iLength = static_cast<int>(strSkel.length());

		fwrite(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fwrite(strSkel.c_str(), 1, iLength, pFile);
		}*/
	}

	fwrite(&m_bEnd, 1, 1, pFile);
	fwrite(&m_fAnimationTime, 4, 1, pFile);
	fwrite(&m_fTransitionTime, 4, 1, pFile);
	fwrite(&m_fTransitionTimeLimit, 4, 1, pFile);

	int iSize = static_cast<int>(m_NotifyList.size());
	fwrite(&iSize, 4, 1, pFile);

	std::list<PANIMATIONNOTIFY>::iterator iterN = m_NotifyList.begin();
	std::list<PANIMATIONNOTIFY>::iterator iterNEnd = m_NotifyList.end();
	for (; iterN != iterNEnd; ++iterN)
	{
		int iLength = static_cast<int>((*iterN)->strName.length());
		fwrite(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fwrite((*iterN)->strName.c_str(), 1, iLength, pFile);
		}
		std::unordered_map<size_t, CAnimationSequence*>::iterator iterA = m_mapSequence.begin();
		std::unordered_map<size_t, CAnimationSequence*>::iterator iterAEnd = m_mapSequence.end();

		for (; iterA != iterAEnd; iterA++)
		{
			if (iterA->second == (*iterN)->pSequence)
			{
				size_t iKey = iterA->first;

				fwrite(&iKey, sizeof(size_t), 1, pFile);
				break;
			}
		}
		fwrite(&(*iterN)->fTime, 4, 1, pFile);
		fwrite(&(*iterN)->bCall, 1, 1, pFile);
	}

	int iSockSize = static_cast<int>(m_SocketList.size());
	fwrite(&iSockSize, 4, 1, pFile);
	std::list<CBoneSocket*>::iterator iterS = m_SocketList.begin();
	std::list<CBoneSocket*>::iterator iterSEnd = m_SocketList.end();

	for (; iterS != iterSEnd; ++iterS)
	{
		(*iterS)->Save(pFile);
	}

	fwrite(&m_bStop, 1, 1, pFile);
}

void CAnimation::Load(FILE* pFile)
{
	CRef::Load(pFile);

	bool bSeq = false;
	fread(&bSeq, 1, 1, pFile);

	char strCurSeq[MAX_PATH] = {};

	if (bSeq)
	{

		int iLength = 0;
		fread(&iLength, 4, 1, pFile);

		if (iLength > 0)
		{
			fread(strCurSeq, 1, iLength, pFile);
		}
	}

	int iSeqSize = 0;

	fread(&iSeqSize, 4, 1, pFile);

	for (int i = 0; i < iSeqSize; i++)			 
	{
		size_t iKey = 0;
		fread(&iKey, 8, 1, pFile);
		char strSeq[MAX_PATH] = {};

		int iLength = 0;

		fread(&iLength, 4, 1, pFile);

		if (iLength > 0)
		{
			fread(strSeq, 1, iLength, pFile);
		}

		CAnimationSequence* pProto= GET_SINGLE(CResourceManager)->FindSequence(strSeq);

		CAnimationSequence* pSequence = pProto->Clone();

		SAFE_RELEASE(pProto);

		m_mapSequence.insert(std::make_pair(iKey, pSequence));

		if (!strcmp(strCurSeq, strSeq))
		{
			m_pSequence = pSequence;
		}
	}

	bool bSkel = false;

	fread(&bSkel, 1, 1, pFile);

	if (bSkel)
	{
		m_pSkeleton = GET_SINGLE(CResourceManager)->LoadSkeleton(pFile);
	}

	fread(&m_bEnd, 1, 1, pFile);
	fread(&m_fAnimationTime, 4, 1, pFile);
	fread(&m_fTransitionTime, 4, 1, pFile);
	fread(&m_fTransitionTimeLimit, 4, 1, pFile);

	int iSize = 0;
	fread(&iSize, 4, 1, pFile);

	for (int i = 0; i < iSize; i++)
	{
		PANIMATIONNOTIFY pNot = new ANIMATIONNOTIFY;
		int iLength = 0;
		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			char strName[MAX_PATH] = {};
			fread(strName, 1, iLength, pFile);
			pNot->strName = strName;
		}
		size_t iKey = 0;
		fread(&iKey, sizeof(size_t), 1, pFile);

		std::unordered_map<size_t, CAnimationSequence*>::iterator iter = m_mapSequence.find(iKey);

		pNot->pSequence = iter->second;

		fread(&pNot->fTime, 4, 1, pFile);
		fread(&pNot->bCall, 1, 1, pFile);

		m_NotifyList.push_back(pNot);
	}

	int iSockSize = 0;
	fread(&iSockSize, 4, 1, pFile);
	std::list<CBoneSocket*>::iterator iterS = m_SocketList.begin();
	std::list<CBoneSocket*>::iterator iterSEnd = m_SocketList.end();

	for (int i = 0; i < iSockSize; i++)
	{
		CBoneSocket* pSoc = new CBoneSocket;

		pSoc->Load(pFile);

		m_SocketList.push_back(pSoc);
	}

	fread(&m_bStop, 1, 1, pFile);

	m_pUpdateShader = GET_SINGLE(CShaderManager)->FindShader("Anim");
	Start();
}

CAnimation* CAnimation::LoadAnimation(FILE* pFile)
{
	CAnimation* pAnimation = new CAnimation;

	pAnimation->Load(pFile);

	return pAnimation;
}

void CAnimation::SetShaderResource()
{
	CONTEXT->VSSetShaderResources(110, 1 , &m_pSrv);
}

void CAnimation::ResetShaderResource()
{
	ID3D11ShaderResourceView* pSrv = nullptr;

	CONTEXT->VSSetShaderResources(110, 1, &pSrv);
}

void CAnimation::AddSocket(const std::string& strSocket, const std::string& strBone)
{
	CBoneSocket* pSocket = FindSocket(strSocket);

	if (pSocket)
	{
		SAFE_RELEASE(pSocket);
		return;
	}

	int iBoneIndex = m_pSkeleton->GetBoneIndex(strBone);

	if (iBoneIndex == -1)
		return;
	
	pSocket = new CBoneSocket;

	if (!pSocket->Init(strSocket, iBoneIndex, strBone))
	{
		SAFE_RELEASE(pSocket);
		return;
	}
	
	m_SocketList.push_back(pSocket);
}

void CAnimation::SetSocketObject(const std::string& strSocket, const std::shared_ptr<class CGameObject>& pObj)
{
	CBoneSocket* pSocket = FindSocket(strSocket);

	if (!pSocket)
		return;

	if (!pObj)
	{
		SAFE_RELEASE(pSocket);
		return;
	}

	std::shared_ptr<CComponent> pCom = pObj->GetRootComponent();

	if (!pCom)
	{
		SAFE_RELEASE(pSocket);
		return;
	}
		

	pSocket->AddComponent(pCom);

	SAFE_RELEASE(pSocket);
}

void CAnimation::SetSocketObject(const std::string& strSocket, CGameObject* pObj)
{
	CBoneSocket* pSocket = FindSocket(strSocket);

	if (!pSocket)
		return;

	if (!pObj)
	{
		SAFE_RELEASE(pSocket);
		return;
	}
	std::shared_ptr<CComponent> pCom = pObj->GetRootComponent();

	if (!pCom)
	{
		SAFE_RELEASE(pSocket);
		return;
	}

	pSocket->AddComponent(pCom);

	SAFE_RELEASE(pSocket);
}

void CAnimation::SetSocketComponent(const std::string& strSocket, const std::shared_ptr<class CComponent>& pCom)
{
	CBoneSocket* pSocket = FindSocket(strSocket);

	if (!pSocket)
		return;

	pSocket->AddComponent(pCom);
	SAFE_RELEASE(pSocket);
}

void CAnimation::SetSocketOffset(const std::string& strSocket, const Vector3& vOffset)
{
	CBoneSocket* pSocket = FindSocket(strSocket);

	if (!pSocket)
		return;

	pSocket->SetOffset(vOffset);
	SAFE_RELEASE(pSocket);
}

void CAnimation::SetSocketRotation(const std::string& strSocket, const Vector3& vRot)
{
	CBoneSocket* pSocket = FindSocket(strSocket);

	if (!pSocket)
		return;

	pSocket->SetRotation(vRot);
	SAFE_RELEASE(pSocket);
}

void CAnimation::DeleteSocketObject(const std::string& strSocket, const std::shared_ptr<class CGameObject>& pObj)
{
	CBoneSocket* pSocket = FindSocket(strSocket);

	if (!pSocket)
		return;

	if (!pObj)
	{
		SAFE_RELEASE(pSocket);
		return;
	}

	std::shared_ptr<CComponent> pCom = pObj->GetRootComponent();

	if (!pCom)
	{
		SAFE_RELEASE(pSocket);
		return;
	}

	pSocket->DeleteComponent(pCom);
	SAFE_RELEASE(pSocket);
}

void CAnimation::DeleteSocketObject(const std::string& strSocket, CGameObject* pObj)
{
	CBoneSocket* pSocket = FindSocket(strSocket);

	if (!pSocket)
		return;

	if (!pObj)
	{
		SAFE_RELEASE(pSocket);
		return;
	}

	std::shared_ptr<CComponent> pCom = pObj->GetRootComponent();

	if (!pCom)
	{
		SAFE_RELEASE(pSocket);
		return;
	}

	pSocket->DeleteComponent(pCom);
	SAFE_RELEASE(pSocket);
}

void CAnimation::DeleteSocketComponent(const std::string& strSocket, const std::shared_ptr<class CComponent>& pCom)
{
	CBoneSocket* pSocket = FindSocket(strSocket);

	if (!pSocket)
		return;

	pSocket->DeleteComponent(pCom);
	SAFE_RELEASE(pSocket);
}

void CAnimation::UpdateSocket(float fTime)
{
	m_pOwner->GetTransform()->SetWorldMatrix();

	std::list<CBoneSocket*>::iterator iterB = m_SocketList.begin();
	std::list<CBoneSocket*>::iterator iterBEnd = m_SocketList.end();

	for (; iterB != iterBEnd; ++iterB)
	{
		if ((*iterB)->m_iBoneIndex >= m_vecFinalBone.size())
			continue;

		Matrix mat = m_vecFinalBone[(*iterB)->m_iBoneIndex];

		(*iterB)->Update(fTime, mat * m_pOwner->GetMatWorld());
	}
}

bool CAnimation::LoadSkeletonFromFull(const std::string& strBone, const char* pFullPath)
{
	if (!GET_SINGLE(CResourceManager)->LoadSkeletonFullPath(strBone, pFullPath))
		return false;

	CSkeleton* pSkeleton = GET_SINGLE(CResourceManager)->FindSkeleton(strBone);

	m_pSkeleton = pSkeleton->Clone();

	SAFE_RELEASE(pSkeleton);

	return true;
}

CBoneSocket* CAnimation::FindSocket(const std::string& strKey)
{
	std::list<CBoneSocket*>::iterator iter = m_SocketList.begin();
	std::list<CBoneSocket*>::iterator iterEnd = m_SocketList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == strKey)
		{
			(*iter)->AddRef();

			return *iter;
		}
	}

	return nullptr;
}

void CAnimation::AddTransition(const std::string& strTransition, const std::string& strSeq, 
	bool(*pFunc)(), const std::string& strNextSeq)
{
	CAnimationSequence* pSeq = FindSequence(strSeq);

	if (!pSeq)
		return;

	std::list<PANIMATIONTRANSITION>::iterator iter = pSeq->m_TransitionList.begin();
	std::list<PANIMATIONTRANSITION>::iterator iterEnd = pSeq->m_TransitionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->strName == strTransition)
		{
			SAFE_RELEASE(pSeq);
			return;
		}
	}

	CAnimationSequence* pNextSequence = FindSequence(strNextSeq);

	if (!pNextSequence)
	{
		SAFE_RELEASE(pSeq);
		return;
	}

	PANIMATIONTRANSITION pTransition = new ANIMATIONTRANSITION;

	pTransition->pNextSequence = pNextSequence;
	pTransition->strSequence = strSeq;
	pTransition->strName = strTransition;
	pTransition->vecTransitionFunc.push_back(std::bind(pFunc));
	
	pSeq->m_TransitionList.push_back(pTransition);

	SAFE_RELEASE(pSeq);
	SAFE_RELEASE(pNextSequence);
}

void CAnimation::AddTransitionNotify(const std::string& strTransition, const std::string& strSequence, void(*pFunc)())
{
   	CAnimationSequence* pSequence = FindSequence(strSequence);

	if (!pSequence)
		return;

	std::list<PANIMATIONTRANSITION>::iterator	iter = pSequence->m_TransitionList.begin();
	std::list<PANIMATIONTRANSITION>::iterator	iterEnd = pSequence->m_TransitionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->strName == strTransition)
		{
			(*iter)->NotifyFuncList.push_back(std::bind(pFunc));
			SAFE_RELEASE(pSequence);
			return;
		}
	}
	SAFE_RELEASE(pSequence);
}

void CAnimation::AutoTransition(const std::string& strSequence, const std::string& strTransition, const std::string& strNext)
{
	CAnimationSequence* pSeq = FindSequence(strSequence);

	if (!pSeq)
		return;

	std::list<PANIMATIONTRANSITION>::iterator iter = pSeq->m_TransitionList.begin();
	std::list<PANIMATIONTRANSITION>::iterator iterEnd = pSeq->m_TransitionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->strName == strTransition)
		{
			SAFE_RELEASE(pSeq);
			return;
		}
	}

	CAnimationSequence* pNextSequence = FindSequence(strNext);

	if (!pNextSequence)
	{
		SAFE_RELEASE(pSeq);
		return;
	}

	PANIMATIONTRANSITION pTransition = new ANIMATIONTRANSITION;
	pTransition->bAutoTransition = true;
	pTransition->pNextSequence = pNextSequence;
	pTransition->strSequence = strSequence;
	pTransition->strName = strTransition;

	pSeq->m_TransitionList.push_back(pTransition);

	SAFE_RELEASE(pNextSequence);
	SAFE_RELEASE(pSeq);
}

void CAnimation::CreateNotify(const std::string& strSequence, 
	const std::string& strNotify, float fTime, float fEndTime)
{
	PANIMATIONNOTIFY pNotify = FindNotify(strNotify);

	if (pNotify)
		return;

	CAnimationSequence* pSequence = FindSequence(strSequence);

	if (!pSequence)
		return;

	pNotify = new ANIMATIONNOTIFY;

	pNotify->fTime = fTime;
	pNotify->fEndTime = fEndTime;

	if (pNotify->fEndTime != -1.f)
	{
		pNotify->bOnce = false;
	}
	pNotify->pSequence = pSequence;
	pNotify->strName = strNotify;

	m_NotifyList.push_back(pNotify);	

	SAFE_RELEASE(pSequence);
}

void CAnimation::AddNotifyFunction(const std::string& strNotify, void(*pFunc)(float))
{
	PANIMATIONNOTIFY pNotify = FindNotify(strNotify);

	if (!pNotify)
		return;

	pNotify->vecNotify.push_back(std::bind(pFunc, std::placeholders::_1));
}

void CAnimation::AddTimeLineNotifyFunction(const std::string& strNotify, void(*pFunc)(float, float))
{
	PANIMATIONNOTIFY pNotify = FindNotify(strNotify);

	if (!pNotify)
		return;

	pNotify->vecTimeLineNotify.push_back(std::bind(pFunc, std::placeholders::_1, std::placeholders::_2));
}

PANIMATIONNOTIFY CAnimation::FindNotify(const std::string& strKey)
{
	std::list<PANIMATIONNOTIFY>::iterator iter = m_NotifyList.begin();
	std::list<PANIMATIONNOTIFY>::iterator iterEnd = m_NotifyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->strName == strKey)
			return (*iter);
	}

	return nullptr;
}

void CAnimation::AddEndCallBack(const std::string& strSequence, void(pFunc)())
{
	CAnimationSequence* pSequence = FindSequence(strSequence);

	if (!pSequence)
		return;

	pSequence->AddEndCallBack(pFunc);

	SAFE_RELEASE(pSequence);
}

void CAnimation::ShowWindow()
{
	//if (ImGui::Begin("Animation"))
	{
		if (m_pSkeleton)
		{
			m_pSkeleton->ShowWindow();
		}

		ImGui::SliderFloat("Ratio", &m_tCBuffer.fRatio, 0.f, 1.f);
		ImGui::SliderInt("BoneCount", &m_tCBuffer.iBoneCount, 0, 256);
		if (m_pSequence)
		{
			ImGui::SliderInt("CurFrame", &m_tCBuffer.iFrame, 0, m_pSequence->m_iFrameCount);
			ImGui::SliderInt("FrameCount", &m_tCBuffer.iFrameCount, 0, m_pSequence->m_iFrameCount);
			ImGui::Text("Sequence");
			ImGui::Text(m_pSequence->GetName().c_str());
			if (m_pTransition)
			{
				ImGui::Text("Transition InfoName");
				ImGui::Text(m_pTransition->strName.c_str());
				ImGui::Text("Transition Sequence Name");
				ImGui::Text(m_pTransition->strSequence.c_str());
			}
			ImGui::SliderFloat("GlobalTime", &m_fAnimationTime, m_pSequence->m_fStartTime, m_pSequence->m_fEndTime);

			int iSize = static_cast<int>(m_pSequence->m_TransitionList.size());

			std::vector<const char*> vecName;

			std::list<PANIMATIONTRANSITION>::iterator iter = m_pSequence->m_TransitionList.begin();
			std::list<PANIMATIONTRANSITION>::iterator iterEnd = m_pSequence->m_TransitionList.end();

			for (; iter != iterEnd; ++iter)
			{
				vecName.push_back((*iter)->strName.c_str());
			}

			static int iSel = -1;

			if (iSize > 0)
			{
				ImGui::ListBox("Transition List", &iSel, &vecName[0], iSize);
			}

			if (iSel != -1 && iSel < iSize)
			{
				std::list<PANIMATIONTRANSITION>::iterator iter2 = m_pSequence->m_TransitionList.begin();
				std::list<PANIMATIONTRANSITION>::iterator iter2End = m_pSequence->m_TransitionList.end();

				for (int i = 0; iter2 != iter2End; iter2++, i++)
				{
					if ((*iter2)->strName == vecName[i])
					{
						ImGui::Text("Name: %s", (*iter2)->strName.c_str());
						ImGui::Text("Sequence Name: %s", (*iter2)->strName.c_str());
						ImGui::Text("Next Name: %s", (*iter2)->pNextSequence->GetName().c_str());
						ImGui::Checkbox("Auto Transition", &(*iter2)->bAutoTransition);
					}
				}
			}
		}
		ImGui::SliderFloat("Transition Limit Time", &m_fTransitionTimeLimit, 0.f, 1.f);
		ImGui::SliderFloat("Transition Time", &m_fTransitionTime, 0.f, m_fTransitionTimeLimit);
		ImGui::SliderFloat("Transition Ratio", &m_tCBuffer.fTransitionRatio, 0.f, 1.f);
		ImGui::SliderInt("Transition", &m_tCBuffer.iTransition, 0, 1);
		ImGui::SliderInt("ChangeFrame", &m_tCBuffer.iNextFrameCount, 0, 256);
		if (m_pBlendSequence)
		{
			ImGui::SliderFloat("Blend Time", &m_fBlendTime, m_pBlendSequence->m_fStartTime, m_pBlendSequence->m_fEndTime);
			ImGui::SliderFloat("Blend Rate", &m_tCBuffer.fBlendRate, 0.f, 1.f);
			ImGui::SliderInt("Blend Frame", &m_tCBuffer.iBlendFrame, m_pBlendSequence->m_iStartFrame, m_pBlendSequence->m_iEndFrame);
			ImGui::Text("Blend Frame Count: %d", m_tCBuffer.iBlendFrameCount);
		}
		static int iItem = -1;
		size_t iSize = m_mapSequence.size();
		std::vector<const char*> vecStr;
		std::vector<size_t> vecHs;

		std::unordered_map<size_t,CAnimationSequence*>::iterator iter = m_mapSequence.begin();
		std::unordered_map<size_t,CAnimationSequence*>::iterator iterEnd = m_mapSequence.end();

		for (int i = 0; iter != iterEnd; ++iter, i++)
		{
			vecStr.push_back(iter->second->GetName().c_str());
			vecHs.push_back(iter->first);
		}

		if (iSize > 0)
		{
			ImGui::ListBox("Sequence", &iItem, &vecStr[0], static_cast<int>(iSize));
		}

		if (iItem >= 0)
		{
			std::unordered_map<size_t, CAnimationSequence*>::iterator iter = m_mapSequence.find(vecHs[iItem]);
			iter->second->ShowWindow();
			if (ImGui::Button("Change Sequence"))
			{
				m_pSequence = iter->second;
			}
		}

		if (ImGui::Button("Pause"))
		{
			m_bStop = true;
		}

		if (ImGui::Button("Play"))
		{
			m_bStop = false;
		}

		if (ImGui::Button("Stop"))
		{
			m_bStop = true;
			m_fAnimationTime = 0.f;
		}

		ImGui::SliderFloat("Time Scale", &m_fTimeScale, 0.f, 2.f);

		size_t iSock = m_SocketList.size();

		std::vector<const char*> vecSocket(iSock);

		std::list<CBoneSocket*>::iterator iterS = m_SocketList.begin();
		std::list<CBoneSocket*>::iterator iterSEnd = m_SocketList.end();

		for (int i = 0; iterS != iterSEnd; ++iterS, ++i)
		{
			vecSocket[i] = (*iterS)->m_strName.c_str();
		}

		static int iSel = -1;
		if (iSock > 0)
			ImGui::ListBox("BoneSocket List", &iSel, &vecSocket[0], static_cast<int>(iSock));

		if (iSel != -1 && iSel < iSock)
		{
			CBoneSocket* pBoneSocket = FindSocket(vecSocket[iSel]);

			if (pBoneSocket)
			{
				pBoneSocket->ShowWindow();
			}
			SAFE_RELEASE(pBoneSocket);
		}
		static char pText[MAX_PATH] = {};
		ImGui::InputText("Sequence Name", pText, MAX_PATH);

		if (ImGui::Button("Load Sequence"))
		{
			const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(MESH_PATH);
			ImGuiFileDialog::Instance()->OpenDialog("Load Sequence", "Load Sequence", ".sqc", pPath);
		}

		if (ImGuiFileDialog::Instance()->Display("Load Sequence"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string strFilePath = ImGuiFileDialog::Instance()->GetFilePathName();

				TCHAR pFilePath[MAX_PATH] = {};

#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, strFilePath.c_str(), -1, pFilePath, static_cast<int>(strFilePath.length()));
#else
				strcpy_s(pFilePath, MAX_PATH, strFilePath.c_str());
#endif

				if (GET_SINGLE(CResourceManager)->LoadAnimationSequenceFullPath(pText, true, pFilePath))
				{
					AddAnimationSequence(pText, pText);
				}
			}

			ImGuiFileDialog::Instance()->Close();
		}
	}
	//ImGui::End();
}

void CAnimation::Pause()
{
	m_bStop = true;
}

void CAnimation::CreateIndentirySequence(int iFrame)
{
	CAnimationSequence* pSequence = new CAnimationSequence;

	pSequence->CreateIndentitySequence(m_pSkeleton->GetBoneCount(), iFrame);

	std::string strKey = "Identity";

	std::hash<std::string> hs;

	size_t iKey = hs(strKey);

	m_mapSequence.insert(std::make_pair(iKey,pSequence));

	if (!m_pSequence)
		m_pSequence = pSequence;
}


void CAnimation::SetSkeleton(const std::string& strBone)
{
	SAFE_RELEASE(m_pSkeleton);

	CSkeleton* pSkel = GET_SINGLE(CResourceManager)->FindSkeleton(strBone);

	if (!pSkel)
		return;

	m_pSkeleton = pSkel->Clone();

	SAFE_RELEASE(pSkel);
}

bool CAnimation::LoadSkeleton(const std::string& strBone, const char* pFileName, const std::string& strPathKey)
{
	if (!GET_SINGLE(CResourceManager)->LoadSkeleton(strBone, pFileName, strPathKey))
		return false;

	CSkeleton* pSkeleton = GET_SINGLE(CResourceManager)->FindSkeleton(strBone);

	m_pSkeleton = pSkeleton->Clone();

	SAFE_RELEASE(pSkeleton);

	return true;
}