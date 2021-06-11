#include "Alistar.h"

CAlistar::CAlistar()
{
}

CAlistar::CAlistar(const CAlistar& cham)	:
	CGameObject(cham)
{
}

CAlistar::~CAlistar()
{
}

bool CAlistar::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CAlistar::Start()
{
	CGameObject::Start();
}

void CAlistar::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CAlistar::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CAlistar::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CAlistar::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CAlistar::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CAlistar::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CAlistar::Clone()
{
	return std::shared_ptr<CGameObject>(new CAlistar(*this));
}

void CAlistar::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CAlistar::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
