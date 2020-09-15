#include "Ref.h"

CRef::CRef()	:
	m_iRefCount(1)
{
}

CRef::~CRef()
{
}

void CRef::AddRef()
{
	++m_iRefCount;
}

int CRef::Release()
{
	--m_iRefCount;

	if (m_iRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_iRefCount;
}
