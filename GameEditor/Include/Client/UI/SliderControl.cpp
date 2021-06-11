#include "SliderControl.h"
#include "Component/ColliderRect.h"
#include "UI/UIButton.h"
#include "UI/Slider.h"
#include "UI/UIImage.h"

CSliderControl::CSliderControl()
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::SLIDERCONTROL;
}

CSliderControl::CSliderControl(const CSliderControl& ui)	:
	CGameObject(ui)
{
}

CSliderControl::~CSliderControl()
{
}


bool CSliderControl::Init()
{
	if (!CGameObject::Init())
		return false;



	return true;
}

void CSliderControl::Start()
{
	CGameObject::Start();
}

void CSliderControl::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CSliderControl::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CSliderControl::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CSliderControl::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CSliderControl::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CSliderControl::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CSliderControl::Clone()
{
	return std::shared_ptr<CGameObject>(new CSliderControl(*this));
}

void CSliderControl::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CSliderControl::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
