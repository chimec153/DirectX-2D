#include "AnimationSequence.h"
#include "FBXLoader.h"
#include "../Device.h"
#include "../PathManager.h"
#include "../UI/imgui/imgui.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

CAnimationSequence::CAnimationSequence()	:
	m_fStartTime(0.f)
	, m_fEndTime(0.f)
	, m_fTimeLength(0.f)
	, m_fFrameTime(0.f)
	, m_iStartFrame(0)
	, m_iEndFrame(0)
	, m_iFrameCount(0)
	, m_iTimeMode(0)
	, m_bLoop(false)
	, m_pBuffer(nullptr)
	, m_pSRV(nullptr)
	, m_strFullPath()
	, m_fPlayTime(0.f)
	, m_fPlayScale(1.f)
	, m_iChangeFrame(0)
	, m_pBlendMaskSRV(nullptr)
	, m_pBlendBuffer(nullptr)
{
}

CAnimationSequence::CAnimationSequence(const CAnimationSequence& seq)	:
	CRef(seq)
	, m_fStartTime(seq.m_fStartTime)
	, m_fEndTime(seq.m_fEndTime)
	, m_fTimeLength(seq.m_fTimeLength)
	, m_fFrameTime(seq.m_fFrameTime)
	, m_iStartFrame(seq.m_iStartFrame)
	, m_iEndFrame(seq.m_iEndFrame)
	, m_iFrameCount(seq.m_iFrameCount)
	, m_iTimeMode(seq.m_iTimeMode)
	, m_bLoop(seq.m_bLoop)
	, m_pBuffer(nullptr)
	, m_pSRV(nullptr)
	, m_fPlayTime(seq.m_fPlayTime)
	, m_fPlayScale(seq.m_fPlayScale)
	, m_iChangeFrame(seq.m_iChangeFrame)
	, m_pBlendMaskSRV(nullptr)
	, m_pBlendBuffer(nullptr)
{
	memcpy_s(m_strFullPath, MAX_PATH, seq.m_strFullPath, MAX_PATH);

	size_t iSize = seq.m_vecBoneKeyFrames.size();

	m_vecBoneKeyFrames.resize(iSize);

	std::vector<ANIMATIONKEYFRAME>	vecFrames(iSize * m_iFrameCount);

	for (size_t i = 0; i < iSize; i++)
	{
		PBONEKEYFRAME pBoneKeyFrame = new BONEKEYFRAME;

		m_vecBoneKeyFrames[i] = pBoneKeyFrame;

		size_t iFrame = seq.m_vecBoneKeyFrames[i]->vecKeyFrame.size();
		m_vecBoneKeyFrames[i]->vecKeyFrame.resize(iFrame);
		for (size_t j = 0; j < iFrame; j++)
		{
			PKEYFRAME pFrame = new KEYFRAME;

			m_vecBoneKeyFrames[i]->vecKeyFrame[j] = pFrame;

			*pFrame = *seq.m_vecBoneKeyFrames[i]->vecKeyFrame[j];

			if (j < m_iFrameCount)
			{
				memcpy_s(&vecFrames[i * m_iFrameCount + j].vPos.x, 16, &pFrame->vPos.x, 12);
				memcpy_s(&vecFrames[i * m_iFrameCount + j].vScale.x, 16, &pFrame->vScale.x, 12);
				memcpy_s(&vecFrames[i * m_iFrameCount + j].vRot.x, 16, &pFrame->qRot.x, 16);
			}
		}
	}

	m_vecBlendMask.resize(seq.m_vecBlendMask.size());

	memcpy_s(&m_vecBlendMask[0], 4 * m_vecBlendMask.size(), &seq.m_vecBlendMask[0], seq.m_vecBlendMask.size() * 4);

	CreateBufferAndSRV(vecFrames, static_cast<int>(iSize));
}

CAnimationSequence::~CAnimationSequence()
{
	SAFE_DELETE_VECLIST(m_vecBoneKeyFrames);
	SAFE_DELETE_VECLIST(m_TransitionList);
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pBlendBuffer);
	SAFE_RELEASE(m_pBlendMaskSRV);
}

