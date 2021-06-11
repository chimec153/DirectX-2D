#include "LightHelper.fx"

cbuffer cbPerObject	:	register(b13)
{
	float4x4 gWorldInvTranspose;
};

struct VertexIn
{
	float3 PosL	:	POSITION;
	float3 NormalL	:	NORMAL;
	float2 UV	:	TEXCOORD;
};

struct VertexOut
{
	float4 PosH	:	SV_POSITION;
	float3 PosW	:	POSITION;
	float3 NormalW	:	NORMAL;
	float2 UV	:	TEXCOORD;
	float3 PosV	:	POSITION1;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.PosW = mul(float4(vin.PosL, 1.0f), g_matWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)g_matWInvTrans);
	vout.PosH = mul(float4(vin.PosL, 1.0f), g_matWVP);
	vout.UV = mul(float4(vin.UV, 0.f, 1.f), g_matTexTransform).xy;
	vout.PosV = mul(float4(vin.PosL, 1.0f), g_matWV);

	return vout;
}

float4 PS(VertexOut pin, float4 vPos	:	SV_POSITION) : SV_Target
{
	pin.NormalW = normalize(pin.NormalW);

	float3 toEyeW = normalize(gEyePosW - pin.PosW);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;

	ComputeDirectionalLight(gDirLight,
		pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(gPointLight,
		pin.PosW, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputeSpotLight(gSpotLight,
		pin.PosW, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;


	float4 texColor = float4(1.f, 1.f, 1.f, 1.f);
	texColor = g_tTex.Sample(g_tLinear, pin.UV);
	//float4 alphaColor = float4(1.f, 1.f, 1.f, 1.f);
	//alphaColor = g_tNormTex.Sample(g_tLinear, pin.UV);
	float4 litColor = (ambient + diffuse) * texColor + spec;

	litColor.a = g_vDif.a * texColor.a;

	//litColor = PaperBurn(litColor, vPos.xy / float2(1280.f, 720.f));

	return litColor;
}
float4 PaperPS(VertexOut pin, float4 vPos	:	SV_POSITION) : SV_Target
{
	pin.NormalW = normalize(pin.NormalW);

	float3 toEyeW = normalize(gEyePosW - pin.PosW);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;

	ComputeDirectionalLight(gDirLight,
		pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(gPointLight,
		pin.PosW, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputeSpotLight(gSpotLight,
		pin.PosW, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	float4 litColor = ambient + diffuse * g_tTex.Sample(g_tPoint, pin.UV) + spec;

	litColor.a = g_vDif.a;

	litColor = PaperBurn(litColor, ((pin.PosV.xy / float2(1280.f, 1024.f)) + 1.f) / 2.f);

	clip((litColor.w == 0.f) * -1);

	return litColor;
}