#pragma once

#include "Matrix.h"

typedef __declspec(align(16)) struct _tagVector2
{
	float x, y;

	_tagVector2();
	_tagVector2(const _tagVector2& v);
	_tagVector2(const DirectX::XMVECTOR& v);
	_tagVector2(float x, float y);

	float operator[] (const int& iIdx)	const;
	float& operator[] (const int& iIdx);

	// =
	_tagVector2& operator= (const _tagVector2& v);
	_tagVector2& operator= (const DirectX::XMVECTOR& v);
	_tagVector2& operator= (float f);

	// ==
	bool operator== (const _tagVector2& v)	const;
	bool operator== (const DirectX::XMVECTOR& v)	const;
	bool operator!= (const _tagVector2& v)	const;
	bool operator!= (const DirectX::XMVECTOR& v)	const;

	// +
	_tagVector2 operator+ (const _tagVector2& v)	const;
	_tagVector2 operator+ (const DirectX::XMVECTOR& v)	const;
	_tagVector2 operator+ (float f)	const;

	// +=
	_tagVector2 operator+= (const _tagVector2& v);
	_tagVector2 operator+= (const DirectX::XMVECTOR& v);
	_tagVector2 operator+= (float f);

	// -
	_tagVector2 operator- (const _tagVector2& v)	const;
	_tagVector2 operator- (const DirectX::XMVECTOR& v)	const;
	_tagVector2 operator- (float f)	const;

	// -=
	_tagVector2 operator-= (const _tagVector2& v);
	_tagVector2 operator-= (const DirectX::XMVECTOR& v);
	_tagVector2 operator-= (float f);

	// *
	_tagVector2 operator* (const _tagVector2& v)	const;
	_tagVector2 operator* (const DirectX::XMVECTOR& v)	const;
	_tagVector2 operator* (float f)	const;

	// *=
	_tagVector2 operator*= (const _tagVector2& v);
	_tagVector2 operator*= (const DirectX::XMVECTOR& v);
	_tagVector2 operator*= (float f);

	// /
	_tagVector2 operator/ (const _tagVector2& v)	const;
	_tagVector2 operator/ (const DirectX::XMVECTOR& v)	const;
	_tagVector2 operator/ (float f)	const;

	// /=
	_tagVector2 operator/= (const _tagVector2& v);
	_tagVector2 operator/= (const DirectX::XMVECTOR& v);
	_tagVector2 operator/= (float f);

	float Length();
	float Angle(const _tagVector2& v);
	float AngleAxis();
	float Dot(const _tagVector2& v)	const;
	void Normalize();
	_tagVector2 Cross(const _tagVector2& v);
	float Distance(const _tagVector2& v)	const;

	void Convert(const DirectX::XMVECTOR& v);
	DirectX::XMVECTOR Convert()	const;
}Vector2, *PVector2;

