#include "Matrix.h"
#include "Vector3.h"

_tagMatrix::_tagMatrix()
{
	m = DirectX::XMMatrixIdentity();
}

_tagMatrix::_tagMatrix(const _tagMatrix& _m)	:
	m(_m.m)
{
}

_tagMatrix::_tagMatrix(const DirectX::XMMATRIX& _m)	:
	m(_m)
{
}

_tagMatrix::_tagMatrix(Vector4 _v[4])
{
	for (int i = 0; i < 4; ++i)
		v[i] = _v[i];
}

_tagVector4 _tagMatrix::operator[](int iIdx)	const
{
	return v[iIdx];
}

_tagMatrix _tagMatrix::operator=(const _tagMatrix& _m)
{
	m = _m.m;

	return *this;
}

_tagMatrix _tagMatrix::operator= (const DirectX::XMMATRIX& _m)
{
	m = _m;

	return *this;
}

_tagMatrix _tagMatrix::operator= (Vector4 _v[4])
{
	for(int i=0;i<4;++i)
		v[i] = _v[i];

	return *this;
}

_tagMatrix _tagMatrix::operator*(const _tagMatrix& _m)	const
{
	return _tagMatrix(m * _m.m);
}

_tagMatrix _tagMatrix::operator*(const DirectX::XMMATRIX& _m)	const
{
	return _tagMatrix(m * _m);
}

_tagMatrix _tagMatrix::operator*(Vector4 v[4])	const
{
	return _tagMatrix(v) * m;
}

_tagMatrix _tagMatrix::operator*=(const _tagMatrix& _m)
{
	m *= _m.m;

	return *this;
}

_tagMatrix _tagMatrix::operator*=(const DirectX::XMMATRIX& _m)
{
	m *= _m;

	return *this;
}

_tagMatrix _tagMatrix::operator*=(Vector4 _v[4])
{
	m *= _tagMatrix(_v).m;

	return *this;
}

void _tagMatrix::Identity()
{
	m = DirectX::XMMatrixIdentity();
}

void _tagMatrix::Inverse()
{
	m = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(m), m);
}

void _tagMatrix::Transpose()
{
	m = DirectX::XMMatrixTranspose(m);
}

void _tagMatrix::Scaling(const struct _tagVector3& v)
{
	m = DirectX::XMMatrixScaling(v.x, v.y, v.z);
}

void _tagMatrix::Scaling(float x, float y, float z)
{
	m = DirectX::XMMatrixScaling(x, y, z);
}

void _tagMatrix::Translate(const _tagVector3& v)
{
	m = DirectX::XMMatrixTranslation(v.x, v.y, v.z);
}

void _tagMatrix::Translate(float x, float y, float z)
{
	m = DirectX::XMMatrixTranslation(x, y, z);
}

void _tagMatrix::Rotation(const _tagVector3& v)
{
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(DegToRad(v.x));
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(DegToRad(v.y));
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(DegToRad(v.z));

	m = X * Y * Z;
}

void _tagMatrix::Rotation(float x, float y, float z)
{
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(DegToRad(x));
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(DegToRad(y));
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(DegToRad(z));

	m = X * Y * Z;
}

void _tagMatrix::RotationX(float x)
{
	m = DirectX::XMMatrixRotationX(DegToRad(x));
}

void _tagMatrix::RotationY(float y)
{
	m = DirectX::XMMatrixRotationY(DegToRad(y));
}

void _tagMatrix::RotationZ(float z)
{
	m = DirectX::XMMatrixRotationZ(DegToRad(z));
}

void _tagMatrix::RotationAxis(const struct _tagVector3& v, float fAngle)
{
	DirectX::XMVECTOR v1 = v.Convert();

	m = DirectX::XMMatrixRotationAxis(v1, fAngle);
}

_tagMatrix _tagMatrix::StaticIdentity(const _tagMatrix& m)
{
	_tagMatrix m1 = m;

	m1.Identity();

	return m1;
}

_tagMatrix _tagMatrix::StaticInverse(const _tagMatrix& m)
{
	_tagMatrix m1 = m;
	
	m1.Inverse();

	return m1;
}

_tagMatrix _tagMatrix::StaticTranspose(const _tagMatrix& m)
{
	_tagMatrix m1 = m;

	m1.Transpose();

	return m1;
}

_tagMatrix _tagMatrix::StaticRotation(const _tagVector3& v)
{
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(v.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationX(v.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationX(v.z);

	return X * Y * Z;
}

_tagMatrix _tagMatrix::StaticRotation(float x, float y, float z)
{
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationX(y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationX(z);

	return X * Y * Z;
}

_tagMatrix _tagMatrix::StaticRotationX(float x)
{
	return DirectX::XMMatrixRotationX(DegToRad(x));
}

_tagMatrix _tagMatrix::StaticRotationY(float y)
{
	return DirectX::XMMatrixRotationY(DegToRad(y));
}

_tagMatrix _tagMatrix::StaticRotationZ(float z)
{
	return DirectX::XMMatrixRotationZ(DegToRad(z));
}

_tagMatrix _tagMatrix::StaticRotationAxis(const _tagVector3& v, float fAngle)
{
	return DirectX::XMMatrixRotationAxis(v.Convert(), fAngle);
}
