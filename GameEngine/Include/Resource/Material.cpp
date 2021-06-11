#include "Material.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "../Resource/Texture.h"
#include "ResourceManager.h"
#include "../UI/imgui/imgui.h"
#include "../UI/imgui/ImGuiFileDialog.h"
#include "../PathManager.h"

CMaterial::CMaterial()	:
	m_pShader(nullptr),
	m_iKey(0)
{
	memset(&m_tCBuffer, 0, sizeof(m_tCBuffer));
	m_tCBuffer.vDif = Vector4::White;
	m_tCBuffer.vEmb = Vector4::White;
	m_tCBuffer.vSpec = Vector4::White;
}

CMaterial::CMaterial(const CMaterial& material)	:
	CRef(material)
{
	m_tCBuffer = material.m_tCBuffer;
	m_pShader = material.m_pShader;
	m_iKey = material.m_iKey;

	m_vecTextureInfo.clear();

	size_t iSize = material.m_vecTextureInfo.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		PMaterialTextureInfo pInfo = new MaterialTextureInfo;

		*pInfo = *material.m_vecTextureInfo[i];

		m_vecTextureInfo.push_back(pInfo);
	}
}

CMaterial::~CMaterial()
{
	SAFE_DELETE_VECLIST(m_vecTextureInfo);
}

void CMaterial::SetTexture(LINK_TYPE eLink, std::shared_ptr<CTexture> pTexture, int iShaderType, int iRegister)
{
	PMaterialTextureInfo pInfo = new MaterialTextureInfo;

	pInfo->eLink = eLink;
	pInfo->pTexture = pTexture;

	if (pTexture)
		pTexture->AddRef();

	pInfo->iShaderType = iShaderType;
	pInfo->iRegister = iRegister;

	switch (eLink)
	{
	case LINK_TYPE::LT_DIF:
		m_tCBuffer.iDifTex = 1;
		pInfo->iRegister = 0;
		break;
	case LINK_TYPE::LT_NOR:
		m_tCBuffer.iNrmTex = 1;
		pInfo->iRegister = 1;
		break;
	case LINK_TYPE::LT_SPE:
		m_tCBuffer.iSpecTex = 1;
		pInfo->iRegister = 2;
		break;
	case LINK_TYPE::LT_EMI:
		m_tCBuffer.iEmiTex = 1;
		pInfo->iRegister = 3;
		break;
	case LINK_TYPE::DIF_ARRAY:
		pInfo->iRegister = 30;
		break;
	case LINK_TYPE::NRM_ARRAY:
		pInfo->iRegister = 31;
		break;
	case LINK_TYPE::SPC_ARRAY:
		pInfo->iRegister = 32;
		break;
	case LINK_TYPE::EMV_ARRAY:
		pInfo->iRegister = 33;
		break;
	case LINK_TYPE::LT_CUSTOM:
		break;
	}

	m_vecTextureInfo.push_back(pInfo);
}

void CMaterial::SetTexture(LINK_TYPE eLink, const std::string& strTag, int iShaderType, int iRegister)
{
	std::shared_ptr<CTexture> pTex = GET_SINGLE(CResourceManager)->FindTexture(strTag);

	if (!pTex)
	{
#ifdef _DEBUG
		assert(false);
#endif
		return;
	}
		

	PMaterialTextureInfo pInfo = new MaterialTextureInfo;

	pInfo->eLink = eLink;
	pInfo->pTexture = pTex;
	pInfo->iShaderType = iShaderType;
	pInfo->iRegister = iRegister;

	switch (eLink)
	{
	case LINK_TYPE::LT_DIF:
		m_tCBuffer.iDifTex = 1;
		pInfo->iRegister = 0;
		break;
	case LINK_TYPE::LT_NOR:
		m_tCBuffer.iNrmTex = 1;
		pInfo->iRegister = 1;
		break;
	case LINK_TYPE::LT_SPE:
		m_tCBuffer.iSpecTex = 1;
		pInfo->iRegister = 2;
		break;
	case LINK_TYPE::LT_EMI:
		m_tCBuffer.iEmiTex = 1;
		pInfo->iRegister = 3;
		break;
	case LINK_TYPE::DIF_ARRAY:
		pInfo->iRegister = 30;
		break;
	case LINK_TYPE::NRM_ARRAY:
		pInfo->iRegister = 31;
		break;
	case LINK_TYPE::SPC_ARRAY:
		pInfo->iRegister = 32;
		break;
	case LINK_TYPE::EMV_ARRAY:
		pInfo->iRegister = 33;
		break;
	case LINK_TYPE::LT_CUSTOM:
		break;
	}

	m_vecTextureInfo.push_back(pInfo);
}

