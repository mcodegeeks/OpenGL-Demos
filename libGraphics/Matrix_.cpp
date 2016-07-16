//
//  Matrix.cpp
//  Matrix
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#include "Matrix.h"
#include "Vector.h"
#include "Macro.h"
#include <cmath>

namespace grp
{
    Matrix4::Matrix4()
    {
        identity();
    }

    Matrix4& Matrix4::identity()
    {
        _m[ 0] = 1;	_m[ 1] = 0;	_m[ 2] = 0;	_m[ 3] = 0;
        _m[ 4] = 0;	_m[ 5] = 1;	_m[ 6] = 0;	_m[ 7] = 0;
        _m[ 8] = 0;	_m[ 9] = 0;	_m[10] = 1;	_m[11] = 0;
        _m[12] = 0;	_m[13] = 0;	_m[14] = 0;	_m[15] = 1;

        return *this;
    }

    Matrix4& Matrix4::translate(float x, float y, float z)
    {
        Matrix4	mat;

        mat._m[12] = x;
        mat._m[13] = y;
        mat._m[14] = z;

        return *this = mat * *this;
    }

    Matrix4& Matrix4::rotate(float angle, float x, float y, float z)
    {
        Matrix4	mat;

        float s  = std::sinf (DEG2RAD(angle));
        float c  = std::cosf (DEG2RAD(angle));
        float l  = std::sqrtf (x * x + y * y + z * z);
        float ux = x / l;
        float uy = y / l;
        float uz = z / l;
        float c1 = 1 - c;

        mat._m[ 0] = ux * ux * c1 + c;
        mat._m[ 1] = uy * ux * c1 + uz * s;
        mat._m[ 2] = ux * uz * c1 - uy * s;

        mat._m[ 4] = ux * uy * c1 - uz * s;
        mat._m[ 5] = uy * uy * c1 + c;
        mat._m[ 6] = uy * uz * c1 + ux * s;

        mat._m[ 8] = ux * uz * c1 + uy * s;
        mat._m[ 9] = uy * uz * c1 - ux * s;
        mat._m[10] = uz * uz * c1 + c;

        return *this = mat * *this;
    }

    Matrix4& Matrix4::scale(float x, float y, float z)
    {
        Matrix4	mat;

        mat._m[ 0] = x;
        mat._m[ 5] = y;
        mat._m[10] = z;

        return *this = mat * *this;
    }

    Matrix4& Matrix4::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        _m[ 0] = 2.f / (right - left);
        _m[ 1] = 0;
        _m[ 2] = 0;
        _m[ 3] = 0;

        _m[ 4] = 0;
        _m[ 5] = 2.f / (top - bottom);
        _m[ 6] = 0;
        _m[ 7] = 0;

        _m[ 8] = 0;
        _m[ 9] = 0;
        _m[10] = -2.f / (zFar - zNear);
        _m[11] = 0;

        _m[12] = -(right + left) / (right - left);
        _m[13] = -(top + bottom) / (top - bottom);
        _m[14] = -(zFar + zNear) / (zFar - zNear);
        _m[15] = 1.f;

