#pragma once

#include "Editor.h"

class CItem
{
private:
	CItem();
	CItem(const CItem& item);
	virtual ~CItem();

private:
	EQUIP_TYPE  m_eType;
	std::string m_strName;
	int         m_iAttackMax;
	int         m_iAttackMin;
	int         m_iArmorMax;
	int         m_iArmorMin;
	int         m_iMagicAttackMax;
	int         m_iMagicAttackMin;
	int			m_iIndexX;
	int			m_iIndexY;
	float		m_fDropRate;
	std::string	m_strTexture;
	int			m_iPrice;
	std::string	m_strMesh;
	Vector3		m_vSocketOffset;
	Vector3		m_vSocketRotation;
	int			m_iContainer;

public:
	const Vector2 GetIndex()	const
	{
		return Vector2(static_cast<float>(m_iIndexX), static_cast<float>(m_iIndexY));
	}
	int GetAttackMax()	const
	{
		return m_iAttackMax;
	}
	int GetAttackMin()	const
	{
		return m_iAttackMin;
	}
	int GetArmorMax()	const
	{
		return m_iArmorMax;
	}
	int GetArmorMin()	const
	{
		return m_iArmorMin;
	}
	int GetMagicAttackMax()	const
	{
		return m_iMagicAttackMax;
	}
	int GetMagicAttackMin()	const
	{
		return m_iMagicAttackMin;
	}
	const std::string& GetTextureName()	const
	{
		return m_strTexture;
	}
	EQUIP_TYPE GetType()	const
	{
		return m_eType;
	}
	float GetDropRate()	const
	{
		return m_fDropRate;
	}
	int GetPrice()	const
	{
		return m_iPrice;
	}
	void SetName(const std::string& strKey)
	{
		m_strName = strKey;
	}
	const std::string& GetName()	const
	{
		return m_strName;
	}
	const std::string& GetMeshName()	const
	{
		return m_strMesh;
	}
	const Vector3& GetSocketOffset()	const
	{
		return m_vSocketOffset;
	}
	const Vector3& GetSocketRotation()	const
	{
		return m_vSocketRotation;
	}
	int GetContainer()	const
	{
		return m_iContainer;
	}

private:
	static std::unordered_map<std::string, class CItem*> m_mapItem;

public:
	static class CItem* FindItem(const std::string& strKey);
	static void CreateItem(const std::string& strKey, EQUIP_TYPE eType, int iAttackMax, int iAttackMin,
		 int iArmorMax, int iArmorMin, int iiMagicAttackMax, int iMagicAttackMin, int x, int y, const std::string& strTexture,
		float fDropRate, int iPrice);

public:
	static void LoadItemTable(const char* pFilePath, const std::string& strPathKey = DATA_PATH);
	static void Destroy();
};

