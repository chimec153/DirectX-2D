#include "BoneSocket.h"
#include "../Component/SceneComponent.h"
#include "../Component/Transform.h"
#include "Animation.h"

CBoneSocket::CBoneSocket()	:
	m_iBoneIndex(-1)
	, m_strBone()
	, m_vOffset()
	, m_vRotation()
{
}

CBoneSocket::CBoneSocket(const CBoneSocket& socket)	:
	CRef(socket)
	, m_iBoneIndex(socket.m_iBoneIndex)
	, m_strBone(socket.m_strBone)
	, m_vOffset(socket.m_vOffset)
	, m_vRotation(socket.m_vRotation)
{
}

CBoneSocket::~CBoneSocket()
{
}

bool CBoneSocket::Init(const std::string& strKey, int iBoneIndex, const std::string& strBone)
{
	SetName(strKey);
	m_strBone = strBone;
	m_iBoneIndex = iBoneIndex;

	return true;
}

void CBoneSocket::SetOffset(const Vector3& vOffset)
{
	m_vOffset = vOffset;
}

void CBoneSocket::SetRotation(const Vector3& vRotation)
{
	m_vRotation = vRotation;
}

void CBoneSocket::AddComponent(const std::shared_ptr<class CComponent>& pCom)
{
	m_ComponentList.push_back(pCom);
}

void CBoneSocket::DeleteComponent(const std::shared_ptr<class CComponent>& pCom)
{
	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ComponentList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == pCom)
		{
			static_cast<CSceneComponent*>((*iter).get())->GetTransform()->DisableBoneMatrix();
			m_ComponentList.erase(iter);
			return;
		}
	}
}

void CBoneSocket::Update(float fTime, const Matrix& matBone)
{
	Matrix matRot = {};
	Matrix matTrans = {};

	matRot.Rotation(m_vRotation);
	matTrans.Translate(m_vOffset);

	Matrix matCom = matRot * matTrans * matBone;

	std::list<std::shared_ptr<CComponent>>::iterator iter = m_ComponentList.begin();
	std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>((*iter).get())->GetTransform();

		pTransform->SetBoneMatrix(matCom);
		pTransform->SetWorldMatrix();
	}
}

CBoneSocket* CBoneSocket::Clone()
{
	return new CBoneSocket(*this);
}

void CBoneSocket::Save(FILE* pFile)
{
	CRef::Save(pFile);

	fwrite(&m_iBoneIndex, 4, 1, pFile);
	int iBone = static_cast<int>(m_strBone.length());

	fwrite(&iBone, 4, 1, pFile);
	if (iBone > 0)
	{
		fwrite(m_strBone.c_str(), 1, iBone, pFile);
	}

	fwrite(&m_vOffset, 12, 1, pFile);
	fwrite(&m_vRotation, 12, 1, pFile);

	
}

void CBoneSocket::Load(FILE* pFile)
{
	CRef::Load(pFile);

	fread(&m_iBoneIndex, 4, 1, pFile);
	int iBone = 0;

	fread(&iBone, 4, 1, pFile);
	if (iBone > 0)
	{
		char strBone[MAX_PATH] = {};
		fread(strBone, 1, iBone, pFile);
	}

	fread(&m_vOffset, 12, 1, pFile);
	fread(&m_vRotation, 12, 1, pFile);
}

void CBoneSocket::ShowWindow()
{
	CRef::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		ImGui::Text("Bone Socket");

		
		ImGui::Text("Bone Name: %s", m_strBone.c_str());
		ImGui::InputInt("Bone Index", &m_iBoneIndex);
		ImGui::InputFloat3("Offset", &m_vOffset.x);
		ImGui::InputFloat3("Rotation", &m_vRotation.x);

		std::vector<const char*> vecComponent;

		int iSize = static_cast<int>(m_ComponentList.size());

		std::list<std::shared_ptr<CComponent>>::iterator iter = m_ComponentList.begin();
		std::list<std::shared_ptr<CComponent>>::iterator iterEnd = m_ComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			vecComponent.push_back((*iter)->GetName().c_str());
		}

		static int iSelect = -1;

		if (iSize > 0)
		{
			ImGui::ListBox("Component List", &iSelect, &vecComponent[0], iSize);
		}
	}
	ImGui::End();
}
