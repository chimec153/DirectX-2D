#include "Share.fx"

struct AniKeyFrame
{
	float4	vPos;
	float4	vScale;
	float4	qRot;
};

StructuredBuffer<matrix>		g_vecmatOffset	:	register(t13);
StructuredBuffer<AniKeyFrame>	g_vecKeyFrame	:	register(t14);
StructuredBuffer<AniKeyFrame>	g_vecTransitionBlendKeyFrame	:	register(t15);
StructuredBuffer<float>			g_vecBlendMask	:	register(t16);
StructuredBuffer<AniKeyFrame>	g_vecBlendKeyFrame	:	register(t17);
RWStructuredBuffer<matrix>		g_matFinalBone	:	register(u0);
RWStructuredBuffer<matrix>		g_matBone		:	register(u1);

float4 XMVectorLess(in float4 V1, in float4 V2)
{
	uint4 Control = uint4(
			(V1.x < V2.x) ? 0xFFFFFFFF : 0,
			(V1.y < V2.y) ? 0xFFFFFFFF : 0,
			(V1.z < V2.z) ? 0xFFFFFFFF : 0,
			(V1.w < V2.w) ? 0xFFFFFFFF : 0
		);
	return asfloat(Control);
}

float4 XMVectorSelect(in float4 V1, in float4 V2, in float4 Control)
{
	uint4 Result = uint4(
			(asuint(V1.x) & ~asuint(Control.x)) | (asuint(V2.x) & asuint(Control.x)),
			(asuint(V1.y) & ~asuint(Control.y)) | (asuint(V2.y) & asuint(Control.y)),
			(asuint(V1.z) & ~asuint(Control.z)) | (asuint(V2.z) & asuint(Control.z)),
			(asuint(V1.w) & ~asuint(Control.w)) | (asuint(V2.w) & asuint(Control.w))
		);
	return asfloat(Result);
}

float4 XMVectorNegativeMultiplySubtract(in float4 V1, in float4 V2, in float4 V3)
{
	float4 Result = float4(
			V3.x - (V1.x * V2.x),
			V3.y - (V1.y * V2.y),
			V3.z - (V1.z * V2.z),
			V3.w - (V1.w * V2.w)
		);
	return Result;
}

float4 XMVectorSplatSignMask()
{
	uint4 vResult = 0x80000000U;

	return asfloat(vResult);
}

void XMVectorPermute(in float4 V1, in float4 V2, uint PermuteX, uint PermuteY, 
	uint PermuteZ, uint PermuteW, out float4 vResult)
{
	float4 aPtr[2] = { V1,V2 };
	float4 _vResult = 0.f;

	const uint i0 = PermuteX & 3;
	const uint vi0 = PermuteX >> 2;
	_vResult[0] = aPtr[vi0][i0];

	const uint i1 = PermuteY & 3;
	const uint vi1 = PermuteY >> 2;
	_vResult[1] = aPtr[vi1][i1];

	const uint i2 = PermuteZ & 3;
	const uint vi2 = PermuteZ >> 2;
	_vResult[2] = aPtr[vi2][i2];

	const uint i3 = PermuteW & 3;
	const uint vi3 = PermuteW >> 2;
	_vResult[3] = aPtr[vi3][i3];

	vResult = _vResult;
}

float4 XMVectorShiftLeft(in float4 V1, in float4 V2, uint Elements)
{
	float4 vResult = 0.f;

	XMVectorPermute(V1,V2, Elements, Elements+1, Elements+2, Elements+3, vResult);

	return vResult;
}

float4 XMVectorXorInt(in float4 V1, in float4 V2)
{
	uint4 Result = uint4(
			asuint(V1.x) ^ 2147483648,
			asuint(V1.y) ^ 0,
			asuint(V1.z) ^ 0,
			asuint(V1.w) ^ 0
		);
	return asfloat(Result);
}

float4 XMVectorReciprocal(in float4 V)
{
	float4 Result = float4(
			1.f / V.x,
			1.f / V.y,
			1.f / V.z,
			1.f / V.w
		);
	return Result;
}

float4 XMVectorMultiplyAdd(in float4 V1, in float4 V2, in float4 V3)
{
	float4 Result = float4(
			V1.x * V2.x + V3.x,
			V1.y * V2.y + V3.y,
			V1.z * V2.z + V3.z,
			V1.w * V2.w + V3.w
		);
	return Result;
}

