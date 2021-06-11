#pragma once

#include "../Ref.h"

class CRenderState :
	public CRef
{
	friend class CRenderManager;

protected:
	CRenderState();
	virtual ~CRenderState();

protected:
	ID3D11DeviceChild* m_pState;
	ID3D11DeviceChild* m_pPrevState;
	RENDER_STATE_TYPE	m_eType;

public:
	RENDER_STATE_TYPE GetType()	const;

public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;

public:
	virtual void ShowWindow() = 0;
};