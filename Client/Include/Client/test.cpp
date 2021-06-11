#include "GameEngine.h"
const Vector3 Cross(const Vector3& v, const Vector3& v2);
const Vector3 Normalize(const Vector3* v);
void test()
{
	int mNumTriangles = 10;
	int mNumVertices = 6;
	UINT mIndices[32] = {};
	VERTEX3D mVertices[12] = {};

	for (UINT i = 0; i < mNumTriangles; ++i)
	{
		UINT i0 = mIndices[i * 3 + 0];
		UINT i1 = mIndices[i * 3 + 1];
		UINT i2 = mIndices[i * 3 + 2];

		VERTEX3D v0 = mVertices[i0];
		VERTEX3D v1 = mVertices[i1];
		VERTEX3D v2 = mVertices[i2];

		Vector3 e0 = v1.vPos - v0.vPos;
		Vector3 e1 = v2.vPos - v0.vPos;
		Vector3 faceNormal = Cross(e0, e1);

		mVertices[i0].vNormal += faceNormal;
		mVertices[i1].vNormal += faceNormal;
		mVertices[i2].vNormal += faceNormal;
	}

	for (UINT i = 0; i < mNumVertices; ++i)
		mVertices[i].vNormal = Normalize(&mVertices[i].vNormal);
}

XMMATRIX InverseTranspose(CXMMATRIX M)
{
	XMMATRIX A = M;
	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixTranspose(XMMatrixInverse(&det, A));
}


void Test2()
{

}

const Vector3 Cross(const Vector3& v, const Vector3& v2)
{
	return Vector3();
}

const Vector3 Normalize(const Vector3* v)
{
	return Vector3();
}
