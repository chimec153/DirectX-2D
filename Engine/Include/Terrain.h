#pragma once

#include "GameEngine.h"
struct InitInfo
{
	std::wstring HeightMapFilename;

	std::wstring LayerMapFilename0;
	std::wstring LayerMapFilename1;
	std::wstring LayerMapFilename2;
	std::wstring LayerMapFilename3;
	std::wstring LayerMapFilename4;
	std::wstring BlendMapFilename;

	float HeightScale;

	UINT HeightmapWidth;
	UINT HeightmapHeight;

	float CellSpacing;
};

struct Terrain
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
	XMFLOAT2 BoundsY;
};
class CTerrain
{
	int mNumPatchQuadFaces;
	int mNumPatchVertRows;
	int mNumPatchVertCols;
	ID3D11ShaderResourceView* mHeightMapSRV;
	std::vector<float> mHeightmap;
	InitInfo mInfo;
	void LoadHeightmap();
	bool InBounds(int i, int j);
	float Average(int i, int j);
	void Smooth();
	void BuildHeightmapSRV(ID3D11Device* device);
	float GetWidth()	const;
	float GetDepth()	const;

	void BuildQuadPatchVB(ID3D11Device* device);
	void BuildQuadPatchIB(ID3D11Device* device);
};