bool CAnimationSequence::LoadFbxAnimation(const char* pFullPath, bool bLoop, bool bRootMotionRemove)
{
	CFBXLoader loader;

	loader.LoadFbx(pFullPath, false);

	const std::vector<PFBXANIMATIONCLIP>* pvecClip = loader.GetClips();

	strcpy_s(m_strFullPath, MAX_PATH, pFullPath);

	std::vector<PFBXANIMATIONCLIP>::const_iterator	iter = pvecClip->begin();
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterEnd = pvecClip->end();

	for (; iter != iterEnd; ++iter)
	{
		if (!CreateSequence(bLoop, *iter, bRootMotionRemove))
		{
			return false;
		}
#ifdef _DEBUG
		else
		{
			OutputDebugStringA("Create Sequence");
			OutputDebugStringA((*iter)->strName.c_str());
			OutputDebugStringA("\n");
		}
#endif
			
	}

	return true;
}

bool CAnimationSequence::CreateSequence(bool bLoop, _tagFbxAnimationClip* pClip, bool bRootMotionRemove)
{
	m_bLoop = bLoop;

	switch (pClip->eTimeMode)
	{
	case FbxTime::eFrames24:
		m_iTimeMode = 24;
		break;
	case FbxTime::eFrames30:
		m_iTimeMode = 30;
		break;
	case FbxTime::eFrames60:	//	³ë¸» ÅºÁ¨Æ®
		m_iTimeMode = 60;
		break;
	}

	m_iStartFrame = static_cast<int>(pClip->tStart.GetFrameCount(pClip->eTimeMode));
	m_iEndFrame = static_cast<int>(pClip->tEnd.GetFrameCount(pClip->eTimeMode));

	m_iFrameCount = m_iEndFrame - m_iStartFrame;

	m_fStartTime = pClip->tStart.GetMilliSeconds() / 1000.f;
	m_fEndTime = pClip->tEnd.GetMilliSeconds() / 1000.f;

	m_fTimeLength = m_fEndTime - m_fStartTime;

	m_fFrameTime = m_fTimeLength / m_iFrameCount;

	size_t iSize = pClip->vecBoneKeyFrame.size();

	std::vector<ANIMATIONKEYFRAME>	vecFrames(iSize * m_iFrameCount);

	std::vector<KEYFRAME> vecRootFrames;
	if (bRootMotionRemove)
	{
		for (size_t i = 0; i < iSize; i++)
		{
			size_t iFrame = pClip->vecBoneKeyFrame[i].vecKeyFrame.size();

			if (iFrame == 0)
				continue;

			vecRootFrames.resize(iFrame);

			for (size_t j = 0; j < iFrame; j++)
			{
				KEYFRAME tFrame = {};

				fbxsdk::FbxAMatrix matTransform = pClip->vecBoneKeyFrame[i].vecKeyFrame[j].matTransform;
				FbxVector4 vT = matTransform.GetT();
				tFrame.vPos.x =  static_cast<float>(vT.mData[0]);
				tFrame.vPos.y = static_cast<float>(vT.mData[1]);
				tFrame.vPos.z = static_cast<float>(vT.mData[2]);

				vecRootFrames[j] = tFrame;
			}

			break;
		}
	}

	for (size_t i = 0; i < iSize; i++)
	{
		PBONEKEYFRAME pBoneKeyFrame = new BONEKEYFRAME;

		pBoneKeyFrame->iBoneIndex = pClip->vecBoneKeyFrame[i].iBoneIndex;

		m_vecBoneKeyFrames.push_back(pBoneKeyFrame);

		size_t iFrame = pClip->vecBoneKeyFrame[i].vecKeyFrame.size();
		pBoneKeyFrame->vecKeyFrame.resize(iFrame);

		for (size_t j = 0; j < iFrame; j++)
		{
			PKEYFRAME pFrame = new KEYFRAME;

			pBoneKeyFrame->vecKeyFrame[j] = pFrame;

			fbxsdk::FbxAMatrix matTransform = pClip->vecBoneKeyFrame[i].vecKeyFrame[j].matTransform;

			FbxVector4 vT = matTransform.GetT();
			FbxVector4 vS = matTransform.GetS();
			FbxQuaternion vQ = matTransform.GetQ();

			pFrame->dTime = pClip->vecBoneKeyFrame[i].vecKeyFrame[j].dTime;
			pFrame->vPos.x =	static_cast<float>(vT.mData[0]);
			pFrame->vPos.y =	static_cast<float>(vT.mData[1]);
			pFrame->vPos.z =	static_cast<float>(vT.mData[2]);
			if (bRootMotionRemove)
			{
				pFrame->vPos.x -= vecRootFrames[j].vPos.x;
				pFrame->vPos.y -= 15.f;// vecRootFrames[j].vPos.y;
				pFrame->vPos.z -= vecRootFrames[j].vPos.z;
			}
			pFrame->vScale.x =	static_cast<float>(vS.mData[0]);
			pFrame->vScale.y =	static_cast<float>(vS.mData[1]);
			pFrame->vScale.z =	static_cast<float>(vS.mData[2]);
			pFrame->qRot.x =	static_cast<float>(vQ.mData[0]);
			pFrame->qRot.y =	static_cast<float>(vQ.mData[1]);
			pFrame->qRot.z =	static_cast<float>(vQ.mData[2]);
			pFrame->qRot.w =	static_cast<float>(vQ.mData[3]);

			if (j < m_iFrameCount)
			{
				memcpy_s(&vecFrames[i * m_iFrameCount + j].vPos.x, 16, &pFrame->vPos.x, 12);
				memcpy_s(&vecFrames[i * m_iFrameCount + j].vScale.x, 16, &pFrame->vScale.x, 12);
				memcpy_s(&vecFrames[i * m_iFrameCount + j].vRot.x, 16, &pFrame->qRot.x, 16);
			}
		}
	}

	CreateBufferAndSRV(vecFrames, static_cast<int>(iSize));

	int iLength = static_cast<int>(strlen(m_strFullPath));
	if (iLength)
	{
		char strFileName[_MAX_FNAME] = {};

		_splitpath_s(m_strFullPath, nullptr, 0, nullptr, 0, strFileName, _MAX_FNAME, nullptr, 0);

		for (int i = iLength - 1; i >= 0; i--)
		{
			if (m_strFullPath[i] == '/' || m_strFullPath[i] == '\\')
			{
				size_t iSize = (iLength - i - 1) * sizeof(TCHAR);
				memset(m_strFullPath + i + 1, 0, iSize);
				break;
			}
		}

		char strNew[MAX_PATH] = {};

		strcpy_s(strNew, m_strFullPath);
		strcat_s(strNew, strFileName);
		strcat_s(strNew, ".sqc");

		strcpy_s(m_strFullPath, strNew);

		SaveFullPath(strNew);
	}

	return true;
}

