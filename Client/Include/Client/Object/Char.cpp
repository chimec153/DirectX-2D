#include "Char.h"
#include "UI/UFont.h"
#include "Device.h"

std::unordered_map<CChar*, size_t> CChar::m_mapChat = {};

CChar::CChar()	:
	m_pFont(nullptr)
{
	m_mapChat.insert(std::make_pair(this, m_mapChat.size()));
}

CChar::CChar(const CChar& obj)	:
	CGameObject(obj)
	, m_pFont(FindComponentByType<CUFont>())
{
	m_mapChat.insert(std::make_pair(this, m_mapChat.size()));
}

CChar::~CChar()
{
	std::unordered_map<CChar*, size_t>::iterator iter = m_mapChat.find(this);

	if (iter != m_mapChat.end())
	{
		m_mapChat.erase(iter);
	}
}

void CChar::SetText(const TCHAR* pText)
{
	static_cast<CUFont*>(m_pFont.get())->SetText(pText);
}

bool CChar::Init()
{
	if(!CGameObject::Init())
		return false;

	m_pFont = CreateComponent<CUFont>("Char");

	SetRootComponent(m_pFont);	

	static_cast<CUFont*>(m_pFont.get())->SetRect(-250.f, 0.f, 250.f, 0.f);
	static_cast<CUFont*>(m_pFont.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::LEFT);
	static_cast<CUFont*>(m_pFont.get())->SetWorldPosY(-420.f);
	static_cast<CUFont*>(m_pFont.get())->SetSize(15.f);
	return true;
}

void CChar::Start()
{
	CGameObject::Start();

	Resolution tRS = RESOLUTION;

	std::unordered_map<CChar*, size_t>::iterator iter = m_mapChat.begin();
	std::unordered_map<CChar*, size_t>::iterator iterEnd = m_mapChat.end();

	for (; iter != iterEnd;)
	{
		if (m_mapChat.size() > 13)
		{
			iter->first->Destroy();
			iter = m_mapChat.erase(iter);
			iterEnd = m_mapChat.end();
			continue;
		}

		iter->first->AddWorldPosY(20.f);
		++iter;
	}

	SetWorldPosX(-690.f);
	AddWorldPosY(-20.f);
}

void CChar::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CChar::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CChar::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CChar::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CChar::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CChar::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CChar::Clone()
{
	return std::shared_ptr<CGameObject>(new CChar(*this));
}

void CChar::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CChar::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
