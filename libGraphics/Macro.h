#pragma once
//
//  Macro.h
//  macro
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#define RAD2DEG(X) ((X) * 57.2957795129f)
#define DEG2RAD(X) ((X) * 0.01745329252f)

#define SHAPE_ALL 0xffffffff
#define FACET_ALL 0xffffffff
#define	HOLE_NULL 0

#ifdef _WIN32
   //define something for Windows (32-bit and 64-bit, this part is common)
   #define
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
    #define GL_ES
    #elif TARGET_OS_IPHONE
    #define GL_ES
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __linux__
    // linux
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#elif __ANDROID__
#define GL_ES
#else
#   error "Unknown compiler"
#endif


