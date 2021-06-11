#include "Tree.h"
#include "Component/Renderer.h"
#include "../Editor3D.h"

CTree::CTree()
{
	m_iObjType = static_cast<int>(EDITOR_OBJECT_TYPE::TREE);
}

CTree::CTree(const CTree& tree)	:
	CGameObject(tree)
{
}

CTree::~CTree()
{
}

bool CTree::Init()
{
	if (!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> _pTree = CreateComponent<CSceneComponent>("TreeCom");

	SetRootComponent(_pTree);

	static_cast<CSceneComponent*>(_pTree.get())->SetWorldScale(1.f, 1.f, 1.f);
	static_cast<CSceneComponent*>(_pTree.get())->SetMesh("Tree");
	//static_cast<CSceneComponent*>(_pTree.get())->SetShader("Alpha");
	//static_cast<CSceneComponent*>(_pTree.get())->SetShader("EmvGClipAlpha", 0, 1);
	static_cast<CSceneComponent*>(_pTree.get())->AddRenderState(CULL_NONE, 0, 1);
	static_cast<CSceneComponent*>(_pTree.get())->AddRenderState("DepthNoWrite", 0, 1);

	return true;
}

void CTree::Start()
{
	CGameObject::Start();
}

void CTree::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CTree::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CTree::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CTree::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CTree::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CTree::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CTree::Clone()
{
	return std::shared_ptr<CGameObject>(new CTree(*this));
}

void CTree::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CTree::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
