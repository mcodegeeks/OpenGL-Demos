#pragma once
//
//  Camera.h
//  Camera
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#include "Vector.h"
#include "Matrix.h"

namespace grp
{
    class Camera
    {
    public:
        Camera();

        void apply();

        void setViewport(float x, float y, float w, float h);

        void setOrtho(float left, float right, float bottom, float top, float zNear, float zFar);

        void setPerspective(float fovy, float aspect, float zNear, float zFar);

        void setProject(const Matrix4& mat);

        void setLookAt(const Vector3& eye, const Vector3& at, const Vector3& up);

        void walk(float val);

        void fly(float val);

        void strafe(float val);

        void tilt(float val);

        void rotateX(float val);
        void rotateY(float val);
        void rotateZ(float val);

        void rotate(float val, float x, float y, float z);
        void rotate(float val, const Vector3& vec);

    private:
        void updateLook();

    private:
        float _x;
        float _y;
        float _w;
        float _h;

        Matrix4 _proj;
        Matrix4 _look;

        Vector3 _eye;
        Vector3 _at;
        Vector3 _up;
        Vector3	_side;
    };
}
