//
//  Camera.cpp
//  Camera
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#include "Camera.h"
#include "Canvas.h"
#include "libKazmath/GL/matrix.h"

#if defined(TARGET_QT)
#include <QOpenGLFunctions>
#endif

namespace grp
{
    Camera::Camera()
    {

    }

    void Camera::apply()
    {
        Canvas::getInstance()->viewport(_x, _y, _w, _h);

        kmGLMatrixMode(KM_GL_PROJECTION);
        kmGLLoadMatrix((const kmMat4*) &_proj);

        kmGLMatrixMode(KM_GL_MODELVIEW);
        kmGLLoadMatrix((const kmMat4*) &_look);
    }

    void Camera::setViewport(float x, float y, float w, float h)
    {
        _x = x;
        _y = y;
        _w = w;
        _h = h;
    }

    void Camera::setOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        _proj.ortho(left, right, bottom, top, zNear, zFar);
    }

    void Camera::setPerspective(float fovy, float aspect, float zNear, float zFar)
    {
        _proj.perspective(fovy, aspect, zNear, zFar);
    }

    void Camera::setProject(const Matrix4& mat)
    {
        _proj = mat;
    }

    void Camera::setLookAt( const Vector3& eye, const Vector3& at, const Vector3& up)
    {
        _eye = eye;
        _at = at;
        _up = up;
    }

    void Camera::walk(float val)
    {
        _eye += _at*val;
        updateLook();
    }

    void Camera::fly(float val)
    {
        _eye.y += val;
        updateLook();
    }

    void Camera::strafe(float val)
    {
        _eye += _side*val;
        updateLook();
    }

    void Camera::tilt(float val)
    {
        Matrix4 mat;
        mat.rotate(val, _side.x, _side.y, _side.z);
        _at = mat.transform(_at);
        _up = mat.transform(_up);
        updateLook();
    }

    void Camera::rotateX(float val)
    {
        rotate(val, Vector3(1, 0, 0));
    }

    void Camera::rotateY(float val)
    {
        rotate(val, Vector3(0, 1, 0));
    }

    void Camera::rotateZ(float val)
    {
        rotate(val, Vector3(0, 0, 1));
    }

    void Camera::rotate(float val, float x, float y, float z)
    {
        rotate(val, Vector3(x, y, z));
    }

    void Camera::rotate(float val, const Vector3& vec)
    {
        Matrix4 mat;
        mat.rotate(val, vec.x, vec.y, vec.z);
        _at = mat.transform(_at);
        _up = mat.transform(_up);
        updateLook();
    }

    void Camera::updateLook()
    {
        _at = _at.normalize();

        _side = _at.cross(_up);
        _side = _side.normalize();

        _up = _side.cross(_at);
        _up = _up.normalize();

        _look._m[ 0] = _side.x;	_look._m[ 4] = _side.y;	_look._m[ 8] = _side.z;	_look._m[12] = -_side.dot(_eye);
        _look._m[ 1] =   _up.x;	_look._m[ 5] =   _up.y;	_look._m[ 9] =   _up.z;	_look._m[13] =   -_up.dot(_eye);
        _look._m[ 2] =  -_at.x;	_look._m[ 6] =  -_at.y;	_look._m[10] =  -_at.z;	_look._m[14] =    _at.dot(_eye);
        _look._m[ 3] =       0;	_look._m[ 7] =       0;	_look._m[11] =       0;	_look._m[15] =             1.0f;
    }
}
