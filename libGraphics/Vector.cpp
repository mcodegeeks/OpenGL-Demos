//
//  Vector.cpp
//  Vector
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#include "Vector.h"
#include <cmath>

namespace grp
{
    //
    // Vector2
    Vector2::Vector2() : x(0), y(0)
    {

    }

    bool Vector2::operator==(const Vector2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    //
    // Vector3
    Vector3::Vector3() : x(0), y(0), z(0)
    {

    }

    Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
    {

    }

    float Vector3::length() const
    {
        return std::sqrtf(x*x + y*y + z*z);
    }

    Vector3	Vector3::normalize() const
    {
        float len = length();
        return Vector3(x/len, y/len, z/len);
    }

    float Vector3::dot (const Vector3& vec) const
    {
        return x*vec.x + y*vec.y + z*vec.z;
    }

    Vector3 Vector3::cross(const Vector3& vec) const
    {
        Vector3 out;
        out.x = y * vec.z - z * vec.y;
        out.y = z * vec.x - x * vec.z;
        out.z = x * vec.y - y * vec.x;
        return out;
    }

    Vector3	Vector3::operator+(const Vector3& rhs) const
    {
        return Vector3(x+rhs.x, y+rhs.y, z+rhs.z);
    }

    Vector3	Vector3::operator-(const Vector3& rhs) const
    {
        return Vector3(x-rhs.x, y-rhs.y, z-rhs.z);
    }

    Vector3	Vector3::operator*(float val) const
    {
        return Vector3(x*val, y*val, z*val);
    }

    Vector3	Vector3::operator/(float val) const
    {
        return Vector3(x/val, y/val, z/val);
    }

    Vector3& Vector3::operator+=(const Vector3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vector3& Vector3::operator-=(const Vector3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vector3& Vector3::operator*=(float val)
    {
        x *= val;
        y *= val;
        z *= val;
        return *this;
    }

    Vector3& Vector3::operator/=(float val)
    {
        x /= val;
        y /= val;
        z /= val;
        return *this;
    }

    bool Vector3::operator==(const Vector3& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    //
    // Vector4
    Vector4::Vector4() : x(0), y(0), z(0), w(0)
    {

    }

    bool Vector4::operator==(const Vector4& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
    }
}
