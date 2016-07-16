#pragma once
//
//  Vector.h
//  Vector
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

namespace grp
{
    class Vector2
    {
    public:
        Vector2();

        bool operator==(const Vector2& rhs) const;

    public:
        float x;
        float y;
    };

    class Vector3
    {
    public:
        Vector3();
        Vector3(float x, float y, float z);

        float length() const;

        Vector3	normalize() const;

        float dot(const Vector3& vec) const;

        Vector3 cross(const Vector3& vec) const;

        Vector3 operator+(const Vector3& rhs) const;
        Vector3 operator-(const Vector3& rhs) const;
        Vector3 operator*(float val) const;
        Vector3 operator/(float val) const;

        Vector3& operator+=(const Vector3& rhs);
        Vector3& operator-=(const Vector3& rhs);
        Vector3& operator*=(float val);
        Vector3& operator/=(float val);

        bool operator==(const Vector3& rhs) const;

    public:
        float x;
        float y;
        float z;
    };

    class Vector4
    {
    public:
        Vector4();

        bool operator==(const Vector4& rhs) const;

    public:
        float x;
        float y;
        float z;
        float w;
    };
}
