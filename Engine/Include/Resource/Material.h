#pragma once

#include "../Ref.h"

	typedef struct _tagMaterialTextureInfo
	{
		LINK_TYPE		eLink;
		std::shared_ptr<class CTexture> pTexture;
		int				iRegister;
		int				iShaderType;
		_tagMaterialTextureInfo()	:
			eLink(LINK_TYPE::LT_CUSTOM)
			, iRegister(0)
			, iShaderType(0)
		{

		}
	}MaterialTextureInfo, * PMaterialTextureInfo;

	class CMaterial :
		public CRef
	{
		friend class CResourceManager;

	public:
		CMaterial();
		CMaterial(const CMaterial& material);
		~CMaterial();

	private:
		std::shared_ptr<class CShader>		m_pShader;
		ShaderCBuffer						m_tCBuffer;
		std::vector<PMaterialTextureInfo>	m_vecTextureInfo;
		unsigned int						m_iKey;

	public:
		void SetTexture(LINK_TYPE eType, std::shared_ptr<CTexture>,
			int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0);
		void SetTexture(LINK_TYPE eType, const std::string& strTag,
			int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0);
		void SetTexture(LINK_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath,
			int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0);
		void SetTexture(LINK_TYPE eType, const std::string& strTag, const TCHAR* pFileName,
			int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0);
		void ClearTexture();
		size_t GetInfoSize()	const;
		PMaterialTextureInfo GetTexInfo(int idx)	const;

	public:
		void SetShader(const std::string& strName);
		void SetDiffuseColor(const Vector4& v);
		void SetDiffuseColor(float x, float y, float z, float a);
		void SetAmbientColor(const Vector4& v);
		void SetAmbientColor(float x, float y, float z, float a);
		void SetSpecularColor(const Vector4& v);
		void SetSpecularColor(float x, float y, float z, float a);
		void SetEmissiveColor(const Vector4& v);
		void SetEmissiveColor(float x, float y, float z, float a);
		void SetSpecularPower(float fPower);
		void EnableBump();
		void EnableAnimation();
		void SetAnimation(bool bAni);
		void SetEnableBump(bool bBump);
		void SetDiffuseTex(bool bEnable);
		void SetAmbientTex(bool bEnable);
		void SetSpecularTex(bool bEnable);
		void SetEmissiveTex(bool bTex);
		void SetDecal(bool bDecal);
		const Vector4& GetDiffuseColor()	const;
		std::shared_ptr<class CShader> GetShader()	const;

	public:
		void Render(float fTime);
		void SetMaterialNoShader();
		std::shared_ptr<class CMaterial> Clone();

	public:
		virtual void Save(FILE* pFile);
		virtual void Load(FILE* pFile);

	public:
		void ShowWindow();
	};
