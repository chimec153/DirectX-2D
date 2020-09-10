#include "Vector3.h"

_tagVector3 _tagVector3::One(1.f, 1.f, 1.f);
_tagVector3 _tagVector3::Zero(0.f, 0.f, 0.f);

_tagVector3 _tagVector3::Axis[AXIS_END] = {
	_tagVector3(1.f , 0.f, 0.f),
	_tagVector3(0.f , 1.f, 0.f),
	_tagVector3(0.f , 0.f, 1.f)
};

_tagVector3::_tagVector3()	:
	x(0.f),
	y(0.f),
	z(0.f)
{
}

_tagVector3::_tagVector3(const _tagVector3& v)
{
	*this = v;
}

_tagVector3::_tagVector3(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);
}

_tagVector3::_tagVector3(float x, float y, float z)	:
	x(x),
	y(y),
	z(z)
{
}

float _tagVector3::operator[](const int& iIdx)	const
{
	switch (iIdx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}

	assert(false);

	return -1.f;
}

float& _tagVector3::operator[](const int& iIdx)
{
	switch (iIdx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}

	assert(false);

	return z;
}

_tagVector3& _tagVector3::operator=(const _tagVector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

_tagVector3& _tagVector3::operator=(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);

	return *this;
}

_tagVector3& _tagVector3::operator=(float f)
{
	x = f;
	y = f;
	z = f;

	return *this;
}

bool _tagVector3::operator==(const _tagVector3& v) const
{
	return v.x == x && v.y == y && v.z == z;
}

bool _tagVector3::operator==(const DirectX::XMVECTOR& v) const
{
	_tagVector3 v1(v);

	return v1.x == x && v1.y == y && v1.y == y;
}

bool _tagVector3::operator!=(const _tagVector3& v) const
{
	return v.x != x || v.y != y || v.z != z;
}

bool _tagVector3::operator!=(const DirectX::XMVECTOR& v) const
{
	_tagVector3 v1(v);

	return v1.x != x || v1.y != y || v1.y != y;
}

_tagVector3 _tagVector3::operator+(const _tagVector3& v) const
{
	DirectX::XMVECTOR v1 = Convert();
	DirectX::XMVECTOR v2 = v.Convert();

	return _tagVector3(v1 + v2);
}

_tagVector3 _tagVector3::operator+(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector3(v1 + v);
}

_tagVector3 _tagVector3::operator+(float f) const
{
	return _tagVector3(x+f, y+f,z+f);
}

_tagVector3 _tagVector3::operator+=(const _tagVector3& v)
{
	DirectX::XMVECTOR v1 = Convert();
	DirectX::XMVECTOR v2 = v.Convert();

	v1 += v2;

	Convert(v1);

	return _tagVector3(v1);
}

_tagVector3 _tagVector3::operator+=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 += v;

	Convert(v1);

	return _tagVector3(v1);
}

_tagVector3 _tagVector3::operator+=(float f)
{
	x += f;
	y += f;
	z += f;

	return *this;
}

_tagVector3 _tagVector3::operator-(const _tagVector3& v) const
{
	return _tagVector3(x - v.x, y - v.y, z - v.z);
}

_tagVector3 _tagVector3::operator-(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector3(v1 - v);
}

_tagVector3 _tagVector3::operator-(float f) const
{
	return _tagVector3(x - f, y - f, z - f);
}

_tagVector3 _tagVector3::operator-=(const _tagVector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

_tagVector3 _tagVector3::operator-=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 -= v;

	Convert(v1);

	return _tagVector3(v1);
}

_tagVector3 _tagVector3::operator-=(float f)
{
	x -= f;
	y -= f;
	z -= f;

	return *this;
}

_tagVector3 _tagVector3::operator*(const _tagVector3& v) const
{
	return _tagVector3(x * v.x, y * v.y, z * v.z);
}

_tagVector3 _tagVector3::operator*(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector3(v1 * v);
}

_tagVector3 _tagVector3::operator*(float f) const
{
	return _tagVector3(x * f, y * f, z * f);
}

_tagVector3 _tagVector3::operator*=(const _tagVector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

_tagVector3 _tagVector3::operator*=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 *= v;

	Convert(v1);

	return _tagVector3(v1);
}

_tagVector3 _tagVector3::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

_tagVector3 _tagVector3::operator/(const _tagVector3& v) const
{
	return _tagVector3(x / v.x, y / v.y, z / v.z);
}

_tagVector3 _tagVector3::operator/(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector3(v1/v);
}

_tagVector3 _tagVector3::operator/(float f) const
{
	return _tagVector3(x/f, y/f, z/f);
}

_tagVector3 _tagVector3::operator/=(const _tagVector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

_tagVector3 _tagVector3::operator/=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 /= v;

	Convert(v1);

	return _tagVector3(v1);
}

_tagVector3 _tagVector3::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;

	return *this;
}

float _tagVector3::Length()	const
{
	DirectX::XMVECTOR v1 = Convert();

	return DirectX::XMVectorGetX(DirectX::XMVector3Length(v1));
}

float _tagVector3::Angle(const _tagVector3& v)	const
{
	_tagVector3 v1 = *this;
	_tagVector3 v2 = v;

	v1.Normalize();
	v2.Normalize();

	float fDot = v1.Dot(v2);

	float fAngle = acosf(fDot);

	return RadToDeg(fAngle);
}

float _tagVector3::Dot(const _tagVector3& v)	const
{
	DirectX::XMVECTOR v1 = Convert();
	DirectX::XMVECTOR v2 = v.Convert();

	return DirectX::XMVectorGetX(DirectX::XMVector3Dot(v1, v2));
}

void _tagVector3::Normalize()
{
	DirectX::XMVECTOR v = Convert();

	Convert(DirectX::XMVector3Normalize(v));
}

_tagVector3 _tagVector3::Cross(const _tagVector3& v)	const
{
	DirectX::XMVECTOR v1 = Convert();
	DirectX::XMVECTOR v2 = v.Convert();

	return DirectX::XMVector3Cross(v1, v2);
}

_tagVector3 _tagVector3::TranslationNorm(const _tagMatrix& m)	const
{
	DirectX::XMVECTOR v1 = Convert();

	return DirectX::XMVector3TransformNormal(v1, m.m);
}

_tagVector3 _tagVector3::TranslationCoor(const _tagMatrix& m)	const
{
	DirectX::XMVECTOR v1 = Convert();

	return DirectX::XMVector3TransformCoord(v1, m.m);
}

float _tagVector3::Distance(const _tagVector3& v)	const
{
	_tagVector3 v1 = v - *this;

	return v1.Length();
}

void _tagVector3::Convert(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);
}

DirectX::XMVECTOR _tagVector3::Convert() const
{
	return DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
}

_tagVector3 _tagVector3::Normalize(const _tagVector3& v)	
{
	DirectX::XMVECTOR v1 = v.Convert();

	return _tagVector3(DirectX::XMVector3Normalize(v1));
}

float _tagVector3::Length(const _tagVector3& v)
{
	DirectX::XMVECTOR v1 = v.Convert();

	return DirectX::XMVectorGetX(DirectX::XMVector3Length(v1));
}

float _tagVector3::Distance(const _tagVector3& vSrc, const _tagVector3& vDest)
{
	_tagVector3 v = vSrc - vDest;

	return v.Length();
}
