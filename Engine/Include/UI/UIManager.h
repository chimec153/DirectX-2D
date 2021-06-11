#pragma once

#include "../GameEngine.h"

class CUIManager
{
private:
	Matrix							m_matProj;

public:
	const Matrix& GetProjMat()	const;
	void SetProjMat(const Matrix& mat);

public:
	bool Init();

	DECLARE_SINGLE(CUIManager)
};
