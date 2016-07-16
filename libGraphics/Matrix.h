#pragma once
//
//  Matrix.h
//  Matrix
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

namespace grp
{
    class Vector3;

    class Matrix4
    {
    public:
        Matrix4();

        Matrix4& identity();

        Matrix4& translate(float x, float y, float z);

        Matrix4& rotate(float angle, float x, float y, float z);

        Matrix4& scale(float x, float y, float z);

        Matrix4& ortho(float left, float right, float bottom, float top, float zNear, float zFar);

        Matrix4& frustum(float left, float right, float bottom, float top, float zNear, float zFar);

        Matrix4& perspective(float fovy, float aspect, float zNear, float zFar);

        Vector3	transform(const Vector3& vec) const;

        Matrix4	operator*(const Matrix4& mat);

        Matrix4& operator*=(const Matrix4& mat);

    public:
        float _m[16];
    };
}
