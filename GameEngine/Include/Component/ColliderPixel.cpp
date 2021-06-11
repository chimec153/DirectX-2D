#include "ColliderPixel.h"
#include "../PathManager.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture.h"
#include "Collision.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Material.h"
#include "ColliderLine.h"
#include "ColliderCircle.h"
#include "ColliderOBB2D.h"
#include "ColliderRect.h"
#include "ColliderPoint.h"

CColliderPixel::CColliderPixel()	:
	m_tInfo()
{
	m_eColType = COLLIDER_TYPE::CT_PIXEL;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_PIXEL;

	m_b2D = true;
}

CColliderPixel::CColliderPixel(const CColliderPixel& col)	:
	CCollider(col)
{
	m_tInfo = col.m_tInfo;

	memcpy(m_tInfo.pPixel, col.m_tInfo.pPixel, m_tInfo.iSize);
}

CColliderPixel::~CColliderPixel()
{
}

const PixelInfo& CColliderPixel::GetInfo() const
{
	return m_tInfo;
}

bool CColliderPixel::LoadInfo(const TCHAR* pFileName, const std::string& strPathName)
{
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	m_pFileName = pFileName;

	m_strPathName = strPathName;

	return LoadInfoFromFullPath(strFullPath);
}

bool CColliderPixel::LoadInfoFromFullPath(const TCHAR* pFullPath)
{
	m_pFullPath = pFullPath;

	TCHAR strExt[_MAX_EXT] = {};

	_wsplitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

	char pExt[_MAX_EXT] = {};

	WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

	_strupr_s(pExt);

	ScratchImage* pImage = new ScratchImage;

	if (strcmp(pExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
			
	}

	else if (strcmp(pExt, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(pFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	size_t iSize = pImage->GetPixelsSize();

	const Image* pPixel = pImage->GetImages();

	m_tInfo.iWidth = pPixel->width;
	m_tInfo.iHeight = pPixel->height;

	if (iSize / 4 != m_tInfo.iWidth * m_tInfo.iHeight)
	{
		SAFE_DELETE(pImage);
		return false;
	}

	m_tInfo.pPixel = new ColorInfo[iSize / 4];

	memcpy(m_tInfo.pPixel, pImage->GetPixels(), iSize);

	if (strcmp(pExt, ".BMP") == 0)
	{
		PColorInfo pPix = new ColorInfo[m_tInfo.iWidth + 1];

		for (size_t i = 0; i < m_tInfo.iHeight / 2; ++i)
		{
			memcpy(pPix, m_tInfo.pPixel + m_tInfo.iWidth * i, m_tInfo.iWidth * sizeof(ColorInfo));
			memcpy(m_tInfo.pPixel + m_tInfo.iWidth  * i, m_tInfo.pPixel + (m_tInfo.iHeight - 1 - i) * m_tInfo.iWidth , m_tInfo.iWidth * sizeof(ColorInfo));
			memcpy(m_tInfo.pPixel + (m_tInfo.iHeight - 1 - i) * m_tInfo.iWidth, pPix, m_tInfo.iWidth * sizeof(ColorInfo));
		}

		delete[] pPix;
	}

	SAFE_DELETE(pImage);

	return true;
}

void CColliderPixel::SetPixelType(PIXEL_COLLISION_TYPE eType)
{
	m_tInfo.eType = eType;
}

void CColliderPixel::SetIgnoreColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_tInfo.tColor.r = r;
	m_tInfo.tColor.g = g;
	m_tInfo.tColor.b = b;
}

void CColliderPixel::SetCheckColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_tInfo.tCheckColor.r = r;
	m_tInfo.tCheckColor.g = g;
	m_tInfo.tCheckColor.b = b;
}

void CColliderPixel::SetAlpha(unsigned char a)
{
	m_tInfo.tColor.a = a;
}

bool CColliderPixel::Init()
{
	if (!CCollider::Init())
		return false;

#ifdef _DEBUG
	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("Collider2DRect");
#endif

	SetInheritRotX(false);
	SetInheritRotY(false);
	SetInheritRotZ(false);

	return true;
}

void CColliderPixel::Start()
{
	CCollider::Start();
}

void CColliderPixel::Update(float fTime)
{
	CCollider::Update(fTime);

	Vector3 vPos = GetWorldPos();

	m_tInfo.tRt.fL = vPos.x - m_tInfo.iWidth / 2.f;
	m_tInfo.tRt.fR = vPos.x + m_tInfo.iWidth / 2.f;
	m_tInfo.tRt.fB = vPos.y - m_tInfo.iHeight / 2.f;
	m_tInfo.tRt.fT = vPos.y + m_tInfo.iHeight / 2.f;

	m_vMin.x = m_tInfo.tRt.fL;
	m_vMax.x = m_tInfo.tRt.fR;
	m_vMin.y = m_tInfo.tRt.fB;
	m_vMax.y = m_tInfo.tRt.fT;

	SetWorldScale((float)m_tInfo.iWidth, (float)m_tInfo.iHeight, 0.f);
}

void CColliderPixel::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);
}