void CAnimationSequence::SetStartFrame(int iFrame)
{
	m_iStartFrame = iFrame;

	m_fStartTime = m_iStartFrame * m_fFrameTime;

	m_iEndFrame = m_iStartFrame + m_iFrameCount;
	m_fEndTime = m_iEndFrame * m_fFrameTime;
}

void CAnimationSequence::SetEndFrame(int iFrame)
{
	m_iEndFrame = iFrame;

	m_fEndTime = m_iEndFrame * m_fFrameTime;

	m_iStartFrame = m_iEndFrame - m_iFrameCount;
	m_fStartTime = m_iStartFrame * m_fFrameTime;
}

bool CAnimationSequence::CreateSequence(const std::string& strKey, bool bLoop, const TCHAR* pFilePath, const std::string& strPathKey, bool bRootMotionRemove)
{
	SetName(strKey);

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFilePath);

	return CreateSequenceFullPath(bLoop, strFullPath,bRootMotionRemove);
}

bool CAnimationSequence::CreateSequenceFullPath(bool bLoop, const TCHAR* pFullPath, bool bRootMotionRemove)
{
	char strFullPath[MAX_PATH] = {};
#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFullPath, lstrlen(pFullPath), NULL, NULL);
#else
	strcpy_s(strFullPath, MAX_PATH, pFullPath);
#endif

	return CreateSequenceFullPath(bLoop, strFullPath,  bRootMotionRemove);
}

bool CAnimationSequence::CreateSequenceFullPath(bool bLoop, const char* pFullPath, bool bRootMotionRemove)
{
	char pExt[_MAX_EXT] = {};

	_splitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, pExt, _MAX_EXT);

	_strupr_s(pExt);

	if (!strcmp(pExt, ".FBX"))
	{
		return LoadFbxAnimation(pFullPath, bLoop,bRootMotionRemove);
	}

	return LoadFullPath(pFullPath, bRootMotionRemove);
}

