#include "Vector2.h"

_tagVector2::_tagVector2()	:
	x(0.f),
	y(0.f)
{
}

_tagVector2::_tagVector2(const _tagVector2& v)	:
	x(v.x),
	y(v.y)
{
}

_tagVector2::_tagVector2(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, v);
}

_tagVector2::_tagVector2(float x, float y)	:
	x(x),
	y(y)
{
}

float _tagVector2::operator[](const int& iIdx)	const
{
	switch (iIdx)
	{
	case 0:
		return x;
	case 1:
		return y;
	}

	assert(false);

	return 0.0f;
}

float& _tagVector2::operator[](const int& iIdx)
{
	switch (iIdx)
	{
	case 0:
		return x;
	case 1:
		return y;
	}

	assert(false);

	return y;
}

_tagVector2& _tagVector2::operator=(const _tagVector2& v)
{
	x = v.x;
	y = v.y;

	return *this;
}

_tagVector2& _tagVector2::operator=(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, v);

	return *this;
}

_tagVector2& _tagVector2::operator=(float f)
{
	x = f;
	y = f;

	return *this;
}

bool _tagVector2::operator==(const _tagVector2& v) const
{
	return v.x == x && v.y == y;
}

bool _tagVector2::operator==(const DirectX::XMVECTOR& v) const
{
	_tagVector2 v1(v);

	return v1.x == x && v1.y == y;
}

bool _tagVector2::operator!=(const _tagVector2& v) const
{
	return v.x != x || v.y != y;
}

bool _tagVector2::operator!=(const DirectX::XMVECTOR& v) const
{
	_tagVector2 v1(v);

	return v1.x != x || v1.y != y;
}

_tagVector2 _tagVector2::operator+(const _tagVector2& v) const
{
	return _tagVector2(x + v.x, y + v.y);
}

_tagVector2 _tagVector2::operator+(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector2(v1 + v);
}

_tagVector2 _tagVector2::operator+(float f) const
{
	return _tagVector2(x + f, y + f);
}

_tagVector2 _tagVector2::operator+=(const _tagVector2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

_tagVector2 _tagVector2::operator+=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 += v;

	Convert(v1);

	return *this;
}

_tagVector2 _tagVector2::operator+=(float f)
{
	x += f;

	return *this;
}

_tagVector2 _tagVector2::operator-(const _tagVector2& v) const
{
	return _tagVector2(x - v.x, y - v.y);
}

_tagVector2 _tagVector2::operator-(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector2(v1 - v);
}

_tagVector2 _tagVector2::operator-(float f) const
{
	return _tagVector2(x - f, y - f);
}

_tagVector2 _tagVector2::operator-=(const _tagVector2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

_tagVector2 _tagVector2::operator-=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 -= v;

	Convert(v1);

	return *this;
}

_tagVector2 _tagVector2::operator-=(float f)
{
	x -= f;
	y -= f;

	return *this;
}

_tagVector2 _tagVector2::operator*(const _tagVector2& v) const
{
	return _tagVector2(x * v.x, y * v.y);
}

_tagVector2 _tagVector2::operator*(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector2(v1 * v);
}

_tagVector2 _tagVector2::operator*(float f) const
{
	return _tagVector2(x * f, y * f);
}

_tagVector2 _tagVector2::operator*=(const _tagVector2& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

_tagVector2 _tagVector2::operator*=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 *= v;

	Convert(v1);

	return *this;
}

_tagVector2 _tagVector2::operator*=(float f)
{
	x *= f;
	y *= f;

	return *this;
}

_tagVector2 _tagVector2::operator/(const _tagVector2& v) const
{
	return _tagVector2(x / v.x, y / v.y);
}

_tagVector2 _tagVector2::operator/(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector2(v1 / v);
}

_tagVector2 _tagVector2::operator/(float f) const
{
	return _tagVector2(x / f, y / f);
}

_tagVector2 _tagVector2::operator/=(const _tagVector2& v)
{
	x /= v.x;
	y /= v.y;

	return *this;
}

_tagVector2 _tagVector2::operator/=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 /= v;

	Convert(v1);

	return *this;
}

_tagVector2 _tagVector2::operator/=(float f)
{
	x /= f;
	y /= f;

	return *this;
}

float _tagVector2::Length()
{
	DirectX::XMVECTOR v1 = Convert();

	return DirectX::XMVectorGetX(DirectX::XMVector2Length(v1));
}

float _tagVector2::Angle(const _tagVector2& v)
{
	_tagVector2 v1 = *this;
	_tagVector2 v2 = v;

	float fDot = v1.Dot(v2);

	float fAngle = acosf(fDot);

	return RadToDeg(fDot);
}

float _tagVector2::AngleAxis()
{
	_tagVector2 v1 = *this;
	_tagVector2 v2(1.f, 0.f);

	return v1.Angle(v2);
}

float _tagVector2::Dot(const _tagVector2& v)	const
{
	DirectX::XMVECTOR v1 = Convert();
	DirectX::XMVECTOR v2 = v.Convert();

	return DirectX::XMVectorGetX(DirectX::XMVector2Dot(v1, v2));
}

void _tagVector2::Normalize()
{
	Convert(DirectX::XMVector2Normalize(Convert()));
}

_tagVector2 _tagVector2::Cross(const _tagVector2& v)
{
	DirectX::XMVECTOR v1 = Convert();
	DirectX::XMVECTOR v2 = v.Convert();

	return DirectX::XMVector2Cross(v1, v2);
}

float _tagVector2::Distance(const _tagVector2& v)	const
{
	_tagVector2 v1 = v - *this;

	return v1.Length();
}

void _tagVector2::Convert(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, v);
}

DirectX::XMVECTOR _tagVector2::Convert() const
{
	return DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
}
