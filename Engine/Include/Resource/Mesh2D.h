#pragma once
#include "Mesh.h"

	class CMesh2D :
		public CMesh
	{
		friend class CResourceManager;
		friend class CSceneResource;

	public:
		CMesh2D();
		~CMesh2D();

	public:
		bool Init();
		bool CreateMesh();
		virtual void Render(float fTime,int iContainer = 0, int iSubset=0);
		virtual void RenderInstancing(InstancingBuffer* pIBuffer,int iCount, int iSize, float fTime);
		virtual void RenderParticle(int iCount, float fTime);
		virtual bool CreateMesh(D3D11_PRIMITIVE_TOPOLOGY eTopo, int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage, void* pVtxData,
			int iIdxCount = 0, int iIdxSize = 0, D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
			DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN, void* pIdxData = nullptr, 
			D3D11_CPU_ACCESS_FLAG eCpuFlag = static_cast<D3D11_CPU_ACCESS_FLAG>(0), 
			D3D11_CPU_ACCESS_FLAG eIdxCpuFlag = static_cast<D3D11_CPU_ACCESS_FLAG>(0));
		void RenderTest(float fTime);
		void RenderTest2(float fTime);

	public:
		virtual void Save(FILE* pFile);
		virtual void Load(FILE* pFile);
	};