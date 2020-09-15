#pragma once

#include "EngineMath.h"

typedef __declspec(align(16)) struct _tagVector4
{
	float x, y, z, w;

	_tagVector4();
	_tagVector4(const _tagVector4& v);
	_tagVector4(const DirectX::XMVECTOR& v);
	_tagVector4(float x, float y, float z, float w);

	float operator[] (const int& iIdx)	const;
	float& operator[] (const int& iIdx);

	// =
	_tagVector4& operator= (const _tagVector4& v);
	_tagVector4& operator= (const DirectX::XMVECTOR& v);
	_tagVector4& operator= (float f);

	// ==
	bool operator== (const _tagVector4& v)	const;
	bool operator== (const DirectX::XMVECTOR& v)	const;
	bool operator!= (const _tagVector4& v)	const;
	bool operator!= (const DirectX::XMVECTOR& v)	const;

	// +
	_tagVector4 operator+ (const _tagVector4& v)	const;
	_tagVector4 operator+ (const DirectX::XMVECTOR& v)	const;
	_tagVector4 operator+ (float f)	const;

	// +=
	_tagVector4 operator+= (const _tagVector4& v);
	_tagVector4 operator+= (const DirectX::XMVECTOR& v);
	_tagVector4 operator+= (float f);

	// -
	_tagVector4 operator- (const _tagVector4& v)	const;
	_tagVector4 operator- (const DirectX::XMVECTOR& v)	const;
	_tagVector4 operator- (float f)	const;

	// -=
	_tagVector4 operator-= (const _tagVector4& v);
	_tagVector4 operator-= (const DirectX::XMVECTOR& v);
	_tagVector4 operator-= (float f);

	// *
	_tagVector4 operator* (const _tagVector4& v)	const;
	_tagVector4 operator* (const DirectX::XMVECTOR& v)	const;
	_tagVector4 operator* (float f)	const;

	// *=
	_tagVector4 operator*= (const _tagVector4& v);
	_tagVector4 operator*= (const DirectX::XMVECTOR& v);
	_tagVector4 operator*= (float f);

	// /
	_tagVector4 operator/ (const _tagVector4& v)	const;
	_tagVector4 operator/ (const DirectX::XMVECTOR& v)	const;
	_tagVector4 operator/ (float f)	const;

	// /=
	_tagVector4 operator/= (const _tagVector4& v);
	_tagVector4 operator/= (const DirectX::XMVECTOR& v);
	_tagVector4 operator/= (float f);

	void Convert(const DirectX::XMVECTOR& v);
	DirectX::XMVECTOR Convert()	const;

	static _tagVector4 Black;
	static _tagVector4 White;
	static _tagVector4 Green;
	static _tagVector4 Blue;
	static _tagVector4 Red;

}Vector4, *PVector4;
