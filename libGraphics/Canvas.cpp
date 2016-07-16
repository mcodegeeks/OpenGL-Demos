//
//  Canvas.cpp
//  Canvas
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#include "Canvas.h"
#include "Shaders.h"
#include "libKazmath/GL/matrix.h"

#if defined(TARGET_QT)
#include <QOpenGLFunctions>
#endif

namespace grp
{
    Canvas* Canvas::getInstance()
    {
        static Canvas instance;
        return &instance;
    }

    Canvas::Canvas()
        : _program(nullptr)
        , _scale(1)
        , _width(0)
        , _height(0)
    {
        //_program = new GLProgram(color_vert, color_frag);

        glClearColor(0.0f, 0.0f, 0.25f, 1.0f);

        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glEnable(GL_CULL_FACE);
        glDepthFunc(GL_LEQUAL);
        //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    Canvas::~Canvas()
    {
        delete _program;
    }

    void Canvas::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Canvas::flush()
    {
        glFlush();
    }

    void Canvas::clearColor(float r, float g, float b, float a)
    {
        glClearColor(r , g, b, a);
    }

    void Canvas::loadIdentity()
    {
        kmGLMatrixMode(KM_GL_MODELVIEW);
        kmGLLoadIdentity();

        kmGLTranslatef(0, 0, -10);
        kmGLScalef(_scale, _scale, _scale);
        kmGLTranslatef(-_width/2, -_height/2, 0);
    }

    void Canvas::pushMatrix()
    {
        kmGLPushMatrix();
    }

    void Canvas::popMatrix()
    {
        kmGLPopMatrix();
    }

    void Canvas::mulMatrix(const Matrix4& mat)
    {
        kmGLMultMatrix((const kmMat4*) &mat);
    }

    void Canvas::translate(const Vector3& vec)
    {
        kmGLTranslatef(vec.x, vec.y, vec.z);
    }

    void Canvas::translate(float x, float y, float z)
    {
        kmGLTranslatef(x, y, z);
    }

    void Canvas::rotate(float angle, const Vector3& vec)
    {
        kmGLRotatef(angle, vec.x, vec.y, vec.z);
    }

    void Canvas::rotate(float angle, float x, float y, float z)
    {
        kmGLRotatef(angle, x, y, z);
    }

    void Canvas::scale(const Vector3& vec)
    {
        kmGLScalef(vec.x, vec.y, vec.z);
    }

    void Canvas::scale(float x, float y, float z)
    {
        kmGLScalef(x, y, z);
    }

    void Canvas::viewport(float x, float y, float w, float h)
    {
        glEnable(GL_SCISSOR_TEST);
        glViewport(x, y, w, h);
        glScissor(x, y, w, h);
    }

    void Canvas::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        kmGLMatrixMode(KM_GL_PROJECTION);

        kmMat4 mat;
        kmMat4OrthographicProjection(&mat, left, right, bottom, top, zNear, zFar);
        kmGLLoadMatrix(&mat);

        kmGLMatrixMode(KM_GL_MODELVIEW);
        kmGLLoadIdentity();
    }

    void Canvas::perspective(float fovy, float aspect, float zNear, float zFar)
    {
        kmGLMatrixMode(KM_GL_PROJECTION);

        kmMat4 mat;
        kmMat4PerspectiveProjection(&mat, fovy, aspect, zNear, zFar);
        kmGLLoadMatrix(&mat);

        kmGLMatrixMode(KM_GL_MODELVIEW);
        kmGLLoadIdentity();
    }

    void Canvas::perspective(unsigned w, unsigned h)
    {
        kmGLMatrixMode(KM_GL_PROJECTION);

        Matrix4 mat;
        mat.perspective(45, w/h, 1, 100);
        kmGLLoadMatrix((const kmMat4*) &mat);

        kmGLMatrixMode(KM_GL_MODELVIEW);
        kmGLLoadIdentity();

        mat.translate(0, 0, -10);
        _scale = (2*mat._m[15])  / (mat._m[0]*w);
        _width = w;
        _height = h;
    }   
}
