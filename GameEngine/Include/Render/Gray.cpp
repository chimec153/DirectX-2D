#include "Gray.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/ComputeShader.h"
#include "../Device.h"
#include "../RenderManager.h"

CGray::CGray()
{
}

CGray::CGray(const CGray& pro)	:
	CPostProcess(pro)
{
}

CGray::~CGray()
{
}

bool CGray::Init()
{
	m_pGrayShader = CShaderManager::GetInst()->FindShader("Gray");

	m_tRS = RESOLUTION;

	return true;
}

void CGray::Start()
{
}

void CGray::Update(float fTime)
{
}

void CGray::PostUpdate(float fTime)
{
}

void CGray::Collision(float fTime)
{
}

void CGray::PreRender(float fTime)
{
}

void CGray::Render(float fTime)
{
	static_cast<CComputeShader*>(m_pGrayShader.get())->SetShader();

	CRenderManager::GetInst()->SetShader("Scene", 0, (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE);
	CONTEXT->Dispatch(m_tRS.iWidth / 16 + 1, m_tRS.iHeight / 16 + 1, 1);

	CONTEXT->CSSetShader(NULL, NULL, 0);
}

void CGray::PostRender(float fTime)
{
}
