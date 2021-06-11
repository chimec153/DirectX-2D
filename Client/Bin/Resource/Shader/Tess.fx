// HLSL ÄÚµå
#include "Share.fx"

struct VertexIn
{
	float3 PosL	:	POSITION;
};

struct VertexOut
{
	float3 PosL	:	POSITION;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.PosL = vin.PosL;

	return vout;
}

struct PatchTess
{
	float EdgeTess[3]	:	SV_TessFactor;
	float InsideTess	:	SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 3> patch, uint patchID	:	SV_PrimitiveID)
{
	PatchTess pt;

	float3 centerL = 0.25f * (patch[0].PosL + patch[1].PosL +
		patch[2].PosL);
	float3 centerW = mul(float4(centerL, 1.0f), g_matWorld).xyz;

	float3 centerV = mul(float4(centerW, 1.0f), g_matView).xyz;

	float d = length(centerV);

	const float d0 = 20.0f;
	const float d1 = 100.0f;
	float tess = 64.0f * saturate((d1 - d) / (d1 - d0)) + 3.f;

	pt.EdgeTess[0] = tess;
	pt.EdgeTess[1] = tess;
	pt.EdgeTess[2] = tess;

	pt.InsideTess = tess/3.f;

	return pt;
}

struct HullOut
{
	float3 PosL	:	POSITION;
};

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 3> p,
	uint i	:	SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
	HullOut hout;

	hout.PosL = p[i].PosL;

	return hout;
}

struct DomainOut
{
	float4 PosH	:	SV_POSITION;
};

[domain("tri")]
DomainOut DS(PatchTess patchTess,
	float3 uvw	:	SV_DomainLocation,
	const OutputPatch<HullOut, 3> quad)
{
	DomainOut dout;

	float3 p = quad[0].PosL * uvw.x + quad[1].PosL * uvw.y + quad[2].PosL * uvw.z;

	p = normalize(p);

	dout.PosH = mul(float4(p, 1.0f), g_matWVP);

	return dout;
}

float4 PS(DomainOut pin) : SV_TARGET
{
	return g_vDif;
}