float4 XMQuaternionSlerpV(in float4 Q0, in float4 Q1, float4 T)
{
	const float4 OneMinusEpsilon = float4(1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f);

	float4 CosOmega = dot(Q0, Q1);

	const float4 Zero = 0.f;
	float4 Control = XMVectorLess(CosOmega, Zero);
	float4 Sign = XMVectorSelect(float4(1.f, 1.f, 1.f, 1.f), float4(-1.f, -1.f, -1.f, -1.f), Control);

	CosOmega = CosOmega * Sign;

	Control = XMVectorLess(CosOmega, OneMinusEpsilon);

	float4 SinOmega = XMVectorNegativeMultiplySubtract(CosOmega, CosOmega, float4(1.f, 1.f, 1.f, 1.f));
	SinOmega = sqrt(SinOmega);

	float4 Omega = atan2(SinOmega, CosOmega);

	float4 SignMask = XMVectorSplatSignMask();
	float4 V01 = XMVectorShiftLeft(T, Zero,2);
	SignMask = XMVectorShiftLeft(SignMask, Zero,3);
	V01 = XMVectorXorInt(V01, SignMask);
	V01 = float4(1.f, 0.f, 0.f, 0.f)+ V01;

	float4 InvSinOmega = XMVectorReciprocal(SinOmega);

	float4 S0 = V01* Omega;
	S0 = sin(S0);
	S0 = S0* InvSinOmega;

	S0 = XMVectorSelect(V01, S0, Control);

	float4 S1 = S0.y;
	S0 = S0.x;

	S1 = S1 * Sign;

	float4 Result = Q0 * S0;
	Result = XMVectorMultiplyAdd(Q1, S1, Result);

	return Result;
}

float4 XMQuaternionSlerp(in float4 Q0, in float4 Q1, float t)
{
	float4 T = t;
	return XMQuaternionSlerpV(Q0, Q1, T);
}

float4 XMVectorSwizzle(in float4 V, uint E0, uint E1, uint E2, uint E3)
{
	float4 Result = float4(
			V[E0],
			V[E1],
			V[E2],
			V[E3]
		);
	return Result;
}

void XMMatrixRotationQuaternion(in float4 Quaternion, out matrix M)
{
	static const float4 Constant1110 = float4(1.0f, 1.0f, 1.0f, 0.0f);

	float4 Q0 = Quaternion + Quaternion;
	float4 Q1 = Quaternion* Q0;

	float4 V0 = 0.f;
	XMVectorPermute(Q1, Constant1110, 1, 0, 0, 7, V0);
	float4 V1 = 0.f;
	XMVectorPermute(Q1, Constant1110, 2, 2, 1, 7, V1);
	float4 R0 = Constant1110 - V0;
	R0 = R0 - V1;

	V0 = XMVectorSwizzle(Quaternion, 0, 0, 1, 3);
	V1 = XMVectorSwizzle(Q0, 2, 1, 2, 3);
	V0 = V0 * V1;

	V1 = Quaternion.w;
	float4 V2 = XMVectorSwizzle(Q0, 1, 2, 0, 3);
	V1 = V1 * V2;

	float4 R1 = V0 + V1;
	float4 R2 = V0 - V1;

	V0 = 0.f;
	XMVectorPermute(R1, R2, 1, 4, 5, 2, V0);
	V1 = 0.f;
	XMVectorPermute(R1, R2, 0, 6, 0, 6, V1);

	matrix m;
	float4 v = 0.f; 
	XMVectorPermute(R0, V0, 0, 4, 5, 3, v);
	m._11_12_13_14 = v;
	XMVectorPermute(R0, V0, 6, 1, 7, 3, v);
	m._21_22_23_24 = v;
	XMVectorPermute(R0, V1, 4, 5, 2, 3, v);
	m._31_32_33_34 = v;
	m._41_42_43_44 = float4(0.0f, 0.0f, 0.0f, 1.0f);
	M = m;
}

void XMMatrixAffineTransformation(in float4 Scaling, in float4 RotationOrigin,
	in float4 RotationQuaternion, in float4 Translation, out matrix M)
{
	matrix MScaling = 0.f;
	MScaling._11_22_33_44 = float4(Scaling.xyz, 1.f);

	float4 VRotationOrigin = float4(RotationOrigin.xyz , 0.f);
	matrix MRotation = 0.f;
	XMMatrixRotationQuaternion(RotationQuaternion, MRotation);
	float4 VTranslation = float4(Translation.xyz, 0.f);

	matrix m;
	m = MScaling;
	m._41_42_43_44 = m._41_42_43_44 - VRotationOrigin;
	m =mul( m , MRotation);
	m._41_42_43_44 = m._41_42_43_44 + VRotationOrigin;
	m._41_42_43_44 = m._41_42_43_44 + VTranslation;
	M = m;
}

