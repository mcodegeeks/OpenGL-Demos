//
//  Program.cpp
//  GL Program
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#include "Program.h"
#include <iostream>

#if defined(TARGET_QT)
#include <QOpenGLFunctions>
#endif

namespace grp
{
    GLProgram::GLProgram()
        : _progId(0)
    {

    }

    GLProgram::GLProgram(const char* vertSource, const char* fragSource)
        : _progId(0)
    {
        loadShaders(vertSource, fragSource);
    }

    GLProgram::~GLProgram()
    {
        if (_progId)
            glDeleteProgram(_progId);
    }

    bool GLProgram::loadShaders(const char* vertSource, const char* fragSource)
    {
        GLuint vertShader = compileShapder(vertSource, GL_VERTEX_SHADER);
        if (!vertShader)
            return false;

        GLuint fragShader = compileShapder(fragSource, GL_FRAGMENT_SHADER);
        if (!fragShader) {
            glDeleteShader(vertShader);
            return false;
        }

        _progId = glCreateProgram();
        glAttachShader(_progId, vertShader);
        glAttachShader(_progId, fragShader);
        glLinkProgram(_progId);

        GLint status;
        glGetProgramiv(_progId, GL_LINK_STATUS, &status);
        if (!status)
        {
            GLsizei length;
            glGetProgramiv(_progId, GL_INFO_LOG_LENGTH, &length);
            if (length > 0)
            {
                std::vector<char> message(length+1);
                glGetProgramInfoLog(_progId, length, 0, &message[0]);
                std::cout << &message[0] << std::endl;
            }
        }

        glDetachShader(_progId, vertShader);
        glDetachShader(_progId, fragShader);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return status != 0;
    }

    unsigned GLProgram::compileShapder(const char* source, unsigned type)
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);

        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (!status)
        {
            GLsizei length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            if (length > 0)
            {
                std::vector<char> message(length+1);
                glGetShaderInfoLog(shader, length, 0, &message[0]);
                std::cout << &message[0] << std::endl;
            }

            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }
}


