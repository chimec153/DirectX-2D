#pragma once

#include "Matrix.h"

typedef __declspec(align(16)) struct _tagVector3
{
	float x, y, z;

	_tagVector3();
	_tagVector3(const _tagVector3& v);
	_tagVector3(const DirectX::XMVECTOR& v);
	_tagVector3(float x, float y, float z);

	float operator[] (const int& iIdx)	const;
	float& operator[] (const int& iIdx);

	// =
	_tagVector3& operator= (const _tagVector3& v);
	_tagVector3& operator= (const DirectX::XMVECTOR& v);
	_tagVector3& operator= (float f);

	// ==
	bool operator== (const _tagVector3& v)	const;
	bool operator== (const DirectX::XMVECTOR& v)	const;
	bool operator!= (const _tagVector3& v)	const;
	bool operator!= (const DirectX::XMVECTOR& v)	const;

	// +
	_tagVector3 operator+ (const _tagVector3& v)	const;
	_tagVector3 operator+ (const DirectX::XMVECTOR& v)	const;
	_tagVector3 operator+ (float f)	const;

	// +=
	_tagVector3 operator+= (const _tagVector3& v);
	_tagVector3 operator+= (const DirectX::XMVECTOR& v);
	_tagVector3 operator+= (float f);

	// -
	_tagVector3 operator- (const _tagVector3& v)	const;
	_tagVector3 operator- (const DirectX::XMVECTOR& v)	const;
	_tagVector3 operator- (float f)	const;

	// -=
	_tagVector3 operator-= (const _tagVector3& v);
	_tagVector3 operator-= (const DirectX::XMVECTOR& v);
	_tagVector3 operator-= (float f);

	// *
	_tagVector3 operator* (const _tagVector3& v)	const;
	_tagVector3 operator* (const DirectX::XMVECTOR& v)	const;
	_tagVector3 operator* (float f)	const;

	// *=
	_tagVector3 operator*= (const _tagVector3& v);
	_tagVector3 operator*= (const DirectX::XMVECTOR& v);
	_tagVector3 operator*= (float f);

	// /
	_tagVector3 operator/ (const _tagVector3& v)	const;
	_tagVector3 operator/ (const DirectX::XMVECTOR& v)	const;
	_tagVector3 operator/ (float f)	const;

	// /=
	_tagVector3 operator/= (const _tagVector3& v);
	_tagVector3 operator/= (const DirectX::XMVECTOR& v);
	_tagVector3 operator/= (float f);

	float Length()	const;
	float Angle(const _tagVector3& v)	const;
	float Dot(const _tagVector3& v)	const;
	void Normalize();
	_tagVector3 Cross(const _tagVector3& v)	const;
	_tagVector3 TranslationNorm(const _tagMatrix& m)	const;
	_tagVector3 TranslationCoor(const _tagMatrix& m)	const;
	float Distance(const _tagVector3& v)	const;

	void Convert(const DirectX::XMVECTOR& v);
	DirectX::XMVECTOR Convert()	const;

	static float Length(const _tagVector3& v);
	static _tagVector3 Normalize(const _tagVector3& v);
	static float Distance(const _tagVector3& vSrc, const _tagVector3& vDest);

	static _tagVector3 One;
	static _tagVector3 Zero;
	static _tagVector3 Axis[AXIS_END];
}Vector3, *PVector3;

