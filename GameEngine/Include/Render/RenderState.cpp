#include "RenderState.h"

CRenderState::CRenderState()	:
	m_pState(nullptr),
	m_pPrevState(nullptr),
	m_eType(RENDER_STATE_TYPE::BLEND)
{
}

CRenderState::~CRenderState()
{
	SAFE_RELEASE(m_pState);
	SAFE_RELEASE(m_pPrevState);
}

RENDER_STATE_TYPE CRenderState::GetType() const
{
	return m_eType;
}

void CRenderState::ShowWindow()
{
}