        return *this;
    }

    Matrix4& Matrix4::frustum(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        _m[ 0] = 2.f * zNear / (right - left);
        _m[ 1] = 0;
        _m[ 2] = 0;
        _m[ 3] = 0;

        _m[ 4] = 0;
        _m[ 5] = 2.f * zNear / (top - bottom);
        _m[ 6] = 0;
        _m[ 7] = 0;

        _m[ 8] = (right + left) / (right - left);
        _m[ 9] = (top + bottom) / (top - bottom);
        _m[10] = (-zFar - zNear) / (zFar - zNear);
        _m[11] = -1.f;

        _m[12] = 0;
        _m[13] = 0;
        _m[14] = ( -(2.f * zNear) * zFar) / (zFar - zNear);

        return *this;
    }

    Matrix4& Matrix4::perspective(float fovy, float aspect, float zNear, float zFar)
    {
        float h  = 2.0f * zNear * std::tanf(DEG2RAD(fovy) * 0.5f);
        float w  = aspect * h;
        float h2 = h * 0.5f;
        float w2 = w * 0.5f;

        return frustum(-w2, w2, -h2, h2, zNear, zFar);
    }

    Vector3	Matrix4::transform(const Vector3& vec) const
    {
        Vector3	out;
        out.x = vec.x * _m[0] + vec.y * _m[4] + vec.z * _m[8];
        out.y = vec.x * _m[1] + vec.y * _m[5] + vec.z * _m[9];
        out.z = vec.x * _m[2] + vec.y * _m[6] + vec.z * _m[10];
        return out;
    }

    Matrix4	Matrix4::operator*(const Matrix4& mat)
    {
        Matrix4 out;

        out._m[ 0] = _m[ 0] * mat._m[ 0] + _m[ 1] * mat._m[ 4] + _m[ 2] * mat._m[ 8] + _m[ 3] * mat._m[12];
        out._m[ 1] = _m[ 0] * mat._m[ 1] + _m[ 1] * mat._m[ 5] + _m[ 2] * mat._m[ 9] + _m[ 3] * mat._m[13];
        out._m[ 2] = _m[ 0] * mat._m[ 2] + _m[ 1] * mat._m[ 6] + _m[ 2] * mat._m[10] + _m[ 3] * mat._m[14];
        out._m[ 3] = _m[ 0] * mat._m[ 3] + _m[ 1] * mat._m[ 7] + _m[ 2] * mat._m[11] + _m[ 3] * mat._m[15];

        out._m[ 4] = _m[ 4] * mat._m[ 0] + _m[ 5] * mat._m[ 4] + _m[ 6] * mat._m[ 8] + _m[ 7] * mat._m[12];
        out._m[ 5] = _m[ 4] * mat._m[ 1] + _m[ 5] * mat._m[ 5] + _m[ 6] * mat._m[ 9] + _m[ 7] * mat._m[13];
        out._m[ 6] = _m[ 4] * mat._m[ 2] + _m[ 5] * mat._m[ 6] + _m[ 6] * mat._m[10] + _m[ 7] * mat._m[14];
        out._m[ 7] = _m[ 4] * mat._m[ 3] + _m[ 5] * mat._m[ 7] + _m[ 6] * mat._m[11] + _m[ 7] * mat._m[15];

        out._m[ 8] = _m[ 8] * mat._m[ 0] + _m[ 9] * mat._m[ 4] + _m[10] * mat._m[ 8] + _m[11] * mat._m[12];
        out._m[ 9] = _m[ 8] * mat._m[ 1] + _m[ 9] * mat._m[ 5] + _m[10] * mat._m[ 9] + _m[11] * mat._m[13];
        out._m[10] = _m[ 8] * mat._m[ 2] + _m[ 9] * mat._m[ 6] + _m[10] * mat._m[10] + _m[11] * mat._m[14];
        out._m[11] = _m[ 8] * mat._m[ 3] + _m[ 9] * mat._m[ 7] + _m[10] * mat._m[11] + _m[11] * mat._m[15];

        out._m[12] = _m[12] * mat._m[ 0] + _m[13] * mat._m[ 4] + _m[14] * mat._m[ 8] + _m[15] * mat._m[12];
        out._m[13] = _m[12] * mat._m[ 1] + _m[13] * mat._m[ 5] + _m[14] * mat._m[ 9] + _m[15] * mat._m[13];
        out._m[14] = _m[12] * mat._m[ 2] + _m[13] * mat._m[ 6] + _m[14] * mat._m[10] + _m[15] * mat._m[14];
        out._m[15] = _m[12] * mat._m[ 3] + _m[13] * mat._m[ 7] + _m[14] * mat._m[11] + _m[15] * mat._m[15];

        return out;
    }

    Matrix4& Matrix4::operator*=(const Matrix4& mat)
    {
        return *this = *this * mat;
    }
}

