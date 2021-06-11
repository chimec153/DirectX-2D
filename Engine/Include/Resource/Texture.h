#pragma once
#include "../Ref.h"

	typedef struct _tagResourceInfo
	{
		ScratchImage* pImage;
		TCHAR* pFullPath;
		char* pPathName;
		TCHAR* pFileName;
		ID3D11ShaderResourceView* pSRV;
		unsigned int				iWidth;
		unsigned int				iHeight;

		_tagResourceInfo() :
			pImage(nullptr),
			pFullPath(nullptr),
			pPathName(nullptr),
			pFileName(nullptr),
			pSRV(nullptr),
			iWidth(0),
			iHeight(0)
		{
			pFullPath = new TCHAR[MAX_PATH];
			pPathName = new char[MAX_PATH];
			pFileName = new TCHAR[MAX_PATH];

			memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);
			memset(pPathName, 0, sizeof(char) * MAX_PATH);
			memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
		}

		~_tagResourceInfo()
		{
			delete[] pFullPath;
			delete[] pPathName;
			delete[] pFileName;
			SAFE_RELEASE(pSRV);
		}
	}ResourceInfo, * PResourceInfo;

	class CTexture :
		public CRef
	{
		friend class CResourceManager;
		friend class CSceneResource;

	public:
		CTexture();
		~CTexture();

	private:
		std::vector<PResourceInfo>		m_vecResourceInfo;
		IMAGE_TYPE						m_eType;
		ID3D11ShaderResourceView*		m_pArraySRV;

	public:
		IMAGE_TYPE GetType()	const
		{
			return m_eType;
		}

		unsigned int GetWidth(int idx = 0)	const
		{
			return m_vecResourceInfo[idx]->iWidth;
		}

		unsigned int GetHeight(int idx = 0)	const
		{
			return m_vecResourceInfo[idx]->iHeight;
		}

		size_t GetSize()	const;

		PResourceInfo GetInfo(int idx = 0)	const;
		void SetImageType(IMAGE_TYPE eType);

	public:
		void SetTexture(ID3D11ShaderResourceView* pSRV, int idx = 0);
		bool LoadTexture(const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath = TEXTURE_PATH);
		bool LoadTextureFromFullPath(const std::string& strTag, const TCHAR* pFullPath);
		bool LoadTexture(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName,
			const std::string& strPathName);
		bool LoadTextureFromFullPath(const std::string& strTag, const std::vector<const TCHAR*> vecFullPath);
		bool LoadTextureArray(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName,
			const std::string& strPathName);
		bool LoadTextureArrayFromFullPath(const std::string& strTag, const std::vector<const TCHAR*> vecFullPath);

		bool CreateImage(int idx = 0);
		bool CreateArrayImage();
		void SetShader(int iRegister, int iShaderType, int idx = 0);

	public:
		virtual void Save(FILE* pFile);
		virtual void Load(FILE* pFile);

	};