bool CAnimationSequence::CreateBufferAndSRV(const std::vector<ANIMATIONKEYFRAME>& vecFrames, int iSize)
{
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pSRV);

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.ByteWidth = m_iFrameCount * static_cast<int>(sizeof(ANIMATIONKEYFRAME)) * iSize;
	tDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	tDesc.StructureByteStride = static_cast<int>(sizeof(ANIMATIONKEYFRAME));

	D3D11_SUBRESOURCE_DATA tSub = {};

	tSub.pSysMem = &vecFrames[0];

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tSub, &m_pBuffer)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC tSrv = {};

	tSrv.Format = DXGI_FORMAT_UNKNOWN;
	tSrv.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSrv.BufferEx.FirstElement = 0;
	tSrv.BufferEx.NumElements = m_iFrameCount * iSize;
	tSrv.BufferEx.Flags = 0;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pBuffer, &tSrv, &m_pSRV)))
		return false;

	m_vecBlendMask.resize(iSize);

	CreateBlendMaskSRV(iSize);

	return true;
}

void CAnimationSequence::AddSequence(CAnimationSequence* pSequence)
{
	if (pSequence->m_vecBoneKeyFrames.size() != m_vecBoneKeyFrames.size())
		return;

	size_t iCount = m_vecBoneKeyFrames.size();

	for (size_t i = 0; i < iCount; i++)
	{
		if (m_vecBoneKeyFrames[i]->iBoneIndex == 0 && m_vecBoneKeyFrames[i]->vecKeyFrame.size() == 0)
		{
			if (pSequence->m_vecBoneKeyFrames[i]->vecKeyFrame.size() != 0)
			{
				m_vecBoneKeyFrames[i]->iBoneIndex = pSequence->m_vecBoneKeyFrames[i]->iBoneIndex;
				size_t iFrame = pSequence->m_vecBoneKeyFrames[i]->vecKeyFrame.size();

				for (size_t j = 0; j < iFrame; j++)
				{
					PKEYFRAME pFrame = new KEYFRAME;

					*pFrame = *pSequence->m_vecBoneKeyFrames[i]->vecKeyFrame[j];

					m_vecBoneKeyFrames[i]->vecKeyFrame.push_back(pFrame);
				}
			}
		}
	}
}

void CAnimationSequence::CreateIndentitySequence(int iBone, int iFrame)
{
	for (int i = 0; i < iBone; i++)
	{
		PBONEKEYFRAME pKeyFrame = new BONEKEYFRAME;

		m_vecBoneKeyFrames.push_back(pKeyFrame);

		for (int j = 0; j < iFrame; j++)
		{
			PKEYFRAME pFrame = new KEYFRAME;

			pKeyFrame->vecKeyFrame.push_back(pFrame);

			pFrame->dTime = j / static_cast<float>(iFrame);
			pFrame->vPos = Vector3(10.f, 10.f, 10.f);
			pFrame->qRot = Vector4(0.f, 0.f, 0.f, 1.f);
			pFrame->vScale = Vector3(1.f, 1.f, 1.f);			
		}
	}

	m_fStartTime = 0.f;
	m_fEndTime = 1.f;
	m_fFrameTime = 1.f / iFrame;
	m_fTimeLength = m_fEndTime - m_fStartTime;
	m_iStartFrame = 0;
	m_iEndFrame = iFrame;
	m_iFrameCount = iFrame;
	m_iTimeMode = 30;
}