void CMaterial::SetTexture(LINK_TYPE eLink, const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath, int iShaderType, int iRegister)
{
	GET_SINGLE(CResourceManager)->LoadTexture(strTag, pFileName, strRootPath);

	std::shared_ptr<CTexture> pTex = GET_SINGLE(CResourceManager)->FindTexture(strTag);

	if (!pTex)
		return;

	PMaterialTextureInfo pInfo = new MaterialTextureInfo;

	pInfo->eLink = eLink;
	pInfo->pTexture = pTex;
	pInfo->iShaderType = iShaderType;
	pInfo->iRegister = iRegister;

	switch (eLink)
	{
	case LINK_TYPE::LT_DIF:
		m_tCBuffer.iDifTex = 1;
		pInfo->iRegister = 0;
		break;
	case LINK_TYPE::LT_NOR:
		m_tCBuffer.iNrmTex = 1;
		pInfo->iRegister = 1;
		break;
	case LINK_TYPE::LT_SPE:
		m_tCBuffer.iSpecTex = 1;
		pInfo->iRegister = 2;
		break;
	case LINK_TYPE::LT_EMI:
		m_tCBuffer.iEmiTex = 1;
		pInfo->iRegister = 3;
		break;
	case LINK_TYPE::DIF_ARRAY:
		pInfo->iRegister = 30;
		break;
	case LINK_TYPE::NRM_ARRAY:
		pInfo->iRegister = 31;
		break;
	case LINK_TYPE::SPC_ARRAY:
		pInfo->iRegister = 32;
		break;
	case LINK_TYPE::EMV_ARRAY:
		pInfo->iRegister = 33;
		break;
	case LINK_TYPE::LT_CUSTOM:
		break;
	}

	m_vecTextureInfo.push_back(pInfo);
}

void CMaterial::SetTexture(LINK_TYPE eLink, const std::string& strTag, const TCHAR* pFileName, int iShaderType, int iRegister)
{
	GET_SINGLE(CResourceManager)->LoadTextureFromFullPath(strTag, pFileName);

	std::shared_ptr<CTexture> pTex = GET_SINGLE(CResourceManager)->FindTexture(strTag);

	if (!pTex)
		return;

	PMaterialTextureInfo pInfo = new MaterialTextureInfo;

	pInfo->eLink = eLink;
	pInfo->pTexture = pTex;
	pInfo->iShaderType = iShaderType;
	pInfo->iRegister = iRegister;

	switch (eLink)
	{
	case LINK_TYPE::LT_DIF:
		m_tCBuffer.iDifTex = 1;
		pInfo->iRegister = 0;
		break;
	case LINK_TYPE::LT_NOR:
		m_tCBuffer.iNrmTex = 1;
		pInfo->iRegister = 1;
		break;
	case LINK_TYPE::LT_SPE:
		m_tCBuffer.iSpecTex = 1;
		pInfo->iRegister = 2;
		break;
	case LINK_TYPE::LT_EMI:
		m_tCBuffer.iEmiTex = 1;
		pInfo->iRegister = 3;
		break;
	case LINK_TYPE::DIF_ARRAY:
		pInfo->iRegister = 30;
		break;
	case LINK_TYPE::NRM_ARRAY:
		pInfo->iRegister = 31;
		break;
	case LINK_TYPE::SPC_ARRAY:
		pInfo->iRegister = 32;
		break;
	case LINK_TYPE::EMV_ARRAY:
		pInfo->iRegister = 33;
		break;
	case LINK_TYPE::LT_CUSTOM:
		break;
	}

	m_vecTextureInfo.push_back(pInfo);
}

void CMaterial::ClearTexture()
{
	SAFE_DELETE_VECLIST(m_vecTextureInfo);
}

size_t CMaterial::GetInfoSize() const
{
	return m_vecTextureInfo.size();
}

PMaterialTextureInfo CMaterial::GetTexInfo(int idx) const
{
	return m_vecTextureInfo[idx];
}

void CMaterial::SetShader(const std::string& strName)
{
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(strName);
}

void CMaterial::SetDiffuseColor(const Vector4& v)
{
	m_tCBuffer.vDif = v;
}

void CMaterial::SetDiffuseColor(float x, float y, float z, float a)
{
	SetDiffuseColor(Vector4(x, y, z, a));
}

void CMaterial::SetAmbientColor(const Vector4& v)
{
	m_tCBuffer.vEmb = v;
}

void CMaterial::SetAmbientColor(float x, float y, float z, float a)
{
	SetAmbientColor(Vector4(x, y, z, a));
}

void CMaterial::SetSpecularColor(const Vector4 & v)
{
	m_tCBuffer.vSpec = v;
}

void CMaterial::SetSpecularColor(float x, float y, float z, float a)
{
	SetSpecularColor(Vector4(x, y, z, a));
}

void CMaterial::SetEmissiveColor(const Vector4& v)
{
	m_tCBuffer.vEmi = v;
}

