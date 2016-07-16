//
//  Color.cpp
//  Color
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#include "Color.h"

namespace grp
{
    Color::Color()
        : r(0), g(0), b(0), a(0)
    {

    }

    Color::Color(float r, float g, float b, float a)
        : r(r), g(g), b(b), a(a)
    {

    }

    void Color::clear()
    {
        r = 0;
        g = 0;
        b = 0;
        a = 0;
    }

    bool Color::operator==(const Color& rhs) const
    {
        return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
    }

    bool Color::operator!=(const Color& rhs) const
    {
        return r != rhs.r || g != rhs.g || b != rhs.b || a != rhs.a;
    }
}
