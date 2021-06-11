#include "Ref.h"
#include "UI/imgui/imgui.h"

CRef::CRef()	:
	m_iRefCount(1)
	, m_strName()
	, m_bActive(true)
	, m_bEnable(true)
{
}

CRef::CRef(const CRef& ref)
{
	*this = ref;
	m_iRefCount = 1;
}

CRef::~CRef()
{
}

int CRef::GetRef() const
{
	return m_iRefCount;
}

void CRef::AddRef()
{
	m_iRefCount++;
}

int CRef::Release()
{
	if (--m_iRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_iRefCount;
}

void CRef::Save(FILE* pFile)
{
	int iLength = static_cast<int>(m_strName.length());

	fwrite(&m_iRefCount, 4, 1, pFile);
	fwrite(&iLength, 4, 1, pFile);
	if (iLength > 0)
	{
		fwrite(m_strName.c_str(), 1, iLength, pFile);
	}
	fwrite(&m_bActive, 1, 1, pFile);
}

void CRef::Load(FILE* pFile)
{
	int iLength = 0;
	fread(&m_iRefCount, 4, 1, pFile);
	m_iRefCount = 1;
	fread(&iLength, 4, 1, pFile);
	if (iLength > 0)
	{
		char strName[MAX_PATH] = {};

		fread(strName, 1, iLength, pFile);
		m_strName = strName;
	}
	fread(&m_bActive, 1, 1, pFile);
}

void CRef::ShowWindow()
{
	const char* pName = m_strName.c_str();
	if (m_strName == "")
	{
		pName = "NULL";
	}

	if (ImGui::Begin(pName))
	{
		ImGui::Text("Reference Counter");
		char strName[MAX_PATH] = {};
		memcpy_s(strName, MAX_PATH, m_strName.c_str(), m_strName.length());
		if(ImGui::InputText("Name", strName,m_strName.length()))
		{
			SetName(strName);
		}

		if (ImGui::Checkbox("Enable", &m_bEnable))
		{
			SetEnable(m_bEnable);
		}
		if (ImGui::Checkbox("Active", &m_bActive))
		{
			Destroy();
		}
		ImGui::Text("Reference Count %d", m_iRefCount);
	}
	ImGui::End();
}
