//
//  Shaders.cpp
//  Shaders
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//
#include "shaders.h"

namespace grp
{
    const char* vertex_shader =
"                                                   \n\
#ifdef GL_ES                                        \n\
precision mediump int;                              \n\
precision lowp float;                               \n\
#endif                                              \n\
                                                    \n\
varying vec4 v_fragColor;			                \n\
                                                    \n\
void main()								            \n\
{										            \n\
    gl_FragColor = v_fragColor;		                \n\
}													\n\
";

    const char* fragment_shader =
"                                                   \n\
#ifdef GL_ES                                        \n\
precision mediump int;                              \n\
precision mediump float;                            \n\
#endif                                              \n\
                                                    \n\
uniform mat4 mvp_matrix;                            \n\
attribute vec4 a_position;							\n\
                                                    \n\
#ifdef GL_ES                                        \n\
varying lowp vec4 v_fragColor;                      \n\
#else                                               \n\
varying vec4 v_fragColor;                           \n\
#endif                                              \n\
                                                    \n\
uniform	vec4 u_color;								\n\
uniform float u_pointSize;							\n\
                                                    \n\
void main()											\n\
{													\n\
    gl_Position = mvp_matrix * a_position;          \n\
    gl_PointSize = u_pointSize;						\n\
    v_fragColor = u_color;                          \n\
}													\n\
";
}
