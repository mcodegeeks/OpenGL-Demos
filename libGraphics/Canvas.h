#pragma once
//
//  Canvas.h
//  Canvas
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#include "Matrix.h"
#include "Vector.h"
#include "Program.h"

namespace grp
{
    class Canvas
    {
    public:
        static Canvas* getInstance();

        void clear();

        void flush();

        void clearColor(float r, float g, float b, float a);

        void loadIdentity();

        void pushMatrix();
        void popMatrix();

        void mulMatrix(const Matrix4& mat);

        void translate(const Vector3& vec);
        void translate(float x, float y, float z);

        void rotate(float angle, const Vector3& vec);
        void rotate(float angle, float x, float y, float z);

        void scale(const Vector3& vec);
        void scale(float x, float y, float z);

        void viewport(float x, float y, float width, float height);

        void ortho(float left, float right, float bottom, float top, float zNear, float zFar);

        void perspective(float fovy, float aspect, float zNear, float zFar);
        void perspective(unsigned width, unsigned height);

    private:
         Canvas();
        ~Canvas();        

        GLProgram* _program;

        float _scale;
        float _width;
        float _height;
    };
}
