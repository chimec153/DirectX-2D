#include "InputObj.h"

CInputObj::CInputObj()
{
}

CInputObj::~CInputObj()
{
	std::unordered_map<std::string, PInputAxis>::iterator iterAx = m_mapAxis.begin();
	std::unordered_map<std::string, PInputAxis>::iterator iterAxEnd = m_mapAxis.end();

	for (; iterAx != iterAxEnd; ++iterAx)
		GET_SINGLE(CInput)->DeleteBindAxis(iterAx->first, this);

	std::unordered_map<std::string, PInputAction>::iterator iterAc = m_mapAction.begin();
	std::unordered_map<std::string, PInputAction>::iterator iterAcEnd = m_mapAction.end();

	for (; iterAc != iterAcEnd; ++iterAc)
		GET_SINGLE(CInput)->DeleteBindAction(iterAc->first, this);

	SAFE_DELETE_MAP(m_mapAction);
	SAFE_DELETE_MAP(m_mapAxis);
}

PInputAxis CInputObj::FindAxis(const std::string& strTag)
{
	std::unordered_map<std::string, PInputAxis>::iterator iter = m_mapAxis.find(strTag);

	if (iter == m_mapAxis.end())
		return nullptr;

	return iter->second;
}

PInputAction CInputObj::FindAction(const std::string& strTag)
{
	std::unordered_map<std::string, PInputAction>::iterator iter = m_mapAction.find(strTag);

	if (iter == m_mapAction.end())
		return nullptr;

	return iter->second;
}

void CInputObj::CallAxisFunc(const std::string& strTag, float fScale, float fTime)
{
	PInputAxis pAxis = FindAxis(strTag);

	if (!pAxis)
		return;

	size_t iSize = pAxis->vecFunc.size();

	for (size_t i = 0; i < iSize; ++i)
		pAxis->vecFunc[i](fScale, fTime);
}

void CInputObj::CallActionFunc(const std::string& strTag, KEY_TYPE eType, float fTime)
{
	PInputAction pAction = FindAction(strTag);

	if (!pAction)
		return;

	size_t iSize = pAction->vecFunc[eType].size();

	for (size_t i = 0; i < iSize; ++i)
		pAction->vecFunc[eType][i](fTime);
}
