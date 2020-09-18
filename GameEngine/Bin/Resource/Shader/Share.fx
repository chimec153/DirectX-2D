
struct PS_OUTPUT_SINGLE
{
	float4 vColor	:	SV_TARGET;
};

cbuffer Transform : register(b0)
{
	matrix					fmatWorld;
	matrix					fmatView;
	matrix					fmatProj;
	matrix					fmatWV;
	matrix					fmatWVP;
	float3					vPivot;
	//float					vE1;
	float3					vMeshSize;
	//float					vE2;
};

cbuffer Material : register(b1)
{
	float4	vDif;
	float4	vAmb;
	float4	vSpe;
};