void CColliderPixel::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderPixel::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderPixel::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderPixel::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

std::shared_ptr<CComponent> CColliderPixel::Clone()
{
	return std::shared_ptr<CComponent> (new CColliderPixel(*this));
}

void CColliderPixel::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tInfo.iWidth, sizeof(size_t), 1, pFile);
	fwrite(&m_tInfo.iHeight, sizeof(size_t), 1, pFile);
	fwrite(&m_tInfo.iSize, 4, 1, pFile);
	fwrite(&m_tInfo.eType, 4, 1, pFile);
	fwrite(&m_tInfo.tColor, sizeof(ColorInfo), 1, pFile);
	fwrite(&m_tInfo.tCheckColor, sizeof(ColorInfo), 1, pFile);
	fwrite(&m_tInfo.tRt, sizeof(RectInfo), 1, pFile);

	int iLength = (int)m_pFileName.length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_pFileName.c_str(), 2, iLength, pFile);

	iLength = (int)m_strPathName.length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strPathName.c_str(), 1, iLength, pFile);

	iLength = (int)m_pFullPath.length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_pFullPath.c_str(), 2, iLength, pFile);
}

void CColliderPixel::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	fread(&m_tInfo.iWidth, sizeof(size_t), 1, pFile);
	fread(&m_tInfo.iHeight, sizeof(size_t), 1, pFile);
	fread(&m_tInfo.iSize, 4, 1, pFile);
	fread(&m_tInfo.eType, 4, 1, pFile);
	fread(&m_tInfo.tColor, sizeof(ColorInfo), 1, pFile);
	fread(&m_tInfo.tCheckColor, sizeof(ColorInfo), 1, pFile);
	fread(&m_tInfo.tRt, sizeof(RectInfo), 1, pFile);

	int iLength = 0;
	wchar_t strFileName[256] = {};

	fread(&iLength, 4, 1, pFile);
	fread(strFileName, 2, iLength, pFile);

	m_pFileName = strFileName;

	fread(&iLength, 4, 1, pFile);

	char pPathName[256] = {};

	fread(pPathName, 1, iLength, pFile);

	m_strPathName = pPathName;

	fread(&iLength, 4, 1, pFile);

	memset(strFileName, 0, 2 * 256);

	fread(strFileName, 2, iLength, pFile);

	m_pFullPath = strFileName;

	if (m_pFileName == L"")
		LoadInfoFromFullPath(m_pFullPath.c_str());

	else
		LoadInfo(m_pFileName.c_str(), m_strPathName);
}

bool CColliderPixel::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionPixelToRect((CColliderPixel*)this, (CColliderRect*)pDest);
	case COLLIDER_TYPE::CT_SPHERE2D:
		return CCollision::CollisionPixelToSphere2D((CColliderPixel*)this, (CColliderCircle*)pDest);
	case COLLIDER_TYPE::CT_POINT:
		return CCollision::CollisionPixelToPoint((CColliderPixel*)this, (CColliderPoint*)pDest);
	case COLLIDER_TYPE::CT_OBB2D:
		return CCollision::CollisionPixelToOBB2D((CColliderPixel*)this, (CColliderOBB2D*)pDest);
	case COLLIDER_TYPE::CT_LINE:
		return CCollision::CollisionPixelToLine((CColliderPixel*)this, (CColliderLine*)pDest);
	}

	return false;
}

bool CColliderPixel::CollisionMouse(const Vector2& vMousePos)
{
	Vector3 vCrs;

	if (CCollision::CollisionPixelToPoint(vCrs, GetInfo(), Vector3(vMousePos.x, vMousePos.y, 0.f)))
	{
		SetCross(vCrs);

		return true;
	}

	return false;
}
