#include "TileObj.h"

CTileObj::CTileObj()
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::TILEOBJ;
}

CTileObj::CTileObj(const CTileObj& obj)	:
	CGameObject(obj)
{
}

CTileObj::~CTileObj()
{
}

bool CTileObj::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CTileObj::Start()
{
	CGameObject::Start();
}

void CTileObj::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CTileObj::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CTileObj::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CTileObj::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CTileObj::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CTileObj::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CTileObj::Clone()
{
	return std::shared_ptr<CGameObject>(new CTileObj(*this));
}

void CTileObj::Save(FILE * pFile)
{
	CGameObject::Save(pFile);
}

void CTileObj::Load(FILE * pFile)
{
	CGameObject::Load(pFile);
}