void CMaterial::SetEmissiveColor(float x, float y, float z, float a)
{
	m_tCBuffer.vEmi = Vector4(x, y, z, a);
}

void CMaterial::SetSpecularPower(float fPower)
{
	m_tCBuffer.vSpec.w = fPower;
}

void CMaterial::EnableBump()
{
	m_tCBuffer.iBump = true;
}

void CMaterial::EnableAnimation()
{
	m_tCBuffer.iAnimation = true;
}

void CMaterial::SetAnimation(bool bAni)
{
	m_tCBuffer.iAnimation = bAni;
}

void CMaterial::SetEnableBump(bool bBump)
{
	m_tCBuffer.iBump = bBump;
}

void CMaterial::SetDiffuseTex(bool bEnable)
{
	m_tCBuffer.iDifTex = bEnable;
}

void CMaterial::SetAmbientTex(bool bEnable)
{
	m_tCBuffer.iNrmTex = bEnable;
}

void CMaterial::SetSpecularTex(bool bEnable)
{
	m_tCBuffer.iSpecTex = bEnable;
}

void CMaterial::SetEmissiveTex(bool bTex)
{
	m_tCBuffer.iEmiTex = bTex;
}

void CMaterial::SetDecal(bool bDecal)
{
	m_tCBuffer.iDecal = bDecal;
}

const Vector4& CMaterial::GetDiffuseColor() const
{
	return m_tCBuffer.vDif;
}

std::shared_ptr<CShader> CMaterial::GetShader() const
{
	return m_pShader;
}

void CMaterial::Render(float fTime)
{
	if (m_pShader)
		m_pShader->SetShader();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Material", &m_tCBuffer);

	size_t iSize = m_vecTextureInfo.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecTextureInfo[i]->pTexture->SetShader(m_vecTextureInfo[i]->iRegister, m_vecTextureInfo[i]->iShaderType);
}

void CMaterial::SetMaterialNoShader()
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Material", &m_tCBuffer);

	size_t iSize = m_vecTextureInfo.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecTextureInfo[i]->pTexture->SetShader(
			m_vecTextureInfo[i]->iRegister, 
			m_vecTextureInfo[i]->iShaderType);
}

std::shared_ptr<CMaterial> CMaterial::Clone()
{
	return std::shared_ptr<CMaterial>(new CMaterial(*this));
}

void CMaterial::Save(FILE* pFile)
{
	CRef::Save(pFile);

	bool bShader = false;

	if (m_pShader)
	{
		bShader = true;
	}

	fwrite(&bShader, 1, 1, pFile);

	if (m_pShader)
	{
		std::string strTag = m_pShader->GetName();

		int iLength = (int)strTag.length();

		fwrite(&iLength, sizeof(int), 1, pFile);

		if (iLength > 0)
		{
			fwrite(strTag.c_str(), sizeof(char), iLength, pFile);
		}
	}

	fwrite(&m_tCBuffer, sizeof(ShaderCBuffer), 1, pFile);

	size_t iSize = m_vecTextureInfo.size();

	fwrite(&iSize, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iSize; i++)
	{
		fwrite(&m_vecTextureInfo[i]->eLink, 4, 1, pFile);
		fwrite(&m_vecTextureInfo[i]->iRegister, 4, 1, pFile);
		fwrite(&m_vecTextureInfo[i]->iShaderType, 4, 1, pFile);

		m_vecTextureInfo[i]->pTexture->Save(pFile);
	}
}

void CMaterial::Load(FILE* pFile)
{
	CRef::Load(pFile);

	bool bShader = false;

	fread(&bShader, 1, 1, pFile);

	if (bShader)
	{
		int iLength = 0;

		fread(&iLength, sizeof(int), 1, pFile);

		char strTag[256] = {};
		if (iLength > 0)
		{
			fread(strTag, sizeof(char), iLength, pFile);
		}

		std::string strShader = strTag;

		SetShader(strShader);
	}

	fread(&m_tCBuffer, sizeof(ShaderCBuffer), 1, pFile);

	SAFE_DELETE_VECLIST(m_vecTextureInfo);

	size_t iSize = 0;

	fread(&iSize, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iSize; i++)
	{
		PMaterialTextureInfo pInfo = new MaterialTextureInfo;

		fread(&pInfo->eLink, 4, 1, pFile);
		fread(&pInfo->iRegister, 4, 1, pFile);
		fread(&pInfo->iShaderType, 4, 1, pFile);

		pInfo->pTexture = GET_SINGLE(CResourceManager)->LoadTexture(pFile);

		m_vecTextureInfo.push_back(pInfo);
	}
}

