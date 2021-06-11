#pragma once
#include "../Ref.h"

	class CSample :
		public CRef
	{
		friend class CResourceManager;
		friend class CSceneResource;

	public:
		CSample();
		virtual ~CSample();

	private:
		ID3D11SamplerState* m_pState;

	public:
		ID3D11SamplerState* GetState()	const;
		void SetSampler(int iRegister, int iType);
		void ResetSampler(int iRegister, int iType);

	public:
		bool CreateSample(D3D11_FILTER eFilter, 
			D3D11_TEXTURE_ADDRESS_MODE eUMode = D3D11_TEXTURE_ADDRESS_WRAP, 
			D3D11_TEXTURE_ADDRESS_MODE eVMode = D3D11_TEXTURE_ADDRESS_WRAP, 
			D3D11_TEXTURE_ADDRESS_MODE eWMode = D3D11_TEXTURE_ADDRESS_WRAP, 
			D3D11_COMPARISON_FUNC eFunc = D3D11_COMPARISON_NEVER,
			int iMaxAnisotropy = 0,
			float pColor[4] = nullptr);

	};