#pragma once
#include "Collider.h"
class CColliderPixel :
	public CCollider
{
	friend class CGameObject;

public:
	CColliderPixel();
	CColliderPixel(const CColliderPixel& col);
	virtual ~CColliderPixel();

private:
	PixelInfo		m_tInfo;
	std::wstring	m_pFileName;
	std::string		m_strPathName;
	std::wstring	m_pFullPath;

public:
	const PixelInfo& GetInfo()	const;
	bool LoadInfo(const TCHAR* pFileName, const std::string& strPathName = TEXTURE_PATH);
	bool LoadInfoFromFullPath(const TCHAR* pFullPath);
	void SetPixelType(PIXEL_COLLISION_TYPE eType);
	void SetIgnoreColor(unsigned char r, unsigned char g, unsigned char b);
	void SetCheckColor(unsigned char r, unsigned char g, unsigned char b);
	void SetAlpha(unsigned char a);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CComponent> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual bool Collision(class CCollider* pDest);
	virtual bool CollisionMouse(const Vector2& vMousePos);
};