void CAnimationSequence::RotateFrames()
{

	size_t iSize = m_vecBoneKeyFrames.size();
	std::vector<ANIMATIONKEYFRAME>	vecAni;

	for (size_t i = 0; i < iSize; i++)
	{
		if (m_vecBoneKeyFrames[i]->vecKeyFrame.empty())
			continue;

		size_t iFrames = m_vecBoneKeyFrames[i]->vecKeyFrame.size();

		vecAni.resize(iSize * iFrames);

		PKEYFRAME pLastKeyFrame = *(m_vecBoneKeyFrames[i]->vecKeyFrame.begin() + iFrames - 1);

		for (size_t j = iFrames - 1; j > 0; --j)
		{
			ANIMATIONKEYFRAME tAniFrame = {};
			m_vecBoneKeyFrames[i]->vecKeyFrame[j] = m_vecBoneKeyFrames[i]->vecKeyFrame[j - 1];

			memcpy_s(&tAniFrame.vPos.x, 12, &m_vecBoneKeyFrames[i]->vecKeyFrame[j]->vPos.x, 12);
			memcpy_s(&tAniFrame.vScale.x, 12, &m_vecBoneKeyFrames[i]->vecKeyFrame[j]->vScale.x, 12);
			memcpy_s(&tAniFrame.vRot.x, 16, &m_vecBoneKeyFrames[i]->vecKeyFrame[j]->qRot.x, 16);

			vecAni[i * iFrames + j] = tAniFrame;
		}

		ANIMATIONKEYFRAME tAniFrame = {};

		memcpy_s(&tAniFrame.vPos.x, 12, &pLastKeyFrame->vPos.x, 12);
		memcpy_s(&tAniFrame.vScale.x, 12, &pLastKeyFrame->vScale.x, 12);
		memcpy_s(&tAniFrame.vRot.x, 16, &pLastKeyFrame->qRot.x, 16);

		m_vecBoneKeyFrames[i]->vecKeyFrame[0] = pLastKeyFrame;
		vecAni[i * iFrames] = tAniFrame;
	}

	CreateBufferAndSRV(vecAni, static_cast<int>(iSize));
}

void CAnimationSequence::SetBlendMask(int iBoneIndex, float fBlend)
{
	if (m_vecBlendMask.size() <= iBoneIndex)
		return;

	m_vecBlendMask[iBoneIndex] = fBlend;

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_pBlendBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

	memcpy_s(tSub.pData, m_vecBlendMask.size() * 4, 
		&m_vecBlendMask[0], m_vecBlendMask.size() * 4);

	CONTEXT->Unmap(m_pBlendBuffer, 0);
}

void CAnimationSequence::CreateBlendMaskSRV(int iSize)
{
	SAFE_RELEASE(m_pBlendMaskSRV);
	SAFE_RELEASE(m_pBlendBuffer);

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = iSize * 4;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.StructureByteStride = 4;
	tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA tData = {};

	float* pMask = &m_vecBlendMask[0];

	tData.pSysMem = pMask;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &m_pBlendBuffer)))
		return;

	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};

	tSRVDesc.BufferEx.FirstElement = 0;
	tSRVDesc.BufferEx.Flags = 0;
	tSRVDesc.BufferEx.NumElements = iSize;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pBlendBuffer, &tSRVDesc, &m_pBlendMaskSRV)))
	{
		SAFE_RELEASE(m_pBlendBuffer);
		return;
	}
}

void CAnimationSequence::AddEndCallBack(void(*pFunc)())
{
	m_vecEndNotify.push_back(std::bind(pFunc));
}

void CAnimationSequence::EndCallBack()
{
	size_t iSize = m_vecEndNotify.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecEndNotify[i]();
	}
}

void CAnimationSequence::SetShaderResource(int iRegister)
{
	CONTEXT->CSSetShaderResources(iRegister, 1, &m_pSRV);
}

void CAnimationSequence::ResetShaderResource(int iRegister)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->CSSetShaderResources(iRegister, 1, &pSRV);
}

void CAnimationSequence::SetBlendMaskSRV(int iRegister)
{
	CONTEXT->CSSetShaderResources(iRegister, 1, &m_pBlendMaskSRV);
}

CAnimationSequence* CAnimationSequence::Clone()
{
	return new CAnimationSequence(*this);
}

