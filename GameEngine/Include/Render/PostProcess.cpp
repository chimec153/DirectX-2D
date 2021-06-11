#include "PostProcess.h"

CPostProcess::CPostProcess()
{
}

CPostProcess::CPostProcess(const CPostProcess& pro)	:
	CRef(pro)
{
}

CPostProcess::~CPostProcess()
{
}

bool CPostProcess::Init()
{
	return true;
}

void CPostProcess::Start()
{
}

void CPostProcess::Update(float fTime)
{
}

void CPostProcess::PostUpdate(float fTime)
{
}

void CPostProcess::Collision(float fTime)
{
}

void CPostProcess::PreRender(float fTime)
{
}

void CPostProcess::Render(float fTime)
{
}

void CPostProcess::PostRender(float fTime)
{
}
