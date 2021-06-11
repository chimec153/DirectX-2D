#include "Share.fx"

struct VS_INPUT_DIST_2D
{
	float3 vPos	:	POSITION;
	float2 vUV	:	TEXCOORD;
	float4 vProjPos	:	PROJECTION;
};

struct VS_OUTPUT_DIST_2D
{
	float4 vPos	:	SV_POSITION;
	float2 vUV	:	TEXCOORD;
	float4 vProjPos	:	PROJECTION;
};

VS_OUTPUT_DIST_2D DistortionVS2D(VS_INPUT_DIST_2D input)
{
	VS_OUTPUT_DIST_2D output = (VS_OUTPUT_DIST_2D)0.f;

	float fcosTime = cos(g_fAccTime * g_fSpeed + input.vUV.x * g_fFrequency) * g_fWeight * 20.f;
	float3 vPos = input.vPos - g_vMeshSize * g_vPivot;

	vPos.y += fcosTime;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vUV = input.vUV;
	output.vProjPos = output.vPos;

	return output;
}

PS_OUTPUT_SINGLE DistortionPS2D(VS_OUTPUT_DIST_2D input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	float2 vUV = float2(input.vProjPos.x / input.vProjPos.w, -input.vProjPos.y / input.vProjPos.w);

	vUV += 1.f;

	vUV *= 0.5f;

	float fcosTime = cos(g_fAccTime * g_fSpeed + input.vUV.x * g_fFrequency) * g_fWeight;
	float fsinTime = sin(g_fAccTime * g_fSpeed + input.vUV.y * g_fFrequency) * g_fWeight;

	float4 vNormalColor = g_DistortNormal.Sample(g_tPoint, input.vUV + float2(fcosTime, fsinTime));
	float4 vFilterColor = g_DistortFilter.Sample(g_tPoint, input.vUV + float2(fcosTime, fsinTime));

	//vUV.x += cos(g_fAccTime * g_fSpeed + input.vUV.x) * g_fWeight;
	//vUV.y += cos(g_fAccTime * g_fSpeed + sqrt((input.vUV.x - 0.5f) * (input.vUV.x - 0.5f) + (input.vUV.y - 0.5f)/2.f * (input.vUV.y - 0.5f) / 2.f) * g_fFrequency) * g_fWeight;

	float4 vColor = vNormalColor * vFilterColor;

	output.vColor = g_Scene.Sample(g_tPoint, vUV + vColor.xy);

	return output;
}

VS_OUTPUT_DIST_2D DistortionVS(VS_INPUT_3D input)
{
	VS_OUTPUT_DIST_2D output = (VS_OUTPUT_DIST_2D)0.f;

	//float fcosTime = cos(g_fAccTime * g_fSpeed + input.vUV.x * g_fFrequency) * g_fWeight * 20.f;
	//vPos.y += fcosTime;
	float3 vPos = input.vPos - g_vMeshSize * g_vPivot;


	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vUV = input.vUV;
	output.vProjPos = output.vPos;

	return output;
}

PS_OUTPUT_SINGLE DistortionPS(VS_OUTPUT_DIST_2D input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	float2 vUV = float2(input.vProjPos.x / input.vProjPos.w, -input.vProjPos.y / input.vProjPos.w);

	vUV += 1.f;

	vUV *= 0.5f;

	float fcosTime = cos(g_fAccTime * g_fSpeed + input.vUV.x * g_fFrequency) * g_fWeight;
	float fsinTime = sin(g_fAccTime * g_fSpeed + input.vUV.y * g_fFrequency) * g_fWeight;

	//vUV.x += fcosTime;
	//vUV.y += fsinTime;

	float4 vNormalColor = g_DistortNormal.Sample(g_tPoint, input.vUV + float2(fcosTime, fsinTime));
	float4 vFilterColor = g_DistortFilter.Sample(g_tPoint, input.vUV + float2(fcosTime, fsinTime));

	//vUV.x += cos(g_fAccTime * g_fSpeed + input.vUV.x) * g_fWeight;
	//vUV.y += cos(g_fAccTime * g_fSpeed + sqrt((input.vUV.x - 0.5f) * (input.vUV.x - 0.5f) + (input.vUV.y - 0.5f)/2.f * (input.vUV.y - 0.5f) / 2.f) * g_fFrequency) * g_fWeight;

	float4 vColor = vNormalColor;// *vFilterColor;

	vColor.xy *= 0.05f;

	output.vColor = g_Scene.Sample(g_tPoint, vUV + vColor.xy);
	//output.vColor = float4(1.f, 0.f, 0.f, 1.f);
	return output;
}