void CAnimationSequence::Save(FILE* pFile)
{
	size_t	iLength = m_strName.length();
	fwrite(&iLength, sizeof(size_t), 1, pFile);
	fwrite(m_strName.c_str(), sizeof(char), iLength, pFile);

	fwrite(&m_bLoop, sizeof(bool), 1, pFile);
	fwrite(&m_fStartTime, sizeof(float), 1, pFile);
	fwrite(&m_fEndTime, sizeof(float), 1, pFile);
	fwrite(&m_fTimeLength, sizeof(float), 1, pFile);
	fwrite(&m_fFrameTime, sizeof(float), 1, pFile);
	fwrite(&m_fPlayTime, sizeof(float), 1, pFile);
	fwrite(&m_fPlayScale, sizeof(float), 1, pFile);
	fwrite(&m_iStartFrame, sizeof(int), 1, pFile);
	fwrite(&m_iEndFrame, sizeof(int), 1, pFile);
	fwrite(&m_iFrameCount, sizeof(int), 1, pFile);
	fwrite(&m_iTimeMode, sizeof(int), 1, pFile);
	fwrite(&m_iChangeFrame, sizeof(int), 1, pFile);

	size_t	iCount = m_vecBoneKeyFrames.size();

	fwrite(&iCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iCount; ++i)
	{
		fwrite(&m_vecBoneKeyFrames[i]->iBoneIndex, sizeof(int), 1,
			pFile);

		size_t	iFrameCount = m_vecBoneKeyFrames[i]->vecKeyFrame.size();

		fwrite(&iFrameCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iFrameCount; ++j)
		{
			fwrite(&m_vecBoneKeyFrames[i]->vecKeyFrame[j]->dTime, sizeof(double), 1, pFile);
			fwrite(&m_vecBoneKeyFrames[i]->vecKeyFrame[j]->vPos, sizeof(Vector3), 1, pFile);
			fwrite(&m_vecBoneKeyFrames[i]->vecKeyFrame[j]->vScale, sizeof(Vector3), 1, pFile);
			fwrite(&m_vecBoneKeyFrames[i]->vecKeyFrame[j]->qRot, sizeof(Vector4), 1, pFile);
		}
	}
}

bool CAnimationSequence::SaveFullPath(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (pFile)
	{
		Save(pFile);

		fclose(pFile);

		return true;
	}

	return false;
}

void CAnimationSequence::Load(FILE* pFile, bool bRootMotionErase)
{
	SAFE_DELETE_VECLIST(m_vecBoneKeyFrames);

	size_t	iLength = 0;
	fread(&iLength, sizeof(size_t), 1, pFile);
	char	strName[256] = {};
	if (iLength > 0)
	{
		fread(strName, sizeof(char), iLength, pFile);
	}

	fread(&m_bLoop, sizeof(bool), 1, pFile);
	fread(&m_fStartTime, sizeof(float), 1, pFile);
	fread(&m_fEndTime, sizeof(float), 1, pFile);
	fread(&m_fTimeLength, sizeof(float), 1, pFile);
	fread(&m_fFrameTime, sizeof(float), 1, pFile);
	fread(&m_fPlayTime, sizeof(float), 1, pFile);
	fread(&m_fPlayScale, sizeof(float), 1, pFile);
	fread(&m_iStartFrame, sizeof(int), 1, pFile);
	fread(&m_iEndFrame, sizeof(int), 1, pFile);
	fread(&m_iFrameCount, sizeof(int), 1, pFile);
	fread(&m_iTimeMode, sizeof(int), 1, pFile);
	fread(&m_iChangeFrame, sizeof(int), 1, pFile);
	m_iFrameCount = m_iEndFrame - m_iStartFrame + 1;
	size_t	iCount = 0;

	fread(&iCount, sizeof(size_t), 1, pFile);

	std::vector<ANIMATIONKEYFRAME> vecFrame(iCount * m_iFrameCount);

	PBONEKEYFRAME pRootFrame = new BONEKEYFRAME;

	for (size_t i = 0; i < iCount; ++i)
	{
		PBONEKEYFRAME	pBoneKeyFrame = new BONEKEYFRAME;
		m_vecBoneKeyFrames.push_back(pBoneKeyFrame);

		fread(&pBoneKeyFrame->iBoneIndex, sizeof(int), 1,
			pFile);

		size_t	iBoneFrameCount = 0;

		fread(&iBoneFrameCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iBoneFrameCount; ++j)
		{

			PKEYFRAME	pKeyFrame = new KEYFRAME;
			pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);

			fread(&pKeyFrame->dTime, sizeof(double), 1, pFile);
			fread(&pKeyFrame->vPos, sizeof(Vector3), 1, pFile);
			fread(&pKeyFrame->vScale, sizeof(Vector3), 1, pFile);
			fread(&pKeyFrame->qRot, sizeof(Vector4), 1, pFile);

			if (bRootMotionErase)
			{
				if (pRootFrame->vecKeyFrame.size() <= j)
				{
					PKEYFRAME pRootKeyFrame = new KEYFRAME;
					pRootFrame->vecKeyFrame.push_back(pRootKeyFrame);
					pRootKeyFrame->vPos = pKeyFrame->vPos;
				}

				pKeyFrame->vPos -= pRootFrame->vecKeyFrame[j]->vPos;
				pKeyFrame->vPos.x -= 15.f;
				pKeyFrame->vPos.y -= 13.f;
				pKeyFrame->vPos.z -= 123.f;
				
			}

			if (j < m_iFrameCount)
			{
				ANIMATIONKEYFRAME tFrame = {};

				tFrame.vPos = Vector4(pKeyFrame->vPos.x, pKeyFrame->vPos.y, pKeyFrame->vPos.z, 0.f);
				tFrame.vScale = Vector4(pKeyFrame->vScale.x, pKeyFrame->vScale.y, pKeyFrame->vScale.z, 0.f);
				tFrame.vRot = Vector4(pKeyFrame->qRot.x, pKeyFrame->qRot.y, pKeyFrame->qRot.z, pKeyFrame->qRot.w);

				vecFrame[i * m_iFrameCount + j] = tFrame;
			}
		}
	}

	CreateBufferAndSRV(vecFrame, static_cast<int>(iCount));

	if (bRootMotionErase)
	{
		
		SAFE_DELETE_VECLIST(pRootFrame->vecKeyFrame);
	}
	SAFE_DELETE(pRootFrame);
}

