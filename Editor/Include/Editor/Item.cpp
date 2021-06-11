#include "Item.h"
#include "PathManager.h"

std::unordered_map<std::string, CItem*> CItem::m_mapItem;

CItem::CItem()	:
	m_eType(EQUIP_TYPE::NONE)
	, m_strName()
	, m_iAttackMax(0)
	, m_iAttackMin(0)
	, m_iArmorMax(0)
	, m_iArmorMin(0)
	, m_iMagicAttackMax(0)
	, m_iMagicAttackMin(0)
	, m_iIndexX(0)
	, m_iIndexY(0)
	, m_fDropRate(0.f)
	, m_strTexture()
	, m_iPrice(0)
	, m_iContainer(0)
{
}

CItem::CItem(const CItem& item)	
{
	*this = item;
}

CItem::~CItem()
{
}

CItem* CItem::FindItem(const std::string& strKey)
{
	std::unordered_map<std::string, CItem*>::iterator iter = m_mapItem.find(strKey);

	if(iter == m_mapItem.end())
		return nullptr;

	return iter->second;
}

void CItem::CreateItem(const std::string& strKey, EQUIP_TYPE eType, 
	int iAttackMax, int iAttackMin, int iArmorMax, int iArmorMin, 
	int iMagicAttackMax, int iMagicAttackMin,
	int x, int y, const std::string& strTexture, float fDropRate, int iPrice)
{
	CItem* pItem = FindItem(strKey);

	if (pItem)
		return;

	pItem = new CItem;

	pItem->m_strName = strKey;
	pItem->m_eType = eType;
	pItem->m_iAttackMax = iAttackMax;
	pItem->m_iAttackMin = iAttackMin;
	pItem->m_iArmorMax = iArmorMax;
	pItem->m_iArmorMin = iArmorMin;
	pItem->m_iMagicAttackMax = iMagicAttackMax;
	pItem->m_iMagicAttackMin = iMagicAttackMin;
	pItem->m_iIndexX = x;
	pItem->m_iIndexY = y;
	pItem->m_strTexture=strTexture;
	pItem->m_fDropRate = fDropRate;
	pItem->m_iPrice = iPrice;

	m_mapItem.insert(std::make_pair(strKey, pItem));
}

void CItem::LoadItemTable(const char* pFilePath, const std::string& strPathKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strPathKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFilePath);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		char strText[MAX_PATH] = {};

		fgets(strText, MAX_PATH, pFile);

		while (true)
		{
			fgets(strText, MAX_PATH, pFile);

			if (!strcmp(strText, ""))
				break;

			char* pContext = nullptr;

			char* pName = strtok_s(strText, ",", &pContext);

			char* pType = strtok_s(nullptr, ",", &pContext);

			if (!pType)
				break;

			int iType = atoi(pType);

			if (!strcmp(pContext,""))
				break;
			int iAttackMax = atoi(strtok_s(nullptr, ",", &pContext));
			if (!strcmp(pContext, ""))
				break;
			int iAttackMin = atoi(strtok_s(nullptr, ",", &pContext));
			if (!strcmp(pContext, ""))
				break;
			int iArmorMax = atoi(strtok_s(nullptr, ",", &pContext));
			int iArmorMin = atoi(strtok_s(nullptr, ",", &pContext));
			int iMagicAttackMax = atoi(strtok_s(nullptr, ",", &pContext));
			int iMagicAttackMin = atoi(strtok_s(nullptr, ",", &pContext));
			int iX = atoi(strtok_s(nullptr, ",", &pContext));
			int iY = atoi(strtok_s(nullptr, ",", &pContext));
			char* pText = strtok_s(nullptr, ",", &pContext);
			float fDropRate = static_cast<float>(atof(strtok_s(nullptr, ",", &pContext)));
			int  iPrice = atoi(strtok_s(nullptr, ",", &pContext));
			char* pMesh = strtok_s(nullptr, ",", &pContext);
			float fSocketOffsetX = static_cast<float>(atof(strtok_s(nullptr, ",", &pContext)));
			float fSocketOffsetY = static_cast<float>(atof(strtok_s(nullptr, ",", &pContext)));
			float fSocketOffsetZ = static_cast<float>(atof(strtok_s(nullptr, ",", &pContext)));
			float fSocketRotationX = static_cast<float>(atof(strtok_s(nullptr, ",", &pContext)));
			float fSocketRotationY = static_cast<float>(atof(strtok_s(nullptr, ",", &pContext)));
			float fSocketRotationZ = static_cast<float>(atof(strtok_s(nullptr, ",", &pContext)));
			int  iContainer = atoi(strtok_s(nullptr, ",", &pContext));

			if (pMesh[strlen(pMesh) - 1] == '\n')
				pMesh[strlen(pMesh) - 1] = 0;

			CreateItem(pName, static_cast<EQUIP_TYPE>(iType), iAttackMax, iAttackMin,
				iArmorMax, iArmorMin, iMagicAttackMax, iMagicAttackMin, iX, iY, pText, fDropRate, iPrice);

			CItem* pItem = FindItem(pName);

			if (pItem)
			{
				pItem->m_strMesh = pMesh;
				pItem->m_vSocketOffset = Vector3(fSocketOffsetX, fSocketOffsetY, fSocketOffsetZ);
				pItem->m_vSocketRotation = Vector3(fSocketRotationX, fSocketRotationY, fSocketRotationZ);
				pItem->m_iContainer = iContainer;
			}
		}

		fclose(pFile);
	}
}

void CItem::Destroy()
{
	SAFE_DELETE_MAP(m_mapItem);
}
