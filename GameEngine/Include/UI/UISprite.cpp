#include "UISprite.h"
#include "../Component/SpriteComponent.h"

CUISprite::CUISprite()
{
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_SPRITE;
}

CUISprite::CUISprite(const CUISprite& ui)	:
	CUIControl(ui)
{
}

CUISprite::~CUISprite()
{
}

bool CUISprite::Init()
{
	if (!CUIControl::Init())
		return false;

	return true;
}

void CUISprite::Start()
{
	CUIControl::Start();
}

void CUISprite::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CUISprite::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CUISprite::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CUISprite::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CUISprite::Render(float fTime)
{
	CUIControl::Render(fTime);
}

void CUISprite::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CUISprite::Clone()
{
	return std::shared_ptr<CComponent>(new CUISprite(*this));
}

void CUISprite::Save(FILE* pFile)
{
	CUIControl::Save(pFile);
}

void CUISprite::Load(FILE* pFile)
{
	CUIControl::Load(pFile);
}
