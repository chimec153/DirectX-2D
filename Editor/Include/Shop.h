#pragma once
#include "Object/GameObject.h"
#include "Editor/EditorFlags.h"
class CShop	:
	public CGameObject
{
public:
	CShop();
	CShop(const CShop& ui);
	virtual ~CShop();

private:
	class CItem* m_pItems[MAX_SHOP];
	int			m_iSelect;

public:
	void SetItem(int iSlot, const std::string& strKey);
	void BuyItem(int iSlot);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CGameObject> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void ButtonCallBack(class CUIButton* pButton);
};

