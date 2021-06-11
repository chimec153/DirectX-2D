#include "Share.fx"

struct VIn
{
	float3	PosL	:	POSITION;
	float3	vNormal	:	NORMAL;
	float2	vUV	:	TEXCOORD;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
	float4	vBlendWeight	:	BLENDWEIGHT;
	float4	vBlendIndex	:	BLENDINDICES;
};

struct VOut
{
	float4	PosH	:	SV_POSITION;
	float3	PosL	:	POSITION;
	float4	vProjPos	:	POSITION2;
	float3	vPosW	:	POSITION3;
	float3	vPosV	:	POSITION4;
	float2	vUV	:	TEXCOORD;
	float3	vNormal	:	NORMAL;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
	float4	vBlendWeight	:	BLENDWEIGHT;
	float4	vBlendIndex	:	BLENDINDICES;
};

VOut VS(VIn vin)
{
	VOut vout = (VOut)0.f;

	vout.PosH = mul(float4(vin.PosL, 1.0f), g_matWVP).xyww;

	vout.PosL = vin.PosL;
	
	return vout;
}

PS_OUTPUT_SINGLE PS(VOut input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = gCubeMap.Sample(g_tPoint, input.PosL);

	return output;
}

static const int CubeMapSize = 256;



struct vin
{
	float3 PosL	:	POSITION;
};

struct vout
{
	float4 PosH	:	SV_POSITION;
	float3 PosL	:	POSITION;
	float4 vProjPos	:	POSITION1;
};

vout VS2(vin v)
{
	vout _v;

	_v.PosH = mul(float4(v.PosL, 1.0f), g_matWVP).xyww;

	_v.vProjPos = _v.PosH;

	_v.PosL = v.PosL;

	return _v;
}

PS_OUTPUT_SINGLE PS2(vout input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	float3 vPos = normalize(input.PosL);

	if (vPos.y < 0.f)
	{
		output.vColor = g_vDif;
	}
	else
	{
		float u = atan2(vPos.x, vPos.z) * 0.159154f + 0.5f;
		float v = atan2(length(vPos.xz), vPos.y) * 0.318310f * 2.f;

		output.vColor = g_tTex.Sample(g_tLinear, float2(u, v));
	}
	//output.GBuffer2.x = 1.f;
	//output.GBuffer2.w = input.vProjPos.w;

	//output.GBuffer3.w = ConvertColor(output.Albedo);
	

	//output.Albedo = gCubeMap2.Sample(g_tLinear, v.PosL);
	//output.GBuffer1.xyz = (-input.PosL).xyz;
	//output.GBuffer2.w = 1.f;
	//output.GBuffer2.y = g_vSpe.w;
	//output.GBuffer3.x = ConvertColor(g_vDif);
	//output.GBuffer3.y = ConvertColor(g_vAmb);

	//float4 vSpc = g_vSpe ;
	//vSpc.w = 1.f;
	//output.GBuffer3.z = ConvertColor(vSpc);
	//output.GBuffer3.w = ConvertColor(g_vEmi);

	return output;
}

struct VertexIn
{
	float3 PosL	:	POSITION;
	float3 NormalL	:	NORMAL;
	float2 Tex	:	TEXCOORD;
	float3 TangentL	:	TANGENT;
};

struct VertexOut
{
	float3 PosW	:	POSITION;
	float3 PosV	:	POSITION1;
	float3 NormalW	:	NORMAL;
	float3 TangentW	:	TANGENT;
	float2 Tex	:	TEXCOORD;
	float TessFactor : TESS;
};

VertexOut VSH(VertexIn vin)
{
	VertexOut vout;

	vout.PosW = mul(float4(vin.PosL, 1.0f), g_matWorld).xyz;
	vout.PosV = mul(float4(vin.PosL, 1.0f), g_matWV).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)g_matWInvTrans);
	vout.TangentW = mul(vin.TangentL, (float3x3)g_matWorld);

	vout.Tex = mul(float4(vin.Tex, 0.f, 1.f), g_matTexTransform).xy;

	float d = length(vout.PosV);

	float tess = saturate((g_fMinTessDistance - d) / (g_fMinTessDistance - g_fMaxTessDistance));

	vout.TessFactor = g_fMinTessFactor + tess * (g_fMaxTessFactor - g_fMinTessFactor);

	return vout;
}

struct PatchTess
{
	float EdgeTess[3]	:	SV_TessFactor;
	float InsideTess : SV_InsideTessFactor;
};

PatchTess PatchHS(InputPatch<VertexOut, 3> patch,
	uint patchID	:	SV_PrimitiveID)
{
	PatchTess pt;

	pt.EdgeTess[0] = 0.5f * (patch[1].TessFactor + patch[2].TessFactor);
	pt.EdgeTess[1] = 0.5f * (patch[2].TessFactor + patch[0].TessFactor);
	pt.EdgeTess[2] = 0.5f * (patch[0].TessFactor + patch[1].TessFactor);

	pt.InsideTess = pt.EdgeTess[0];
	return pt;
}

float3 CalcPoint(float3 LightColor, float3 pos, float normal)
{
	float3 ToLight = g_vLightPos - pos;
	float3 ToEye = -pos;
	float DistToLight = length(ToLight);

	ToLight /= DistToLight;
	float NDotL = saturate(dot(ToLight, normal));
	float3 finalColor = LightColor * NDotL;
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + ToLight);
	float NDotH = saturate(dot(HalfWay, normal));
}