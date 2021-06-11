#pragma once
#include "PrimitiveComponent.h"

	class CMeshComponent :
		public CPrimitiveComponent
	{
		friend class CGameObject;

	protected:
		CMeshComponent();
		CMeshComponent(const CMeshComponent& com);
		virtual ~CMeshComponent();

	protected:
		class CMaterial* m_pMaterial;

	public:
		void SetMaterial(class CMaterial* pMaterial);
		class CMaterial* GetMaterial()	const;

	public:
		void SetTexture(LINK_TYPE eType, class CTexture* pTexture,
			int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0);
		void SetTexture(LINK_TYPE eType, const std::string& strTag,
			int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0);
		void SetTexture(LINK_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath,
			int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0);
		void SetTexture(LINK_TYPE eType, const std::string& strTag, const TCHAR* pFileName,
			int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0);

	public:
		virtual bool Init();
		virtual void Start();
		virtual void Update(float fTime);
		virtual void PostUpdate(float fTime);
		virtual void Collision(float fTime);
		virtual void PreRender(float fTime);
		virtual void Render(float fTime);
		virtual void PostRender(float fTime);
		virtual CMeshComponent* Clone();

	public:
		virtual void Save(FILE* pFile);
		virtual void Load(FILE* pFile);

	};