bool CAnimationSequence::LoadFullPath(const char* pFullPath, bool bRootMotionRemove)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rb");

	if (pFile)
	{
		Load(pFile, bRootMotionRemove);

		fclose(pFile);

		if (bRootMotionRemove)
		{
			return SaveFullPath(pFullPath);
		}

		return true;
	}

	return false;
}

void CAnimationSequence::SetPlayTime(float fTime)
{
	m_fPlayTime = fTime;
	m_fEndTime = fTime;

	m_fTimeLength = fTime;

	m_fFrameTime = m_fTimeLength / m_iFrameCount;

	size_t iSize = m_vecBoneKeyFrames.size();

	for (size_t i = 0; i < iSize; i++)
	{
		size_t iFrames = m_vecBoneKeyFrames[i]->vecKeyFrame.size();

		for (size_t j = 0; j < iFrames; j++)
		{
			double dTime = j * m_fFrameTime;
			m_vecBoneKeyFrames[i]->vecKeyFrame[j]->dTime = dTime;
		}
	}
	
}

void CAnimationSequence::ShowWindow()
{
	if (ImGui::Begin("Sequence"))
	{
		ImGui::Text(GetName().c_str());

		ImGui::Checkbox("Loop", &m_bLoop);
		ImGui::SliderFloat("Time Length", &m_fTimeLength, 0.f, 5.f);
		ImGui::SliderFloat("Frame Time", &m_fFrameTime, 0.f, 1.f);
		ImGui::SliderFloat("Play Time", &m_fPlayTime, 0.f, m_fTimeLength);
		ImGui::SliderFloat("Play Scale", &m_fPlayScale, 0.f, 5.f);
		ImGui::InputFloat("Start Time", &m_fStartTime);
		ImGui::InputFloat("End Time", &m_fEndTime);
		if (ImGui::InputInt("End Frame", &m_iEndFrame))
		{
			SetEndFrame(m_iEndFrame);
		}
		if (ImGui::InputInt("Start Frame", &m_iStartFrame))
		{
			SetStartFrame(m_iStartFrame);
		}
		ImGui::InputInt("Frame Count", &m_iFrameCount);
		ImGui::SliderInt("Change Frame", &m_iChangeFrame, 0, 100);

		
		for (int i = 0; i < m_vecBlendMask.size(); i++)
		{
			char strBox[MAX_PATH] = {};
			sprintf_s(strBox, "Bone%d", i);
			if (ImGui::InputFloat(strBox, &m_vecBlendMask[i]))
			{
				SetBlendMask(i, m_vecBlendMask[i]);
			}
		}

		if (ImGui::Button("Set Blend Mask"))
		{
			CreateBlendMaskSRV(static_cast<int>(m_vecBlendMask.size()));
		}

	}
	ImGui::End();
}
