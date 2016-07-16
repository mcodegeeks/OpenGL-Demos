#pragma once
//
//  Program.h
//  GL Program
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//
namespace grp
{
    class GLProgram
    {
    public:
         GLProgram();
         GLProgram(const char* vertSource, const char* fragSource);
        ~GLProgram();

        bool loadShaders(const char* vertSource, const char* fragSource);

    private:
        unsigned compileShapder(const char* source, unsigned type);

    private:
        unsigned _progId;
    };
}