void CMaterial::ShowWindow()
{
	std::string strName = m_strName;

	if (strName == "")
	{
		strName = "Material";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		ImGui::ColorPicker4("Diffuse", &m_tCBuffer.vDif.x);
		ImGui::ColorPicker4("Ambient", &m_tCBuffer.vEmb.x);
		ImGui::InputFloat4("Specular", &m_tCBuffer.vSpec.x);
		ImGui::ColorPicker4("Emissive", &m_tCBuffer.vEmi.x);
		ImGui::Checkbox("Diffuse Texture", reinterpret_cast<bool*>(&m_tCBuffer.iDifTex));
		ImGui::Checkbox("Bump Texture", reinterpret_cast<bool*>(&m_tCBuffer.iNrmTex));
		ImGui::Checkbox("Specular Texture", reinterpret_cast<bool*>(&m_tCBuffer.iSpecTex));
		ImGui::Checkbox("Emissive Texture", reinterpret_cast<bool*>(&m_tCBuffer.iEmiTex));
		ImGui::Checkbox("Bump", reinterpret_cast<bool*>(&m_tCBuffer.iBump));
		ImGui::Checkbox("Animation", reinterpret_cast<bool*>(&m_tCBuffer.iAnimation));

		size_t iSize = m_vecTextureInfo.size();

		for (size_t i = 0; i < iSize; i++)
		{
			ImGui::Text("Type: %d", m_vecTextureInfo[i]->eLink);
			ImGui::InputInt("Regiter", &m_vecTextureInfo[i]->iRegister);

			bool bShader = static_cast<int>(m_vecTextureInfo[i]->iShaderType) & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);
			if (ImGui::Checkbox("Vertex Shader", &bShader))
			{
				m_vecTextureInfo[i]->iShaderType ^= static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);
			}
			bShader = static_cast<int>(m_vecTextureInfo[i]->iShaderType) & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
			if (ImGui::Checkbox("Pixel Shader", &bShader))
			{
				m_vecTextureInfo[i]->iShaderType ^= static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
			}
			bShader = static_cast<int>(m_vecTextureInfo[i]->iShaderType) & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_HULL);
			if (ImGui::Checkbox("Hull Shader", &bShader))
			{
				m_vecTextureInfo[i]->iShaderType ^= static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_HULL);
			}
			bShader = static_cast<int>(m_vecTextureInfo[i]->iShaderType) & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN);
			if (ImGui::Checkbox("Domain Shader", &bShader))
			{
				m_vecTextureInfo[i]->iShaderType ^= static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN);
			}
			bShader = static_cast<int>(m_vecTextureInfo[i]->iShaderType) & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);
			if (ImGui::Checkbox("geometry Shader", &bShader))
			{
				m_vecTextureInfo[i]->iShaderType ^= static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);
			}
			bShader = static_cast<int>(m_vecTextureInfo[i]->iShaderType) & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE);
			if (ImGui::Checkbox("Compute Shader", &bShader))
			{
				m_vecTextureInfo[i]->iShaderType ^= static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE);
			}

			char strButton[MAX_PATH] = {};

			sprintf_s(strButton, "Delete Texture %d", static_cast<int>(i));
			
			if (ImGui::Button(strButton))
			{
				std::vector<PMaterialTextureInfo>::iterator iter = m_vecTextureInfo.begin() + i;

				SAFE_DELETE(m_vecTextureInfo[i]);
				m_vecTextureInfo.erase(iter);
				i--;
				iSize--;
			}
		}

		static char strName[MAX_PATH] = {};
		ImGui::InputText("Texture", strName, MAX_PATH);
		static int iType = 0;
		ImGui::SliderInt("Texture Type", &iType, static_cast<int>(LINK_TYPE::LT_DIF), static_cast<int>(LINK_TYPE::LT_CUSTOM));

		if (ImGui::Button("Load Texture"))
		{
			const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(TEXTURE_PATH);
			ImGuiFileDialog::Instance()->OpenDialog("Texture Load", "Texture Load", ".dds,.png,.tga,.bmp,.jpg", pPath);
		}

		if (ImGuiFileDialog::Instance()->Display("Texture Load"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string strPath = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string strFileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
				TCHAR strPathName[MAX_PATH] = {};
#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, strPath.c_str(), -1, strPathName, static_cast<int>(strPath.length()));
#else
				strcpy_s(strPathName, strPath.c_str());
#endif
				if (!strcmp(strName, ""))
				{
					SetTexture(static_cast<LINK_TYPE>(iType), strFileName, strPathName, "");
				}
				else
				{
					SetTexture(static_cast<LINK_TYPE>(iType), strName, strPathName, "");
				}

			}
			ImGuiFileDialog::Instance()->Close();
		}
		char strMtrl[MAX_PATH] = {};
		if (ImGui::InputText("Change Name", strMtrl, MAX_PATH))
		{
			SetName(strMtrl);
		}
	}
	ImGui::End();
}
