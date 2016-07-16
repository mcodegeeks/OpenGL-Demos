#pragma once
//
//  Enum.h
//  Enum
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

namespace grp
{
    typedef enum
    {
        GEO_REGULAR               = 0x0000,
        GEO_SOLID                 = 0x0001,
        GEO_PILLAR                = 0x0002,
        GEO_WALL                  = 0x0003,
        GEO_BSOLID                = 0x0004,
        GEO_BPILLAR               = 0x0005,
        GEO_BWALL                 = 0x0006,
        GEO_CONE                  = 0x0007,
        GEO_CWALL                 = 0x0008
    } Geometry;

    typedef enum
    {
        DISP_POINT                = 0x0001,
        DISP_LINE                 = 0x0002,
        DISP_FILL                 = 0x0003,
        DISP_TEXTURE              = 0x0004
    } DispMode;

    typedef	enum
    {
        DISP_LINES                = 0x0001,
        DISP_LINE_LOOP            = 0x0002,
        DISP_LINE_STRIP           = 0x0003
    } DispLine;

    typedef enum
    {
        DISP_TRIANGLES            = 0x0004,
        DISP_TRIANGLE_STRIP       = 0x0005,
        DISP_TRIANGLE_FAN         = 0x0006
    } DispTri;

    typedef enum
    {
        CULL_DISABLE              = 0x0000,
        CULL_FRONT                = 0x0001,
        CULL_BACK                 = 0x0002
    } Cull;

    typedef enum
    {
        WIND_CLOCK                = 0x0900,
        WIND_ANTI_CLOCK           = 0x0901
    } Wind;

    typedef enum
    {
        BLEND_ZERO                = 0x0000,
        BLEND_ONE                 = 0x0001,
        BLEND_SRC_COLOR           = 0x0300,
        BLEND_ONE_MINUS_SRC_COLOR = 0x0301,
        BLEND_SRC_ALPHA           = 0x0302,
        BLEND_ONE_MINUS_SRC_ALPHA = 0x0303,
        BLEND_DST_ALPHA           = 0x0304,
        BLEND_ONE_MINUS_DST_ALPHA = 0x0305,
        BLEND_DST_COLOR           = 0x0306,
        BLEND_ONE_MINUS_DST_COLOR = 0x0307,
        BLEND_SRC_ALPHA_SATURATE  = 0x0308
    } Blend;

    typedef enum
    {
        TEX_UNIT_0                = 0x0000,
        TEX_UNIT_1                = 0x0001
    } TexUnit;

    typedef enum
    {
        TEX_NULL                  = 0x0000,
        TEX_REPLACE               = 0x1E01,
        TEX_MODULATE              = 0x2100,
    } TexEnv;

    typedef enum
    {
        COMBINE_NULL              = 0x0000,
        COMBINE_MODULATE          = 0x2100,
        COMBINE_ADD               = 0x0104,
        COMBINE_ADD_SIGNED        = 0x8574,
        COMBINE_REPLACE           = 0x1E01,
        COMBINE_SUBTRACT          = 0x84E7
    } Combine;

    typedef enum
    {
        LIMIT_NONE                = 0x0000,
        LIMIT_LINE                = 0x0001,
        LIMIT_TRI                 = 0x0002
    } DrawLimit;

    typedef enum
    {
        ANI_POSITION              = 0x0100,
        ANI_AXIS                  = 0x0200,
        ANI_ROTATE                = 0x0400,
        ANI_SCALE                 = 0x0800,
        ANI_ALPHA                 = 0x1000,
        ANI_RGB                   = 0x2000
    } AniMask;

    typedef enum
    {
        TESS_VERTEX               = 0x0000,
        TESS_COLOR                = 0x0001,
        TESS_COORD                = 0x0002
    } TessMode;

    typedef enum
    {
        ARRAY_BUFFER              = 0x0000,
        ELEMENT_ARRAY_BUFFER      = 0x0001,
    } BufferType;
}