[numthreads(256, 1, 1)]
void CS(int3 ID	:	SV_DispatchThreadID)
{
	if (ID.x >= g_iAniBoneCount)
		return;

	float4 vPos = 0.f;
	float4 vScale = 0.f;
	float4 qRot = 0.f;

	[flatten]
	if (!g_iAniTransition)
	{
		int iIndex = g_iAniFrame + g_iAniFrameCount * ID.x;

		AniKeyFrame tKeyFrame = g_vecKeyFrame[iIndex];

		int iNextIndex = g_iAniNextFrame + g_iAniFrameCount * ID.x;

		AniKeyFrame tNextKeyFrame = g_vecKeyFrame[iNextIndex];

		int iBlendIndex = g_iAniBlendFrame + g_iAniBlendFrameCount * ID.x;

		AniKeyFrame tBlendKeyFrame = g_vecBlendKeyFrame[iBlendIndex];

		int iBlendNextIndex = (g_iAniBlendFrame + 1) % g_iAniBlendFrameCount + g_iAniBlendFrameCount * ID.x;

		AniKeyFrame tBlendNextKeyFrame = g_vecBlendKeyFrame[iBlendNextIndex];

		float4 vAniPos = lerp(tKeyFrame.vPos, tNextKeyFrame.vPos, g_fAniRatio);
		float4 vAniScale = lerp(tKeyFrame.vScale, tNextKeyFrame.vScale, g_fAniRatio);
		float4 qAniRot = XMQuaternionSlerp(tKeyFrame.qRot, tNextKeyFrame.qRot, g_fAniRatio);

		float4 vBlendPos = lerp(tBlendKeyFrame.vPos, tBlendNextKeyFrame.vPos, g_fAniBlendRatio) + g_vecKeyFrame[g_iAniFrame + g_iAniFrameCount * 2].vPos - g_vecBlendKeyFrame[g_iAniBlendFrame + g_iAniBlendFrameCount * 2].vPos;
		float4 vBlendScale = lerp(tBlendKeyFrame.vScale, tBlendNextKeyFrame.vScale, g_fAniBlendRatio);
		float4 qBlendRot = XMQuaternionSlerp(tBlendKeyFrame.qRot, tBlendNextKeyFrame.qRot, g_fAniBlendRatio);

		vPos = lerp(vAniPos, vBlendPos, g_fAniBlendRate * g_vecBlendMask[ID.x]);
		vScale = lerp(vAniScale, vBlendScale, g_fAniBlendRate * g_vecBlendMask[ID.x]);
		qRot = XMQuaternionSlerp(qAniRot, qBlendRot, g_fAniBlendRate * g_vecBlendMask[ID.x]);

		/*[flatten] INIÆÄÀÏ
		if (g_iAniBlendSpacing)
		{
			float4 vBlendPos = 0.f;
			float4 vBlendScale = 0.f;
			float4 qBlendRot = 0.f;

			int iCurrentBlendFrame = g_iAniBlendFrame;

			int iBlendIndex = iCurrentBlendFrame + g_iAniBlendFrameCount * ID.x;

			AniKeyFrame tBlendKeyFrame = g_vecBlendKeyFrame[iBlendIndex];

			int iNextBlendFrame = iCurrentBlendFrame + 1;
			AniKeyFrame tNextBlendKeyFrame = (AniKeyFrame)0.f;

			[flatten]
			if (iNextBlendFrame == g_iAniBlendFrameCount)
			{
				int iCurrentFrame = g_iAniFrame;

				int iIndex = iCurrentFrame + g_iAniFrameCount * ID.x;

				tNextBlendKeyFrame = g_vecKeyFrame[iIndex];
			}
			else
			{
				int iNextBlendIndex = iNextBlendFrame + g_iAniBlendFrameCount * ID.x;

				tNextBlendKeyFrame = g_vecBlendKeyFrame[iNextBlendIndex];
			}

			vBlendPos = lerp(tBlendKeyFrame.vPos, tNextBlendKeyFrame.vPos, g_fAniBlendRatio);
			vBlendScale = lerp(tBlendKeyFrame.vScale, tNextBlendKeyFrame.vScale, g_fAniBlendRatio);
			qBlendRot = XMQuaternionSlerp(tBlendKeyFrame.qRot, tNextBlendKeyFrame.qRot, g_fAniBlendRatio);

			int iCurrentFrame = g_iAniFrame;

			int iIndex = iCurrentFrame + g_iAniFrameCount * ID.x;

			AniKeyFrame tKeyFrame = g_vecKeyFrame[iIndex];

			int iNextFrame = g_iAniNextFrame;

			int iNextIndex = iNextFrame + g_iAniFrameCount * ID.x;

			AniKeyFrame tNextKeyFrame = g_vecKeyFrame[iNextIndex];
			vPos = lerp(tKeyFrame.vPos, tNextKeyFrame.vPos, g_fAniRatio);
			vScale = lerp(tKeyFrame.vScale, tNextKeyFrame.vScale, g_fAniRatio);
			qRot = XMQuaternionSlerp(tKeyFrame.qRot, tNextKeyFrame.qRot, g_fAniRatio);

			vPos = lerp(vPos, vBlendPos, g_vecBlendMask[ID.x]);
			vScale = lerp(vScale, vBlendScale, g_vecBlendMask[ID.x]);
			qRot = XMQuaternionSlerp(qRot, qBlendRot, g_vecBlendMask[ID.x]);
		}
		else
		{
			int iCurrentFrame = g_iAniFrame;

			int iIndex = iCurrentFrame + g_iAniFrameCount * ID.x;

			AniKeyFrame tKeyFrame = g_vecKeyFrame[iIndex];

			int iNextFrame = g_iAniNextFrame;

			int iNextIndex = iNextFrame + g_iAniFrameCount * ID.x;
			AniKeyFrame tNextKeyFrame = g_vecKeyFrame[iNextIndex];
			vPos = lerp(tKeyFrame.vPos, tNextKeyFrame.vPos, g_fAniRatio);
			vScale = lerp(tKeyFrame.vScale, tNextKeyFrame.vScale, g_fAniRatio);
			qRot = XMQuaternionSlerp(tKeyFrame.qRot, tNextKeyFrame.qRot, g_fAniRatio);
		}*/
	}
	else
	{
		int iCurrentFrame = 0;

		int iIndex = 0;

		AniKeyFrame tKeyFrame = (AniKeyFrame)0.f;
		iCurrentFrame = g_iAniFrame;

		iIndex = iCurrentFrame + g_iAniFrameCount * ID.x;

		tKeyFrame = g_vecKeyFrame[iIndex];

		AniKeyFrame tTransitionKeyFrame = g_vecTransitionBlendKeyFrame[g_iAniNextFrameCount * ID.x];

		vPos = lerp(tKeyFrame.vPos, tTransitionKeyFrame.vPos, g_fAniTransitionRatio);
		vScale = lerp(tKeyFrame.vScale, tTransitionKeyFrame.vScale, g_fAniTransitionRatio);
		qRot = XMQuaternionSlerp(tKeyFrame.qRot, tTransitionKeyFrame.qRot, g_fAniTransitionRatio);

		[flatten]
		if (g_fAniBlendRate != 0.f)
		{
			int iBlendIndex = g_iAniBlendFrame;
			int iNextBlendIndex = g_iAniBlendFrame + 1;

			AniKeyFrame tBlendKeyFrame = g_vecBlendKeyFrame[iBlendIndex + ID.x * g_iAniBlendFrameCount];
			AniKeyFrame tNextBlendKeyFrame = (AniKeyFrame)0.f;

			[flatten]
			if (iNextBlendIndex == g_iAniBlendFrameCount)
			{
				tNextBlendKeyFrame = tKeyFrame;
			}
			else
			{
				tNextBlendKeyFrame = g_vecBlendKeyFrame[iNextBlendIndex + ID.x * g_iAniBlendFrameCount];
			}


			float4 vBlendPos = lerp(tBlendKeyFrame.vPos, tNextBlendKeyFrame.vPos, g_fAniBlendRatio);
			float4 vBlendScale = lerp(tBlendKeyFrame.vScale, tNextBlendKeyFrame.vScale, g_fAniBlendRatio);
			float4 qBlendRot = XMQuaternionSlerp(tBlendKeyFrame.qRot, tNextBlendKeyFrame.qRot, g_fAniBlendRatio);

			vPos = lerp(vPos, vBlendPos, g_vecBlendMask[ID.x] * g_fAniBlendRate);
			vScale = lerp(vScale, vBlendScale, g_vecBlendMask[ID.x] * g_fAniBlendRate);
			qRot = XMQuaternionSlerp(qRot, qBlendRot, g_vecBlendMask[ID.x] * g_fAniBlendRate);
		}
	}

	matrix matFinal = 0.f;

	XMMatrixAffineTransformation(vScale, float4(0.f, 0.f, 0.f, 1.f), qRot, vPos, matFinal);

	matrix matOffset = transpose(g_vecmatOffset[ID.x]);

	g_matFinalBone[g_iAniBoneCount * g_iInstIndex + ID.x] = mul(matOffset, matFinal);
	g_matBone[ID.x] = transpose(matFinal);

}