#include "Share.fx"

Texture2D	g_DepthTexture	:	register(t12);

struct VS_INPUT_OCEAN
{
	float3	vPos	:	POSITION;
	float2	vUV	:	TEXCOORD;
	float3	vNormal	:	NORMAL;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
	float4	vBlendWeight	:	BLENDWEIGHT;
	float4	vBlendIndices	:	BLENDINDICES;
};

struct VS_OUTPUT_OCEAN
{
	float4	vPos	:	SV_POSITION;
	float4	vProjPos	:	POSITION;
	float3	vViewPos	:	POSITION1;
	float2	vUV	:	TEXCOORD;
	float3	vNormal	:	NORMAL;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
};

float GetWaveHeight(float x, float z)
{
	return (sin(x) + cos(z)) * 2.f + sin(x * 0.2f) + cos(z * 0.2f);
}

VS_OUTPUT_OCEAN VS(VS_INPUT_OCEAN input)
{
	VS_OUTPUT_OCEAN output = (VS_OUTPUT_OCEAN)0.f;

	float3 vPos = input.vPos - g_vMeshSize * g_vPivot;
	vPos.y = GetWaveHeight(g_fAccTime + vPos.x, g_fAccTime + vPos.z);

	float3 vLeftPos = 0.f;
	vLeftPos.x = vPos.x - 1.f;
	vLeftPos.y = GetWaveHeight(g_fAccTime + vPos.x- 1.f, g_fAccTime + vPos.z);// sin(g_fAccTime + vPos.x - 1.f) + cos(g_fAccTime + vPos.z);
	vLeftPos.z = vPos.z;

	float3 vRightPos = 0.f;
	vRightPos.x = vPos.x + 1.f;
	vRightPos.y = GetWaveHeight(g_fAccTime + vPos.x + 1.f, g_fAccTime + vPos.z);//sin(g_fAccTime + vPos.x + 1.f) + cos(g_fAccTime + vPos.z);
	vRightPos.z = vPos.z;

	float3 vTopPos = 0.f;
	vTopPos.x = vPos.x;
	vTopPos.y = GetWaveHeight(g_fAccTime + vPos.x , g_fAccTime + vPos.z - 1.f);// sin(g_fAccTime + vPos.x) + cos(g_fAccTime + vPos.z - 1.f);
	vTopPos.z = vPos.z - 1.f;

	float3 vBottomPos = 0.f;
	vBottomPos.x = vPos.x;
	vBottomPos.y = GetWaveHeight(g_fAccTime + vPos.x - 1, g_fAccTime + vPos.z+ 1.f);//sin(g_fAccTime + vPos.x) + cos(g_fAccTime + vPos.z + 1.f);
	vBottomPos.z = vPos.z + 1.f;

	float3 vLTPos = 0.f;
	vLTPos.x = vPos.x - 1.f;
	vLTPos.y = GetWaveHeight(g_fAccTime + vPos.x - 1.f, g_fAccTime + vPos.z -1.f);//sin(g_fAccTime + vPos.x - 1.f) + cos(g_fAccTime + vPos.z - 1.f);
	vLTPos.z = vPos.z - 1.f;

	float3 vRBPos = 0.f;
	vRBPos.x = vPos.x + 1.f;
	vRBPos.y = GetWaveHeight(g_fAccTime + vPos.x + 1.f, g_fAccTime + vPos.z+1.f);//sin(g_fAccTime + vPos.x + 1.f) + cos(g_fAccTime + vPos.z + 1.f);
	vRBPos.z = vPos.z + 1.f;

	float3 vFaceNormal[6];

	float3 vEdge1 = vTopPos - vLTPos;
	float3 vEdge2 = vPos - vLTPos;

	vFaceNormal[0] = normalize(cross(vEdge1, vEdge2));

	vEdge1 = vRightPos - vTopPos;
	vEdge2 = vPos - vTopPos;

	vFaceNormal[1] = normalize(cross(vEdge1, vEdge2));

	vEdge1 = vRightPos - vPos;
	vEdge2 = vRBPos - vPos;

	vFaceNormal[2] = normalize(cross(vEdge1, vEdge2));

	vEdge1 = vBottomPos - vPos;

	vFaceNormal[3] = normalize(cross(vEdge2, vEdge1));

	vEdge2 = vLeftPos - vPos;

	vFaceNormal[4] = normalize(cross(vEdge1, vEdge2));

	vEdge1 = vLTPos - vPos;

	vFaceNormal[5] = normalize(cross(vEdge2, vEdge1));

	float3 vNormal = (vFaceNormal[0] + vFaceNormal[1] + vFaceNormal[2] + vFaceNormal[3] + vFaceNormal[4] + vFaceNormal[5]) / 6.f;

	vNormal = normalize(vNormal);

	float3 vTangent = 0.f;
	vTangent.x = 2.f;
	vTangent.y = vRightPos.y - vLeftPos.y;

	vTangent = normalize(vTangent);

	float3 vBinormal = normalize(cross(vNormal, vTangent));

	vTangent = cross(vBinormal, vNormal);

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vProjPos = output.vPos;
	output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
	output.vUV = input.vUV + float2(g_fAccTime / g_fDetailLevelX / 100.f, g_fAccTime / g_fDetailLevelY / 100.f);
	output.vNormal = vNormal;
	output.vTangent = vTangent;
	output.vBinormal= vBinormal;

	return output;
}

float4 PS(VS_OUTPUT_OCEAN input) : SV_TARGET
{
	float2 vTiledUV = input.vUV * float2(g_fDetailLevelX, g_fDetailLevelY);
	LightResult tResult = ComputeLight(input.vViewPos, input.vNormal, input.vTangent, input.vBinormal, vTiledUV);

	float2 vUV = 0.f;

	vUV.x = input.vProjPos.x / input.vProjPos.w * 0.5f + 0.5f;
	vUV.y = input.vProjPos.y / input.vProjPos.w * -0.5f + 0.5f;

	float fDepth = g_DepthTexture.Sample(g_tPoint, vUV).w;
	
	if (fDepth < input.vProjPos.w)
	{
		if(fDepth != 0.f)
			clip(-1);
	}

	float4 vColor = g_tTex.Sample(g_tAnisotropic, vTiledUV);
	vColor = vColor * (tResult.vDif + tResult.vAmb);

	vColor.a = g_vDif.a;
	
	return vColor + tResult.vSpc + tResult.vEmi;
}