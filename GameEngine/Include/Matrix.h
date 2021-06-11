#pragma once

#include "Vector4.h"

typedef __declspec(align(16)) union _tagMatrix
{
	DirectX::XMMATRIX	m;

	struct
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};

	Vector4 v[4];

	_tagMatrix();
	_tagMatrix(const _tagMatrix& m);
	_tagMatrix(const DirectX::XMMATRIX& m);
	_tagMatrix(Vector4 v[4]);

	_tagVector4& operator[] (int iIdx);

	_tagMatrix operator=(const _tagMatrix& m);
	_tagMatrix operator=(const DirectX::XMMATRIX& m);
	_tagMatrix operator=(Vector4 v[4]);

	_tagMatrix operator*(const _tagMatrix& m)	const;
	_tagMatrix operator*(const DirectX::XMMATRIX& m)	const;
	_tagMatrix operator*(Vector4 v[4])	const;

	_tagMatrix operator*=(const _tagMatrix& m);
	_tagMatrix operator*=(const DirectX::XMMATRIX& m);
	_tagMatrix operator*=(Vector4 v[4]);

	void Identity();
	void Inverse();
	void Transpose();

	void Scaling(const struct _tagVector3& v);
	void Scaling(float x, float y, float z);
	void Translate(const struct _tagVector3& v);
	void Translate(float x, float y, float z);
	void Rotation(const _tagVector3& v);
	void Rotation(float x, float y, float z);
	void RotationX(float x);
	void RotationY(float y);
	void RotationZ(float z);
	void RotationAxis(const struct _tagVector3& v, float fAngle);

	static _tagMatrix StaticIdentity(const _tagMatrix& m);
	static _tagMatrix StaticInverse(const _tagMatrix& m);
	static _tagMatrix StaticTranspose(const _tagMatrix& m);

	static _tagMatrix StaticScaling(const _tagVector3& v);
	static _tagMatrix StaticScaling(float x, float y, float z);
	static _tagMatrix StaticTranslate(const _tagVector3& v);
	static _tagMatrix StaticTranslate(float x, float y, float z);
	static _tagMatrix StaticRotation(const _tagVector3& v);
	static _tagMatrix StaticRotation(float x, float y, float z);
	static _tagMatrix StaticRotationX(float x);
	static _tagMatrix StaticRotationY(float y);
	static _tagMatrix StaticRotationZ(float z);
	static _tagMatrix StaticRotationAxis(const struct _tagVector3& v, float fAngle);

}Matrix, * PMatrix;

