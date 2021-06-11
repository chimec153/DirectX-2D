#pragma once

#include "GameEngine.h"

class CViewManager
{
private:
	class CWorldObj*	m_pWorldObj;
	class CDetailObj*	m_pDetailObj;

public:
	class CWorldObj* GetWorldDlg()	const
	{
		return m_pWorldObj;
	}

	class CDetailObj* GetDetailDlg()	const
	{
		return m_pDetailObj;
	}

public:
	bool Init(class CWorldObj*, class CDetailObj*);
	void ClearProtoList();

	DECLARE_SINGLE(CViewManager)
};

